#include "../cfg/cfg.h"
#include "code-generation.h"
#include "../data-structures/list.h"
#include "../data-structures/hashtable.h"
#include "../data-structures/stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "./const-code/const-code.h"

////////////////////// EVENTS /////////////////////

int SEEN_SETUP = 0;
int SEEN_TURN = 0;
int SEEN_CLOSE = 0;
int SEEN_GET_TOKEN = 0;

/////////////////// FUNCTION CALLS ////////////////

int STMT_INCLUDES_CALL_WITH_N_STRING = 0;

//////////////////// RETURN TYPE //////////////////

Type* RETURN_TYPE;

//////////////////// Text Stack ////////////////////

YablStack CG_TEXT_STACK = YABL_STACK_CREATE;
typedef enum CgStackItemType { cg_scope_start, cg_loop_start, cg_text } CgStackItemType;
typedef struct GgStackItem {
	CgStackItemType type;
	char* name;
} CgStackItem;

CgStackItem* createCgStackItem(CgStackItemType type, char* name){
	CgStackItem* rtn = malloc(sizeof(CgStackItem));
	rtn->name = name;
	rtn->type = type;
	return rtn;
}

////////////////////////////////////////////////////

void cgScope(Scope* self, FILE* writer){
	CgStackItem* cgsi = malloc(sizeof(CgStackItem));
	cgsi->type = cg_scope_start;
	cgsi->name = NULL;
	yablStackPush(&CG_TEXT_STACK, cgsi);

    fprintf(writer, "\n{\n");
    YABL_LIST_FOREACH(Stmt*, self->children, cgStmt(foreach_value, writer););

	while( (cgsi = yablStackPop(&CG_TEXT_STACK))->type != cg_scope_start ) {
		fprintf(writer, "__DESTROY_STRING__(");
		cgId(&cgsi->name, writer);
		fprintf(writer, ");");
		free(cgsi);
	}

	free(cgsi);

	fprintf(writer, "\n}\n");
}


void cgStmt(Stmt* self, FILE* writer){

    switch (*(Nonterminals*)self)
    {
    case assign:
        cgAssign((Assign*)self, writer);
        break;
    case ifstmt:
        cgIfStmt((IfStmt*)self, writer);
        break;
    case repeatstmt:
        cgRepeatStmt((Repeat*)self, writer);
        break;
    case initialization:
        cgInitialization((Initialization*)self, writer);
        fprintf(writer, ";\n");
        break;
    case scope:
        cgScope((Scope*)self, writer);
        break;
    case returnstmt:
        cgReturnStmt((ReturnStmt*)self, writer);
        break;
    case breakstmt:
        cgBreakStmt((Break*)self, writer);
        break;
    case expr:
		if(((Expr*)self)->extension->type == bt_text)
			fprintf(writer, "__DESTROY_STRING__");
        cgExpr((Expr*)self, writer);
        fprintf(writer, ";\n");
        break;
    default:
        break;
    }

	for(; 0 != STMT_INCLUDES_CALL_WITH_N_STRING; STMT_INCLUDES_CALL_WITH_N_STRING--)
		fprintf(writer, "__DESTROY_STRING__(*--STRING_STACK);\n");
}

void cgBreakStmt(Break* self, FILE* writer){
	YablStack top = CG_TEXT_STACK;
	do {
		CgStackItem* cgsi = top->item;
		if(cgsi->type == cg_text){
			fprintf(writer, "__DESTROY_STRING__(%s);\n", cgsi->name);
		}
	} while(*(CgStackItemType*)((top = top->next)->item) != cg_loop_start);
    fprintf(writer, "break;\n");
}

void cgReturnStmt(ReturnStmt* self, FILE* writer){
	cgType(RETURN_TYPE, writer);
	fprintf(writer, " __RETURN__;\n");
    fprintf(writer, "return ( __RETURN__ = ");
	if(self->expr->extension->type == bt_text)
		fprintf(writer, "__COPY_STRING__");
    cgExpr(self->expr, writer);
	YablStack top = CG_TEXT_STACK;
	do {
		CgStackItem* cgsi = top->item;
		if(cgsi->type == cg_scope_start) continue;
		fprintf(writer, ", __DESTROY_STRING__(%s)", cgsi->name);
	} while((top = top->next) != NULL);

    fprintf(writer, ", __RETURN__);\n");
}

