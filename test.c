// SETUP
#include <stdio.h>
#include "test.h"

int test_func_success(){return 1;}
int test_func_fail(){return 0;}

int YABL_TEST_FAILED = 0;
int YABL_TEST_SUCCESS  = 0;

void test(){
	TEST("TEST MUST SUCCEDE", test_func_success());
	TEST("TEST MUST FAIL", !test_func_fail());
}


int main() {
	printf("\x1b[1m~ RUNNING TESTS ~\x1b[0m\n");
	test();
	printf("\x1b[1m~ FINNISHED TESTS ~\x1b[0m\n");
	printf("|-------------------|\n");
	printf("| SUCCEDED | FAILED |\n");
	printf("| %8d | %6d |\n", YABL_TEST_SUCCESS, YABL_TEST_FAILED);
	printf("|-------------------|\n");

	return YABL_TEST_FAILED == 0 ? 0 : -1;
}



