/**
 * <h1>JavaToken</h1>
 *
 * <p>Base class for Java token classes.</p>
 *
 * <p>Copyright (c) 2017 by Ronald Mak</p>
 * <p>For instructional purposes only.  No warranties.</p>
 */

#ifndef WCI_FRONTEND_JAVA_JAVATOKEN_H_
#define WCI_FRONTEND_JAVA_JAVATOKEN_H_

#include <string>
#include <map>
#include "../Token.h"
#include "../Source.h"

namespace wci { namespace frontend { namespace java {

using namespace std;
using namespace wci::frontend;

/**
 * Java token types.
 */
enum class JavaTokenType
{
    // Reserved words.
    ABSTRACT, BREAK, CASE, CHAR, CLASS, CONST, CONTINUE, DO, DOUBLE, ELSE, ENUM,
    EXTENDS, FLOAT, FOR, GOTO, IF, INT, LONG, NATIVE, RETURN, SHORT, PACKAGE,
	PROTECTED, STATIC, SWITCH, SUPER, THIS, THROW, VOID, VOLATILE, WHILE,

    // Special symbols.
	TILDE, LOGICAL_NOT, AT, MODULUS, CARET, AMPERSAND,
	ASTERISK, MINUS, PLUS, EQUALS, OR, SLASH, COLON, SEMICOLON,
	QUESTION_MARK, LESS_THAN, GREATER_THAN, DOT, COMMA,
	APOSTROPHE, QUOTE, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET, RIGHT_BRACKET,
	LEFT_BRACE, RIGHT_BRACE, INCREMENT, DECREMENT, LEFT_SHIFT, RIGHT_SHIFT,
	LESS_EQUALS, GREATER_EQUALS,PLUS_EQUALS, MINUS_EQUALS, MULT_EQUALS, SLASH_EQUALS, EQUALS_EQUALS,
	BAR_EQUALS, MODULUS_EQUALS, AMPERSAND_EQUALS, CAROT_EQUALS, NOT_EQUALS,
	LEFT_SHIFT_EQUALS, RIGHT_SHIFT_EQUALS, LOGICAL_OR, LOGICAL_AND,
	SLASH_SLASH, SLASH_ASTERISK, ASTERISK_SLASH,

