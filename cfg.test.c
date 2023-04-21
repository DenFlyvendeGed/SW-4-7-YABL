#include"test.h"
#include<stdio.h>
#include<stdlib.h>
#include"cfg.h"
#include".target/cfg.tab.h"
#include".target/lex.yy.h"

extern Repeatable* YABL_AST;


int cfgEventTest(){
	yy_scan_string("setup do end");
	yyparse();
	int result = 1;	

	Event* eventType = (Event*)(YABL_AST->children->item);
	result &= eventType->nonterminal = event;
	result &= eventType->eventType   = event_setup;
	return result;
}



void cfgTest(){
	testHeader("Create the AST");
	doTest("Setup Event", cfgEventTest());
}


