#include "stdio.h"
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#if defined(__STDC__) || defined(__STDC_VERSION__)
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#else
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
#ifndef bool
#define bool char
#define true 1
#define false 0
#endif
#define const
#endif

#ifndef INT_MAX
#define INT_MAX 32767 /* max for int */
#endif
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
enum type {
    T_INVALID = 0,
    T_SIMPLE,         /*    1  one byte instruction: */
    T_TWOBYTE,        /*    2  two byte instruction: */
    T_3,              /*    3  Unknown: */
    T_INCDEC,         /*    4  Unknown: */
    T_5,              /*    5  Unknown */
    T_SHIFT,          /*    6  Unknown: */
    T_BIT,            /*    7  Bit operations */
    T_JP,             /*    8  Absolute jumps to address */
    T_JR,             /*    9  Relative jumps to address */
    T_DJNZ,           /*  0xA  Dec., Jump Non-Zero */
    T_CALL,           /*  0xB  Call */
    T_RET,            /*  0xC  Uunknown */
    T_RST,            /*  0xD  Restart Commands */
    T_0xE,            /*  0xE  Unknown */
    T_0xF,            /*  0xF  Unknown */
    T_STK,            /* 0x10  Stack operation: */
    T_EX,             /* 0x11  Exchange */
    T_CARR,           /* 0x12  With Carry: */
    T_CADD,           /* 0x13  Add */
    T_LD,             /* 0x14  Load */
    T_UPLUS,          /* 0x15  unary + */
    T_UMINUS,         /* 0x16  unary - */
    T_NOT,            /* 0x17  Bitwise complement */
    T_HI,             /* 0x18  Hi byte of operand */
    T_LOW,            /* 0x19  Low byte of operand */
    T_RES,            /* 0x1A  .res. */
    T_MARKER,         /* 0x1B  used to demark types */
    T_PLUS,           /* 0x1C  Addition */
    T_MINUS,          /* 0x1D  Subtraction */
    T_MUL,            /* 0x1E  Multiplication */
    T_DIV,            /* 0x1F  Divison */
    T_MOD,            /* 0x20  Modulus */
    T_SHR,            /* 0x21  Shift right */
    T_SHL,            /* 0x22  Shift left */
    T_AND,            /* 0x23  Bitwise AND */
    T_OR,             /* 0x24  Bitwise or */
    T_XOR,            /* 0x25  Exclusive or */
    T_EQ,             /* 0x26  Equality */
    T_LE,             /* 0x27  Signed greater than */
    T_GE,             /* 0x28  Signed less than */
    T_ULE,            /* 0x29  Unigned greater than */
    T_UGE,            /* 0x2A  Unigned less than */
    T_OPAR,           /* 0x2B  Open parenthesis */
    T_CPAR,           /* 0x2C  Closing parenthesis */
    T_LASTOP,         /* 0x2D  used to demark end of ops */
    T_FWD,            /* 0x2E  Local refernce forward */
    T_BWD,            /* 0x2F  Local reference backward */
    T_SYMBOL = 0x31,  /* 0x31  Symbol */
    T_CONST,          /* 0x32  Constants */
    T_STRING,         /* 0x33  String of characters */
    T_PSCT,           /* 0x34  Psect */
    T_GLB,            /* 0x35  Global */
    T_COMM,           /* 0x36  Comma */
    T_DEFW,           /* 0x37  Definition word */
    T_DEFL,           /* 0x38  Definition label */
    T_DEFB,           /* 0x39  Definition byte */
    T_DEFM,           /* 0x3A  Definition a message */
    T_DEFS,           /* 0x3B  Memory reservation */
    T_DEFF,           /* 0x3C  Definition real */
    T_EQU,            /* 0x3D  Set value of symbol */

    T_EOL = 0x40,     /* 0x40  end of line      ';' */
    T_COLN,           /* 0x41  Label separator  ':' */

    T_REG = 0x46,     /* 0x46  Registers */
    T_COND,           /* 0x47  Condition code: */

    T_INDEXED = 0x64, /* 0x64  (IX / IY) */
    T_ADDRREF,        /* 0x65  (addr expression) */
    T_REGREF          /* 0x66  (reg) */
};

/****************************************************************
 *  Definitions of structures
 *
 * Unfortunately, the members of the structures have been assigned
 * uninformative names. Perhaps there are extra structures. The
 * links between structures and are wrong.
 ****************************************************************/

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
            operand_t *lhs;   /* used for operands */
            operand_t *rhs;
        } o;
        struct {              /* used for symbols */
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
    char *pChar;
    sym_t *pSym;
    int16_t i;
} YYSTYPE;

/*
 * to avoid lots of u.o. and u.s. used some #defines to simpilfy
 */
