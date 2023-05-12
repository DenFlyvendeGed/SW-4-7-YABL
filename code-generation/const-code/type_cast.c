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

int tcTextToLogic(String* string){
    int res;
    if(strcmp(string->string, "true") == 0){
        res = 1;
    } 
    else if(strcmp(string->string, "false") == 0){
        res = 0;
    }
    else{
        res = 1;
    }
    return res;
}

String* tcNumberToText(int number){
    String* res = makeString("");
    sprintf(res->string, "%d", number);
    return res;
}

int tcNumberToLogic(int number){
    return !!number;
}


String* tcLogicToText(int number){
    String* res;
    if(number == 1){
        res = makeString("true");
    }else if(number == 0){
        res = makeString("false");
    }else{
        res = makeString("true");
    }
    return res;

}

int tcLogicToNumber(int number){
    int res = number;
    return res;
}





//#
#endif

   