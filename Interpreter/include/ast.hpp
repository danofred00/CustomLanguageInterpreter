#pragma once

#include <vector>
#include <string>
#include <types.hpp>

/*
	This represent a statement
*/
class Statement
{
public: 
	enum class NodeType {
		PROGRAM,
		/* Values */
		NUMBER_LITERAL,
		STRING_LITERAL,
		NULL_LITERAL,
		/* Identifier */
		IDENTIFIER,
		BINARY_EXPR,
		CALL_EXPR,
		UNARY_EXPR,
		FUNCTION_DECLARATION,
		EXPRESSION,
		UNKNOW,
	};

	virtual Statement::NodeType getType() = 0;

	virtual std::string toString() {
		return "Statement<>";
	}

	std::string nodeTypeToString(Statement::NodeType type);
};


class ProgramStatement : public Statement
{
public:

	~ProgramStatement();

	Statement::NodeType getType() override {
		return Statement::NodeType::PROGRAM;
	}

	std::vector<Statement *> getBody() const;

	void addChild(Statement * stmt);

	void killChildren();

	std::string toString() override;

private:
	std::vector<Statement *> children;
};


/*
	This is a representation of an expression
*/
class Expression : public Statement
{
public:
	Statement::NodeType getType() override {
		return Statement::NodeType::EXPRESSION;
	}
};

class BinaryExpression : public Expression
{
public:
	BinaryExpression(): Expression(), left{nullptr}, right{nullptr} {}

	BinaryExpression(Expression * left, Expression * right): Expression(), left{left}, right{right} {}

	~BinaryExpression();

	Statement::NodeType getType() override {
		return Statement::NodeType::BINARY_EXPR;
	}

	inline Expression * getLeft() const {
		return this->left;
	}

	inline void setLeft(Expression * expr) {
		this->left = expr;
	}

	inline Expression * getRight() const {
		return this->right;
	}
	
	inline void setRight(Expression * expr) {
		this->right = expr;
	}

	inline Operator getOperator() const {
		return this->op;
	}

	inline void setOperator(const Operator& op) {
		this->op = op;
	}

	std::string toString() override;

private: 
	Expression * left;
	Expression * right;
	Operator op;
};


/**
 * This expressing represent an identifier
 */
class Identifier : public Expression
{
public:
	Identifier(): Expression(), identifier{} {};
	
	Identifier(const std::string & str): Expression(), identifier{str} {};

	Statement::NodeType getType() override {
		return Statement::NodeType::IDENTIFIER;
	}

	inline void setIdentifier(const std::string & str){
		this->identifier = std::move(str);
	}

	inline std::string getIdentifier() const {
		return this->identifier;
	}

	std::string toString() override {
		return "Identifier<symbol='" + identifier + "'>";
	}

private:
	std::string identifier;
};

/*
	This represent a value expression
*/
template<typename T>
class ValueExpression : public Expression
{
public:

	ValueExpression() : Expression(), value{} {};

	ValueExpression(const T x) : Expression(), value{x} {};

	inline void setValue(const T x) { this->value = x; }

	inline T getValue() const { return this->value; }

private:
	T value;
};


/*
	This represent a number expression
*/
template<typename T>
class NumberLiteral : public ValueExpression<T>
{
public:
	NumberLiteral(const T x) : ValueExpression<T>(x) {};

	Statement::NodeType getType() override {
		return Statement::NodeType::NUMBER_LITERAL;
	}
};


/*
	An Integer expression
*/
class IntegerLiteral : public NumberLiteral<int> 
{
	IntegerLiteral(int x) : NumberLiteral<int>(x) {};

	std::string toString() override { 
		return "IntegerLiteral<value='" + std::to_string(getValue()) + "'>"; 
	}
};

/*
	An Floating expression
*/
class FloatLiteral : public NumberLiteral<float>
{ 
public:
	FloatLiteral(float x) : NumberLiteral<float>(x) {};
	
	std::string toString() override { 
		return "FloatLiteral<value='" + std::to_string(getValue()) + "'>"; 
	}
};

/*
	An string expression
*/
class StringLiteral : public ValueExpression<std::string>
{
public:
	StringLiteral(const std::string& str) : ValueExpression(str) {};

	Statement::NodeType getType() override {
		return Statement::NodeType::STRING_LITERAL;
	}

	std::string toString() override { 
		return "StringLiteral<value='" + getValue() + "'>"; 
	}
};

/**
 * A NULL expression
*/
class NullLiteral : public StringLiteral
{
public:
	NullLiteral(): StringLiteral("null") {};
	
	NodeType getType() {
		return Statement::NodeType::NULL_LITERAL;
	}

	std::string toString() override {
		return "NullLiteral<value='null'>";
	}
};