void cgInitialization(Initialization* self, FILE* writer){
	int isText = self->type->typeval->type == bt_text;
	if(isText) {
		CgStackItem* cgsi = malloc(sizeof(CgStackItem));
		cgsi->type = cg_text;
		cgsi->name = self->variable;
		yablStackPush(&CG_TEXT_STACK, cgsi);
	}
    cgType(self->type, writer);
    cgId(&self->variable, writer);
    if(self->initialValue != NULL){
		fprintf(writer, " = ");
		if(self->initialValue->extension->type == bt_text 
		&& self->initialValue->exprType == et_id_mutation){
			fprintf(writer, "__COPY_STRING__");
		} 
		cgExpr(self->initialValue, writer);
    } else if(isText) {
		fprintf(writer, " = __MAKE_STRING__(\"\")");
	}
}

void cgId(Id* self, FILE* writer){
	int i;
	for(i = 0; (*self)[i] != '.' && (*self)[i] != '\0'; i++){}
	if((*self)[i] == '.') fprintf(writer, "%s",(*self) + i + 1);
	else				  fprintf(writer, "%s", *self         ); 
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
        fprintf(writer, "__STRING__T* ");
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
	yablStackPush(&CG_TEXT_STACK, createCgStackItem(cg_loop_start, NULL));
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
	yablStackPop(&CG_TEXT_STACK);
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
	if(self->expression->extension->type == bt_text){
		fprintf(writer, "__DESTROY_STRING__(");
		cgIdMutation(self->variable, writer);
		fprintf(writer, ");\n");
	}
	cgIdMutation(self->variable, writer);
	fprintf(writer, "=");
	if(self->expression->extension->type == bt_text 
	&& self->expression->exprType == et_id_mutation){
		fprintf(writer, "__COPY_STRING__");
	}
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
		cgDot((IdMutationDot*)self, writer);
        break;
    case im_call:
        cgCall((IdMutationCall*)self, writer);
        break;
    case im_index:
		cgIndex((IdMutationIndex*)self, writer);
        break;
    default:
        break;
    }
}

void cgCall(IdMutationCall* self, FILE* writer){
    fprintf(writer, "(");
	YABL_LIST_FOREACH(Expr*, self->args->children, 
		if(foreach_value->extension->type == bt_text){
			fprintf(writer, "*(STRING_STACK++) = __COPY_STRING__");
			STMT_INCLUDES_CALL_WITH_N_STRING++;
		}
		cgExpr(foreach_value, writer);
	)
    if(self->child != NULL){
        cgIdMutationChild(self->child, writer);
    }
    fprintf(writer, ")");
}

void cgIndex(IdMutationIndex* self, FILE* writer){
	fprintf(writer, "[");
	cgExpr(self->index, writer);
	fprintf(writer, "]");
	cgIdMutationChild(self->child, writer);
}

void cgDot(IdMutationDot* self, FILE* writer){
	fprintf(writer, ".");
	cgIdMutation(self->child, writer);
}

void cgExprs(Exprs* self, FILE* writer){
	if(self->children->item != NULL){ 
		cgExpr(self->children->item, writer);
		if(self->children->next != NULL) {
			YABL_LIST_FOREACH(Expr*, self->children->next,
				fprintf(writer, ",");
				cgExpr(foreach_value, writer);
			);
		}
	}
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
        cgBinaryOperator(self, writer);
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
    switch (self->typeDcl)
    {
    case td_logic:
        if(strcmp(self->value, "true") == 0){
            fprintf(writer, "1");
        }else{
            fprintf(writer, "0");
        }
        break;
	case td_text:
        fprintf(writer, "__MAKE_STRING__(%s)", self->value);
		break;
    default:
        fprintf(writer, "%s", self->value);
        break;
    }
    
    
}

