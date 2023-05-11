#include "string.h"

#define DEFAULT_STRING_SIZE 30

String makeStringSize(char* string, int stringSize){
    char* string_ = (char*) malloc(stringSize);
    strcpy(string_, string);
    String new_string = {.string = string_, .alloc = stringSize};
    return new_string;
}

String makeString(char *string){
	return makeStringSize(string, DEFAULT_STRING_SIZE);
}

void reallocString(String string, int stringSize){
    string.string = realloc(string.string ,stringSize);
    string.alloc = stringSize;
}

String strConcat(String string_1, String string_2){
    int newLen = strlen(string_1.string) + strlen(string_2.string)+1;

    String new = makeStringSize("", newLen >= DEFAULT_STRING_SIZE ? newLen : DEFAULT_STRING_SIZE);
    strcat(new.string, string_1.string);
    strcat(new.string, string_2.string);

    return new;
}

void destroyString(String string){
    free(string.string);
}