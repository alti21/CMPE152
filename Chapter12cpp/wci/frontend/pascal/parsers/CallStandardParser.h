/**
 * <h1>DeclaredStandardParser</h1>
 *
 * <p>Parse a called to a standard procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef CALLSTANDARDPARSER_H_
#define CALLSTANDARDPARSER_H_

#include <string>
#include "CallParser.h"
#include "../../../frontend/Token.h"
#include "../../../frontend/pascal/PascalParserTD.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;

class CallStandardParser : public CallParser
{
public:
    /**
     * Constructor.
     * @param parent the parent parser.
     */
    CallStandardParser(PascalParserTD *parent);

    /**
     * Parse a call to a standard procedure or function.
     * @param token the initial token.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_statement(Token *token) throw (string);

private:
    /**
     * Parse a call to read or readln.
     * @param token the current token.
     * @param call_node the CALL node.
     * @param pf_id the symbol table entry of the standard routine name.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_read_readln(Token *token, ICodeNode *call_node,
                                 SymTabEntry *pf_id)
        throw (string);

    /**
     * Parse a call to write or writeln.
     * @param token the current token.
     * @param call_node the CALL node.
     * @param pf_id the symbol table entry of the standard routine name.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_write_writeln(Token *token, ICodeNode *call_node,
                                   SymTabEntry *pf_id)
        throw (string);

    /**
     * Parse a call to eof or eoln.
     * @param token the current token.
     * @param call_node the CALL node.
     * @param pf_id the symbol table entry of the standard routine name.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_eof_eoln(Token *token, ICodeNode *call_node,
                              SymTabEntry *pf_id)
        throw (string);

    /**
     * Parse a call to abs or sqr.
     * @param token the current token.
     * @param call_node the CALL node.
     * @param pf_id the symbol table entry of the standard routine name.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_abs_sqr(Token *token, ICodeNode *call_node,
                             SymTabEntry *pf_id)
        throw (string);

    /**
     * Parse a call to arctan, cos, exp, ln, sin, or sqrt.
     * @param token the current token.
     * @param call_node the CALL node.
     * @param pf_id the symbol table entry of the standard routine name.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_arctan_cos_exp_ln_sin_sqrt(Token *token,
                                                ICodeNode *call_node,
                                                SymTabEntry *pf_id)
        throw (string);

    /**
     * Parse a call to pred or succ.
     * @param token the current token.
     * @param call_node the CALL node.
     * @param pf_id the symbol table entry of the standard routine name.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_pred_succ(Token *token, ICodeNode *call_node,
                               SymTabEntry *pf_id)
        throw (string);

    /**
     * Parse a call to chr.
     * @param token the current token.
     * @param call_node the CALL node.
     * @param pf_id the symbol table entry of the standard routine name.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_chr(Token *token, ICodeNode *call_node,
                         SymTabEntry *pf_id)
        throw (string);

    /**
     * Parse a call to odd.
     * @param token the current token.
     * @param call_node the CALL node.
     * @param pf_id the symbol table entry of the standard routine name.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_odd(Token *token, ICodeNode *call_node,
                         SymTabEntry *pf_id)
        throw (string);

    /**
     * Parse a call to ord.
     * @param token the current token.
     * @param call_node the CALL node.
     * @param pf_id the symbol table entry of the standard routine name.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_ord(Token *token, ICodeNode *call_node,
                         SymTabEntry *pf_id)
        throw (string);

    /**
     * Parse a call to round or trunc.
     * @param token the current token.
     * @param call_node the CALL node.
     * @param pf_id the symbol table entry of the standard routine name.
     * @return the root node of the generated parse tree.
     * @throw a string message if an error occurred.
     */
    ICodeNode *parse_round_trunc(Token *token, ICodeNode *call_node,
                                 SymTabEntry *pf_id)
        throw (string);

    /**
     * Check the number of actual parameters.
     * @param token the current token.
     * @param parmsNode the PARAMETERS node.
     * @param count the correct number of parameters.
     * @return true if the count is correct.
     */
    bool check_parm_count(Token *token, ICodeNode *parms_node, int count);
};

}}}}  // namespace wci::frontend::pascal::parsers

#endif /* CALLSTANDARDPARSER_H_ */
