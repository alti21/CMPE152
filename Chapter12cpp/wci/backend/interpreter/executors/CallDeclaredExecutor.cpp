/**
 * <h1>CallDeclaredExecutor</h1>
 *
 * <p>Execute a call a declared procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include "CallDeclaredExecutor.h"
#include "CallExecutor.h"
#include "StatementExecutor.h"
#include "AssignmentExecutor.h"
#include "ExpressionExecutor.h"
#include "../Cell.h"
#include "../MemoryFactory.h"
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

CallDeclaredExecutor::CallDeclaredExecutor(Executor *parent)
    : CallExecutor(parent)
{
}

Object CallDeclaredExecutor::execute(ICodeNode *node)
{
    Object attribute = node->get_attribute((ICodeKey) ID);
    SymTabEntry *routine_id = cast(attribute, SymTabEntry*);
    ActivationRecord *new_ar =
        MemoryFactory::create_activation_record(routine_id);

    // Execute any actual parameters and initialize
    // the formal parameters in the new activation record.
    if (node->get_children().size() > 0)
    {
        ICodeNode *parms_nodes = node->get_children()[0];
        vector<ICodeNode *> actual_nodes = parms_nodes->get_children();
        attribute = routine_id->get_attribute((SymTabKey) ROUTINE_PARMS);
        vector<SymTabEntry *> formal_ids = cast(attribute,
                                                vector<SymTabEntry *>);
        execute_actual_parms(actual_nodes, formal_ids, new_ar);
    }

    // Push the new activation record.
    runtime_stack->push(new_ar);

    send_call_message(node, routine_id->get_name());

    // Get the root node of the routine's intermediate code.
    attribute = routine_id->get_attribute((SymTabKey) ROUTINE_ICODE);
    ICode *icode = cast(attribute, ICode*);
    ICodeNode *root_node = icode->get_root();

    // Execute the routine.
    StatementExecutor statement_executor(this);
    Object cell_value = statement_executor.execute(root_node);

    // Pop off the activation record.
    runtime_stack->pop();
    delete new_ar;

    send_return_message(node, routine_id->get_name());
    return cell_value;
}

void CallDeclaredExecutor::execute_actual_parms(
                                        vector<ICodeNode *> actual_nodes,
                                        vector<SymTabEntry *> formal_ids,
                                        ActivationRecord *new_ar)
{
    ExpressionExecutor expression_executor(this);
    AssignmentExecutor assignment_executor(this);

    for (int i = 0; i < formal_ids.size(); ++i)
    {
        SymTabEntry *formal_id = formal_ids[i];
        Definition formal_defn = formal_id->get_definition();
        Cell *formal_cell = new_ar->get_cell(formal_id->get_name());
        ICodeNode *actual_node = actual_nodes[i];

        // Value parameter.
        if (formal_defn == (Definition) DF_VALUE_PARM)
        {
            TypeSpec *formal_typespec = formal_id->get_typespec();
            TypeSpec *value_typespec  = actual_node->get_typespec()->base_type();
            Object cell_value = expression_executor.execute(actual_node);

            assignment_executor.assign_value(actual_node, formal_id,
                                             formal_cell, formal_typespec,
                                             cell_value, value_typespec);
        }

        // VAR parameter.
        else
        {
            Cell *actual_cell =
                expression_executor.execute_variable(actual_node);
            formal_cell->set_value(actual_cell);
        }
    }
}

}}}}  // namespace wci::backend::interpreter::executors
