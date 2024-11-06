/*
	The parser is the program responsible to generate AST
	by parsing tokens into statements.

	@author Daniel Leussa
*/

#pragma once

#include <lexer.hpp>
#include <ast.hpp>
#include <types.hpp>

class Parser
{
public:
	Parser();
	~Parser();
	ProgramStatement * produceAST(const std::string& sourceCode);

	Statement *	parseStatement();
	Expression * parseExpression();

	/**
	 * Like an assertion
	 */
	void expectToken(TokenType expected, TokenType type, const char * message);

private:
	bool isEOF();
	Token consumeToken();

	/**
	 * Primary expresssion is something like a value, an identifier
	 * Ex: x, 10, 'str'
	 */
	Expression * parsePrimaryExpression();

	/**
	 * Additive expression are expression used for addition or substraction
	 * Ex: x + 10 - 5
	 */
	Expression * parseAdditiveExpressions();

	/**
	 * Multiplicative expressions are expressions for multiplication
	 * or division. Ex: 110 / 2 * 4
	 */
	Expression * parseMultiplicativeExpression();

	/**
	 * Parse a variable declaration
	 */
	Statement * parseVariableDeclaration();

	/**
	 * Parse an assignment expression
	 * Ex: x = 10; x = left = 10; x[1] = 0; etc...
	 */
	Expression * parseAssignmentExpression();

	/**
	 * TODO: refactor this method
	 * parse function call expression
	 */
	Expression * parseFunctionCallExpression();

	/**
	 * Parse a function call expression
	 * Ex: print('Hello, World!'), add(10, 20), hello(), etc...
	 */
	Expression * parseCallExpression(Expression *caller);

	Lexer lexer;
	TokenList * tokens;
	TokenList::iterator pos;
	ProgramStatement* program;
};