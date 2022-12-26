void	InitDynM(void);

void*	myalloc(void);
void	myfree(void*);

/*	source is in Upper RAM, destination is in Lower RAM */
void	ReadMem(char* dest, char* source, short count);

/*	source is in Lower RAM, destination is in Upper RAM */
void	WriteMem(char* source, char* dest, short count);

/*	dest is in Upper RAM */
void	PutByte(char* dest, char b);

/*	source is in Upper RAM */
char	GetByte(char* source);

/*	dest is in Upper RAM */
void	PutWord(char* dest, short w);

/*	source is in Upper RAM */
short	GetWord(char* source);

bool	IsValid(char* addr);

short	TotalSize(void);

