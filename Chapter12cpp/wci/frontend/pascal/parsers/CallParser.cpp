/**
 * <h1>CallParser</h1>
 *
 * <p>Parse a called to a procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <vector>
#include <set>
#include "CallParser.h"
#include "CallDeclaredParser.h"
#include "CallStandardParser.h"
#include "StatementParser.h"
#include "ExpressionParser.h"
#include "../PascalToken.h"
#include "../PascalError.h"
#include "../../../Object.h"
#include "../../../frontend/Token.h"
#include "../../../frontend/pascal/PascalParserTD.h"
#include "../../../frontend/pascal/parsers/ExpressionParser.h"
#include "../../../intermediate/symtabimpl/SymTabEntryImpl.h"
#include "../../../intermediate/symtabimpl/Predefined.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/ICodeFactory.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"
#include "../../../intermediate/icodeimpl/ICodeImpl.h"
#include "../../../intermediate/typeimpl/TypeSpecImpl.h"
#include "../../../intermediate/typeimpl/TypeChecker.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::intermediate::icodeimpl;
using namespace wci::intermediate::typeimpl;

bool CallParser::INITIALIZED = false;

set<PascalTokenType> CallParser::COMMA_SET;

void CallParser::initialize()
{
    if (INITIALIZED) return;

    ExpressionParser::initialize();
    COMMA_SET = ExpressionParser::EXPR_START_SET;
    COMMA_SET.insert(PT_COMMA);
    COMMA_SET.insert(PT_RIGHT_PAREN);

    INITIALIZED = true;
}

CallParser::CallParser(PascalParserTD *parent) : StatementParser(parent)
{
    initialize();
}

ICodeNode *CallParser::parse_statement(Token *token) throw (string)
{
    string name = to_lower(token->get_text());
    SymTabEntry *pf_id = symtab_stack->lookup(name);
    Object attribute = pf_id->get_attribute((SymTabKey) ROUTINE_CODE);
    RoutineCode routine_code = cast(attribute, RoutineCode);

    if (   (routine_code == (RoutineCode) RT_DECLARED)
        || (routine_code == (RoutineCode) RT_FORWARD))
    {
        CallDeclaredParser call_declared_parser(this);
        return call_declared_parser.parse_statement(token);
    }
    else
    {
        CallStandardParser call_standard_parser(this);
        return call_standard_parser.parse_statement(token);
    }
}

ICodeNode *CallParser::parse_actual_parameters(Token *token,
                                               SymTabEntry *pf_id,
                                               const bool is_declared,
                                               const bool is_read_readln,
                                               const bool is_write_writeln)
    throw (string)
{
    ExpressionParser expression_parser(this);
    ICodeNode *parms_node =
        ICodeFactory::create_icode_node((ICodeNodeType) NT_PARAMETERS);
    vector<SymTabEntry *> formal_parms;
    int parm_count = 0;
    int parm_index = -1;

    if (is_declared)
    {
        Object attribute = pf_id->get_attribute((SymTabKey) ROUTINE_PARMS);
        if (!attribute.empty())
        {
            formal_parms = cast(attribute, vector<SymTabEntry *>);
            parm_count = formal_parms.size();
        }
    }

    if (token->get_type() != (TokenType) PT_LEFT_PAREN)
    {
        if (parm_count != 0)
        {
            error_handler.flag(token, WRONG_NUMBER_OF_PARMS, this);
        }

        return nullptr;
    }

    token = next_token(token);  // consume opening (

    // Loop to parse each actual parameter.
    while (token->get_type() != (TokenType) PT_RIGHT_PAREN)
    {
        Token *actual_token = new Token(*token);
        ICodeNode *actual_node = expression_parser.parse_statement(token);

        // Declared procedure or function: Check the number of actual
        // parameters, and check each actual parameter against the
        // corresponding formal parameter.
        if (is_declared)
        {
            if (++parm_index < parm_count)
            {
                SymTabEntry *formal_id = formal_parms[parm_index];
                check_actual_parameter(actual_token, formal_id,
                                       actual_node);
            }
            else if (parm_index == parm_count)
            {
                error_handler.flag(actual_token, WRONG_NUMBER_OF_PARMS, this);
            }
        }

        // read or readln: Each actual parameter must be a variable that is
        //                 a scalar, bool, or subrange of integer.
        else if (is_read_readln)
        {
            TypeSpec *typespec = actual_node->get_typespec();
            TypeForm form = typespec->get_form();

            if (! (   (actual_node->get_type() ==
                                             (ICodeNodeType) NT_VARIABLE)
                   && (   (form == (TypeForm) TF_SCALAR)
                       || (typespec == Predefined::boolean_type)
                       || (   (form == (TypeForm) TF_SUBRANGE)
                           && (typespec->base_type() ==
                                            Predefined::integer_type))
                      )
                  )
               )
            {
                error_handler.flag(actual_token, INVALID_VAR_PARM, this);
            }
        }

        // write or writeln: The type of each actual parameter must be a
        // scalar, bool, or a Pascal string. Parse any field width and
        // precision.
        else if (is_write_writeln)
        {
            // Create a WRITE_PARM node which adopts the expression node.
            ICodeNode *expr_node = actual_node;
            actual_node = ICodeFactory::create_icode_node((ICodeNodeType) NT_WRITE_PARM);
            actual_node->add_child(expr_node);

            TypeSpec *typespec = expr_node->get_typespec() != nullptr
                            ? expr_node->get_typespec()->base_type()
                            : nullptr;
            TypeForm form = typespec != nullptr
                                ? (TypeForm) typespec->get_form()
                                : (TypeForm) -1;

            if (! (   (form == (TypeForm) TF_SCALAR)
                   || (typespec == Predefined::boolean_type)
                   || (   (typespec != nullptr)
                       && (typespec->is_pascal_string()))))
            {
                error_handler.flag(actual_token, INCOMPATIBLE_TYPES, this);
            }

            // Optional field width.
            token = current_token();
            actual_node->add_child(parse_write_spec(token));

            // Optional precision.
            token = current_token();
            actual_node->add_child(parse_write_spec(token));
        }

        parms_node->add_child(actual_node);
        token = synchronize(COMMA_SET);
        TokenType tokenType = token->get_type();

        // Look for the comma.
        if (tokenType == (TokenType) PT_COMMA)
        {
            token = next_token(token);  // consume ,
        }
        else if (ExpressionParser::EXPR_START_SET.find(
                                             (PascalTokenType) tokenType)
                               != ExpressionParser::EXPR_START_SET.end())
        {
            error_handler.flag(token, MISSING_COMMA, this);
        }
        else if (tokenType != (TokenType) PT_RIGHT_PAREN)
        {
            token = synchronize(ExpressionParser::EXPR_START_SET);
        }

        delete actual_token;
    }

    token = next_token(token);  // consume closing )

    if (   (parms_node->get_children().size() == 0)
        || (is_declared && (parm_index != parm_count-1)))
    {
        error_handler.flag(token, WRONG_NUMBER_OF_PARMS, this);
    }

    return parms_node;
}

void CallParser::check_actual_parameter(Token *token,
                                        SymTabEntry *formal_id,
                                        ICodeNode *actual_node)
{
    Definition formal_defn = formal_id->get_definition();
    TypeSpec *formal_type = formal_id->get_typespec();
    TypeSpec *actual_type = actual_node->get_typespec();

    // VAR parameter: The actual parameter must be a variable of the same
    //                type as the formal parameter.
    if (formal_defn == (Definition) DF_VAR_PARM)
    {
        if (   (actual_node->get_type() != (ICodeNodeType) NT_VARIABLE)
            || (actual_type != formal_type))
        {
            error_handler.flag(token, INVALID_VAR_PARM, this);
        }
    }

    // Value parameter: The actual parameter must be assignment-compatible
    //                  with the formal parameter.
    else if (!TypeChecker::are_assignment_compatible(formal_type,
                                                     actual_type))
    {
        error_handler.flag(token, INCOMPATIBLE_TYPES, this);
    }
}

/**
 * Parse the field width or the precision for an actual parameter
 * of a call to write or writeln.
 * @param token the current token.
 * @return the INTEGER_CONSTANT node or nullptr
 * @throws Exception if an error occurred.
 */
ICodeNode *CallParser::parse_write_spec(Token *token) throw (string)
{
    if (token->get_type() == (TokenType) PT_COLON)
    {
        token = next_token(token);  // consume :

        ExpressionParser expression_parser = new ExpressionParser(this);
        ICodeNode *spec_node = expression_parser.parse_statement(token);

        if (spec_node->get_type() == (ICodeNodeType) NT_INTEGER_CONSTANT)
        {
            return spec_node;
        }
        else {
            error_handler.flag(token, INVALID_NUMBER, this);
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}

}}}}  // namespace wci::frontend::pascal::parsers
