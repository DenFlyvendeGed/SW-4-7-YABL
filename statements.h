#ifndef __STATEMENT_H
#define __STATEMENT_H

#include "base.h"



class StatementAssign :public Statement
{
    LocationValue* lvalue;
    AritmeticExpression* expression;
    public:
        StatementAssign(LocationValue* lvalue, AritmeticExpression* expression)
        {
            this->lvalue = lvalue;
            this->expression = expression;
        }
        void Execute(Context* context) override
        {
            int temp = expression->Evaluate(context);
            lvalue->Set(temp, context);
        }
};

//StatementIfThenElse
//StatementWhile

#endif