#include <runtime/interpreter.hpp>

RuntimeValue * Interpreter::evaluate(Statement * statement)
{
    auto type = statement->getType();
    
    switch (type)
    {
        case Statement::NodeType::NUMBER_LITERAL:
            return new NumberValue((static_cast<FloatLiteral*>(statement))->getValue());
        case Statement::NodeType::BINARY_EXPR:
            return evalBinaryExpression(static_cast<BinaryExpression*>(statement));
        case Statement::NodeType::PROGRAM:
            return evalProgram(static_cast<ProgramStatement*>(statement));
        case Statement::NodeType::NULL_LITERAL:
        default:
            return new NullValue();
    }
}

RuntimeValue * Interpreter::evalProgram(ProgramStatement * program)
{
    RuntimeValue * lastEvaluated = new NullValue();

    for(auto stmt : program->getBody()) {
        delete lastEvaluated;
        lastEvaluated = evaluate(stmt);
    }

    return lastEvaluated;
}

RuntimeValue * Interpreter::evalBinaryExpression(BinaryExpression * binary)
{
    auto left = evaluate(binary->getLeft());
    auto right = evaluate(binary->getRight());
    auto op = binary->getOperator();

    if(left->getType() == RuntimeValue::Type::NUMBER_LITERAL && right->getType() == RuntimeValue::Type::NUMBER_LITERAL) {
        return evalNumericExpression(
            static_cast<NumberValue*>(left), 
            static_cast<NumberValue*>(right), op
        );
    }

    return new NullValue();
}

RuntimeValue * Interpreter::evalNumericExpression(NumberValue * left, NumberValue * right, std::string op)
{
    RuntimeValue * result = nullptr;
    auto lVal = left->getValue();
    auto rVal = right->getValue();

    if(op == "+") {
        result = new NumberValue(lVal + rVal);
    } else if (op == "-") {
        result = new NumberValue(lVal - rVal);
    } else if(op == "*") {
        result = new NumberValue(lVal * rVal);
    } else if(op == "/") {
        if(rVal == 0) {
            std::cerr << "DivisionByZero Error: you a trying to divide some number by 0." << std::endl;
            std::exit(EXIT_FAILURE);
        } else { 
            result = new NumberValue(lVal / rVal);
        }
    } else {
        std::cerr << "Syntax Error: Unexpected operator " + op << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    return result == nullptr ? new NullValue() : result;
}