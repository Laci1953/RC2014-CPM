#ifndef _CGEN_H
#define _CGEN_H 1
/* File cgen.h Created 17.05.2019 Last Modified 24.12.2021 */

/* Not a commercial goal of this laborious work is to popularize among
 * potential fans of 8-bit computers the old HI-TECH C compiler V3.09
 * (HI-TECH Software) and extend its life, outside of the CP/M environment
 * (Digital Research, Inc), for full operation in a  Unix-like operating
 * system UZI-180 without using the CP/M emulator.
 *
 * The HI-TECH C compiler V3.09 is provided free of charge for any use,
 * private or commercial, strictly as-is. No warranty or product support
 * is offered or implied including merchantability, fitness for a particular
 * purpose, or non-infringement. In no event will HI-TECH Software or its
 * corporate affiliates be liable for any direct or indirect damages.
 *
 * You may use this software for whatever you like, providing you acknowledge
 * that the copyright to this software remains with HI-TECH Software and its
 * corporate affiliates.
 *
 * All copyrights to the algorithms used, binary code, trademarks, etc.
 * belong to the legal owner - Microchip Technology Inc. and its subsidiaries.
 * Commercial use and distribution of recreated source codes without permission
 * from the copyright holderis strictly prohibited.
 *
 * The solution to this problem is to recreate the object code being moved,
 * replace the CP/M system functions (I/O, memory allocation, etc.) with
 * similar UZI-180 calls, and compile an executable file for this operating
 * system.
 *
 *	Mark Ogden & Andrey Nikitin   24.12.2021
 */

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__STDC__) || defined(__STDC_VERSION__)
#include <stdbool.h>
#include <stdint.h>
#if __STDC_VERSION__ < 201112L
#define _Noreturn
#endif
#if __STDC_VERSION__ >= 201710L
#define register
#endif
/* in certain functions the VC compiler complains reasonably that
 *  certain variables may be used before being initialised
 * this macro forces a 0 initialisation which will cause
 * exceptions if the variable is used, rather than rely on
 * random data
 */
#ifdef _MSC_VER
#define FORCEINIT = NULL
#else
#define FORCEINIT
#endif
#else
typedef unsigned short uint16_t;
typedef short          int16_t;
typedef unsigned char  uint8_t;
typedef char           int8_t;
typedef unsigned long  uint32_t;
#ifndef bool
#define bool           char
#define true  1
#define false 0
#endif
#define FORCEINIT
#define _Noreturn
#endif

#undef max /* replaced by function call */
#ifndef CPM
#define bmove(src, dst, cnt) memcpy(dst, src, cnt)
#endif
/*
 *	Constant declarations
 */
#define MININT  -32768 /* min for int (0x8000)	         */
#define MAXINT  32767  /* max for int (0x7fff)	         */
#define MAXFUN  0x20   /* Maximum limit nested function  */
#define MAXBUF  60     /* Maximum buffer size 		 */
#define MAXNAME 40     /* Maximum file name length	 */
#define MAXERR  30     /* Maximum number nonfatal errors */

