/**
 * <h1>CallDeclaredExecutor</h1>
 *
 * <p>Execute a call a declared procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef CALLDECLAREDEXECUTOR_H_
#define CALLDECLAREDEXECUTOR_H_

#include "CallExecutor.h"
#include "../../../intermediate/ICodeNode.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;

class CallDeclaredExecutor : public CallExecutor
{
public:
    /**
     * Constructor.
     * @param the parent executor.
     */
    CallDeclaredExecutor(Executor *parent);

    /**
     * Execute procedure or function call statement.
     * @param node the root node of the call.
     * @return nullptr.
     */
    Object execute(ICodeNode *node);

private:
    /**
     * Execute the actual parameters of a call.
     * @param actualNodes the list of nodes of the actual parms.
     * @param formalIds the list of symbol table entries of the formal parms.
     * @param newAr the new activation record.
     */
    void execute_actual_parms(vector<ICodeNode *> actual_nodes,
                              vector<SymTabEntry *> formal_ids,
                              ActivationRecord *new_ar);
};

}}}}  // namespace wci::backend::interpreter::executors

#endif /* CALLDECLAREDEXECUTOR_H_ */
