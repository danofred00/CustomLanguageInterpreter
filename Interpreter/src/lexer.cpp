
#include <lexer.hpp>
#include <utils.hpp>

Lexer::Lexer() { }

void Lexer::tokenize(const std::string & input)
{
	tokens = std::vector<Token>();
	auto src = str_split(input, ' ');

	for (auto item : src) {
		if (item == "(") {
			// handle open bracket
			tokens.push_back(Token(TokenType::OPEN_BRACKET, item));
		} else if (item == ")") {
			// handle close bracket
			tokens.push_back(Token(TokenType::CLOSE_BRACKET, item));
		} else if ((item == "+") || (item == "-") || (item == "/") || (item == "*")) {
			// handle binairies operators
			tokens.push_back(Token(TokenType::BINARY_OPERATOR, item));
		} else if (item == "=") {
			// handle equal
			tokens.push_back(Token(TokenType::EQUALS, item));
		} else if (item == ";") {
			// handle semicolon
			tokens.push_back(Token(TokenType::SEMICOLON, item));
		} else {
			// handle multicharacters tokens
			if (isNumeric(item)) {
				tokens.push_back(Token(TokenType::NUMBER_LITERAL, item));
			}
			else if (isKeyword(item)) {
				// handle keywords
				tokens.push_back(Token(TokenType::RESERVED, item));
			}
			else if(isIdentifier(item)) {
				// handle identifier
				tokens.push_back(Token(TokenType::IDENTIFIER, item));
			}
			else if (isAlpha(item)) {
				// handle string
				tokens.push_back(Token(TokenType::STRING_LITERAL, item));
			}
			else {
				std::cerr << "Syntax Error: Unexpected Identifier " << item << std::endl;
				std::exit(EXIT_FAILURE);
			}
		}
	}

	tokens.push_back(Token(TokenType::END_OF_FILE, "EOF"));
}

TokenList Lexer::getTokens()
{
	return tokens;
}