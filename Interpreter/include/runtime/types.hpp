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
        BOOL = Statement::NodeType::BOOL,
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

    static std::string typeToString(Type type) {
        switch (type)
        {
        case Type::NUMBER_LITERAL:
            return "NUMBER_LITERAL";
        case Type::STRING_LITERAL:
            return "STRING_LITERAL";
        case Type::NULL_LITERAL:
            return "NULL_LITERAL";
        case Type::BOOL:
            return "BOOL";
        default:
            return "UNKNOW";
        }
    }

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
 * String runtime value
 */
class StringValue : public RuntimeValue
{
public: 
    StringValue(std::string value): RuntimeValue(std::any(value)) {};

    Type getType() const { 
        return Type::STRING_LITERAL;
    }

    std::string getValue() const {
        return std::any_cast<std::string>(RuntimeValue::getValue());
    }

    std::string toString() override {
        return getValue();
    }
};


class BoolValue : public RuntimeValue
{
public: 
    BoolValue(bool value): RuntimeValue(std::any(value)) {};

    Type getType() const { 
        return Type::BOOL;
    }

    bool getValue() const {
        return std::any_cast<bool>(RuntimeValue::getValue());
    }

    std::string toString() override {
        return getValue() ? "true" : "false";
    }
};


bool isCorrectType(RuntimeValue::Type type, VariableDeclaration::ValueType vType);