#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

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
void yablHashPushCpy(YablHash* self, void* key, void* value,  int size_of_value);

/// Puts the pointer in the hashmap
void yablHashPush(YablHash* self, void* key, void* value);

/// Deallocates the hashmap
void yablHashDelete(YablHash* self, void(*delete_var)(void*));

/// Loops through all components in the map
void yablHashForeach(YablHash* self, void* key, void(*foreach)(void *, va_list), int n_args, ...);

void yablHashTests();

//#endif // !YABL_HASHTABLE



