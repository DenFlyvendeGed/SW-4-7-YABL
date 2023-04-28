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
    rtn.map = calloc(sizeof(void **),(rtn.sizeOfList));
    rtn.hashFunc = hashFunc;
    return rtn;
}

void* yablHashGet(YablHash* self, void* key, int(*compare)(void*, void*)){
    int hashIndex = self->hashFunc(key);
    YablList list = self->map[hashIndex];
    int length = 0;
    for(int i = 0; i < length; i++){
        if(compare(((Node*)list->item)->key, key)){
            return list->item;
        }
        list = list->next;
    }
    return NULL;
}// returns item as a void pointer

/// Puts the pointer in the hashmap
void yablHashPush(YablHash* self, void* key, void* value, int(*compare)(void*, void*)){
    Node* node = malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    int hashIndex = self->hashFunc(node->key);
    if( (self->map[hashIndex]) == NULL ){ // if there is no pointer at index
        self->map[hashIndex] = yablListCreate();
        yablListPush(self->map[hashIndex], node);
    }else{
        if (((Node*)yablHashGet(self, key, compare)) != NULL) {
            ((Node*)yablHashGet(self, key, compare))->value = node->value;
        }else{
            yablListPush(self->map[hashIndex], node);
        }
    }
}

/// Creates a copy of value and puts it in the hashmap, mainly for if values go out of scope
void yablHashPushCpy(YablHash* self, void* key, void* value, int(*compare)(void*, void*), int size_of_value){
    char * cpy = malloc(size_of_value);
	for(int i = 0; i < size_of_value; i++)
		cpy[i] = ((char*)value)[i];
	yablHashPush(self, key, cpy, compare);
}

void deleteHelpFunc(YablList self, void(*delete_var)(void*)){
	if(self->next != NULL) deleteHelpFunc(self->next, delete_var);
	if(self->item != NULL){
        delete_var(((Node*)self->item)->value);
        delete_var(self->item);
    }
	delete_var(self);
}
void yablHashDelete(YablHash *self, void(*delete_var)(void*)) {
    YablList list;
    for (int i = 0; i < self->sizeOfList; i++) {
        list = self->map[i];
        if(list != NULL){
            deleteHelpFunc(self->map[i], delete_var);
        }
    }
    delete_var(self->map);
    self->map = NULL;
}

/// Tests

int testHashFunc(void* key){
    int index = *(int *)key % 10;
    return index;
}

int compare(void* item_key, void* key){
    if(*(int*)item_key == *(int*)key){
        return 1;
    }else{
        return 0;
    }
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
    int result = 0; 
    int key_ = 7;
    int key2_ = 11;
    int value_ = 69;
    int value2_ = 79;
    YablHash hash_ = yablHashCreate(10, &testHashFunc);

    void* key = &key_;
    int* value = (int*)malloc(sizeof(int));
    *value = value_;

    void* key2 = &key2_;
    int* value2 = (int*)malloc(sizeof(int));
    *value2 = value2_;

    YablHash* hash = &hash_;

    yablHashPush(hash, key, value, &compare);
    yablHashPush(hash, key, value2, &compare);
    yablHashPush(hash, key2, value, &compare);
    Node* hashItem = yablHashGet(hash, key, &compare);
    int* hashValue1 = (int*)hashItem->value;
    Node* hashItem2 = yablHashGet(hash, key2, &compare);
    int* hashValue2 = (int*)hashItem2->value;
    if(*(int*)hashValue1 == value2_){
        result++;
    }
    if(*(int*)hashValue2 == value_){
        result++;
    }
    yablHashDelete(hash, &free);
    return result == 2;
 }

int yablHashPushCpyTest(){
    int result = 0; 
    int key_ = 7;
    int key2_ = 11;
    int value_ = 69;
    int value2_ = 79;
    YablHash hash_ = yablHashCreate(10, &testHashFunc);

    void* key = &key_;
    void* value = &value_;
    void* key2 = &key2_;
    void* value2 = &value2_;
    YablHash* hash = &hash_;

    yablHashPushCpy(hash, key, value, &compare, sizeof(int));
    yablHashPushCpy(hash, key, value2, &compare, sizeof(int));
    yablHashPushCpy(hash, key2, value, &compare, sizeof(int));

    Node* hashItem = yablHashGet(hash, key, &compare);
    int* hashValue1 = (int*)hashItem->value;
    Node* hashItem2 = yablHashGet(hash, key2, &compare);
    int* hashValue2 = (int*)hashItem2->value;
    if(*(int*)hashValue1 == value2_){
        result++;
    }
    if(*(int*)hashValue2 == value_){
        result++;
    }
    yablHashDelete(hash, &free);
    return result == 2;
}


int yablHashDeleteTest(){
    int result = 0; 
    int key_ = 7;
    int value_ = 69;
    YablHash hash_ = yablHashCreate(10, &testHashFunc);
    YablHash* hash = &hash_;
    yablHashDelete(hash, &free);
    if(hash->map == NULL){
        result = 1;
        return result;
    }else{
        return 0;
    }
}

void yablHashTests(){
    testHeader("YablHash");
    doTest("Hash Create", yablHashCreateTest());
	doTest("Hash Get and Push", yablHashGetTest());
	doTest("Hash Push Copy", yablHashPushCpyTest());
    doTest("Hash Delete", yablHashDeleteTest());
}
