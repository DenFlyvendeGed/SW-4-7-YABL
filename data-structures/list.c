#include "list.h"
#include <stdlib.h>

YablList yabl_list_create(){
	YablList list = malloc(sizeof(YablListNode));
	list->next = NULL;
	list->item = NULL;
	return list;
}

void yabl_list_delete(YablList self, void(*delete_var)(void*)){
	if(self->next == NULL) return;
	yabl_list_delete(self->next);
	if(self->item != NULL) delete_var(self->item);
	free(self);
}

void yabl_list_push(YablList self, void *item){
	if(self->item == NULL){
		self->item = item;
	} else {
		while(self->next != NULL) self = self->next;
		self->next = yabl_list_create();
		self->next->item = item;
	}
}

void yabl_list_push_cpy(YablList self, void *item, int size_of_item){
	char * cpy = malloc(size_of_item);
	for(int i = 0; i < size_of_item; i++)
		cpy[i] = ((char*)item)[i];
	yabl_list_push(self, cpy);
}

void* yabl_list_get(YablList self, int index){
	for(int i = 0; i < index; i++)
		self = self->next;
	return self->item;
}


/// Tests
#include "../test.h"

int yabl_list_create_test(){
	YablList list = yabl_list_create();	
	int result = list->next == NULL;
	void* item = yabl_list_get(list, 6);
	switch((ExprType)item){
		case func:
	}
	yabl_list_delete(list, &free);
	return result;
}


void yabl_list_tests(){
	TEST("CREATE LIST", yabl_list_create_test());
}


