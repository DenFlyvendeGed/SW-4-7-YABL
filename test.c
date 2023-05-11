// SETUP
#include <stdio.h>
#include <stdlib.h>
#include "test.h"
#include "./data-structures/list.h"
#include <unistd.h>
#include <string.h>

void cfgTest();
#include "./data-structures/hashtable.h"

int testFuncSuccess(){return 1;}
int testFuncFail(){return 0;}

int YABL_TEST_FAILED = 0;
int YABL_TEST_SUCCESS  = 0;

int OUT;

#define DO_TEST(name, result)\
	result ? (dprintf(OUT, "[\x1b[32mSUCCESS\x1b[0m] %s\n", name), YABL_TEST_SUCCESS++)\
		   : (dprintf(OUT, "[\x1b[31mFAILED \x1b[0m] %s\n", name), YABL_TEST_FAILED++)
/////////////////////// TEST /////////////////////////
void test(){
	yablListTests();
	cfgTest();
	yablHashTests();

	testHeader("Test of Tests");
	doTest("TEST MUST SUCCEED", testFuncSuccess());
	doTest("TEST MUST FAIL", !testFuncFail());
}
/////////////////////////////////////////////////////

void testHeader(char* name){
	dprintf(OUT, "\n\x1b[1m~ ~ %s ~ ~\x1b[0m\n", name);
}

void doTest(char* name, int result){
	DO_TEST(name, result);
}


int runTests(int pstdout) {
	if(pstdout) {
		OUT = STDOUT_FILENO;
	} else {
		OUT = dup(STDOUT_FILENO);
		close(STDOUT_FILENO);	
		close(STDERR_FILENO);	
	}

	dprintf(OUT, "\x1b[1m~ RUNNING TESTS ~\x1b[0m\n");
	test();
	dprintf(OUT, "\n\x1b[1m~ FINNISHED TESTS ~\x1b[0m\n");
	dprintf(OUT, "┌──────────┬────────┐\n");
	dprintf(OUT, "│ SUCCEDED │ FAILED │\n");
	dprintf(OUT, "│ %8d │ %6d │\n", YABL_TEST_SUCCESS, YABL_TEST_FAILED);
	dprintf(OUT, "└──────────┴────────┘\n");

	return YABL_TEST_FAILED == 0 ? 0 : -1;
}




