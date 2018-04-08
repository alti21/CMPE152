/**
 * <h1>RuntimeDisplayImpl</h1>
 *
 * <p>The interpreter's runtime display.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef RUNTIMEDISPLAYIMPL_H_
#define RUNTIMEDISPLAYIMPL_H_

#include <string>
#include <vector>
#include "../RuntimeDisplay.h"
#include "../ActivationRecord.h"

namespace wci { namespace backend { namespace interpreter { namespace memoryimpl {

using namespace wci;
using namespace wci::backend::interpreter;

class RuntimeDisplayImpl : public RuntimeDisplay
{
public:
    /**
     * Constructor.
     */
    RuntimeDisplayImpl();

    /**
     * Destructor.
     */
    ~RuntimeDisplayImpl();

    /**
     * Get the activation record at a given nesting level.
     * @param nesting_level the nesting level.
     * @return the activation record.
     */
    ActivationRecord *get_activation_record(const int nesting_level) const;

    /**
     * Update the display for a call to a routine at a given nesting level.
     * @param nesting_level the nesting level.
     * @param ar the activation record for the routine.
     */
    void call_update(const int nesting_level, ActivationRecord *ar);

    /**
     * Update the display for a return from a routine at a given nesting level.
     * @param nestingLevel the nesting level.
     */
    void return_update(const int nesting_level);

private:
    vector<ActivationRecord *> display;  // runtime display
};

}}}}  // namespace wci::backend::interpreter::memoryimpl

#endif /* RUNTIMEDISPLAYIMPL_H_ */
