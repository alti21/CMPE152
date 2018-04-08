/**
 * <h1>RuntimeDisplayImpl</h1>
 *
 * <p>The interpreter's runtime display.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <iostream>
#include <string>
#include <vector>
#include "RuntimeStackImpl.h"
#include "../RuntimeDisplay.h"
#include "../ActivationRecord.h"
#include "../MemoryFactory.h"

namespace wci { namespace backend { namespace interpreter { namespace memoryimpl {

using namespace wci;
using namespace wci::intermediate;

RuntimeStackImpl::RuntimeStackImpl()
{
    display = MemoryFactory::create_runtime_display();
}

RuntimeStackImpl::~RuntimeStackImpl()
{
}

vector<ActivationRecord *> RuntimeStackImpl::records() const
{
    return stack;
}

ActivationRecord *RuntimeStackImpl::get_topmost(int nesting_level) const
{
    return display->get_activation_record(nesting_level);
}

int RuntimeStackImpl::current_nesting_level() const
{
    int top_index = stack.size() - 1;
    return top_index >= 0 ? stack[top_index]->get_nesting_level() : -1;
}

void RuntimeStackImpl::push(ActivationRecord *ar)
{
    int nesting_level = ar->get_nesting_level();

    stack.push_back(ar);
    display->call_update(nesting_level, ar);
}

void RuntimeStackImpl::pop()
{
    display->return_update(current_nesting_level());
    stack.erase(stack.begin() + stack.size() - 1);
}

}}}}  // namespace wci::backend::interpreter::memoryimpl
