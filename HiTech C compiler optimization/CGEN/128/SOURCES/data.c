/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File cgen.c Created 17.05.2019 Last Modified 24.12.2021
 *
 * Mark Ogden &	Andrey Nikitin   24.12.2021
 */

#include "cgen.h"

/* ===== start bss section ======= */

int lineno;             /* getToken, sub_6AD0, prMsg  */
char progname[MAXNAME]; /* getToken, prWarning, prMsg */

member_t *typeLong; 	/* "l" - long	*/    /* sub_1680, sub_415E, sub_43EF */
int array_AE13[MAXFUN]; /*		*/    /* leaveBlock,  sub_17E0, sub_5CF5 */
member_t *typeUChar;                          /* "uc" - uint8_t	   */
uint16_t nstdpth;                             /* Current nesting depth   */
int array_AE57[MAXFUN];                       /*			   */
int array_AE97[MAXFUN];                       /*			   */
member_t *typeDouble;                         /* "d" - double 	   */
member_t *typeChar;                           /* "c" - char 		   */
member_t *array_AEDB[MAXFUN];                 /*			   */
member_t *typeB;                              /* "b"			   */
member_t *typeX;                              /* "x"			   */
member_t *hashtab[101];                       /* hash table (array_AF1F) */
member_t *typeVar;                            /* "v" - variable  	   */
int lvlidx; /* Level nested function   */     /* leaveBlock, sub_1680 */

int word_AFF8;            /* sub_2D09, sub_3564 */
char byte_AFFA;           /* sub_283E, sub_2B79 */
int word_AFFB;            /* sub_2D09, sub_35E6 */
node_t *array_AFFD[0x14]; /* sub_3CDF, sub_3DC9 */
/* void *nodeFreeList; */       /* sub_36E0, allocNode, releaseNodeFreeList */
char bf1;
bool byte_B013;           /* sub_36E0, allocNode, ... */
char bf2;
uint8_t byte_B014;        /* sub_3CDF, sub_3DC9 */
char *warningMsg;         /* Pointer for printf      */
int word_B017;            /* leaveBlock, sub_6D1, sub_283E, sub_2BD0, sub_61AA */
bool wflag;               /* Warning messages	   */
bool pflag;               /* Not used		   */
bool hflag;               /* Not used		   */

int errcnt;               /* Number of errors	   */
bool bflag;               /* Not used		   */
#ifdef CPM
char *baseHeap;           /* Current highest memory  */
#endif
bool eflag;               /* Not used		   */
bool rflag;               /* Not used		   */
char	tmpBuf[MAXBUF];		/* temporary buffer for strings handling */

/* ===== End of bss section ======= */

/* these tables were buit using a perl script so that string tables did
 * not cause out of memory
 */
extern char stab0[], stab1[], stab2[], stab3[], stab4[];
/* clang-format off */
char *otherFrag[] = {
0,           0,           stab4+0x1BB, 0,           0,           0,           stab4+0x18D, stab4+0x189, 
0,           0,           0,           0,           stab4+0x185, 0,           stab4+0x179, stab4+0x175, 
0,           0,           stab4+0x171, stab4+0x1A9, 0,           stab3+0x101, 0,           0,           
stab4+0x1A5, 0,           0,           0,           0,           0,           stab4+0x1C7, stab0+0x2F5, 
0,           0,           stab4+0x18D, stab4+0x189, stab4+0x185, stab4+0x179, stab4+0x171, stab4+0x1A5, 
stab0+0x2F5, stab4+0x1BC, stab0+0x22E, stab4+0x1A1, stab4+0x1A2, 0,           stab4+0x1C4, stab0+0x22E, 
0,           0,           0,           0,           0,           0,           0,           0,           
0,           0,           stab4+0x1A1, 0,           stab4+0x1A2, 0,           0,           stab4+0x1C1, 
0,           0,           0,           0,           0,           0,           0,           0,           
};


