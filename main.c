#include <stdio.h>
#include "cfg.h"
#include "test.h"
#include "visitor/visitor.h"
#include "string.h"
#include "code-generation/code-generation.h"

void yyparse();

#ifndef TEST

extern Repeatable* YABL_AST;

int main(int argv, char ** args){
	yyparse();
	//visit(YABL_AST);
	cgStart(YABL_AST, stdout);
}

#else

int runTests();

int main(int argc, char *argv[]){
	return runTests(argc - 1);
}


#endif



