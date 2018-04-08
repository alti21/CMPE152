/**
 * <h1>BackendFactory</h1>
 *
 * <p>A factory class that creates compiler and interpreter components.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#ifndef WCI_BACKEND_BACKENDFACTORY_H_
#define WCI_BACKEND_BACKENDFACTORY_H_

#include <string>
#include "Backend.h"
#include "interpreter/Cell.h"
#include "../Object.h"
#include "../intermediate/TypeSpec.h"

namespace wci { namespace backend {

using namespace std;
using namespace wci::backend::interpreter;

class BackendFactory
{
public:
    /**
     * Create a compiler or an interpreter back end component.
     * @param operation either "compile" or "execute"
     * @return a compiler or an interpreter back end component.
     * @throw a string message if an error occurred.
     */
	static Backend *create_backend(string operation) throw (string);

    /**
     * Return the default value for a data type.
     * @param typespec the data type.
     * @return the default value.
     */
    static Object default_value(TypeSpec *typespec);
};

}} // namespace wci::backend

#endif /* WCI_BACKEND_BACKENDFACTORY_H_ */
