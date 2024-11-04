#include <ast.hpp>
#include <sstream>

/*
	Statement class routines
*/
std::string Statement::nodeTypeToString(Statement::NodeType type)
{
	switch (type)
	{
	case NodeType::PROGRAM:
		return "PROGRAM";
	case NodeType::NUMBER_LITERAL:
		return "NUMBER_LITERAL";
	case NodeType::STRING_LITERAL:
		return "STRING_LITERAL";
	case NodeType::IDENTIFIER:
		return "IDENTIFIER";
	case NodeType::BINARY_EXPR:
		return "BINARY_EXPR";
	case NodeType::CALL_EXPR:
		return "CALL_EXPR";
	case NodeType::UNARY_EXPR:
		return "UNARY_EXPR";
	case NodeType::FUNCTION_DECLARATION:
		return "FUNCTION_DECLARATION";
	case NodeType::EXPRESSION:
		return "EXPRESSION";
	case NodeType::UNKNOW:
	default:
		return "UNKNOW";
	}
}


ProgramStatement::~ProgramStatement()
{
	killChildren();
}

void ProgramStatement::killChildren()
{
	auto begin = std::begin(children);
	while (begin != std::end(children)) {
		delete* begin;
		(*begin) = nullptr;
		begin++;
	}
}

void ProgramStatement::addChild(Statement * stmt) {
	children.push_back(stmt);
}

std::vector<Statement*> ProgramStatement::getBody() const {
	return children;
}

std::string ProgramStatement::toString()
{
	std::stringstream ss {};
	ss <<  "Program { \n";
	for (auto stmt : children) {
		ss << "\t" << (*stmt).toString() << "\n";
	}
	ss << "}";
	return ss.str();
}

/**
 * VARIABLE DECLARATION
 */
VariableDeclaration::~VariableDeclaration() {
	delete value;
}

std::string VariableDeclaration::toString()
{
	std::stringstream ss {};
	ss << "VariableDeclaration<identifier='" << identifier;
	ss << "' value='" << ((value == nullptr) ? "null" : value->toString()) << "'>";
	return ss.str();
}


/**
 * BINARY EXPRESSION
 */
BinaryExpression::~BinaryExpression() {
	delete left, right;
}

std::string BinaryExpression::toString()
{
	std::stringstream ss {};
	ss << "BinaryExpression { \n";
	ss << "\tleft: " << left->toString();
	ss << "\n\trigth: " << right->toString();
	ss << "\n\toperator: " + op;
	ss << "\n}";

	return ss.str();
}