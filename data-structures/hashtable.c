#include "hashtable.h"

typedef struct Node {
	char* key;
	void* item;
} Node;

YablHash yabl_hash_create(int size_of_items) {
	YablHash rtn = {size_of_items};
	return rtn;
}

void yabl_hash_delete(YablHash *self) {
	
}

void yabl_hash_foreach(char* key, void (*foreach)(void *)){

}





