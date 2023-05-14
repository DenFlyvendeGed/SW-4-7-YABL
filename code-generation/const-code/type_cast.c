#ifndef CODE_TYPECAST
#define CODE_TYPECAST

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "string.c"

//#


int __TEXT_TO_NUMBER__(__STRING__T* string){
    int res;
    res = atoi(string->string);
    return res;
}

int __TEXT_TO_LOGIC__(__STRING__T* string){
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

__STRING__T* __NUMBER_TO_TEXT__(int number){
    __STRING__T* res = __MAKE_STRING__("");
    sprintf(res->string, "%d", number);
    return res;
}

int __NUMBER_TO_LOGIC__(int number){
    return !!number;
}


__STRING__T* __LOGIC_TO_TEXT__(int number){
    __STRING__T* res;
    if(number == 1){
        res = __MAKE_STRING__("true");
    }else if(number == 0){
        res = __MAKE_STRING__("false");
    }else{
        res = __MAKE_STRING__("true");
    }
    return res;

}

int __LOGIC_TO_NUMBER__(int number){
    int res = number;
    return res;
}


//#
#endif

   
