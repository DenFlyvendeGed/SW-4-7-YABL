#include "hashtable.h"
#include "list.h"

typedef struct Node {
    char* key;
    void* item;
} Node;


YablHash yablHashCreate(int sizeOfList, void(*hashFunc)(char *)) {
    YablHash rtn;
    rtn.sizeOfList;
    rtn.map = malloc(sizeof(void **)*(rtn.sizeOfList));
    rtn.hashFunc = hashFunc;
    return rtn;
}

void* yablHashGet(YablHash* self, char* key, void* value){
	int hashIndex = self->hashFunc(key);
    void* item = yablListFind(self->map[hashIndex], compare(value));
    return item;
}// returns item as a void pointer

/// Puts the pointer in the hashmap
void* yablHashPush(YablHash* self, char* key, void* value){
    if( !(self->map[self->hashFunc(key)]) ){ // if there is no pointer at index
        self->map[self->hashFunc(key)] = yablListCreate();
        yablListPush(self->map[self->hashFunc(key)], value);
    }else{
        yablListPush(self->map[self->hashFunc(key)], value);
    }
}

/// Creates a copy of value and puts it in the hashmap, mainly for if values go out of scope
void* yablHashPushCpy(YablHash* self, char* key, void* value, int size_of_value){
    char * cpy = malloc(size_of_value);
	for(int i = 0; i < size_of_value; i++)
		cpy[i] = ((char*)value)[i];
	yablHashPush(self, key, cpy);
}

void yablHashDelete(YablHash *self, void(*delete_var)(void*)) {
    for (int i = 0; i < ((sizeof(self->map))/(sizeof(void**))); i++) {
        if(self->map[i]){
            yablListDelete(self->map[i], delete_var);
        }
    }
    free(self->map);
}

/// Loops through all components in the map
void yablHashForeach(YablHash *self, char* key, void(*foreach)(void *, va_list), int n_args, ...){
    for (int i = 0; i < ((sizeof(self->map))/(sizeof(void**))); i++){
        YablList list = self->map[i];
        yablListForeach(list, *foreach, n_args);
    }
}