#define iLhs      u.o.lhs        /* used in inst */
#define iRhs      u.o.rhs
#define iPSym     u.s.pSym
#define iSymId    u.s.symId
#define tPSym     p.pSym         /* used in term*/
#define tPInst    p.pInst
#define sPOperand p.pOperand     /* used in sym_t && term_t*/
#define oPSym     term.tPSym     /* used in operand */
#define oPOperand term.sPOperand
#define oVal      term.val

enum {
    O_RED_LAB = 0,                /* 6fbd   0 Redundant labels */
    O_JMP_TO_JMP,                 /* 6fbf   1 Jumps to jumps */
    O_STK_ADJUST,                 /* 6fc1   2 Stack adjustments */
    O_TMP_LAB,                    /* 6fc3   3 Temporary labels */
    O_UNREF_LAB,                  /* 6fc5   4 Unref'ed labels */
    O_UNREACH_LAB,                /* 6fc7   5 Unreachable code */
    O_JMP_TO_PLUS1,               /* 6fc9   6 Jumps to .+1 */
    O_SKIP_OVER_JMP,              /* 6fcb   7 Skips over jumps */
    O_CMN_CODE_SEQ,               /* 6fcd   8 Common code seq's */
    O_RED_EXX,                    /* 6fcf   9 Redundant exx's */
    O_EX_SPHL,                    /* 6fd1  10 Ex (sp),hl's used */
    O_RED_OPS,                    /* 6fd3  11 Redundant operations */
    O_RED_LD,                     /* 6fd5  12 Redundant loads/stores */
    O_SIMPLE_ADDR,                /* 6fd7  13 Simplified addresses */
    O_XOR_A,                      /* 6fd9  14 Xor a's used */
    O_RED_EX_DEHL,                /* 6fdb  15 Redundant ex de,hl's */
    O_CODE_MOTIONS,               /* 6fdd  16 Code motions */
    O_LOOPS_INV                   /* 6fdf  17 Loops inverted */
};

#define _Z 0  /* 0000 0000 */
#define _U 1  /* 0000 0001 */
#define _L 2  /* 0000 0010 */
#define _D 4  /* 0000 0100 */
#define _H 8  /* 0000 1000 */
#define _S 16 /* 0001 0000 */

#define ISALPHA(c)  (ccClass[c] & (_U | _L))
#define ISUPPER(c)  (ccClass[c] & _U)
#define ISDIGIT(c)  (ccClass[c] & _D)
#define ISXDIGIT(c) (ccClass[c] & _H)
#define ISSPACE(c)  (ccClass[c] & _S)
#define ISALNUM(c)  (ccClass[c] & (_U | _L | _D))

/*
 * Determining the condition number
 * (its offset in the conditions[offset] table) */

#define   COND_NZ       1 /*   anz, fnz, lnz,   nz */
#define   COND_Z        2 /*    az,  fz,  lz,   z  */
#define   COND_LGE      3 /*              lge,  nc */
#define   COND_LLT      4 /*              llt      */
/*#define COND_PO       5                       po */
/*#define COND_PE       6                       pe */
#define   COND_GE       7 /*    age, fge,       p  */
#define   COND_LT       8 /*    alt, flt,       m  */

/*
 *  Determining register number
 *  (its offset in regs[offset] table)
 */

#define REG_B       0x00
#define REG_C       0x01
#define REG_D       0x02
#define REG_E       0x03
#define REG_H       0x04
#define REG_L       0x05
#define REG_F       0x06
#define REG_A       0x07
#define REG_I       0x08
#define REG_R       0x09 /* Memory Refresh Register */
#define REG_BC      0x0A
#define REG_DE      0x0B
#define REG_HL      0x0C
#define REG_SP      0x0D
#define REG_AF      0x0E
#define REG_AF1     0x0F
#define REG_IX      0x10
#define REG_IY      0x11
#define REG_TRACKER 0x12

#define NOPERATORS (sizeof(operators) / sizeof(operators[0]))

#define NOPTIM (sizeof(opt_msg) / sizeof(opt_msg[0]))

/*
 *  psect definitions
 */
