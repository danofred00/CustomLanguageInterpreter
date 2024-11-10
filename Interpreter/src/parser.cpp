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
	lexer.tokenize(sourceCode);
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
		case TokenType::FN:
			return parseFunctionDeclaration();
		case TokenType::RETURN:
			return parseReturnStatement();
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
	auto [identifier, type] = parseArgDeclaration(token);
	
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

std::tuple<std::string, VariableDeclaration::ValueType> Parser::parseArgDeclaration(Token token)
{
	std::string name;
	VariableDeclaration::ValueType type = VariableDeclaration::ValueType::NUMBER;
	
	if (token.value == "string") {
		type = VariableDeclaration::ValueType::STRING;
	}
	else if (token.value == "bool") {
		type = VariableDeclaration::ValueType::BOOLEAN;
	}
	else {
		// nothing to do, because we knows that the token is a var declaration
	}
	// ensure that the next token is an identifier
	expectToken(TokenType::IDENTIFIER, (*pos).type, std::string("Expected an identifier after a variable declaration.").c_str());
	name = consumeToken().value;

	return std::make_tuple(name, type);
}

Statement * Parser::parseFunctionDeclaration()
{
	consumeToken(); // consume the function token
	auto fn = new FunctionDeclaration();

	// ensure that the next token is an identifier
	auto token = consumeToken();
	expectToken(TokenType::IDENTIFIER, token.type, "Expected an identifier after a function declaration.");
	fn->setName(token.value);

	// ensure the next token is an open paren
	expectToken(TokenType::OPEN_PAREN, (*pos).type, "Expected an open paren '(' inside a function declaration");
	consumeToken(); // consume the open paren

	// read arguments
	while (!isEOF() && (*pos).type != TokenType::CLOSE_PAREN) {
		auto [name, type] = parseArgDeclaration(consumeToken());
		fn->getParameters().push_back(new Argument(name, type));

		// expect the next should be a comma
		if ((*pos).type == TokenType::COMMA) {
			consumeToken();
		}
	}
	// expect close paren
	expectToken(TokenType::CLOSE_PAREN, (*pos).type, "Expected a ')' after var declaration inside a function declaration");
	consumeToken();

	// ensure the next is a block expression
	expectToken(TokenType::OPEN_BRACKET, (*pos).type, "Expected an open bracket '{' after a function declaration.");
	fn->setBody(static_cast<BlocStatement *>(parseBlockStatement()));

	return fn;
}

Statement * Parser::parseReturnStatement() 
{
	consumeToken(); // skip the return token
	auto left = parseExpression();
	return new ReturnStatement(left);
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
	case TokenType::OPEN_PAREN:
		value = parseExpression();
		expectToken(TokenType::CLOSE_PAREN, consumeToken().type, "Expected to close bracket inside an expression.");
		return value;
	case TokenType::RESERVED_LITERAL:
		return static_cast<Expression*>(new ReservedExpression(token.value));
	case TokenType::CONDITION_IF:
		return parseConditionalExpression();
	case TokenType::LOGIC_NOT:
		value = parseExpression();
		return static_cast<Expression *>(new Not(value));
	default:
		std::cerr << "Syntax Error: Unable to parse Token " << token << std::endl;
		std::exit(EXIT_FAILURE);
	}

	return value;
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
	Expression* left = parseFunctionCallExpression();

	while((pos->value == "*") || (pos->value == "/") || (pos->value == "%")) {
		auto op = consumeToken().value;
		auto right = parseFunctionCallExpression();
		
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
	auto left = parseLogicComparaisonExpression();

	if((*pos).type == TokenType::EQUALS) {
		consumeToken(); // skip the equals caracter
		auto value = parseLogicComparaisonExpression();
		value = new AssignmentExpression(left, value);

		// remove the last semicolon expression
		if((*pos).type == TokenType::SEMICOLON) {
			consumeToken();
		}

		return value;
	}

	return left;
}

Expression * Parser::parseCallExpression(Expression *caller)
{
	// expect the caller is an identier
	if(caller->getType() != NodeType::IDENTIFIER) {
		std::cerr << "Syntax Error: Expected an identifier as a caller in a function call." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	consumeToken(); // skip the open bracket
	std::vector<Expression *> args {};

	// parse arguments
	while((*pos).type != TokenType::CLOSE_PAREN) {
		auto arg = parseExpression();
		args.push_back(arg);
		
		// expect the next should be a comma
		if((*pos).type == TokenType::COMMA) {
			consumeToken();
		}
	}

	// expect the next should be a close bracket
	expectToken(TokenType::CLOSE_PAREN, (*pos).type, "Expected a close parenthese after a function call.");

	consumeToken(); // skip the close bracket

	return new CallFunctionExpression(caller, args);
}

Expression * Parser::parseFunctionCallExpression()
{
	Expression * left = parsePrimaryExpression();

	if((*pos).type == TokenType::OPEN_PAREN) {
		return parseCallExpression(left);
	}

	return left;
}

Expression * Parser::parseConditionalExpression()
{
	Expression * condition = nullptr;
	Statement * thenBlock = nullptr;
	Statement * elseBlock = nullptr;

	// expect the next is an open_paren token
	expectToken(TokenType::OPEN_PAREN, (*pos).type, "Expect '(' after if conditional statement");
	consumeToken(); // read the open paren

	// read condition
	condition = parseLogicComparaisonExpression();
	// expect close parent after conditional
	expectToken(TokenType::CLOSE_PAREN, (*pos).type, "Expect ')' inside if conditional statement");
	consumeToken(); // read the close paren

	// read the thenBlock
	thenBlock = parseBlockStatement();

	// check if there is an else statement, if true, just run it
	if((*pos).type == TokenType::CONDITION_ELSE) {
		consumeToken(); // read the else token
		// check if is another if conditionnal
		if((*pos).type == TokenType::CONDITION_IF) {
			elseBlock = parsePrimaryExpression();
		} else {
			elseBlock = parseBlockStatement();
		}
	}

	return new ConditionalExpression(condition, thenBlock, elseBlock);
}

Statement * Parser::parseBlockStatement()
{
	BlocStatement * block = new BlocStatement();
	
	expectToken(TokenType::OPEN_BRACKET, (*pos).type, "Expect open bracket '{' before another block statement");
	consumeToken(); // read the open bracket
	
	// read the body
	while (!isEOF() && (*pos).type != TokenType::CLOSE_BRACKET)
	{
		block->addChild(parseStatement());
	}
	
	// expect close paren and open_bracket
	expectToken(TokenType::CLOSE_BRACKET, (*pos).type, "Expect close bracket '}' after block statement");
	consumeToken(); // read the close bracket
	
	return static_cast<Statement *>(block);
}

Expression * Parser::parseLogicComparaisonExpression()
{
	Expression * left = parseAdditiveExpressions();
	Expression * rigth = nullptr;
	auto token = (*pos);

	switch (token.type)
	{
		case TokenType::LOGIC_AND:
		case TokenType::LOGIC_EQUAL:
		case TokenType::LOGIC_GREATER:
		case TokenType::LOGIC_GREATER_EQUAL:
		case TokenType::LOGIC_LOWER:
		case TokenType::LOGIC_LOWER_EQUAL:
		case TokenType::LOGIC_OR:
			token = consumeToken();
			rigth = parseExpression();
			left = new LogicBinary(left, rigth, token.value);
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
