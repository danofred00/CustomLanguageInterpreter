#pragma once

#include <vector>
#include <string>

/*
	split a string into a vector of string
*/
std::vector<std::string> str_split(const std::string str, char delimitor);

/*
	Display in the outstream a tokenList object
*/
void showTokenList(const TokenList& tokenList);


/* LEXER helpers */

bool str_match(const std::string& str, const std::string& pattern);

bool isNumeric(const std::string& str);

bool isAlpha(const std::string& str);

bool isKeyword(const std::string& str);

bool isIdentifier(const std::string& str);