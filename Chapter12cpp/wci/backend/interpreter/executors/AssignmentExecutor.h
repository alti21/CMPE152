/**
 * <h1>AssignmentExecutor</h1>
 *
 * <p>Execute an assignment statement.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef ASSIGNMENTEXECUTOR_H_
#define ASSIGNMENTEXECUTOR_H_

#include <string>
#include "StatementExecutor.h"
#include "../Cell.h"
#include "../../../Object.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../message/Message.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;
using namespace wci::message;

class AssignmentExecutor : public StatementExecutor
{
public:
    /**
     * Constructor.
     * @param the parent executor.
     */
    AssignmentExecutor(Executor *parent);

    /**
     * Execute an assignmnent statement.
     * @param node the root node of the compound statement.
     * @return nullptr.
     */
    Object execute(ICodeNode *node);

    /**
     * Assign a value to a target cell.
     * @param node the ancester parse tree node of the assignment.
     * @param target_id the symbol table entry of the target variable or parm.
     * @param target_cell the target cell.
     * @param target_typespec the target type.
     * @param value the value to assign.
     * @param value_typespec the value type.
     */
    void assign_value(ICodeNode *node, SymTabEntry *target_id,
                      Cell *target_cell, TypeSpec *target_typespec,
                      Object value, TypeSpec *value_typespec);
};

}}}}  // namespace wci::backend::interpreter::executors

#endif /* ASSIGNMENTEXECUTOR_H_ */
