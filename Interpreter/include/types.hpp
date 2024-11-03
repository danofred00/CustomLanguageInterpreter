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
	RESERVED,
	
	/* container tokens */
	SEMICOLON,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	EQUALS,
	BINARY_OPERATOR, /* +, -, *, / */

	/* Value tokens */
	NUMBER_LITERAL,
	STRING_LITERAL,

};

inline std::string tokenTypeToString(TokenType type)
{
	switch (type)
	{
	case TokenType::IDENTIFIER:
		return "IDENTIFIER";
	case TokenType::RESERVED:
		return "RESERVED";
	case TokenType::SEMICOLON:
		return "SEMICOLON";
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

