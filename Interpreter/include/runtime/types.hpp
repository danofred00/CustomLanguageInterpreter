#pragma once

#include <any>
#include <sstream>
#include <ast.hpp>
#include <functional>


class RuntimeValue
{
public:
    enum class Type {
        NULL_LITERAL,
        NUMBER_LITERAL,
        STRING_LITERAL,
        BOOL,
        FUNCTION,
        NATIVE_FUNCTION
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
            return "number";
        case Type::STRING_LITERAL:
            return "string";
        case Type::NULL_LITERAL:
            return "null";
        case Type::BOOL:
            return "bool";
        case Type::NATIVE_FUNCTION:
        case Type::FUNCTION:
            return "function";
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

    int getValue() const { return 0; }

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

class NativeFunctionValue : public RuntimeValue
{
    using NativeFunction = std::function<RuntimeValue*(std::vector<RuntimeValue*> &)>;
public:
    NativeFunctionValue(NativeFunction func): RuntimeValue(std::any(func)) {};

    Type getType() const { 
        return Type::NATIVE_FUNCTION;
    }

    NativeFunction getValue() const {
        return std::any_cast<NativeFunction>(RuntimeValue::getValue());
    }

    RuntimeValue* call(std::vector<RuntimeValue*> args) {
        return getValue()(args);
    }

    std::string toString() override {
        return "Function<0x" + std::to_string((unsigned long long)this) + ">";
    }
};

class FunctionValue : public RuntimeValue
{
public:
    FunctionValue(std::string name, FunctionDeclaration* declaration)
        : RuntimeValue(), name{ name }, declaration{ declaration } {};

	Type getType() const {
		return Type::FUNCTION;
	}

    std::string toString() override {
        return "Function<" + name + ">";
    }

    FunctionDeclaration* getDeclaration() const {
        return declaration;
    }

private:
    std::string name;
	FunctionDeclaration* declaration;
};

bool isCorrectType(RuntimeValue::Type type, VariableDeclaration::ValueType vType);
