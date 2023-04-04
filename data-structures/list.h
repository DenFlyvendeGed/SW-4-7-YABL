#ifndef YABL_LIST
#define YABL_LIST

#include <stdarg.h>

typedef struct YablListNode{
	struct YablListNode* next;	
	void* item;
} YablListNode;

typedef YablListNode* YablList;

YablList yablListCreate();
void  yablListPush(YablList self, void* item);
void  yablListPushCpy(YablList self, void* item, int sizeOfItem);
void* yablListGet(YablList self, int index);
void  yablListDelete(YablList self, void(*deleteItem)(void *));
void  yablListForeach(YablList self, void(*foreach)(void *, va_list), int n_args, ...);
int   yablListRemove(YablList* self, int index, void(*deleteFunc)(void*));
void* yablListRemoveRtn(YablList* self, int index);
void* yablListPopRtn(YablList self);
int   yablListPop(YablList self, void(*deleteFunc)(void*));
int   yablListInsert(YablList* self, int index, void* item);
int   yablListInsertCpy(YablList* self, int index, void* item, int sizeOfItem);
int	  yablListLen(YablList self);

void yablListTests();

#endif


