#include <stdio.h>
#include "cfg.h"
#include "test.h"
#include "visitor/visitor.h"

void yyparse();

#ifndef TEST

extern Repeatable* YABL_AST;

int main(int argv, char ** args){
	yyparse();
	visit(YABL_AST);
}

#else

int main(int argc, char *argv[]){
	return runTests();
}


#endif



