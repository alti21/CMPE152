/**
 * <h1>StatementExecutor</h1>
 *
 * <p>Execute a statement.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef STATEMENTEXECUTOR_H_
#define STATEMENTEXECUTOR_H_

#include <string>
#include <map>
#include "../Executor.h"
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

class StatementExecutor : public Executor
{
public:
    /**
     * Constructor.
     * @param the parent executor.
     */
    StatementExecutor(Executor *parent);

    /**
     * Execute a statement.
     * To be overridden by the specialized statement executor subclasses.
     * @param node the root node of the statement.
     * @return an empty Object.
     */
    virtual Object execute(ICodeNode *node);

protected:
    Executor *parent;  // parent executor

    /**
     * Convert a Java string to a Pascal string or character.
     * @param target_typespec the target type specification.
     * @param java_value the Java string.
     * @return the Pascal string or character.
     */
    Object to_pascal(TypeSpec *target_typespec, Object java_value);

    /**
     * Convert a Pascal string to a Java string.
     * @param target_typespec the target type specification
     * @param pascal_value the Pascal string.
     * @param node the statement node.
     * @return the Java string.
     */
    Object to_java(TypeSpec *target_typespec, Object pascal_value,
                       ICodeNode *node);

    /**
     * Return a copy of a Pascal value.
     * @param cell_value the value.
     * @param node the statement node.
     * @return the copy.
     */
    Object copy_of(Object cell_value, ICodeNode *node);

    /**
     * Runtime range check.
     * @param node the root node of the expression subtree to check.
     * @param typespec the target type specification.
     * @param cell_value the value.
     * @return the value to use.
     */
    Object check_range(ICodeNode *node, TypeSpec *typespec,
                       Object cell_value);

    /**
     * Send a message about an assignment operation.
     * @param node the parse tree node.
     * @param variable_name the name of the target variable.
     * @param cell_value the value of the expression.
     */
    void send_assign_message(ICodeNode *node, string variable_name,
                             Object cell_value);

    /**
     * Send a message about a value fetch operation.
     * @param node the parse tree node.
     * @param variable_name the name of the variable.
     * @param cell_value the value of the expression.
     */
    void send_fetch_message(ICodeNode *node, string variable_name,
                            Object cell_value);

    /**
     * Send a message about a call to a declared procedure or function.
     * @param node the parse tree node.
     * @param routine_name the name of the routine.
     */
    void send_call_message(ICodeNode *node, string routine_name);

    /**
     * Send a message about a return from a declared procedure or function.
     * @param node the parse tree node.
     * @param routine_name the name of the routine.
     */
    void send_return_message(ICodeNode *node, string routine_name);

private:
    /**
     * Return a copy of a Pascal array.
     * @param cell_value the cell array.
     * @param node the statement node.
     * @return the copy of the array cells.
     */
    Object copy_array(Object cell_value, ICodeNode *node);

    /**
     * Return a copy of a Pascal record.
     * @param record_value the record value.
     * @param node the statement node.
     * @return the copy of the hashmap.
     */
    Object copy_record(Object record_value, ICodeNode *node);

    /**
     * Convert a cell value to a display string.
     */
    string display_value(const Object cell_value) const;

    /**
     * Send a message about the current source line.
     * @param node the statement node.
     */
    void send_at_line_message(ICodeNode *node);

    /**
     * Get the source line number of a parse tree node.
     * @param node the parse tree node.
     * @return the line number.
     */
    int get_line_number(ICodeNode *node);
};

}}}}  // namespace wci::backend::interpreter::executors

#endif /* STATEMENTEXECUTOR_H_ */
