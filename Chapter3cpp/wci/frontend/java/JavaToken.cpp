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
    		"abstract", "break", "case", "char", "class", "const", "continue", "do", "double", "else", "enum",
    		"extends", "float", "for", "goto", "if", "int", "long", "native", "return", "short", "package",
    		"protected", "static", "long", "switch", "super", "this", "throw", "void", "volatile", "while"
    };

    vector<JavaTokenType> rw_keys =
    {
    		JavaTokenType::ABSTRACT,
    		JavaTokenType::BREAK,
    		JavaTokenType::CASE,
    		JavaTokenType::CHAR,
    		JavaTokenType::CLASS,
    		JavaTokenType::CONST,
    		JavaTokenType::CONTINUE,
    		JavaTokenType::DO,
    		JavaTokenType::DOUBLE,

    		JavaTokenType::ELSE,
    		JavaTokenType::ENUM,
    		JavaTokenType::EXTENDS,
    		JavaTokenType::FOR,
    		JavaTokenType::GOTO,
    		JavaTokenType::IF,
    		JavaTokenType::INT,

    		JavaTokenType::LONG,
    		JavaTokenType::NATIVE,
    		JavaTokenType::RETURN,
    		JavaTokenType::SHORT,
    		JavaTokenType::PACKAGE,
    		JavaTokenType::PROTECTED,
    		JavaTokenType::STATIC,
    		JavaTokenType::LONG,

    		JavaTokenType::SWITCH,
    		JavaTokenType::SUPER,
    		JavaTokenType::THIS,
    		JavaTokenType::VOID,
    		JavaTokenType::VOLATILE,
    		JavaTokenType::WHILE
    };

    for (int i = 0; i < rw_strings.size(); i++)
    {
        RESERVED_WORDS[rw_strings[i]] = rw_keys[i];
    }

    vector<string> ss_strings =
    {
    		"~", "!", "@", "%", "^", "&", "*", "-", "+", "=",
    		"|", "/", ":", ";", "?", "<", ">", ".", ",",
    		"'", "\"", "(", ")", "[", "]", "{", "}",
    		"++", "--", "<<", ">>", "<=", ">=", "+=", "-=", "*=", "/=",
    		"==", "|=", "%=", "&=", "^=", "!=", "<<=", ">>=", "||", "&&",
    		"//", "/*", "*/"
    };

    vector<JavaTokenType> ss_keys =
    {
    		JavaTokenType::TILDE,
    		JavaTokenType::LOGICAL_NOT,
    		JavaTokenType::AT,
    		JavaTokenType::MODULUS,
    		JavaTokenType::CARET,
    		JavaTokenType::AMPERSAND,

    		JavaTokenType::ASTERISK,
    		JavaTokenType::MINUS,
    		JavaTokenType::PLUS,
    		JavaTokenType::EQUALS,
    		JavaTokenType::OR,
    		JavaTokenType::SLASH,
    		JavaTokenType::COLON,
    		JavaTokenType::SEMICOLON,

    		JavaTokenType::QUESTION_MARK,
    		JavaTokenType::LESS_THAN,
    		JavaTokenType::GREATER_THAN,
    		JavaTokenType::DOT,
    		JavaTokenType::COMMA,

    		JavaTokenType::APOSTROPHE,
    		JavaTokenType::QUOTE,
    		JavaTokenType::LEFT_PAREN,
    		JavaTokenType::RIGHT_PAREN,
    		JavaTokenType::LEFT_BRACKET,
    		JavaTokenType::RIGHT_BRACKET,

    		JavaTokenType::LEFT_BRACE,
    		JavaTokenType::RIGHT_BRACE,
    		JavaTokenType::INCREMENT,
    		JavaTokenType::DECREMENT,
    		JavaTokenType::LEFT_SHIFT,
    		JavaTokenType::RIGHT_SHIFT,

    		JavaTokenType::LESS_EQUALS,
    		JavaTokenType::GREATER_EQUALS,
    		JavaTokenType::PLUS_EQUALS,
    		JavaTokenType::MINUS_EQUALS,
    		JavaTokenType::MULT_EQUALS,
    		JavaTokenType::SLASH_EQUALS,
    		JavaTokenType::EQUALS_EQUALS,

    		JavaTokenType::BAR_EQUALS,
    		JavaTokenType::MODULUS_EQUALS,
    		JavaTokenType::AMPERSAND_EQUALS,
    		JavaTokenType::CAROT_EQUALS,
    		JavaTokenType::NOT_EQUALS,

    		JavaTokenType::LEFT_SHIFT_EQUALS,
    		JavaTokenType::RIGHT_SHIFT_EQUALS,
    		JavaTokenType::LOGICAL_OR,
    		JavaTokenType::LOGICAL_AND,

    		JavaTokenType::SLASH_SLASH,
    		JavaTokenType::SLASH_ASTERISK,
    		JavaTokenType::ASTERISK_SLASH,
    };

    for (int i = 0; i < ss_strings.size(); i++)
    {
        SPECIAL_SYMBOLS[ss_strings[i]] = ss_keys[i];
    }

    vector<string> ss_names =
    {
    		"TILDE", "LOGICAL_NOT", "AT", "MODULUS", "CARET", "AMPERSAND",
    		"ASTERISK", "MINUS", "PLUS", "EQUALS", "OR", "SLASH", "COLON", "SEMICOLON",
    		"QUESTION_MARK", "LESS_THAN", "GREATER_THAN", "DOT", "COMMA",

    		"APOSTROPHE", "QUOTE", "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACKET", "RIGHT_BRACKET",
    		"LEFT_BRACE", "RIGHT_BRACE", "INCREMENT", "DECREMENT", "LEFT_SHIFT", "RIGHT_SHIFT",

    		"LESS_EQUALS", "GREATER_EQUALS","PLUS_EQUALS", "MINUS_EQUALS", "MULT_EQUALS", "SLASH_EQUALS", "EQUALS_EQUALS",
    		"BAR_EQUALS", "MODULUS_EQUALS", "AMPERSAND_EQUALS", "CAROT_EQUALS", "NOT_EQUALS",

    		"LEFT_SHIFT_EQUALS", "RIGHT_SHIFT_EQUALS", "LOGICAL_OR", "LOGICAL_AND",
    		"SLASH_SLASH", "SLASH_ASTERISK", "ASTERISK_SLASH"
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
