#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>


/*
 * TokenType enum class
 * Represents the type of a token.
*/
enum class TokenType
{
	/* declaration tokens */
	IDENTIFIER,
	RESERVED, // To remove later

	/* Reserved keywords */
	VAR_DECLARATION,
	FN, // fn -> for function declaration
	RETURN, // return 
	
	/* container tokens */
	SEMICOLON,	// ;
	OPEN_PAREN, // (
	CLOSE_PAREN, // )
	OPEN_BRACKET, // {
	CLOSE_BRACKET, // }
	EQUALS, // =
	BINARY_OPERATOR, /* +, -, *, /, %  */
	COMMA, // ,

	/* Value tokens */
	NUMBER_LITERAL,
	STRING_LITERAL,
	RESERVED_LITERAL,

	/* Conditional expression */
	CONDITION_IF,
	CONDITION_ELSE,

	/* Logical Operators */
	LOGIC_GREATER,	// >
	LOGIC_GREATER_EQUAL, // >=
	LOGIC_LOWER, // <
	LOGIC_LOWER_EQUAL, // <=
	LOGIC_NOT, // not | !
	LOGIC_AND, // and | &&
	LOGIC_OR,  // or  | ||
	LOGIC_EQUAL, // ==

	/* Others */
	END_OF_FILE,	// EOF of the source code
	UNKNOW,
};

/*
 * Token struct
 *
 * Represents a token in the source code.
 *
 * type: The type of the token.
 * value: The value of the token.
*/
typedef struct Token
{
	TokenType type;
	std::string value;

	Token(TokenType type, const std::string& value)
	{
		this->type = type;
		this->value = value;
	};

	std::string toString() const
	{
		std::stringstream ss{};
		ss << "Token<type=" << static_cast<int>(type) << ", value='" << value << "'>";
		return ss.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const Token & token)
	{
		os << token.toString();
		return os;
	}
} Token;

typedef std::vector<Token> TokenList;


/* AST Types */
typedef std::string Operator;