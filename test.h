#ifndef YABL_TEST
#define YABL_TEST
#include <stdio.h>


extern int YABL_TEST_SUCCESS;
extern int YABL_TEST_FAILED;

#define TEST(name, result)\
	result ? (printf("[\x1b[32mSUCCESS\x1b[0m] %s\n", name), YABL_TEST_SUCCESS++)\
		   : (printf("[\x1b[31mFAILED \x1b[0m] %s\n", name), YABL_TEST_FAILED++)

#endif // !YABL_TEST

