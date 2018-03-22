/**
 * <h1>MemoryMapImpl</h1>
 *
 * <p>The interpreter's runtime memory map.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef MEMORYMAPIMPL_H_
#define MEMORYMAPIMPL_H_

#include <string>
#include <vector>
#include <map>
#include "../Cell.h"
#include "../MemoryMap.h"
#include "../../../intermediate/SymTab.h"
#include "../../../intermediate/TypeSpec.h"

namespace wci { namespace backend { namespace interpreter { namespace memoryimpl {

using namespace std;
using namespace wci;
using namespace wci::intermediate;
using namespace wci::backend::interpreter;

class MemoryMapImpl : public MemoryMap
{
public:
    /**
     * Default constructor.
     */
    MemoryMapImpl();

    /**
     * Constructor.
     * Create a memory map and allocate its memory cells
     * based on the entries in a symbol table.
     * @param symtab the symbol table.
     */
    MemoryMapImpl(SymTab *symtab);

    /**
     * Destructor.
     */
    virtual ~MemoryMapImpl();

    /*
     * Getter.
     * @return the memory map contents.
     */
    map<string, Cell *> *get_contents();

    /**
     * Return the memory cell with the given name.
     * @param name the name.
     * @return the cell.
     */
    Cell *get_cell(const string name);

    /**
     * @return the list of all the names.
     */
    vector<string> get_all_names();

private:
    map<string, Cell *> contents;

    /**
     * Make an allocation for a value of a given data type for a memory cell.
     * @param typespec the data type.
     * @return the allocation.
     */
    Object allocate_cell_value(TypeSpec *typespec);

    /**
     * Allocate the memory cells of an array.
     * @param typespec the array type.
     * @return the allocation.
     */
    Object allocate_array_cells(TypeSpec *typespec);

    /**
     * Allocate the memory map for a record.
     * @param typespec the record type.
     * @return the allocation.
     */
    MemoryMapImpl *allocate_record_map(TypeSpec *typespec);
};

}}}}  // namespace wci::backend::interpreter::memoryimpl

#endif /* MEMORYMAPIMPL_H_ */
