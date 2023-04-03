#include "hashtable.h"
#include "list.h"
#include "../test.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void* key;
    void* item;
} Node;


YablHash yablHashCreate(int sizeOfList, void*(*hashFunc)(void *)) {
    YablHash rtn;
    rtn.sizeOfList;
    rtn.map = malloc(sizeof(void **)*(rtn.sizeOfList));
    rtn.hashFunc = hashFunc;
    return rtn;
}

void* yablHashGet(YablHash* self, void* key, void* value){
	int hashIndex = self->hashFunc(key);
    void* item = yablListFind(self->map[hashIndex], compare(value));
    return item;
}// returns item as a void pointer

/// Puts the pointer in the hashmap
void* yablHashPush(YablHash* self, void* key, void* value){
    if( !(self->map[self->hashFunc(key)]) ){ // if there is no pointer at index
        self->map[self->hashFunc(key)] = yablListCreate();
        yablListPush(self->map[self->hashFunc(key)], value);
    }else{
        yablListPush(self->map[self->hashFunc(key)], value);
    }
}

/// Creates a copy of value and puts it in the hashmap, mainly for if values go out of scope
void* yablHashPushCpy(YablHash* self, void* key, void* value, int size_of_value){
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
void yablHashForeach(YablHash *self, void* key, void(*foreach)(void *, va_list), int n_args, ...){
    for (int i = 0; i < ((sizeof(self->map))/(sizeof(void**))); i++){
        YablList list = self->map[i];
        yablListForeach(list, *foreach, n_args);
    }
}

/// Tests

testHashFunc(void* key){
    int index = *(int *)key % 20;
    return index;
}

foreachFunc(void* item, va_list ap){
    (*va_arg(ap, int*))++;
}

int yablHashCreateTest(){
    int result = 0;
    int key_ = 7;
    void* key = &key_;
    void*(*func)(void*) = &testHashFunc;
    YablHash hash = yablHashCreate(10, &testHashFunc);
    if(hash.sizeOfList == 10)
        result++;
    if(sizeof(hash.map) == sizeof(void **)*hash.sizeOfList)
        result++;
    if(hash.hashFunc(key) == func(key)){
        result++;
    }
    return result;

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
    void* hashValue = yablHashGet(hash, key, value);
    if((int*)hashValue == value_){
        result = 1;
        return result;
    }
}

yablHashPushTest(){
    
}

yablHashPushCpyTest(){
    int result; 
    int key_ = 7;
    int value_ = 69;
    YablHash hash_ = yablHashCreate(10, &testHashFunc);

    void* key = &key_;
    void* value = &value_;
    YablHash* hash = &hash_;

    yablHashPushCpy(hash, key, value, sizeof(int));
    void* hashValue = yablHashGet(hash, key, value);
    if((int*)hashValue == value_){
        result = 1;
        return result;
    }
}

yablHashDeleteTest(){
    int result; 
    int key_ = 7;
    int value_ = 69;
    YablHash hash_ = yablHashCreate(10, &testHashFunc);
    YablHash* hash = &hash_;
    yablHashDelete(hash, &free);

}

yablHashForeachTest(){
    int result = 0; 
    int key_ = 7;
    int value_ = 69;
    YablHash hash_ = yablHashCreate(3, &testHashFunc);

    void* key = &key_;
    void* value = &value_;
    YablHash* hash = &hash_;

    yablHashPushCpy(hash, key, value, sizeof(int));
    yablHashPushCpy(hash, key, value, sizeof(int));
    yablHashPushCpy(hash, key, value, sizeof(int));
    yablHashForeach(hash, key, &foreachFunc, 1, &result);
    yablHashDelete(hash, &free);
    return result == 3;
}

void yablHashTests(){
    testHeader("YablList");
    doTest("Hash Create", yablHashCreateTest());
	doTest("Hash Get", yablHashGetTest());
	doTest("Hash Push Copy", yablHashPushCpyTest());
	doTest("Hash Delete", yablHashDeleteTest());
    doTest("Hash Foreach", yablHashForeachTest());
}
