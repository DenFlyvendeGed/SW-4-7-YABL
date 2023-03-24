#include "cfg.h"
#include <stdlib.h>
// Type dcl

void destroyList();

    // int int;
    // char *string;
    
    //constant
    Constant createConstant()
    {
        Constant* p = malloc(sizeof(Constant));

    };

    void destroyConstant(Constant* p)
    {
        free(p);
    };


    // struct Func *Function;
    Func createFunc()
    {
        Func* p = malloc(sizeof(Func));
        p->nonterminal = func;

        return *p;
    };

    void destroyFunc(Func* p)
    {
        destroyArgs(&(p->args));
        destroyType(&(p->returntype));
        destroyScope(&(p->scope));
        free(p);
    }

    // struct TypeValue *TypeValue;
    TypeValue createTypeValue()
    {
        TypeValue* p = malloc(sizeof(TypeValue));

        return *p;
    };

    void destroyTypeValue(TypeValue* p)
    {
        if(p->type == list)
            destroyType(&p->list);
        free(p);
    };

    // struct Type *Type;
    Type createType()
    {
        Type* p = malloc(sizeof(Type));
        p->nonterminal = type;

        return *p;
    };

    void destroyType(Type* p)
    {
        destroyTypeVal(&(p->typeval));
        free(p);
    };

    // struct Repeatable *Repeatable;
    Repeatable creatRepeatable()
    {
        Repeatable* p = malloc(sizeof(Repeatable));

        return *p;  
    };

    void destroyRepeatable(Repeatable* p)
    {
        switch (p->nonterminal)
        {
        case exprs:
            destroyExprs(&(p->children));
            break;
        case stmts:
            destroyStmts(&(p->children));
            break;
        case scope:
            destroyScope(&(p->children));
            break;
        case args:
            destroyArgs(&(p->children));
            break;

        case funcs:
            destroyFuncs(&(p->children));
            break;
        case listConstant:
            destroyListConstant(&(p->children));
            break;
        case preambles:
            destroyPreambles(&(p->children));
            break;
        default:
            //error
            break;
        };
        free(p);
    };


    // struct Expr *Expr;
    Expr createExpr()
    {
        Expr* p = malloc(sizeof(Expr));
        p->nonterminal = expr;

        return *p;
    }

    void destroyExpr(Expr* p)
    {
        switch(p->expr_type)
        {
            case et_constant:
                destroyConstant(&(p->child));
                break;
            case et_id_mutation:
                destroyIdMutation(&(p->child));
                break;
            case et_unary_operator:
                destroyUnaryOperator(&(p->child));
                break;
            case et_binary_operator:
                destroyBinaryOperator(&(p->child));
                break;
            case et_expression:
                destroyExpr(&(p->child));
                break;
        };
        free(p);
    };

    // struct BinaryOperator *BinaryOp;
    BinaryOperator createBinaryOperator()
    {
        BinaryOperator* p = malloc(sizeof(BinaryOperator));
        p->nonterminal = binaryOperator;

        return *p;
    };

    void destroyBinaryOperator(BinaryOperator* p)
    {
        destroyExpr(&(p->childExpr1));
        destroyExpr(&(p->childExpr2));
        free(p);
    };


    // struct UnaryOperator *UnaryOp;
    UnaryOperator createUnaryOperator()
    {
        UnaryOperator *p = malloc(sizeof(UnaryOperator));
        p->nonterminal = unaryOperator;

        return *p;
    };

    void destroyUnaryOperator(UnaryOperator* p)
    {
        destroyExpr(&(p->child_expr));
        free(p);
    };

    // struct IdMutationDot *IdMutationDot;
    IdMutationDot createIdMutationDot()
    {
        IdMutationDot *p = malloc(sizeof(IdMutationDot));
        p->child_type = im_none;

        return *p;
    };

    void destroyIdMutationDot(IdMutationDot* p)
    {
         switch(p->child_type)
        {
            case im_none:
                free(p);
                break;
            case im_dot:
                destroyIdMutationDot(&(p->child));
                free(p);
                break;
            case im_call:
                destroyIdMutationCall(&(p->child));
                free(p);
                break;
            case im_index:
                destroyIdMutationIndex(&(p->child));
                free(p);
        };
    };


    // struct IdMutationCall *IdMutationCall;
    IdMutationCall createIdMutationCall()
    {
        IdMutationCall *p = malloc(sizeof(IdMutationCall));
        p->child_type = im_none;

        return *p;
    };

    void destroyIdMutationCall(IdMutationCall* p)
    {
        destroyArgs(&(p->args));
        switch(p->child_type)
        {
            case im_none:
                free(p);
                break;
            case im_dot:
                destroyIdMutationDot(&(p->child));
                free(p);
                break;
            case im_call:
                destroyIdMutationCall(&(p->child));
                free(p);
                break;
            case im_index:
                destroyIdMutationIndex(&(p->child));
                free(p);
        };

    };


    // struct IdMutationIndex *IdMutationIndex;
    IdMutationIndex createIdMutationIndex()
    {
        IdMutationIndex *p = malloc(sizeof(IdMutationIndex));
        p->child_type = im_none;

        return *p;
    };

    void destroyIdMutatuinIndex(IdMutationIndex* p)
    {
        destroyExpr(&(p->index));
        switch(p->child_type)
        {
            case im_none:
                free(p);
                break;
            case im_dot:
                destroyIdMutationDot(&(p->child));
                free(p);
                break;
            case im_call:
                destroyIdMutationCall(&(p->child));
                free(p);
                break;
            case im_index:
                destroyIdMutationIndex(&(p->child));
                free(p);
        };
    };



    // struct IdMutation *IdMutation;
    IdMutation createIdMutation()
    {
        IdMutation *p = malloc(sizeof(IdMutation));
        p->nonterminal = idMutation;
        p->id_mutation = im_none;
        return *p;
    };

    void destroyIdMutation(IdMutation* p)
    {
        switch(p->id_mutation)
        {
            case im_none:
                free(p);
                break;
            case im_dot:
                destroyIdMutationDot(&(p->child));
                free(p);
                break;
            case im_call:
                destroyIdMutationCall(&(p->child));
                free(p);
                break;
            case im_index:
                destroyIdMutationIndex(&(p->child));
                free(p);
        };
    };


    // struct IfStmt *IfStmt;
    IfStmt createIfStmt()
    {
        IfStmt *p = malloc(sizeof(IfStmt));
        p->nonterminal = ifstmt;

        return *p;
    };

    void destroyIfStmt(IfStmt* p)
    {
        destroyExprs(&(p->condition));
        destroyScope(&(p->then));
        destroyScope(&(p->elsestmt));
        free(p);
    };


    // struct Repeat *Repeat;
    Repeat createRepeat()
    {
        Repeat *p = malloc(sizeof(Repeat));
        p->loop_type = lt_repeatloop;

        return *p;
    };

    // struct TimesLoop *TimesLoop;
    TimesLoop createTimesLoop()
    {
        TimesLoop *p = malloc(sizeof(TimesLoop));
        p->loop_type = lt_timesloop;

        return *p;
    };

    destroyTimesLoop(TimesLoop* p)
    {
        free(p);
    };

    // struct ForLoop *ForLoop;
    ForLoop createForLoop()
    {
        ForLoop *p = malloc(sizeof(ForLoop));
        p->loop_type = lt_forloop;

        return *p;
    };

    void destroyForLoop(ForLoop *p)
    {
        free(p);
    };

    // struct whileLoop *whileLoop;
    WhileLoop createWhileLoop()
    {
        WhileLoop *p = malloc(sizeof(WhileLoop));
        p->loop_type = lt_whileloop;

        return *p;
    };

    void destroyWhileloop(WhileLoop* p)
    {
        destroyExpr(&(p->condition));
        free(p);
    };

    // struct RepeatLoop *RepeatLoop;
    RepeatLoop createRepeatLoop()
    {
        RepeatLoop *p = malloc(sizeof(RepeatLoop));
        p->loop_type = lt_repeatloop;

        return *p;
    };

    void destroyRepeatLoop(RepeatLoop* p)
    {
        free(p);
    };

    // struct Initialization *Initialization;
    Initialization createInitialization()
    {
        Initialization *p = malloc(sizeof(Initialization));
        p->nonterminal = initialization;

        return *p;
    };
    destroyInitialization(Initialization* p)
    {
        destroyType(&(p->type));
        free(p);
    };

    // struct Assign *Assign;
    Assign createAssign()
    {
        Assign *p = malloc(sizeof(Assign));
        p->nonterminal = assign;

    };
    void destroyAssign(Assign* p)
    {
        destroyExpr(&(p->expression));
        free(p);
    }

    // struct Event *Event;
    Event createEvent()
    {
        Event *p = malloc(sizeof(Event));
        p->nonterminal = event;
        p->scope = NULL; //createList
        return *p;
    };
    void destroyEvent(Event* p)
    {
        destroyScope(&(p->scope));
        free(p);
    };

    // struct Variable *Variable;
    Variable createVariable()
    {
        Variable *p = malloc(sizeof(Variable));
        p->nonterminal = variable;

        return *p;
    };

    void destroyVariable(Variable* p)
    {
        destroyType(&(p->type));
        free(p);
    };

    
    // struct Preamble *Preamble;
    // struct PreamblBoard *PreamblBoard;
    // struct PreambleTileItem *PreambleTileItem;
    // struct PreambleTile *PreambleTile;
    // struct PreamblePlayes *PreamblePlayes;


    // PreamblePlayers createPreamblePlayers()
    // {
    //     PreamblePlayers *p = malloc(sizeof(PreamblePlayers));
    //     p->nonterminal = PreamblePlayers;
        
    // };


    // typedef Repeatable Exprs;
     Exprs createExprs() 
     {
        Exprs *p = malloc(sizeof(Exprs));
        p->nonterminal = expr;
        p->children = NULL; //createList
        return *p;
     };
    void  destroyExprs(Exprs* p)
    {
        destroyList(p->children);
    };

    // typedef Repeatable Stmts;
    Stmts createStmts()
     {
        Stmts *p = malloc(sizeof(Stmts));
        p->nonterminal = stmts;
        p->children = NULL; //CreateList

        return *p;
     };
    void destroyStmts(Stmts* p)
    {
        destroyList(p->children);
    };;

    // typedef Repeatable Scope;
    Scope createScope()
    {
        Scope *p = malloc(sizeof(Scope));
        p->nonterminal = scope;
        p->children = NULL; //CreateList

        return *p;
    };
    void destroyScope(Scope* p)
    {
        destroyList(p->children);
    };

    // typedef Repeatable Args;
    Args createArgs()
    {
     Args *p = malloc(sizeof(Args));
        p->nonterminal = args;
        p->children = NULL; //CreateList

        return *p;
    };
    void destroyArgs(Args* p)
    {
        destroyList(p->children);
    };

    // typedef Repeatable Funcs;
    Funcs createFuncs()
    {
        Funcs *p = malloc(sizeof(Funcs));
        p->nonterminal = funcs;
        p->children = NULL; //CreateList

        return *p;
    };
    void  destroyFuncs(Funcs* p)
    {
        destroyList(p->children);
    };

    // typedef Repeatable ListConstant;
    ListConstant createListConstant()
    {
        ListConstant *p = malloc(sizeof(ListConstant));
        p->nonterminal = list; //??
        p->children = NULL; //CreateList

        return *p;
    };
    void destroyListConstant(ListConstant* p)
    {
        destroyList(p->children);
    };

    // typedef Repeatable Preambles;
    Funcs createPreambles()
    {
        Funcs *p = malloc(sizeof(Funcs));
        p->nonterminal = funcs;
        p->children = NULL; //CreateList

        return *p;
    };
    void  destroyPreambles(Funcs* p)
    {
        destroyList(p->children);
    };