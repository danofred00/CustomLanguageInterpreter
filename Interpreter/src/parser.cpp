#include <parser.hpp>

Parser::Parser() : program { nullptr }
{
	lexer = Lexer();
	tokens = lexer.getTokens();
	pos = std::begin(*tokens);
}

Parser::~Parser() {
	delete program;
}

ProgramStatement * Parser::produceAST(const std::string& sourceCode)
{
	lexer.tokenize(std::move(sourceCode));
	pos = std::begin(*tokens);
	program = new ProgramStatement();
	
	while (!isEOF()) {
		auto stmt = std::move(parseStatement());
		program->addChild(stmt);
	}

	return program;
}

Statement* Parser::parseStatement()
{
	return parseExpression();
}

Expression* Parser::parseExpression()
{
	// TODO: parse statament
	return parseMultiplicativeExpression();
}

Expression* Parser::parsePrimaryExpression()
{
	auto token = consumeToken();
	Expression * value = nullptr;

	switch (token.type)
	{
	// handle itentifiers
	case TokenType::IDENTIFIER:
		return static_cast<Expression*>(new Identifier(token.value));
	// handle numbers for the moment
	case TokenType::NUMBER_LITERAL:
		return static_cast<Expression*>(new FloatLiteral(static_cast<float>(std::atof(token.value.c_str()))));
	case TokenType::STRING_LITERAL:
		return static_cast<Expression*>(new StringLiteral(token.value));
	case TokenType::OPEN_BRACKET:
		value = parseExpression();
		expectToken(TokenType::CLOSE_BRACKET, consumeToken().type, "Expected to close bracket inside an expression.");
		return value;
	default:
		std::cerr << "Syntax Error: Unable to parse Token " << token << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

Expression * Parser::parseAdditiveExpressions()
{
	auto binary = new BinaryExpression();
	Expression* left = parsePrimaryExpression();

	while((pos->value == "+") || (pos->value == "-")) {
		auto op = consumeToken().value;
		auto right = parsePrimaryExpression();
		
		// update values
		binary->setLeft(left);
		binary->setRight(right);
		binary->setOperator(op);
		left = new BinaryExpression(*binary);
	}

	return left;
}

Expression * Parser::parseMultiplicativeExpression()
{
	auto binary = new BinaryExpression();
	Expression* left = parseAdditiveExpressions();

	while((pos->value == "*") || (pos->value == "/") || (pos->value == "%")) {
		auto op = consumeToken().value;
		auto right = parseAdditiveExpressions();
		
		// update values
		binary->setLeft(left);
		binary->setRight(right);
		binary->setOperator(op);
		left = new BinaryExpression(*binary);
	}

	return left;
}

bool Parser::isEOF()
{
	if (pos == std::end(*tokens)) {
		return true;
	}

	if (tokens->size() > 0 && (*pos).type == TokenType::END_OF_FILE) {
		return true;
	}

	return false;
}

void Parser::expectToken(TokenType expected, TokenType type, const char * message) {
	if(type != expected) {
		std::cerr << "Syntax Error: " << message << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

Token Parser::consumeToken() {
	
	if (tokens->size() == 0) {
		return Token(TokenType::UNKNOW, "UNKNOW");
	}
	return *(pos++);
}
