/**
 * <h1>CallExecutor</h1>
 *
 * <p>Execute a call to a procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef CALLEXECUTOR_H_
#define CALLEXECUTOR_H_

#include "StatementExecutor.h"
#include "../../../intermediate/ICodeNode.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;

class CallExecutor : public StatementExecutor
{
public:
    /**
     * Constructor.
     * @param the parent executor.
     */
    CallExecutor(Executor *parent);

    /**
     * Execute procedure or function call statement.
     * @param node the root node of the call.
     * @return nullptr.
     */
    Object execute(ICodeNode *node);
};

}}}}  // namespace wci::backend::interpreter::executors

#endif /* CALLEXECUTOR_H_ */
