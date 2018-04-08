/**
 * <h1>DeclaredRoutineParser</h1>
 *
 * <p>Parse a main program routine or a declared procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include "DeclaredRoutineParser.h"
#include "DeclarationsParser.h"
#include "VariableDeclarationsParser.h"
#include "BlockParser.h"
#include "../../../frontend/pascal/PascalToken.h"
#include "../../../frontend/pascal/PascalError.h"
#include "../../../intermediate/SymTabEntry.h"
#include "../../../intermediate/ICodeFactory.h"
#include "../../../intermediate/symtabimpl/SymTabEntryImpl.h"
#include "../../../intermediate/symtabimpl/Predefined.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"
#include "../../../intermediate/typeimpl/TypeSpecImpl.h"
#include "../../../Object.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::intermediate::icodeimpl;
using namespace wci::intermediate::typeimpl;

bool DeclaredRoutineParser::INITIALIZED = false;

set<PascalTokenType> DeclaredRoutineParser::PARAMETER_SET;
set<PascalTokenType> DeclaredRoutineParser::LEFT_PAREN_SET;
set<PascalTokenType> DeclaredRoutineParser::RIGHT_PAREN_SET;
set<PascalTokenType> DeclaredRoutineParser::PARAMETER_FOLLOW_SET;
set<PascalTokenType> DeclaredRoutineParser::COMMA_SET;

void DeclaredRoutineParser::initialize()
{
    if (INITIALIZED) return;
    DeclarationsParser::initialize();

    PARAMETER_SET = DeclarationsParser::DECLARATION_START_SET;
    PARAMETER_SET.insert(PascalTokenType::VAR);
    PARAMETER_SET.insert(PascalTokenType::IDENTIFIER);
    PARAMETER_SET.insert(PascalTokenType::RIGHT_PAREN);

    LEFT_PAREN_SET = DeclarationsParser::DECLARATION_START_SET;
    LEFT_PAREN_SET.insert(PascalTokenType::LEFT_PAREN);
    LEFT_PAREN_SET.insert(PascalTokenType::SEMICOLON);
    LEFT_PAREN_SET.insert(PascalTokenType::COLON);

    RIGHT_PAREN_SET = LEFT_PAREN_SET;
    RIGHT_PAREN_SET.insert(PascalTokenType::RIGHT_PAREN);
    RIGHT_PAREN_SET.erase(PascalTokenType::LEFT_PAREN);

    PARAMETER_FOLLOW_SET = DeclarationsParser::DECLARATION_START_SET;
    PARAMETER_FOLLOW_SET.insert(PascalTokenType::COLON);
    PARAMETER_FOLLOW_SET.insert(PascalTokenType::RIGHT_PAREN);
    PARAMETER_FOLLOW_SET.insert(PascalTokenType::SEMICOLON);

    COMMA_SET = DeclarationsParser::DECLARATION_START_SET;
    COMMA_SET.insert(PascalTokenType::COMMA);
    COMMA_SET.insert(PascalTokenType::COLON);
    COMMA_SET.insert(PascalTokenType::IDENTIFIER);
    COMMA_SET.insert(PascalTokenType::RIGHT_PAREN);
    COMMA_SET.insert(PascalTokenType::SEMICOLON);

    INITIALIZED = true;
}

int DeclaredRoutineParser::dummy_counter = 0;

DeclaredRoutineParser::DeclaredRoutineParser(PascalParserTD *parent)
    : DeclarationsParser(parent)
{
    initialize();
}

SymTabEntry *DeclaredRoutineParser::parse_declaration(
                                    Token *token, SymTabEntry *parent_id)
    throw (string)
{
    Definition defn;
    string dummy_name;
    SymTabEntry *routine_id = nullptr;
    TokenType routine_type = token->get_type();

    // Initialize.
    switch ((PascalTokenType) routine_type)
    {
        case PT_PROGRAM:
        {
            token = next_token(token);  // consume PROGRAM
            defn = (Definition) DF_PROGRAM;
            dummy_name = "dummyprogramname";
            break;
        }

        case PT_PROCEDURE:
        {
            token = next_token(token);  // consume PROCEDURE
            defn = (Definition) DF_PROCEDURE;
            stringstream ss;
            ss << setw(3) << setfill('0') << ++dummy_counter;
            dummy_name = "dummyprocedurename_" + ss.str();
            break;
        }

        case PT_FUNCTION:
        {
            token = next_token(token);  // consume FUNCTION
            defn = (Definition) DF_FUNCTION;
            stringstream ss;
            ss << setw(3) << setfill('0') << ++dummy_counter;
            dummy_name = "dummyprocedurename_" + ss.str();
            break;
        }

        default:
        {
            defn = (Definition) DF_PROGRAM;
            dummy_name = "dummyprocedurename_";
            break;
        }
    }

    // Parse the routine name.
    routine_id = parse_routine_name(token, dummy_name);
    routine_id->set_definition(defn);

    token = current_token();

    // Create new intermediate code for the routine.
    ICode *icode = ICodeFactory::create_icode();
    vector<SymTabEntry *> subroutines;
    routine_id->set_attribute((SymTabKey) ROUTINE_ICODE, icode);
    routine_id->set_attribute((SymTabKey) ROUTINE_ROUTINES, subroutines);

    // Push the routine's new symbol table onto the stack.
    // If it was forwarded, push its existing symbol table.
    bool forwarded = false;
    Object attribute = routine_id->get_attribute((SymTabKey) ROUTINE_CODE);
    if (   (!attribute.empty())
        && (cast(attribute, RoutineCode) == (RoutineCode) RT_FORWARD))
    {
        attribute = routine_id->get_attribute((SymTabKey) ROUTINE_SYMTAB);
        SymTab *symtab = cast(attribute, SymTab*);
        symtab_stack->push(symtab);
        forwarded = true;
    }
    else
    {
        routine_id->set_attribute((SymTabKey) ROUTINE_SYMTAB,
                                  symtab_stack->push());
    }

    // Program: Set the program identifier in the symbol table stack.
    if (defn == (Definition) DF_PROGRAM)
    {
        symtab_stack->set_program_id(routine_id);
    }

    // Non-forwarded procedure or function: Append to the parent's list
    //                                      of routines.
    else if (!forwarded)
    {
        Object attribute =
                parent_id->get_attribute((SymTabKey) ROUTINE_ROUTINES);
        vector<SymTabEntry *> subroutines = cast(attribute,
                                                 vector<SymTabEntry *>);
        subroutines.push_back(routine_id);
        parent_id->set_attribute((SymTabKey) ROUTINE_ROUTINES, subroutines);
    }

    // If the routine was forwarded, there should not be
    // any formal parameters or a function return type.
    // But parse them anyway if they're there.
    if (forwarded)
    {
        if (token->get_type() != (TokenType) PT_SEMICOLON)
        {
            error_handler.flag(token, ALREADY_FORWARDED, this);
            parse_header(token, routine_id);
        }
    }

    // Parse the routine's formal parameters and function return type.
    else parse_header(token, routine_id);

    // Look for the semicolon.
    token = current_token();
    if (token->get_type() == (TokenType) PT_SEMICOLON)
    {
        do
        {
            token = next_token(token);  // consume ;
        } while (token->get_type() == (TokenType) PT_SEMICOLON);
    }
    else
    {
        error_handler.flag(token, MISSING_SEMICOLON, this);
    }

    // Parse the routine's block or forward declaration.
    if (   (token->get_type() == (TokenType) PT_IDENTIFIER)
        && (to_lower(token->get_text()) == "forward"))
    {
        token = next_token(token);  // consume forward
        routine_id->set_attribute((SymTabKey) ROUTINE_CODE,
                                  (RoutineCode) RT_FORWARD);
    }
    else
    {
        routine_id->set_attribute((SymTabKey) ROUTINE_CODE,
                                  (RoutineCode) RT_DECLARED);

        BlockParser block_parser(this);
        ICodeNode *root_node = block_parser.parse_block(token, routine_id);
        icode->set_root(root_node);
    }

    // Pop the routine's symbol table off the stack.
    symtab_stack->pop();

    return routine_id;
}

SymTabEntry *DeclaredRoutineParser::parse_routine_name(
                                         Token *token, string dummy_name)
    throw (string)
{
    SymTabEntry *routine_id = nullptr;

    // Parse the routine name identifier.
    if (token->get_type() == (TokenType) PT_IDENTIFIER)
    {
        string routine_name = to_lower(token->get_text());
        routine_id = symtab_stack->lookup_local(routine_name);

        // Not already defined locally: Enter into the local symbol table.
        if (routine_id == nullptr)
        {
            routine_id = symtab_stack->enter_local(routine_name);
        }

        // If already defined, it should be a forward definition.
        else if (cast(routine_id->get_attribute(
                     (SymTabKey) ROUTINE_CODE), RoutineCode)
                                           != (RoutineCode) RT_FORWARD)
        {
            routine_id = nullptr;
            error_handler.flag(token, IDENTIFIER_REDEFINED, this);
        }

        token = next_token(token);  // consume routine name identifier
    }
    else
    {
        error_handler.flag(token, MISSING_IDENTIFIER, this);
    }

    // If necessary, create a dummy routine name symbol table entry.
    if (routine_id == nullptr)
    {
        routine_id = symtab_stack->enter_local(dummy_name);
    }

    return routine_id;
}

void DeclaredRoutineParser::parse_header(
                                   Token *token, SymTabEntry *routine_id)
    throw (string)
{
    // Parse the routine's formal parameters.
    parse_formal_parameters(token, routine_id);
    token = current_token();

    // If this is a function, parse and set its return type.
    if (routine_id->get_definition() == (Definition) DF_FUNCTION)
    {
        VariableDeclarationsParser variable_declarations_parser(this);
        variable_declarations_parser.set_definition(
                                               (Definition) DF_FUNCTION);
        TypeSpec *typespec =
                      variable_declarations_parser.parse_typespec(token);

        token = current_token();

        // The return type cannot be an array or record.
        if (typespec != nullptr)
        {
            TypeForm form = typespec->get_form();
            if (   (form == (TypeForm) TypeFormImpl::ARRAY)
                || (form == (TypeForm) TypeFormImpl::RECORD))
            {
                error_handler.flag(token, INVALID_TYPE, this);
            }
        }

        // Missing return type.
        else
        {
            typespec = Predefined::undefined_type;
        }

        routine_id->set_typespec(typespec);
        token = current_token();
    }
}

void DeclaredRoutineParser::parse_formal_parameters(
                                   Token *token, SymTabEntry *routine_id)
        throw (string)
{
    // Parse the formal parameters if there is an opening left parenthesis.
    token = synchronize(LEFT_PAREN_SET);
    if (token->get_type() == (TokenType) PT_LEFT_PAREN)
    {
        token = next_token(token);  // consume (

        vector<SymTabEntry *> parms;

        token = synchronize(PARAMETER_SET);
        TokenType token_type = token->get_type();

        // Loop to parse sublists of formal parameter declarations.
        while (   (token_type == (TokenType) PT_IDENTIFIER)
               || (token_type == (TokenType) PT_VAR))
        {
            vector<SymTabEntry *> parm_sublist =
                                   parse_parm_sublist(token, routine_id);
            for (SymTabEntry *parm : parm_sublist)
            {
                parms.push_back(parm);
            }

            token = current_token();
            token_type = token->get_type();
        }

        // Closing right parenthesis.
        if (token->get_type() == (TokenType) PT_RIGHT_PAREN)
        {
            token = next_token(token);  // consume )
        }
        else
        {
            error_handler.flag(token, MISSING_RIGHT_PAREN, this);
        }

        routine_id->set_attribute((SymTabKey) ROUTINE_PARMS, parms);
    }
}

vector<SymTabEntry *> DeclaredRoutineParser::parse_parm_sublist(
                                   Token *token, SymTabEntry *routine_id)
    throw (string)
{
    bool is_program =
        routine_id->get_definition() == (Definition) DF_PROGRAM;
    Definition defn = is_program
                            ? (Definition) DF_PROGRAM_PARM
                            : (Definition) -1;
    TokenType token_type = token->get_type();

    // VAR or value parameter?
    if (token_type == (TokenType) PT_VAR)
    {
        if (!is_program)
        {
            defn = (Definition) DF_VAR_PARM;
        }
        else {
            error_handler.flag(token, INVALID_VAR_PARM, this);
        }

        token = next_token(token);  // consume VAR
    }
    else if (!is_program)
    {
        defn = (Definition) DF_VALUE_PARM;
    }

    // Parse the parameter sublist and its type specification.
    VariableDeclarationsParser variable_declarations_parser(this);
    variable_declarations_parser.set_definition(defn);
    vector<SymTabEntry *> sublist =
            variable_declarations_parser.parse_identifier_sublist(
                                 token, PARAMETER_FOLLOW_SET, COMMA_SET);
    token = current_token();
    token_type = token->get_type();

    if (!is_program)
    {
        // Look for one or more semicolons after a sublist.
        if (token_type == (TokenType) PT_SEMICOLON)
        {
            while (token->get_type() ==
                                  (TokenType) PT_SEMICOLON)
            {
                token = next_token(token);  // consume the ;
            }
        }

        // If at the start of the next sublist, then missing a semicolon.
        else if (VariableDeclarationsParser::NEXT_START_SET.find(
                                            (PascalTokenType) token_type)
                     != VariableDeclarationsParser::NEXT_START_SET.end())
        {
            error_handler.flag(token, MISSING_SEMICOLON, this);
        }

        token = synchronize(PARAMETER_SET);
    }

    return sublist;
}

}}}}  // namespace wci::frontend::pascal::parsers
