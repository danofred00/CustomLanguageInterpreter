/**
 * @file ast/function_expressions.hpp
 * @brief Contains the definition of functional expressions
 */

#pragma once

#include <ast/core_statements.hpp>

/**
 * Block statement
 * Represent of isolated statemet blocks
 */
class BlocStatement : public ProgramStatement
{
public:
    NodeType getType() {
        return NodeType::BLOCK_PROGRAM;
    }
};

/**
 * Conditional Expression
 */
class ConditionalExpression : public Expression
{
public:
    ConditionalExpression(Expression * condition, Statement * thenBlock, Statement * elseBlock)
        : Expression(), condition{condition}, thenBlock{thenBlock}, elseBlock{elseBlock} {};

    ~ConditionalExpression();

    inline void setCondition(Expression * expr) {
        this->condition = expr;
    }

    inline Expression * getCondition() const {
        return this->condition;
    }

    inline void setThenBlock(Statement * stmt) {
        this->thenBlock = stmt;
    }

    inline Statement * getThenBlock() const {
        return this->thenBlock;
    }

    inline void setElseBlock(Statement * stmt) {
        this->elseBlock = stmt;
    }

    inline Statement * getElseBlock() const {
        return this->elseBlock;
    }

    NodeType getType() override {
        return NodeType::CONDITIONAL;
    }

    std::string toString() override;
private:
    Expression * condition;
    Statement * thenBlock;
    Statement * elseBlock;
};

