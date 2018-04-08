/**
 * <h1>MemoryFactory</h1>
 *
 * <p>A factory class that creates runtime components.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef MEMORYFACTORY_H_
#define MEMORYFACTORY_H_

#include "RuntimeStack.h"
#include "RuntimeDisplay.h"
#include "ActivationRecord.h"
#include "MemoryMap.h"
#include "Cell.h"
#include "../../intermediate/SymTabEntry.h"

namespace wci { namespace backend { namespace interpreter {

using namespace std;
using namespace wci::intermediate;

class MemoryFactory
{
public:
    /**
     * Create a runtime stack.
     * @return the new runtime stack.
     */
    static RuntimeStack *create_runtime_stack();

    /**
     * Create a runtime display.
     * @return the new runtime display.
     */
    static RuntimeDisplay *create_runtime_display();

    /**
     * Create an activation record for a routine.
     * @param routine_id the symbol table entry of the routine's name.
     * @return the new activation record.
     */
    static ActivationRecord *create_activation_record
                                    (SymTabEntry *routine_id);

    /**
     * Create a memory map from a symbol table.
     * @param symtab the symbol table.
     * @return the new memory map.
     */
    static MemoryMap *create_memory_map(SymTab *symtab);

    /**
     * Create a memory cell.
     * @param value the value for the cell.
     * @return the new memory cell.
     */
    static Cell *create_cell(Object value);
};

}}}  // namespace wci::backend::interpreter

#endif /* MEMORYFACTORY_H_ */
