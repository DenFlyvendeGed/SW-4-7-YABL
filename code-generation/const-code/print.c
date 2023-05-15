#ifndef CODE_PRINT
#define CODE_PRINT
#include <stdio.h>
#include "./string.c"

//#
void print(__STRING__T* input){
    printf("%s\n", input->string);
}

__STRING__T* input(){
    char in[100];
    scanf("%[^\n]s", in);
	getc(stdin);
    return __MAKE_STRING__(in);
}
//#
#endif

