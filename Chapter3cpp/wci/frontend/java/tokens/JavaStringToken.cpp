/**
 * <h1>JavaStringToken</h1>
 *
 * <p> Java string tokens.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include "JavaStringToken.h"

#include "../JavaError.h"

namespace wci { namespace frontend { namespace java { namespace tokens {

using namespace std;
using namespace wci::frontend;
using namespace wci::frontend::java;

JavaStringToken::JavaStringToken(Source *source) throw (string)
    : JavaToken(source)
{
    extract();
}

void JavaStringToken::extract() throw (string)
{
    string value_str = "";

    char current_ch = next_char();  // consume initial quote

    text += "\""; // text += "'"; //EDITED HERE

    // Get string characters.
    do
    {
        // Replace any whitespace character with a blank.
        if (isspace(current_ch)) current_ch = ' ';

        //EDITED HERE
        if ((current_ch != '"') && (current_ch != EOF))// ((current_ch != '\'') && (current_ch != EOF))
        {
        		if (current_ch == '\\')
        		{
        			char next_ch = next_char();
        			text += current_ch;
        			if (next_ch == 't')
        			{
        				text += next_ch;
            			value_str  += "\t";
            			current_ch = next_char();
        			}
        			if (next_ch == 'n')
        			{
        				text += next_ch;
            			value_str  += "\n";
            			current_ch = next_char();
        			}
        			if (next_ch == '\"')
        			{
        				text += next_ch;
            			value_str  += "\"";
            			current_ch = next_char();
        			}
        		}
        		else
        		{
        			text += current_ch;
        			value_str  += current_ch;
        			current_ch = next_char();  // consume character
        		}
        }
    } while ((current_ch != '"') && (current_ch != Source::END_OF_FILE)); // EDITED HERE
    // while ((current_ch != '\'') && (current_ch != Source::END_OF_FILE));

    if (current_ch == '"') //(current_ch == '\'') // EDITED HERE
    {
        next_char();  // consume final quote
        text += '"'; //text += '\''; // EDITED HERE
        type = (TokenType) PT_STRING;
        value = value_str;
    }
    else
    {
        type = (TokenType) PT_ERROR;
        value = (int) UNEXPECTED_EOF;
    }
}

}}}}  // namespace wci::frontend::java::tokens
