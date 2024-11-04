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
	auto current = *pos;

	switch (current.type)
	{
		case TokenType::VAR_DECLARATION:
			return parseVariableDeclaration();
		default:
			return parseExpression();
	}
}

Expression* Parser::parseExpression()
{
	return parseAssignmentExpression();
}

Statement * Parser::parseVariableDeclaration() 
{
	auto token = consumeToken();
	VariableDeclaration::ValueType type = VariableDeclaration::ValueType::NUMBER;
	std::string identifier {};
	if(token.value == "string") {
		type = VariableDeclaration::ValueType::STRING;
	} else if(token.value == "bool") {
		type = VariableDeclaration::ValueType::BOOLEAN;
	} else {
		// nothing to do, because we knows that the token is a var declaration
	}
	// ensure that the next token is an identifier
	token = consumeToken();
	expectToken(TokenType::IDENTIFIER, token.type, std::string("Expected an identifier after a variable declaration.").c_str());
	identifier = token.value;

	token = consumeToken();
	// handle if its a semicolon
	if(token.type == TokenType::SEMICOLON) {
		// TODO: handle constant declaration
		return new VariableDeclaration(identifier, type, nullptr);
	}

	// ensure that the next token is an equal sign
	expectToken(TokenType::EQUALS, token.type, "Expected an equal sign or semicolon after an identifier in a variable declaration.");

	// evaluate the assigned expression
	auto value = parseExpression();

	// ensure that the next token is a semicolon
	expectToken(TokenType::SEMICOLON, consumeToken().type, "Expected a semicolon after an expression in a variable declaration.");

	return new VariableDeclaration(identifier, type, value);
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
	case TokenType::RESERVED_LITERAL:
		return static_cast<Expression*>(new ReservedExpression(token.value));
	default:
		std::cerr << "Syntax Error: Unable to parse Token " << token << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

Expression * Parser::parseAdditiveExpressions()
{
	auto binary = new BinaryExpression();
	Expression* left = parseMultiplicativeExpression();

	while((pos->value == "+") || (pos->value == "-")) {
		auto op = consumeToken().value;
		auto right = parseMultiplicativeExpression();
		
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
	Expression* left = parsePrimaryExpression();

	while((pos->value == "*") || (pos->value == "/") || (pos->value == "%")) {
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

Expression * Parser::parseAssignmentExpression()
{
	auto left = parseAdditiveExpressions();

	if((*pos).type == TokenType::EQUALS) {
		consumeToken(); // skip the equals caracter
		auto value = parseAssignmentExpression();
		value = new AssignmentExpression(left, value);

		// remove the last semicolon expression
		if((*pos).type == TokenType::SEMICOLON) {
			consumeToken();
		}

		return value;
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
