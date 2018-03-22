/**
 * <h1>DeclarationsParser</h1>
 *
 * <p>Parse Pascal declarations.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <set>
#include "DeclarationsParser.h"
#include "ConstantDefinitionsParser.h"
#include "TypeDefinitionsParser.h"
#include "VariableDeclarationsParser.h"
#include "DeclaredRoutineParser.h"
#include "../PascalParserTD.h"
#include "../../../frontend/pascal/PascalToken.h"
#include "../../../intermediate/TypeSpec.h"
#include "../../../intermediate/symtabimpl/SymTabEntryImpl.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;

bool DeclarationsParser::INITIALIZED = false;

set<PascalTokenType> DeclarationsParser::DECLARATION_START_SET =
{
    PT_CONST, PT_TYPE, PT_VAR, PT_PROCEDURE, PT_FUNCTION, PT_BEGIN
};

set<PascalTokenType> DeclarationsParser::TYPE_START_SET;
set<PascalTokenType> DeclarationsParser::VAR_START_SET;
set<PascalTokenType> DeclarationsParser::ROUTINE_START_SET;

void DeclarationsParser::initialize()
{
    if (INITIALIZED) return;

    TYPE_START_SET = DECLARATION_START_SET;
    TYPE_START_SET.erase(PT_CONST);

    VAR_START_SET = TYPE_START_SET;
    VAR_START_SET.erase(PT_TYPE);

    ROUTINE_START_SET = VAR_START_SET;
    ROUTINE_START_SET.erase(PT_VAR);

    INITIALIZED = true;
}

DeclarationsParser::DeclarationsParser(PascalParserTD *parent)
    : PascalParserTD(parent)
{
    initialize();
}

SymTabEntry *DeclarationsParser::parse_declaration(Token *token,
                                                   SymTabEntry *parent_id)
    throw (string)
{
    token = synchronize(DECLARATION_START_SET);

    if (token->get_type() == (TokenType) PT_CONST)
    {
        token = next_token(token);  // consume CONST

        ConstantDefinitionsParser constant_definitions_parser(this);
        constant_definitions_parser.parse_declaration(token, nullptr);
    }

    token = synchronize(TYPE_START_SET);

    if (token->get_type() == (TokenType) PT_TYPE)
    {
        token = next_token(token);  // consume TYPE

        TypeDefinitionsParser type_definitions_parser(this);
        type_definitions_parser.parse_declaration(token, nullptr);
    }

    token = synchronize(VAR_START_SET);

    if (token->get_type() == (TokenType) PT_VAR)
    {
        token = next_token(token);  // consume VAR

        VariableDeclarationsParser variable_declarations_parser(this);
        variable_declarations_parser.set_definition(
                                               (Definition) DF_VARIABLE);
        variable_declarations_parser.parse_declaration(token, nullptr);
    }

    token = synchronize(ROUTINE_START_SET);
    TokenType token_type = token->get_type();

    while (   (token_type == (TokenType) PT_PROCEDURE)
           || (token_type == (TokenType) PT_FUNCTION))
    {
        DeclaredRoutineParser routine_parser(this);
        routine_parser.parse_declaration(token, parent_id);

        // Look for one or more semicolons after a definition.
        token = current_token();
        if (token->get_type() == (TokenType) PT_SEMICOLON)
        {
            while (token->get_type() == (TokenType) PT_SEMICOLON)
            {
                token = next_token(token);  // consume the ;
            }
        }

        token = synchronize(ROUTINE_START_SET);
        token_type = token->get_type();
    }

    return nullptr;
}

}}}}  // namespace wci::frontend::pascal::parsers
