/**
 * <h1>WhenStatementParser</h1>
 *
 * <p>Parse a Pascal WHEN statement.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef WHENSTATEMENTPARSER_H_
#define WHENSTATEMENTPARSER_H_

#include <string>
#include <set>
#include "StatementParser.h"
#include "../../Token.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;

class WhenStatementParser : public StatementParser
{
public:
    /**
     * Constructor.
     * @param parent the parent parser.
     */
    WhenStatementParser(PascalParserTD *parent);

    /**
     * Parse an IF statement.
     * @param token the initial token.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_statement(Token *token) throw (string);

private:
    // Synchronization set for starting a WHEN option constant.
    static set<PascalTokenType> EQUALS_GREATER_SET;

    static bool INITIALIZED;

    /**
     * Initialize the synchronization sets.
     */
    static void initialize();

    /**
     * Parse a WHEN branch.
     * @param token the current token.
     * @param constant_set the set of WHEN branch constants.
     * @return the root SELECT_BRANCH node of the subtree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_branch(Token *token)
        throw (string);
};

}}}}  // namespace wci::frontend::pascal::parsers

#endif /* WHENSTATEMENTPARSER_H_ */
