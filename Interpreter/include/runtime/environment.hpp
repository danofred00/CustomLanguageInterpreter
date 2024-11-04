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

        Environment * resolve(const std::string& name);

        const Environment * getParent() const {
            return parent;
        };



    private:
        Environment * parent;
        std::map<std::string, RuntimeValue *> variables;
};