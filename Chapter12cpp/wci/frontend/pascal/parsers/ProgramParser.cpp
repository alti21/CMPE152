/**
 * <h1>ProgramParser</h1>
 *
 * <p>Parse a Pascal program.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include <vector>
#include <set>
#include "ProgramParser.h"
#include "DeclarationsParser.h"
#include "DeclaredRoutineParser.h"
#include "../../../frontend/Token.h"
#include "../../../frontend/pascal/PascalParserTD.h"
#include "../../../frontend/pascal/PascalToken.h"
#include "../../../intermediate/SymTabEntry.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::pascal;

bool ProgramParser::INITIALIZED = false;

set<PascalTokenType> ProgramParser::PROGRAM_START_SET;

void ProgramParser::initialize()
{
    DeclarationsParser::initialize();
    PROGRAM_START_SET = DeclarationsParser::DECLARATION_START_SET;
    PROGRAM_START_SET.insert(PT_PROGRAM);
    PROGRAM_START_SET.insert(PT_SEMICOLON);
}

ProgramParser::ProgramParser(PascalParserTD *parent)
    : DeclarationsParser(parent)
{
    initialize();
}

SymTabEntry *ProgramParser::parse_declaration(Token *token,
                                              SymTabEntry *parent_id)
    throw (string)
{
    token = synchronize(PROGRAM_START_SET);

    // Parse the program.
    DeclaredRoutineParser routine_parser(this);
    routine_parser.parse_declaration(token, parent_id);

    // Look for the final period.
    token = current_token();
    if (token->get_type() != (TokenType) PT_DOT)
    {
        error_handler.flag(token, MISSING_PERIOD, this);
    }

    return nullptr;
}

}}}}  // namespace wci::frontend::pascal::parsers