#define TEXT   1
#define DATA   2
#define BSS    3
#define SWDATA 4

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
char          *ptr_token();                                        /*  2 sub-020f ok++ */
int            const_value();                                      /*  3 sub_0289 ok+  */
int            get_token();                                        /*  4 sub_03c7 ok+  */
int            get_line();                                         /*  5 sub_0758 ok+  */
void           clr_len_inbuf();                                    /*  6 sub_07aa ok++ */
int            main(int, char **);                                 /*  7 sub_07b1 ok+  */
#if defined(__STDC__) || defined(__STDC_VERSION__)
void _Noreturn pr_error(char const *fmt, ...);
void           pr_warning(char const *fmt, ...);
void           pr_message(char const *fmt, va_list args);
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
operand_t     *allocOperand();                                     /* 16 sub_0ba7 ok++ */
inst_t        *allocInst(inst_t *);                                /* 17 sub_0be2 ok++ */
inst_t        *syntheticLabel(inst_t *);                           /* 18 sub_0ca2 ok++ */
void           optimise();                                         /* 19 sub_0ce4 ok++ */
void           chkIXYUsage();                                      /* 20 sub_0e67 ok++ */
void           sub_0ed1();                                         /* 21 sub_0ed1 ok++ */
bool           sub_1071(inst_t *);                                 /* 22 sub_1071 ok++ */
void           sub_122f();                                         /* 23 sub_122f ok++ */
bool           sub_1369(operand_t const *);                        /* 24 sub_1369 ok++ */
void           removeInstruction(inst_t *);                        /* 25 sub_1397 ok++ */
inst_t        *getNextRealInst(inst_t *);                          /* 26 sub_140b ok++ */
bool           operandsSame(operand_t const *, operand_t const *); /* 27 sub_142f ok++ */
bool           instructionsSame(inst_t const *, inst_t const *);   /* 28 sub_14ac ok++ */
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
bool           sub_29c3();                                         /* 39 sub_29c3 ok++ */
bool           sub_2bdb();                                         /* 40 sub_2bdb ok++ */
bool           sub_2d3b();                                         /* 41 sub_2d3b ok++ */
bool           sub_2ef8();                                         /* 42 sub_2ef8 ok++ */
bool           sub_3053();                                         /* 42asub_3053 ok++ */
void           swapHLDE();                                         /* 43 sub_31ee ok++ */
void           pr_psect(int psect);                                /* 44 sub_328a ok++ */
int            num_psect(char const *);                            /* 45 sub_32bf ok++ */
term_t        *evalExpr();                                         /* 46 sub_3313 ok++ */
void           exp_err();                                          /* 47 sub_3595 ok++ */
void           uconv(int, term_t *);                               /* 48 sub_359e ok+  */
void           bconv(int, term_t *, term_t const *);               /* 49 sub_3630 ok++ */
/*
 * File PART2.3
 */
void           rel_err();                          /* 50 sub_384d ok++ */
operand_t     *evalOperand();                                      /* 51 sub_3856 ok+  */
void           oper_err();                                         /* 52 sub_398e ok++ */
void           getOperands(inst_t *);                              /* 53 sub_39a3 ok++ */
void           loadFunction();                                     /* 54 sub_3a15 ok++ */
bool           sub_4000(inst_t const *);                           /* 55 sub_4000 ok+  */
void           sub_404d();                                         /* 56 sub_404d ok++ */
void           pr_instruction(inst_t *);                           /* 57 sub_420a ok++ */
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
sym_t         *allocItem();                                        /* 68 sub_4c33 ok+  */
void           freeSymbol(sym_t *);                                /* 69 sub_4c6b ok+  */
int            hash_index(char const *, int);                      /* 70 sub_4cab ok++ */
sym_t         *lookupSym(char const *);                            /* 71 sub_4cf0 ok++ */
sym_t         *allocBlankSym();                                    /* 72 sub_4da7 ok++ */
void           resetHeap();                                        /* 73 sub_4dbf ok++ */
void           freeHashtab();                                      /* 74 sub_4e20 ok++ */
void          *alloc_mem(int);                                     /* 75 sub_4e2d ok+  */

void          *sbrk(int);
int            brk(void *);
void           heapchk(void const *p);
#ifdef _DEBUG
#define HEAP(p) heapchk(p)
#else
#define HEAP(p)
#endif

/* common code sequences */

#define PEEKCH()       (charsLeft > 0 ? *ptr_inbuf : '\n')
#define GETCH()        (--charsLeft >= 0 ? *ptr_inbuf++ : get_line())
#define logOptimise(n) (optimiseCounters[n]++, hasChanged = true)

extern char       yyline[80];            /* 6f00   Working buffer */
extern int        charsLeft;             /* 6f50   Length string in input buffer */
extern char      *ptr_inbuf;             /* 6f52   Pointer to input buffer */
extern int         yytype;               /* 6f54   */
extern char        inp_buf[80];          /* 6f56   Input buffer */
extern bool        key_l;                /* 6fa6   Prints additional information on each pass */
extern bool        key_n;                /* 6fa7   Prints statistics */
extern bool        key_f;                /* 6fa8   Use inline frame initialisation */
extern bool        key_s;                /* 6fa9   Key_s = 1 Unbuffered stdout */
extern int         num_warn;             /* 6faa   Number of errors */
extern bool        key_r;                /* 6fac   Disables register load optimisation */
extern operator_t  const *tableBase;     /* 6fad   Pointer to operators[] */
extern char       *yytext;               /* 6faf   */
extern YYSTYPE     yylval;               /* 6fb1   Contains different types of data */
extern int         symbolId;             /* 6fb3   Used only in sub_0ca2 */
extern sym_t      *gPs;                  /* 6fb5   */
extern inst_t     *gPi;                  /* 6fb7   */
                                  /* 6fb9   Moved to static in function*/
