/**
 * <h1>RuntimeDisplay</h1>
 *
 * <p>Interface for the interpreter's runtime display.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef RUNTIMEDISPLAY_H_
#define RUNTIMEDISPLAY_H_

#include "ActivationRecord.h"

namespace wci { namespace backend { namespace interpreter {

using namespace std;

class RuntimeDisplay
{
public:
    /**
     * Destructor.
     */
    virtual ~RuntimeDisplay() {}

    /**
     * Get the activation record at a given nesting level.
     * Defined by an implementation subclass.
     * @param nesting_level the nesting level.
     * @return the activation record.
     */
    virtual ActivationRecord *get_activation_record(const int nesting_level)
        const = 0;

    /**
     * Update the display for a call to a routine at a given nesting level.
     * Defined by an implementation subclass.
     * @param nesting_level the nesting level.
     * @param ar the activation record for the routine.
     */
    virtual void call_update(const int nesting_level,
                             ActivationRecord *ar) = 0;

    /**
     * Update the display for a return from a routine at a given nesting level.
     * Defined by an implementation subclass.
     * @param nestingLevel the nesting level.
     */
    virtual void return_update(const int nesting_level) = 0;
};

}}}  // namespace wci::backend::interpreter

#endif /* RUNTIMEDISPLAY_H_ */
