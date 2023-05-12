#ifndef CODE_STRING
#define CODE_STRING

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.c"

//#
typedef struct String{
    char* string;
    int alloc;
	int free;
}String;

String* _STRING_STACK[15];
String** STRING_STACK = _STRING_STACK;
String* STRING_RTN;

String* makeStringSize(char* string, int stringSize){
    char* string_ = (char*) malloc(stringSize);
    strcpy(string_, string);

    String* new_string = malloc(sizeof(string));
	new_string->string = string_; 
	new_string->alloc = stringSize;
	new_string->free = 1;
	
    return new_string;
}

String* makeString(char *string){
	int len = strlen(string);
	return makeStringSize(string, len <= 30 ? 30 : len);
}

void reallocString(String* string, int stringSize){
    string->string = realloc(string->string ,stringSize);
    string->alloc = stringSize;
}

String* strConcat(String* string_1, String* string_2){
    int newLen = strlen(string_1->string) + strlen(string_2->string)+1;
    if(string_1->alloc > newLen){
        strcat(string_1->string, string_2->string);
    }else{
        reallocString(string_1, newLen);
        strcat(string_1->string, string_2->string);
    }
    return string_1;
}

String* strConcatCpy(String* string_1, String* string_2){
    int newLen = strlen(string_1->string) + strlen(string_2->string)+1;

	String* new = makeStringSize("", newLen <= 30 ? 30 : newLen);
	strcat(new->string, string_1->string);
	strcat(new->string, string_2->string);
	return new;
}

String* copyString(String* string){
	return makeStringSize(string->string, string->alloc);
}

String* addStringToGC(String* string){
	return string;
}

void destroyString(String* self){
	free(self->string);
	free(self);
}

//#
#endif

