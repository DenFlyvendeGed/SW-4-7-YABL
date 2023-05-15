#include <stdio.h>
#define TEST(name, result)\
	result ? (printf("[\x1b[32mSUCCESS\x1b[0m] %s\n", name), success++)\
		   : (printf("[\x1b[31mFAILED \x1b[0m] %s\n", name), failed++)

int test_func_success(){return 1;}
int test_func_fail(){return 0;}

int success = 0;
int failed  = 0;

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
	printf("| %8d | %6d |\n", success, failed);
	printf("|-------------------|\n");

	return failed == 0 ? 0 : -1;
}



