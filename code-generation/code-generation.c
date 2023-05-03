#include "../cfg.h"
#include "code-generation.h"
#include "../data-structures/list.h"
#include <stdlib.h>
#include <stdio.h>


void cgScope(Scope* self, FILE* writer){
    fprintf(writer, "\n{\n");
    YABL_LIST_FOREACH(Nonterminals*, self->children, cgStmt(foreach_value, writer););
    fprintf(writer, "\n}\n");
}


void cgStmt(Stmt* self, FILE* writer){

    switch (*(Nonterminals*)self)
    {
    case assign:
        cgAssign(self, writer);
        break;
    case ifstmt:
        cgIfStmt(self, writer);
        break;
    case repeatstmt:
        cgRepeatStmt(self, writer);
        break;
    case initialization:
        cgInitialization(self, writer);
        fprintf(writer, ";\n");
        break;
    case scope:
        cgScope(self, writer);
        break;
    case returnstmt:
        cgReturnStmt(self, writer);
        break;
    case breakstmt:
        cgBreakStmt(self, writer);
        break;
    case expr:
        cgExpr(self, writer);
        fprintf(writer, ";\n");
        break;
    default:
        break;
    }
    
}

void cgBreakStmt(Break* self, FILE* writer){
    fprintf(writer, "break;\n");
}

void cgReturnStmt(ReturnStmt* self, FILE* writer){
    fprintf(writer, "return");
    cgExpr(self->expr, writer);
    fprintf(writer, ";\n");
}

void cgInitialization(Initialization* self, FILE* writer){
    cgType(self->type, writer);
    cgId(self->variable, writer);
    if(self->initialValue != NULL){
         fprintf(writer, " = ");
        cgExpr(self->initialValue, writer);
    }
    
}

void cgId(Id* self, FILE* writer){
    fprintf(writer, "%s",self);
}

void cgType(Type* self, FILE* writer){
    switch (self->typeval->type)
    {
    case bt_number:
        fprintf(writer, "int ");
        break;
    case bt_logic:
        fprintf(writer, "int ");
        break;
    case bt_text:
        fprintf(writer, "char* ");
        break;
    case bt_list:
        fprintf(writer, "list ");
        break;
    case bt_NULL:
        fprintf(writer, "NULL ");
        break;
    default:
        break;
    }
}

void cgRepeatStmt(Repeat* self, FILE* writer){
    switch (*(LoopType*)self->loopType)
    {
    case lt_timesloop:
        cgTimesLoop(self->loopType, writer);
        break;
    case lt_forloop:
        cgForLoop(self->loopType, writer);
        break;
    case lt_whileloop:
        cgWhileLoop(self->loopType, writer);
        break;
    case lt_repeatloop:
        cgRepeatLoop(self->loopType, writer);
        break;
    default:
        break;
    }
    cgScope(self->scope, writer);
}

void cgRepeatLoop(RepeatLoop* self, FILE* writer){
    fprintf(writer, "while(1)");
}

void cgWhileLoop(WhileLoop* self, FILE* writer){
    fprintf(writer, "while(");
    cgExpr(self->condition, writer);
    fprintf(writer, ")");
    
}

void cgForLoop(ForLoop* self, FILE* writer){
    //På vej
    //fprintf(writer, "YABL_LIST_FOREACH()");

}


void cgTimesLoop(TimesLoop* self, FILE* writer){
    fprintf(writer, "for(int YABL_TIMESLOOP_I = 0; YABL_TIMESLOOP_I < ");
    cgExpr(self->goal, writer);
    fprintf(writer, "; YABL_TIMESLOOP_I++)");
}


void cgIfStmt(IfStmt* self, FILE* writer){
    fprintf(writer, "if(");
    cgExpr(self->condition, writer);
    fprintf(writer, ")");
    cgScope(self->then, writer);
    fprintf(writer, "else");
    cgScope(self->elsestmt, writer);

}


void cgAssign(Assign* self, FILE* writer){
    cgIdMutation(self->variable, writer);
    fprintf(writer, "=");
    cgExpr(self->expression, writer);
    fprintf(writer, ";\n");
}

void cgIdMutation(IdMutation* self, FILE* writer){
    fprintf(writer, "%s", self->name);
    if(self->child != NULL){
        cgIdMutationChild(self->child, writer);
    }
}

void cgIdMutationChild(IdMutations* self, FILE* writer){
    switch (*self)
    {
    case im_none:
        /* code - Not implemented */
        break;
    case im_value:
        /* code - Not implemented */
        break;
    case im_dot:
        /* code */
        break;
    case im_call:
        cgCall(self, writer);
        break;
    case im_index:
        /* code */
        break;
    
    default:
        break;
    }
}

void cgCall(IdMutationCall* self, FILE* writer){
    fprintf(writer, "(");
    cgExprs(self->args, writer);
    if(self->child != NULL){
        cgIdMutationChild(self->child, writer);
    }
    fprintf(writer, ")");
}

void cgExprs(Exprs* self, FILE* writer){
    YABL_LIST_FOREACH(Expr*, self->children,cgExpr(foreach_value, writer););
}



void cgExpr(Expr* self, FILE* writer){
    fprintf(writer, "(");
    switch (self->exprType)
    {
    case et_constant:
        cgConstant(self->child, writer);
        break;
    case et_id_mutation:
        cgIdMutation(self->child, writer);
        break;
    case et_unary_operator:
        cgUnaryOperator(self->child, writer);
        break;
    case et_binary_operator:
        cgBinaryOperator(self->child, writer);
        break;
    case et_expression:
        cgExpr(self->child, writer);
        break;
    case et_typecast:
        cgTypeCast(self->child, writer);
        break;
    default:
        break;
    }
    fprintf(writer, ")");
}

