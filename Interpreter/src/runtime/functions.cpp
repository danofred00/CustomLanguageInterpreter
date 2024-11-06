#include <runtime/functions.hpp>

RuntimeValue* printFunction(const std::vector<RuntimeValue*>& args)
{
    for(auto& arg : args) {
        std::cout << arg->toString() << std::endl;
    }

    return nullptr;
}