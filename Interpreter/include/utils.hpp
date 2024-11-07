#pragma once

#include <vector>
#include <string>
#include <functional>
#include <types.hpp>


/*
	split a string into a vector of string
*/
std::vector<std::string> str_split(const std::string str, char delimitor);

/*
	Display in the outstream a tokenList object
*/
// void showTokenList(const TokenList& tokenList);


/* LEXER helpers */

/**
 * Char To String function
 */
inline std::string atos(char c) {
	std::string s = { c };
	return s;
}

bool str_match(const std::string& str, const std::string& pattern);

bool isNumeric(const std::string& str);

bool isAlpha(const std::string& str);

bool isKeyword(const std::string& str);

bool isIdentifier(const std::string& str);

bool isSkippable(const std::string & str);

/**
 * Check if string is a simple quote or a double quote
 */
bool isStringDeclarator(const std::string & str);

/**
 * Check if it's a comparaison operator
 */
bool isComparaisonOperator(const std::string & str);

/**
 * Check if it's a single operator
 */
bool isSingleOperator(const std::string & str);