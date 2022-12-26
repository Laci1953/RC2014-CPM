/*
 * File optim1.h created 14.08.2021, last modified 11.11.2021.
 *
 * The optim1.h file is part of the restored optimization program
 * from the Hi-Tech C compiler v3.09 package.
 *
 *      Andrey Nikitin & Mark Ogden 11.11.2021
 */

/*#define _DEBUG  1 */

/*
 * instructions only used in the tables
 */
#define I_ADC  0x88 /*   Add */
#define I_ADD  0x80 /* */
#define I_BIT  0x40 /*   Tests if the specified bit is set */
#define I_DJNZ 0x10 /* */
#define I_JR   0x18 /* */
#define I_LDIR 0xB0 /*   Load, Inc., Repeat */
#define I_RESB 0x80 /*   Reset bit */
#define I_RL   0x10 /*   rl */
#define I_RLA  0x17 /*   rla */
#define I_RLC  0    /*   rlc */
#define I_RLCA 7    /*   rlca */
#define I_RLD  0x6F /*   rld */
#define I_RR   0x18 /*   rr */
#define I_RRA  0x1F /*   rra */
#define I_RRC  8    /*   rrc */
#define I_RRCA 0xF  /*   rrca */
#define I_RRD  0x67 /*   rrd */
#define I_RST  0xC7 /* */
#define I_SBC  0x98 /*   Subtract */
#define I_SETB 0xC0 /*   Set bit */
#define I_SLA  0x20 /*   sla */
#define I_SLL  0x20 /*   sll */
#define I_SRA  0x28 /*   sra */
#define I_SRL  0x38 /*   srl */
/*
 * instructions used in the code
 */
#define I_AND  0xA0 /*   Logical AND */
#define I_CCF  0x3F /*   Complement Carry Flag */
#define I_CP   0xB8 /*   Compare */
#define I_CPL  0x2F /*   Complement */
#define I_DI   0xF3 /*   Disable Interrupts */
#define I_EI   0xFB /*   Enable Interrupts */
#define I_EXX  0xD9 /*   Exchange */
#define I_HALT 0x76
#define I_NEG  0x44 /*   Negates the accumulator */
#define I_NOP  0    /*   No operation */
#define I_OR   0xB0 /*   Logical inclusive OR */
#define I_POP  0xC1 /*   pop */
#define I_PUSH 0xC5 /*   push */
#define I_SCF  0x37 /*   Set Carry Flag */
#define I_SUB  0x90 /*   sub */
#define I_XOR  0xA8 /*   xor */
/*
 * special shared instruction
 */
#define SI_DEC 5    /*   Decrement */
#define SI_INC 4    /*   Increment */

/*
 * The precedence values P_NA is for not applicable
 */
#define P_ADD  5 /* */
#define P_AND  4 /* */
#define P_CPAR 1 /* */
#define P_DIV  6 /* */
#define P_EQU  2 /* */
#define P_GE   2 /* */
#define P_HI   8 /* */
#define P_LE   2 /* */
#define P_LOW  8 /* */
#define P_MOD  6 /* */
#define P_MUL  6 /* */
#define P_NA   0 /* */
#define P_NOT  8 /* */
#define P_OPAR 9 /* */
#define P_OR   3 /* */
#define P_RES  7 /* */
#define P_SHL  6 /* */
#define P_SHR  6 /* */
#define P_SUB  5 /* */
#define P_XOR  3 /* */

/****************************************************************
 *  Token definitions
 *
 *  Symbol    Code  Description
 */

