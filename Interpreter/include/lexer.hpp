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
	TokenList tokens;
};