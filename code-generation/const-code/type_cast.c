#ifndef CODE_TYPECAST
#define CODE_TYPECAST

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "string.c"

//#


int tcTextToNumber(String* string){
    int res;
    res = atoi(string->string);
    return res;
}




//#
#endif

