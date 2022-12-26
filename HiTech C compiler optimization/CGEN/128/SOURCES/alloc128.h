void	InitDynM(void);

void*	myallocmbr(void);
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

short	TotalSize(void);

#define	OFF_b_name	0
#define	OFF_b_class	2
#define	OFF_b_sloc	3
#define	OFF_b_ndpth	4
#define	OFF_b_b5	5
#define	OFF_b_b6	6
#define	OFF_b_next	7
#define	OFF_b_size	9
#define	OFF_b_nelem	11
#define	OFF_b_refl	13
#define	OFF_b_type	15
#define	OFF_b_off	17
#define	OFF_b_memb	19
#define	OFF_b_flag	21
