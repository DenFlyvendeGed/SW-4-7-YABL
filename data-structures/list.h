#ifndef YABL_LIST
#define YABL_LIST

typedef struct YablListNode{
	struct YablListNode* next;	
	void* item;
} YablListNode;

typedef YablListNode* YablList;

YablList yabl_list_create();
void  yablListPush(YablList self, void* item);
void  yablListPushCpy(YablList self, void* item, int sizeOfItem);
void* yablListGet(YablList self, int index);
void  yablListDelete(YablList self, void(*deleteItem)(void *));
void  yablListForeach(YablList self, void(*foreach)(void *));

void yablListTests();

#endif