extern bool        hasChanged;           /* 6fbb   */
extern bool        usesIXorIY;           /* 6fbc   */
extern int         optimiseCounters[18]; /* 6fbd   Array of counters types of optimizations */

extern list_t     *freeOperandList;      /* 6fe1   struct size 6 */
extern inst_t     *seq1;                 /* 6fe3   */
extern inst_t     *seq2;                 /* 6fe5   */
extern inst_t     *freeInstList;         /* 6fe7   */
extern char        psect;                /* 6fe9   Program section */
extern int         cur_psect;            /* 6fea   Current program section */
extern int         expectCond;           /* 6fec   */
extern inst_t     *word_6fee;            /* 6fee   */
extern int         tokType;              /* 6ff0   Token value */
extern inst_t     *switchVectors;        /* 6ff2   */
/* static item_t termTmp;            6ff4   [4] */
extern inst_t     *root;                 /* 6ff8   */
extern int         cntOperand;           /* 6ffa   used only in sub_39a3 */
extern inst_t     *word_6ffc;            /* 6ffc   */
extern jmp_buf     jmpbuf;               /* 6ffe   [8] */
extern int         hlDelta;              /* 7006   hl inc/dec from original load */
extern operand_t   regValues[19];        /* 7008   */
extern char       *alloct;               /* 707a   Top of the current region */
extern char       *name_fun;             /* 707c   Function name */
                                  /* 707e   */
                                  /* 707f   */
extern list_t     *freeItemList;         /* 7080   */
extern char       *allocs;               /* 7082   current next allocated heap location */
extern char       *programBreak;         /* 7084   */
#define HASHSIZE 311

extern sym_t      * hashtab[HASHSIZE];   /* 7086   [622] */
                                  /* 72f4   */

extern char ccClass[];
extern char *conditions[];
extern char *regs[];
extern operator_t operators[];
extern char *opt_msg[];
extern int ccSwap[];
extern char *psectNames[];

extern struct {
    char hiReg;
    char loReg;
} regHiLoMap[];

extern struct {
    operand_t *pHiRegVal;
    operand_t *pLoRegVal;
} regHiLoValMap[];

extern int regTestMasks[];

/**************************************************************************
 56 sub_404d    ok++ (PMO)  Used in: optimise
 **************************************************************************/
void sub_404d() {

    register inst_t *pi;

    if (root->pNext) {
        pr_psect(TEXT);
        for (pi = root->pNext; pi; pi = pi->pNext) {
            if (pi->type == T_CALL && strcmp(pi->iLhs->oPSym->label, "ncsv") == 0) {
                pi = pi->pNext;
                if (pi->type != T_DEFW) { /* "defw" */
                    pr_error("Expecting defw after call ncsv");
                }
                if (pi->iLhs->oVal == 0) {
                    if (usesIXorIY) {
                        printf("global csv\ncall csv\n");
                    }
                } else {
                    usesIXorIY = true;
                    if (pi->iLhs->oVal >= -4) {
                        printf("global csv\ncall csv\npush hl\n");
                        if (pi->iLhs->oVal < -2) {
                            printf("push hl\n");
                        }
                    } else {
                        printf("call ncsv\ndefw %d\n", pi->iLhs->oVal);
                    }
                }
            } else if (!usesIXorIY && sub_4000(pi)) {
                pi->type   = T_RET;
                pi->opCode = NULL;
                pr_instruction(pi);
            } else if (!usesIXorIY && pi->type == T_CALL && pi->aux == 0 && pi->pNext->aux == 0 &&
                       sub_4000(pi->pNext) && pi->iLhs->oPSym->label[0] == '_') {
               pi->type   = T_JP; /* "jp" */
                pi->opCode = NULL;
                pr_instruction(pi);
                pi = pi->pNext;
            } else {
                pr_instruction(pi);
            }
        }
    }
    if (switchVectors->pNext) {
        pr_psect(DATA);
        for (pi = switchVectors->pNext; pi; pi = pi->pNext) {
            pr_instruction(pi);
        }
    }
}

/**************************************************************************
 57 sub_420a    ok++ (PMO)
 benign differences
 1) printf call/return code is shared
 2) fputc('\n', stdout) is located differently
 **************************************************************************/