struct codeFrag_t codeFrag[] = {
    {0,    0,    0,    0,    0,    0,    0,           0},           /* 0 */
    {0,    0,    0,    0x1C, 'H',  'H',  0,           stab0+0x261}, /* 1 */
    {1,    0,    0,    0x2C, 'I',  'J',  stab3+0x28E, stab0+0x58},  /* 2 */
    {2,    0,    0,    0x23, 'I',  'J',  stab3+0x2B5, stab0+0x58},  /* 3 */
    {3,    0,    0,    0x0F, 'I',  'K',  stab4+0x21,  stab0+0x58},  /* 4 */
    {3,    0,    0,    0x0F, 'I',  'L',  stab4+0x35,  stab0+0x58},  /* 5 */
    {3,    0,    0,    0x13, 'I',  'K',  stab4+0x3F,  stab0+0x58},  /* 6 */
    {0,    0,    0x12, 0x0A, 'M',  'N',  stab1+0x218, stab0+0x58},  /* 7 */
    {0,    0,    0,    5,    'O',  0,    stab4+0x15D, stab0+0x261}, /* 8 */
    {0,    0,    0,    4,    'P',  'O',  stab4+0x12F, stab0+0x58},  /* 9 */
    {0,    0,    0,    4,    'Q',  'O',  stab3+0x24D, stab0+0x58},  /* 10 */
    {4,    0,    0,    0x0B, 'R',  0,    0,           stab0+0x58},  /* 11 */
    {5,    0,    0,    0x0B, 'S',  0,    0,           stab0+0x58},  /* 12 */
    {6,    0,    0,    0x0B, 'T',  0,    0,           stab0+0x58},  /* 13 */
    {3,    6,    0,    0x0B, 'U',  0,    0,           stab0+0x58},  /* 14 */
    {3,    0,    6,    0x0B, 'V',  0,    stab4+0x95,  stab0+0x58},  /* 15 */
    {0,    0,    0,    0x1A, 'O',  0,    stab4+0x1B8, stab0+0x58},  /* 16 */
    {3,    0,    0,    0x21, 'I',  'J',  stab4+0xA7,  stab0+0x58},  /* 17 */
    {3,    0,    0,    0x46, 'V',  0,    0,           stab0+0x58},  /* 18 */
    {3,    0,    0,    0x31, 'J',  0,    stab4+0x128, stab0+0x58},  /* 19 */
    {0,    0,    0,    0x57,  0,   0,    0,           stab0+0x58},  /* 20 */
    {3,    0,    0,    0x55,  0,   0,    0,           stab0+0x58},  /* 21 */
    {7,    0,    0,    0x21, 'I',  'J',  stab0+0x107, stab0+0x58},  /* 22 */
    {8,    0,    0,    0x31, 'X',  0,    stab4+0x121, stab0+0x58},  /* 23 */
    {5,    0,    0,    0x31, 'J',  0,    stab3+0x115, stab0+0x58},  /* 24 */
    {5,    0,    0,    0x53, 0,    0,    0,           stab0+0x58},  /* 25 */
    {6,    0,    0,    0x21, 'I',  'J',  stab2+0x3D,  stab0+0x261}, /* 26 */
    {6,    0,    0,    0x31, 'Y',  0,    stab4+0x113, stab0+0x58},  /* 27 */
    {6,    0,    0,    0x5A, 0,    0,    0,           stab0+0x58},  /* 28 */
    {0,    0,    0,    0x5B, 0,    0,    0,           stab0+0x58},  /* 29 */
    {0,    1,    0,    0x55, 0,    0,    0,           stab0+0x261}, /* 30 */
    {0,    0,    1,    0x55, 0,    0,    stab4+0xEF,  stab0+0x2C3}, /* 31 */
    {0,    0,    0,    0x1B, 'J',  0,    0,           stab4+0x14E}, /* 32 */
    {0,    0,    0,    9,    '\\', 0,    0,           stab0+0x261}, /* 33 */
    {0,    0,    0,    0x43, 0,    0,    0,           stab4+0x1BE}, /* 34 */
    {6,    0,    0,    0x14, 'Y',  0,    0,           stab0+0x261}, /* 35 */
    {0,    0,    0,    0x0E, 'Y',  'J',  0,           stab4+0x162}, /* 36 */
    {0,    0,    0,    0x49, 0,    0,    0,           stab0+0x261}, /* 37 */
    {3,    0,    0,    0x55, 0,    0,    0,           stab0+0x261}, /* 38 */
    {3,    0,    0x1A, 0x56, 0,    0,    stab4+0x95,  stab0+0x2C3}, /* 39 */
    {0,    0,    0,    0x5A, 0,    0,    0,           stab4+0xF7},  /* 40 */
    {0,    0,    0,    0x5E, 0,    0,    0,           stab4+0xF7},  /* 41 */
    {5,    0,    0,    0x53, 0,    0,    0,           stab4+0xE7},  /* 42 */
    {3,    6,    0,    0x55, 0,    0,    stab4+0xDF,  stab4+0x1CA}, /* 43 */
    {3,    0,    2,    0x56, 0,    0,    stab3+0x127, stab4+0x1CA}, /* 44 */
    {0,    0,    0,    0x57, 0,    0,    stab4+0x10C, stab4+0x1CA}, /* 45 */
    {0,    0,    0,    0x5F, 0,    0,    0,           stab4+0x1CC}, /* 46 */
    {0,    0,    0,    0x5A, 0,    0,    stab4+0xD7,  stab4+0x1CA}, /* 47 */
    {4,    0,    0x10, 0x60, 0,    0,    stab0+0x1FC, stab4+0x1B5}, /* 48 */
    {0,    0,    0,    0x11, 'N',  'N',  0,           stab4+0x19D}, /* 49 */
    {0,    0,    0,    0x17, 0,    0,    0,           stab3+0x101}, /* 50 */
    {3,    0,    1,    0x62, 0,    0,    stab4+0xCF,  stab4+0xC7},  /* 51 */
    {3,    0,    1,    0x5C, 0,    0,    stab4+0x5D,  stab4+0x53},  /* 52 */
    {6,    0,    0x0E, 0x5C, 0,    0,    stab4+0x5D,  stab4+0x53},  /* 53 */
    {0,    0,    0x0E, 0x57, 0,    0,    stab3+0x215, stab3+0x231}, /* 54 */
    {6,    0,    0x0E, 0x49, 0,    0,    stab3+0x9E,  stab2+0x259}, /* 55 */
    {6,    0,    0x0E, 0x0D, 'd',  0,    stab0+0x2C6, stab0+0x295}, /* 56 */
    {0,    0,    0x51, 0x65, 0,    0,    stab0+0xB1,  stab0+0x1C6}, /* 57 */
    {0,    0,    0x18, 0x57, 0,    0,    stab3+0x215, stab3+0x231}, /* 58 */
    {6,    0,    0x18, 0x49, 0,    0,    stab3+0x9E,  stab3+0xC8},  /* 59 */
    {6,    0,    0x1D, 0x0D, 'T',  0,    stab1+0x29B, stab1+0x2BA}, /* 60 */
    {0,    0,    0,    0x63, 0,    0,    0,           stab0+0x261}, /* 61 */
    {0,    0,    0,    0x47, 'R',  0,    0,           stab2+0x2B6}, /* 62 */
    {1,    0,    0,    7,    ']',  'J',  stab3+0x25A, stab0+0x58},  /* 63 */
    {0,    0,    0,    8,    'h',  'Q',  stab2+0x171, stab0+0x58},  /* 64 */
    {0,    0,    0,    0x51, 0,    0,    stab2+0x11F, stab0+0x261}, /* 65 */
    {0,    0,    0,    1,    'Q',  0,    stab2+0x1BF, stab0+0x58},  /* 66 */
    {0,    0,    0,    0x3D, 'i',  'Q',  stab2+0x171, stab0+0x58},  /* 67 */
    {0,    0,    0,    0x51, 0,    0,    stab3+0x256, stab0+0x261}, /* 68 */
    {0,    0,    0,    0x50, 0,    0,    stab3+0x18C, stab0+0x58},  /* 69 */
    {3,    0,    0,    0x5D, 0,    0,    0,           stab0+0x261}, /* 70 */
    {3,    0,    0x1A, 0x0D, 'k',  0,    stab3+0x1F7, stab0+0x2C3}, /* 71 */
    {3,    0,    0x1A, 0x0D, 'l',  0,    stab3+0x29B, stab0+0x2C3}, /* 72 */
    {3,    0,    0x5A, 0x49, 0,    0,    stab4+0xCF,  stab0+0x2C3}, /* 73 */
    {0,    0,    0,    0x1C, 'H',  'U',  0,           stab3+0x1B8}, /* 74 */
    {0,    0,    0,    0x21, 'I',  'U',  stab4+0xBF,  stab3+0x1B8}, /* 75 */
    {3,    0,    0,    0x30, 'i',  'm',  0,           stab3+0x1B8}, /* 76 */
    {3,    0,    0x1A, 0x14, 'P',  0,    stab2+0x107, stab0+0x2C3}, /* 77 */
    {3,    0,    0x1A, 0x14, 'Q',  0,    stab1+0x1F6, stab0+0x2C3}, /* 78 */
    {3,    0,    0,    0x14, 'U',  0,    0,           stab2+0x2B6}, /* 79 */
    {3,    0,    0x5A, 0x14, 'Z',  0,    stab4+0x2B,  stab0+0x2C3}, /* 80 */
    {9,    0,    0x52, 0x14, 'S',  0,    stab3+0x72,  stab4+0x6},   /* 81 */
    {3,    0,    0x5A, 0x14, 'S',  0,    stab4+0x0,   stab0+0x2C3}, /* 82 */
    {3,    0,    0,    0x21, '\\', 'V',  stab4+0x49,  stab3+0x1B8}, /* 83 */
    {3,    0,    0x5A, 0x6E, 0,    0,    stab4+0x16,  stab0+0x2C3}, /* 84 */
    {0,    0,    0,    0x1F, 'U',  'o',  stab3+0x267, stab2+0x2B6}, /* 85 */
    {0,    0,    0,    0x1F, 'U',  'p',  stab1+0x91,  stab2+0x2B6}, /* 86 */
    {0,    0,    0,    0x28, 'a',  'o',  stab3+0x1CA, stab2+0x2B6}, /* 87 */
    {0,    0,    0,    0x28, 'a',  'p',  stab1+0x62,  stab2+0x2B6}, /* 88 */
    {0,    0,    0,    0x0E, 'V',  ']',  stab4+0x11A, stab2+0x2B6}, /* 89 */
    {0x0A, 0,    0,    7,    'V',  ']',  stab4+0x167, stab2+0x2B6}, /* 90 */
    {0,    0,    0,    0x25, 'a',  ']',  stab4+0x8C,  stab2+0x2B6}, /* 91 */
    {0x0A, 0,    0,    0x23, 'a',  ']',  stab4+0x105, stab2+0x2B6}, /* 92 */
    {3,    0,    0x1A, 0x0F, 'I',  'K',  stab3+0x16B, stab0+0x2C3}, /* 93 */
    {0,    0,    0x1B, 0x0F, 'a',  'q',  stab3+0x1BB, stab0+0x2C3}, /* 94 */
    {0,    0,    0,    0x3F, 'V',  0,    stab4+0x17D, stab2+0x2B6}, /* 95 */
    {0,    0,    0,    0x15, 'V',  0,    stab4+0x181, stab2+0x2B6}, /* 96 */
    {3,    0,    1,    0x5C, 0,    0,    stab4+0x5D,  stab0+0x2C3}, /* 97 */
    {0,    0,    0x12, 0x0A, 'M',  'N',  stab1+0xE9,  stab4+0x16C}, /* 98 */
    {0,    0,    0,    0x19, 'r',  's',  stab1+0x1F2, stab2+0x2B6}, /* 99 */
    {0,    0,    0,    0x72, 0,    0,    stab3+0x2F1, stab0+0x58},  /* 100 */
    {5,    0,    0,    0x53, 0,    0,    0,           stab0+0x261}, /* 101 */
    {6,    0,    0,    0x54, 0,    0,    0,           stab0+0x261}, /* 102 */
    {3,    0,    0,    0x56, 0,    0,    0,           stab0+0x261}, /* 103 */
    {0,    0,    0,    0x43, 0,    0,    0,           stab4+0xBC},  /* 104 */
    {0,    3,    0,    0x55, 0,    0,    0,           stab0+0x261}, /* 105 */
    {0,    0,    3,    0x5D, 0,    0,    stab4+0xCF,  stab0+0x2C3}, /* 106 */
    {0,    0x0D, 0,    0x5A, 0,    0,    0,           stab0+0x261}, /* 107 */
    {0,    0x0E, 0x0D, 0x5A, 0,    0,    stab4+0x9E,  stab0+0x2C3}, /* 108 */
    {0,    0x19, 0,    0x5A, 0,    0,    0,           stab0+0x261}, /* 109 */
    {0,    0,    0,    0x64, 0,    0,    0,           stab0+0x261}, /* 110 */
    {0,    0,    0,    0x0D, 'Y',  0,    0,           stab0+0x261}, /* 111 */
    {0,    0,    0,    0x42, 0,    0,    0,           stab4+0x1BE}, /* 112 */
    {0,    0,    0,    0x44, 0,    0,    stab1+0x1B0, stab4+0x199}, /* 113 */
    {0,    0,    0,    0x17, 0,    0,    0,           stab0+0x58},  /* 114 */
    {0,    0,    0,    0x44, 0,    0,    0,           stab4+0x1BE}, /* 115 */
    {0,    0,    0,    2,    'g',  'u',  0,           stab2+0x104}, /* 116 */
    {0,    0,    0,    2,    'v',  'u',  0,           stab4+0x153}, /* 117 */
    {0,    0,    0,    2,    'V',  'u',  stab4+0x13C, stab4+0x153}, /* 118 */
    {0x0A, 0,    0,    2,    'V',  ']',  stab4+0x158, stab2+0x104}, /* 119 */
    {0x0A, 0,    0,    2,    'V',  'p',  stab2+0x1A5, stab4+0x153}, /* 120 */
    {0x0B, 0,    1,    2,    'I',  'u',  stab3+0x17C, stab2+0x104}, /* 121 */
    {0x0B, 0,    0x1C, 2,    '\\', 'u',  stab2+0x79,  stab2+0x104}, /* 122 */
    {0x0B, 0,    1,    2,    'R',  'u',  stab2+0x1F3, stab2+0x104}, /* 123 */
    {0x0B, 0,    1,    2,    ']',  'u',  stab3+0x17C, stab2+0x104}, /* 124 */
    {0x0B, 0,    0,    0x1E, ']',  'u',  stab3+0x2E6, stab4+0x1BB}, /* 125 */
    {0x0B, 0,    0,    0x2E, ']',  'u',  stab3+0x2E6, stab4+0x1BC}, /* 126 */
    {0x0C, 0,    1,    2,    ']',  'u',  stab1+0x25B, stab2+0x104}, /* 127 */
    {0x0C, 0,    0,    0x1E, ']',  'u',  stab3+0x2DB, stab4+0x1BB}, /* 128 */
    {0x0C, 0,    0,    0x2E, ']',  'u',  stab3+0x2DB, stab4+0x1BC}, /* 129 */
    {0x0B, 0,    0,    2,    'T',  'd',  stab3+0x1E8, stab2+0x104}, /* 130 */
    {0,    0,    1,    2,    'T',  'd',  stab2+0x1D9, stab4+0x153}, /* 131 */
    {0,    0,    0x52, 2,    'S',  '_',  stab2+0xCF,  stab4+0x153}, /* 132 */
    {0,    0,    0,    0x5C, 0,    0,    0,           stab0+0x261}, /* 133 */
    {0,    0,    0,    0x0D, 'T',  0,    0,           stab4+0x142}, /* 134 */
    {0,    0x0E, 0,    0x5A, 0,    0,    0,           stab2+0x2B6}, /* 135 */
    {0,    0x0D, 0x0E, 0x5A, 0,    0,    stab4+0x9E,  stab0+0x2C3}, /* 136 */
    {0,    0,    0,    0x0F, 'c',  'J',  0,           stab3+0x281}, /* 137 */
    {0,    0,    0,    0x0E, 'W',  'x',  0,           stab4+0x162}, /* 138 */
    {0,    0,    0,    0x57, 0,    0,    0,           stab4+0x195}, /* 139 */
    {0,    0,    9,    0x0E, '\\', 'x',  stab4+0x5D,  stab4+0x148}, /* 140 */
    {0x0D, 0,    0,    0x0F, 'W',  'J',  0,           stab2+0x2C6}, /* 141 */
    {0x0E, 0,    0,    0x45, 0,    0,    0,           stab4+0x1BE}, /* 142 */
    {0x0D, 0,    0,    0x25, 'W',  'J',  stab4+0x21,  stab0+0x261}, /* 143 */
    {0,    0,    0,    0x25, 'W',  '^',  stab4+0x7A,  stab0+0x261}, /* 144 */
    {0,    0,    0,    0x21, 'W',  'Y',  stab4+0xFE,  stab0+0x261}, /* 145 */
    {0,    0,    0,    0x21, 'W',  '\\', stab4+0x71,  stab0+0x261}, /* 146 */
    {0,    0,    0,    0x21, '\\', 'W',  stab4+0x83,  stab0+0x148}, /* 147 */
    {0,    0,    0,    0x1B, 'J',  0,    0,           stab4+0x1AD}, /* 148 */
    {0,    0,    0,    0x42, 0,    0,    0,           stab4+0x1BE}, /* 149 */
    {0,    0,    0,    0x43, 0,    0,    0,           stab4+0x1B1}, /* 150 */
    {5,    0,    0,    0x53, 0,    0,    stab3+0x19C, stab0+0x58},  /* 151 */
    {3,    0,    0,    0x49, 0,    0,    0,           stab0+0x261}, /* 152 */
    {3,    0,    0,    0x0D, 'Z',  0,    0,           stab1+0x2B5}, /* 153 */
    {0x0F, 0,    0,    0x43, 0,    0,    0,           stab4+0xBC},  /* 154 */
    {0x10, 0,    0,    0x43, 0,    0,    0,           stab0+0x58},  /* 155 */
    {3,    0,    0,    0x25, 'I',  'o',  stab4+0x21,  stab0+0x261}, /* 156 */
    {3,    0,    0,    0x25, 'I',  'L',  stab4+0x35,  stab0+0x261}, /* 157 */
    {0,    0,    0x52, 0x47, 'T',  0,    stab0+0xDC,  stab4+0x136}, /* 158 */
    {0,    0,    0,    0x21, 'e',  'd',  stab1+0xBE,  stab2+0x2B6}, /* 159 */
    {6,    0,    0,    0x30, 'i',  'm',  0,           stab3+0x1B8}, /* 160 */
    {0,    0,    0x18, 0x59, 0,    0,    stab4+0xCF,  stab0+0x2C3}, /* 161 */
    {6,    0,    0x18, 0x5C, 0,    0,    stab4+0x5D,  stab0+0x2C3}, /* 162 */
    {6,    0,    0x18, 0x49, 0,    0,    stab3+0x9E,  stab0+0x2C3}, /* 163 */
    {0,    0,    0,    0x1C, 'H',  'Z',  0,           stab3+0x1B8}, /* 164 */
    {0,    0,    0x18, 0x57, 0,    0,    stab3+0x215, stab0+0x2C3}, /* 165 */
    {6,    0,    0x17, 0x22, 'R',  'd',  stab2+0x18B, stab2+0x2B6}, /* 166 */
    {6,    0,    0x17, 6,    'T',  'd',  stab3+0x5C,  stab2+0x2B6}, /* 167 */
    {0,    0,    0,    0x2F, 'Z',  'o',  stab3+0xDD,  stab2+0x2B6}, /* 168 */
    {0,    0,    0,    0x1F, 'T',  'o',  stab3+0x223, stab2+0x2B6}, /* 169 */
    {6,    0,    1,    0x28, 'R',  'p',  stab2+0x18B, stab2+0x2B6}, /* 170 */
    {0,    0,    1,    0x1F, 'T',  'p',  stab2+0x20D, stab2+0x2B6}, /* 171 */
    {0,    0,    0,    0x13, 'f',  'o',  stab2+0x2B9, stab2+0x2B6}, /* 172 */
    {0,    0,    0,    0x0F, 'f',  'o',  stab2+0x2D1, stab2+0x2B6}, /* 173 */
    {2,    0,    0,    0x23, 'f',  'J',  stab3+0x138, stab2+0x2B6}, /* 174 */
    {2,    0,    0,    7,    'Z',  'J',  stab3+0x1D9, stab2+0x2B6}, /* 175 */
    {1,    0,    0,    0x2C, 'f',  'J',  stab3+0x149, stab2+0x2B6}, /* 176 */
    {1,    0,    0,    0x3C, 'Z',  'J',  stab3+0x206, stab2+0x2B6}, /* 177 */
    {6,    0,    0x5C, 7,    ']',  ']',  stab0+0x263, stab1+0x2D5}, /* 178 */
    {6,    0,    1,    0x23, 'f',  ']',  stab0+0x230, stab2+0x2B6}, /* 179 */
    {6,    0,    0x1D, 0x0F, 'c',  'J',  stab2+0x0,   stab2+0x2B6}, /* 180 */
    {6,    0,    0x1D, 0x13, 'c',  'J',  stab1+0x18B, stab2+0x2B6}, /* 181 */
    {6,    0,    0x18, 0x0D, 'k',  0,    stab2+0x5B,  stab0+0x2C3}, /* 182 */
    {6,    0,    0x1D, 0x0D, 'l',  0,    stab1+0x113, stab0+0x2C3}, /* 183 */
    {6,    0,    0x1D, 0x0D, 'T',  0,    stab1+0x29B, stab0+0x2C3}, /* 184 */
    {6,    0,    0x1A, 0x52, 0,    0,    stab1+0x13B, stab2+0x2B6}, /* 185 */
    {6,    0,    0x18, 0x14, 'P',  0,    stab2+0x107, stab0+0x2C3}, /* 186 */
    {6,    0,    0x18, 0x14, 'Q',  0,    stab1+0x1D3, stab0+0x2C3}, /* 187 */
    {0,    0,    0x18, 0x25, 'W',  '^',  stab3+0x18,  stab0+0x2C3}, /* 188 */
    {6,    0,    0,    0x25, 'f',  'o',  stab3+0x2A8, stab2+0x2B6}, /* 189 */
    {6,    0,    0,    0x26, 'f',  'o',  stab3+0x274, stab2+0x2B6}, /* 190 */
    {6,    0,    0,    0x25, 'f',  'L',  stab3+0x2C2, stab2+0x2B6}, /* 191 */
    {6,    0,    0,    0x26, 'c',  '^',  stab3+0x15A, stab2+0x2B6}, /* 192 */
    {6,    0,    0,    0x25, 'c',  '^',  stab3+0x2CF, stab2+0x2B6}, /* 193 */
    {6,    0,    0,    0x21, '\\', 'Z',  stab4+0x49,  stab3+0x1B8}, /* 194 */
    {6,    0,    0,    0x21, 'I',  'Z',  stab3+0xB3,  stab3+0x1B8}, /* 195 */
    {6,    0,    0,    0x21, 'R',  '^',  stab1+0x2D9, stab3+0x1B8}, /* 196 */
    {6,    0,    0,    0x12, 'T',  '^',  stab3+0x1AC, stab2+0x2B6}, /* 197 */
    {0,    0,    0,    0x0E, 'Z',  'o',  stab2+0x2C6, stab2+0x2B6}, /* 198 */
    {0,    0,    0,    0x0E, 'Z',  'L',  stab4+0xB,   stab2+0x2B6}, /* 199 */
    {6,    0x1D, 0x0E, 0x0E, 'Z',  'Y',  stab3+0x103, stab0+0x2C3}, /* 200 */
    {6,    0,    0,    0x0E, 'T',  '^',  stab4+0x67,  stab2+0x2B6}, /* 201 */
    {0,    0,    0x0E, 0x15, '^',  0,    stab2+0x2A1, stab0+0x2C3}, /* 202 */
    {0,    0,    0,    0x21, 'W',  'Z',  stab3+0x23F, stab3+0x1B8}, /* 203 */
    {6,    0,    0x58, 0x6E, 0,    0,    stab2+0x289, stab0+0x2C3}, /* 204 */
    {0x11, 0,    0x58, 0x14, 'j',  0,    stab3+0xF1,  stab0+0x2C3}, /* 205 */
    {0x12, 0,    0x18, 0x14, 'V',  0,    stab2+0x1F,  stab0+0x2C3}, /* 206 */
    {0,    0,    0,    9,    '[',  0,    0,           stab0+0x261}, /* 207 */
    {0x13, 0,    0x52, 0x14, 'S',  0,    stab3+0x72,  stab3+0x1E4}, /* 208 */
    {6,    0,    0,    0x14, 'Z',  0,    0,           stab2+0x2B6}, /* 209 */
    {6,    0,    0,    0x14, 'S',  0,    0,           stab3+0x1E4}, /* 210 */
    {8,    0,    1,    0x15, 'S',  0,    stab3+0x88,  stab2+0x2B6}, /* 211 */
    {5,    0,    0x0F, 0x5C, 0,    0,    stab2+0x240, stab0+0x2C3}, /* 212 */
    {0,    0,    0x0F, 0x43, 0,    0,    stab3+0x46,  stab0+0x2C3}, /* 213 */
    {5,    0,    0x0F, 0x49, 0,    0,    stab1+0x31,  stab0+0x2C3}, /* 214 */
    {5,    0,    0,    0x6E, 0,    0,    0,           stab0+0x261}, /* 215 */
    {5,    0,    0,    0x30, 'i',  'm',  0,           stab3+0x1B8}, /* 216 */
    {5,    0,    0x54, 0x0D, 'T',  0,    stab0+0x59,  stab1+0x2D5}, /* 217 */
    {7,    0,    0x0C, 0x0E, 'S',  'J',  stab0+0x18C, stab2+0x2B6}, /* 218 */
    {5,    0,    0x52, 0x22, 'R',  '_',  stab2+0xEB,  stab4+0x16C}, /* 219 */
    {5,    0,    0x52, 0x0F, 'R',  0,    stab2+0x271, stab4+0x16C}, /* 220 */
    {5,    0,    0x52, 6,    'S',  '_',  stab3+0x0,   stab2+0x2B6}, /* 221 */
    {7,    0,    0x4F, 0x28, 'R',  'p',  stab1+0x27B, stab0+0x2C3}, /* 222 */
    {7,    0,    0x54, 0x1F, 'S',  'p',  stab2+0xB3,  stab2+0x2B6}, /* 223 */
    {5,    0,    0,    0x21, '\\', 'S',  stab2+0x227, stab3+0x1B8}, /* 224 */
    {5,    0,    0,    0x21, 'I',  'S',  stab1+0x0,   stab3+0x1B8}, /* 225 */
    {5,    0,    0x51, 0x21, 'R',  '_',  stab0+0x0,   stab0+0xAD},  /* 226 */
    {7,    0,    0x11, 0x15, '_',  0,    stab0+0x14A, stab0+0xAD},  /* 227 */
    {0x14, 0,    0x54, 0x14, 'd',  0,    stab1+0x23A, stab1+0x2D5}, /* 228 */
    {0x14, 0,    0x54, 0x14, 'V',  0,    stab1+0x163, stab1+0x2D5}, /* 229 */
    {0x15, 0,    0x4F, 0x14, 'd',  0,    stab4+0xAF,  stab0+0x2C3}, /* 230 */
    {0x15, 0,    0x4F, 0x14, 'U',  0,    stab3+0x2F,  stab0+0x2C3}, /* 231 */
    {0x16, 0,    0x52, 0x14, 'S',  0,    stab2+0x123, stab2+0x2B6}, /* 232 */
    {0x17, 0,    0x52, 0x14, 'T',  0,    stab2+0x13D, stab4+0x16C}, /* 233 */
    {0x18, 0,    0x52, 0x14, 'V',  0,    stab2+0x157, stab4+0x16C}, /* 234 */
    {0x19, 0,    0x52, 0x14, 'S',  0,    stab3+0x72,  stab2+0x2B6}, /* 235 */
    {0,    0,    0,    0x14, 'S',  0,    0,           stab2+0x2B6}, /* 236 */
    {0,    0,    0,    0x43, 0,    0,    0,           stab4+0xB7},  /* 237 */
    {0,    0,    0,    0x76, 0,    0,    0,           stab0+0x261}, /* 238 */
    {0,    0,    0,    0x0D, 'w',  0,    0,           stab4+0x63},  /* 239 */
    {3,    0,    0,    0x0D, 'T',  0,    0,           stab1+0x2B5}, /* 240 */
    {0x1A, 0,    0,    0x43, 0,    0,    0,           stab0+0x58},  /* 241 */
    {0x1B, 0,    0,    0x43, 0,    0,    0,           stab0+0x58},  /* 242 */
    {0,    0x0D, 0,    0x5B, 0,    0,    0,           stab0+0x261}, /* 243 */
    {0,    0,    0x0D, 0x0D, 'T',  0,    stab4+0x9E,  stab0+0x2C3}, /* 244 */
    {0,    0x0E, 0,    0x5B, 0,    0,    0,           stab0+0x261}, /* 245 */
    {0,    0,    0,    0x0D, 'T',  0,    0,           stab0+0x261}, /* 246 */
    {4,    0,    0x0C, 0x21, '`',  'R',  stab2+0x96,  stab3+0x1B8}, /* 247 */
    {0,    0,    0x18, 0x5C, 0,    0,    stab4+0xCF,  stab0+0x2C3}, /* 248 */
    {0,    0,    0,    0x0D, 'Z',  0,    0,           stab0+0x261}, /* 249 */
    {4,    0,    0,    0x6E, 0,    0,    0,           stab4+0x191}, /* 250 */
    {0x1C, 0,    0,    0x14, '[',  0,    0,           stab0+0x261}, /* 251 */
    {0x1D, 0,    0,    0x43, 0,    0,    0,           stab4+0xBC},  /* 252 */
    {0x1E, 0,    0,    0x43, 0,    0,    0,           stab0+0x58},  /* 253 */
};

