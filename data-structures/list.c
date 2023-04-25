#include "list.h"
#include "../test.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

YablList yablListCreate(){
	YablList list = malloc(sizeof(YablListNode));
	list->next = NULL;
	list->item = NULL;
	return list;
}

void yablListDelete(YablList self, void(*delete_var)(void*)){
	if(self->next == NULL) goto end;
	yablListDelete(self->next, delete_var);
	if(self->item != NULL) delete_var(self->item);
	end:
	free(self);
}

void yablListPush(YablList self, void *item){
	if(self->item == NULL){
		self->item = item;
	} else {
		while(self->next != NULL) self = self->next;
		self->next = yablListCreate();
		self->next->item = item;
	}
}

void yablListPushCpy(YablList self, void *item, int size_of_item){
	char * cpy = malloc(size_of_item);
	for(int i = 0; i < size_of_item; i++)
		cpy[i] = ((char*)item)[i];
	yablListPush(self, cpy);
}

void* yablListGet(YablList self, int index){
	for(int i = 0; i < index; i++){
		self = self->next;
		if( self == NULL ) return NULL;
	}
	return self->item;
}

void* yablListPopRtn(YablList self){
	if(self->next == NULL) {
		void* item = self->item;
		self->item = NULL;
		return item;
	}
	while(self->next != NULL){
		self = self->next;
	}
	void* item = self->item;
	free(self);
	return item;
}

int yablListPop(YablList self, void(*deleteFunc)(void*)){
	void* var = yablListPopRtn(self);
	if(var == NULL) return 0;
	deleteFunc(var);
	return 1;
}

void* yablListRemoveRtn(YablList* self, int index){
	if((*self)->next == NULL) {
		void* item = (*self)->item;
		(*self)->item = NULL;
		return item;
	}
	for(int i = 0; i < index; i++){
		self = &(*self)->next;
	}
	void* item = (*self)->item;
	YablList tmp = (*self);
	*self = tmp->next;
	free(tmp);
	return item;
}

int yablListRemove(YablList* self, int index, void(*deleteFunc)(void*)){
	void* var = yablListRemoveRtn(self, index);
	if(var == NULL) return 0;
	deleteFunc(var);
	return 1;
}

int yablListInsert(YablList* self, int index, void* item){
	YablList new = yablListCreate();
	new->item = item;
	if(index == 0){
		new->next = (*self);
		(*self) = new;
		return 1;
	}
	for(int i = 1; i < index; i++){
		self = &(*self)->next;
		if( *self == NULL ){
			free(new);
			return 0;
		}
	}
	new->next = (*self)->next;
	(*self)->next = new;
	return 1;
}

int yablListInsertCpy(YablList* self, int index, void* item, int size_of_item){
	char* cpy = malloc(size_of_item);
	for(int i = 0; i < size_of_item; i++)
		cpy[i] = ((char*)item)[i];
	int res = yablListInsert(self, index, cpy);
	if(!res) free(cpy);
	return res;
}

void* yablListFind(YablList self, int(*compare)(void*)){
	while(self->next != NULL){
		if(compare(self->item)){
			return self->item;
		}
	}
	return NULL;
}

int yablListLen(YablList self){
	if(self->item == NULL && self->next == NULL)
		return 0;
	int len = 1;
	for(; self->next != NULL; len++)
		self = self->next;
	return len;
}

void yablListForeach(YablList self, void(*foreach)(void *, va_list), int n_args, ...){
	if(self->item == NULL) return;
	va_list ap;
	for(; self != NULL; self = self->next){
		va_start(ap, n_args);
		foreach(self->item, ap);
	}
	va_end(ap);
}

void yablListSimpleForeach(YablList self, void(*foreach)(void*)){
	if(self->item == NULL) return;
	for(; self != NULL; self = self->next){
		foreach(self->item);
	}
}

/// Tests
int yablListCreateTest(){
	YablList list = yablListCreate();
	int result = list->next == NULL;
	yablListDelete(list, &free);
	return result;
}

int yablListPushPopTest(){
	int val = 3;
	YablList list = yablListCreate();
	yablListPushCpy(list, &val, sizeof(int));

	int* result = yablListPopRtn(list);
	yablListDelete(list, &free);

	int rtn = *result == val;
	free(result);
	return rtn;
}

int yablListGetTest(){
	int val = 3;
	YablList list = yablListCreate();
	yablListPushCpy(list, &val, sizeof(int));
	int result = *(int*)yablListGet(list, 0);
	yablListDelete(list, &free);
	return result == val;
}

int yablListRemoveTest(){
	int val = 3;
	YablList list = yablListCreate();
	yablListPushCpy(list, &val, sizeof(int));
	int len = yablListLen(list) == 1;
	yablListRemove(&list, 0, &free);
	int result = yablListLen(list) == 0 && len;
	yablListDelete(list, &free);
	return result;
}

int yablListInsertTest(int pos){
	int val1 = 3;
	int val2 = 3;
	int val3 = 4;
	YablList list = yablListCreate();
	yablListPushCpy(list, &val1, sizeof(int));
	yablListPushCpy(list, &val2, sizeof(int));
	yablListInsertCpy(&list, pos, &val3, sizeof(int));
	int rtn = *(int*)yablListGet(list, pos);
	yablListDelete(list, &free);
	return val3 == rtn;
}
int yablListLenTest(){
	int v = 3;
	int result = 1;
	YablList list = yablListCreate();
	result = yablListLen(list) == 0 && result;
	yablListPushCpy(list, &v, sizeof(v));
	yablListPushCpy(list, &v, sizeof(v));
	yablListPushCpy(list, &v, sizeof(v));
	result = yablListLen(list) == 3 && result;
	yablListPushCpy(list, &v, sizeof(v));
	yablListPushCpy(list, &v, sizeof(v));
	result = yablListLen(list) == 5 && result;
	return result;
}

void _yablListForeachTestFn(void* item, va_list ap ){
	(*va_arg(ap, int*))++;
}

int yablListForeachTest(){
	int count = 0;
	int v = 3;
	YablList list = yablListCreate();
	yablListPushCpy(list, &v, sizeof(v));
	yablListPushCpy(list, &v, sizeof(v));
	yablListPushCpy(list, &v, sizeof(v));
	yablListForeach(list, &_yablListForeachTestFn, 1, &count);
	yablListDelete(list, &free);
	return count == 3;
}

void yablListTests(){
	testHeader("YablList");
	doTest("List Create", yablListCreateTest());
	doTest("List Push Pop", yablListPushPopTest());
	doTest("List Get", yablListGetTest());
	doTest("List Remove", yablListRemoveTest());
	doTest("List Insert pos=1", yablListInsertTest(1));
	doTest("List Insert pos=0", yablListInsertTest(0));
	doTest("List Len", yablListLenTest());
	doTest("List Foreach", yablListForeachTest());
}


