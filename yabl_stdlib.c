#include <stdio.h>
#include <stdarg.h>
#include "test.h"

void print(char* input){
    printf("%s", input);
}

char* input(char* input){
    char* inputText;
    sscanf(inputText, "%s", input);
    return inputText;
}