char stab0[] = {
    "pop de\n"               /* 0x0 */
    "ld (hl),e\n"
    "inc\thl\n"
    "ld (hl),d\n"
    "inc hl\n"
    "pop bc\n"
    "ld (hl),c\n"
    "inc hl\n"
    "ld (hl),b\n"
    "ld\tl,c\n"
    "ld\th,b\0"
    "ld X000N,(GL)\n"        /* 0x59 */
    "inc GL\n"
    "ld X001N,(GL)\n"
    "inc GL\n"
    "ld X1N,(GL)\n"
    "inc\tGL\n"
    "ld X011N,(GL)\n"
    "ld X010N,X1N\0"
    "ld e,(hl)\n"            /* 0xB1 */
    "inc hl\n"
    "ld d,(hl)\n"
    "dec hl\n"
    "ex de,hl\n"
    "global bfext\n"
    "call bfext\n"
    "defb TN+(CN.shl.4)\0"
    "ld L,.low.VR\n"         /* 0x107 */
    "ld ~+1L,.high.VR\n"
    "ld ~+2L,.low.V+2R\n"
    "ld ~+3L,.high.V+2R\0"
    "ld hl,ON\n"             /* 0x14A */
    "pop bc\n"
    "or a\n"
    "sbc hl,bc\n"
    "pop\tbc\n"
    "ex de,hl\n"
    "ld hl,ON\n"
    "sbc hl,bc\0"
    "ex de,hl\n"             /* 0x18C */
    "ld bc,VR\n"
    "add hl,bc\n"
    "ex de,hl\n"
    "ld bc,V+2R\n"
    "adc hl,bc\0"
    "ex X10N,GL\n"           /* 0x1C6 */
    "global bfins\n"
    "call bfins\n"
    "defb TN+(CN.shl.4)\0"
    "ld hl,-ZN\n"            /* 0x1FC */
    "add\thl,sp\n"
    "ld sp,hl\n"
    "ex de,hl\n"
    "ld bc,ZN\n"
    "ldir\0"
    "ld a,G0L\n"             /* 0x230 */
    "ON R\n"
    "ld G0L,a\n"
    "ld a,G1L\n"
    "ON\t~+1R\n"
    "ld G1L,a\n"
    "L\0"
    "ld a,L\n"               /* 0x263 */
    "ON R\n"
    "ld X00N,a\n"
    "ld a,~+1L\n"
    "ON\t~+1R\n"
    "ld X01N,a\0"
    "ex de,hl\n"             /* 0x295 */
    "ld (XN),G1L\n"
    "dec XN\n"
    "ld (XN),G0L\n"
    "ex\tde,XN\0"
    "ex de,hl\n"             /* 0x2C6 */
    "ld G0L,(XN)\n"
    "inc XN\n"
    "ld G1L,(XN)\n"
    "ex\tde,hl"
};

