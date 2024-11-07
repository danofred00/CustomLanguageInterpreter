
#include <cmath>
#include <runtime/functions/math.hpp>

RuntimeValue * sqrtFunction(std::vector<RuntimeValue *> args)
{
    if (args.size() != 1) {
        throw std::runtime_error("sqrt() takes exactly 1 argument");
    }

    if (args[0]->getType() != RuntimeValue::Type::NUMBER_LITERAL) {
        throw std::runtime_error("sqrt() takes a number as argument");
    }
    float value = static_cast<NumberValue *>(args[0])->getValue();

    return new NumberValue(sqrt(value));
}