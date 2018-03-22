/**
 * <h1>CellImpl</h1>
 *
 * <p>The interpreter's runtime memory cell.</p>
 *
 * <p>Copyright (c) 2009 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef CELLIMPL_H_
#define CELLIMPL_H_

#include "../Cell.h"
#include "../../../Object.h"

namespace wci { namespace backend { namespace interpreter { namespace memoryimpl {

using namespace wci;

class CellImpl : public Cell
{
public:
    /**
     * Constructor.
     */
    CellImpl();

    /**
     * Constructor
     * @param value the value for the cell.
     */
    CellImpl(Object value);

    /**
     * Destructor.
     */
    ~CellImpl();

    /**
     * Getter.
     * @return the value in the cell.
     */
    Object get_value() const;

    /**
     * Setter.
     * @param new_value the new value.
     */
    void set_value(Object new_value);

private:
    Object cell_value;
};

}}}}  // namespace wci::backend::interpreter::memoryimpl

#endif /* CELLIMPL_H_ */
