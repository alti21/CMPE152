/**
 * <h1>ActivationRecordImpl</h1>
 *
 * <p>The runtime activation record.</p>
 *
 * <p>Copyright (c) 2009 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef ACTIVATIONRECORDIMPL_H_
#define ACTIVATIONRECORDIMPL_H_

#include <string>
#include <vector>
#include "../ActivationRecord.h"
#include "../../../intermediate/SymTabEntry.h"

namespace wci { namespace backend { namespace interpreter { namespace memoryimpl {

using namespace std;
using namespace wci;
using namespace wci::intermediate;

class ActivationRecordImpl : public ActivationRecord
{
public:
    /**
     * Constructor.
     * @param routine_id the symbol table entry of the routine's name.
     */
    ActivationRecordImpl(SymTabEntry *routine_id);

    /**
     * Destructor.
     */
    virtual ~ActivationRecordImpl();

    /**
     * Getter.
     * @return the symbol table entry of the routine's name.
     */
    SymTabEntry *get_routine_id() const;

    /**
     * Return the memory cell for the given name from the memory map.
     * @param name the name.
     * @return the cell.
     */
    Cell *get_cell(string name) const;

    /**
     * @return the list of all the names in the memory map.
     */
    vector<string> get_all_names() const;

    /**
     * Getter.
     * @return the scope nesting level.
     */
    int get_nesting_level() const;

    /**
     * @return the activation record to which this record is dynamically linked.
     */
    ActivationRecord *linked_to() const;

    /**
     * Make a dynamic link from this activation record to another one.
     * @param ar the activation record to link to.
     * @return this activation record.
     */
    ActivationRecord *make_link_to(ActivationRecord *ar);

private:
    int nesting_level;        // scope nesting level of this record
    ActivationRecord *link;   // dynamic link to the previous record
    SymTabEntry *routine_id;  // symbol table entry of the routine's name
    MemoryMap *memory_map;    // memory map of this stack record
};

}}}}  // namespace wci::backend::interpreter::memoryimpl

#endif /* ACTIVATIONRECORDIMPL_H_ */
