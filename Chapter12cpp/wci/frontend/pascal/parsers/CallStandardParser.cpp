/**
 * <h1>DeclaredStandardParser</h1>
 *
 * <p>Parse a called to a standard procedure or function.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include "CallStandardParser.h"
#include "CallParser.h"
#include "StatementParser.h"
#include "ExpressionParser.h"
#include "../PascalToken.h"
#include "../PascalError.h"
#include "../../../frontend/Token.h"
#include "../../../frontend/pascal/PascalParserTD.h"
#include "../../../frontend/pascal/parsers/ExpressionParser.h"
#include "../../../intermediate/symtabimpl/SymTabEntryImpl.h"
#include "../../../intermediate/symtabimpl/Predefined.h"
#include "../../../intermediate/ICodeNode.h"
#include "../../../intermediate/ICodeFactory.h"
#include "../../../intermediate/icodeimpl/ICodeNodeImpl.h"
#include "../../../intermediate/icodeimpl/ICodeImpl.h"
#include "../../../intermediate/typeimpl/TypeSpecImpl.h"
#include "../../../intermediate/typeimpl/TypeChecker.h"

namespace wci { namespace frontend { namespace pascal { namespace parsers {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::pascal;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;
using namespace wci::intermediate::icodeimpl;
using namespace wci::intermediate::typeimpl;

CallStandardParser::CallStandardParser(PascalParserTD *parent)
    : CallParser(parent)
{
}

ICodeNode *CallStandardParser::parse_statement(Token *token) throw (string)
{
    ICodeNode *call_node =
            ICodeFactory::create_icode_node((ICodeNodeType) NT_CALL);
    string name = to_lower(token->get_text());
    SymTabEntry *pf_id = symtab_stack->lookup(name);
    Object attribute = pf_id->get_attribute((SymTabKey) ROUTINE_CODE);
    RoutineCode routine_code = cast(attribute, RoutineCode);
    call_node->set_attribute((ICodeKey) ID, pf_id);

    token = next_token(token); // consume procedure or function identifier

    switch ((RoutineCodeImpl) routine_code)
    {
        case RT_READ:
        case RT_READLN:  return parse_read_readln(token, call_node, pf_id);

        case RT_WRITE:
        case RT_WRITELN: return parse_write_writeln(token, call_node, pf_id);

        case RT_EOF:
        case RT_EOLN:    return parse_eof_eoln(token, call_node, pf_id);

        case RT_ABS:
        case RT_SQR:     return parse_abs_sqr(token, call_node, pf_id);

        case RT_ARCTAN:
        case RT_COS:
        case RT_EXP:
        case RT_LN:
        case RT_SIN:
        case RT_SQRT:    return parse_arctan_cos_exp_ln_sin_sqrt(
                                                token, call_node, pf_id);

        case RT_PRED:
        case RT_SUCC:    return parse_pred_succ(token, call_node, pf_id);

        case RT_CHR:     return parse_chr(token, call_node, pf_id);
        case RT_ODD:     return parse_odd(token, call_node, pf_id);
        case RT_ORD:     return parse_ord(token, call_node, pf_id);

        case RT_ROUND:
        case RT_TRUNC:   return parse_round_trunc(token, call_node, pf_id);

        default:         return nullptr;  // should never get here
    }
}

ICodeNode *CallStandardParser::parse_read_readln(Token *token,
                                                 ICodeNode *call_node,
                                                 SymTabEntry *pf_id)
    throw (string)
{
    // Parse any actual parameters.
    ICodeNode *parms_node = parse_actual_parameters(token, pf_id,
                                                    false, true, false);
    call_node->add_child(parms_node);

    // Read must have parameters.
    if (   (pf_id == Predefined::read_id)
        && (call_node->get_children().size() == 0))
    {
        error_handler.flag(token, WRONG_NUMBER_OF_PARMS, this);
    }

    return call_node;
}

ICodeNode *CallStandardParser::parse_write_writeln(Token *token,
                                                   ICodeNode *call_node,
                                                   SymTabEntry *pf_id)
    throw (string)
{
    // Parse any actual parameters.
    ICodeNode *parms_node = parse_actual_parameters(token, pf_id,
                                                    false, false, true);
    call_node->add_child(parms_node);

    // Write must have parameters.
    if (   (pf_id == Predefined::write_id)
        && (call_node->get_children().size() == 0))
    {
        error_handler.flag(token, WRONG_NUMBER_OF_PARMS, this);
    }

    return call_node;
}

ICodeNode *CallStandardParser::parse_eof_eoln(Token *token,
                                              ICodeNode *call_node,
                                              SymTabEntry *pf_id)
    throw (string)
{
    // Parse any actual parameters.
    ICodeNode *parms_node = parse_actual_parameters(token, pf_id,
                                                    false, false, false);
    call_node->add_child(parms_node);

    // There should be no actual parameters.
    if (check_parm_count(token, parms_node, 0))
    {
        call_node->set_typespec(Predefined::boolean_type);
    }

    return call_node;
}

ICodeNode *CallStandardParser::parse_abs_sqr(Token *token,
                                             ICodeNode *call_node,
                                             SymTabEntry *pf_id)
    throw (string)
{
    // Parse any actual parameters.
    ICodeNode *parms_node = parse_actual_parameters(token, pf_id,
                                                    false, false, false);
    call_node->add_child(parms_node);

    // There should be one integer or real parameter.
    // The function return type is the parameter type.
    if (check_parm_count(token, parms_node, 1)) {
        TypeSpec *arg_typespec =
            parms_node->get_children()[0]->get_typespec()->base_type();

        if (   (arg_typespec == Predefined::integer_type)
            || (arg_typespec == Predefined::real_type))
        {
            call_node->set_typespec(arg_typespec);
        }
        else
        {
            error_handler.flag(token, INVALID_TYPE, this);
        }
    }

    return call_node;
}

ICodeNode *CallStandardParser::parse_arctan_cos_exp_ln_sin_sqrt(
                Token *token, ICodeNode *call_node, SymTabEntry *pf_id)
    throw (string)
{
    // Parse any actual parameters.
    ICodeNode *parms_node = parse_actual_parameters(token, pf_id,
                                                false, false, false);
    call_node->add_child(parms_node);

    // There should be one integer or real parameter.
    // The function return type is real.
    if (check_parm_count(token, parms_node, 1))
    {
        TypeSpec *arg_typespec =
            parms_node->get_children()[0]->get_typespec()->base_type();

        if (   (arg_typespec == Predefined::integer_type)
            || (arg_typespec == Predefined::real_type))
        {
            call_node->set_typespec(Predefined::real_type);
        }
        else
        {
            error_handler.flag(token, INVALID_TYPE, this);
        }
    }

    return call_node;
}

ICodeNode *CallStandardParser::parse_pred_succ(Token *token,
                                               ICodeNode *call_node,
                                               SymTabEntry *pf_id)
    throw (string)
{
    // Parse any actual parameters.
    ICodeNode *parms_node = parse_actual_parameters(token, pf_id,
                                                    false, false, false);
    call_node->add_child(parms_node);

    // There should be one integer or enumeration parameter.
    // The function return type is the parameter type.
    if (check_parm_count(token, parms_node, 1))
    {
        TypeSpec *arg_typespec =
            parms_node->get_children()[0]->get_typespec()->base_type();

        if (   (arg_typespec == Predefined::integer_type)
            || (arg_typespec->get_form() == (TypeForm) TF_ENUMERATION))
        {
            call_node->set_typespec(arg_typespec);
        }
        else
        {
            error_handler.flag(token, INVALID_TYPE, this);
        }
    }

    return call_node;
}

ICodeNode *CallStandardParser::parse_chr(Token *token, ICodeNode *call_node,
                         SymTabEntry *pf_id)
    throw (string)
{
    // Parse any actual parameters.
    ICodeNode *parms_node = parse_actual_parameters(token, pf_id,
                                                    false, false, false);
    call_node->add_child(parms_node);

    // There should be one integer parameter.
    // The function return type is character.
    if (check_parm_count(token, parms_node, 1))
    {
        TypeSpec *arg_typespec =
            parms_node->get_children()[0]->get_typespec()->base_type();

        if (arg_typespec == Predefined::integer_type)
        {
            call_node->set_typespec(Predefined::char_type);
        }
        else
        {
            error_handler.flag(token, INVALID_TYPE, this);
        }
    }

    return call_node;
}

ICodeNode *CallStandardParser::parse_odd(Token *token, ICodeNode *call_node,
                         SymTabEntry *pf_id)
    throw (string)
{
    // Parse any actual parameters.
    ICodeNode *parms_node = parse_actual_parameters(token, pf_id,
                                                    false, false, false);
    call_node->add_child(parms_node);

    // There should be one integer parameter.
    // The function return type is boolean.
    if (check_parm_count(token, parms_node, 1))
    {
        TypeSpec *arg_typespec =
            parms_node->get_children()[0]->get_typespec()->base_type();

        if (arg_typespec == Predefined::integer_type)
        {
            call_node->set_typespec(Predefined::boolean_type);
        }
        else {
            error_handler.flag(token, INVALID_TYPE, this);
        }
    }

    return call_node;
}

ICodeNode *CallStandardParser::parse_ord(Token *token, ICodeNode *call_node,
                         SymTabEntry *pf_id)
    throw (string)
{
    // Parse any actual parameters.
    ICodeNode *parms_node = parse_actual_parameters(token, pf_id,
                                                    false, false, false);
    call_node->add_child(parms_node);

    // There should be one character or enumeration parameter.
    // The function return type is integer.
    if (check_parm_count(token, parms_node, 1))
    {
        TypeSpec *arg_typespec =
            parms_node->get_children()[0]->get_typespec()->base_type();

        if (   (arg_typespec == Predefined::char_type)
            || (arg_typespec->get_form() == (TypeForm) TF_ENUMERATION))
        {
            call_node->set_typespec(Predefined::integer_type);
        }
        else
        {
            error_handler.flag(token, INVALID_TYPE, this);
        }
    }

    return call_node;
}

ICodeNode *CallStandardParser::parse_round_trunc(Token *token,
                                                 ICodeNode *call_node,
                                                 SymTabEntry *pf_id)
    throw (string)
{
    // Parse any actual parameters.
    ICodeNode *parms_node = parse_actual_parameters(token, pf_id,
                                                false, false, false);
    call_node->add_child(parms_node);

    // There should be one real parameter.
    // The function return type is integer.
    if (check_parm_count(token, parms_node, 1))
    {
        TypeSpec *arg_typespec =
            parms_node->get_children()[0]->get_typespec()->base_type();

        if (arg_typespec == Predefined::real_type)
        {
            call_node->set_typespec(Predefined::integer_type);
        }
        else
        {
            error_handler.flag(token, INVALID_TYPE, this);
        }
    }

    return call_node;
}

bool CallStandardParser::check_parm_count(Token *token,
                                          ICodeNode *parms_node,
                                          int count)
{
    if (   ((parms_node == nullptr) && (count == 0))
        || (parms_node->get_children().size() == count))
    {
        return true;
    }
    else
    {
        error_handler.flag(token, WRONG_NUMBER_OF_PARMS, this);
        return false;
    }
}

}}}}  // namespace wci::frontend::pascal::parsers
