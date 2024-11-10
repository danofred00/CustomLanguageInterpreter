#include <runtime/interpreter.hpp>
#include <runtime/utils.hpp>
#include <runtime/exceptions/return_exception.hpp>

RuntimeValue * Interpreter::evaluate(Statement * statement, Environment * env)
{
    auto type = statement->getType();
    
    switch (type)
    {
        case NodeType::NUMBER_LITERAL:
            return new NumberValue((static_cast<FloatLiteral*>(statement))->getValue());
        case NodeType::STRING_LITERAL:
            return new StringValue((static_cast<StringLiteral*>(statement))->getValue());
        case NodeType::BOOL:
            return new BoolValue((static_cast<BoolLiteral*>(statement))->getValue());
        case NodeType::BINARY_EXPR:
            return evalBinaryExpression(static_cast<BinaryExpression*>(statement), env);
        case NodeType::RESERVED_LITERAL:
            return evalReserved(static_cast<ReservedExpression*>(statement), env);
        case NodeType::IDENTIFIER:
            return evalIdentifier(static_cast<Identifier*>(statement), env);
        case NodeType::BLOCK_PROGRAM:
        case NodeType::PROGRAM:
            return evalProgram(static_cast<ProgramStatement*>(statement), env);
        case NodeType::ASSIGNMENT:
            return evalAssignmentExpression(static_cast<AssignmentExpression *>(statement), env);
        // Handle function call
        case NodeType::CALL_EXPR:
            return evalFunctionCallExpression(static_cast<CallFunctionExpression *>(statement), env);
        // Handle contional expressions
        case NodeType::CONDITIONAL:
            return evalConditionalExpression(static_cast<ConditionalExpression*>(statement), env);
        // Handle statements
        case NodeType::VAR_DECLARATION:
            return evalVariableDeclaration(static_cast<VariableDeclaration*>(statement), env);
        case NodeType::FUNCTION_DECLARATION:
            return evalFunctionDeclaration(static_cast<FunctionDeclaration*>(statement), env);
        case NodeType::RETURN:
            return evalReturnStatement(static_cast<ReturnStatement*>(statement), env);
        // Not expression
        case NodeType::NOT:
            return evalNotExpression(static_cast<Not *>(statement), env);
        case NodeType::LOGIC_BINARY:
            return evalLogicExpression(static_cast<LogicBinary*>(statement), env);
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
    if(env->existsLocal(identifier)) {
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

RuntimeValue * Interpreter::evalFunctionDeclaration(FunctionDeclaration* func, Environment* env)
{
	auto name = func->getName();
    FunctionValue* fn = nullptr;
    
	// check if the function is already defined
    if (env->existsLocal(name)) {
        throw std::runtime_error("RuntimeError: Function " + name + " is already defined.");
    }

	fn = new FunctionValue(name, func);
	env->defineVariable(name, fn);
    return nullptr;
}

RuntimeValue * Interpreter::evalReturnStatement(ReturnStatement * stmt, Environment * env)
{
    auto value = evaluate(stmt->getValue(), env);
    throw ReturnException(value);
    //return value;
}

/**
 * EVALUATES EXPRESSIONS
 */

RuntimeValue * Interpreter::evalProgram(ProgramStatement * program,  Environment * env)
{
    RuntimeValue * lastEvaluated = nullptr;
    auto scopeEnv = Environment(env);
    
    for(auto stmt : program->getBody()) {
        // delete lastEvaluated;
        lastEvaluated = evaluate(stmt, &scopeEnv);
		// break if return statement
        if (stmt->getType() == NodeType::RETURN) {
			break;
		}
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
    if( left->getType() != NodeType::IDENTIFIER ) {
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
    auto scopeEnv = Environment(env);

    if (function->getType() != RuntimeValue::Type::NATIVE_FUNCTION && function->getType() != RuntimeValue::Type::FUNCTION) {
        throw std::runtime_error("Runtime Error: " + caller->getIdentifier() + " is not a function.");
    }

    // evaluate the arguments
    for(auto arg : expr->getArguments()) {
        auto val = evaluate(arg, &scopeEnv);
        // check if it's not null
        if(val == nullptr) {
            std::cerr << "TypeError: Can not pass 'Non-Value' as argument." << std::endl;
            std::exit(EXIT_FAILURE);
        }
        args.push_back(val);
    }

    // call the user defined function
	if (function->getType() == RuntimeValue::Type::FUNCTION)
    {
		auto fn = static_cast<FunctionValue*>(function)->getDeclaration();
		auto body = fn->getBody();
		auto params = fn->getParameters();
		RuntimeValue* lastEvaluated = nullptr;
		auto newEnv = new Environment(scopeEnv);

		// check if the number of arguments is correct
		if (args.size() != params.size()) {
			std::cerr << "TypeError: Expected " + std::to_string(params.size()) + " arguments, but got " + std::to_string(args.size()) << std::endl;
			std::exit(EXIT_FAILURE);
		}

		// bind the arguments
		for (size_t i = 0; i < params.size(); i++) {
			auto param = static_cast<Argument*>(params[i]);
			auto arg = args[i];
			if (!isCorrectType(arg->getType(), param->getValueType())) {
				std::cerr << "TypeError: Expected " + VariableDeclaration::valueTypeToString(param->getValueType());
				std::cerr << " but got " + RuntimeValue::typeToString(arg->getType()) << std::endl;
				std::exit(EXIT_FAILURE);
			}
			newEnv->defineVariable(param->getName(), arg);
		}

		// exexcute the function
        try{
            for(auto &stmt : (fn->getBody())->getBody()) {
                lastEvaluated = evaluate(stmt, newEnv);
            }
        } catch (ReturnException &e) {
            lastEvaluated = e.getValue();
        }

        return lastEvaluated;
	}

    // call the native function
    return (static_cast<NativeFunctionValue *>(function))->call(args);
}

RuntimeValue * Interpreter::evalNotExpression(Not * expr, Environment * env)
{
    auto value = evaluate(expr->getValue(), env);
    auto type = value->getType();

    if((type != RuntimeValue::Type::BOOL) && (type != RuntimeValue::Type::NUMBER_LITERAL)) {
        std::cerr << "LogicError: Expected a boolean value for not expression, got a ";
        std::cerr << RuntimeValue::typeToString(type) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return new BoolValue(!castToBool(value));
}

RuntimeValue * Interpreter::evalLogicExpression(LogicBinary * logic, Environment * env)
{
    auto left = evaluate(logic->getLeft(), env);
    auto right = evaluate(logic->getRight(), env);
    auto op = logic->getOperator();
    auto result = false;

    if(op == "and") {
        result = castToBool(left) && castToBool(right);
    } else if(op == "or") {
        result = castToBool(left) || castToBool(right);  
    } else {
        result = comparaison(left, right, op);
    }

    return new BoolValue(result);
}

RuntimeValue * Interpreter::evalConditionalExpression(ConditionalExpression * expr, Environment * env )
{
    RuntimeValue * lastEvaluated = nullptr;
    auto condition = evaluate(expr->getCondition(), env);

    auto execThen = std::any_cast<bool>(condition->getValue());
    if(execThen) {
        lastEvaluated = evaluate(static_cast<BlocStatement*>(expr->getThenBlock()), env);
    } else {
        // exec else block if it exists
        auto elseBlock = expr->getElseBlock();
        if(elseBlock != nullptr) {
            lastEvaluated = evaluate(static_cast<BlocStatement*>(elseBlock), env);
        }
    }

    return lastEvaluated;
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
