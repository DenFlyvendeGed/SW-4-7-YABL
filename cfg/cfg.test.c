#include"../test.h"
#include<stdio.h>
#include<stdlib.h>
#include"cfg.h"
#include"../.target/cfg.tab.h"
#include"../.target/lex.yy.h"

extern Repeatable* YABL_AST;


int cfgSetupEventTest(){
	yy_scan_string("setup do end");
	yyparse();
	int result = 1;	

	Event* eventType = (Event*)(YABL_AST->children->item);
	result &= eventType->nonterminal == event;
	result &= eventType->eventType   == event_setup;
	return result;
}

int cfgEventsTest(){
	yy_scan_string("setup do end\nturn do end\nclose do end");
	yyparse();

	int result = 1;
	Event* eventType = (Event*)(YABL_AST->children->item);
	result &= eventType->nonterminal == event;
	result &= eventType->eventType   == event_setup;

	eventType = (Event*)(YABL_AST->children->next->item);
	result &= eventType->nonterminal == event;
	result &= eventType->eventType   == event_turn;

	eventType = (Event*)(YABL_AST->children->next->next->item);
	result &= eventType->nonterminal == event;
	result &= eventType->eventType   == event_close;	

	return result;
}

int cfgVoidFunctionTest(){
	yy_scan_string("function test() do end");
	yyparse();

	int result = 1;
	Func* function = (Func*)(YABL_AST->children->item);
	result &= function->nonterminal == func;
	result &= strcmp(function->name, "test") == 0;
	result &= function->returntype == NULL;

	return result;
}


void cfgTest(){
	testHeader("Create the AST");
	doTest("Setup Event", cfgSetupEventTest());
	doTest("All Events", cfgEventsTest());
	doTest("(Void) Function", cfgVoidFunctionTest());
}