char stab1[] = {
    "ld L,G00R\n"            /* 0x0 */
    "ld ~+1L,G01R\n"
    "ld ~+2L,G10R\n"
    "ld ~+3L,G11R\0"
    "ld X00N,L\n"            /* 0x31 */
    "ld X01N,~+1L\n"
    "ld X10N,~+2L\n"
    "ld X11N,~+3L\0"
    "inc GR\n"               /* 0x62 */
    "dec GR\n"
    "jr z,2f\n"
    "1:\n"
    "sONAN GL\n"
    "djnz 1b\n"
    "2:\n"
    "L\0"
    "inc GR\n"               /* 0x91 */
    "dec GR\n"
    "jr z,2f\n"
    "1:\n"
    "sONAN GL\n"
    "djnz 1b\n"
    "2:\0"
    "global bfins\n"         /* 0xBE */
    "call bfins\n"
    "defb TL+(CL.shl.4)\0"
    "call L\n"               /* 0xE9 */
    "exx\n"
    "ld hl,R\n"
    "add hl,sp\n"
    "ld sp,hl\n"
    "exx\0"
    "ld X0N,(GL)\n"          /* 0x113 */
    "inc GL\n"
    "ld X1N,(GL)\n"
    "dec GL\n"
    "L\0"
    "ld XN,(GL)\n"           /* 0x13B */
    "inc GL\n"
    "ld\tG1L,(GL)\n"
    "ld G0L,XN\0"
    "ld e,a\n"               /* 0x163 */
    "rla\n"
    "sbc a,a\n"
    "ld d,a\n"
    "ld l,a\n"
    "ld h,a\0"
    "ld XN,-VR\n"            /* 0x18B */
    "add\tGL,XN\n"
    "L\n"
    "or a\n"
    "sbc GL,XN\0"
    "psect data\n"           /* 0x1B0 */
    "eFN: deff SN\n"
    "psect text\0"
    "U0:\n"                  /* 0x1D3 */
    "ld XN,1\n"
    "jp U2f\n"
    "U1:\n"
    "ld XN,0\n"
    "U2:\0"
    "U0:\n"                  /* 0x1F6 */
    "ld XN,1\n"
    "p U2f\n"
    "U1:\n"
    "ld XN,0\n"
    "U2:\0"
    "call L\n"               /* 0x218 */
    "ld hl,R\n"
    "add hl,sp\n"
    "ld\tsp,hl\0"
    "ld a,d\n"               /* 0x23A */
    "rla\n"
    "sbc a,a\n"
    "ld l,a\n"
    "ld h,a\0"
    "ld XN,L\n"              /* 0x25B */
    "or ~+1L\n"
    "or ~+2L\n"
    "or ~+3L\0"
    "global\tasANlONsh\n"    /* 0x27B */
    "call\tasANlONsh\0"
    "ld X0N,(GL)\n"          /* 0x29B */
    "inc GL\n"
    "ld X1N,(GL)\0"
    "ld (GL),X1N\n"          /* 0x2BA */
    "dec GL\n"
    "ld (GL),X0N\0"
    "ld (GL),G0R\n"          /* 0x2D9 */
    "inc GL\n"
    "ld (GL),G1R"
};

