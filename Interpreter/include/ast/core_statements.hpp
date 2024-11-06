/**
 * @file ast/core_expressions.hpp
 * @brief Contains the definition core expressions 
*/

#pragma once

#include <vector>
#include <string>
#include <types.hpp>
#include <ast/types.hpp>

/*
	This represent a statement
*/
class Statement
{
public: 
	virtual NodeType getType() = 0;

	virtual std::string toString() {
		return "Statement<>";
	}

	static std::string nodeTypeToString(NodeType type);
};

/**
 * This is the program statement, used to create routines and others
 */
class ProgramStatement : public Statement
{
public:

	~ProgramStatement();

	NodeType getType() override {
		return NodeType::PROGRAM;
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
	NodeType getType() override {
		return NodeType::EXPRESSION;
	}
};


class BinaryExpression : public Expression
{
public:
	BinaryExpression(): Expression(), left{nullptr}, right{nullptr} {}

	BinaryExpression(Expression * left, Expression * right): Expression(), left{left}, right{right} {}

	~BinaryExpression();

	NodeType getType() override {
		return NodeType::BINARY_EXPR;
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

	NodeType getType() override {
		return NodeType::IDENTIFIER;
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

class ReservedExpression : public Expression
{
public: 
	ReservedExpression(): Expression(), keyword{} {};
	ReservedExpression(std::string & keyword): Expression(), keyword{keyword} {};

	NodeType getType() override {
		return NodeType::RESERVED_LITERAL;
	};

	inline void setKeyword(const std::string & str) {
		this->keyword = str;
	}
	
	inline std::string getKeyword() const {
		return this->keyword;
	}

private:
	std::string keyword;
};

/* Some Statements */
class VariableDeclaration : public Statement
{
public: 
	enum class ValueType {
		NUMBER,
		STRING,
		BOOLEAN,
		// TODO: add more types
	};
	VariableDeclaration(): Statement(), identifier{}, value{nullptr}, type{ValueType::NUMBER} {};

	VariableDeclaration(const std::string & identifier, ValueType type, Expression * value): Statement(), identifier{identifier}, type{type}, value{value} {};

	~VariableDeclaration();

	NodeType getType() override {
		return NodeType::VAR_DECLARATION;
	}

	inline void setIdentifier(const std::string & str) {
		this->identifier = str;
	}

	inline std::string getIdentifier() const {
		return this->identifier;
	}

	inline void setValue(Expression * stmt) {
		this->value = stmt;
	}

	inline Expression * getValue() const {
		return this->value;
	}

	inline void setValueType(ValueType type) {
		this->type = type;
	}

	inline ValueType getValueType() const {
		return this->type;
	}

	std::string toString() override;

	static std::string valueTypeToString(ValueType type) {
		switch (type)
		{
		case ValueType::NUMBER:
			return "number";
		case ValueType::STRING:
			return "string";
		case ValueType::BOOLEAN:
			return "boolean";
		default:
			return "unknow";
		}
	}

private:
	std::string identifier;
	Expression * value;
	ValueType type;
	// TODO: const value support
};

class AssignmentExpression : public Expression
{
public: 
	AssignmentExpression(Expression * left = nullptr, Expression * value = nullptr)
		: Expression(), left{left}, value{value} {};

	NodeType getType() override {
		return NodeType::ASSIGNMENT;
	}

	Expression * getLeft() const { return left; }
	void setLeft(Expression * expr) { left = expr; }

	Expression * getValue() const { return value; }
	void setValue(Expression * expr) { value = expr; }

	std::string toString() {
		std::stringstream ss {};
		ss << "AssignmentExpression<\n\tleft=" << left->toString();
		ss << "\n\tvalue=" << value->toString() << "\n>";
		return ss.str();
	}

private:
	Expression * left;
	Expression * value;
};

class CallFunctionExpression : public Expression
{
public:
	
	CallFunctionExpression(Expression * caller, std::vector<Expression *> args)
		: Expression(), caller{caller}, args{args} {};
	
	~CallFunctionExpression();

	inline void setCaller(Expression * expr) {
		this->caller = expr;
	}

	inline Expression * getCaller() const {
		return this->caller;
	}

	inline void addArgument(Expression * expr) {
		this->args.push_back(expr);
	}

	inline std::vector<Expression *> getArguments() const {
		return this->args;
	}

	NodeType getType() override {
		return NodeType::CALL_EXPR;
	}

	std::string toString() override;

private:
	Expression * caller;
	std::vector<Expression *> args;
};


