#include <runtime/functions/core.hpp>

RuntimeValue * typeFunction(std::vector<RuntimeValue *> & args)
{
    if(args.size() != 1) {
        throw std::runtime_error("RuntimeError: Expected only one arguments as parameter of type() function");
    }

    auto arg = args[0];

    if(arg == nullptr) {
        throw std::runtime_error("RuntimeError: Can not pass 'Non-Value' as argument.");
    }

    return new StringValue(RuntimeValue::typeToString(arg->getType()));
}