char stab2[] = {
    "ld XN,VR\n"             /* 0x0 */
    "add GL,XN\n"
    "L\n"
    "sbc GL,XN\0"
    "ld X0N,a\n"             /* 0x1F */
    "rla\n"
    "sbc a,a\n"
    "ld X1N,a\0"
    "ld L,.low.VR\n"         /* 0x3D */
    "ld ~+1L,.high.VR\0"
    "ld X0N,(GL+0)\n"        /* 0x5B */
    "ld X1N,(GL+1)\n"
    "L\0"
    "ld X0N,(L)\n"           /* 0x79 */
    "ld\ta,X00N\n"
    "or X01N\0"
    "push hl\n"              /* 0x96 */
    "ld XN,ZN\n"
    "ldir\n"
    "pop hl\0"
    "global\tANlONsh\n"      /* 0xB3 */
    "call ANlONsh\0"
    "global\tALrelop\n"      /* 0xCF */
    "call ALrelop\0"
    "global\tasANlON\n"      /* 0xEB */
    "call asANlON\0"
    "ld XN,1\n"              /* 0x107 */
    "jp L,U0f\n"
    "dec\tXN\n"
    "U0:\0"
    "global\tALltof\n"       /* 0x123 */
    "call ALltof\0"
    "global\tALitof\n"       /* 0x13D */
    "call ALitof\0"
    "global\tALbtof\n"       /* 0x157 */
    "call ALbtof\0"
    "U1:jp ~-1U1f\n"         /* 0x171 */
    "U0:jp ~-1U0f\0"
    "global\tasANON\n"       /* 0x18B */
    "call asANON\0"
    "global\tbrelop\n"       /* 0x1A5 */
    "call brelop\0"
    "U1:jp ~-1U0f\n"         /* 0x1BF */
    "U0:jp ~-1U1f\0"
    "global wrelop\n"        /* 0x1D9 */
    "call wrelop\0"
    "ld XN,(GL)\n"           /* 0x1F3 */
    "inc GL\n"
    "or\t(GL)\0"
    "global\tshANON\n"       /* 0x20D */
    "call shANON\0"
    "ld (L),G0R\n"           /* 0x227 */
    "ld\t(~+2L),G1R\0"
    "ld X0N,(L)\n"           /* 0x240 */
    "ld\tX1N,(~+2L)\0"
    "ld L,X0N\n"             /* 0x259 */
    "ld ~+1L,X1N;XN\0"
    "global\tlANON\n"        /* 0x271 */
    "call lANON\0"
    "ld X0N,G10L\n"          /* 0x289 */
    "ld X1N,G11L\0"
    "ld XN,ON\n"             /* 0x2A1 */
    "or a\n"
    "sbc XN,GL\0"
    "dec GLV--R\n"           /* 0x2B9 */
    "L\n"
    "inc GLV--R\0"
    "inc GLV--R\n"           /* 0x2D1 */
    "L\n"
    "dec GLV--R"
};

