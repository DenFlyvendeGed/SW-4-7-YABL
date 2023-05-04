#include "string.h"

String makeStringSize(char* string, int stringSize){
    char* string_ = (char*) malloc(stringSize);
    strcpy(string_, string);
    String new_string = {.string = string_, .alloc = stringSize};
    return new_string;
}

void reallocString(String string, int stringSize){
    string.string = realloc(string.string ,stringSize);
    string.alloc = stringSize;
}

String strConcat(String string_1, String string_2){
    int newLen = strlen(string_1.string) + strlen(string_2.string)+1;
    if(string_1.alloc > newLen){
        strcat(string_1.string, string_2.string);
    }else{
        reallocString(string_1, newLen);
        strcat(string_1.string, string_2.string);
    }
    return string_1;
}