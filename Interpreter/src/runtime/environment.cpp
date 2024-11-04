#include <runtime/environment.hpp>

Environment::Environment(Environment * parent) : parent { parent } { }

Environment::~Environment()
{
    for(auto& pair : variables) {
        delete pair.second;
    }
}

Environment * Environment::resolve(const std::string& name)
{
    if(variables.find(name) != std::end(variables)) {
        return this;
    }

    if(parent != nullptr) {
        return parent->resolve(name);
    }

    return nullptr;
}

bool Environment::exists(const std::string& name)
{
    return resolve(name) != nullptr;
}

void Environment::setVariable(const std::string& name, RuntimeValue* value)
{
    auto env = resolve(name);

    if(env != nullptr) {
        env->variables[name] = value;
    }
}

RuntimeValue* Environment::getVariable(const std::string& name)
{
    auto env = resolve(name);

    if(env != nullptr) {
        return env->variables[name];
    }

    return nullptr;
}

void Environment::defineVariable(const std::string& name, RuntimeValue* value)
{
    if(exists(name)) {
        throw std::runtime_error("Variable already defined");
    }
    this->variables[name] = value;
}