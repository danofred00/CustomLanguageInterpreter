#include <ast.hpp>

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