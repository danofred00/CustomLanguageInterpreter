/**
 * @file ast/logic_expressions.hpp
 * This contains definitions for logic and comparaison operators
 */

#pragma once

#include <ast/literal_expressions.hpp>

/**
 * Handle not expression
 */
class Not : public ValueExpression<Expression *>
{
public:
	Not(Expression * value) : ValueExpression(value) {};

	NodeType getType() override {
		return NodeType::NOT;
	}

	std::string toString() override {
		return "NotLiteral<value=" + getValue()->toString() + ">";
	}
};

/**
 * Handle all logic or comparaison binary expression
 */
class LogicBinary : public BinaryExpression
{
public: 
	using BinaryExpression::BinaryExpression;
	
    NodeType getType() override {
        return NodeType::LOGIC_BINARY;
    }
};

