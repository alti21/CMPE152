/**
 * <h1>CallStandardExecutor</h1>
 *
 * <p>Execute a call a standard procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef CALLSTANDARDEXECUTOR_H_
#define CALLSTANDARDEXECUTOR_H_

#include "CallExecutor.h"
#include "ExpressionExecutor.h"
#include "../Cell.h"
#include "../../../intermediate/ICodeNode.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::backend::interpreter;
using namespace wci::intermediate;

class CallStandardExecutor : public CallExecutor
{
public:
    /**
     * Constructor.
     * @param the parent executor.
     */
    CallStandardExecutor(Executor *parent);

    /**
     * Execute procedure or function call statement.
     * @param node the root node of the call.
     * @return nullptr.
     */
    Object execute(ICodeNode *node);

private:
    /**
     * Execute a call to read or readln.
     * @param call_node the CALL node.
     * @param routine_code the routine code.
     * @return nullptr.
     */
    Object execute_read_readln(ICodeNode *call_node,
                                   RoutineCode routine_code);

    /**
     * Parse an integer or real value from the standard input.
     * @param token the current input token.
     * @param typespec the input value type.
     * @return the integer or real value.
     * @throw a string message if an error occurred.
     */
    Object parse_number(Token *token, TypeSpec *typespec) throw (string);

    /**
     * Parse a boolean value from the standard input.
     * @param token the current input token.
     * @param typespec the input value type.
     * @return the boolean value.
     * @throw a string message if an error occurred.
     */
    Object parse_boolean(Token *token) throw (string);

    /**
     * Execute a call to write or writeln.
     * @param call_node the CALL node.
     * @param routine_code the routine code.
     * @return nullptr.
     */
    Object execute_write_writeln(ICodeNode *call_node,
                                     RoutineCode routine_code);

    /**
     * Execute a call to eof or eoln.
     * @param call_node the CALL node.
     * @param routine_code the routine code.
     * @return true or false.
     */
    Object execute_eof_eoln(ICodeNode *call_node,
                                RoutineCode routine_code);

    /**
     * Execute a call to abs or sqr.
     * @param call_node the CALL node.
     * @param routine_code the routine code.
     * @param actual_node the actual parameter node.
     * @return the function value.
     */
    Object execute_abs_sqr(ICodeNode *call_node,
                               RoutineCode routine_code,
                               ICodeNode *actual_node);

    /**
     * Execute a call to arctan, cos, exp, ln, sin, or sqrt.
     * @param call_node the CALL node.
     * @param routine_code the routine code.
     * @param actual_node the actual parameter node.
     * @return the function value.
     */
    Object execute_arctan_cos_exp_ln_sin_sqrt(ICodeNode *call_node,
                                                  RoutineCode routine_code,
                                                  ICodeNode *actual_node);

    /**
     * Execute a call to pred or succ.
     * @param call_node the CALL node.
     * @param routine_code the routine code.
     * @param actual_node the actual parameter node.
     * @param typespec the value type.
     * @return the function value.
     */
    Object execute_pred_succ(ICodeNode *call_node,
                                 RoutineCode routine_code,
                                 ICodeNode *actual_node,
                                 TypeSpec *typespec);

    /**
     * Execute a call to chr.
     * @param call_node the CALL node.
     * @param routine_code the routine code.
     * @param actual_node the actual parameter node.
     * @return the function value.
     */
    Object execute_chr(ICodeNode *call_node, RoutineCode routine_code,
                           ICodeNode *actual_node);

    /**
     * Execute a call to odd.
     * @param call_node the CALL node.
     * @param routine_code the routine code.
     * @param actual_node the actual parameter node.
     * @return true or false.
     */
    Object execute_odd(ICodeNode *call_node, RoutineCode routine_code,
                           ICodeNode *actual_node);

    /**
     * Execute a call to ord.
     * @param call_node the CALL node.
     * @param routine_code the routine code.
     * @param actual_node the actual parameter node.
     * @return the function value.
     */
    Object execute_ord(ICodeNode *call_node, RoutineCode routine_code,
                           ICodeNode *actual_node);

    /**
     * Execute a call to round or trunc.
     * @param call_node the CALL node.
     * @param routine_code the routine code.
     * @param actual_node the actual parameter node.
     * @return the function value.
     */
    Object execute_round_trunc(ICodeNode *call_node,
                                   RoutineCode routine_code,
                                   ICodeNode *actual_node);
};

}}}}  // namespace wci::backend::interpreter::executors

#endif /* CALLSTANDARDEXECUTOR_H_ */
