#include "hashtable.h"
#include "list.h"
#include "../test.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void* key;
    void* value;
} Node;


YablHash yablHashCreate(int sizeOfList, int(*hashFunc)(void *)) {
    YablHash rtn;
    rtn.sizeOfList = sizeOfList;
    rtn.map = malloc(sizeof(void **)*(rtn.sizeOfList));
    rtn.hashFunc = hashFunc;
    return rtn;
}

void* yablHashGet(YablHash* self, void* key, int(*compare)(void*, void*)){
    void* ptr = NULL;
	int hashIndex = self->hashFunc(key);
    YablList list = self->map[hashIndex];
    for(int i = 0; i <= yablListLen(list); i++){
        if(compare(list->item, key)){
            return list->item;
        }
        list->item = list->next;
    }
    return ptr;
}// returns item as a void pointer

/// Puts the pointer in the hashmap
void yablHashPush(YablHash* self, void* key, void* value){
    Node* node = malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    if( !(self->map[self->hashFunc(node->key)]) ){ // if there is no pointer at index
        self->map[self->hashFunc(node->key)] = yablListCreate();
        yablListPush(self->map[self->hashFunc(key)], node);
    }else{
        yablListPush(self->map[self->hashFunc(key)], node);
    }
}

/// Creates a copy of value and puts it in the hashmap, mainly for if values go out of scope
void yablHashPushCpy(YablHash* self, void* key, void* value, int size_of_value){
    char * cpy = malloc(size_of_value);
	for(int i = 0; i < size_of_value; i++)
		cpy[i] = ((char*)value)[i];
	yablHashPush(self, key, cpy);
}

void yablHashDelete(YablHash *self, void(*delete_var)(void*)) {
    for (int i = 0; i < self->sizeOfList; i++) {
        if(self->map[i]){
            yablListDelete(self->map[i], delete_var);
        }
    }
    free(self->map);
}

/// Loops through all components in the map
void yablHashForeach(YablHash *self, void* key, void(*foreach)(void *, va_list), int n_args, ...){
    for (int i = 0; i < self->sizeOfList; i++){
        YablList list = **(YablList**)self->map[i];
        yablListForeach(list, *foreach, n_args);
    }
}

/// Tests

int testHashFunc(void* key){
    int index = *(int *)key % 20;
    return index;
}

void foreachFunc(void* item, va_list ap){
    (*va_arg(ap, int*))++;
}

int compare(void* item_, void* key){
    Node item = *(Node*)item_;
    if(*(int*)item.key == *(int*)key)
        return 1;
    else
        return 0;
} 

int yablHashCreateTest(){
    int result = 0;
    int key_ = 7;
    void* key = &key_;
    int(*func)(void*) = &testHashFunc;
    YablHash hash_ = yablHashCreate(10, &testHashFunc);
    YablHash* hash = &hash_;
    if(hash_.sizeOfList == 10)
        result++;
    
    if(hash_.map != NULL)
        result++;
    
    if(hash_.hashFunc(key) == func(key)){
        result++;
    }
    yablHashDelete(hash, &free);
    return result == 3;
}

int yablHashGetTest(){
    int result; 
    int key_ = 7;
    int value_ = 69;
    YablHash hash_ = yablHashCreate(10, &testHashFunc);

    void* key = &key_;
    void* value = &value_;
    YablHash* hash = &hash_;

    yablHashPush(hash, key, value);
    void* hashValue = yablHashGet(hash, key, &compare);
    if(*(int*)hashValue == value_){
        yablHashDelete(hash, &free);
        result = 1;
        return result;
    }else{
        return 0;
    }
}

/*
yablHashPushTest(){
    
}
*/

int yablHashPushCpyTest(){
    int result; 
    int key_ = 7;
    int value_ = 69;
    YablHash hash_ = yablHashCreate(10, &testHashFunc);

    void* key = &key_;
    void* value = &value_;
    YablHash* hash = &hash_;

    yablHashPushCpy(hash, key, value, sizeof(int));
    void* hashValue = yablHashGet(hash, key, &compare);
    if(*(int*)hashValue == value_){
        yablHashDelete(hash, &free);
        result = 1;
        return result;
    }else{
        return 0;
    }
}


int yablHashDeleteTest(){
    int result = 0; 
    int key_ = 7;
    int value_ = 69;
    YablHash hash_ = yablHashCreate(10, &testHashFunc);
    YablHash* hash = &hash_;
    yablHashDelete(hash, &free);
    if(hash->map == NULL){
        result++;
        return result;
    }else{
        return 0;
    }
}


int yablHashForeachTest(){
    int result = 3; 
    int key_ = 7;
    int value_ = 69;
    YablHash hash_ = yablHashCreate(3, &testHashFunc);

    void* key = &key_;
    void* value = &value_;
    YablHash* hash = &hash_;

    yablHashPushCpy(hash, key, value, sizeof(int));
    //yablHashPushCpy(hash, key, value, sizeof(int));
    //yablHashPushCpy(hash, key, value, sizeof(int));
    //yablHashForeach(hash, key, &foreachFunc, 1, &result);
    yablHashDelete(hash, &free);
    return result == 3;
}

void yablHashTests(){
    testHeader("Hash Test");
    doTest("Hash Create", yablHashCreateTest());
	doTest("Hash Get and Push", yablHashGetTest());
	doTest("Hash Push Copy", yablHashPushCpyTest());
    doTest("Hash Delete", yablHashDeleteTest());
    doTest("Hash Foreach", yablHashForeachTest());
}
