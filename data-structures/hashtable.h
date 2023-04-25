#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct YablHashNode {
    void* key;
    void* item;
} YablHashNode;

typedef struct YablHash {
    int sizeOfList;
    void** map; // idea is that this is the array itself
    int(*hashFunc)(void*);
    YablHashNode * first;
} YablHash;

YablHash yablHashCreate(int sizeOfList, int(*hashFunc)(void *));

void* yablHashGet(YablHash* self, void* key, int(*compare)(void*, void*));

/// Creates a copy of value and puts it in the hashmap
void yablHashPushCpy(YablHash* self, void* key, void* value, int(*compare)(void*, void*), int size_of_value);

/// Puts the pointer in the hashmap
void yablHashPush(YablHash* self, void* key, void* value, int(*compare)(void*, void*));

void deleteHelpFunc(YablList self, void(*delete_var)(void*));

/// Deallocates the hashmap
void yablHashDelete(YablHash* self, void(*delete_var)(void*));

void yablHashTests();

//#endif // !YABL_HASHTABLE



