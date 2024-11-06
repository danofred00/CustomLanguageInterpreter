#include <runtime/interpreter.hpp>

RuntimeValue * Interpreter::evaluate(Statement * statement, Environment * env)
{
    auto type = statement->getType();
    
    switch (type)
    {
        case Statement::NodeType::NUMBER_LITERAL:
            return new NumberValue((static_cast<FloatLiteral*>(statement))->getValue());
        case Statement::NodeType::STRING_LITERAL:
            return new StringValue((static_cast<StringLiteral*>(statement))->getValue());
        case Statement::NodeType::BOOL:
            return new BoolValue((static_cast<BoolLiteral*>(statement))->getValue());
        case Statement::NodeType::BINARY_EXPR:
            return evalBinaryExpression(static_cast<BinaryExpression*>(statement), env);
        case Statement::NodeType::RESERVED_LITERAL:
            return evalReserved(static_cast<ReservedExpression*>(statement), env);
        case Statement::NodeType::IDENTIFIER:
            return evalIdentifier(static_cast<Identifier*>(statement), env);
        case Statement::NodeType::PROGRAM:
            return evalProgram(static_cast<ProgramStatement*>(statement), env);
        case Statement::NodeType::ASSIGNMENT:
            return evalAssignmentExpression(static_cast<AssignmentExpression *>(statement), env);
        // Handle function call
        case Statement::NodeType::CALL_EXPR:
            return evalFunctionCallExpression(static_cast<CallFunctionExpression *>(statement), env);
        // Handle statements
        case Statement::NodeType::VAR_DECLARATION:
            return evalVariableDeclaration(static_cast<VariableDeclaration*>(statement), env);
        default:
            // print the expression for debug only
            std::cerr << "Runtime Error: Unexpected statement : " + statement->toString() << std::endl;
            return nullptr;
    }
}

/**
 * EVALUATES STATEMENTS
 */
RuntimeValue * Interpreter::evalVariableDeclaration(VariableDeclaration * varDecl, Environment * env)
{
    auto identifier = varDecl->getIdentifier();
    auto type = varDecl->getValueType();
    // check if the variable is already defined
    if(env->exists(identifier)) {
        std::cerr << "Runtime Error: Variable " + identifier + " is already defined." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    RuntimeValue * value = varDecl->getValue() != nullptr ? evaluate(varDecl->getValue(), env) : new NullValue();
    
    if(!isCorrectType(value->getType(), type)) {
        std::cerr << "TypeError: Expected " + VariableDeclaration::valueTypeToString(type) + " but got " + RuntimeValue::typeToString(value->getType()) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    env->defineVariable(identifier, value);

    return value;
}


/**
 * EVALUATES EXPRESSIONS
 */

RuntimeValue * Interpreter::evalProgram(ProgramStatement * program,  Environment * env)
{
    RuntimeValue * lastEvaluated = nullptr;

    for(auto stmt : program->getBody()) {
        // delete lastEvaluated;
        lastEvaluated = evaluate(stmt, env);
    }

    return lastEvaluated;
}

RuntimeValue * Interpreter::evalBinaryExpression(BinaryExpression * binary,  Environment * env)
{
    auto left = evaluate(binary->getLeft(), env);
    auto right = evaluate(binary->getRight(), env);
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

RuntimeValue * Interpreter::evalIdentifier(Identifier * identifier,  Environment * env)
{
    auto name = identifier->getIdentifier();
    if(env->exists(name)) {
        return env->getVariable(name);
    }

    std::cerr << "Runtime Error: Undefined variable " + name << std::endl;
    std::exit(EXIT_FAILURE);
}

RuntimeValue * Interpreter::evalReserved(ReservedExpression * reserved, Environment * env)
{
    auto keyword = reserved->getKeyword();
    return env->getVariable(keyword);
}

RuntimeValue * Interpreter::evalAssignmentExpression(AssignmentExpression * expr, Environment * env)
{
    auto left = expr->getLeft();
    
    // check 
    if( left->getType() != Statement::NodeType::IDENTIFIER ) {
        std::cerr << "AssignmentError: expected an Identifier as left expression, but a " ;
        std::cerr << Statement::nodeTypeToString(left->getType()) + " is gived." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    auto _left = static_cast<Identifier *>(left);
    auto leftValue = evaluate(left, env);
    auto right = evaluate(expr->getValue(), env);
    
    // check if types
    if((leftValue->getType() != right->getType()) && (leftValue->getType() != RuntimeValue::Type::NULL_LITERAL)) {
        std::cerr << "TypeError: Can not assigne a value of type " + RuntimeValue::typeToString(right->getType());
        std::cerr << " to " + _left->getIdentifier() << " of type " + RuntimeValue::typeToString(leftValue->getType());
        std::cerr << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // assign new value
    env->setVariable(_left->getIdentifier(), right);

    // return values
    return right;
}

RuntimeValue * Interpreter::evalFunctionCallExpression(CallFunctionExpression * expr, Environment * env)
{
    auto caller = static_cast<Identifier *>(expr->getCaller());
    std::vector<RuntimeValue *> args {};

    // check if the function is defined
    if(!env->exists(caller->getIdentifier())) {
        std::cerr << "Runtime Error: Undefined function " + caller->getIdentifier() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // get the function
    auto function = env->getVariable(caller->getIdentifier());
    if(function->getType() != RuntimeValue::Type::FUNCTION) {
        std::cerr << "Runtime Error: " + caller->getIdentifier() + " is not a function." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // evaluate the arguments
    for(auto arg : expr->getArguments()) {
        auto val = evaluate(arg, env);

        // check if it's not null
        if(val == nullptr) {
            std::cerr << "TypeError: Can not pass 'Non-Value' as argument." << std::endl;
            std::exit(EXIT_FAILURE);
        }

        args.push_back(val);
    }

    // call the function
    return (static_cast<FunctionValue *>(function))->call(args);
}

/**
 * HELPERS
 */
bool isCorrectType(RuntimeValue::Type type, VariableDeclaration::ValueType vType)
{
    switch (vType)
    {
        case VariableDeclaration::ValueType::NUMBER:
            return type == RuntimeValue::Type::NUMBER_LITERAL || type == RuntimeValue::Type::NULL_LITERAL;
        case VariableDeclaration::ValueType::STRING:
            return type == RuntimeValue::Type::STRING_LITERAL || type == RuntimeValue::Type::NULL_LITERAL;
        case VariableDeclaration::ValueType::BOOLEAN:
            return type == RuntimeValue::Type::BOOL || type == RuntimeValue::Type::NULL_LITERAL;
        default:
            return false;
    }
}