#define T_INVALID  0
#define T_SIMPLE  1 /* 1  one byte instruction: */
#define T_TWOBYTE  2 /* 2  two byte instruction: */
#define T_3    3 /*  Unknown: */
#define T_INCDEC    4 /* Unknown: */
#define T_5    5  /* Unknown */
#define T_SHIFT    6  /* Unknown: */
#define T_BIT    7  /* Bit operations */
#define T_JP    8  /* Absolute jumps to address */
#define T_JR    9  /* Relative jumps to address */
#define T_DJNZ  0xA  /* Dec., Jump Non-Zero */
#define T_CALL  0xB  /* Call */
#define T_RET  0xC  /* Uunknown */
#define T_RST  0xD  /* Restart Commands */
#define T_0xE  0xE  /* Unknown */
#define T_0xF  0xF  /* Unknown */
#define T_STK 0x10  /* Stack operation: */
#define T_EX 0x11  /* Exchange */
#define T_CARR 0x12  /* With Carry: */
#define T_CADD 0x13  /* Add */
#define T_LD 0x14  /* Load */
#define T_UPLUS 0x15  /* unary + */
#define T_UMINUS 0x16  /* unary - */
#define T_NOT 0x17  /* Bitwise complement */
#define T_HI 0x18  /* Hi byte of operand */
#define T_LOW 0x19  /* Low byte of operand */
#define T_RES 0x1A  /* .res. */
#define T_MARKER 0x1B  /* used to demark types */
#define T_PLUS 0x1C  /* Addition */
#define T_MINUS 0x1D /* Subtraction */
#define T_MUL 0x1E  /* Multiplication */
#define T_DIV 0x1F  /* Divison */
#define T_MOD 0x20  /* Modulus */
#define T_SHR 0x21  /* Shift right */
#define T_SHL 0x22  /* Shift left */
#define T_AND 0x23  /* Bitwise AND */
#define T_OR 0x24  /* Bitwise or */
#define T_XOR 0x25  /* Exclusive or */
#define T_EQ 0x26  /* Equality */
#define T_LE 0x27  /* Signed greater than */
#define T_GE 0x28  /* Signed less than */
#define T_ULE 0x29  /* Unigned greater than */
#define T_UGE 0x2A  /* Unigned less than */
#define T_OPAR 0x2B  /* Open parenthesis */
#define T_CPAR 0x2C  /* Closing parenthesis */
#define T_LASTOP 0x2D  /* used to demark end of ops */
#define T_FWD 0x2E  /* Local refernce forward */
#define T_BWD 0x2F  /* Local reference backward */
#define T_SYMBOL 0x31  /* 0x31  Symbol */
#define T_CONST 0x32  /* Constants */
#define T_STRING 0x33  /* String of characters */
#define T_PSCT 0x34  /* Psect */
#define T_GLB 0x35  /* Global */
#define T_COMM 0x36  /* Comma */
#define T_DEFW 0x37  /* Definition word */
#define T_DEFL 0x38  /* Definition label */
#define T_DEFB 0x39  /* Definition byte */
#define T_DEFM 0x3A  /* Definition a message */
#define T_DEFS 0x3B  /* Memory reservation */
#define T_DEFF 0x3C  /* Definition real */
#define T_EQU 0x3D  /* Set value of symbol */

#define T_EOL  0x40 /* 0x40  end of line      ';' */
#define T_COLN 0x41  /* Label separator  ':' */

#define T_REG  0x46 /* 0x46  Registers */
#define T_COND 0x47  /* Condition code: */

#define T_INDEXED 0x64 /* 0x64  (IX / IY) */
#define T_ADDRREF 0x65  /* (addr expression) */
#define T_REGREF  0x66  /* (reg) */

/*
 *      Determining register number
 *      (its offset in regs[offset] table)
 */
#define REG_B           0x00
#define REG_C           0x01
#define REG_D           0x02
#define REG_E           0x03
#define REG_H           0x04
#define REG_L           0x05
#define REG_F           0x06
#define REG_A           0x07
#define REG_I           0x08
#define REG_R           0x09
#define REG_BC          0x0A
#define REG_DE          0x0B
#define REG_HL          0x0C
#define REG_SP          0x0D
#define REG_AF          0x0E
#define REG_AF1         0x0F
#define REG_IX          0x10
#define REG_IY          0x11
#define REG_TRACKER     0x12
/*
 * Determining the condition number
 * (its offset in the conditions[offset] table) */

#define COND_NZ         1 /*   anz, fnz, lnz,   nz */
#define COND_Z          2 /*    az,  fz,  lz,   z  */
#define COND_LGE        3 /*              lge,  nc */
#define COND_LLT        4 /*              llt      */
/*#define COND_PO       5                       po */
/*#define COND_PE       6                       pe */
#define COND_GE         7 /*    age, fge,       p  */
#define COND_LT         8 /*    alt, flt,       m  */

/*
 *      psect definitions
 */
#define TEXT    1
#define DATA    2
#define BSS     3
#define SWDATA  4

/*
 *      Assigned type abbreviations
 */

#ifdef CPM
/* typedef unsigned short uint16_t; */
/* typedef short int16_t; */
/* typedef unsigned char uint8_t; */
#define const
#endif

typedef short int16_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

#ifndef bool
#define bool char /* int may be better */
#define true  1
#define false 0
#endif

#ifndef INT_MAX
#define INT_MAX 32767 /* max for int */
#endif

/*
 *      Definitions of structures
 *
 * Mark Ogden has given structure members meaningful names
 */
typedef struct {
    char *str;
    int type;
    int aux;
} operator_t;

typedef struct _list {
    struct _list *pNext;
} list_t;

