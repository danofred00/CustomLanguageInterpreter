#pragma once

#include <exception>
#include <runtime/types.hpp>

/**
 * @brief Exception thrown when a return statement is encountered
 */
class ReturnException: public std::exception
{
public:
    ReturnException(): std::exception() {};
    ReturnException(RuntimeValue * value): std::exception(), value(value) {};

    RuntimeValue * getValue() const { 
        return value; 
    }

    virtual const char * what() const noexcept override {
        return "Return statement encountered";
    }
private: 
    RuntimeValue * value = nullptr;
};