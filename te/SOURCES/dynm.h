void 	InitDynM(void);

void*	Alloc(short size, char* flag);
void	Free(void* buf, char flag);
unsigned int	GetTotalFree(void);

/*	destination is in Lower RAM */
void	GetString(char* dest, char* source, char source_flag);

/*	source is in Lower RAM */
void	PutString(char* source, char* dest, char dest_flag);

short	StringLen(char* s, char s_flag);

/*	get/put vector[index] */
char	GetByte(char* vector, short index, char flag);
void	PutByte(char* vector, short index, char byte, char flag);
char*	GetWord(char* vector, short index, char flag);
void	PutWord(char* vector, short index, short word, char flag);
