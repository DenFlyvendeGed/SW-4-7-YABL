#ifndef CODE_GC
#define CODE_GC

#include "list.c"
#include "string.c"
#include <stdlib.h>
#include "globals.c"

//#
typedef YablList GarbageCollector;
void _gcFluch(GarbageCollector gc){
	if(gc == NULL) return;
	_gcFluch(gc->next);
	__DESTROY_STRING__(gc->item);
	free(gc);
}

void gcFlush(GarbageCollector gc){
	if(gc->item == NULL) return;
	_gcFluch(gc->next);
	__DESTROY_STRING__(gc->item);
}
//#
#endif





