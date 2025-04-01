void 	InitDynM(void);

void*	Alloc(short size);
void	Free(void* buf);
unsigned int	GetTotalFree(void);

/*	destination is in Lower RAM */
void	GetString(char* dest, char* source);

/*	source is in Lower RAM */
void	PutString(char* source, char* dest);

short	StringLen(char* s);
