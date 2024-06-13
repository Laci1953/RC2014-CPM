void init_mem(void);
int total_free(void);
void* sbrk(int size);
void* myalloc(int size);
void myfree(void* p);
void* myrealloc(void* p, int size);