#define NULSTR   0    /*  0  ""	*/
#define NOT      1    /*  1  "!"	*/
#define NEQL     2    /*  2  "!="	*/
#define HASHSIGN 3    /*  3  "#"	*/
#define DOLLAR   4    /*  4  "$"	*/
#define DOLLAR_U 5    /*  5  "$U"	*/
#define MOD      6    /*  6  "%"	*/
#define BAND     7    /*  7  "&"	*/
#define LAND     8    /*  8  "&&"	*/
#define GADDR    9    /*  9  "&U"	*/
#define LPAREN   0xA  /* 10  "("	*/
#define RPAREN   0xB  /* 11  ")"	*/
#define MUL      0xC  /* 12  "*"	*/
#define MUL_U    0xD  /* 13  "*U"	*/
#define ADD      0xE  /* 14  "+"	*/
#define INCR     0xF  /* 15  "++"	*/
#define PLUS_U   0x10 /* 16  "+U"	*/
#define COMMA    0x11 /* 17  ","	*/
#define SUB      0x12 /* 18  "-"	*/
#define DECR     0x13 /* 19  "--"	*/
#define CONV     0x14 /* 20  "->"	*/
#define MINUS_U  0x15 /* 21  "-U"	*/
#define DOT      0x16 /* 22  "."	*/
#define MEMBER   0x16 /* Alias for use in class */
#define DOT_DOT  0x17 /* 23  ".."	*/
#define DIV      0x18 /* 24  "/"	*/
#define COLON    0x19 /* 25  ":"	*/
#define COLON_U  0x1A /* 26  ":U"	*/
#define COLON_S  0x1B /* 27  ":s"	*/
#define SCOLON   0x1C /* 28  ";"	*/
#define T_SCOLON 0x1D /* 29  ";;"	*/
#define LT       0x1E /* 30  "<"	*/
#define LSHIFT   0x1F /* 31  "<<"	*/
#define LEQ      0x20 /* 32  "<="	*/
#define ASSIGN   0x21 /* 33  "="	*/
#define ASMOD    0x22 /* 34  "=%"	*/
#define ASAND    0x23 /* 35  "=&"	*/
#define ASMUL    0x24 /* 36  "=*"	*/
#define ASADD    0x25 /* 37  "=+"	*/
#define ASSUB    0x26 /* 38  "=-"	*/
#define ASDIV    0x27 /* 39  "=/"	*/
#define ASLSHIFT 0x28 /* 40  "=<<"	*/
#define EQL      0x29 /* 41  "=="	*/
#define ASRSHIFT 0x2A /* 42  "=>>"	*/
#define ASEXOR   0x2B /* 43  "=^"	*/
#define ASEOR    0x2C /* 44  "=|"	*/
#define GT       0x2D /* 45  ">"	*/
#define GEQ      0x2E /* 46  ">="	*/
#define RSHIFT   0x2F /* 47  ">>"	*/
#define QUEST    0x30 /* 48  "?"	*/
#define ATGIGN   0x31 /* 49  "@"	*/
#define CASE     0x32 /* 50  "[\\"	*/
#define UNKNOWN  0x33 /* 51  "[a"	*/
#define ENUM     0x34 /* 52  "[c"	*/
#define EXPR     0x35 /* 53  "[e"	*/
#define INIT     0x36 /* 54  "[i"	*/
#define STRUCT   0x37 /* 55  "[s"	*/
#define UNION    0x38 /* 56  "[u"	*/
#define VAR      0x39 /* 57  "[v"	*/
#define BXOR     0x3A /* 58  "^"	*/
#define LBRACE   0x3B /* 59  "{"	*/
#define BOR      0x3C /* 60  "|"	*/
#define LOR      0x3D /* 61  "||"	*/
#define RBRACE   0x3E /* 62  "}"	*/
#define BNOT     0x3F /* 63  "~"	*/
#define RECIP    0x40 /* 64  "RECIP"    */
#define TYPE     0x41 /* 65  "TYPE"     */
#define IDOP     0x42 /* 66  "ID"	*/
#define CONST    0x43 /* 67  "CONST"    */
#define FCONST   0x44 /* 68  "FCONST"   */
#define USEREG   0x45 /* 69  "REG"      */
#define INAREG   0x46 /* 70  "INAREG"   */
#define BFIELD   0x47 /* 71  "BITFIELD" */


/* register indexes */
enum { /* REG_A=1, REG_C, REG_B, REG_E, REG_D, REG_L */
       REG_H = 7,
       /* REG_IX, REG_IY, REG_SP */
       REG_AF = 11,
       /* REG_BC, REG_DE, REG_HL */
       REG_DEHL = 15
};

/* enums */
enum psect { P_BSS = 1, P_TEXT, P_DATA };

/*
 *	Structural declarations
 */

