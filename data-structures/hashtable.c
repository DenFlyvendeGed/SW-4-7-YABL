#include "hashtable.h"
#define SIZE 20

typedef struct Node {
    char* key;
    void* item;
} Node;

int hashFunc(char* key){
 return *key % SIZE;
}

YablHash yablHashCreate(int size_of_items) {
    YablHash rtn;
    rtn.map = malloc(sizeof(void **)*20);
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
    if( !(self->map[self->hashFunc(key)]) ){
        void** list = malloc(sizeof(void *)*20);
        list[*key2] = value;
        self->map[self->hashFunc(key)] = list;
    }else{
        void** list = self->map[self->hashFunc(key)];
        list[self->hashFunc(key2)] = value; 
    }
}

/// Creates a copy of value and puts it in the hashmap
void* yablHashPushCpy(YablHash* self, char* key, void* value){

}

void yablHashDelete(YablHash *self) {
    for (int i = 0; i < ((sizeof(self->map))/(sizeof(void**))); ++i) {
        if(self->map[i]){
            free(self->map[i]);
        }
    }
    free(self->map);
}

/// Loops through all components in the map
void yablHashForeach(char* key, void (*foreach)(void *)){

}

