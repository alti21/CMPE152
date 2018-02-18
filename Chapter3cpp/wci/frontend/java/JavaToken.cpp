/**
 * <h1>JavaToken</h1>
 *
 * <p>Base class for Java token classes.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */
#include <string>
#include <vector>
#include <map>
#include "JavaToken.h"

namespace wci { namespace frontend { namespace java {

using namespace std;

map<string, JavaTokenType> JavaToken::RESERVED_WORDS;
map<string, JavaTokenType> JavaToken::SPECIAL_SYMBOLS;
map<JavaTokenType, string> JavaToken::SPECIAL_SYMBOL_NAMES;

bool JavaToken::INITIALIZED = false;

void JavaToken::initialize()
{
    if (INITIALIZED) return;

    vector<string> rw_strings =
    {
        "AND", "ARRAY", "BEGIN", "CASE", "CONST", "DIV", "DO", "DOWNTO",
        "ELSE", "END", "FILE", "FOR", "FUNCTION", "GOTO", "IF", "IN",
        "LABEL", "MOD", "NIL", /*"NOT",*/ "OF", "OR", "PACKED", "PROCEDURE",
        "PROGRAM", "RECORD", "REPEAT", "SET", "THEN", "TO", "TYPE",
        "UNTIL", "VAR", "WHILE", "WITH"
    };

    vector<JavaTokenType> rw_keys =
    {
        JavaTokenType::AND,
        JavaTokenType::ARRAY,
        JavaTokenType::BEGIN,
        JavaTokenType::CASE,
        JavaTokenType::CONST,
        JavaTokenType::DIV,
        JavaTokenType::DO,
        JavaTokenType::DOWNTO,

        JavaTokenType::ELSE,
        JavaTokenType::END,
        JavaTokenType::FILE,
        JavaTokenType::FOR,
        JavaTokenType::FUNCTION,
        JavaTokenType::GOTO,
        JavaTokenType::IF,
        JavaTokenType::IN,

        JavaTokenType::LABEL,
        JavaTokenType::MOD,
        JavaTokenType::NIL,
        //		JavaTokenType::NOT,
        JavaTokenType::OF,
        JavaTokenType::OR,
        JavaTokenType::PACKED,
        JavaTokenType::PROCEDURE,

        JavaTokenType::PROGRAM,
        JavaTokenType::RECORD,
        JavaTokenType::REPEAT,
        JavaTokenType::SET,
        JavaTokenType::THEN,
        JavaTokenType::TO,
        JavaTokenType::TYPE,

        JavaTokenType::UNTIL,
        JavaTokenType::VAR,
        JavaTokenType::WHILE,
        JavaTokenType::WITH
    };

    for (int i = 0; i < rw_strings.size(); i++)
    {
        RESERVED_WORDS[rw_strings[i]] = rw_keys[i];
    }

    vector<string> ss_strings =
    {
        "+", "-", "*", "/", ":=", ".", ",", ";", ":", /*"'",*/ "=", "<>", //COMMENTED OUT (V6)
        "<", "<=", ">=", ">", "(", ")", "[", "]", "{", "}",  "^", "..",
		//ADDED HERE
		"?"
    };

    vector<JavaTokenType> ss_keys =
    {
        JavaTokenType::PLUS,
        JavaTokenType::MINUS,
        JavaTokenType::STAR,
        JavaTokenType::SLASH,
        JavaTokenType::COLON_EQUALS,
        JavaTokenType::DOT,
        JavaTokenType::COMMA,
        JavaTokenType::SEMICOLON,
        JavaTokenType::COLON,
        // JavaTokenType::QUOTE, //COMMENTED OUT (V6)
        JavaTokenType::EQUALS,
        JavaTokenType::NOT_EQUALS,

        JavaTokenType::LESS_THAN,
        JavaTokenType::LESS_EQUALS,
        JavaTokenType::GREATER_EQUALS,
        JavaTokenType::GREATER_THAN,
        JavaTokenType::LEFT_PAREN,
        JavaTokenType::RIGHT_PAREN,
        JavaTokenType::LEFT_BRACKET,
        JavaTokenType::RIGHT_BRACKET,
        JavaTokenType::LEFT_BRACE,
        JavaTokenType::RIGHT_BRACE,
        JavaTokenType::UP_ARROW,
        JavaTokenType::DOT_DOT,
		//ADDED HERE
		JavaTokenType::QUESTION_MARK
    };

    for (int i = 0; i < ss_strings.size(); i++)
    {
        SPECIAL_SYMBOLS[ss_strings[i]] = ss_keys[i];
    }

    vector<string> ss_names =
    {
        "PLUS", "MINUS", "STAR", "SLASH", "COLON_EQUALS", "DOT", "COMMA",
        "SEMICOLON", "COLON", /*"QUOTE",*/ "EQUALS", "NOT_EQUALS", //COMMENTED OUT (V6)

        "LESS_THAN", "LESS_EQUALS", "GREATER_EQUALS", "GREATER_THAN",
        "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACKET", "RIGHT_BRACKET",
        "LEFT_BRACE", "RIGHT_BRACE", "UP_ARROW", "DOT_DOT",
		//ADDED HERE
		"QUESTION_MARK"
    };

    for (int i = 0; i < ss_names.size(); i++)
    {
        SPECIAL_SYMBOL_NAMES[ss_keys[i]] = ss_names[i];
    }

    INITIALIZED = true;
}

JavaToken::JavaToken(Source *source) throw (string)
    : Token(source)
{
    initialize();
}

}}}  // namespace wci::frontend::java
