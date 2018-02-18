#include <string>
#include "JavaCharacterToken.h"

#include "../JavaError.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::java;

JavaCharacterToken::JavaCharacterToken(Source *source) throw (string)
    : JavaToken(source)
{
    extract();
}

void JavaCharacterToken::extract() throw (string)
{
    string value_ch = "";

    char current_ch = next_char();  // consume initial quote

    text += "'";

    do
    {
        if ((current_ch != '\'') && (current_ch != EOF))
        {
        		if (current_ch == '\\')
        		{
        			char next_ch = next_char();
        			text += current_ch;
        			if (next_ch == 't')
        			{
        				text += next_ch;
            			value_ch  += "\t";
            			current_ch = next_char();
        			}
        			if (next_ch == 'n')
        			{
        				text += next_ch;
            			value_ch  += "\n";
            			current_ch = next_char();
        			}
        			if (next_ch == '\\')
        			{
        				text += next_ch;
            			value_ch  += "\\";
            			current_ch = next_char();
        			}
        			if (next_ch == '\'')
        			{
        				text += next_ch;
            			value_ch  += "\'";
            			current_ch = next_char();
        			}
        		}
        		else
        		{
        			text += current_ch;
        			value_ch  += current_ch;
        			current_ch = next_char();  // consume character
        		}
        }
    } while ((current_ch != '\'') && (current_ch != Source::END_OF_FILE));

    if (current_ch == '\'')
    {
        next_char();  // consume final quote
        text += '\'';
        type = (TokenType) PT_CHAR;
        value = value_ch;
    }
    else
    {
        type = (TokenType) PT_ERROR;
        value = (int) INVALID_CHARACTER;
    }
}

}}}}  // namespace wci::frontend::java::tokens
