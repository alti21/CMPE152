/**
 * <h1>CallExecutor</h1>
 *
 * <p>Execute a call to a procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include "CallExecutor.h"
#include "CallDeclaredExecutor.h"
#include "CallStandardExecutor.h"
#include "StatementExecutor.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/SymTabEntry.h"
#include "../../../intermediate/symtabimpl/SymTabEntryImpl.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::intermediate::icodeimpl;

CallExecutor::CallExecutor(Executor *parent) : StatementExecutor(parent)
{
}

Object CallExecutor::execute(ICodeNode *node)
{
    Object attribute = node->get_attribute((ICodeKey) ID);
    SymTabEntry *routine_id = cast(attribute, SymTabEntry*);
    attribute = routine_id->get_attribute((SymTabKey) ROUTINE_CODE);
    RoutineCode routine_code = cast(attribute, RoutineCode);

    ++execution_count;  // count the call statement

    if (routine_code == (RoutineCode) RT_DECLARED)
    {
        CallDeclaredExecutor call_executor(this);
        return call_executor.execute(node);
    }
    else
    {
        CallStandardExecutor call_executor(this);
        return call_executor.execute(node);
    }
}

}}}}  // namespace wci::backend::interpreter::executors
