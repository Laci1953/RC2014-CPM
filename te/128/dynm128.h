void	InitDynM(void);

void*	Alloc128(short size, char* type);
void	Free128(void* buf, char type);
int	GetTotalFree(void);

/*	source is in Upper RAM, destination is in Lower RAM */
void	GetString(char* dest, char* source);

/*	source is in Lower RAM, destination is in Upper RAM */
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
