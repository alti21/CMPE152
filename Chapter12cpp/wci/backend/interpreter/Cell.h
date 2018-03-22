/**
 * <h1>Cell</h1>
 *
 * <p>Interface for the interpreter's runtime memory cell.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef CELL_H_
#define CELL_H_

#include <string>
#include "MemoryMap.h"
#include "../../Object.h"

namespace wci { namespace backend { namespace interpreter {

using namespace std;
using namespace wci;

class Cell
{
public:
    /**
     * Destructor.
     */
    virtual ~Cell() {}

    /**
     * Defined by an implementation subclass.
     * @return the value in the cell.
     */
    virtual Object get_value() const = 0;

    /**
     * Set a new value into the cell.
     * Defined by an implementation subclass.
     * @param new_value the new value.
     */
    virtual void set_value(Object new_value) = 0;
};

}}}  // namespace wci::backend::interpreter

#endif /* CELL_H_ */
