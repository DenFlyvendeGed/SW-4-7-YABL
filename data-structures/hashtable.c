#include "hashtable.h"
#include "list.h"
#define SIZE 20

typedef struct Node {
    char* key;
    void* item;
} Node;

int hashFunc(char* key){
 return *key % SIZE;
}

YablHash yablHashCreate(int sizeOfList) {
    YablHash rtn;
    rtn.sizeOfList;
    rtn.map = malloc(sizeof(void **)*(rtn.sizeOfList));
    rtn.hashFunc = &hashFunc;
    return rtn;
}

void* yablHashGet(YablHash* self, char* key){
	int hashIndex = self->hashFunc(key);
    void* value = self->map[hashIndex];
    return value;
}// returns item as a void pointer

/// Puts the pointer in the hashmap
void* yablHashPush(YablHash* self, char* key, char* key2, void* value){
    if( !(self->map[self->hashFunc(key)]) ){ // if there is no pointer at index
        void** list = malloc(sizeof(value)*20);
        list[*key2] = value;
        self->map[self->hashFunc(key)] = list;
    }else{
        void** list;
        if(list[self->hashFunc(key2)]){
           list= (void**)realloc(list, sizeof(value)*20+1);
        }
        list[self->hashFunc(key2)] = value; 
        self->map[self->hashFunc(key)] = list;
    }
}

/// Creates a copy of value and puts it in the hashmap
void* yablHashPushCpy(YablHash* self, char* key, void* value){

}

void yablHashDelete(YablHash *self) {
    for (int i = 0; i < ((sizeof(self->map))/(sizeof(void**))); i++) {
        if(self->map[i]){
            free(self->map[i]);
        }
    }
    free(self->map);
}

/// Loops through all components in the map
void yablHashForeach(char* key, void (*foreach)(void *)){

}

