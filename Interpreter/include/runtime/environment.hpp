#pragma once

#include <string>
#include <map>
#include <runtime/types.hpp>

class Environment
{

public:
    Environment(Environment * parent = nullptr);
    ~Environment();

    void defineVariable(const std::string& name, RuntimeValue* value);
    
    RuntimeValue* getVariable(const std::string& name);
    
    void setVariable(const std::string& name, RuntimeValue* value);

    bool exists(const std::string& name);

    bool existsLocal(const std::string& name);

    Environment * resolve(const std::string& name);

    const Environment * getParent() const {
        return parent;
    };

    bool isGlobal() const { return parent == nullptr; };

private:
    Environment * parent;
    std::map<std::string, RuntimeValue *> variables;
};


/**
 * Fill the environment with some default default values
 */
void setupGlobalScope(Environment * env);


/**
 * Setup the global functions
 */
void setupGlobalFunctions(Environment * env);

/**
 * Setup the math functions
 */
void setupMathFunctions(Environment * env);