char stab3[] = {
    "global\tANlON\n"        /* 0x0 */
    "call ANlON\0"
    "add L,GR\n"             /* 0x18 */
    "push\tL\n"
    "pop XN\0"
    "ld hl,0\n"              /* 0x2F */
    "ld d,l\n"
    "ld e,GL\0"
    "ld X0N,VN\n"            /* 0x46 */
    "ld X1N,V+2N\0"
    "global\tANON\n"         /* 0x5C */
    "call ANON\0"
    "global\tftol\n"         /* 0x72 */
    "call ftol\0"
    "ld a,h\n"               /* 0x88 */
    "xor 80h\n"
    "ld h,a\0"
    "ld X0N,L\n"             /* 0x9E */
    "ld X1N,~+1L\0"
    "ld L,G0R\n"             /* 0xB3 */
    "ld ~+1L,G1R\0"
    "ld L,X0N\n"             /* 0xC8 */
    "ld ~+1L,X1N\0"
    "srAN G1L\rrr G0LV--R\0" /* 0xDD */
    "ld X0N,L\n"             /* 0xF1 */
    "ld G1N,0\0"
    "ld XN,R\n"              /* 0x103 */
    "add XN,GL\0"
    "defw VL\n"              /* 0x115 */
    "defw V+2L\0"
    "ld XN,GL\n"             /* 0x127 */
    "push bc\0"
    "res TR&7CR,G0L\n"       /* 0x138 */
    "L\0"
    "set TR&7CR,G0L\n"       /* 0x149 */
    "L\0"
    "or a\n"                 /* 0x15A */
    "sbc GL,GR\n"
    "L\0"
    "ld XN,L\n"              /* 0x16B */
    "ON LV--R\0"
    "ld XN,L\n"              /* 0x17C */
    "or ~+1L\0"
    "jp L,U0f\n"             /* 0x18C */
    "jp U1f\0"
    "push hl\n"              /* 0x19C */
    "push de\0"
    "or a\n"                 /* 0x1AC */
    "sbc GL,GR\0"
    "ld XN,a\n"              /* 0x1BB */
    "ON a\n"
    "L\0"
    "sONAN GLV--R\n"         /* 0x1CA */
    "L\0"
    "res TR&7CR,G0L\0"       /* 0x1D9 */
    "or a\n"                 /* 0x1E8 */
    "sbc hl,de\0"
    "ld XN,(GL+0)\n"         /* 0x1F7 */
    "L\0"
    "set TR&7CR,G0L\0"       /* 0x206 */
    "push L\n"               /* 0x215 */
    "pop XN\0"
    "add hl,hlV--R\0"        /* 0x223 */
    "push XN\n"              /* 0x231 */
    "pop L\0"
    "push GR\n"              /* 0x23F */
    "pop L\0"
    "U0:\n"                  /* 0x24D */
    "jp R\n"
    "U1:\0"
    "bit\tTR&7CR,L\0"        /* 0x25A */
    "sONAN GLV--R\0"         /* 0x267 */
    "dec GLV--R\n"           /* 0x274 */
    "L\0"
    "inc hlV--R\n"           /* 0x281 */
    "L\0"
    "set\tTR&7CR,L\0"        /* 0x28E */
    "ld XN,(GL)\n"           /* 0x29B */
    "L\0"
    "inc GLV--R\n"           /* 0x2A8 */
    "L\0"
    "res\tTR&7CR,L\0"        /* 0x2B5 */
    "dec GLV++R\n"           /* 0x2C2 */
    "L\0"
    "add GL,GR\n"            /* 0x2CF */
    "L\0"
    "bit 7,~+3L\0"           /* 0x2DB */
    "bit 7,~+1L\0"           /* 0x2E6 */
    "jp U2f\n"               /* 0x2F1 */
    "U0:"
};

