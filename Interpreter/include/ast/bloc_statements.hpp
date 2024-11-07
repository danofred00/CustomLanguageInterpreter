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

class FunctionDeclaration : public Statement
{
public:
    FunctionDeclaration() : Statement(), body{ nullptr } {};

    FunctionDeclaration(const std::string& name, std::vector<Statement*> parameters)
        : Statement(), name{ name }, parameters{ parameters }, body{ nullptr } {};
    
    NodeType getType() override {
        return NodeType::FUNCTION_DECLARATION;
    }

    const std::string getName() const {
        return this->name;
    };

    void setName(const std::string& name) {
        this->name = name;
    }

	std::vector<Statement*> & getParameters() {
		return this->parameters;
	}

    BlocStatement * getBody() const {
        return this->body;
    }

    void setBody(BlocStatement* body) {
        this->body = body;
    }

    std::string toString() override {
        std::string str = "FunctionDeclaration<" + name + ", [";
        for (auto param : parameters) {
            str += param->toString() + ", ";
        }
        str += "]>";
        return str;
    }

private:
    std::string name;
    std::vector<Statement *> parameters;
    BlocStatement* body;
};

/**
 * Function argument
 * Ex: number x, number y, string foo
*/
class Argument : public Statement
{
public:
	Argument(const std::string& name, VariableDeclaration::ValueType type)
        : Statement(), name{ name }, type{ type } {};

	NodeType getType() override {
		return NodeType::FVAR_DECLARATION;
	}

	const std::string getName() const {
		return this->name;
	}

	void setName(const std::string& name) {
		this->name = name;
	}

	VariableDeclaration::ValueType getValueType() {
		return this->type;
	}

    void setValueType(VariableDeclaration::ValueType type) {
        this->type = type;
    }


	std::string toString() override {
		return "FunctionArgument<" + name + ", " + std::to_string(static_cast<int>(type)) + ">";
	}
private:
	std::string name;
    VariableDeclaration::ValueType type;
};

/**
 * Return statement
 * Ex: return 5; return foo()
 */
class ReturnStatement : public ValueExpression<Expression*>
{
public:
	ReturnStatement(Expression* value = nullptr) : ValueExpression(value) {};

	NodeType getType() override {
		return NodeType::RETURN;
	}

	std::string toString() override {
		return "ReturnStatement<" + getValue()->toString() + ">";
	}
};


