#pragma once

#include <vector>
#include <string>
#include <functional>


/*
	split a string into a vector of string
*/
std::vector<std::string> str_split(const std::string str, char delimitor);

// Trim from begin
inline std::string &str_ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) {
		return !std::isspace(c);
	}));
    return s;
}

// trim from end
inline std::string &str_rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char c) {
		return !std::isspace(c);
	}).base(), s.end());
    return s;
}

// trim from both ends
inline std::string &str_trim(std::string &s) {
    return str_ltrim(str_rtrim(s));
}

/*
	Display in the outstream a tokenList object
*/
void showTokenList(const TokenList& tokenList);


/* LEXER helpers */

bool str_match(const std::string& str, const std::string& pattern);

bool isNumeric(const std::string& str);

bool isAlpha(const std::string& str);

bool isKeyword(const std::string& str);

bool isKeywordLiteral(const std::string& str);

bool isVariableDeclarationKeyword(const std::string& str);

bool isIdentifier(const std::string& str);

bool isSkippable(const std::string & str);