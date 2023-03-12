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