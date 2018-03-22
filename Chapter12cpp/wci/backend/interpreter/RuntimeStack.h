/**
 * <h1>RuntimeStack</h1>
 *
 * <p>Interface for the interpreter's runtime stack.</p>
 *
 * <p>Copyright (c) 2009 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef RUNTIMESTACK_H_
#define RUNTIMESTACK_H_

#include <string>
#include <vector>
#include "ActivationRecord.h"

namespace wci { namespace backend { namespace interpreter {

using namespace std;

class RuntimeStack
{
public:
    /**
     * Destructor.
     */
    virtual ~RuntimeStack() {}

    /**
     * Defined by an implementation subclass.
     * @return an array list of the activation records on the stack.
     */
    virtual vector<ActivationRecord *> records() const = 0;

    /**
     * Get the topmost activation record at a given nesting level.
     * Defined by an implementation subclass.
     * @param nesting_level the nesting level.
     * @return the activation record.
     */
    virtual ActivationRecord *get_topmost(const int nesting_level) const = 0;

    /**
     * Defined by an implementation subclass.
     * @return the current nesting level.
     */
    virtual int current_nesting_level() const = 0;

    /**
     * Pop an activation record off the stack.
     * Defined by an implementation subclass.
     */
    virtual void pop() = 0;
    /**
     * Push an activation record onto the stack.
     * Defined by an implementation subclass.
     * @param ar the activation record to push.
     */
    virtual void push(ActivationRecord *ar) = 0;
};

}}}  // namespace wci::backend::interpreter

#endif /* RUNTIMESTACK_H_ */
