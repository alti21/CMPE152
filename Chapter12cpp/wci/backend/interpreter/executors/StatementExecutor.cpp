/**
 * <h1>StatementExecutor</h1>
 *
 * <p>Execute a statement.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include "StatementExecutor.h"
#include "CompoundExecutor.h"
#include "AssignmentExecutor.h"
#include "LoopExecutor.h"
#include "IfExecutor.h"
#include "SelectExecutor.h"
#include "CallExecutor.h"
#include "../Executor.h"
#include "../Cell.h"
#include "../MemoryFactory.h"
#include "../RuntimeError.h"
#include "../memoryimpl/MemoryMapImpl.h"
#include "../../../Object.h"
#include "../../../intermediate/TypeSpec.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/symtabimpl/Predefined.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"
#include "../../../intermediate/typeimpl/TypeSpecImpl.h"
#include "../../../message/Message.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::backend::interpreter::memoryimpl;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::intermediate::icodeimpl;
using namespace wci::intermediate::typeimpl;
using namespace wci::message;

StatementExecutor::StatementExecutor(Executor *parent)
    : parent(parent)
{
}

Object StatementExecutor::execute(ICodeNode *node)
{
    ICodeNodeTypeImpl node_type = (ICodeNodeTypeImpl) node->get_type();

    // Send a message about the current source line.
    send_at_line_message(node);

    switch (node_type)
    {
        case NT_COMPOUND:
        {
            CompoundExecutor compound_executor(this);
            return compound_executor.execute(node);
        }

        case NT_ASSIGN:
        {
            AssignmentExecutor assignment_executor(this);
            return assignment_executor.execute(node);
        }

        case NT_LOOP:
        {
            LoopExecutor loop_executor(this);
            return loop_executor.execute(node);
        }

        case NT_IF:
        {
            IfExecutor if_executor(this);
            return if_executor.execute(node);
        }

        case NT_SELECT:
        {
            SelectExecutor select_executor(this);
            return select_executor.execute(node);
        }

        case NT_CALL:
        {
            CallExecutor call_executor(this);
            return call_executor.execute(node);
        }

        case NT_NO_OP: return Object();  // empty

        default:
        {
            error_handler.flag(node, UNIMPLEMENTED_FEATURE, this);
            return Object();  // empty
        }
    }
}

Object StatementExecutor::to_pascal(TypeSpec *target_typespec,
                                    Object java_value)
{
    if (java_value.empty()) return java_value;

    if (instanceof(java_value, string))
    {
        string value = cast(java_value, string);

        if (target_typespec == Predefined::char_type)
        {
            return value[0];  // Pascal character.
        }
        else if (target_typespec->is_pascal_string())
        {
            int length = value.length();
            vector<Cell *> *array = new vector<Cell *>;
            array->reserve(length);

            // Build an array of characters.
            for (int i = 0; i < length; ++i)
            {
                char ch = value[i];
                array->push_back(MemoryFactory::create_cell(ch));
            }

            return array;  // Pascal string (array of characters).
        }
    }

    return java_value;
}

Object StatementExecutor::to_java(TypeSpec *target_typespec,
                                  Object pascal_value,
                                  ICodeNode *node)
{
    if (   instanceof(pascal_value, vector<Cell *>*)
        && instanceof((*(cast(pascal_value, vector<Cell *>*)))[0]->get_value(), char))
    {
        vector<Cell *> *array = cast(pascal_value, vector<Cell *>*);
        int length = array->size();

        // Build a Java string.
        string str = "";
        for (int i = 0; i < length; i++)
        {
            char ch = cast((*array)[i]->get_value(), char);
            str += ch;
        }

        return str;  // C++ string
    }

    return pascal_value;
}

Object StatementExecutor::copy_of(Object cell_value, ICodeNode *node)
{
    if (cell_value.empty()) return Object();  // empty

    if (instanceof(cell_value, vector<Cell *>*))
    {
        return copy_array(cell_value, node);
    }
    else if (instanceof(cell_value, MemoryMap))
    {
        return copy_record(cell_value, node);
    }
    else
    {
        return cell_value;
    }
}

Object StatementExecutor::copy_array(Object array_value,
                                     ICodeNode *node)
{
    vector<Cell *> *copy_array = new vector<Cell *>;

    if (!array_value.empty())
    {
        vector<Cell *> *orig_array = cast(array_value, vector<Cell *>*);
        int size = orig_array->size();

        copy_array->reserve(size);
        for (int i = 0; i < size; i++)
        {
            Object copy_value = copy_of((*orig_array)[i]->get_value(), node);
            copy_array->push_back(MemoryFactory::create_cell(copy_value));
        }
    }
    else
    {
        error_handler.flag(node, UNINITIALIZED_VALUE, this);
    }

    return copy_array;
}

Object StatementExecutor::copy_record(Object record_value, ICodeNode *node)
{
    MemoryMapImpl *copy_map = new MemoryMapImpl();

    if (!record_value.empty())
    {
        MemoryMapImpl *orig_map = cast(record_value, MemoryMapImpl*);
        map<string, Cell *> *orig_contents = orig_map->get_contents();
        map<string, Cell *> *copy_contents = copy_map->get_contents();
        map<string, Cell *>::iterator it;

        for (it = orig_contents->begin(); it != orig_contents->end(); it++)
        {
            Object copy_value = copy_of(it->second->get_value(), node);
            Cell *copy_cell = MemoryFactory::create_cell(copy_value);
            pair<string, Cell *> pair(it->first, copy_cell);
            copy_contents->insert(pair);
        }
    }
    else
    {
        error_handler.flag(node, UNINITIALIZED_VALUE, this);
    }

    return copy_map;
}

Object StatementExecutor::check_range(
             ICodeNode *node, TypeSpec *typespec, Object cell_value)
{
    if (typespec->get_form() == (TypeForm) TF_SUBRANGE)
    {
        Object attribute;
        attribute = typespec->get_attribute((TypeKey) SUBRANGE_MIN_VALUE);
        int min_value = cast(attribute, int);
        attribute = typespec->get_attribute((TypeKey) SUBRANGE_MAX_VALUE);
        int max_value = cast(attribute, int);
        int value = cast(cell_value, int);

        if (value < min_value)
        {
            error_handler.flag(node, VALUE_RANGE, this);
            return min_value;
        }
        else if (value > max_value)
        {
            error_handler.flag(node, VALUE_RANGE, this);
            return max_value;
        }
    }

    return cell_value;
}

string StatementExecutor::display_value(const Object cell_value) const
{
    string value_str =
          cell_value.empty()                 ? "<?>"
        : instanceof(cell_value, Cell*)      ? "<reference>"
        : instanceof(cell_value, Cell**)     ? "<array>"
        : instanceof(cell_value, MemoryMap*) ? "<record>"
                                             : stringify(cell_value);
    return value_str;
}

void StatementExecutor::send_at_line_message(ICodeNode *node)
{
    Object attribute = node->get_attribute((ICodeKey) LINE_NUMBER);
    int line_number = cast(attribute, int);

    // Send the SOURCE_LINE message.
    Message message(AT_LINE,
                    LINE_NUMBER, to_string(line_number));
    send_message(message);
}

void StatementExecutor::send_assign_message(ICodeNode *node,
                                            string variable_name,
                                            Object cell_value)
{
    int line_number = get_line_number(node);

    // Send an ASSIGN message.
    string value_str = display_value(cell_value);
    Message message(ASSIGN,
                    LINE_NUMBER, to_string(line_number),
                    VARIABLE_NAME, variable_name,
                    RESULT_VALUE, value_str);
    send_message(message);
}

void StatementExecutor::send_fetch_message(ICodeNode *node,
                                           string variable_name,
                                           Object cell_value)
{
    int line_number = get_line_number(node);

    // Send a FETCH message.
    string value_str = display_value(cell_value);
    Message message(FETCH,
                    LINE_NUMBER, to_string(line_number),
                    VARIABLE_NAME, variable_name,
                    RESULT_VALUE, value_str);
    send_message(message);
}

void StatementExecutor::send_call_message(ICodeNode *node,
                                          string routine_name)
{
    int line_number = get_line_number(node);

    // Send a CALL message.
    Message message(CALL,
                    LINE_NUMBER, to_string(line_number),
                    VARIABLE_NAME, routine_name);
    send_message(message);
}

void StatementExecutor::send_return_message(ICodeNode *node,
                                            string routine_name)
{
    int line_number = get_line_number(node);

    // Send a RETURN message.
    Message message(RETURN,
                    LINE_NUMBER, to_string(line_number),
                    VARIABLE_NAME, routine_name);
    send_message(message);
}

int StatementExecutor::get_line_number(ICodeNode *node)
{
    // Go up the parent links to look for a line number.
    while ((node != nullptr) &&
           (node->get_attribute((ICodeKey) LINE).empty()))
    {
        node = node->get_parent();
    }

    return cast(node->get_attribute((ICodeKey) LINE), int);
}

}}}}  // namespace wci::backend::interpreter::executors
