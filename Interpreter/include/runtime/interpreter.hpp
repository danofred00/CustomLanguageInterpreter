#pragma once

#include <parser.hpp>
#include <runtime/types.hpp>

class Interpreter
{
public:

    RuntimeValue* evaluate(Statement * statement);

private:
    RuntimeValue* evalProgram(ProgramStatement * program);

    RuntimeValue* evalBinaryExpression(BinaryExpression * binary);

    RuntimeValue* evalNumericExpression(NumberValue * left, NumberValue * right, std::string op);

};