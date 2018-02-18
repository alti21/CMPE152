#ifndef WCI_FRONTEND_JAVA_TOKENS_JAVACHARACTERTOKEN_H_
#define WCI_FRONTEND_JAVA_TOKENS_JAVACHARACTERTOKEN_H_

#include <string>

#include "../JavaToken.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend::java;

class JavaCharacterToken : public JavaToken
{
public:
    JavaCharacterToken(Source *source) throw (string);

protected:
    void extract() throw (string);
};

}}}}  // namespace wci::frontend::java::tokens

#endif /* WCI_FRONTEND_JAVA_TOKENS_JAVACHARACTERTOKEN_H_ */
