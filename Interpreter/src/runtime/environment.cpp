#include <runtime/environment.hpp>
#include <runtime/functions.hpp>

Environment::Environment(Environment * parent) : parent { parent } 
{ 
    // Ensure to setup only the root environment
    if(isGlobal()) {
        setupGlobalScope(this);
    }
}

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
    if(existsLocal(name)) {
        throw std::runtime_error("Variable already defined");
    }
    this->variables[name] = value;
}

bool Environment::existsLocal(const std::string& name)
{
    return variables.find(name) != std::end(variables);
}

void setupGlobalScope(Environment * env)
{
    /* TYPES SETUP */
	env->defineVariable("false", new BoolValue(false));
	env->defineVariable("true", new BoolValue(true));
	env->defineVariable("null", new NullValue());

    /* TODO: MATH CONSTANTS SETUP */
    
    /* SETUP NATIVE FUNCTIONS */
    setupGlobalFunctions(env);
}

void setupGlobalFunctions(Environment * env)
{
    // SETUP `print` FUNCTION to display something in command line
    env->defineVariable("print", new NativeFunctionValue(printFunction));
    // SETUP `read` FUNCTION to read something from command line
    env->defineVariable("read", new NativeFunctionValue(readFunction));
    // SETUP `type` FUNCTION to get the type of something
    env->defineVariable("type", new NativeFunctionValue(typeFunction));

    // setup math functions
    setupMathFunctions(env);
}

void setupMathFunctions(Environment * env)
{
    // SETUP `sqrt` FUNCTION to get the square root of a number
    env->defineVariable("sqrt", new NativeFunctionValue(sqrtFunction));
}