void cgConstant(Constant* self, FILE* writer){
    fprintf(writer, "%s", self->value);
}

//Skal snakke med Simon
void cgTypeCast(TypeCast* self, FILE* writer){
    fprintf(writer, "(");
    cgType(self->type, writer);
    fprintf(writer, ")");

    cgExpr(self->cast, writer);
}

void cgBinaryOperator(BinaryOperator* self, FILE* writer){
    cgExpr(self->childExpr1, writer);
    switch (self->bo)
    {
    case bo_plus:
        fprintf(writer, " + ");
        break;
    case bo_minus:
        fprintf(writer, " - ");
        break;
    case bo_division:
        fprintf(writer, " / ");
        break;
    case bo_mul:
        fprintf(writer, " * ");
        break;
    case bo_modulus:
        fprintf(writer, " %% ");
        break;
    case bo_not:
        fprintf(writer, " ! ");
        break;
    case bo_eq:
        fprintf(writer, " == ");
        break;
    case bo_neq:
        fprintf(writer, " != ");
        break;
    case bo_gt:
        fprintf(writer, " > ");
        break;
    case  bo_gteq:
        fprintf(writer, " >= ");
        break;
    case bo_lt:
        fprintf(writer, " < ");
        break;
    case bo_lteq:
        fprintf(writer, " <= ");
        break;
    case bo_and:
        fprintf(writer, " && ");
        break;
    case bo_or:
        fprintf(writer, " || ");
        break;
    
    default:
        break;
    }
    cgExpr(self->childExpr2, writer);
}

void cgUnaryOperator(UnaryOperator* self, FILE* writer){
    switch (self->uo)
    {
    case uo_negate:
        fprintf(writer, "-");
        break;
    case uo_not:
        fprintf(writer, "!");
        break;
    default:
        break;
    }
}


void cgStart(Repeatable* tree, FILE* writer){

    YABL_LIST_FOREACH(Nonterminals*, tree->children, 
        switch (*foreach_value)
        {
        case funcs:
            cgFuncs(foreach_value, writer);
            break;
        case preambles:
            cgPreambles(foreach_value, writer);
            break;
        default:
            break;
        } 
    );

    fprintf(writer, "\n");
}

void cgPreambles(Preambles* self, FILE* writer){
    PreamblePlayers* players = NULL;
    PreambleBoard* board = NULL;
    PreambleTile* tile = NULL;

    YABL_LIST_FOREACH(Nonterminals*, self->children, 
        switch (*foreach_value)
        {
        case preambleTile:
            tile = (PreambleTile*)foreach_value;
            break;
        case preambleBoard:
            board = (PreambleBoard*)foreach_value;
            break;
        case preamblePlayers:
            players = (PreamblePlayers*)foreach_value;
            break;
        default:
            break;
        };
    );
    cgPreambleTile(tile, writer);
    cgPreambleBoard(board, writer);
    cgPreamblePlayers(players, writer);
}

void cgPreamblePlayers(PreamblePlayers* self, FILE* writer){
    fprintf(writer, "char* PLAYERS[] = {");
    if(self->children->item != NULL){
        fprintf(writer, "%s", (char*)self->children->item);
        if(self->children->next != NULL){
            YABL_LIST_FOREACH(char*, self->children->next, 
                fprintf(writer, ",%s", foreach_value);
            );
        }
    }
    fprintf(writer, "};\n");
}


void cgPreambleBoard(PreambleBoard* self, FILE* writer){
    fprintf(writer, "struct Tile BOARD[%d][%d];\n",self->width, self->height);
}

void cgPreambleTile(PreambleTile* self, FILE* writer){
    fprintf(writer, "struct Tile {");
    if(self->children->item != NULL){
        cgInitialization(self->children->item, writer);
        if(self->children->next != NULL){
            YABL_LIST_FOREACH(Initialization*, self->children->next, 
                fprintf(writer, ";");
                cgInitialization(foreach_value, writer);
            );
        }
    }
    fprintf(writer, "};\n");
}



void cgFuncs(Funcs* self, FILE* writer){
    YABL_LIST_FOREACH(Nonterminals*, self->children, 
        switch (*foreach_value)
        {
        case event:
            cgEvent(foreach_value, writer);
            break;
        case func:
            cgFunc(foreach_value, writer);
            break;
        default:
            break;
        } 
    );
} 

void cgFunc(Func* self, FILE* writer){
    if(self->returntype != NULL){
        cgType(self->returntype, writer);
    }else{
        fprintf(writer, "void ");
    }
    fprintf(writer, "%s (", self->name);
    if(self->args->children->item != NULL){
        cgInitialization(self->args->children->item, writer);
        if(self->args->children->next != NULL){
            YABL_LIST_FOREACH(Initialization*, self->args->children->next, 
                fprintf(writer, ",");
                cgInitialization(foreach_value, writer);
            );
        }
    }
    fprintf(writer, ")");
    cgScope(self->scope, writer);
}

void cgEvent(Event* self, FILE* writer){
    fprintf(writer, "void ");
    switch (self->eventType)
    {
    case event_setup:
        fprintf(writer, "setup()");
        break;
    case event_turn:
        fprintf(writer, "turn()");
        break;
    case event_close:
        fprintf(writer, "close()");
        break;
    default:
        break;
    }
    cgScope(self->scope, writer);
}