char stab4[] = {
    "ld XN,G00L\0"           /* 0x0 */
    "dec GLV++R\0"           /* 0xB */
    "ld XN,G10L\0"           /* 0x16 */
    "inc LV--R\0"            /* 0x21 */
    "ld XN,G0L\0"            /* 0x2B */
    "dec LV++R\0"            /* 0x35 */
    "dec LV--R\0"            /* 0x3F */
    "ld (L),GR\0"            /* 0x49 */
    "ld (L),XN\0"            /* 0x53 */
    "ld XN,(L)\0"            /* 0x5D */
    "add GL,GR\0"            /* 0x67 */
    "ld L,(R)\0"             /* 0x71 */
    "add L,GR\0"             /* 0x7A */
    "ld (L),R\0"             /* 0x83 */
    "ON a,R\n"               /* 0x8C */
    "L\0"
    "ld XN,GL\0"             /* 0x95 */
    "ex de,hl\0"             /* 0x9E */
    "ld L,VR\0"              /* 0xA7 */
    "ld hl,0\0"              /* 0xAF */
    ".low.VN\0"              /* 0xB7 */
    "ld L,GR\0"              /* 0xBF */
    "ld L,XN\0"              /* 0xC7 */
    "ld XN,L\0"              /* 0xCF */
    "push GL\0"              /* 0xD7 */
    "push hl\0"              /* 0xDF */
    "G00L;GL\0"              /* 0xE7 */
    "ld a,GL\0"              /* 0xEF */
    "G0L;GL\0"               /* 0xF7 */
    "ld L,R\0"               /* 0xFE */
    "ON R\n"                 /* 0x105 */
    "L\0"
    "push L\0"               /* 0x10C */
    "defw L\0"               /* 0x113 */
    "ON a,R\0"               /* 0x11A */
    "deff L\0"               /* 0x121 */
    "defb L\0"               /* 0x128 */
    "jp L,R\0"               /* 0x12F */
    "X111N\0"                /* 0x136 */
    "or GL\0"                /* 0x13C */
    "indir\0"                /* 0x142 */
    "XN+SR\0"                /* 0x148 */
    "VL9f\0"                 /* 0x14E */
    "ALON\0"                 /* 0x153 */
    "cp R\0"                 /* 0x158 */
    "jp L\0"                 /* 0x15D */
    "L+VR\0"                 /* 0x162 */
    "ON R\0"                 /* 0x167 */
    "X11N\0"                 /* 0x16C */
    "sub\0"                  /* 0x171 */
    "inc\0"                  /* 0x175 */
    "add\0"                  /* 0x179 */
    "cpl\0"                  /* 0x17D */
    "neg\0"                  /* 0x181 */
    "mul\0"                  /* 0x185 */
    "and\0"                  /* 0x189 */
    "mod\0"                  /* 0x18D */
    "G1L\0"                  /* 0x191 */
    "L+0\0"                  /* 0x195 */
    "eFN\0"                  /* 0x199 */
    "L+R\0"                  /* 0x19D */
    "xor\0"                  /* 0x1A1 */
    "div\0"                  /* 0x1A5 */
    "dec\0"                  /* 0x1A9 */
    "VL9\0"                  /* 0x1AD */
    "lVN\0"                  /* 0x1B1 */
    "ZN\0"                   /* 0x1B5 */
    "L:\0"                   /* 0x1B8 */
    "nz\0"                   /* 0x1BB */
    "SN\0"                   /* 0x1BE */
    "-1\0"                   /* 0x1C1 */
    "ge\0"                   /* 0x1C4 */
    "lt\0"                   /* 0x1C7 */
    "2\0"                    /* 0x1CA */
    "4"                      /* 0x1CC */
};



