void 	Init512Banks(void);
char*	alloc512(short Size);
void	free512(char* Buf);
char	IsValid(char* p);

/*	source is in Upper RAM, destination is in Lower RAM */
void	ReadMem(char* dest, char* source, short offset, short count);

/*	source is in Lower RAM, destination is in Upper RAM */
void	WriteMem(char* source, char* dest, short offset, short count);

/*	dest is in Upper RAM */
void	PutByte(char* dest, short offset, char b);

/*	source is in Upper RAM */
char	GetByte(char* source, short offset);

/*	dest is in Upper RAM */
void	PutWord(char* dest, short offset, short w);

/*	source is in Upper RAM */
short	GetWord(char* source, short offset);

/*	source is in Upper RAM */
char*	GetString(char* source, short offset);

/*	dest is in Upper RAM */
void	ZeroFill(char* dest, short size);

