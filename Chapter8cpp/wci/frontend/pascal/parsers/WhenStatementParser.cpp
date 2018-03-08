/**
 * <h1>WhenStatementParser</h1>
 *
 * <p>Parse a Pascal CASE statement.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include <set>
#include "WhenStatementParser.h"
#include "StatementParser.h"
#include "AssignmentStatementParser.h"
#include "ExpressionParser.h"
#include "../PascalParserTD.h"
#include "../PascalToken.h"
#include "../PascalError.h"
#include "../../Token.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/ICodeFactory.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;
using namespace wci::intermediate::icodeimpl;

set<PascalTokenType> WhenStatementParser::EQUALS_GREATER_SET;

bool WhenStatementParser::INITIALIZED = false;

void WhenStatementParser::initialize()
{
    if (INITIALIZED) return;

    EQUALS_GREATER_SET = StatementParser::STMT_START_SET;
    EQUALS_GREATER_SET.insert(PascalTokenType::EQUALS_GREATER);

    set<PascalTokenType>::iterator it;
    for (it  = StatementParser::STMT_FOLLOW_SET.begin();
         it != StatementParser::STMT_FOLLOW_SET.end();
         it++)
    {
    		EQUALS_GREATER_SET.insert(*it);
    }

    INITIALIZED = true;
}

WhenStatementParser::WhenStatementParser(PascalParserTD *parent)
    : StatementParser(parent)
{
    initialize();
}

ICodeNode *WhenStatementParser::parse_statement(Token *token) throw (string)
{
    token = next_token(token);  // consume the WHEN

    // Create a SELECT node.
    ICodeNode *select_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_WHEN);

    // Loop to parse each WHEN branch until the END token
    // or the end of the source file.
    while ((token != nullptr) &&
           (token->get_type() != (TokenType) PT_END) &&
		   (token->get_type() != (TokenType) PT_OTHERWISE))
    {
        // The SELECT node adopts the WHEN branch subtree.
        select_node->add_child(parse_branch(token));

        token = current_token();
        TokenType token_type = token->get_type();

        // Look for the semicolon between WHEN branches.
        if (token_type == (TokenType) PT_SEMICOLON)
        {
            token = next_token(token);  // consume the ;
        }
        else
        {
        		error_handler.flag(token, MISSING_SEMICOLON, this);
        }
    }

    // Look for OTHERWISE.
    if (token->get_type() == (TokenType) PT_OTHERWISE)
    {
        token = next_token(token);  // consume the OTHERWISE

        // Synchronize at the EQUALS_GREATER.
        token = synchronize(EQUALS_GREATER_SET);
        if (token->get_type() == (TokenType) PT_EQUALS_GREATER)
        {
            token = next_token(token);  // consume the EQUALS_GREATER
        }
        else {
            error_handler.flag(token, MISSING_EQUALS_GREATER, this);
        }

        // Parse the EQUALS_GREATER statement.
        // The branch node adopts the statement subtree as its second child.
        StatementParser statement_parser(this);
        select_node->add_child(statement_parser.parse_statement(token));

        // Look for the END token.
        if (token->get_type() == (TokenType) PT_END)
        {
            token = next_token(token);  // consume END
        }
        else
        {
            error_handler.flag(token, MISSING_END, this);
        }
    }
    else
    {
    		error_handler.flag(token, MISSING_OTHERWISE, this);

            // Look for the END token.
            if (token->get_type() == (TokenType) PT_END)
            {
                token = next_token(token);  // consume END
            }
            else
            {
                error_handler.flag(token, MISSING_END, this);
            }
    }

    return select_node;
}

ICodeNode *WhenStatementParser::parse_branch(Token *token)
    throw (string)
{
    // Create an SELECT_BRANCH node and a SELECT_CONSTANTS node.
    // The SELECT_BRANCH node adopts the SELECT_CONSTANTS node as its
    // first child.
    ICodeNode *branch_node =
            ICodeFactory::create_icode_node(
                                       (ICodeNodeType) NT_SELECT_BRANCH);

    // Parse the expression.
    // The branch node adopts the expression subtree as its first child.
    ExpressionParser expression_parser(this);
    branch_node->add_child(expression_parser.parse_statement(token));

    // Synchronize at the EQUALS_GREATER.
    token = synchronize(EQUALS_GREATER_SET);
    if (token->get_type() == (TokenType) PT_EQUALS_GREATER)
    {
        token = next_token(token);  // consume the EQUALS_GREATER
    }
    else {
        error_handler.flag(token, MISSING_EQUALS_GREATER, this);
    }

    // Parse the EQUALS_GREATER statement.
    // The branch node adopts the statement subtree as its second child.
    StatementParser statement_parser(this);
    branch_node->add_child(statement_parser.parse_statement(token));

    return branch_node;
}

}}}}  // namespace wci::frontend::pascal::parsers
