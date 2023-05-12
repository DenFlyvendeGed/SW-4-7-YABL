#ifndef CODE_LIST
#define CODE_LIST

#include <stdlib.h>
//#
typedef struct YablListNode{
	struct YablListNode* next;	
	void* item;
} YablListNode;

typedef YablListNode* YablList;

#define YABL_LIST_FOREACH(TYPE, LIST, CODE){\
	YablList _l = LIST;\
	if(_l->item != NULL) for(; _l != NULL; _l = _l->next){\
		TYPE foreach_value = _l->item;\
		CODE\
	}\
}

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
	YablList s = *self;
	if(s->item == NULL && s->next == NULL){
		if( index > 0 ) return 0;
		s->item = item;
		return 1;
	}
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

//#
#endif

