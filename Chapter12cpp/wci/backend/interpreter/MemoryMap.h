/**
 * <h1>MemoryMap</h1>
 *
 * <p>Interface for the interpreter's runtime memory map.</p>
 *
 * <p>Copyright (c) 2009 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef MEMORYMAP_H_
#define MEMORYMAP_H_

#include <string>
#include <vector>

namespace wci { namespace backend { namespace interpreter {

using namespace std;
using namespace wci;

class Cell;  // forward reference

class MemoryMap
{
public:
    /**
     * Destructor.
     */
    virtual ~MemoryMap() {}

    /**
     * Return the memory cell with the given name.
     * Defined by an implementation subclass.
     * @param name the name.
     * @return the cell.
     */
    virtual Cell *get_cell(const string name) = 0;

    /**
     * Defined by an implementation subclass.
     * @return the list of all the names.
     */
    virtual vector<string> get_all_names() = 0;
};

}}}  // namespace wci::backend::interpreter

#endif /* MEMORYMAP_H_ */