void pr_instruction(register inst_t *pi) {

    if (pi->type == T_INVALID)
        ;
    else if (pi->type == T_SYMBOL) {
        if ((pi->iPSym->label[0])) {
            printf("%s:\n", pi->iPSym->label);
        } else {
            printf("L%d:\n", pi->iSymId);
        }
    } else if (pi->type == T_CONST) { /* m4: */
        printf("%d:\n", pi->aux);     /* OPTIMISER[1]: shares printf call with above*/
    } else if (key_f && pi->type == T_CALL && strcmp(pi->iLhs->oPSym->label, "csv") == 0) {
        printf("push\tiy\npush\tix\nld\tix,0\nadd\tix,sp\n");
    } else {
        if (key_n) {
            fputc('\t', stdout);    /* m7: */
        }

        pr_token(pi);

        if (pi->type == T_JP || pi->type == T_CALL || pi->type == T_RET) {
            fputc('\t', stdout);
            if (pi->aux != 0) {
                printf("%s", conditions[pi->aux]);
            }
            if (pi->type != T_RET) {
                if (pi->aux != 0) {
                    fputc(',', stdout);
                }
                sub_436e(pi->iLhs); /* m11: */
            }
        } else if (pi->iLhs) { /* m14: */
            fputc('\t', stdout);
            sub_436e(pi->iLhs);
            if (pi->iRhs) {
                fputc(',', stdout);
                sub_436e(pi->iRhs);
            }
        }
        fputc('\n', stdout); /* OPTIMISER: minor movement in where this is located also optimises return */
    }
}

/**************************************************************************
 58 sub_436e    ok++ (PMO)
 Same except optimiser misses the optimisation of fputc(')', stdout)
 **************************************************************************/
void sub_436e(register operand_t const *pi) {

    HEAP(pi);
    switch (pi->type) {
        case T_INDEXED:
        case T_ADDRREF:
            fputc('(', stdout);
            if (pi->type == T_INDEXED) {
                if (pi->aux == REG_IX) {
                    printf("ix");
                } else {
                    printf("iy");
                }
                fputc('+', stdout);
            }
        case T_CONST:
            if (pi->oPSym) {
                if (pi->oPSym->label[0]) {
                    printf("%s", pi->oPSym->label);
                } else {
                    printf("L%d", pi->oPSym->p.pInst->iSymId);
                }
                if (0 < pi->oVal) {
                    fputc('+', stdout);
                }
            }
            if (pi->oVal != 0 || !pi->oPSym) {
                printf("%d", pi->oVal);
            }
            if (pi->type != T_CONST) {
                fputc(')', stdout);
            }
            break;
        case T_REGREF:
            fputc('(', stdout);
        case T_REG:
            printf("%s", regs[pi->aux]);
            if (pi->type == T_REGREF) {
                fputc(')', stdout);    /* OPTIMISER[1]: misses optimising htis with same fputc above */
            }
            break;
        case T_FWD:
            printf("%df", pi->oVal);
            break;
        default:
            pr_error("Bad operand");
            break;
    }
    HEAP(pi);
}

/**************************************************************************
 59 sub_44b2    ok++ (PMO)
 Optimiser saves some code with minor changes to code for regValues[REG_TRACKER]

 original                   replacement
 ld a,(regValues + 6ch)     ld  hl,regvalues + 6ch  ; test regValues[REG_TRACKER].type
 or a                       ld  a,(hl)
 jp z,cret                  or  a
 push   iy                  jp  z,cret
 ld hl,regVales + 6ch       push    iy
 push   hl                  push    hl              ; &regValues[REG_TRACKER]

 The replacement code is slightly short
 **************************************************************************/
void sub_44b2(register operand_t const *po) {
    int n;

    while ((n = sub_46b1(po, REG_B)) != -1) {
        sub_4544(n);
    }
    while ((n = sub_46b1(po, REG_BC)) != -1) {
        sub_4544(n);
    }

    if (regValues[17].type && operandsSame(po, &regValues[17])) {
        sub_4544(17);
    }

    if (regValues[REG_TRACKER].type && operandsSame(&regValues[REG_TRACKER], po)) {
        regValues[REG_TRACKER].type = T_INVALID;
    }
}

/**************************************************************************
 60 sub_4544    ok++ (PMO)
 *  Optimiser generates marginally less efficient code for regValues[REG_TRACKER] access
 * it chooses to use hl and (hl) rather than the load directly to/from a
 * code functionally is the same
 **************************************************************************/
