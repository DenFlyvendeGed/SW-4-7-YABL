#ifndef __BASE_H
#define __BASE_H

#include <map>
#include <string>

class Context
{
    public:
        std::map<std::string, int> values;
        Context(){}
        ~Context() {}
};

class Statement
{
    public:
        Statement();
            virtual ~Statement();
            virtual void Execute(Context* context) = 0;
};

class Variable
{
    std::string name;
    public:
        Variable(const std::string& name)
        {
            this->name = name;
        }
        void Set(int value, Context* context) 
        {
            context->values[name] = value;
        }
        int Evaluate(Context* context) 
        {
            return context->values[name];
        }

};

#endif