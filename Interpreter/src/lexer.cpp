
#include <lexer.hpp>
#include <utils.hpp>

Lexer::Lexer() { }

void Lexer::tokenize(const std::string & input)
{
	tokens = std::vector<Token>();
	std::string src { input };
	
	auto begin = std::begin(src);
	auto end = std::end(src);
	
	while (begin != end) {
		std::string item = atos(*begin);
		if (item == "(") {
			// handle open bracket
			tokens.push_back(Token(TokenType::OPEN_BRACKET, atos(*(begin++))));
		} else if (item == ")") {
			// handle close bracket
			tokens.push_back(Token(TokenType::CLOSE_BRACKET, atos(*(begin++))));
		} else if ((item == "+") || (item == "-") || (item == "/") || (item == "*") || (item == "%")) {
			// handle binairies operators
			tokens.push_back(Token(TokenType::BINARY_OPERATOR, atos(*(begin++))));
		} else if (item == "=") {
			// handle equal
			tokens.push_back(Token(TokenType::EQUALS, atos(*(begin++))));
		} else if (item == ";") {
			// handle semicolon
			tokens.push_back(Token(TokenType::SEMICOLON, atos(*(begin++))));
		} else if (item == ",") {
			// handle comma
			tokens.push_back(Token(TokenType::COMMA, atos(*(begin++))));
		} else if (isSkippable(item)) {
			// NOTHING to do
			begin++;
		} else {
			// HANDLE MULTICHARACTER TOKENS

			// parse numbers
			if (std::isdigit(item[0])) {
				std::string num = "";
				while(begin < end && std::isdigit(*begin)) { 
					num += *(begin++); 
				}
				tokens.push_back(Token(TokenType::NUMBER_LITERAL, num));
			}
			// handle identifiers and keywords
			else if (std::isalpha(item[0]) || item[0] == '_') {
				std::string ident = "";
				while ((begin < end) && (std::isalpha(*begin) || (*begin) == '_')) {
					ident += *(begin++);
				}
				
				// handle keywords
				if (isKeyword(ident)) {
					if (isKeywordLiteral(ident)) {
						tokens.push_back(Token(TokenType::RESERVED_LITERAL, ident));
					} else if (isVariableDeclarationKeyword(ident)) {
						tokens.push_back(Token(TokenType::VAR_DECLARATION, ident));
					} else { 
						tokens.push_back(Token(TokenType::RESERVED, ident));
					}
				} else if(isIdentifier(item)) {
					// handle identifier
					tokens.push_back(Token(TokenType::IDENTIFIER, ident));
				}
			
			} 
			// handle string vars
			else if (isStringDeclarator(item)) {
				std::string str = "";
				begin++; // skip the first quote
				while ((begin < end) && (atos(*begin) != item)) {
					str += *(begin++);
				}
				begin++; // skip the last quote
				
				tokens.push_back(Token(TokenType::STRING_LITERAL, str));
			}
			else {
				std::cerr << "Syntax Error: Unexpected Identifier '" + item + "'" << std::endl;
				std::exit(EXIT_FAILURE);
			}
		}
	}
	
	tokens.push_back(Token(TokenType::END_OF_FILE, "EOF"));
}

TokenList * Lexer::getTokens()
{
	return &tokens;
}