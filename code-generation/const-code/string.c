#ifndef CODE_STRING
#define CODE_STRING

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.c"

//#
typedef struct __STRING__T{
    char* string;
    int alloc;
	int free;
}__STRING__T;

__STRING__T* _STRING_STACK[15];
__STRING__T** STRING_STACK = _STRING_STACK;
__STRING__T* STRING_RTN;

__STRING__T* __MAKE_STRING_SIZE__(char* string, int stringSize){
    char* string_ = (char*) malloc(stringSize);
    strcpy(string_, string);

    __STRING__T* new_string = malloc(sizeof(string));
	new_string->string = string_; 
	new_string->alloc = stringSize;
	new_string->free = 1;
	
    return new_string;
}

__STRING__T* __MAKE_STRING__(char *string){
	int len = strlen(string);
	return __MAKE_STRING_SIZE__(string, len <= 30 ? 30 : len);
}

void __REALLOC_STRING__(__STRING__T* string, int stringSize){
    string->string = realloc(string->string ,stringSize);
    string->alloc = stringSize;
}

__STRING__T* __STR_CONCAT__(__STRING__T* string_1, __STRING__T* string_2){
    int newLen = strlen(string_1->string) + strlen(string_2->string)+1;
    if(string_1->alloc > newLen){
        strcat(string_1->string, string_2->string);
    }else{
        __REALLOC_STRING__(string_1, newLen);
        strcat(string_1->string, string_2->string);
    }
    return string_1;
}

__STRING__T* __STR_CONCAT_CPY__(__STRING__T* string_1, __STRING__T* string_2){
    int newLen = strlen(string_1->string) + strlen(string_2->string)+1;

	__STRING__T* new = __MAKE_STRING_SIZE__("", newLen <= 30 ? 30 : newLen);
	strcat(new->string, string_1->string);
	strcat(new->string, string_2->string);
	return new;
}

__STRING__T* __COPY_STRING__(__STRING__T* string){
	return __MAKE_STRING_SIZE__(string->string, string->alloc);
}

void __DESTROY_STRING__(__STRING__T* self){
	free(self->string);
	free(self);
}

//#
#endif