	IDENTIFIER, INTEGER, REAL, STRING,
    ERROR, END_OF_FILE
};

constexpr JavaTokenType PT_ABSTRACT = JavaTokenType::ABSTRACT;
constexpr JavaTokenType PT_BREAK = JavaTokenType::BREAK;
constexpr JavaTokenType PT_CASE = JavaTokenType::CASE;
constexpr JavaTokenType PT_CHAR = JavaTokenType::CHAR;
constexpr JavaTokenType PT_CLASS = JavaTokenType::CLASS;
constexpr JavaTokenType PT_CONST = JavaTokenType::CONST;
constexpr JavaTokenType PT_CONTINUE = JavaTokenType::CONTINUE;
constexpr JavaTokenType PT_DO = JavaTokenType::DO;
constexpr JavaTokenType PT_DOUBLE = JavaTokenType::DOUBLE;

constexpr JavaTokenType PT_ELSE = JavaTokenType::ELSE;
constexpr JavaTokenType PT_ENUM = JavaTokenType::ENUM;
constexpr JavaTokenType PT_EXTENDS = JavaTokenType::EXTENDS;
constexpr JavaTokenType PT_FOR = JavaTokenType::FOR;
constexpr JavaTokenType PT_GOTO = JavaTokenType::GOTO;
constexpr JavaTokenType PT_IF = JavaTokenType::IF;
constexpr JavaTokenType PT_INT = JavaTokenType::INT;

constexpr JavaTokenType PT_LONG = JavaTokenType::LONG;
constexpr JavaTokenType PT_NATIVE = JavaTokenType::NATIVE;
constexpr JavaTokenType PT_RETURN = JavaTokenType::RETURN;
constexpr JavaTokenType PT_SHORT = JavaTokenType::SHORT;
constexpr JavaTokenType PT_PACKAGE = JavaTokenType::PACKAGE;
constexpr JavaTokenType PT_PROTECTED = JavaTokenType::PROTECTED;
constexpr JavaTokenType PT_STATIC = JavaTokenType::STATIC;

constexpr JavaTokenType PT_SWITCH = JavaTokenType::SWITCH;
constexpr JavaTokenType PT_SUPER = JavaTokenType::SUPER;
constexpr JavaTokenType PT_THIS = JavaTokenType::THIS;
constexpr JavaTokenType PT_THROW = JavaTokenType::VOID;
constexpr JavaTokenType PT_VOLATILE = JavaTokenType::VOLATILE;
constexpr JavaTokenType PT_WHILE = JavaTokenType::WHILE;

constexpr JavaTokenType PT_TILDE = JavaTokenType::TILDE;
constexpr JavaTokenType PT_LOGICAL_NOT = JavaTokenType::LOGICAL_NOT;
constexpr JavaTokenType PT_AT = JavaTokenType::AT;
constexpr JavaTokenType PT_MODULUS = JavaTokenType::MODULUS;
constexpr JavaTokenType PT_CARET = JavaTokenType::CARET;
constexpr JavaTokenType PT_AMPERSAND = JavaTokenType::AMPERSAND;

constexpr JavaTokenType PT_ASTERISK = JavaTokenType::ASTERISK;
constexpr JavaTokenType PT_MINUS = JavaTokenType::MINUS;
constexpr JavaTokenType PT_PLUS = JavaTokenType::PLUS;
constexpr JavaTokenType PT_EQUALS = JavaTokenType::EQUALS;
constexpr JavaTokenType PT_OR = JavaTokenType::OR;
constexpr JavaTokenType PT_SLASH = JavaTokenType::SLASH;
constexpr JavaTokenType PT_COLON = JavaTokenType::COLON;
constexpr JavaTokenType PT_SEMICOLON = JavaTokenType::SEMICOLON;

constexpr JavaTokenType PT_QUESTION_MARK = JavaTokenType::QUESTION_MARK;
constexpr JavaTokenType PT_LESS_THAN = JavaTokenType::LESS_THAN;
constexpr JavaTokenType PT_GREATER_THAN = JavaTokenType::GREATER_THAN;
constexpr JavaTokenType PT_DOT = JavaTokenType::DOT;
constexpr JavaTokenType PT_COMMA = JavaTokenType::COMMA;

constexpr JavaTokenType PT_APOSTROPHE = JavaTokenType::APOSTROPHE;
constexpr JavaTokenType PT_QUOTE = JavaTokenType::QUOTE;
constexpr JavaTokenType PT_LEFT_PAREN = JavaTokenType::LEFT_PAREN;
constexpr JavaTokenType PT_RIGHT_PAREN = JavaTokenType::RIGHT_PAREN;
constexpr JavaTokenType PT_LEFT_BRACKET = JavaTokenType::LEFT_BRACKET;
constexpr JavaTokenType PT_RIGHT_BRACKET = JavaTokenType::RIGHT_BRACKET;

constexpr JavaTokenType PT_LEFT_BRACE = JavaTokenType::LEFT_BRACE;
constexpr JavaTokenType PT_RIGHT_BRACE = JavaTokenType::RIGHT_BRACE;
constexpr JavaTokenType PT_INCREMENT = JavaTokenType::INCREMENT;
constexpr JavaTokenType PT_DECREMENT = JavaTokenType::DECREMENT;
constexpr JavaTokenType PT_LEFT_SHIFT = JavaTokenType::LEFT_SHIFT;
constexpr JavaTokenType PT_RIGHT_SHIFT = JavaTokenType::RIGHT_SHIFT;

constexpr JavaTokenType PT_LESS_EQUALS = JavaTokenType::LESS_EQUALS;
constexpr JavaTokenType PT_GREATER_EQUALS = JavaTokenType::GREATER_EQUALS;
constexpr JavaTokenType PT_PLUS_EQUALS = JavaTokenType::PLUS_EQUALS;
constexpr JavaTokenType PT_MINUS_EQUALS = JavaTokenType::MINUS_EQUALS;
constexpr JavaTokenType PT_MULT_EQUALS = JavaTokenType::MULT_EQUALS;
constexpr JavaTokenType PT_SLASH_EQUALS = JavaTokenType::SLASH_EQUALS;
constexpr JavaTokenType PT_EQUALS_EQUALS = JavaTokenType::EQUALS_EQUALS;

constexpr JavaTokenType PT_BAR_EQUALS = JavaTokenType::BAR_EQUALS;
constexpr JavaTokenType PT_MODULUS_EQUALS = JavaTokenType::MODULUS_EQUALS;
constexpr JavaTokenType PT_AMPERSAND_EQUALS = JavaTokenType::AMPERSAND_EQUALS;
constexpr JavaTokenType PT_CAROT_EQUALS = JavaTokenType::CAROT_EQUALS;
constexpr JavaTokenType PT_NOT_EQUALS = JavaTokenType::NOT_EQUALS;

constexpr JavaTokenType PT_LEFT_SHIFT_EQUALS = JavaTokenType::LEFT_SHIFT_EQUALS;
constexpr JavaTokenType PT_RIGHT_SHIFT_EQUALS = JavaTokenType::RIGHT_SHIFT_EQUALS;
constexpr JavaTokenType PT_LOGICAL_OR = JavaTokenType::LOGICAL_OR;
constexpr JavaTokenType PT_LOGICAL_AND = JavaTokenType::LOGICAL_AND;

constexpr JavaTokenType PT_SLASH_SLASH = JavaTokenType::SLASH_SLASH;
constexpr JavaTokenType PT_SLASH_ASTERISK = JavaTokenType::SLASH_ASTERISK;
constexpr JavaTokenType PT_ASTERISK_SLASH = JavaTokenType::ASTERISK_SLASH;

constexpr JavaTokenType PT_IDENTIFIER = JavaTokenType::IDENTIFIER;
constexpr JavaTokenType PT_INTEGER = JavaTokenType::INTEGER;
constexpr JavaTokenType PT_REAL = JavaTokenType::REAL;
constexpr JavaTokenType PT_STRING = JavaTokenType::STRING;
constexpr JavaTokenType PT_ERROR = JavaTokenType::ERROR;
constexpr JavaTokenType PT_END_OF_FILE = JavaTokenType::END_OF_FILE;

class JavaToken : public Token
{
public:
    static map<string, JavaTokenType> RESERVED_WORDS;
    static map<string, JavaTokenType> SPECIAL_SYMBOLS;
    static map<JavaTokenType, string> SPECIAL_SYMBOL_NAMES;

protected:
    /**
     * Constructor.
     * @param source the source from where to fetch the token's characters.
     * @throw a string message if an error occurred.
     */
    JavaToken(Source *source) throw (string);

private:
    static bool INITIALIZED;

    /**
     * Initialize the static maps.
     */
    static void initialize();
};

}}}  // namespace wci::frontend::java

#endif /* WCI_FRONTEND_JAVA_JAVATOKEN_H_ */