void sub_4544(int reg) {
    register operand_t *pi;

    regValues[reg].type = T_INVALID;
    if (regTestMasks[REG_HL] & regTestMasks[reg]) {
        hlDelta                     = 0;
        regValues[REG_TRACKER].type = T_INVALID;
    }
    if (pi = regHiLoValMap[reg].pHiRegVal) {
        pi->type = T_INVALID;
        if (pi = regHiLoValMap[reg].pLoRegVal) {
            pi->type = T_INVALID;
        }
    }
    if (reg != 17) {
        return;
    }
    for (pi = regValues; pi < &regValues[REG_TRACKER]; ++pi)
        if (pi->type == T_INDEXED && pi->aux == REG_IY) {
            pi->type = T_INVALID;
        }

    if (regValues[REG_TRACKER].type == T_INDEXED && regValues[REG_TRACKER].aux == REG_IY) {
        regValues[REG_TRACKER].type = T_INVALID;
    }
}

/**************************************************************************
 61 sub_4601    ok+ (PMO)
 **************************************************************************/
void sub_4601() {
    register operand_t *po;

    for (po = regValues; po < &regValues[REG_TRACKER]; ++po) {
        po->type = T_INVALID;
    }
    regValues[REG_TRACKER].type = T_INVALID;
}

/**************************************************************************
 62 sub_4625    ok++ (PMO)
 **************************************************************************/
bool sub_4625(register inst_t const *pi) {

    switch (pi->type) {
        case T_LD:
        case T_STK:
            return true;
        case T_INCDEC:
            return pi->iLhs->type == T_REG && pi->iLhs->aux >= REG_BC;
        case T_EX:
            return pi->iLhs->aux != REG_AF;
    }
    return false;
}

/**************************************************************************
 63 sub_4682    ok++ (PMO)
 **************************************************************************/
bool sub_4682(register operand_t const *pi) {

    return pi->type == T_CONST && !pi->oPSym && pi->oVal == 0;
}

/**************************************************************************
 64 sub_46b1    ok++ (PMO)
 **************************************************************************/
int sub_46b1(register operand_t const *opr, int reg) {
    operand_t *po;
    int        i;

    po = reg < REG_BC ? &regValues[REG_B] : &regValues[REG_BC];
    do {
        if (po->type) {
            if (operandsSame(po, opr)) {
                i = (int)(po - regValues);
                if (i >= REG_BC && reg < REG_BC) {
                    return regHiLoMap[i].loReg;
                }
                return i;
            }
        }
    } while (++po < &regValues[REG_SP]);
    return -1;
}

/**************************************************************************
 65 sub_475c    ok++ (PMO)
 **************************************************************************/
int sub_475c(register operand_t const *po, int p2) {

    if (!po) {
        return false;
    }
    if (po->type != T_REG && po->type != T_INDEXED && po->type != T_REGREF) {
        return false;
    }
    if (p2 & regTestMasks[po->aux]) {
        return true;
    }
    return false;
}

/**************************************************************************
 66 sub_47a2    ok++
 **************************************************************************/
int sub_47a2(register operand_t const *po, int p2) {

    if (!po) {
        return false;
    }
    if (po->type != T_REG) {
        return false;
    }
    if (p2 & regTestMasks[po->aux]) {
        return true;
    }
    return false;
}

/**************************************************************************
 67 sub_47e0    ok++ (PMO)
 *  Optimiser differences
 * 1) the code for case 0 is detected as the same as the code in case T_JP
 *    and reused
 * 2) there are two instances where the test cases are reversed along with
 *    the associated jumps. Leaving the logic the same
 **************************************************************************/
