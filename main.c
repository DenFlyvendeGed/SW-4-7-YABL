#include <stdio.h>
#include "test.h"

void yyparse();

#ifndef TEST


int main(int argv, char ** args){
	yyparse();
}

#else

int runTests();

int main(int argc, char *argv[]){
	return runTests();
}


#endif