uint16_t dopetab[72] = {
    /* array_A7C0 */
    0x000C, /*  0000 0000 0000 1100	 0  0	""      NULSTR		*/
    0x0227, /*  0000 0010 0010 0111	 1  1	"!"	NOT 		*/
    0x0468, /*  0000 0100 0110 1000	 2  2	"!="    NEQL	EQUOP	*/
    0x0014, /*  0000 0000 0001 0100	 3  3	"#"     HASHSIGN	*/
    0x4208, /*  0100 0010 0000 1000	 4  4	"$"     DOLLAR		*/
    0x4004, /*  0100 0010 0000 1000	 5  5	"$U"    DOLLARU		*/
    8,      /*  0000 0000 0000 1000	 6  6	"%"    -MOD 		*/
    0x2848, /*  0010 1000 0100 1000	 7  7	"&"    -BAND	BITWOP	*/
    0x4328, /*  0100 0011 0010 1000	 8  8	"&&"	LAND 	LOGICOP	*/
    4,      /*  0000 0000 0000 0100	 9  9	"&U"    GADDR		*/
    0x4008, /*  0100 0000 0000 1000	10  A	"("	LPAREN 		*/
    0x4004, /*  0100 0000 0000 0100	11  B	")"	RPAREN		*/
    0x2048, /*  0010 0000 0100 1000	12  C	"*"    +MUL   	ARITMOP */
    0x1004, /*  0001 0000 0000 0100	13  D	"*U"    MULU		*/
    0x2C48, /*  0010 1100 0100 1000	14  E	"+"    -ADD   	ARITMOP */
    8,      /*  0000 0000 0000 1000	15  F	"++" 	INCR   		*/
    4,      /*  0000 0000 0000 0100	16 10	"+U"    PLUSU		*/
    8,      /*  0000 0000 0000 1000	17 11	","     COMMA		*/
    0x0C08, /*  0000 1100 0000 1000	18 12	"-"    -SUB   	ARITMOP */
    8,      /*  0000 0000 0000 1000	19 13	"--" 	DECR   		*/
    0x0208, /*  0000 0010 0000 1000	20 14	"->"    TCAST		*/
    0x0C07, /*  0000 1100 0000 0111	21 15	"-U"    MINUSU		*/
    8,      /*  0000 0000 0000 1000	22 16	"."     DOT		*/
    0x0030, /*  0000 0000 0011 0000	23 17	".."    TDOT		*/
    8,      /*  0000 0000 0000 1000	24 18	"/"    -DIV	ARITMOP */
    0x4008, /*  0100 0000 0000 1000	25 19	":"     COLON		*/
    0x4004, /*  0100 0010 0000 1000	26 1A	":U"    COLONU		*/
    0x4004, /*  0100 0010 0000 1000	27 1B	":s"    COLONS		*/
    0x4108, /*  0100 0001 0000 1000	28 1C	";"     SCOLON		*/
    0x000C, /*  0000 0000 0000 1100	29 1D	";;"    TSCOLON		*/
    0x0428, /*  0000 0100 0010 1000	30 1E	"<"     LT	RELOP	*/
    0x0C88, /*  0000 1100 1000 1000	31 1F	"<<"   -LSHIFT  SHIFTOP	*/
    0x0428, /*  0000 0100 0010 1000	32 20	"<="  	LEQ  	RELOP	*/
    8,      /*  0000 0000 0000 1000	33 21	"="   	ASSIGN		*/
    0x0408, /*  0000 0100 0000 1000	34 22	"=%"  	ASMOD		*/
    0x0C08, /*  0000 1100 0000 1000	35 23	"=&"  	ASAND		*/
    0x0C08, /*  0000 1100 0000 1000	36 24	"=*"  	ASMUL		*/
    0x0C08, /*  0000 1100 0000 1000	37 25	"=+"  	ASADD		*/
    0x0C08, /*  0000 1100 0000 1000	38 26	"=-"  	ASSUB		*/
    8,      /*  0000 0000 0000 1000	39 27	"=/"  	ASDIV		*/
    0x0C88, /*  0000 1100 1000 1000	40 28	"=<<" 	ASLSHIFT	*/
    0x0468, /*  0000 0100 0110 1000	41 29	"=="  	EQL  	EQUOP	*/
    0x0088, /*  0000 0000 1000 1000	42 2A	"=>>" 	ASRSHIFT	*/
    0x0C08, /*  0000 1100 0000 1000	43 2B	"=^"  	ASEXOR		*/
    0x0C08, /*  0000 1100 0000 1000	44 2C	"=|"  	ASEOR		*/
    0x0428, /*  0000 0100 0010 1000	45 2D	">"   	GT  	RELOP	*/
    0x0428, /*  0000 0100 0010 1000	46 2E	">="  	GEQ	RELOP	*/
    0x0088, /*  0000 0000 1000 1000	47 2F	">>"   -RSHIFT  SHIFTOP	*/
    0x4308, /*  0100 0011 0000 1000	48 30	"?"     QUEST		*/
    0x4004, /*  0100 0010 0000 1000	49 31	"@"     ATGIGN		*/

    0x000C, /*  0000 0000 0000 1100	50 32	"[\\"   CASE		*/
    0x000C, /*  0000 0000 0000 1100	51 33	"[a"    UNKNOWN		*/
    0x000C, /*  0000 0000 0000 1100	52 34	"[c"    ENUM		*/
    0x000C, /*  0000 0000 0000 1100	53 35	"[e"    EXPR		*/
    0x000C, /*  0000 0000 0000 1100	54 36	"[i"    INIT		*/
    0x000C, /*  0000 0000 0000 1100	55 37	"[s"    STRUCT		*/
    0x000C, /*  0000 0000 0000 1100	56 38	"[u"    UNION		*/

    0x000C, /*  0000 0000 0000 1100	57 39	"[v"    		*/
    0x2848, /*  0010 1000 0100 1000	58 3A	"^"    -BXOR   	BITWOP	*/
    0x000C, /*  0000 0000 0000 1100	59 3B	"{"     LBRACE		*/
    0x2848, /*  0010 1000 0100 1000	60 3C	"|"    -BOR	BITWOP	*/
    0x4328, /*  0100 0011 0010 1000	61 3D	"||" 	LOR   	LOGICOP	*/
    0x000C, /*  0000 0000 0000 1100	62 3E	"}"     RBRACE		*/
    0x0C07, /*  0000 1100 0000 0111	63 3F	"~"  	BNOT	BITWOP	*/
    4,      /*  0000 0000 0000 0100	64 40	"RECIP"	   		*/
    0,      /*  0000 0000 0000 0000	65 41	"TYPE"     		*/
    0x1002, /*  0001 0000 0000 0010	66 42	"ID"	   		*/
    0x0010, /*  0000 0000 0001 0000	67 43	"CONST"    		*/
    3,      /*  0000 0000 0000 0011	68 44	"FCONST"   		*/
    2,      /*  0000 0000 0000 0010	69 45	"REG"      		*/
    0x4004, /*  0100 0010 0000 1000	70 46	"INAREG"   		*/
    4       /*  0000 0000 0000 0100	71 47	"BITFIELD" 		*/
};


char *regNames[] = {
    "", "a", "c", "b", "e", "d", "l", "h", "ix", "iy", "sp", "af", "bc", "de", "hl"
};

/* clang-format off */
int array_AAE8[]     = { 0,     1,    2,    4,    8,    0x10, 0x20, 0x40, 0x80, 0x100,    /* 0-9 */
                         0x200, 1,    6,    0x18, 0x60, 0x78, 0x66, 0x7E, 0x7F, 0,        /* 10-19 */
                         0x79,  0x61, 0x19, 7,    0x7E, 0x1E, 0x3F, 0x3E, 0x7F, 0x1E };   /* 20-29 */

uint8_t array_AB24[] = { 0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
                         0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
                         0,  0,  1,  0,  2,  3,  4,  5,  6, 7,   
                         13, 14, 12, 14, 12, 15, 1,  17, 0, 0,
                         15, 1,  14, 1,  13, 1,  12, 1 };

uint8_t array_AB54[] = { 14, 13, 12, 0, 0, 0,         /* groups of 6 bytes, each is index into array_AAE8 */
                         13, 12, 0,  0, 0, 0,
                         1,  4,  5,  2, 3, 6,
                         4,  5,  2,  3, 6, 0,
                         21, 22, 23, 0, 0, 0,
                         12, 13, 0,  0, 0, 0 };
/* clang-format on */

/* end of file data.c */
