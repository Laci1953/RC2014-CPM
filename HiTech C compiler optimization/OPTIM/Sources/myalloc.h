void	InitDynM(void);

void*	myalloc(void);
void*	myallocstr(int);

/*	source is in Upper RAM, destination is in Lower RAM */
void	ReadMem(char* dest, char* source, short count);
void	GetString(char* dest, char* source);

/*	source is in Lower RAM, destination is in Upper RAM */
void	WriteMem(char* source, char* dest, short count);
void	PutString(char* source, char* dest);

/*	dest is in Upper RAM */
void	PutByte(char* dest, char b);

/*	source is in Upper RAM */
char	GetByte(char* source);

/*	dest is in Upper RAM */
void	PutWord(char* dest, short w);

/*	source is in Upper RAM */
short	GetWord(char* source);

/*	source is in Upper RAM */
short	StringLen(char* source);

bool	IsValid(char* addr);

short	TotalSize(void);

