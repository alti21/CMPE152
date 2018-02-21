/**
 * <h1>WhenExecutor</h1>
 *
 * <p>Execute a When statement.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <vector>
#include "WhenExecutor.h"
#include "StatementExecutor.h"
#include "ExpressionExecutor.h"
#include "../../../Object.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;
using namespace wci::intermediate::icodeimpl;

IfExecutor::IfExecutor(Executor *parent)
    : StatementExecutor(parent)
{
}

Object WhenExecutor::execute(ICodeNode *node)
{
    // Get the IF node's children.

}

}}}}  // namespace wci::backend::interpreter::executors
