#ifndef CODE_PRINT
#define CODE_PRINT
#include <stdio.h>
#include "./string.c"

//#
void print(String* input){
    printf("%s\n", input->string);
}

String* input(){
    char in[100];
    scanf("%[^\n]s", in);
    printf("\x1b[u\x1b[2K");
    return makeString(in);
}
//#
#endif

