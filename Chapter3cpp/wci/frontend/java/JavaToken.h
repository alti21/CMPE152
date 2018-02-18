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
    AND, ARRAY, BEGIN, CASE, CONST, DIV, DO, DOWNTO, ELSE, END,
    FILE, FOR, FUNCTION, GOTO, IF, IN, LABEL, MOD, NIL, /*NOT,*/
    OF, OR, PACKED, PROCEDURE, PROGRAM, RECORD, REPEAT, SET,
    THEN, TO, TYPE, UNTIL, VAR, WHILE, WITH,

    // Special symbols.
    PLUS, MINUS, STAR, SLASH, COLON_EQUALS,
    DOT, COMMA, SEMICOLON, COLON, /*QUOTE,*/ //COMMENTED OUT (V6)
    EQUALS, NOT_EQUALS, LESS_THAN, LESS_EQUALS,
    GREATER_EQUALS, GREATER_THAN, LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACKET, RIGHT_BRACKET, LEFT_BRACE, RIGHT_BRACE,
    UP_ARROW, DOT_DOT,

	//ADDED HERE
	QUESTION_MARK,

    IDENTIFIER, INTEGER, REAL, STRING,
    ERROR, END_OF_FILE,
	//ADDED HERE (V6)
	CHAR,
};

constexpr JavaTokenType PT_AND = JavaTokenType::AND;
constexpr JavaTokenType PT_ARRAY = JavaTokenType::ARRAY;
constexpr JavaTokenType PT_BEGIN = JavaTokenType::BEGIN;
constexpr JavaTokenType PT_CASE = JavaTokenType::CASE;
constexpr JavaTokenType PT_CONST = JavaTokenType::CONST;
constexpr JavaTokenType PT_DIV = JavaTokenType::DIV;
constexpr JavaTokenType PT_DO = JavaTokenType::DO;
constexpr JavaTokenType PT_DOWNTO = JavaTokenType::DOWNTO;

constexpr JavaTokenType PT_ELSE = JavaTokenType::ELSE;
constexpr JavaTokenType PT_END = JavaTokenType::END;
constexpr JavaTokenType PT_FILE = JavaTokenType::FILE;
constexpr JavaTokenType PT_FOR = JavaTokenType::FOR;
constexpr JavaTokenType PT_FUNCTION = JavaTokenType::FUNCTION;
constexpr JavaTokenType PT_GOTO = JavaTokenType::GOTO;
constexpr JavaTokenType PT_IF = JavaTokenType::IF;
constexpr JavaTokenType PT_IN = JavaTokenType::IN;

constexpr JavaTokenType PT_LABEL = JavaTokenType::LABEL;
constexpr JavaTokenType PT_MOD = JavaTokenType::MOD;
constexpr JavaTokenType PT_NIL = JavaTokenType::NIL;
// constexpr JavaTokenType PT_NOT = JavaTokenType::NOT; TOOK OUT HERE
constexpr JavaTokenType PT_OF = JavaTokenType::OF;
constexpr JavaTokenType PT_OR = JavaTokenType::OR;
constexpr JavaTokenType PT_PACKED = JavaTokenType::PACKED;
constexpr JavaTokenType PT_PROCEDURE = JavaTokenType::PROCEDURE;

constexpr JavaTokenType PT_PROGRAM = JavaTokenType::PROGRAM;
constexpr JavaTokenType PT_RECORD = JavaTokenType::RECORD;
constexpr JavaTokenType PT_REPEAT = JavaTokenType::REPEAT;
constexpr JavaTokenType PT_SET = JavaTokenType::SET;
constexpr JavaTokenType PT_THEN = JavaTokenType::THEN;
constexpr JavaTokenType PT_TO = JavaTokenType::TO;
constexpr JavaTokenType PT_TYPE = JavaTokenType::TYPE;

constexpr JavaTokenType PT_UNTIL = JavaTokenType::UNTIL;
constexpr JavaTokenType PT_VAR = JavaTokenType::VAR;
constexpr JavaTokenType PT_WHILE = JavaTokenType::WHILE;
constexpr JavaTokenType PT_WITH = JavaTokenType::WITH;

constexpr JavaTokenType PT_PLUS = JavaTokenType::PLUS;
constexpr JavaTokenType PT_MINUS = JavaTokenType::MINUS;
constexpr JavaTokenType PT_STAR = JavaTokenType::STAR;
constexpr JavaTokenType PT_SLASH = JavaTokenType::SLASH;
constexpr JavaTokenType PT_COLON_EQUALS = JavaTokenType::COLON_EQUALS;
constexpr JavaTokenType PT_DOT = JavaTokenType::DOT;
constexpr JavaTokenType PT_COMMA = JavaTokenType::COMMA;
constexpr JavaTokenType PT_SEMICOLON = JavaTokenType::SEMICOLON;
constexpr JavaTokenType PT_COLON = JavaTokenType::COLON;
//constexpr JavaTokenType PT_QUOTE = JavaTokenType::QUOTE; //COMMENTED OUT (V6)
constexpr JavaTokenType PT_EQUALS = JavaTokenType::EQUALS;
constexpr JavaTokenType PT_NOT_EQUALS = JavaTokenType::NOT_EQUALS;

constexpr JavaTokenType PT_LESS_THAN = JavaTokenType::LESS_THAN;
constexpr JavaTokenType PT_LESS_EQUALS = JavaTokenType::LESS_EQUALS;
constexpr JavaTokenType PT_GREATER_EQUALS = JavaTokenType::GREATER_EQUALS;
constexpr JavaTokenType PT_GREATER_THAN = JavaTokenType::GREATER_THAN;
constexpr JavaTokenType PT_LEFT_PAREN = JavaTokenType::LEFT_PAREN;
constexpr JavaTokenType PT_RIGHT_PAREN = JavaTokenType::RIGHT_PAREN;
constexpr JavaTokenType PT_LEFT_BRACKET = JavaTokenType::LEFT_BRACKET;
constexpr JavaTokenType PT_RIGHT_BRACKET = JavaTokenType::RIGHT_BRACKET;
constexpr JavaTokenType PT_LEFT_BRACE = JavaTokenType::LEFT_BRACE;
constexpr JavaTokenType PT_RIGHT_BRACE = JavaTokenType::RIGHT_BRACE;
constexpr JavaTokenType PT_UP_ARROW = JavaTokenType::UP_ARROW;
constexpr JavaTokenType PT_DOT_DOT = JavaTokenType::DOT_DOT;
//ADDED HERE
constexpr JavaTokenType PT_QUESTION_MARK = JavaTokenType::QUESTION_MARK;

constexpr JavaTokenType PT_IDENTIFIER = JavaTokenType::IDENTIFIER;
constexpr JavaTokenType PT_INTEGER = JavaTokenType::INTEGER;
constexpr JavaTokenType PT_REAL = JavaTokenType::REAL;
constexpr JavaTokenType PT_STRING = JavaTokenType::STRING;
constexpr JavaTokenType PT_ERROR = JavaTokenType::ERROR;
constexpr JavaTokenType PT_END_OF_FILE = JavaTokenType::END_OF_FILE;
//ADDED HERE (V6)
constexpr JavaTokenType PT_CHAR = JavaTokenType::CHAR;

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
