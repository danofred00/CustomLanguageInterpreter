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

	Lexer lexer;
	TokenList * tokens;
	TokenList::iterator pos;
	ProgramStatement* program;
};