typedef struct _term {
    int val;                       /* term's numeric value */
    union {
        struct _sym *pSym;         /* base symbol */
        struct _operand *pOperand; /* or rest of expression */
    } p;
} term_t;

typedef struct _operand {
    uint8_t type;
    uint8_t aux;
    term_t term;
} operand_t;

typedef struct _inst {
    char *opCode;
    uint8_t type;
    int aux;
    struct _inst *pNext;
    struct _inst *pAlt;
    union {
        struct {
            operand_t *lhs; /* used for operands */
            operand_t *rhs;
        } o;
        struct { /* used for symbols */
            struct _sym *pSym;
            int symId;
        } s;
    } u;
} inst_t;

typedef struct _sym {
    char *label;
    union {
        inst_t *pInst;       /* is either a pointer to the instruction associated with label */
        operand_t *pOperand; /* or the constant associated with the label */
    } p;
} sym_t;

typedef union {
    char   *pChar;
    sym_t  *pSym;
    int16_t i;
} YYSTYPE;

/*
 * to avoid lots of u.o. and u.s. used some #defines to simpilfy
 */
#define iLhs      u.o.lhs       /* used in inst */
#define iRhs      u.o.rhs
#define iPSym     u.s.pSym
#define iSymId    u.s.symId
#define tPSym     p.pSym        /* used in term */
#define tPInst    p.pInst
#define sPOperand p.pOperand    /* used in sym_t && term_t */
#define oPSym     term.tPSym    /* used in operand */
#define oPOperand term.sPOperand
#define oVal      term.val

struct _s1 {
    char hiReg;
    char loReg;
};

struct _s2 {
    operand_t *pHiRegVal;
    operand_t *pLoRegVal;
};

/*
 *      Descriptions of uninitialized variables and arrays
 */
extern char                 yyline[80];                 /* Working buffer */
extern int                  charsLeft;                  /* Length string in input buffer */
extern char               * ptr_inbuf;                  /* Pointer to input buffer  */
extern int                  yytype;
extern char                 inp_buf[512];               /* Input buffer (In the original it was 80) */
extern bool                 key_l;                      /* Prints additional information on each pass */
extern bool                 key_n;                      /* Prints statistics */
extern bool                 key_f;                      /* The action is not clear */
extern bool                 key_s;                      /* Key_s = 1 Unbuffered stdout */
extern int                  num_warn;                   /* Number of warnings */
extern bool                 key_r;
extern operator_t const   * tableBase;                  /* Pointer to keyword[] */
extern char               * yytext;
extern YYSTYPE              yylval;                     /* Contains different types of data */
extern int                  symbolId;
extern sym_t              * gPs;
extern inst_t             * gPi;
extern int                  word_6fb9;
extern bool                 hasChanged;
extern bool                 usesIXorIY;
extern int                  optimiseCnters[18]; /* Array of counters types of optimizations */


#define O_RED_LAB  0                /* 6fbd   0 Redundant labels */
#define O_JMP_TO_JMP 1                 /* 6fbf   1 Jumps to jumps */
#define O_STK_ADJUST 2                 /* 6fc1   2 Stack adjustments */
#define O_TMP_LAB 3                    /* 6fc3   3 Temporary labels */
#define O_UNREF_LAB 4                  /* 6fc5   4 Unref'ed labels */
#define O_UNREACH_LAB 5                /* 6fc7   5 Unreachable code */
#define O_JMP_TO_PLUS1 6               /* 6fc9   6 Jumps to .+1 */
#define O_SKIP_OVER_JMP 7              /* 6fcb   7 Skips over jumps */
#define O_CMN_CODE_SEQ 8               /* 6fcd   8 Common code seq's */
#define O_RED_EXX 9                    /* 6fcf   9 Redundant exx's */
#define O_EX_SPHL 10                    /* 6fd1  10 Ex (sp),hl's used */
#define O_RED_OPS 11                    /* 6fd3  11 Redundant operations */
#define O_RED_LD 12                     /* 6fd5  12 Redundant loads/stores */
#define O_SIMPLE_ADDR 13                /* 6fd7  13 Simplified addresses */
#define O_XOR_A 14                      /* 6fd9  14 Xor a's used */
#define O_RED_EX_DEHL 15                /* 6fdb  15 Redundant ex de,hl's */
#define O_CODE_MOTIONS 16               /* 6fdd  16 Code motions */
#define O_LOOPS_INV 17                  /* 6fdf  17 Loops inverted */


