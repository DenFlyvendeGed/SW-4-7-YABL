// ændr naming conventions inden push

typedef struct YablHashNode {
    void* key;
    void* item;
} YablHashNode;

typedef struct YablHash {
    int sizeOfList;
    void*** map; // idea is that this is the array itself
    int(*hashFunc)(void*);
    YablHashNode * first;
} YablHash;

YablHash yablHashCreate(int sizeOfList, void(*hashFunc)(char *));

void* yablHashGet(YablHash* self, void* key, void* value);

/// Creates a copy of value and puts it in the hashmap
void* yablHashPushCpy(YablHash* self, void* key, void* value,  int size_of_value);

/// Puts the pointer in the hashmap
void* yablHashPush(YablHash* self, void* key, char* key2, void* value);

/// Deallocates the hashmap
void yablHashDelete(YablHash* self);

/// Loops through all components in the map
void yablHashForeach(YablHash* self, void* key, void (*foreach)(void *));

void yablHashTests();

//#endif // !YABL_HASHTABLE



