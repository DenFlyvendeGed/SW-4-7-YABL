#include "stack.h"
#include <stdlib.h>

const YablStack yablStackCreate(){ 
	return NULL;
}

void yablStackPush(YablStack* self, void* item){
	YablStack new = malloc(sizeof(YablStackNode));
	new->item = item;
	new->next = *self;
	*self = new;
}

void* yablStackPop(YablStack* self){
	void* item = (*self)->item;
	YablStack next = (*self)->next;
	free(*self);
	*self = next;
	return item;
}

