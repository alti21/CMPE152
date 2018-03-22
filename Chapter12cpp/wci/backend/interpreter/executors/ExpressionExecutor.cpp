/**
 * <h1>ExpressionExecutor</h1>
 *
 * <p>Execute an expression.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include <vector>
#include <set>
#include "ExpressionExecutor.h"
#include "StatementExecutor.h"
#include "CallExecutor.h"
#include "../Cell.h"
#include "../RuntimeError.h"
#include "../../../Object.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/TypeSpec.h"
#include "../../../intermediate/typeimpl/TypeSpecImpl.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"
#include "../../../intermediate/symtabimpl/SymTabEntryImpl.h"
#include "../../../intermediate/symtabimpl/Predefined.h"
#include "../../../backend/BackendFactory.h"
#include "../../../backend/interpreter/memoryimpl/MemoryMapImpl.h"
#include "../../../backend/interpreter/memoryimpl/CellImpl.h"
#include "../../../message/Message.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::intermediate::icodeimpl;
using namespace wci::intermediate::typeimpl;;
using namespace wci::backend;
using namespace wci::backend::interpreter::memoryimpl;
using namespace wci::message;

set<ICodeNodeTypeImpl> ExpressionExecutor::ARITH_OPS =
{
    NT_ADD, NT_SUBTRACT, NT_MULTIPLY,
    NT_FLOAT_DIVIDE, NT_INTEGER_DIVIDE, NT_MOD,
};

ExpressionExecutor::ExpressionExecutor(Executor *parent)
    : StatementExecutor(parent)
{
}

Object ExpressionExecutor::execute(ICodeNode *node)
{
    ICodeNodeTypeImpl node_type = (ICodeNodeTypeImpl) node->get_type();

    switch (node_type)
    {
        case NT_VARIABLE:
        {
            // Return the variable's value.
            return execute_value(node);
        }

        case NT_INTEGER_CONSTANT:
        {
            TypeSpec *typespec = node->get_typespec();
            Object attribute = node->get_attribute((ICodeKey) VALUE);
            int value = cast(attribute, int);

            // If boolean, return true or false.
            // Else return the integer value.
            if (typespec == Predefined::boolean_type)
            {
                if (value == 1) return true;
                else            return false;
            }
            else                return value;
        }

        case NT_REAL_CONSTANT:
        {
            // Return the data value.
            Object attribute = node->get_attribute((ICodeKey) VALUE);
            return cast(attribute, float);
        }

        case NT_STRING_CONSTANT:
        {
            // Return the string value.
            Object attribute = node->get_attribute((ICodeKey) VALUE);
            return cast(attribute, string);
        }

        case NT_NEGATE:
        {
            // Get the NEGATE node's expression node child.
            vector<ICodeNode *> children = node->get_children();
            ICodeNode *expression_node = children[0];

            // Execute the expression and return the negative of its value.
            Object value = execute(expression_node);
            if (instanceof(value,int)) return -cast(value, int);
            else                       return -cast(value, float);
        }

        case NT_NOT:
        {
            // Get the NOT node's expression node child.
            vector<ICodeNode *> children = node->get_children();
            ICodeNode *expression_node = children[0];

            // Execute the expression and return the "not" of its value.
            Object value = execute(expression_node);
            return !cast(value, bool);
        }

        case NT_CALL:
        {
            // Execute a function call.
            Object attribute = node->get_attribute((ICodeKey) ID);
            SymTabEntry *function_id = cast(attribute, SymTabEntry*);
            attribute =
                    function_id->get_attribute((SymTabKey) ROUTINE_CODE);
            RoutineCode routine_code = cast(attribute, RoutineCode);

            CallExecutor call_executor(this);
            Object value = call_executor.execute(node);

            // If it was a declared function, obtain the function value
            // from its name.
            if (routine_code == (RoutineCode) RT_DECLARED)
            {
                string function_name = function_id->get_name();
                int nesting_level =
                        function_id->get_symtab()->get_nesting_level();
                ActivationRecord *ar =
                              runtime_stack->get_topmost(nesting_level);
                Cell *function_value_cell = ar->get_cell(function_name);
                value = function_value_cell->get_value();

                send_fetch_message(node, function_id->get_name(), value);
            }

            // Return the function value.
            return value;
        }

        // Must be a binary operator.
        default: return execute_binary_operator(node, node_type);
    }
}

Object ExpressionExecutor::execute_value(ICodeNode *node)
{
    Object attribute = node->get_attribute((ICodeKey) ID);
    SymTabEntry *variable_id = cast(attribute, SymTabEntry*);
    string variable_name = variable_id->get_name();
    TypeSpec *variable_typespec = variable_id->get_typespec();

    // Get the variable's value.
    Cell *variable_cell = execute_variable(node);
    Object value = variable_cell->get_value();

    if (!value.empty())
    {
        value = to_java(variable_typespec, value, node);
    }

    // Uninitialized value error: Use a default value.
    else
    {
        error_handler.flag(node, UNINITIALIZED_VALUE, this);

        value = BackendFactory::default_value(variable_typespec);
        variable_cell->set_value(value);
    }

    send_fetch_message(node, variable_name, value);
    return value;
}

Cell *ExpressionExecutor::execute_variable(ICodeNode *node)
{
    Object attribute = node->get_attribute((ICodeKey) ID);
    SymTabEntry *variable_id = cast(attribute, SymTabEntry*);
    string variable_name = variable_id->get_name();

    TypeSpec *variable_typespec = variable_id->get_typespec();
    int nesting_level = variable_id->get_symtab()->get_nesting_level();

    // Get the variable reference from the appropriate activation record.
    ActivationRecord *ar = runtime_stack->get_topmost(nesting_level);
    Cell *variable_cell = ar->get_cell(variable_name);
    vector<ICodeNode *> modifiers = node->get_children();

    // Reference to a reference: Use the original reference.
    if (!variable_cell->get_value().empty())
    {
        if (instanceof(variable_cell->get_value(), Cell*))
        {
            Cell *next_cell = cast(variable_cell->get_value(), Cell*);
            variable_cell = next_cell;
        }
    }

    // Execute any array subscripts or record fields.
    for (ICodeNode *modifier : modifiers)
    {
        ICodeNodeType node_type = modifier->get_type();

        // Subscripts.
        if (node_type == (ICodeNodeType) NT_SUBSCRIPTS)
        {
            vector<ICodeNode *> subscripts = modifier->get_children();

            // Compute a new reference for each subscript.
            for (ICodeNode *subscript : subscripts)
            {
                attribute = variable_typespec->get_attribute(
                                            (TypeKey) ARRAY_INDEX_TYPE);
                TypeSpec *index_typespec = cast(attribute, TypeSpec*);
                int min_index = 0;

                if (index_typespec->get_form() == (TypeForm) TF_SUBRANGE)
                {
                    attribute = index_typespec->get_attribute(
                                          (TypeKey) SUBRANGE_MIN_VALUE);
                    min_index = cast(attribute, int);
                }

                Object subscript_value = execute(subscript);
                int value = cast(subscript_value, int);
                value = cast(check_range(node, index_typespec, value), int);

                int index = value - min_index;
                vector<Cell *> *array = cast(variable_cell->get_value(),
                                             vector<Cell *>*);
                variable_cell = (*array)[index];

                attribute = variable_typespec->get_attribute(
                                              (TypeKey) ARRAY_ELEMENT_TYPE);
                variable_typespec = cast(attribute, TypeSpec*);
            }
        }

        // Field.
        else if (node_type == (ICodeNodeType) NT_FIELD)
        {
            Object attribute = modifier->get_attribute((ICodeKey) ID);
            SymTabEntry *field_id = cast(attribute, SymTabEntry*);
            string field_name = field_id->get_name();

            // Compute a new reference for the field.
            Object cell_value = variable_cell->get_value();
            MemoryMapImpl *mmap = cast(cell_value, MemoryMapImpl*);
            variable_cell = mmap->get_cell(field_name);
            variable_typespec = field_id->get_typespec();
        }
    }

    return variable_cell;
}

Object ExpressionExecutor::execute_binary_operator(
                      ICodeNode *node, const ICodeNodeTypeImpl node_type)
{
    // Get the two operand children of the operator node.
    vector<ICodeNode *> children = node->get_children();
    ICodeNode *operand_node1 = children[0];
    ICodeNode *operand_node2 = children[1];

    // Operands.
    Object operand1 = execute(operand_node1);
    Object operand2 = execute(operand_node2);
    TypeSpec *typespec1 = operand_node1->get_typespec()->base_type();
    TypeSpec *typespec2 = operand_node2->get_typespec()->base_type();

    bool integer_mode = false;
    bool character_mode = false;
    bool string_mode = false;

    if (   (typespec1 == Predefined::integer_type)
        && (typespec2 == Predefined::integer_type))
    {
        integer_mode = true;
    }
    else if (   (   (typespec1 == Predefined::char_type)
                 || (   instanceof(operand1, string)
                     && (cast(operand1, string).length() == 1)))
             && (   (typespec1 == Predefined::char_type)
                 || (   (instanceof(operand1, string)
                     && (cast(operand1, string).length() == 1)))))
    {
        character_mode = true;
    }
    else if (   instanceof(operand1, string)
             && instanceof(operand2, string))
    {
        string_mode = true;
    }

    // ====================
    // Arithmetic operators
    // ====================

    if (ARITH_OPS.find(node_type) != ARITH_OPS.end())
    {
        if (integer_mode)
        {
            int value1 = cast(operand1, int);
            int value2 = cast(operand2, int);

            // Integer operations.
            switch (node_type)
            {
                case NT_ADD:      return value1 + value2;
                case NT_SUBTRACT: return value1 - value2;
                case NT_MULTIPLY: return value1 * value2;

                case NT_FLOAT_DIVIDE:
                {
                    // Check for division by zero.
                    if (value2 != 0)
                    {
                        return ((float) value1)/((float) value2);
                    }
                    else
                    {
                        error_handler.flag(node, DIVISION_BY_ZERO, this);
                        return 0;
                    }
                }

                case NT_INTEGER_DIVIDE:
                {
                    // Check for division by zero.
                    if (value2 != 0)
                    {
                        return value1/value2;
                    }
                    else
                    {
                        error_handler.flag(node, DIVISION_BY_ZERO, this);
                        return 0;
                    }
                }

                case NT_MOD:
                {
                    // Check for division by zero.
                    if (value2 != 0)
                    {
                        return value1%value2;
                    }
                    else
                    {
                        error_handler.flag(node, DIVISION_BY_ZERO, this);
                        return 0;
                    }
                }

                default: return Object();  // empty -- shouldn't get here
            }
        }
        else
        {
            float value1 = instanceof(operand1, int) ? cast(operand1, int)
                                                     : cast(operand1, float);
            float value2 = instanceof(operand2, int) ? cast(operand2, int)
                                                     : cast(operand2, float);

            // Float operations.
            switch (node_type)
            {
                case NT_ADD:      return value1 + value2;
                case NT_SUBTRACT: return value1 - value2;
                case NT_MULTIPLY: return value1 * value2;

                case NT_FLOAT_DIVIDE:
                {
                    // Check for division by zero.
                    if (value2 != 0.0f)
                    {
                        return value1/value2;
                    }
                    else 
					{
                        error_handler.flag(node, DIVISION_BY_ZERO, this);
                        return 0.0f;
                    }
                }

                default: return Object();  // empty -- shouldn't get here
            }
        }
    }

    // ==========
    // AND and OR
    // ==========

    else if ((node_type == NT_AND) || (node_type == NT_OR))
    {
        bool value1 = cast(operand1, bool);
        bool value2 = cast(operand2, bool);

        switch (node_type)
        {
            case NT_AND: return value1 && value2;
            case NT_OR:  return value1 || value2;

            default: return Object();  // empty -- shouldn't get here
        }
    }

    // ====================
    // Relational operators
    // ====================

    else if (integer_mode)
    {
        int value1 = cast(operand1, int);
        int value2 = cast(operand2, int);

        // Integer operands.
        switch (node_type)
        {
            case NT_EQ: return value1 == value2;
            case NT_NE: return value1 != value2;
            case NT_LT: return value1 <  value2;
            case NT_LE: return value1 <= value2;
            case NT_GT: return value1 >  value2;
            case NT_GE: return value1 >= value2;

            default: return Object();  // empty -- shouldn't get here
        }
    }
    else if (character_mode)
    {
        int value1 = instanceof(operand1, char) ? cast(operand1, char)
                                                : cast(operand1, string)[0];
        int value2 = instanceof(operand2, char) ? cast(operand2, char)
                                                : cast(operand2, string)[0];

        // Character operands.
        switch (node_type)
        {
            case NT_EQ: return value1 == value2;
            case NT_NE: return value1 != value2;
            case NT_LT: return value1 <  value2;
            case NT_LE: return value1 <= value2;
            case NT_GT: return value1 >  value2;
            case NT_GE: return value1 >= value2;

            default: return Object();  // empty -- shouldn't get here
        }
    }
    else if (string_mode)
    {
        string value1 = cast(operand1, string);
        string value2 = cast(operand2, string);
		
        // String operands.
        switch (node_type)
        {
            case NT_EQ: return value1 == value2;
            case NT_NE: return value1 != value2;
            case NT_LT: return value1 <  value2;
            case NT_LE: return value1 <= value2;
            case NT_GT: return value1 >  value2;
            case NT_GE: return value1 >= value2;

            default: return Object();  // empty -- shouldn't get here
        }
    }
    else
    {
        float value1 = instanceof(operand1, int) ? cast(operand1, int)
                                                 : cast(operand1, float);
        float value2 = instanceof(operand2, int) ? cast(operand2, int)
                                                 : cast(operand2, float);

        // Float operands.
        switch (node_type)
        {
            case NT_EQ: return value1 == value2;
            case NT_NE: return value1 != value2;
            case NT_LT: return value1 <  value2;
            case NT_LE: return value1 <= value2;
            case NT_GT: return value1 >  value2;
            case NT_GE: return value1 >= value2;

            default: return Object();  // empty -- shouldn't get here
        }
    }
}

}}}}  // namespace wci::backend::interpreter::executors
