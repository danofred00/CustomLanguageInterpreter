#pragma once

#include <any>
#include <sstream>
#include <ast.hpp>


class RuntimeValue
{
public:
    enum class Type {
        NUMBER_LITERAL = Statement::NodeType::NUMBER_LITERAL,
        STRING_LITERAL = Statement::NodeType::STRING_LITERAL,
        NULL_LITERAL = Statement::NodeType::NULL_LITERAL,
    };

    RuntimeValue(): value{} {}

    RuntimeValue(const std::any& value): value{value} {}

    std::any getValue() const {
        return value;
    }

    void setValue(std::any &value) {
        this->value = value;
    }

    virtual Type getType() const {
        return Type::NULL_LITERAL;
    };

    virtual std::string toString() { return ""; }

private:
    std::any value;
};

/**
 * Number value
 */
class NumberValue : public RuntimeValue
{
public: 
    NumberValue(float value): RuntimeValue{std::any(value)} {}

    Type getType() const { 
        return Type::NUMBER_LITERAL;
    }

    float getValue() const {
        return std::any_cast<float>(RuntimeValue::getValue());
    }

    std::string toString() override {
        return std::to_string(getValue());
    }
};

/**
 * Null runtime value
 */
class NullValue : public RuntimeValue
{
public: 
    Type getType() const { 
        return Type::NULL_LITERAL;
    }

    int getValue() const { return NULL; }

    std::string toString() override { return "null"; }
};

/**
 * TODO: handle String runtime value
 */