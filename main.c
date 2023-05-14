#include <stdio.h>
#include <stdlib.h>
#include "cfg/cfg.h"
#include "test.h"
#include "visitor/visitor.h"
#include "string.h"
#include "code-generation/code-generation.h"
#include "configuration/configuration.h"

void yyparse();

#ifndef TEST

extern Repeatable* YABL_AST;

int main(int argv, char ** args){
	Configuration c = createConfiguration(argv, args);
	yyparse();

	if(visit(YABL_AST) != 0) 
		return EXIT_FAILURE;

	FILE* output = openConfiguration(c);
	cgStart(YABL_AST, output);
	fclose(output);
}

#else

int runTests();

int main(int argc, char *argv[]){
	return runTests(argc - 1);
}


#endif



