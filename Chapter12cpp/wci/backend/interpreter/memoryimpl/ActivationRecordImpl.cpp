/**
 * <h1>ActivationRecord</h1>
 *
 * <p>Interface for the interpreter's runtime activation record.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include <vector>
#include "ActivationRecordImpl.h"
#include "../ActivationRecord.h"
#include "../Cell.h"
#include "../MemoryFactory.h"
#include "../../../intermediate/SymTabEntry.h"
#include "../../../intermediate/symtabimpl/SymTabEntryImpl.h"

namespace wci { namespace backend { namespace interpreter { namespace memoryimpl {

using namespace std;
using namespace wci;
using namespace wci::intermediate;
using namespace wci::intermediate::symtabimpl;

ActivationRecordImpl::ActivationRecordImpl(SymTabEntry *routine_id)
    : link(nullptr), routine_id(routine_id)
{
    Object attribute =
            routine_id->get_attribute((SymTabKey) ROUTINE_SYMTAB);
    SymTab *symtab = cast(attribute, SymTab*);

    nesting_level = symtab->get_nesting_level();
    memory_map = MemoryFactory::create_memory_map(symtab);
}

ActivationRecordImpl::~ActivationRecordImpl()
{
    delete memory_map;
}

SymTabEntry *ActivationRecordImpl::get_routine_id() const
{
    return routine_id;
}

Cell *ActivationRecordImpl::get_cell(string name) const
{
    return memory_map->get_cell(name);
}

vector<string> ActivationRecordImpl::get_all_names() const
{
    return memory_map->get_all_names();
}

int ActivationRecordImpl::get_nesting_level() const
{
    return nesting_level;
}

ActivationRecord *ActivationRecordImpl::linked_to() const
{
    return link;
}

ActivationRecord *ActivationRecordImpl::make_link_to(ActivationRecord *ar)
{
    link = ar;
    return this;
}

}}}}  // namespace wci::backend::interpreter::memoryimpl
