
#include <iostream>
#include <sstream>
#include <regex>
#include <types.hpp>
#include <constants.hpp>
#include <keywords.hpp>

std::vector<std::string> str_split(const std::string str, char delimitor)
{
	std::stringstream ss{ str };
	std::string line;
	std::vector<std::string> tokens{};

	while (std::getline(ss, line, delimitor)) {
		tokens.push_back(line);
	}

	return tokens;
}


void showTokenList(const TokenList & tokenList)
{
	std::cout << "TokenList {" << std::endl;
	for (auto token : tokenList) {
		std::cout << token << std::endl;
	}
	std::cout << "}" << std::endl;
}

bool str_match(const std::string& str, const std::string& pattern)
{
	std::regex regex(pattern, std::regex_constants::extended);

	return std::regex_match(str.begin(), str.end(), regex);
}

bool isNumeric(const std::string& str)
{
	return str_match(str, REGEX_NUMBER);
}

bool isAlpha(const std::string& str)
{
	return str_match(str, REGEX_STRING);
}

bool isKeyword(const std::string& str)
{
	for (auto it = std::begin(keywords); it != std::end(keywords); it++) {
		if ((*it) == str) {
			return true;
		}
	}

	return false;
}

bool isIdentifier(const std::string& str)
{
	return str_match(str, REGEX_IDENTIFIER);
}