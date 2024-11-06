#include <runtime/functions/io.hpp>
#include <runtime/types.hpp>
#include <utils.hpp>

RuntimeValue* printFunction(const std::vector<RuntimeValue*>& args)
{
    for(auto& arg : args) {
        std::cout << arg->toString() << std::endl;
    }

    return nullptr;
}


RuntimeValue* readFunction(const std::vector<RuntimeValue*>& args)
{
    auto formats = { "s", "n" };
    std::string line {};
    RuntimeValue * result = nullptr;
    // ensure we have only two arguments
    if(args.size() != 2) {
        std::cerr << "Expected two arguments: read(prompt, format)" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // ensure we have a string as the first argument
    if(args[0]->getType() != RuntimeValue::Type::STRING_LITERAL) {
        std::cerr << "Expected a string as the prompt argument" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // ensure we have a string as the second argument
    if(args[1]->getType() != RuntimeValue::Type::STRING_LITERAL) {
        std::cerr << "Expected a string as the format argument" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // ensure the format is a valid format
    if(std::find(formats.begin(), formats.end(), args[1]->toString()) == formats.end()) {
        std::cerr << "Invalid format: " << args[1]->toString() << std::endl;
        std::cerr << "Expected: s (for string), n (for number)" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // print the prompt
    std::cout << args[0]->toString();
    // read the line
    std::getline(std::cin, line);
    // return the line by checking the return type
    if(args[1]->toString() == "s") {
        result = new StringValue(line);
    } else {
        if(isNumeric(line)) {
            result = new RuntimeValue(std::stod(line));
        } else {
            std::cerr << "Invalid number: " << line << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    return result;
}