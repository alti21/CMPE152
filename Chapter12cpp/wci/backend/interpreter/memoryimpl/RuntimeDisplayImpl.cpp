/**
 * <h1>RuntimeDisplayImpl</h1>
 *
 * <p>The interpreter's runtime display.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include <vector>
#include "RuntimeDisplayImpl.h"
#include "../RuntimeDisplay.h"
#include "../ActivationRecord.h"

namespace wci { namespace backend { namespace interpreter { namespace memoryimpl {

using namespace wci;
using namespace wci::backend::interpreter;

RuntimeDisplayImpl::RuntimeDisplayImpl()
{
    display.push_back(nullptr);  // dummy element 0 (never used)
}

RuntimeDisplayImpl::~RuntimeDisplayImpl()
{
}

ActivationRecord *RuntimeDisplayImpl::get_activation_record
                                        (const int nesting_level) const
{
    return display[nesting_level];
}

void RuntimeDisplayImpl::call_update(const int nesting_level,
                                     ActivationRecord *ar)
{
    // Next higher nesting level: Append a new element at the top.
    if (nesting_level >= display.size())
    {
        display.push_back(ar);
    }

    // Existing nesting level: Set at the specified level.
    else
    {
        ActivationRecord *prev_ar = display[nesting_level];
        display[nesting_level] = ar->make_link_to(prev_ar);
    }
}

void RuntimeDisplayImpl::return_update(const int nesting_level)
{
    int top_index = display.size() - 1;
    ActivationRecord *ar = display[nesting_level];  // AR about to be popped off
    ActivationRecord *prev_ar = ar->linked_to();    // previous AR it points to

    // Point the element at that nesting level to the
    // previous activation record.
    if (prev_ar != nullptr)
    {
        display[nesting_level] = prev_ar;
    }

    // The top element has become null, so remove it.
    else if (nesting_level == top_index)
    {
        display.erase(display.begin() + top_index);
    }
}

}}}}  // namespace wci::backend::interpreter::memoryimpl
