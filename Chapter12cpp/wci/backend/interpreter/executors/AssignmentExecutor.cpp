/**
 * <h1>AssignmentExecutor</h1>
 *
 * <p>Execute an assignment statement.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include "AssignmentExecutor.h"
#include "StatementExecutor.h"
#include "ExpressionExecutor.h"
#include "../../../Object.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"
#include "../../../intermediate/SymTabEntry.h"
#include "../../../intermediate/symtabimpl/SymTabEntryImpl.h"
#include "../../../intermediate/symtabimpl/Predefined.h"
#include "../../../intermediate/TypeSpec.h"
#include "../../../intermediate/typeimpl/TypeSpecImpl.h"
#include "../../../backend/interpreter/Cell.h"
#include "../../../message/Message.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::intermediate::typeimpl;
using namespace wci::backend::interpreter;
using namespace wci::message;

AssignmentExecutor::AssignmentExecutor(Executor *parent)
    : StatementExecutor(parent)
{
}

Object AssignmentExecutor::execute(ICodeNode *node)
{
    // The ASSIGN node's children are the target variable
    // and the expression.
    vector<ICodeNode *> children = node->get_children();
    ICodeNode *variable_node = children[0];
    ICodeNode *expression_node = children[1];
    Object attribute = variable_node->get_attribute((ICodeKey) ID);
    SymTabEntry *variable_id = cast(attribute, SymTabEntry*);

    // Execute the target variable to get its reference and
    // execute the expression to get its value.
    ExpressionExecutor expression_executor(this);
    Cell *target_cell =
             expression_executor.execute_variable(variable_node);
    TypeSpec *target_typespec = variable_node->get_typespec();
    TypeSpec *value_typespec  = expression_node->get_typespec()->base_type();
    Object cell_value = expression_executor.execute(expression_node);

    assign_value(node, variable_id, target_cell, target_typespec,
                 cell_value, value_typespec);

    ++execution_count;
    return Object();  // empty
}

void AssignmentExecutor::assign_value(ICodeNode *node,
                                      SymTabEntry *target_id,
                                      Cell *target_cell,
                                      TypeSpec *target_typespec,
                                      Object cell_value,
                                      TypeSpec *value_typespec)
{
    // Range check.
    cell_value = check_range(node, target_typespec, cell_value);

    // Convert an integer value to real if necessary.
    if (   (target_typespec == Predefined::real_type)
        && (value_typespec  == Predefined::integer_type))
    {
        int int_value = cast(cell_value, int);
        float float_value = (float) int_value;
        target_cell->set_value(float_value);
    }

    // String assignment:
    //   target length < value length: truncate the value
    //   target length > value length: blank pad the value
    else if (target_typespec->is_pascal_string())
    {
        Object attribute =
            target_typespec->get_attribute((TypeKey) ARRAY_ELEMENT_COUNT);
        int target_length = cast(attribute, int);
        attribute =
            value_typespec->get_attribute((TypeKey) ARRAY_ELEMENT_COUNT);
        int value_length = cast(attribute, int);
        string string_value = cast(cell_value, string);

        // Truncate the value string.
        if (target_length < value_length) string_value.resize(target_length);

        // Pad the value string with blanks at the right end.
        else if (target_length > value_length)
        {
            for (int i = value_length; i < target_length; ++i)
            {
                string_value += " ";
            }
        }

        target_cell->set_value(copy_of(to_pascal(target_typespec, string_value),
                                       node));
    }

    // Simple assignment.
    else
    {
        target_cell->set_value(copy_of(to_pascal(target_typespec, cell_value), node));
    }

    send_assign_message(node, target_id->get_name(), cell_value);
}

}}}}  // namespace wci::backend::interpreter::executors
