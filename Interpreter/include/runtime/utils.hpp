#pragma once

#include <runtime/types.hpp>

inline bool castToBool(RuntimeValue * x) {

    if(x == nullptr) return false;

    auto type = x->getType();
    // cast to bool
    if(type == RuntimeValue::Type::NULL_LITERAL) {
        return false;
    } else if(type == RuntimeValue::Type::NUMBER_LITERAL) {
        return std::any_cast<float>(x->getValue()) != 0;
    } else if (type == RuntimeValue::Type::BOOL) {
        return std::any_cast<bool>(x->getValue());
    } else {
        throw std::runtime_error("CastError: Can not cast a " + RuntimeValue::typeToString(type) + " to boolean value.");
    }
    
    return false;
}

inline bool isComparable(RuntimeValue * left, RuntimeValue * rigth)
{
    auto leftType = left->getType();
    auto rightType = left->getType();

    // start by null values
    if(leftType == RuntimeValue::Type::NULL_LITERAL || rightType == RuntimeValue::Type::NULL_LITERAL) {
        return false;
    } else if(leftType != rightType){
        return false;
    }

    return true;
}

inline bool equalComparaison(RuntimeValue * left, RuntimeValue * right)
{
    auto leftType = left->getType();
    auto rightType = left->getType();

    return false;
}

inline bool compareNumbers(float left, float right, const std::string & op)
{
    if(op == "<") return left < right;
    if(op == "<=") return left <= right;
    if(op == ">") return left > right;
    if(op == ">=") return left >= right;
    if(op == "==") return left == right;

    throw std::logic_error("LogicError: Unsupported operation with number type");
}

inline bool compareStrings(const std::string & left, const std::string & right, const std::string & op)
{
    int result = left.compare(right);
    // compare by alphabetic order
    if(op == ">") return result < 0;
    if(op == "<") return result > 0;
    if(op == "==") return result == 0;

    throw std::logic_error("LogicError: Unsupported operator for string value");
}

inline bool compare(RuntimeValue * left, RuntimeValue * right, const std::string & op)
{
    auto leftType = left->getType();
    auto rightType = right->getType();

    auto leftVal = left->getValue();
    auto rightVal = right->getValue();

    if(leftType == rightType && leftType == RuntimeValue::Type::NUMBER_LITERAL) {
        return compareNumbers(std::any_cast<float>(leftVal), std::any_cast<float>(rightVal), op);
    } else if(leftType == rightType && leftType == RuntimeValue::Type::STRING_LITERAL) {
        return compareStrings(std::any_cast<std::string>(leftVal), std::any_cast<std::string>(rightVal), op);
    } else {
        auto message = "RuntimeError: Unsupported comporaison for type " + RuntimeValue::typeToString(leftType);
        message += " and type " + RuntimeValue::typeToString(rightType);
        throw std::runtime_error(message);
    }

    return false;
}

inline bool comparaison(RuntimeValue * left, RuntimeValue * right, const std::string & op)
{
    if(left == nullptr || right == nullptr) {
        throw std::runtime_error("RuntimeError: Can not compare a non value type");
    }

    if(op == "==") {
        return equalComparaison(left, right);
    } else if(isComparable(left, right)){
        return compare(left, right, op);
    } else {
        auto message = "LogicError: Can not compare value of type " + RuntimeValue::typeToString(left->getType());
        message += " with value of type " + RuntimeValue::typeToString(right->getType());
        throw std::logic_error(message);
    }
}

