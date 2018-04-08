/**
 * <h1>ActivationRecord</h1>
 *
 * <p>Interface for the interpreter's runtime activation record.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef ACTIVATIONRECORD_H_
#define ACTIVATIONRECORD_H_

#include <string>
#include <vector>
#include "Cell.h"

namespace wci { namespace backend { namespace interpreter {

using namespace std;
using namespace wci;

class ActivationRecord
{
public:
    /**
     * Destructor.
     */
    virtual ~ActivationRecord() {}

    /**
     * Return the memory cell for the given name from the memory map.
     * Defined by an implementation subclass.
     * @param name the name.
     * @return the cell.
     */
    virtual Cell *get_cell(const string name) const = 0;

    /**
     * Defined by an implementation subclass.
     * @return the list of all the names in the memory map.
     */
    virtual vector<string> get_all_names() const = 0;

    /**
     * Getter.
     * Defined by an implementation subclass.
     * @return the scope nesting level.
     */
    virtual int get_nesting_level() const = 0;

    /**
     * Defined by an implementation subclass.
     * @return the activation record to which this record is dynamically linked.
     */
    virtual ActivationRecord *linked_to() const = 0;

    /**
     * Make a dynamic link from this activation record to another one.
     * Defined by an implementation subclass.
     * @param ar the activation record to link to.
     * @return this activation record.
     */
    virtual ActivationRecord *make_link_to(ActivationRecord *ar) = 0;
};

}}}  // namespace wci::backend::interpreter

#endif /* ACTIVATIONRECORD_H_ */
