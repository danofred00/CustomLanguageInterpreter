#pragma once

#include <parser.hpp>
#include <runtime/environment.hpp>
#include <runtime/types.hpp>

class Interpreter
{
public:

    RuntimeValue* evaluate(Statement * statement, Environment * env);

private:
    RuntimeValue* evalProgram(ProgramStatement * program,  Environment * env);

    RuntimeValue* evalBinaryExpression(BinaryExpression * binary,  Environment * env);

    RuntimeValue* evalNumericExpression(NumberValue * left, NumberValue * right, std::string op);

    RuntimeValue* evalIdentifier(Identifier * identifier,  Environment * env);

    RuntimeValue * evalReserved(ReservedExpression * reserved, Environment * env);

    RuntimeValue * evalVariableDeclaration(VariableDeclaration * varDecl, Environment * env);

    RuntimeValue * evalAssignmentExpression(AssignmentExpression * expr, Environment * env);

    RuntimeValue * evalFunctionCallExpression(CallFunctionExpression * expr, Environment * env);

    RuntimeValue * evalConditionalExpression(ConditionalExpression * expr, Environment * env );

    RuntimeValue * evalNotExpression(Not * expr, Environment * env);

    RuntimeValue * evalLogicExpression(LogicBinary * logic, Environment * env);

	RuntimeValue* evalFunctionDeclaration(FunctionDeclaration* func, Environment* env);

    RuntimeValue * evalReturnStatement(ReturnStatement * stmt, Environment * env);

};
