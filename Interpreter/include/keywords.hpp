#pragma once

#include <vector>
#include <map>
#include <string>
#include <types.hpp>

const std::map<std::string, TokenType> singleOperators = {
	{"(", TokenType::OPEN_PAREN},
	{")", TokenType::CLOSE_PAREN},
	{"{", TokenType::OPEN_BRACKET},
	{"}", TokenType::CLOSE_BRACKET},
	{"+", TokenType::BINARY_OPERATOR},
	{"-", TokenType::BINARY_OPERATOR},
	{"/", TokenType::BINARY_OPERATOR},
	{"*", TokenType::BINARY_OPERATOR},
	{"%", TokenType::BINARY_OPERATOR},
	// {"=", TokenType::EQUALS},
	{";", TokenType::SEMICOLON},
	{",", TokenType::COMMA}
};


const std::map<std::string, TokenType> keywords = {
	/* TYPES */
	{"bool", TokenType::VAR_DECLARATION},
	{"number", TokenType::VAR_DECLARATION},
	{"string", TokenType::VAR_DECLARATION},
	/* VALUES */
	{"null", TokenType::RESERVED_LITERAL},
	{"true", TokenType::RESERVED_LITERAL},
	{"false", TokenType::RESERVED_LITERAL},
	/* CONDITIONS */
	{"if", TokenType::CONDITION_IF},
	{"else", TokenType::CONDITION_ELSE},
	/* LOGIC OPERATOR */
	{"and", TokenType::LOGIC_AND},
	{"or", TokenType::LOGIC_OR},
	{"not", TokenType::LOGIC_NOT},
	/* FUNCTIONS */
	{"fn", TokenType::FN},
	{"return", TokenType::RETURN}
};
 	