#include base.h



class SatementAssign :public Statement
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
}