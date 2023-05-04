#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct String{
    char* string;
    int alloc;
}String;

String makeStringSize(char* string, int stringSize);
void reallocString(String string, int stringSize);
String strConcat(String string_1, String string_2);
