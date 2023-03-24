#include "cfg.h"
#include <stdlib.h>
// Type dcl

void destroyList();

    // int int;
    // char *string;
    // struct Func *Function;
    // struct TypeValue *TypeValue;
    // struct Type *Type;
    // struct Repeatable *Repeatable;
    // struct Expr *Expr;
    // struct BinaryOperator *BinaryOp;
    // struct UnaryOperator *UnaryOp;
    // struct IdMutationDot *IdMutationDot;
    // struct IdMutationCall *IdMutationCall;
    // struct IdMutationIndex *IdMutationIndex;
    // struct IdMutation *IdMutation;
    

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