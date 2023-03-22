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
    // struct Repeat *Repeat;
    // struct TimesLoop *TimesLoop;
    // struct ForLoop *ForLoop;
    // struct whileLoop *whileLoop;
    // struct RepeatLoop *RepeatLoop;
    // struct Initialization *Initialization;
    // struct Assign *Assign;
    // struct Event *Event;
    // struct Variable *Variable;
    // struct Preamble *Preamble;
    // struct PreamblBoard *PreamblBoard;
    // struct PreambleTileItem *PreambleTileItem;
    // struct PreambleTile *PreambleTile;
    // struct PreamblePlayes *PreamblePlayes;

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
    Stmts create_stmts()
     {
        Stmts *p = malloc(sizeof(Stmts));
        p->nonterminal = stmts;
        p->children = NULL; //CreateList

        return *p;
     };
    void destroy_stmts(Stmts* p)
    {
        destroyList(p->children);
    };;

    // typedef Repeatable Scope;
    Scope create_scope()
    {
        Scope *p = malloc(sizeof(Scope));
        p->nonterminal = scope;
        p->children = NULL; //CreateList

        return *p;
    };
    void destroy_scope(Scope* p)
    {
        destroyList(p->children);
    };

    // typedef Repeatable Args;
    Args create_args()
    {
     Args *p = malloc(sizeof(Args));
        p->nonterminal = args;
        p->children = NULL; //CreateList

        return *p;
    };
    void destroy_args(Args* p)
    {
        destroyList(p->children);
    };

    // typedef Repeatable Funcs;
    Funcs create_funcs()
    {
        Funcs *p = malloc(sizeof(Funcs));
        p->nonterminal = funcs;
        p->children = NULL; //CreateList

        return *p;
    };
    void  destroy_funcs(Funcs* p)
    {
        destroyList(p->children);
    };

    // typedef Repeatable ListConstant;
    ListConstant create_funcs()
    {
        ListConstant *p = malloc(sizeof(ListConstant));
        p->nonterminal = list; //??
        p->children = NULL; //CreateList

        return *p;
    };
    void destroy_funcs(ListConstant* p)
    {
        destroyList(p->children);
    };

    // typedef Repeatable Preambles;
    Funcs create_preambles()
    {
        Funcs *p = malloc(sizeof(Funcs));
        p->nonterminal = funcs;
        p->children = NULL; //CreateList

        return *p;
    };
    void  destroy_preambles(Funcs* p)
    {
        destroyList(p->children);
    };