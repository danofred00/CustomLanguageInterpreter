#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>


/*
 * TokenType enum class
 *
 * Represents the type of a token.
 *
 * IDENTIFIER: An identifier.
 * TYPE: A type.
 * SEMICOLON: A semicolon.
 * EQUALS: An equals sign.
 * NUMBER_LITERAL: A number literal.
*/
enum class TokenType
{
	/* declaration tokens */
	IDENTIFIER,
	RESERVED, // To remove later

	/* Reserved keywords */
	VAR_DECLARATION,
	
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

inline std::string tokenTypeToString(TokenType type)
{
	switch (type)
	{
	case TokenType::IDENTIFIER:
		return "IDENTIFIER";
	case TokenType::VAR_DECLARATION:
		return "VAR_DECLARATION";
	case TokenType::SEMICOLON:
		return "SEMICOLON";
	case TokenType::COMMA:
		return "COMMA";
	case TokenType::OPEN_BRACKET:
		return "OPEN_BRACKET";
	case TokenType::CLOSE_BRACKET:
		return "CLOSE_BRACKET";
	case TokenType::EQUALS:
		return "EQUALS";
	case TokenType::BINARY_OPERATOR:
		return "BINARY_OPERATOR";
	case TokenType::NUMBER_LITERAL:
		return "NUMBER_LITTERAL";
	case TokenType::STRING_LITERAL:
		return "STRING_LITERAL";
	case TokenType::RESERVED_LITERAL:
		return "RESERVED_LITERAL";
	case TokenType::END_OF_FILE:
		return "END_OF_FILE";
	case TokenType::CONDITION_IF:
		return "CONDITION_IF";
	case TokenType::CONDITION_ELSE:
		return "CONDITION_ELSE";
	case TokenType::OPEN_PAREN:
		return "OPEN_PAREN";
	// comparaison & logic expressions
	case TokenType::LOGIC_AND:
		return "LOGIC_AND";
	case TokenType::LOGIC_EQUAL:
		return "LOGIC_EQUAL";
	case TokenType::LOGIC_GREATER:
		return "LOGIC_GREATER";
	case TokenType::LOGIC_GREATER_EQUAL:
		return "LOGIC_GREATER_EQUAL";
	case TokenType::LOGIC_LOWER:
		return "LOGIC_LOWER";
	case TokenType::LOGIC_LOWER_EQUAL:
		return "LOGIC_LOWER_EQUAL";
	case TokenType::LOGIC_NOT:
		return "LOGIC_NOT";
	case TokenType::LOGIC_OR:
		return "LOGIC_OR";
	// end
	case TokenType::CLOSE_PAREN:
		return "CLOSE_PAREN";
	case TokenType::UNKNOW:
	default:
		return "UNKNOW";
	}
}

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
		ss << "Token<type=" + tokenTypeToString(type) + "(" << static_cast<int>(type);
		ss << "), value='" << value << "'>";
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