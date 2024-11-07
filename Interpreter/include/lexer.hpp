#pragma once
#include <string>
#include <vector>
#include <types.hpp>

class Lexer
{
public:
	Lexer();
	void tokenize(const std::string& input);
	TokenList * getTokens();

private:
	void handleEquals();
	void handleLogicOperators(const std::string & str);
	void handleMultivaluesCharacters(const std::string & str);
	void handleComparaisonOperators();

	TokenList tokens;
	std::string::iterator begin, end;
};