/**
 * <h1>RuntimeStackImpl</h1>
 *
 * <p>The interpreter's runtime stack.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef RUNTIMESTACKIMPL_H_
#define RUNTIMESTACKIMPL_H_

#include <string>
#include <vector>
#include "../RuntimeStack.h"
#include "../RuntimeDisplay.h"
#include "../ActivationRecord.h"

namespace wci { namespace backend { namespace interpreter { namespace memoryimpl {

using namespace wci;
using namespace wci::backend::interpreter;

class RuntimeStackImpl : public RuntimeStack
{
public:
    /**
     * Constructor.
     */
    RuntimeStackImpl();

    /**
     * Destructor.
     */
    ~RuntimeStackImpl();

    /**
     * @return an array list of the activation records on the stack.
     */
    vector<ActivationRecord *> records() const;

    /**
     * Get the topmost activation record at a given nesting level.
     * @param nesting_level the nesting level.
     * @return the activation record.
     */
    ActivationRecord *get_topmost(int nesting_level) const;

    /**
     * @return the current nesting level.
     */
    int current_nesting_level() const;

    /**
     * Push an activation record onto the stack for a routine being called.
     * @param ar the activation record to push.
     */
    void push(ActivationRecord *ar);

    /**
     * Pop an activation record off the stack for a returning routine.
     */
    void pop();

private:
    vector<ActivationRecord *> stack;  // runtime stack
    RuntimeDisplay *display;           // runtime display
};

}}}}  // namespace wci::backend::interpreter::memoryimpl

#endif /* RUNTIMESTACKIMPL_H_ */