bool sub_47e0(int reg, register inst_t const *pi1, inst_t const *pi2) {
    uint16_t msk;
    sym_t   *ps;
    inst_t  *l3;
    int      n; /* number of iterations */

    if (reg == REG_SP || reg == REG_IX) {
        return true;
    }

    if (REG_BC <= reg && reg <= REG_HL) {
        if (sub_47e0(regHiLoMap[reg].hiReg, pi1, pi2)) {
            return true;
        }
        reg = regHiLoMap[reg].loReg;
    }
    if (reg >= sizeof(regTestMasks) / sizeof(regTestMasks[0])) {
        fprintf(stderr, "%d\n", reg);
    }
    /* #pragma warning(suppress : 6385) /* reg has limited values */
    msk = regTestMasks[reg]; /* m3: */
    n   = 40;

    do {
        switch (pi1->type) {

            case T_CALL:
                if (pi1->aux != 0 && (msk & 0x40)) {
                    return true;
                }

                if (reg == REG_IY) {
                    break;
                }
                if (!pi1->iLhs->oPSym || pi1->iLhs->oPSym->label[0] == '_') {
                    return false;
                }

                if (!(msk & 0xBF)) {
                    return false;
                } else {
                    return true;
                }

            case T_JP:
                if (pi1->aux != 0 || pi1->iLhs->type == T_REGREF || !(ps = pi1->iLhs->oPSym)) {
                    return true;
                }
                if (!(l3 = ps->p.pInst))
                    if (strcmp(ps->label, "cret") != 0) {
                        return true;
                    } else if (msk & 0x3C) { /* code reused */
                        return true;
                    } else {
                        return false;
                    }
                pi1 = l3;
                break;

            case T_SIMPLE:
                switch (pi1->aux) { /* m14: */
                    case 0:             /* "nop" */
                    case I_CPL:
                    case I_SCF:
                    case I_CCF:
                    case I_NEG:
                    case I_HALT:
                    case I_DI:
                    case I_EI:
                        break;
                    case I_EXX:
                        while ((pi1 = pi1->pNext) && (pi1->type != T_SIMPLE || pi1->aux != I_EXX))
                            if (pi1->type != T_LD && pi1->type != T_STK && pi1->type != T_CADD) {
                                return false;
                            }
                        if (!pi1) {
                            return false;
                        }
                        break;
                    default:
                        if (msk & 0x80) {
                            return true;
                        }
                        break;
                }
                break;
            case T_TWOBYTE:
                if (reg != 17) {
                    return true;
                }
                break;

            case T_3:
                if (pi1->aux == I_XOR) /* m22: */
                    if (pi1->iLhs->type == T_REG && pi1->iLhs->aux == REG_A && reg == REG_A) {
                        return false;
                    }
                if (msk & 0x80) {
                    return true;    /* m23: */
                }
                if (msk & 0x40) {
                    return false;
                }
                if (sub_475c(pi1->iLhs, msk)) { /* OPTIMISER[1]: see below */
                    return true;
                }
                break;

            case T_SHIFT:
                if ((pi1->aux & 0xFFE7) != 0x20 && (msk & 0x40)) {
                    return true;
                }

            case T_INCDEC:
                if ((msk & 0x40) && (pi1->iLhs->type != T_REG || pi1->iLhs->aux < REG_BC)) {
                    return false;
                }
                if (pi1->iLhs->type == T_REG || sub_475c(pi1->iLhs, msk)) {
                    return true;
                }
                break;

            case T_BIT: /* "set", "res", "bit" */
                if (pi1->aux == 0x40 && (msk & 0x40)) {
                    return false;
                }

            case T_0xF: /* 0xF */
                if (sub_475c(pi1->iRhs, msk) || sub_475c(pi1->iLhs, msk)) {
                    return true;
                }
                break;

            case T_5:
                break;

            case T_DJNZ:
                if (msk & 1) {
                    return true;
                }
                break;

            case T_0xE:
                if (sub_475c(pi1->iRhs, msk)) {
                    return true;
                }
                if (sub_47a2(pi1->iLhs, msk)) {
                    return false;
                }
                break;

            case T_STK:
                if (pi1->aux == I_PUSH && (msk & regTestMasks[pi1->iLhs->aux])) {
                    return true;
                }
                if (msk & regTestMasks[pi1->iLhs->aux]) {
                    return false;
                }
                break;

            case T_EX:
                if (pi1->iLhs->type == T_REGREF && (msk & regTestMasks[pi1->iRhs->aux])) {
                    return true;
                }
                if (msk & 0x3C) {
                    return true;
                }
                break;

            case T_CADD:
                if (msk & 0x40) {
                    return false;
                }

            case T_CARR: /* Add, sub with Carry */
                if ((regTestMasks[pi1->iLhs->aux] | 0x40) & msk) {
                    return true;
                }
                if (sub_475c(pi1->iRhs, msk)) {
                    return true;
                }
                break;
            case T_LD:
                if (!operandsSame(pi1->iLhs, pi1->iRhs))
                    if (sub_475c(pi1->iRhs, msk)) {
                        return true;
                    } else if (sub_47a2(pi1->iLhs, msk)) {
                        return false;
                    } else if (sub_475c(pi1->iLhs, msk)) {
                        return true;
                    }
                break;

            case 0: /* OPTIMISER[2]: optimised to reuse code in T_JP */
                if (msk & 0x3C) {
                    return true;
                } else {
                    return false;
                }
            case T_JR:
            case T_RET:
            case T_RST:
                break;
        }
        pi1 = pi1->pNext;
        if (pi2 == pi1) {
            return false;
        }
    } while (n-- != 0);
    return true;
}

/**************************************************************************
 68 sub_4c33    ok++ (PMO)  Used in: sub_4cf0, sub_4da7
 **************************************************************************/
sym_t *allocItem() {
    register sym_t *ps;

    ps = (sym_t *)freeItemList; /* check the free list*/
    if (ps) {                   /* if there is an entry release it*/
        freeItemList = ((list_t *)ps)->pNext;
        ps->label    = NULL;
        ps->p.pInst  = NULL;
        return ps;
    }
    return (sym_t *)alloc_mem(sizeof(sym_t)); /* else allocate a new one */
}

