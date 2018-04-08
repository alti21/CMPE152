/**
 * <h1>DeclaredCallParser</h1>
 *
 * <p>Parse a called to a declared procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef CALLDECLAREDPARSER_H_
#define CALLDECLAREDPARSER_H_

#include <string>
#include "CallParser.h"
#include "../../../frontend/Token.h"
#include "../../../frontend/pascal/PascalParserTD.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;

class CallDeclaredParser : public CallParser
{
public:
    /**
     * Constructor.
     * @param parent the parent parser.
     */
    CallDeclaredParser(PascalParserTD *parent);

    /**
     * Parse a call to a declared procedure or function.
     * @param token the initial token.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_statement(Token *token) throw (string);
};

}}}}  // namespace wci::frontend::pascal::parsers

#endif /* CALLDECLAREDPARSER_H_ */