typedef struct node {
    uint8_t  a_c0;
    uint8_t  a_c1;
    uint8_t  a_c2; 	/* two bits used */
    uint8_t  a_c3[6];
    uint8_t  a_uc9[6];
    uint8_t  a_reg[6];
    uint16_t a_i15;
    struct member *pm; /* pointer to member */
    union {
        long           l;	/* long */
        unsigned long  ul;	/* unsigned long, to avoid cast*/
        struct node   *np[2];	/* node pair */
        struct member *mp[2];	/* member pair */
        struct {		/* string + int value */
            char *s;
            int   v;
        } sv;
    } info;
} node_t;

typedef struct member {     /* Offset Member   Description		   */
    char *b_name;           /* +0     0 s_name Pointer to Name identifier  */
                            /* +1     1					   */
    char  b_class;          /* +2     2 s_class 0x16 - MEMBER 		   */
                            /*			      '4' -  ENUM	   */
                            /*			      '7' -  STRUCT 	   */
                            /*			      '8' -  UNION	   */
                            /*			      '9' - 		   */
                            /*			      'A' -  TYPE	   */
    char  b_sloc;           /* +3     3 s_strg  Bits used: 0 0000001 -	   */
                            /*		            1 0000010 -	global?	   */
                            /*		            2 0000100 -		   */
                            /*		            3 0001000 -	register   */
                            /*		            4 0010000 -		   */
    uint8_t        b_ndpth; /* +4     4 s_ndpth  Nesting depth of block	   */
    uint8_t        b_b5;    /* +5     5					   */
    uint8_t        b_b6;    /* +6     6					   */
    struct member *b_next;  /* +7     7					   */
                            /* +8     8					   */
    uint16_t b_size;        /* +9     9 s_size  Type size		   */
                            /* +A    10					   */
    uint16_t b_nelem;       /* +B    11 s_nelem				   */
                            /* +C    12					   */
    uint16_t b_refl;        /* +D    13 s_refl				   */
                            /* +E    14					   */
    struct member *b_type;  /* +F    15 s_type				   */
                            /* +10   16					   */
    int b_off;              /* +11   17 s_alig Type alig  size stack frame */
                            /* +12   18					   */
    union {                 /* +13   19  - +14 19			   */
        int16_t i;
        struct _memb {
            int16_t cnt;
            struct member *vals[1];
        } * mlist;
        struct _memi {
            int16_t cnt;
            int16_t vals[1];
        } * ilist;
    } b_memb;
    char b_flag; /* +15 21	 type flag 0, 1, 2, 3, 4, 5,	*/
} member_t;

struct codeFrag_t {
    char    c_0;
    char    c_1;
    char    c_2;
    char    c_3;
    uint8_t c_4;
    uint8_t c_5;
    char   *p_6;
    char   *p_8;
};

/*
 *	Descriptions of variables and arrays
 *
 * Declarations are located in sequence of being in
 * original binary image of CGEN.COM
 *					Purpose				Where it is used
 * ===== Start bss section ======= */
extern int       lineno;             	/* getToken, sub_6AD0, prMsg*/
extern char      progname[MAXNAME]; 	/* getToken  */
extern member_t *typeLong;     		/* sub_1680, sub_415E, sub_43EF */
extern int       array_AE13[MAXFUN]; 	/* leaveBlock,  sub_17E0, sub_5CF5 */
extern member_t *typeUChar;    		/* "uc" - uint8_t	   */
extern uint16_t  nstdpth;       	/* Current nesting depth   */
extern int       array_AE57[MAXFUN];
extern int       array_AE97[MAXFUN];
extern member_t *typeDouble;		/* "d" - double 	   */
extern member_t *typeChar;		/* "c" - char 		   */
extern member_t *array_AEDB[MAXFUN];	/*			   */
extern member_t *typeB;			/* "b"			   */
extern member_t *typeX;			/* "x"			   */
#define HASHTABSIZE 101
extern member_t *hashtab[HASHTABSIZE]; /* hash table (array_AF1F) */
extern member_t *typeVar;              /* "v" - variable  	   */
extern int       lvlidx;                     /* Level nested function - leaveBlock, sub_1680 */

