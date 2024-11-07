
#include <lexer.hpp>
#include <utils.hpp>
#include <keywords.hpp>

Lexer::Lexer() { }

void Lexer::tokenize(const std::string & input)
{
	tokens = std::vector<Token>();
	std::string src { input };
	
	begin = std::begin(src);
	end = std::end(src);
	
	while (begin != end) {
		std::string item = atos(*begin);
		// handle single char operators
		if (isSingleOperator(item)) {
			auto it = singleOperators.find(item);
			tokens.push_back(Token(it->second, item));
			begin++;
		} else if (item == "=") {
			handleEquals();
		} else if (isSkippable(item)) {
			// NOTHING to do
			begin++;
		} else if (item == "#") {
			// handle comments single line comments
			while(begin < end && (*begin) != '\n') begin++;
		} else if(isComparaisonOperator(item)) {
			// handle comparaison operator
			handleComparaisonOperators();
		} else {
			// HANDLE MULTICHARACTER TOKENS
			handleMultivaluesCharacters(item);
		}
	}
	
	tokens.push_back(Token(TokenType::END_OF_FILE, "EOF"));
}

void Lexer::handleComparaisonOperators() {
	if((*begin++) == '>') {
		if((*begin) == '=') {
			tokens.push_back(Token(TokenType::LOGIC_GREATER_EQUAL, ">="));
			begin++;
		} else {
			tokens.push_back(Token(TokenType::LOGIC_GREATER, ">"));
		} 
	} else {
		if((*begin) == '=') {
			tokens.push_back(Token(TokenType::LOGIC_LOWER_EQUAL, "<="));
			begin++;
		} else {
			tokens.push_back(Token(TokenType::LOGIC_LOWER, "<"));
		} 
	}
}

void Lexer::handleEquals() {
	// handle equal
	auto c = atos(*(begin++));
	// check if the next is also equal char
	if(atos(*begin) == "=") {
		tokens.push_back(Token(TokenType::LOGIC_EQUAL, "=="));
		begin++; // skip the current char
	} else {
		tokens.push_back(Token(TokenType::EQUALS, c));
	}
}

void Lexer::handleMultivaluesCharacters(const std::string & item) {
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
		
		if(isKeyword(ident)) {
			auto it = keywords.find(ident);
			tokens.push_back(Token(it->second, ident));
		} else if(isIdentifier(item)) {
			// handle identifier
			tokens.push_back(Token(TokenType::IDENTIFIER, ident));
		} else { 
			throw std::runtime_error("Syntax Error: Unexpected Identifier '" + ident + "'");
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

TokenList * Lexer::getTokens()
{
	return &tokens;
}