extern list_t             * freeOperandLst;             /* ptr to struct size 6 */
extern inst_t             * seq1;
extern inst_t             * seq2;
extern inst_t             * freeInstList;
extern char                 psect;                      /* Program section */
extern int                  cur_psect;                  /* Current program section */
extern int                  expectCond;
extern inst_t             * word_6fee;
extern int                  tokType;                    /* Token value */
extern inst_t             * switchVectors; /* word_6ff2; */
extern inst_t             * root;
extern int                  cntOperand;                 /* used only in sub_39a3 */
extern inst_t             * word_6ffc;

#ifndef CPM
#define MAXHEAP 0xff00

extern char               * heapBase;
extern char               * heapTop;
#endif

extern jmp_buf              jmpbuf;                     /* [8] */
extern int                  hlDelta;    /* word_7006; */
extern operand_t            regValues[19];
extern char               * alloct;
extern char               * name_fun;                   /* Function name */
extern list_t             * freeItemList;               /* 7080 */
extern char               * allocs;
extern char               * programBreak;

#define HASHSIZE 311

extern sym_t              * hashtab[HASHSIZE];          /* [622] */

/*
 *      Initialized variables
 */
extern char                 ccClass[];
extern char               * conditions[];
extern char               * regs[];
extern operator_t           operators[];
extern char               * opt_msg[];
extern int                  ccSwap[];
extern char               * psectNames[];
extern struct _s1           regHiLoMap[];
extern struct _s2           regHiLoValMap[];
extern int                  regTestMasks[];

/****************************************************************
 * Prototype functions are located in sequence of being in
 * original binary image of OPTIM.COM
 *
 * ok++ - Full binary compatibility with code in original file;
 * ok+  - Code generated during compilation differs slightly,
 *        but is logically correct;
 ****************************************************************/
/*
 * File OPTIM1.C
 */
int            strtoi(char const *fmt, int p2);                    /*  1 sub_013d ok++ */
char         * ptr_token();                                        /*  2 sub-020f ok++ */
int            const_value();                                      /*  3 sub_0289 ok+  */
int            get_token();                                        /*  4 sub_03c7 ok+  */
int            get_line();                                         /*  5 sub_0758 ok+  */
void           clr_len_inbuf();                                    /*  6 sub_07aa ok++ */
int            main(int, char **);                                 /*  7 sub_07b1 ok+  */
#if defined(__STDC__) || defined(__STDC_VERSION__)
void _Noreturn pr_error(char const * fmt, ...);
void           pr_warning(char const * fmt, ...);
void           pr_message(char const * fmt, va_list args);
#else
void           pr_error();                                         /*  8 sub_0941 ok++ */
void           pr_warning();                                       /*  9 sub_096f ok++ */
void           pr_message();                                       /* 10 sub_0994 ok++ */
#endif
int            find_token(char const *, operator_t const *, int);  /* 11 sub_09d0 ok++ */
int            num_token(char const *);                            /* 12 sub_0a97 ok++ */
void           pr_token(inst_t const *);                           /* 13 sub_0ab2 ok++ */
void           freeOperand(register operand_t *);                  /* 14 sub_0b6a ok++ */
void           freeInst(register inst_t *);                        /* 15 sub_0b8b ok++ */
operand_t    * allocOperand();                                     /* 16 sub_0ba7 ok++ */
inst_t       * allocInst(inst_t *);                                /* 17 sub_0be2 ok++ */
inst_t       * syntheticLabel(inst_t *);                           /* 18 sub_0ca2 ok++ */
void           optimise();                                         /* 19 sub_0ce4 ok++ */
void           chkIXYUsage();                                      /* 20 sub_0e67 ok++ */
void           sub_0ed1();                                         /* 21 sub_0ed1 ok++ */
bool           sub_1071(inst_t *);                                 /* 22 sub_1071 ok++ */
void           sub_122f();                                         /* 23 sub_122f ok++ */
bool           sub_1369(operand_t const *);                        /* 24 sub_1369 ok++ */
void           removeInstr(inst_t *);                              /* 25 sub_1397 ok++ */
inst_t       * getNxtRealInst(inst_t *);                           /* 26 sub_140b ok++ */
bool           operandsSame(operand_t *, operand_t *);             /* 27 sub_142f ok++ */
bool           instrSame(inst_t const *, inst_t const *);          /* 28 sub_14ac ok++ */
void           removeLabelRef(sym_t *);                            /* 29 sub_153d ok++ */
void           sub_15ad();                                         /* 30 sub_15ad ok++ */
/*
 * File PART2.C
 */