extern int       word_AFF8;            /* sub_2D09, sub_3564 */
extern char      byte_AFFA;            /* sub_283E, sub_2B79 */
extern int       word_AFFB;            /* sub_2D09, sub_35E6 */
extern node_t   *array_AFFD[0x14];     /* sub_3CDF, sub_3DC9 */
extern void     *nodeFreeList;         /* sub_36E0, allocNode, releaseNodeFreeList */
extern bool      byte_B013;            /* sub_36E0, allocNode, ... */
extern uint8_t   byte_B014;            /* sub_3CDF, sub_3DC9 */
extern char     *warningMsg;           /* msgptr  Pointer str printf       */
extern int       word_B017;            /* leaveBlock, sub_6D1, sub_283E, sub_2BD0, sub_61AA */
extern bool      nflag;		       /* statistics */
extern bool      wflag;                /* Warning messages */
extern bool      pflag;                /* Not used */
extern bool      hflag;                /* Not used */
extern int       errcnt;               /* Number of errors */
extern bool      bflag;                /* Not used */
extern char     *baseHeap;             /* Current highest memory */
extern bool      eflag;                /* Not used */
extern bool      rflag;

extern char	tmpBuf[MAXBUF];		/* temporary buffer for strings handling */

/* ===== End of bss section ======= */

/* ===== Start data section ======= */

extern char     *otherFrag[];		/*+sub_2D09 */
extern struct codeFrag_t codeFrag[]; 	/* sub_1F4B, sub_283E, sub_2D09, sub_3564 */

extern uint16_t  dopetab[72]; 	/* +sub_1F4B, sub_2B8A, sub_2D09, sub_35E6, sub_377A, ... */
extern char      array_A94A[];	/* +sub_1F4B */
extern int       array_AAE8[];	/* +leaveBlock,  sub_1F4B, sub_283E, sub_61AA, ... */
extern uint8_t   array_AB24[];	/* +sub_283E, sub_2D09, sub_665B */
extern uint8_t   array_AB54[];	/* +sub_63B8, sub_6589 */

extern char     *regNames[];

/* ===== End of data section ======= */

/*
 * Libbrary Function prototypes
 *
 */
#ifdef CPM
long atol(char *str);
void blkclr(char *, uint16_t size);
void *sbrk(int);
#else
#define blkclr(buf, len) memset(buf, 0, len)
#endif

/****************************************************************
 * Prototype functions are defined in sequence of being in
 * original binary image of CGEN.COM
 ****************************************************************/
#ifndef _WIN32
int fgetchar(void);
int fputchar(int);
#else
#define fgetchar getchar
#define fputchar putchar
#endif

/* for some functions the list of declarations below causes the compilation
 * to fail. When this happens the individual function defines MINI and declares
 * its used functions only
 */
