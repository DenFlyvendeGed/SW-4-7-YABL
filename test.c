// SETUP
#include <stdio.h>
#include "test.h"
#include "./data-structures/list.h"
#include "./data-structures/hashtable.h"

int testFuncSuccess(){return 1;}
int testFuncFail(){return 0;}

int YABL_TEST_FAILED = 0;
int YABL_TEST_SUCCESS  = 0;

#define DO_TEST(name, result)\
	result ? (printf("[\x1b[32mSUCCESS\x1b[0m] %s\n", name), YABL_TEST_SUCCESS++)\
		   : (printf("[\x1b[31mFAILED \x1b[0m] %s\n", name), YABL_TEST_FAILED++)
/////////////////////// TEST /////////////////////////
void test(){
	yablListTests();
	yablHashTests();

	testHeader("Test of Tests");
	doTest("TEST MUST SUCCEED", testFuncSuccess());
	doTest("TEST MUST FAIL", !testFuncFail());
}
/////////////////////////////////////////////////////

void testHeader(char* name){
	printf("\n\x1b[1m~ ~ %s ~ ~\x1b[0m\n", name);
}

void doTest(char* name, int result){
	DO_TEST(name, result);
}


int main() {
	printf("\x1b[1m~ RUNNING TESTS ~\x1b[0m\n");
	test();
	printf("\n\x1b[1m~ FINNISHED TESTS ~\x1b[0m\n");
	printf("┌──────────┬────────┐\n");
	printf("│ SUCCEDED │ FAILED │\n");
	printf("│ %8d │ %6d │\n", YABL_TEST_SUCCESS, YABL_TEST_FAILED);
	printf("└──────────┴────────┘\n");

	return YABL_TEST_FAILED == 0 ? 0 : -1;
}