void cgTypeCast(TypeCast* self, FILE* writer){
    switch (self->cast->extension->type)
    {
    case bt_number:
        if(self->type->typeval->type == bt_text){
            fprintf(writer, "__NUMBER_TO_TEXT__(");
            cgExpr(self->cast, writer);
            fprintf(writer, ")");
        }else if(self->type->typeval->type == bt_logic){
            fprintf(writer, "__NUMBER_TO_LOGIC__(");
            cgExpr(self->cast, writer);
            fprintf(writer, ")");
        }else if(self->type->typeval->type == bt_number){
            cgExpr(self->cast, writer);
        }
        break;
    case  bt_text:
        if(self->type->typeval->type == bt_number){
            fprintf(writer, "__TEXT_TO_NUMBER__(");
            cgExpr(self->cast, writer);
            fprintf(writer, ")");
        }else if(self->type->typeval->type == bt_logic){
            fprintf(writer, "__TEXT_TO_LOGIC__(");
            cgExpr(self->cast, writer);
            fprintf(writer, ")");
        }else if(self->type->typeval->type == bt_text){
            cgExpr(self->cast, writer);
        }
        break;
    case bt_logic:
        if(self->type->typeval->type == bt_number){
            fprintf(writer, "__LOGIC_TO_NUMBER__(");
            cgExpr(self->cast, writer);
            fprintf(writer, ")");
        }else if(self->type->typeval->type == bt_text){
            fprintf(writer, "__LOGIC_TO__TEXT__(");
            cgExpr(self->cast, writer);
            fprintf(writer, ")");
        }else if(self->type->typeval->type == bt_logic){
            cgExpr(self->cast, writer);
        }
        break;
    default:
        break;
    }
}




int cgIsConstantSTRING(Expr* s){
	switch (s->exprType) {
		case et_constant:
		case et_typecast:
		case et_expression:
		case et_binary_operator:
		case et_unary_operator:
			return 1;
		default:
			return 0;
	}
}

void cgPlusSTRING(Expr* right, Expr* left, FILE* writer){
	int usetmp1 = cgIsConstantSTRING(right);
	int usetmp2 = cgIsConstantSTRING(left);

	fprintf(writer, "( STRING_RTN = __STR_CONCAT_CPY__(");

	if(usetmp1) fprintf(writer, "*(STRING_STACK++) = "); 
	cgExpr(right, writer);

	fprintf(writer, ",");

	if(usetmp2) fprintf(writer, "*(STRING_STACK++) = "); 
	cgExpr(left, writer);

	fprintf(writer, ")");
	if(usetmp2) fprintf(writer, ", __DESTROY_STRING__(*(--STRING_STACK))"); 
	if(usetmp1) fprintf(writer, ", __DESTROY_STRING__(*(--STRING_STACK))"); 
	fprintf(writer, ", STRING_RTN)");
}

