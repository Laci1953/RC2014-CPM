void	InitDynM(void);

void*	myallocsym(void);
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

// offsets in header

#define OFF_first		0
#define OFF_last		2

// offsets in sym_t structure

#define	OFF_s8_t_labelId	0
#define	OFF_s8_t_nextSym	0
#define	OFF_s8_t_nextInfo	0
#define	OFF_s8_t_pSym		2
#define	OFF_s8_t_pExpr		2
#define	OFF_s8_t_pArgs		2
#define	OFF_s8_t_pInfo		2
#define	OFF_s8_t_a_i4		4	
#define	OFF_s8_t_a_dataType	6	
#define	OFF_s8_t_c7		7
#define	OFF_nMemberList		8	
#define	OFF_nodeId		10	
#define	OFF_m14			12	
#define	OFF_m16			14	
#define	OFF_m18			16	
#define	OFF_m20			18	
#define	OFF_m21			19	
#define	OFF_nRefCnt		20	
#define	OFF_nVName		21	
#define	OFF_next		23
#define OFF_prev		25

// offsets in attr structure

#define OFF_ATTR_u1		0
#define OFF_ATTR_nextSym	0
#define OFF_ATTR_nextInfo	0
#define OFF_ATTR_labelId	0
#define OFF_ATTR_u2		2
#define OFF_ATTR_pSym		2
#define OFF_ATTR_pExpr		2
#define OFF_ATTR_pArgs		2
#define OFF_ATTR_pInfo		2
#define OFF_ATTR_i4		4
#define OFF_ATTR_dataType	6
#define OFF_ATTR_c7		7
