/**
 * <h1>MemoryMap</h1>
 *
 * <p>Interface for the interpreter's runtime memory map.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef MEMORYMAP_H_
#define MEMORYMAP_H_

#include <string>
#include <vector>
#include "Cell.h"

namespace wci { namespace backend { namespace interpreter {

using namespace std;

class MemoryMap
{
public:
    /**
     * Return the memory cell with the given name.
     * Defined by an implementation subclass.
     * @param name the name.
     * @return the cell.
     */
    Cell *get_cell(const string name) const = 0;

    /**
     * Defined by an implementation subclass.
     * @return the list of all the names.
     */
    vector<string> get_all_names() const = 0;
};

}}}  // namespace wci::backend::interpreter

#endif /* MEMORYMAP_H_ */
