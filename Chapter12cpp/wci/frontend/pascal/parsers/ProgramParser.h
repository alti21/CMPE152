/**
 * <h1>ProgramParser</h1>
 *
 * <p>Parse a Pascal program.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef PROGRAMPARSER_H_
#define PROGRAMPARSER_H_

#include <string>
#include <set>
#include "DeclarationsParser.h"
#include "../../../frontend/Token.h"
#include "../../../frontend/pascal/PascalParserTD.h"
#include "../../../frontend/pascal/PascalToken.h"
#include "../../../intermediate/SymTabEntry.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;

class ProgramParser : public DeclarationsParser
{
public:
    /**
     * Constructor.
     * @param parent the parent parser.
     */
    ProgramParser(PascalParserTD *parent);

    /**
     * Parse a program.
     * @param token the initial token.
     * @param parent_id the symbol table entry of the parent routine's name.
     * @return nullptr
     * @throw a string message if an error occurred.
     */
    SymTabEntry *parse_declaration(Token *token, SymTabEntry *parent_id)
        throw (string);

    // Synchronization set to start a program.
    static set<PascalTokenType> PROGRAM_START_SET;

    static bool INITIALIZED;

    /**
     * Initialize the synchronization set.
     */
    static void initialize();
};

}}}}  // namespace wci::frontend::pascal::parsers

#endif /* PROGRAMPARSER_H_ */
