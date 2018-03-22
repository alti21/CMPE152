/**
 * <h1>CallParser</h1>
 *
 * <p>Parse a called to a procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef CALLPARSER_H_
#define CALLPARSER_H_

#include "StatementParser.h"
#include "../../../frontend/Token.h"
#include "../../../frontend/pascal/PascalParserTD.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;

class CallParser : public StatementParser
{
public:
    /**
     * Constructor.
     * @param parent the parent parser.
     */
    CallParser(PascalParserTD *parent);

    /**
     * Parse a call to a declared procedure or function.
     * @param token the initial token.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_statement(Token *token) throw (string);

    /**
     * Parse the actual parameters of a procedure or function call.
     * @param token the current token.
     * @param pf_id the symbol table entry of the procedure or function name.
     * @param is_declared true if parsing actual parms of a declared routine.
     * @param is_read_readln true if parsing actual parms of read or readln.
     * @param is_write_writeln true if parsing actual parms of write or writeln.
     * @return the PARAMETERS node, or null if there are no actual parameters.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_actual_parameters(Token *token,
                                       SymTabEntry *pf_id,
                                       const bool is_declared,
                                       const bool is_read_readln,
                                       const bool is_write_writeln)
        throw (string);

private:
    // Synchronization set for the , token.
    static set<PascalTokenType> COMMA_SET;

    static bool INITIALIZED;

    /**
     * Initialize the static maps.
     */
    static void initialize();

    /**
     * Check an actual parameter against the corresponding formal parameter.
     * @param token the current token.
     * @param formal_id the symbol table entry of the formal parameter.
     * @param actual_node the parse tree node of the actual parameter.
     */
    void check_actual_parameter(Token *token, SymTabEntry *formal_id,
                                ICodeNode *actual_node);

    /**
     * Parse the field width or the precision for an actual parameter
     * of a call to write or writeln.
     * @param token the current token.
     * @return the INTEGER_CONSTANT node or null
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_write_spec(Token *token) throw (string);
};

}}}}  // namespace wci::frontend::pascal::parsers

#endif /* CALLPARSER_H_ */