void cgBinaryOperator(Expr* self, FILE* writer){
    BinaryOperator* bo = self->child;
    switch (bo->bo){
    case bo_plus:
        if(self->extension->type == bt_number){
            cgExpr(bo->childExpr1, writer);
            fprintf(writer, " + ");
            cgExpr(bo->childExpr2, writer);
        }else{
			cgPlusSTRING(bo->childExpr1, bo->childExpr2, writer);
        }
        
        break;
    case bo_minus:
        cgExpr(bo->childExpr1, writer);
        fprintf(writer, " - ");
        cgExpr(bo->childExpr2, writer);
        break;
    case bo_division:
        cgExpr(bo->childExpr1, writer);
        fprintf(writer, " / ");
        cgExpr(bo->childExpr2, writer);
        break;
    case bo_mul:
        cgExpr(bo->childExpr1, writer);
        fprintf(writer, " * ");
        cgExpr(bo->childExpr2, writer);   
        break;
    case bo_modulus:
        cgExpr(bo->childExpr1, writer);
        fprintf(writer, " %% ");
        cgExpr(bo->childExpr2, writer); 
        break;
    case bo_not:
        cgExpr(bo->childExpr1, writer);
        fprintf(writer, " ! ");
        cgExpr(bo->childExpr2, writer); 
        break;
    case bo_eq:
         if(bo->childExpr1->extension->type != bt_text){
            cgExpr(bo->childExpr1, writer);
            fprintf(writer, " == ");
            cgExpr(bo->childExpr2, writer);
        }else{
            fprintf(writer, "strcmp(");
            cgExpr(bo->childExpr1, writer);
            fprintf(writer, "->string, ");
            cgExpr(bo->childExpr2, writer);
            fprintf(writer, "->string) == 0");
        }
        break;
    case bo_neq:
        if(bo->childExpr1->extension->type != bt_text){
            cgExpr(bo->childExpr1, writer);
            fprintf(writer, " != ");
            cgExpr(bo->childExpr2, writer);
        }else{
            fprintf(writer, "strcmp(");
            cgExpr(bo->childExpr1, writer);
            fprintf(writer, "->string, ");
            cgExpr(bo->childExpr2, writer);
            fprintf(writer, "->string) != 0");
        }
        break;
    case bo_gt:
        cgExpr(bo->childExpr1, writer);
        fprintf(writer, " > ");
        cgExpr(bo->childExpr2, writer); 
        break;
    case  bo_gteq:
        cgExpr(bo->childExpr1, writer);
        fprintf(writer, " >= ");
        cgExpr(bo->childExpr2, writer); 
        break;
    case bo_lt:
        cgExpr(bo->childExpr1, writer);
        fprintf(writer, " < ");
        cgExpr(bo->childExpr2, writer); 
        break;
    case bo_lteq:
        cgExpr(bo->childExpr1, writer);
        fprintf(writer, " <= ");
        cgExpr(bo->childExpr2, writer); 
        break;
    case bo_and:
        cgExpr(bo->childExpr1, writer);
        fprintf(writer, " && ");
        cgExpr(bo->childExpr2, writer); 
        break;
    case bo_or:
        cgExpr(bo->childExpr1, writer);
        fprintf(writer, " || ");
        cgExpr(bo->childExpr2, writer); 
        break;
    
    default:
        break;
    }
    
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
	// CONSTANT CODE
    fprintf(writer, "%s", INCLUDES);
	fprintf(writer, "%s", LIST);
	fprintf(writer, "%s", STRING);
	fprintf(writer, "%s", GLOBALS);
	fprintf(writer, "%s", GARBAGE_COLLECTION);
    fprintf(writer, "%s", PRINT);
	fprintf(writer, "%s", EVENTS_INITIALIZERS);
    fprintf(writer, "%s", TYPE_CAST);

	// PREAMBLES
    Preambles* preamblesNode = tree->children->item;
    Funcs* funcsNode = tree->children->next->item;
    cgPreambles(preamblesNode, writer);

    //Init reserved keywords
    fprintf(writer, "%s", RESERVED_KEYWORD);

	// BOARD CODE	
	fprintf(writer, "%s", BOARD);

    YABL_LIST_FOREACH(Nonterminals*, funcsNode->children,
        if(*foreach_value == func){
            cgFuncProtoType((Func*)foreach_value, writer);
        }
    );
    cgFuncs(funcsNode, writer);
    fprintf(writer, "\n");
    fprintf(writer, "%s", YABL_MAIN);
	if(!SEEN_SETUP) fprintf(writer, "void __EVENT_SETUP__(){}\n");
	if(!SEEN_TURN ) fprintf(writer, "void __EVENT_TURN__(){ GAME_RUNNING = 0; }\n");
	if(!SEEN_CLOSE) fprintf(writer, "void __EVENT_CLOSE__(){}\n");
	if(!SEEN_GET_TOKEN) fprintf(writer, "__STRING__T* gettoken(int i, int j){ return __MAKE_STRING__(\"\");}\n");
}

void cgPreambles(Preambles* self, FILE* writer){
    PreamblePlayers* players = NULL;
    PreambleBoard* board = NULL;
    PreambleTile* tile = NULL;
	PreambleGlobals* globals = NULL;

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
		case preambleGlobals:
			globals = (PreambleGlobals*)foreach_value;
        default:
            break;
        };
    );
    cgPreambleTile(tile, board, writer);
    cgPreamblePlayers(players, writer);
	cgPreambleGlobals(globals, writer);
}

void cgPreamblePlayers(PreamblePlayers* self, FILE* writer){
    if(self != NULL){
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
    }else{
        fprintf(writer, "char* PLAYERS[] = {\"P1\"};\n");
    }
    
}


void cgPreambleBoard(PreambleBoard* self, FILE* writer){
	int width, height, writeWidth, writeHeight;
	width = height = writeWidth = writeHeight = 1;
    if(self != NULL){
		width = self->width;
		height = self->height;
		writeWidth = self->writeWidth;
		writeHeight = self->writerHeight;
    }
    fprintf(writer, 
		"#define YABL_BOARD_WIDTH %d\n"
		"#define YABL_BOARD_HEIGHT %d\n"
		"#define YABL_BOARD_WRITE_WIDTH %d\n"
		"#define YABL_BOARD_WRITE_HEIGHT %d\n"
	,	width, height, writeWidth, writeHeight
    );
}

