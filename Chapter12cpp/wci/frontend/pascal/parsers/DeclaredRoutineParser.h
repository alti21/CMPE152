/**
 * <h1>DeclaredRoutineParser</h1>
 *
 * <p>Parse a main program routine or a declared procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef DECLAREDROUTINEPARSER_H_
#define DECLAREDROUTINEPARSER_H_

#include <string>
#include <vector>
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

class DeclaredRoutineParser : public DeclarationsParser
{
public:
    /**
     * Constructor.
     * @param parent the parent parser.
     */
    DeclaredRoutineParser(PascalParserTD *parent);

    /**
     * Parse a standard subroutine declaration.
     * @param token the initial token.
     * @param parent_id the symbol table entry of the parent routine's name.
     * @return the symbol table entry of the declared routine's name.
     * @throw a string message if an error occurred.
     */
    SymTabEntry *parse_declaration(Token *token, SymTabEntry *parent_id)
        throw (string);

    /**
     * Parse a routine's formal parameter list.
     * @param token the current token.
     * @param routine_id the symbol table entry of the declared routine's name.
     * @throw a string message if an error occurred.
     */
    void parse_formal_parameters(Token *token, SymTabEntry *routine_id)
        throw (string);

    /**
     * Initialize the synchronization set.
     */
    static void initialize();

private:
    // Synchronization set for a formal parameter sublist.
    static set<PascalTokenType> PARAMETER_SET;

    // Synchronization set for the opening left parenthesis.
    static set<PascalTokenType> LEFT_PAREN_SET;

    // Synchronization set for the closing right parenthesis.
    static set<PascalTokenType> RIGHT_PAREN_SET;

    // Synchronization set to follow a formal parameter identifier.
    static set<PascalTokenType> PARAMETER_FOLLOW_SET;

    // Synchronization set for the , token.
    static set<PascalTokenType> COMMA_SET;

    static bool INITIALIZED;
    static int dummy_counter;  // counter for dummy routine names

    /**
     * Parse a routine's name.
     * @param token the current token.
     * @param dummy_name a dummy name in case of parsing problem.
     * @return the symbol table entry of the declared routine's name.
     * @throw a string message if an error occurred.
     */
    SymTabEntry *parse_routine_name(Token *token, string dummy_name)
        throw (string);

    /**
     * Parse a routine's formal parameter list and the function return type.
     * @param token the current token.
     * @param routine_id the symbol table entry of the declared routine's name.
     * @throw a string message if an error occurred.
     */
    void parse_header(Token *token, SymTabEntry *routine_id)
        throw (string);

    /**
     * Parse a sublist of formal parameter declarations.
     * @param token the current token.
     * @param routine_id the symbol table entry of the declared routine's name.
     * @return the sublist of symbol table entries for the parm identifiers.
     * @throw a string message if an error occurred.
     */
    vector<SymTabEntry *> parse_parm_sublist(Token *token,
                                           SymTabEntry *routine_id)
        throw (string);
};

}}}}  // namespace wci::frontend::pascal::parsers

#endif /* DECLAREDROUTINEPARSER_H_ */
