#pragma once

#include <vector>
#include <types.hpp>

/*
	This represent a statement
*/
class Statement
{
public: 
	enum class NodeType {
		PROGRAM,
		NUMBER_LITERAL,
		STRING_LITERAL,
		IDENTIFIER,
		BINARY_EXPR,
		CALL_EXPR,
		UNARY_EXPR,
		FUNCTION_DECLARATION,
		EXPRESSION,
		UNKNOW,
	};

	virtual Statement::NodeType getType() = 0;
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

private:
	std::vector<Statement *> children;
};


/*
	This is a representation of an expression
*/
class Expression : public Statement
{
	Statement::NodeType getType() override {
		return Statement::NodeType::EXPRESSION;
	}
};

class BinaryExpression : public Expression
{
public:

	Statement::NodeType getType() override {
		return Statement::NodeType::BINARY_EXPR;
	}

	inline Expression getLeft() const {
		return this->left;
	}

	inline void setLeft(const Expression & expr) {
		this->left = expr;
	}

	inline Expression getRight() const {
		return this->right;
	}
	
	inline void setRight(const Expression& expr) {
		this->right = expr;
	}

	inline Operator getOperator() const {
		return this->op;
	}

	inline void setOperator(const Operator& op) {
		this->op = op;
	}

private: 
	Expression left;
	Expression right;
	Operator op;
};


class Identifier : public Expression
{
	Statement::NodeType getType() override {
		return Statement::NodeType::IDENTIFIER;
	}

	inline void setIdentifier(const std::string & str){
		this->identifier = std::move(str);
	}

	inline std::string getIdentifier() const {
		return this->identifier;
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

	inline void setValue(const T x) {
		this->value = x;
	}

	inline T getValue() const {
		return this->value;
	}

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

	Statement::NodeType getType() override {
		return Statement::NodeType::NUMBER_LITERAL;
	}
};


/*
	An Integer expression
*/
class IntegerLiteral : public NumberLiteral<int> { };

/*
	An Floating expression
*/
class FloatLiteral : public NumberLiteral<float> { };

/*
	An string expression
*/
class StringLiteral : public ValueExpression<std::string>
{
public:
	Statement::NodeType getType() override {
		return Statement::NodeType::STRING_LITERAL;
	}
};

