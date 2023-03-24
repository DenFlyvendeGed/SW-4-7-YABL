// ændr naming conventions inden push

typedef struct YablHashNode {
    char* key;
    void* item;
} YablHashNode;

typedef struct YablHash {
    int sizeOfList;
    void*** map; // idea is that this is the array itself
    int(*hashFunc)(char*);
    YablHashNode * first;
} YablHash;

YablHash yablHashCreate(int size_of_items);

void* yablHashGet(YablHash* self, char* key);

/// Creates a copy of value and puts it in the hashmap
void* yablHashPushCpy(YablHash* self, char* key, void* value);

/// Puts the pointer in the hashmap
void* yablHashPush(YablHash* self, char* key, char* key2, void* value);

/// Deallocates the hashmap
void yablHashDelete(YablHash* self);

/// Loops through all components in the map
void yablHashForeach(char* key, void (*foreach)(void *));

//#endif // !YABL_HASHTABLE