#ifndef MINI
/* lex.c -----------------------------------------------------*/
int        sub_13D(int, int);
char       sub_1B2(char *);
member_t **gethashptr(char *);
member_t  *sub_265(char *);
char      *getToken();
void       leaveBlock();
member_t  *sub_627(char *, uint16_t *);
void       badIntCode();
void       parseStmt();
void       expect(char);
void       parseData();
/* code.c ----------------------------------------------------*/
int        sub_808(member_t *, node_t *);
void       parseInit();
void       prFrameHead(int);
void       prFrameTail(int, int);
void       sub_BEE(int, int);
void       prGlobalDef(member_t *);
void       sub_C57(member_t *);
void       sub_CAB(member_t *);
void       prDefb0s(int);
void       prPsect(int);
void       sortCaseLabels(int *pCase, int *pLabel, int nCase);
void       parseSwitch();
void       sub_1420(int);
void       prPush(uint8_t);
void       prPop(uint8_t);
void       prIXnPush(member_t *);
uint8_t    sub_14F3(node_t *);
void       sub_153A(node_t *);
void       prDefb(char *, int);
void       prJmpLabel(int);
void       prJump(int);
void       sub_15FB(member_t *, int, int);
int        max(int, int);
void       sub_1680();
int        newLocal();
member_t  *sub_1754(char *, uint8_t);
/* sym.c -----------------------------------------------------*/
void       parseVariable();
void       parseMembers(int);
void       parseEnum();
int        sub_1C6D(int, int);
int        varSize(member_t *);
int        sub_1CC4(member_t *);
void       sub_1CEF(member_t *);
void       sub_1EDF();
int        sub_1F4B(node_t *, int, int, int, int *);
int        sub_283E(node_t *, int);
int        sub_2B2A(node_t *);
uint8_t    sub_2B79(node_t *);
/* cgen.c ----------------------------------------------------*/
void       sub_2BD0(node_t *);
long       sub_2C5E(node_t *, long);
void       sub_2CE0(node_t *, long);
void       sub_2D09(node_t *, char *, char);
void       sub_3564(node_t *);
void       sub_35E6(node_t *);
void       freeNode(node_t *);
node_t    *allocNode(void);
bool       releaseNodeFreeList(void);
node_t    *sub_377A(node_t *);
void       sub_37ED(node_t *);
void       sub_385B(node_t *);
uint16_t   sub_387A(node_t *);
bool       sub_38CA(node_t *, int);
bool       sub_393C(node_t *);
bool       sub_3952(node_t *);
bool       sub_3968(node_t *);
uint16_t   nodesize(node_t *); /*sub_3993*/
node_t    *sub_39CA(node_t *);
void       sub_3A79(node_t *, node_t *);
void       sub_3B65(unsigned long *, long, int);
void       sub_3BC6(long *, long, int);
node_t    *sub_3CDF(node_t *);
/* tree.c ----------------------------------------------------*/
void       sub_3DC9(node_t *);
node_t    *sub_3EAA(node_t *);
node_t    *sub_415E(long);
node_t    *parseExpr(void);
node_t    *sub_43EF(uint8_t, node_t *, node_t *);
uint8_t    sub_46F7(long);
void       sub_475C(node_t *);
uint8_t    sub_47B2(node_t *, int);
bool       sub_4B89(node_t *);
bool       sub_4BE5(node_t *);
int        sub_4C12(node_t *);
bool       sub_4C6D(node_t *);
node_t    *sub_4C8B(node_t *);
node_t    *sub_4DA3(node_t *);
node_t    *sub_4E8D(node_t *);
node_t    *sub_4FA8(node_t *);
node_t    *sub_4FCE(node_t *);
node_t    *sub_508A(node_t *);
node_t    *sub_53EE(node_t *);
/* local.c ---------------------------------------------------*/
node_t    *sub_54B6(node_t *);
void       sub_5CF5(member_t *, int);
node_t    *sub_5DF6(node_t *);
node_t    *sub_5F52(node_t *);
node_t    *sub_600E(node_t *);
int        invertTest(int);
bool       sub_60A8(node_t *, node_t *);
bool       sub_61AA(member_t *, int);
bool       sub_6246(node_t *, node_t *);
bool       sub_628F(node_t *, node_t *);
bool       sub_62BE(node_t *, node_t *);
uint8_t    sub_63B8(int, int, int);
uint8_t    sub_6589(int, int);
uint16_t   sub_665B(uint16_t, uint8_t);
int        sub_66BC(int, int, int, char *);
/* main.c  ---------------------------------------------------*/
/* int main(int, char **);	*/

/*
 * original C functions replaced to use stdarg
 */
#ifdef CPM
#define vfprintf _doprnt
#endif

void fatalErr();
void prWarning();
void prError();
void prMsg();

void *allocMem(size_t);			 /* ok sub_6B9B.c */

/* end of function declarations */
#endif

char* MyAllocStr(size_t);
char* MyAllocMbr(void);

/* End file cgen.h */
#endif
