#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "yabl_stdlib.h"

void print(char* input){
    printf("%s\n", input);
}

char* input(){
    char input[100];
    scanf("%[^\n]s", input);
    printf("\x1b[u\x1b[2K");
    int length = strlen(input);
    char inputText[length+1];
    char* text = strcpy(inputText, input);
    return text;
}
