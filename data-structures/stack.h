#ifndef YABL_STACK
#define YABL_STACK

typedef struct YablStackNode {
	struct YablStackNode* next;
	void* item;
} YablStackNode;

typedef YablStackNode* YablStack;


#define YABL_STACK_CREATE NULL
void yablStackPush(YablStack* self, void* item);
void* yablStackPop(YablStack* self);

#endif // !YABL_STACK


