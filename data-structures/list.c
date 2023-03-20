#include "list.h"
#include "../test.h"
#include <stdlib.h>

YablList yablListCreate(){
	YablList list = malloc(sizeof(YablListNode));
	list->next = NULL;
	list->item = NULL;
	return list;
}

void yablListDelete(YablList self, void(*delete_var)(void*)){
	if(self->next == NULL) return;
	yablListDelete(self->next, delete_var);
	if(self->item != NULL) delete_var(self->item);
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
	for(int i = 0; i < index; i++)
		self = self->next;
	return self->item;
}

void* yablListPop(YablList self){
	while(self->next != NULL){
		self = self->next;
	}
	void* item = self->item;
	free(self);
	return item;
}

void* yablListRemove(YablList* self, int index){
	YablList* temp;
	for(int i = 0; i < index; i++){
		temp = self;
		self = &(*self)->next;
	}
	(*temp) = (*self)->next;
	void* item = (*self)->next;
	free(self);
	return item;
}

void yablListInsert(YablList self, int index, void* item){
	for(int i = 0; i < index; i++){
		self = self->next;
	}
	YablList new = yablListCreate();	
	new->item = item;
	new->next = self->next;
	self->next = new;
}

void* yablListFind(YablList self, int(*compare)(void*)){
	while(self->next != NULL){
		if(compare(self->item)){
			return self->item;
		}
	}
	return NULL;
}


/// Tests
int yablListCreateTest(){
	YablList list = yabl_list_create();	
	int result = list->next == NULL;
	yablListDelete(list, &free);
	return result;
}


void yablListTests(){
	testHeader("YablList");
	doTest("CREATE LIST", yablListCreateTest());
}