/**************************************************************************
 69 sub_4c6b    ok++ (PMO)
 **************************************************************************/
void freeSymbol(register sym_t *ps) {

    if (strlen(ps->label) >= sizeof(sym_t)) { /* if string could be reused as a symbol at it to the free list*/
        ((list_t *)(ps->label))->pNext = freeItemList;
        freeItemList                   = (list_t *)(ps->label);
    }
    ((list_t *)ps)->pNext = freeItemList; /* add the symbol to the free list */
    freeItemList          = (list_t *)ps;
}

/**************************************************************************
 70 hash_index  sub_4cab    ok++ (PMO)  Used in: sub_4cf0
 **************************************************************************/
int hash_index(register char const *s, int size) {
    uint16_t hash;

    for (hash = 0; *s != 0; ++s) {
        hash += *(uint8_t *)s + hash;
    }
    return hash % size;
}

/**************************************************************************
 71 sub_4cf0    ok++ (PMO)          Used in: get_token
 **************************************************************************/
sym_t *lookupSym(register char const *s) {
    sym_t **pps;
    sym_t *ps;

    pps = &hashtab[hash_index(s, HASHSIZE)];
    while (*pps && strcmp((*pps)->label, s))
        if (++pps == &hashtab[HASHSIZE]) {
            pps = hashtab;
        }
    if (ps = *pps) {
        return ps;
    }
    *pps = ps = allocItem();
    ps->label = alloc_mem((int)strlen(s) + 1);
    strcpy(ps->label, s);
    return ps;
}

/**************************************************************************
 72 sub_4da7    ok++ (PMO)
 **************************************************************************/
sym_t *allocBlankSym() {
    register sym_t *ps;

    ps        = allocItem();
    ps->label = "";
    return ps;
}

/**************************************************************************
 73 sub_4dbf    ok++ (PMO)          Used in: optimise
 **************************************************************************/
void resetHeap() {
    int *p;

    if (programBreak == 0) {
        programBreak = sbrk(0);    /* current base of heap */
    } else {
        brk(programBreak);         /* reset the heap */
    }
    alloct = allocs = programBreak;

    for (p = (int *)hashtab; p < (int *)&hashtab[HASHSIZE];) {
        *p++ = 0;
    }
    freeItemList = NULL;
}

/**************************************************************************
 74 sub_4e20    ok++     (PMO)      Used in: optimise
 **************************************************************************/
void freeHashtab() {

    allocs = (char *)&hashtab;           /* */
    alloct = (char *)&hashtab[HASHSIZE]; /* */
}

/**************************************************************************
 75 alloc_mem   sub_4e2d    ok++ (PMO)
 Optimiser differences.
 1) res instruction is used to clear lsb rather than and 0xfe
 2) de is loaded before the size calculation. As it is not used before
    the code does the same thing
 **************************************************************************/
void *alloc_mem(int size) {
    char *p;
    register char *pi;

    if ((size = (size + 1) & ~1) + allocs > alloct) {
        if ((allocs = sbrk(512)) == (char *) -1) {
            pr_error("Out of memory in %s", name_fun);
        }
        alloct = sbrk(0);
    }

    pi = allocs;
    allocs += size;
    for (p = pi; size-- != 0;) {
        *p++ = 0;    /* Clearing allocated memory area */
    }
    return pi;
}

/*
 * simple sbrk & brk implementations
 */
#ifndef CPM

#define MAXHEAP (32000 * sizeof(char *))
static char *heapBase;
static char *heapTop;

void *sbrk(int size) {
    if (!heapBase && !(heapTop = heapBase = malloc(MAXHEAP))) {
        fprintf(stderr, "can't allocate heap!!\n");
    }
    if (heapTop + size >= heapBase + MAXHEAP) {
        return (void *) -1;
    }
    heapTop += size;
    return heapTop - size;
}

int brk(void *p) {

    if (heapBase <= (char *)p && (char *)p < heapBase + MAXHEAP) {
        heapTop = p;
        return 0;
    }
    return -1;
};

void heapchk(void const *p) {

    if (p && (p < (void *)heapBase || p >= (void *)heapTop) &&
            (p < (void *)hashtab || p >= (void *)&hashtab[HASHSIZE])) {
        fprintf(stderr, "out of range heap item\n");
    }
}
#endif

/**************************************************************************

#######                     ####### ######  #######   ###   #     #
#        #    #  #####      #     # #     #    #       #    ##   ##
#        ##   #  #    #     #     # #     #    #       #    # # # #
#####    # #  #  #    #     #     # ######     #       #    #  #  #
#        #  # #  #    #     #     # #          #       #    #     #
#        #   ##  #    #     #     # #          #       #    #     #
#######  #    #  #####      ####### #          #      ###   #     #

*/