bool           sub_1795();                                         /* 31 sub_1795 ok++ */
bool           sub_1aec();                                         /* 32 sub_1aec ok++ */
bool           sub_1b86();                                         /* 33 sub_1b86 ok++ */
bool           sub_1c67();                                         /* 34 sub_1c67 ok++ */
bool           sub_1d94();                                         /* 35 sub_1d94 ok++ */
void           sub_1ec1();                                         /* 36 sub_1ec1 ok++ */
bool           sub_23c1();                                         /* 37 sub_23c1 ok++ */
bool           sub_24c0();                                         /* 38 sub_24c0 ok++ */
bool           sub_29c3();                                         /* 39 sub_29c3 ok+  */
bool           sub_2bdb();                                         /* 40 sub_2bdb ok++ */
bool           sub_2d3b();                                         /* 41 sub_2d3b ok+  */
bool           sub_2ef8();                                         /* 42 sub_2ef8 ok++ */
bool           sub_3053();                                         /* 42asub_3053 ok+  */
void           swapHLDE();                                         /* 43 sub_31ee ok++ */
void           pr_psect(int psect);                                /* 44 sub_328a ok++ */
int            num_psect(char const *);                            /* 45 sub_32bf ok++ */
term_t       * evalExpr();                                         /* 46 sub_3313 ok++ */
void           exp_err();                                          /* 47 sub_3595 ok++ */
void           uconv(int, term_t *);                               /* 48 sub_359e ok+  */
void           bconv(int, term_t *, term_t const *);               /* 49 sub_3630 ok++ */
/*
 * File PART2.3
 */
void           rel_err();                                          /* 50 sub_384d ok++ */
operand_t    * evalOperand();                                      /* 51 sub_3856 ok+  */
void           oper_err();                                         /* 52 sub_398e ok++ */
void           getOperands(inst_t *);                              /* 53 sub_39a3 ok++ */
void           loadFunction();                                     /* 54 sub_3a15 ok+  */
bool           sub_4000(inst_t const *);                           /* 55 sub_4000 ok+  */
void           sub_404d();                                         /* 56 sub_404d ok++ */
void           pr_instruction(inst_t *);                           /* 57 sub_420a ok+  */
void           sub_436e(operand_t const *);                        /* 58 sub_436e ok++ */
void           sub_44b2(operand_t const *);                        /* 59 sub_44b2 ok+  */
void           sub_4544(int);                                      /* 60 sub_4544 ok+  */
void           sub_4601();                                         /* 61 sub_4601 ok+  */
bool           sub_4625(inst_t const *);                           /* 62 sub_4625 ok+  */
bool           sub_4682(operand_t const *);                        /* 63 sub_4682 ok++ */
int            sub_46b1(operand_t const *, int);                   /* 64 sub_46b1 ok++ */
int            sub_475c(operand_t const *, int);                   /* 65 sub_475c ok++ */
int            sub_47a2(operand_t const *, int);                   /* 66 sub_47a2 ok++ */
bool           sub_47e0(int, inst_t const *, inst_t const *);      /* 67 sub_47e0 ok+  */
sym_t        * allocItem();                                        /* 68 sub_4c33 ok+  */
void           freeSymbol(sym_t *);                                /* 69 sub_4c6b ok+  */
int            hash_index(char const *, int);                      /* 70 sub_4cab ok++ */
sym_t        * lookupSym(char const *);                            /* 71 sub_4cf0 ok+  */
sym_t        * allocBlankSym();                                    /* 72 sub_4da7 ok++ */
void           resetHeap();                                        /* 73 sub_4dbf ok++ */
void           sub_4e20();                                         /* 74 sub_4e20 ok++ */
void         * alloc_mem(int);                                     /* 75 sub_4e2d ok+  */

#ifdef CPM
#define new_sbrk sbrk
#define new_brk  brk
char         * sbrk(int);
int            brk(void *);
#else
void         * sbrk(int size);
int            brk(void *);
void         * new_sbrk(int size);
int            new_brk(void *);
#endif

#ifdef _DEBUG
void           heapchk(void const *p);
#define HEAP(p) heapchk(p)
#else
#define HEAP(p)
#endif

#define logOptimise(n) (optimiseCnters[n]++, hasChanged = true)

#include "myalloc.h"

#define OFF_type                0
#define OFF_aux                 1
#define OFF_term                2
#define OFF_oVal                2
#define OFF_oPSym               4
#define OFF_oPOperand           4
#define OFF_iLhs                9
#define OFF_iRhs                11
#define OFF_sPOperand           2

char* my_alloc_mem(void);

#define regTestMasks_cnt 18

extern operand_t lo1,lo2,lo3,lo4;
extern operand_t *plo1, *plo2, *plo3, *plo4;

void PrintPi(void);
void PrintRegVal(short);

/* -----------------------
#define DEBUG 1
#define PRINT_LISTS 1
#define PRINT_REG_VAL 1
------------------------- */
