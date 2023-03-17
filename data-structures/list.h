#ifndef YABL_LIST
#define YABL_LIST

typedef struct YablListNode{
	struct YablListNode* next;	
	void* item;
} YablListNode;

typedef YablListNode* YablList;

YablList yabl_list_create();
void  yabl_list_push(YablList self, void* item);
void  yabl_list_push_cpy(YablList self, void* item, int size_of_item);
void* yabl_list_get(YablList self, int index);
void  yabl_list_delete(YablList self);
void  yabl_list_foreach(YablList self, void(*foreach)(void *));


void yabl_list_tests();

#endif


