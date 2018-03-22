/**
 * <h1>BackendFactory</h1>
 *
 * <p>A factory class that creates compiler and interpreter components.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include "BackendFactory.h"
#include "compiler/CodeGenerator.h"
#include "interpreter/Executor.h"
#include "../intermediate/symtabimpl/Predefined.h"

namespace wci { namespace backend {

using namespace std;
using namespace wci::backend::compiler;
using namespace wci::backend::interpreter;
using namespace wci::intermediate::symtabimpl;

Backend *BackendFactory::create_backend(string operation) throw (string)
{
    if (operation == "compile")
    {
        return new CodeGenerator();
    }
    else if (operation == "execute")
    {
        return new Executor();
    }
    else
    {
        throw new string("Backend factory: Invalid operation '" +
                         operation + "'");
    }
}

Object BackendFactory::default_value(TypeSpec *typespec)
{
    typespec = typespec->base_type();

    if (typespec == Predefined::integer_type)
    {
        return 0;
    }
    else if (typespec == Predefined::real_type)
    {
        return 0.0f;
    }
    else if (typespec == Predefined::boolean_type)
    {
        return false;
    }
    else if (typespec == Predefined::char_type)
    {
        return '#';
    }
    else // string
    {
        return "#";
    }
}

}} // namespace wci::backend
