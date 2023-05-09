#ifndef CODE_PRINT
#define CODE_PRINT
#include <stdio.h>
#include "./string.c"

//#
void print(String* text){
	printf("%s", text->string);
}
//#
#endif

