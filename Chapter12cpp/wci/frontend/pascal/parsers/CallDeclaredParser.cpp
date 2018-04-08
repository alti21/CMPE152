/**
 * <h1>DeclaredCallParser</h1>
 *
 * <p>Parse a called to a declared procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include "CallDeclaredParser.h"
#include "CallParser.h"
#include "StatementParser.h"
#include "ExpressionParser.h"
#include "../PascalToken.h"
#include "../PascalError.h"
#include "../../../frontend/Token.h"
#include "../../../frontend/pascal/PascalParserTD.h"
#include "../../../frontend/pascal/parsers/ExpressionParser.h"
#include "../../../intermediate/symtabimpl/SymTabEntryImpl.h"
#include "../../../intermediate/symtabimpl/Predefined.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/ICodeFactory.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"
#include "../../../intermediate/icodeimpl/ICodeImpl.h"
#include "../../../intermediate/typeimpl/TypeSpecImpl.h"
#include "../../../intermediate/typeimpl/TypeChecker.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::intermediate::icodeimpl;
using namespace wci::intermediate::typeimpl;

CallDeclaredParser::CallDeclaredParser(PascalParserTD *parent)
    : CallParser(parent)
{
}

ICodeNode *CallDeclaredParser::parse_statement(Token *token) throw (string)
{
    // Create the CALL node.
    ICodeNode *call_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_CALL);

    string name = to_lower(token->get_text());
    SymTabEntry *pf_id = symtab_stack->lookup(name);
    call_node->set_attribute((ICodeKey) ID, pf_id);
    call_node->set_typespec(pf_id->get_typespec());

    token = next_token(token);  // consume procedure or function identifier

    ICodeNode *parms_node = parse_actual_parameters(token, pf_id,
                                                    true, false, false);

    call_node->add_child(parms_node);
    return call_node;
}

}}}}  // namespace wci::frontend::pascal::parsers
