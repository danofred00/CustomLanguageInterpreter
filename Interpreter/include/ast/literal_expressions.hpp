/**
 * @file ast/literal_expressions.hpp
 * @brief Contains the definition of literal expressions
 */

#pragma once

#include <ast/core_statements.hpp>

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

	NodeType getType() override {
		return NodeType::NUMBER_LITERAL;
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

	NodeType getType() override {
		return NodeType::STRING_LITERAL;
	}

	std::string toString() override { 
		return "StringLiteral<value='" + getValue() + "'>"; 
	}
};

/**
 * A NULL expression
*/
/* class NullLiteral : public StringLiteral
// {
// public:
// 	NullLiteral(): StringLiteral("null") {};
	
// 	NodeType getType() {
// 		return NodeType::NULL_LITERAL;
// 	}

// 	std::string toString() override {
// 		return "NullLiteral<value='null'>";
// 	}
// };
*/

/**
 * A boolean expression
*/
class BoolLiteral : public ValueExpression<bool>
{
public:
	BoolLiteral(bool x) : ValueExpression(x) {};

	NodeType getType() {
		return NodeType::BOOL;
	}

	std::string toString() override {
		return "BoolLiteral<value='" + std::to_string(getValue()) + "'>";
	}
};

