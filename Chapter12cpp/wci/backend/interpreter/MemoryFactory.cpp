/**
 * <h1>MemoryFactory</h1>
 *
 * <p>A factory class that creates runtime components.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include "MemoryFactory.h"
#include "RuntimeStack.h"
#include "RuntimeDisplay.h"
#include "ActivationRecord.h"
#include "MemoryMap.h"
#include "Cell.h"
#include "memoryimpl/RuntimeStackImpl.h"
#include "memoryimpl/RuntimeDisplayImpl.h"
#include "memoryimpl/ActivationRecordImpl.h"
#include "memoryimpl/MemoryMapImpl.h"
#include "memoryimpl/CellImpl.h"
#include "../../intermediate/SymTab.h"
#include "../../intermediate/SymTabEntry.h"
#include "../../Object.h"

namespace wci { namespace backend { namespace interpreter {

using namespace std;
using namespace wci;
using namespace wci::intermediate;
using namespace wci::backend::interpreter::memoryimpl;

/**
 * Create a runtime stack.
 * @return the new runtime stack.
 */
RuntimeStack *MemoryFactory::create_runtime_stack()
{
    return new RuntimeStackImpl();
}

/**
 * Create a runtime display.
 * @return the new runtime display.
 */
RuntimeDisplay *MemoryFactory::create_runtime_display()
{
    return new RuntimeDisplayImpl();
}

/**
 * Create an activation record for a routine.
 * @param routineId the symbol table entry of the routine's name.
 * @return the new activation record.
 */
ActivationRecord *MemoryFactory::create_activation_record(
                                                SymTabEntry *routine_id)
{
    return new ActivationRecordImpl(routine_id);
}

/**
 * Create a memory map from a symbol table.
 * @param value the value for the cell.
 * @return the new memory map.
 */
MemoryMap *MemoryFactory::create_memory_map(SymTab *symtab)
{
    return new MemoryMapImpl(symtab);
}

/**
 * Create a memory cell.
 * @param value the value for the cell.
 * @return the new memory cell.
 */
Cell *MemoryFactory::create_cell(Object value)
{
    return new CellImpl(value);
}

}}} // namespace wci::backend::interpreter