void cgPreambleTile(PreambleTile* self, PreambleBoard* board, FILE* writer){
	YablStack s = YABL_STACK_CREATE;
    if(self != NULL){
        fprintf(writer, "struct Tile {");
        YABL_LIST_FOREACH(Initialization*, self->children, 
			if(foreach_value->type->typeval->type == bt_text)
				yablStackPush(&s, foreach_value);
			cgType(foreach_value->type, writer);
			cgId(&foreach_value->variable, writer);
            fprintf(writer, ";");
        );
        fprintf(writer, "};\n");
    }
    else{
        fprintf(writer, "struct Tile {};\n");
    }
    cgPreambleBoard(board, writer);
	fprintf(writer, "struct Tile board[YABL_BOARD_WIDTH][YABL_BOARD_HEIGHT];\n");
	fprintf(writer, "void __INITIATE_TILES__(){ for(int i = 0; i < YABL_BOARD_WIDTH; i++) {\nfor(int j = 0; j < YABL_BOARD_HEIGHT; j++){\n");
	while(s != NULL){
		Initialization* str = yablStackPop(&s);
		fprintf(writer, "board[i][j].");
		cgId(&str->variable, writer);
		fprintf(writer, " = __MAKE_STRING__(\" \");\n");
	} 
	fprintf(writer, "}\n}\n}\n");
}

void cgPreambleGlobals(PreambleGlobals* self, FILE* writer){
	YablStack s = YABL_STACK_CREATE;
	if(self != NULL) YABL_LIST_FOREACH(Initialization*, self->children, 
		if(foreach_value->type->typeval->type == bt_text)
			yablStackPush(&s, foreach_value);
		cgType(foreach_value->type, writer);
		cgId(&foreach_value->variable, writer);
		fprintf(writer, ";");
	);

	fprintf(writer, "void __INITIATE_GLOABLS__(){\n");
	while(s != NULL){
		Initialization* str = yablStackPop(&s);
		cgId(&str->variable, writer);
		fprintf(writer, " = __MAKE_STRING__(\"\");\n");
	} 
	fprintf(writer, "}\n");

}



void cgFuncs(Funcs* self, FILE* writer){
    YABL_LIST_FOREACH(Nonterminals*, self->children, 
        switch (*foreach_value)
        {
        case event:
            cgEvent((Event*)foreach_value, writer);
            break;
        case func:
            cgFunc((Func*)foreach_value, writer);
            break;
        default:
            break;
        } 
    );
} 

void cgFunc(Func* self, FILE* writer){
	RETURN_TYPE = self->returntype;
	if(!SEEN_GET_TOKEN && strcmp(self->name, "gettoken") == 0)
		SEEN_GET_TOKEN = 1;

    if(self->returntype != NULL){
        cgType(self->returntype, writer);
    }else{
        fprintf(writer, "void ");
    }
    fprintf(writer, "%s (", self->name);
    if(self->args->children->item != NULL){
        Initialization* first = (Initialization*)self->args->children->item;
		cgType(first->type, writer);
		cgId(&first->variable, writer);
        if(self->args->children->next != NULL){
            YABL_LIST_FOREACH(Initialization*, self->args->children->next, 
                fprintf(writer, ",");
                cgType(foreach_value->type, writer);
                cgId(&foreach_value->variable, writer);
            );
        }
    }
    fprintf(writer, ")");
    cgScope(self->scope, writer);
	RETURN_TYPE = NULL;
}

void cgFuncProtoType(Func* self, FILE* writer){
    if(self->returntype != NULL){
        cgType(self->returntype, writer);
    }else{
        fprintf(writer, "void ");
    }
    fprintf(writer, "%s (", self->name);
    if(self->args->children->item != NULL){
        Initialization* first = (Initialization*)self->args->children->item;
		cgType(first->type, writer);
		cgId(&first->variable, writer);
        if(self->args->children->next != NULL){
            YABL_LIST_FOREACH(Initialization*, self->args->children->next, 
                fprintf(writer, ",");
                cgType(foreach_value->type, writer);
                cgId(&foreach_value->variable, writer);
            );
        }
    }
    fprintf(writer, ");\n");
}

void cgEvent(Event* self, FILE* writer){
    fprintf(writer, "void __EVENT_");
    switch (self->eventType)
    {
    case event_setup:
		SEEN_SETUP = 1;
        fprintf(writer, "SETUP__()");
        break;
    case event_turn:
		SEEN_TURN = 1;
        fprintf(writer, "TURN__()");
        break;
    case event_close:
		SEEN_CLOSE = 1;
        fprintf(writer, "CLOSE__()");
        break;
    default:
        break;
    }
    cgScope(self->scope, writer);
}

