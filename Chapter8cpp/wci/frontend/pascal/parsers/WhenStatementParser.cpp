/*
 * WhenStatementParser.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: Alti
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

bool WhenStatementParser::INITIALIZED = false;

set<PascalTokenType> WhenStatementParser::OTHERWISE_SET;

void WhenStatementParser::initialize()
{
    if (INITIALIZED) return;

    OTHERWISE_SET = StatementParser::STMT_START_SET;
    OTHERWISE_SET.insert(PascalTokenType::OTHERWISE);

    set<PascalTokenType>::iterator it;
    for (it  = StatementParser::STMT_FOLLOW_SET.begin();
         it != StatementParser::STMT_FOLLOW_SET.end();
         it++)
    {
        OTHERWISE_SET.insert(*it);
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
    token = next_token(token);  // consume the WHILE

    ICodeNode *when_node =
               ICodeFactory::create_icode_node((ICodeNodeType) NT_WHEN);
       ICodeNode *otherwise_node =
               ICodeFactory::create_icode_node((ICodeNodeType) NT_OTHERWISE);

       // Parse the expression.
       // The NOT node adopts the expression subtree as its only child.
       ExpressionParser expression_parser(this);
       when_node->add_child(expression_parser.parse_statement(token));


    // Synchronize at the DO.
    token = synchronize(OTHERWISE_SET);
    if (token->get_type() == (TokenType) PT_OTHERWISE)
    {
        token = next_token(token);  // consume the OTHERWISE
    }
    else {
        error_handler.flag(token, MISSING_OTHERWISE, this);
    }

    // Parse the statement.
    // The LOOP node adopts the statement subtree as its second child.
  //  StatementParser statement_parser(this);
    //loop_node->add_child(statement_parser.parse_statement(token));

    return when_node;
}

}}}}


