typedef struct YablHashNode {
	char* key;
	void* item;
} YablHashNode;

typedef struct YablHash {
	int size_of_items;
	void** map;
	int(*hashfunc)(char*);
	YablHashNode * first;
} YablHash;

YablHash yabl_hash_create(int size_of_items);

void* yabl_hash_get(YablHash* self, char* key);

/// Creates a copy of value and puts it in the hashmap
void* yabl_hash_push_cpy(YablHash* self, char* key, void* value);

/// Puts the pointer in the hashmap
void* yabl_hash_push(YablHash* self, char* key, void* value);

/// Deallocates the hashmap
void yabl_hash_delete(YablHash* self);

/// Loops through all components in the map
void yabl_hash_foreach(char* key, void (*foreach)(void *));

#endif // !YABL_HASHTABLE


