/**
 * <h1>CallStandardExecutor</h1>
 *
 * <p>Execute a call a standard procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <iostream>
#include <vector>
#include <math.h>
#include "CallStandardExecutor.h"
#include "CallExecutor.h"
#include "StatementExecutor.h"
#include "AssignmentExecutor.h"
#include "ExpressionExecutor.h"
#include "../Cell.h"
#include "../MemoryFactory.h"
#include "../../BackendFactory.h"
#include "../../../Object.h"
#include "../../../frontend/Token.h"
#include "../../../frontend/pascal/PascalToken.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/SymTabEntry.h"
#include "../../../intermediate/symtabimpl/SymTabEntryImpl.h"
#include "../../../intermediate/symtabimpl/Predefined.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"

namespace wci { namespace backend { namespace interpreter { namespace executors {

using namespace std;
using namespace wci;
using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::intermediate::icodeimpl;
using namespace wci::backend::interpreter;

CallStandardExecutor::CallStandardExecutor(Executor *parent)
    : CallExecutor(parent)
{
}

Object CallStandardExecutor::execute(ICodeNode *node)
{
    Object attribute = node->get_attribute((ICodeKey) ID);
    SymTabEntry *routine_id = cast(attribute, SymTabEntry*);
    attribute = routine_id->get_attribute((SymTabKey) ROUTINE_CODE);
    RoutineCode routine_code = cast(attribute, RoutineCode);
    TypeSpec *typespec = node->get_typespec();
    ExpressionExecutor expression_executor(this);
    ICodeNode *actual_node = nullptr;

    // Get the actual parameters of the call.
    if (node->get_children().size() > 0)
    {
        ICodeNode *parms_node = node->get_children()[0];
        actual_node = parms_node->get_children()[0];
    }

    switch ((RoutineCodeImpl) routine_code)
    {
        case RT_READ:
        case RT_READLN:  return execute_read_readln(node, routine_code);

        case RT_WRITE:
        case RT_WRITELN: return execute_write_writeln(node, routine_code);

        case RT_EOF:
        case RT_EOLN:    return execute_eof_eoln(node, routine_code);

        case RT_ABS:
        case RT_SQR:     return execute_abs_sqr(node, routine_code,
                                                actual_node);

        case RT_ARCTAN:
        case RT_COS:
        case RT_EXP:
        case RT_LN:
        case RT_SIN:
        case RT_SQRT:    return execute_arctan_cos_exp_ln_sin_sqrt(
                                        node, routine_code, actual_node);

        case RT_PRED:
        case RT_SUCC:    return execute_pred_succ(
                              node, routine_code, actual_node, typespec);

        case RT_CHR: return execute_chr(node, routine_code, actual_node);
        case RT_ODD: return execute_odd(node, routine_code, actual_node);
        case RT_ORD: return execute_ord(node, routine_code, actual_node);

        case RT_ROUND:
        case RT_TRUNC:   return execute_round_trunc(node, routine_code,
                                                    actual_node);

        default:      return Object();  // empty -- should never get here
    }
}

Object CallStandardExecutor::execute_read_readln(
                        ICodeNode *call_node, RoutineCode routine_code)
{
    ICodeNode *parms_node = call_node->get_children().size() > 0
                              ? call_node->get_children()[0]
                              : nullptr;
    ExpressionExecutor expression_executor(this);

    if (parms_node != nullptr)
    {
        vector<ICodeNode *> actuals = parms_node->get_children();

        // Loop to process each actual parameter.
        for (ICodeNode *actual_node : actuals)
        {
            TypeSpec *typespec = actual_node->get_typespec();
            TypeSpec *base_type = typespec->base_type();
            Cell *variable_cell =
                    expression_executor.execute_variable(actual_node);
            Object attribute = actual_node->get_attribute((ICodeKey) ID);
            SymTabEntry *actual_id = cast(attribute, SymTabEntry*);
            Object cell_value;
            Token *token;

            // Read a value of the appropriate typespec
            // from the standard input.
            try
            {
                if (base_type == Predefined::integer_type)
                {
                    token = std_in->next_token(nullptr);
                    cell_value = parse_number(token, base_type);
                }
                else if (base_type == Predefined::real_type)
                {
                    token = std_in->next_token(nullptr);
                    cell_value = parse_number(token, base_type);
                }
                else if (base_type == Predefined::boolean_type)
                {
                    token = std_in->next_token(nullptr);
                    cell_value = parse_boolean(token);
                }
                else if (base_type == Predefined::char_type)
                {
                    char ch = std_in->next_char();
                    if (   (ch == Source::END_OF_LINE)
                        || (ch == Source::END_OF_FILE)) ch = ' ';
                    cell_value = ch;
                }
                else throw string("");
            }
            catch (string& str)
            {
                error_handler.flag(call_node, INVALID_INPUT, this);
                cell_value = BackendFactory::default_value(typespec);
            }

            AssignmentExecutor assignment_executor(this);
            assignment_executor.assign_value(call_node, actual_id,
                                             variable_cell, typespec,
                                             cell_value, base_type);
        }
    }

    // Skip the rest of the input line for readln.
    if (routine_code == (RoutineCode) RT_READLN)
    {
        try
        {
            std_in->skip_to_next_line();
        }
        catch (string& str)
        {
            error_handler.flag(call_node, INVALID_INPUT, this);
        }
    }

    return Object();  // empty
}

Object CallStandardExecutor::parse_number(Token *token,
                                          TypeSpec *typespec)
    throw (string)
{
    TokenType token_type = token->get_type();
    bool plus_sign    = token_type == (TokenType) PT_PLUS;
    bool minus_sign   = token_type == (TokenType) PT_MINUS;
    bool leading_sign = plus_sign || minus_sign;

    // Leading sign?
    if (leading_sign)
    {
        token = std_in->next_token(token);  // consume sign
        token_type = token->get_type();
    }

    // Integer value.
    if (token_type == (TokenType) PT_INTEGER)
    {
        int value = cast(token->get_value(), int);

        if (minus_sign) value = -value;

        delete token;

        if (typespec == Predefined::integer_type) return value;
        else                                      return (float) value;
    }

    // Real value.
    else if (   (token_type == (TokenType) PT_REAL)
             && (typespec == Predefined::real_type))
    {
        float value = cast(token->get_value(), float);
        if (minus_sign) value = -value;

        delete token;
        return value;
    }

    // Bad input.
    else
    {
        delete token;
        throw string("");
    }
}

Object CallStandardExecutor::parse_boolean(Token *token) throw (string)
{
    if (token->get_type() == (TokenType) PT_IDENTIFIER)
    {
        string text = to_lower(token->get_text());
        delete token;

        if      (text == "true")  return true;
        else if (text == "false") return false;
        else throw string("");
    }
    else throw string("");
}

Object CallStandardExecutor::execute_write_writeln(
                        ICodeNode *call_node, RoutineCode routine_code)
{
    ICodeNode *parms_node = call_node->get_children().size() > 0
                                          ? call_node->get_children()[0]
                                          : nullptr;
    ExpressionExecutor expression_executor(this);

    if (parms_node != nullptr)
    {
        vector<ICodeNode *> actuals = parms_node->get_children();

        // Loop to process each WRITE_PARM actual parameter node.
        for (ICodeNode *write_parm_node : actuals)
        {
            vector<ICodeNode *> children = write_parm_node->get_children();
            ICodeNode *expr_node = children[0];
            TypeSpec *typespec = expr_node->get_typespec()->base_type();
            string type_code = typespec->is_pascal_string()    ? "s"
                        : typespec == Predefined::integer_type ? "d"
                        : typespec == Predefined::real_type    ? "f"
                        : typespec == Predefined::boolean_type ? "s"
                        : typespec == Predefined::char_type    ? "c"
                        :                                        "s";

            Object value = expression_executor.execute(expr_node);

            if (   (typespec == Predefined::char_type)
                && (instanceof(value, string)))
            {
                value = cast(value, string)[0];
            }

            // C++ format string.
            string format = "%";

            // Process any field width and precision values.
            if (children.size() > 1)
            {
                Object attribute =
                        children[1]->get_attribute((ICodeKey) VALUE);
                int w = cast(attribute, int);
                format += to_string(w == 0 ? 1 : w);
            }
            if (children.size() > 2)
            {
                Object attribute =
                    children[2]->get_attribute((ICodeKey) VALUE);
                int p = cast(attribute, int);
                format += "." + to_string(p == 0 ? 1 : p);
            }

            format += type_code;

            // Write the formatted value to the standard output.
            switch (type_code[0])
            {
                case 'd': printf(format.c_str(), cast(value, int)); break;
                case 'f': printf(format.c_str(), cast(value, float)); break;
                case 'c': printf(format.c_str(), cast(value, char)); break;

                case 's':
                {
                    string str;

                    if (typespec == Predefined::boolean_type)
                    {
                        str = cast(value, bool) ? "true" : "false";
                    }
                    else
                    {
                        str = cast(value, string);
                    }

                    printf(format.c_str(), str.c_str());
                    break;
                }
            }

            cout.flush();
        }
    }

    // Line feed for writeln.
    if (routine_code == (RoutineCode) RT_WRITELN)
    {
        cout << endl;
        cout.flush();
    }

    return nullptr;
}

Object CallStandardExecutor::execute_eof_eoln(
                        ICodeNode *call_node, RoutineCode routine_code)
{
    try
    {
        return routine_code == (RoutineCode) RT_EOF
                                    ? std_in->at_eof()
                                    : std_in->at_eol();
    }
    catch (string& str)
    {
        error_handler.flag(call_node, INVALID_INPUT, this);
        return true;
    }
}

Object CallStandardExecutor::execute_abs_sqr(ICodeNode *call_node,
                                             RoutineCode routine_code,
                                             ICodeNode *actual_node)
{
    ExpressionExecutor expression_executor(this);
    Object arg_value = expression_executor.execute(actual_node);

    if (instanceof(arg_value, int))
    {
        int value = cast(arg_value, int);
        return routine_code == (RoutineCode) RT_ABS ? abs(value)
                                                    : value*value;
    }
    else
    {
        float value = cast(arg_value, float);
        return routine_code == (RoutineCode) RT_ABS ? abs(value)
                                                    : value*value;
    }
}

Object CallStandardExecutor::execute_arctan_cos_exp_ln_sin_sqrt(
                                                ICodeNode *call_node,
                                                RoutineCode routine_code,
                                                ICodeNode *actual_node)
{
    ExpressionExecutor expression_executor(this);
    Object arg_value = expression_executor.execute(actual_node);
    float value = instanceof(arg_value, int)
                                    ? (float) cast(arg_value, int)
                                    :         cast(arg_value, float);

    switch ((RoutineCodeImpl) routine_code)
    {
        case RT_ARCTAN: return tan(value);
        case RT_COS:    return cos(value);
        case RT_EXP:    return exp(value);
        case RT_SIN:    return sin(value);

        case RT_LN:
        {
            if (value > 0.0f) return log(value);
            else
            {
                error_handler.flag(call_node,
                                   INVALID_STANDARD_FUNCTION_ARGUMENT,
                                   this);
                return 0.0f;
            }
        }

        case RT_SQRT:
        {
            if (value >= 0.0f) return sqrt(value);
            else
            {
                error_handler.flag(call_node,
                                   INVALID_STANDARD_FUNCTION_ARGUMENT,
                                   this);
                return 0.0f;
            }
        }

        default: return 0.0f;  // should never get here
    }
}

Object CallStandardExecutor::execute_pred_succ(ICodeNode *call_node,
                                               RoutineCode routine_code,
                                               ICodeNode *actual_node,
                                               TypeSpec *typespec)
{
    ExpressionExecutor expression_executor(this);
    Object arg_value = expression_executor.execute(actual_node);
    int value = cast(arg_value, int);
    int new_value = routine_code == (RoutineCode) RT_PRED
                                        ? --value : ++value;

    arg_value = check_range(call_node, typespec, new_value);
    return new_value;
}

Object CallStandardExecutor::execute_chr(ICodeNode *call_node,
                                         RoutineCode routine_code,
                                         ICodeNode *actual_node)
{
    ExpressionExecutor expression_executor(this);
    Object arg_value = expression_executor.execute(actual_node);
    char ch = cast(arg_value, int);
    return ch;
}

Object CallStandardExecutor::execute_odd(ICodeNode *call_node,
                                         RoutineCode routine_code,
                                         ICodeNode *actual_node)
{
    ExpressionExecutor expression_executor(this);
    Object arg_value = expression_executor.execute(actual_node);
    int value = cast(arg_value, int);
    return (value & 1) == 1;
}

Object CallStandardExecutor::execute_ord(ICodeNode *call_node,
                                         RoutineCode routine_code,
                                         ICodeNode *actual_node)
{
    ExpressionExecutor expression_executor(this);
    Object arg_value = expression_executor.execute(actual_node);

    if (instanceof(arg_value, char))
    {
        char ch = cast(arg_value, char);
        return (int) ch;
    }
    else if (instanceof(arg_value, string))
    {
        char ch = cast(arg_value, string)[0];
        return (int) ch;
    }
    else
    {
        return arg_value;
    }
}

Object CallStandardExecutor::execute_round_trunc(ICodeNode *call_node,
                                                 RoutineCode routine_code,
                                                 ICodeNode *actual_node)
{
    ExpressionExecutor expression_executor(this);
    Object arg_value = expression_executor.execute(actual_node);
    float value = cast(arg_value, float);

    if (routine_code == (RoutineCode) RT_ROUND)
    {
        return value >= 0.0f ? (int) (value + 0.5f) : (int) (value - 0.5f);
    }
    else
    {
        return (int) value;
    }
}

}}}}  // namespace wci::backend::interpreter::executors
