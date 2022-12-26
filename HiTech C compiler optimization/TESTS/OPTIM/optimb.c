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
void           rel_err();                                          /* 50 sub_384d ok++ */
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
 36 sub_1ec1    ok++ (PMO)
 **************************************************************************/
void sub_1ec1() {
    int l1;

    sub_4601();
    for (gPi = root; gPi; gPi = gPi->pNext) {
        switch (gPi->type) {
            case T_LD:
                if (!key_r) {
                    if (sub_24c0()) {
                        continue;
                    }
                    if (gPi->iLhs->type == T_REG && gPi->iLhs->aux == REG_A && sub_4682(gPi->iRhs) &&
                            !sub_47e0(REG_F, gPi->pNext, gPi)) {
                        gPi->iRhs   = NULL;
                        gPi->type   = T_3;
                        gPi->aux    = I_XOR; /* xor */
                        gPi->opCode = NULL;
                        logOptimise(O_XOR_A); /* 6fbf opt_msg[14] = "xor a's used" */
                    }
                }
                break;

            case T_SYMBOL:
                for (; gPi->type == T_SYMBOL && gPi->aux == 0; gPi = gPi->pNext) {
                    removeInstruction(gPi);
                    logOptimise(O_UNREF_LAB); /* 6fc5 opt_msg[4] = "Unref'ed labels" */
                }
            /* fall through */

            case T_TWOBYTE:
            case T_CALL:
                sub_4601();
                break;

            case T_EX:
                if (gPi->iLhs->type == T_REG) {
                    if (sub_23c1()) {
                        continue;
                    }
                    swapHLDE();
                } else {
                    sub_4544(gPi->iRhs->aux);
                }
                break;

            case T_SIMPLE:
                switch (gPi->aux) {
                    case I_NOP:
                    case I_SCF:
                    case I_CCF:
                    case I_HALT:
                    case I_DI:
                    case I_EI:
                        break;
                    case I_EXX:
                        sub_4601();
                        break;
                    default:
                        sub_4544(REG_AF);
                        break;
                }
                break;

            case T_BIT:
                if (gPi->aux != 0x40)
                    if (gPi->iRhs->type == T_REG) {
                        sub_4544(gPi->iRhs->aux);
                    } else if ((l1 = sub_46b1(gPi->iRhs, 0)) != -1) {
                        sub_4544(l1);
                    }
                break;
caseCommon:
            case T_CARR:
                if (gPi->iLhs->type == T_REG && !sub_47e0(gPi->iLhs->aux, gPi->pNext, gPi)) {
                    if ((gPi->type == T_INCDEC && gPi->iLhs->aux >= REG_BC) || !sub_47e0(REG_F, gPi->pNext, gPi)) {
                        removeInstruction(gPi);
                        logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
                        continue;
                    }
                }
                /* fall through */ /* m25 */

            case T_SHIFT:
            case T_0xE:
                if (gPi->iLhs->type == T_REG) {
                    sub_4544(gPi->iLhs->aux);
                } else {
                    sub_44b2(gPi->iLhs);
                }
                break;

            case T_CADD:
                if (!sub_47e0(gPi->iLhs->aux, gPi->pNext, gPi)) {
                    removeInstruction(gPi);
                    logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
                    continue;
                } else if (gPi->iRhs->type == T_CONST && abs(gPi->iRhs->oVal) == 1 && !gPi->iRhs->oPOperand) {
                    gPi->type   = T_INCDEC;
                    gPi->aux    = gPi->iRhs->oVal != 1 ? SI_DEC : SI_INC;
                    gPi->opCode = NULL;
                    freeOperand(gPi->iRhs);
                    gPi->iRhs = NULL;
                    logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
                } else {
                    goto caseCommon;
                }
            /* fall through */

            case T_INCDEC:
                if (gPi->iLhs->type != T_REG) {
                    goto caseCommon;
                }
                if (gPi->iLhs->aux != REG_HL) {
                    goto caseCommon;
                }

                if (sub_2ef8()) {
                    continue;
                }
                break;

            case T_DJNZ:
                sub_4544(0);
                break;

            case T_STK:
                if (gPi->aux == I_PUSH) {
                    if (gPi->iLhs->aux == REG_IY && (l1 = sub_46b1(&regValues[17], REG_IY)) != -1 && l1 != REG_IY) {
                        gPi->iLhs->aux = l1;
                        logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
                    }
                    /* check for pop */
                    /* OTIMISER: fails to optimise the gPi->pNext-aux below */
                    if (gPi->pNext->type == T_STK && gPi->pNext->aux == I_POP && gPi->iLhs->aux == gPi->pNext->iLhs->aux) {
                        removeInstruction(gPi->pNext);
                        removeInstruction(gPi);
                        logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
                    } else {
                        break;
                    }
                } else {
                    sub_4544(gPi->iLhs->aux); /* m39: */
                    if (gPi->pAlt->type == T_STK && gPi->pAlt->aux == I_PUSH) {
                        regValues[gPi->iLhs->aux] = regValues[gPi->pAlt->iLhs->aux];
                        if (gPi->iLhs->aux == REG_IY && gPi->pNext->type == T_STK && gPi->pNext->iLhs->aux == REG_IY) {
                            gPi->pNext->iLhs->aux = gPi->pAlt->iLhs->aux;
                            logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
                        }
                    }
                    break;
                }
                continue;

            case T_3:
                if (sub_2d3b()) {
                    continue;
                }
                break;
        }
        sub_2bdb();
    }
}

/**************************************************************************
 37 sub_23c1    ok++ (PMO)
 **************************************************************************/
bool sub_23c1() {
    register inst_t *pi;

    if ((pi = gPi->pNext)->type == T_STK && pi->aux == I_PUSH && pi->iLhs->aux == gPi->iLhs->aux &&
            !sub_47e0(gPi->iLhs->aux, pi->pNext, gPi) && !sub_47e0(gPi->iRhs->aux, pi, gPi)) {
        pi->iLhs->aux = gPi->iRhs->aux;
        removeInstruction(gPi);
        gPi = pi;
    } else if (gPi->pNext->type == T_EX && gPi->pNext->iLhs->type == T_REG) {
        removeInstruction(gPi->pNext);
        removeInstruction(gPi);
    } else {
        return false;
    }
    return logOptimise(O_RED_EX_DEHL); /* 6fdb opt_msg[15] = "Redundant ex de,hl's" */
}

/**************************************************************************
 38 sub_24c0    ok++ (PMO)
 **************************************************************************/
bool sub_24c0() {
    inst_t const *l1;
    operand_t    *l2;
    int           reg;

    if (sub_3053()) {
        return true;
    }
    if (gPi->iLhs->type == T_REG &&
            (gPi->iRhs->type == T_INDEXED || gPi->iRhs->type == T_ADDRREF || gPi->iRhs->type == T_CONST)) {
        if (sub_29c3()) {
            return true;
        }
    } else if (gPi->iRhs->type == T_CONST && gPi->iLhs->type == T_INDEXED) {
        if ((reg = sub_46b1(gPi->iRhs, REG_A)) != -1) {
            gPi->iRhs->type = T_REG; /* m6: */
            gPi->iRhs->aux  = reg;
            return logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
        }
        sub_44b2(gPi->iLhs);
    } else if (gPi->iRhs->type == T_REG && (gPi->iLhs->type == T_INDEXED || gPi->iLhs->type == T_ADDRREF)) {
        if (operandsSame(gPi->iLhs, &regValues[gPi->iRhs->aux])) {
kill:
            removeInstruction(gPi);
            return logOptimise(O_RED_LD); /* 6fd5 opt_msg[12] = "Redundant loads/stores" */
        }
        sub_44b2(gPi->iLhs);
        if (regValues[gPi->iRhs->aux].type == T_INVALID) {
            sub_4544(gPi->iRhs->aux);
            regValues[gPi->iRhs->aux] = *gPi->iLhs;
        }

    } else if (gPi->iLhs->type == T_REG && gPi->iRhs->type == T_REG) { /* 2824 */
        if (gPi->iLhs->aux == gPi->iRhs->aux || operandsSame(&regValues[gPi->iLhs->aux], &regValues[gPi->iRhs->aux])) {
            goto kill;
        }

        if ((seq2 = gPi->pNext)->type == T_LD && operandsSame(seq2->iLhs, gPi->iRhs) &&
                operandsSame(seq2->iRhs, gPi->iLhs)) {
            removeInstruction(seq2);
            logOptimise(O_RED_LD); /* 6fd5 opt_msg[12] = "Redundant loads/stores" */
        }
        if (!sub_47e0(gPi->iLhs->aux, gPi->pNext, gPi)) {
            goto kill;
        }
        if (gPi->iLhs->aux == REG_E && gPi->iRhs->aux == REG_L && (seq2 = gPi->pNext)->type == T_LD &&
                seq2->iLhs->type == T_REG && seq2->iLhs->aux == REG_D && seq2->iRhs->type == T_REG &&
                seq2->iRhs->aux == REG_H && !sub_47e0(REG_HL, seq2->pNext, gPi)) {
            removeInstruction(gPi->pNext);
            gPi->type      = T_EX;
            gPi->opCode    = NULL;
            gPi->iLhs->aux = REG_DE;
            gPi->iRhs->aux = REG_HL;
            swapHLDE();
            return logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
        }
        sub_4544(gPi->iLhs->aux);
        regValues[gPi->iLhs->aux] = regValues[gPi->iRhs->aux];
    } else if (gPi->iLhs->type == T_REG) {
        sub_4544(gPi->iLhs->aux);
    }

    if (gPi->iLhs->type == T_REG && gPi->iRhs->type == T_REG) {
        if ((l1 = gPi->pAlt)->type == T_LD && (l2 = l1->iLhs)->type == T_REG && l2->aux == gPi->iRhs->aux &&
                !sub_47e0(l2->aux, gPi->pNext, l1) && sub_1369(l1->iRhs)) {
            sub_4544(l2->aux);
            regValues[l2->aux = gPi->iLhs->aux] = *l1->iRhs;
            goto kill;
        }
        if ((l1 = gPi->pNext)->type == T_LD && (l2 = l1->iRhs)->type == T_REG && l2->aux == gPi->iLhs->aux &&
                !sub_47e0(l2->aux, l1->pNext, gPi) && sub_1369(l1->iLhs)) {
            sub_4544(l2->aux);
            l2->aux = gPi->iRhs->aux;
            goto kill;
        }
    }
    return false; /* m26: */
}

/**************************************************************************
 39 sub_29c3    ok++ (PMO)
 Code is identical, except that the optimiser moves the code in the first if block
 to a different location. The jp conditions are changed to reflect this)
 **************************************************************************/
bool sub_29c3() {
    int l1;
    inst_t *pi1;

    if (operandsSame(gPi->iRhs, &regValues[gPi->iLhs->aux]) || !sub_47e0(gPi->iLhs->aux, gPi->pNext, gPi)) {
        /* OPTIMISER: this block is located differently */
        removeInstruction(gPi);
        return logOptimise(O_RED_LD); /* 6fd5 opt_msg[12] = "Redundant loads/stores" */
    }
    sub_4544(gPi->iLhs->aux);
    if (gPi->iLhs->aux <= REG_HL) {
        if ((l1 = sub_46b1(gPi->iRhs, gPi->iLhs->aux)) != -1) {
            /* code hikes gPi->iLhs->aux before test !!! */
            regValues[gPi->iLhs->aux] = *gPi->iRhs;
            gPi                       = gPi;
            gPi->iRhs->type           = T_REG;
            gPi->iRhs->aux            = l1;
            if (gPi->iLhs->aux >= REG_BC) {
                pi1             = allocInst(gPi);
                pi1->iLhs       = allocOperand();
                pi1->iRhs       = allocOperand();
                pi1->iLhs->type = T_REG;
                pi1->iRhs->type = T_REG;
                pi1->iLhs->aux  = regHiLoMap[gPi->iLhs->aux].hiReg;
                pi1->iRhs->aux  = regHiLoMap[gPi->iRhs->aux].hiReg;
                gPi->iLhs->aux  = regHiLoMap[gPi->iLhs->aux].loReg;
                gPi->iRhs->aux  = regHiLoMap[gPi->iRhs->aux].loReg;
                pi1->type       = T_LD;
                gPi             = pi1;
            }
            return logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
        }
    }
    regValues[gPi->iLhs->aux] = *gPi->iRhs;
    return false;
}

/**************************************************************************
 40 sub_2bdb    ok++ (PMO)
 **************************************************************************/
bool sub_2bdb() {
    register inst_t *pi;

    if (gPi->type == T_STK && gPi->iLhs->aux == REG_BC && gPi->aux == I_POP && !sub_47e0(REG_BC, gPi->pNext, gPi)) {
        for (pi = gPi->pNext; pi && (pi->type != T_JP && pi->type != T_CALL && pi->type != T_SYMBOL); pi = pi->pNext) {
            if (pi->type == T_STK)
                if (pi->aux != I_PUSH || pi->pNext->type != T_STK || pi->pNext->aux != I_POP) {
                    break;
                } else {
                    pi = pi->pNext;
                }
            if (pi->type == T_EX || (pi->type == T_LD && pi->iLhs->type == T_REG))
                if (pi->iLhs->aux == REG_SP) {
                    break;
                }
        }
        if (pi->type == T_STK && pi->aux == I_PUSH && pi->iLhs->aux == REG_HL && !sub_47e0(REG_HL, pi->pNext, pi)) {
            removeInstruction(gPi);
            pi->opCode     = NULL;
            pi->type       = T_EX;
            pi->aux        = 0;
            pi->iRhs       = pi->iLhs;
            pi->iLhs       = allocOperand();
            pi->iLhs->type = T_REGREF;
            pi->iLhs->aux  = REG_SP;
            return logOptimise(O_EX_SPHL); /* 6fd1 opt_msg[10] = "Ex (sp),hl'pi used" */
        }
    }
    return false;
}

/**************************************************************************
 41 sub_2d3b    ok++ (PMO)
 six of the basic blocks are in different positions but the code in the blocks
 is the same and the logic flow is maintained.
 **************************************************************************/
bool sub_2d3b() {
    register operand_t *po;

    switch (gPi->aux) {
        default:
            if (sub_4682(gPi->iLhs)) {
                gPi->aux = I_OR;
            } else if ((po = gPi->iLhs)->type != T_REG || po->aux != REG_A) {
                sub_4544(REG_AF);
                return false;
            } else {
                break;
            }
        /* fall through */
        case I_OR:
            po = gPi->iLhs; /* case 0xB0 */
            if (sub_4682(po)) {
                po->type = T_REG;
                po->aux  = REG_A;
            }
        /* fall through */
        case I_SUB:
caseP_SUB: /* common */
            if (gPi->iLhs->type == T_REG && gPi->iLhs->aux == REG_A) {
                if (gPi->aux == I_SUB) {
                    break;
                } else if (!sub_47e0(REG_F, gPi->pNext, gPi)) {
                    removeInstruction(gPi);
                    return logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
                }
                return false;
            }
            sub_4544(REG_AF);
            return false;
        case I_AND:
            po = gPi->iLhs;
            if (sub_4682(po)) {
                break;
            }
            if (po->type == T_CONST && !po->oPOperand && (po->oVal & 0xff) == 255 /* -1 */) {
                gPi->aux = I_OR;
                po->type = T_REG;
                po->aux  = REG_A;
            }
            goto caseP_SUB;

        case I_CP:
            return false;
    }

    if (!sub_47e0(REG_F, gPi->pNext, gPi) && sub_4682(&regValues[REG_A])) {
        removeInstruction(gPi);
        return logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
    }
    gPi->aux                   = I_XOR;
    gPi->opCode                = NULL;
    gPi->iLhs->type            = T_REG;
    gPi->iLhs->aux             = REG_A;
    regValues[REG_A].type      = T_CONST;
    regValues[REG_A].oPOperand = NULL;
    regValues[REG_A].oVal      = 0;
    return false;
}

/**************************************************************************
 42 sub_2ef8    ok++ (PMO)
 **************************************************************************/
bool sub_2ef8() {
    operand_t tmp;
    int       l2;

    if (gPi->pNext->type == T_INCDEC && operandsSame(gPi->iLhs, gPi->pNext->iLhs) && gPi->pNext->aux != gPi->aux) {
        removeInstruction(gPi->pNext);
    }

    else if (sub_47e0(REG_HL, gPi->pNext, gPi)) {
        tmp = regValues[REG_TRACKER];
        l2  = hlDelta;
        if (regValues[REG_HL].type != T_INVALID ||
                (regValues[REG_L].type == T_INDEXED && regValues[REG_H].type == T_INDEXED &&
                 regValues[REG_L].aux == regValues[REG_H].aux && regValues[REG_L].oVal + 1 == regValues[REG_H].oVal)) {
            if (regValues[REG_HL].type != T_INVALID) {
                tmp = regValues[REG_HL];
            } else {
                tmp = regValues[REG_L];
            }
            l2 = 0;
        }
        sub_4544(REG_HL);
        regValues[REG_TRACKER] = tmp;
        hlDelta                = l2;
        if (gPi->aux == SI_INC) {
            ++hlDelta;
        } else {
            --hlDelta;
        }
        return false;
    }
    removeInstruction(gPi);
    return logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
}

/**************************************************************************
 42a    sub_3053    ok++ (PMO)
 **************************************************************************/
bool sub_3053() {
    int l1;
    int l2;

    if (gPi->iLhs->type != T_REG || ((l1 = gPi->iLhs->aux) != REG_HL && l1 != REG_L)) {
        return false;
    }

    if (!operandsSame(gPi->iRhs, &regValues[REG_TRACKER])) {
        return false;
    }

    l2 = hlDelta;
    if (gPi->iLhs->aux == REG_L) {
        if ((seq2 = gPi->pNext)->type != T_LD || seq2->iLhs->type != T_REG || seq2->iLhs->aux != REG_H ||
                seq2->iRhs->oVal != gPi->iRhs->oVal + 1) {
            return false;
        }

        sub_4544(REG_HL);
        regValues[REG_L] = *gPi->iLhs;
        regValues[REG_H] = *seq2->iLhs;
        removeInstruction(seq2);
    } else {
        sub_4544(REG_HL);
        regValues[REG_HL] = *gPi->iLhs;
    }
    gPi = gPi->pAlt; /* m4: */
    removeInstruction(gPi->pNext);
    while (l2 != 0) {
        gPi             = allocInst(gPi);
        gPi->type       = T_INCDEC;
        gPi->iLhs       = allocOperand();
        gPi->iLhs->type = T_REG;
        gPi->iLhs->aux  = REG_HL;
        if (l2 < 0) {
            ++l2;
            gPi->aux = SI_INC;
        } else {
            --l2;
            gPi->aux = SI_DEC;
        }
    }
    return logOptimise(O_RED_LD); /* 6fd5 opt_msg[12] = "Redundant loads/stores" */
}

/**************************************************************************
 43 sub_31ee    ok++ (PMO)
 **************************************************************************/
void swapHLDE() {
    operand_t pi;

    pi                          = regValues[REG_HL];
    regValues[REG_HL]           = regValues[REG_DE];
    regValues[REG_DE]           = pi;

    pi                          = regValues[REG_H];
    regValues[REG_H]            = regValues[REG_D];
    regValues[REG_D]            = pi;

    pi                          = regValues[REG_L];
    regValues[REG_L]            = regValues[REG_E];
    regValues[REG_E]            = pi;
    regValues[REG_TRACKER].type = T_INVALID;
    ;
};

/**************************************************************************
 44 pr_psect    sub_328a    ok++ (PMO)
 **************************************************************************/
void pr_psect(int psect) {

    if (psect == cur_psect) {
        return;
    }
    printf("psect\t%s\n", psectNames[psect]);
    cur_psect = psect;
}

/**************************************************************************
 45 num_psect   sub_32bf    ok++ (PMO)
 **************************************************************************/
int num_psect(char const *fmt) {
    int l1;

    for (l1 = 0; l1 < 4; ++l1) {
        if (strcmp(fmt, psectNames[l1]) == 0) {
            return l1;
        }
    }
    pr_error("Unknown psect");
}

/**************************************************************************
 46 sub_3313    ok++ (PMO) apart from optimiser changes
 1) one code optimisation to remove jp, putting condition code on call
 2) code block moved with optimisation on shared code
 3) two further code block group moves
 Note the other code is identical and overall the logic is the same
 **************************************************************************/
typedef struct {
    int type; /* 01 s->str */
    int prec; /* 23 s->i_2 */
} op_t;

term_t *evalExpr() {
    char expectOp;
    op_t *pOp;
    term_t *pTerm;
    term_t termStack[30];
    op_t opStack[30];
    static term_t exprResult; /* so pointer to term can be returned */

    pTerm     = &termStack[30];
    pOp       = &opStack[29];
    pOp->type = T_MARKER;
    pOp->prec = 0;
    expectOp  = false;
    do {                                 /* REDUCE loop */
        for (;; tokType = get_token()) { /* SHIFT loop */
            if (tokType == T_STRING) {   /* in expressions "A" is treated as ascii value of A */
                if (strlen(yylval.pChar) != 1) {
                    pr_warning("Bad character const");
                }
                yylval.i = *yylval.pChar;
                tokType  = T_CONST;
            }
            if (T_FWD <= tokType && tokType <= T_CONST) { /* get the term, note two terms together is an error */
                if (expectOp) {
                    exp_err();
                }
                expectOp = true; /* flag as expect operator next */
                --pTerm;         /* where to push the term */
                switch (tokType) {
                    case T_SYMBOL: /* its reocatable */
                        pTerm->tPSym = yylval.pSym;
                        pTerm->val   = 0;
                        break;
                    case T_CONST: /* its a constant */
                        pTerm->val   = yylval.i;
                        pTerm->tPSym = NULL;
                        break;
                }
                continue;
            } else if (T_UPLUS <= tokType && tokType <= T_LASTOP) {           /* get the operator */
                if (!expectOp && (tokType == T_PLUS || tokType == T_MINUS)) { /* map unary +/- */
                    tokType  = tokType - 7;
                    yylval.i = 8; /* set its precedence */
                }
                if (tokType <= T_MARKER) {
                    if (expectOp) {
                        exp_err();
                    }
                } else {
                    if (!expectOp && tokType != T_OPAR) { /* binary op only when expecting op*/
                        exp_err();
                    }
                    if (pOp->prec >= yylval.i) { /* pick up precedence */
                        break;
                    }
                }
                if (tokType != T_CPAR) { /* not a closing ) */
                    --pOp;
                    pOp->type = tokType;   /* push its type */
                    if (tokType == T_OPAR) { /* if it was a ( then set prec to 1 */
                        yylval.i = 1;
                    } else {                                 /* OPTIMISER[2]: code moved to here */
                        expectOp = false;    /* now expecting a term */
                    }
                    pOp->prec = yylval.i; /* set the prec */ /* OPTIMISER[3] code block moved / shared */
                } else if (pOp->type ==
                           T_MARKER) /* ) with nothing on stack */ { /* OPTIMISER[4]: test code block moved */
                    break;
                }
            } else {
                break;
            }
        }

        /* REDUCE phase */
        if (pOp->type == T_OPAR) { /* check for matching () */
            if (tokType != T_CPAR) {
                exp_err();    /* ")" */
            }
            tokType  = get_token(); /* prime for next part */
            expectOp = 1;           /* assuming its a term */
        } else if (T_MARKER >= pOp->type) {
            uconv(pOp->type, pTerm);    /* calculate unary operator */
        } else {
            bconv(pOp->type, pTerm + 1, pTerm); /* calculate binary operator*/
            ++pTerm;
        }
    } while (++pOp != &opStack[30]); /* loop till end of operator stack */

    exprResult = *pTerm;
    if (&termStack[29] != pTerm) { /* oops outstanding term */
        exp_err();
    }
    return &exprResult;
}

/**************************************************************************
 47 exp_err     sub_32bf    ok++ (PMO)
 **************************************************************************/
void exp_err() {

    pr_error("Expression error");
}

/**************************************************************************
 48 sub_359e    ok++ (PMO)
 *  Unary Operators
 **************************************************************************/
void uconv(int op, term_t *lhs) {

    switch (op) {
        case T_UMINUS:
            lhs->val = -lhs->val;
            break;
        case T_NOT:
            lhs->val = ~lhs->val;
            break;
        case T_HI:
            lhs->val = (lhs->val >> 8) & 0xff;
            break;
        case T_LOW:
            lhs->val &= 0xff;
            break;
        case T_UPLUS:
            return;
        case T_MARKER:
            return;
        default:
            pr_error("uconv - bad op");
    }
    if (lhs->tPSym) {
        rel_err();
    }
    return;
}

/**************************************************************************
 49 sub_3630    ok++ (PMO)
 *  Binary Operators
 **************************************************************************/
void bconv(int op, term_t *lhs, term_t const *rhs) {

    switch (op) {
        case T_PLUS:
            if (lhs->tPSym && rhs->tPSym) {
                rel_err();
            }
            lhs->val += rhs->val;
            if (!lhs->tPSym) {
                lhs->tPSym = rhs->tPSym;
            }
            return;
        case T_MINUS:
            if (rhs->tPSym) {
                rel_err();
            }
            lhs->val -= rhs->val;
            return;
        case T_MUL:
            lhs->val *= rhs->val;
            break;
        case T_DIV:
            lhs->val /= rhs->val;
            break;
        case T_MOD:
            lhs->val %= rhs->val;
            break;
        case T_SHR:
            lhs->val >>= rhs->val;
            break;
        case T_SHL:
            lhs->val <<= rhs->val;
            break;
        case T_AND:
            lhs->val &= rhs->val;
            break;
        case T_OR:
            lhs->val |= rhs->val;
            break;
        case T_XOR:
            lhs->val ^= rhs->val;
            break;
        case T_EQ:
            lhs->val = lhs->val == rhs->val;
            break;
        case T_LE:
            lhs->val = rhs->val < lhs->val;
            break;
        case T_GE:
            lhs->val = lhs->val < rhs->val;
            break;
        case T_ULE:
            lhs->val = (uint16_t)rhs->val < (uint16_t)lhs->val;
            break;
        case T_UGE:
            lhs->val = (uint16_t)lhs->val < (uint16_t)rhs->val;
            break;
        default:
            pr_error("Bconv - bad op");
            break;
    }

    if (lhs->tPSym || rhs->tPSym) {
        rel_err();
    }
}

/**************************************************************************
 50 rel_err     ok++ (PMO)
 **************************************************************************/
void rel_err() {

    pr_warning("Relocation error");
}

/**************************************************************************
 51 sub_3856    ok++ (PMO) except for one jp z, that is optimised to jr z,
 **************************************************************************/
operand_t *evalOperand() {
    register operand_t *oper;

    oper = allocOperand();

    switch (tokType) {
        case T_EOL:
            oper->type = T_INVALID;
            break;

        case T_REG:
            if (expectCond && yylval.i == REG_C) { /* check if C is used in jp c context */
                tokType  = T_COND;                 /* convert to condition */
                yylval.i = COND_LLT;
            }
        /* fall through */
        case T_COND:
            oper->type = tokType; /* save type, value and prep for next token */
            oper->aux  = (uint8_t)yylval.i;
            tokType    = get_token();
            break;

        case T_OPAR:
            if ((tokType = get_token()) == T_REG) {
                if (yylval.i != REG_C && yylval.i < REG_BC) { /* only (C) and (BC) onwards are valid */
                    oper_err();
                }
                oper->type = T_REGREF;
                oper->aux  = (uint8_t)yylval.i;          /* save reg id */
                if ((tokType = get_token()) == T_CPAR) { /* simple (reg) so prep for next token */
                    tokType = get_token();
                    break;
                }
                if (oper->aux < REG_IX) { /* only IX & IY are allowed expressions */
                    oper_err();
                }
                oper->type = T_INDEXED; /* is IX/IY +/- d operand */
            } else {
                oper->type = T_ADDRREF;    /* is a (addr) operand */
            }

            oper->term = *evalExpr(); /* get the expression */

            if (tokType != T_CPAR) { /* should now be the closing ) */
                oper_err();
            }
            tokType = get_token(); /* prep for next token */
            /* IX & IY are only allowed displacements -128 to 127 */
            if (oper->type == T_INDEXED && (oper->oVal < -128 || oper->oVal >= 128)) {
                pr_warning("Index offset too large");
            }
            break;
        /*
            the operands below are only valid as the last operand on a line
            so there is no preping for the next token
        */
        case T_FWD:
        case T_BWD:
            oper->type = tokType;
            oper->oVal = yylval.i; /* save the label reference number */
            break;
        default:
            oper->type = T_CONST;
            oper->term = *evalExpr(); /* allow for more complex expression */
    }
    return oper;
}
/**************************************************************************
 52 oper_err    ok++ (PMO) but note jmpbuf not set
 **************************************************************************/
void oper_err() {

    pr_warning("Operand error");
    longjmp(jmpbuf, 1); /* BUG: jmpbuf not set */
}

/**************************************************************************
 53 sub_39a3    ok++ (PMO)
 **************************************************************************/
void getOperands(register inst_t *pi) {

    tokType    = get_token();
    cntOperand = 0;
    pi->iLhs   = NULL;
    pi->iRhs   = NULL;
    if (tokType == T_COMM) {
        oper_err();    /* cannot start with a comma */
    }
    if (tokType != T_EOL) { /* no operands */
        pi->iLhs = evalOperand();
        if (tokType == T_COMM) { /* if comma then 2nd operand */
            tokType  = get_token();
            pi->iRhs = evalOperand();
            ++cntOperand;
        }
        ++cntOperand;
    }
    clr_len_inbuf();
}

/**************************************************************************
 54 sub_3a15    ok++ (PMO)
 **************************************************************************/
void loadFunction() {
    sym_t  *ps;
    inst_t *l2;
    inst_t *l3;
    int     fpBase;
    register inst_t *pi;

    pi = root = (inst_t *)alloc_mem(sizeof(inst_t));
    pi->pNext = (inst_t *)alloc_mem(sizeof(inst_t));
    HEAP(pi->pNext);
    pi->pNext->pAlt = pi;
    pi              = pi->pNext;
    l2 = switchVectors = word_6fee = (inst_t *)alloc_mem(sizeof(inst_t));

    for (;; HEAP(pi->iRhs)) {
        tokType = get_token();
        HEAP(pi->iRhs);
        for (;; HEAP(pi->pNext)) {
            if (tokType == T_EOL) {
                clr_len_inbuf();
                break;
            }

            if (pi->type != T_INVALID) {
                pi = allocInst(pi);    /* m3: */
            }
            if (tokType == -1) {    /* m4: */
                word_6ffc = pi;
                word_6fee = l2;
                return;
            }
            pi->type = tokType; /* m5: */
            if (psect == SWDATA) {
                if (tokType == T_DEFW) { /* collect the switch table */
                    pi->opCode = yytext;
                    getOperands(pi);
                    l2->pNext       = pi;
                    pi              = pi->pAlt;
                    pi->pNext       = NULL;
                    l2->pNext->pAlt = l2;
                    l2              = l2->pNext;
                    break;
                }
                psect = DATA; /* revert to normal data handling */
            }
            switch (pi->type = tokType) { /* m7: */
                case T_CONST:
                    if ((psect == DATA) || (psect == BSS)) {
                        pi->type = T_INVALID;
                        pr_psect(psect);
                        printf("%d:\n", yylval.i);
                    } else {
                        pi->aux = yylval.i; /* m10: */
                        l3      = pi->pAlt;
                        if (pi->pAlt->type == T_JP && l3->iLhs->type == T_FWD && l3->iLhs->oVal == pi->aux) {
                            removeInstruction(l3);
                        }
                    }
                    tokType = get_token(); /* m11: */
                    if (tokType != T_COLN) {
                        pr_error("Bad temp label");
                    }
                    break;

                case T_SYMBOL:
                    ps      = yylval.pSym;
                    pi->aux = 0;
                    tokType = get_token();
                    if (tokType == T_EQU) {
                        if (ps->label[0] != 'f') { /* compiler generated equ names begin with f */
                            pr_error("Unknown EQU");
                        }

                        pi->type = T_INVALID;
                        tokType  = get_token();
                        pi->iLhs = evalOperand();
                        /* check is constant with no unresolved symbol ref */
                        if (pi->iLhs->type != T_CONST || pi->iLhs->oPSym) {
                            pr_error("Bad arg to EQU");
                        }

                        fpBase    = pi->iLhs->oVal; /* the frame pointer offset to lowest local (will be 0 or -ve) */

                        word_6ffc = pi;
                        word_6fee = l2;
                        pi        = root;

                        do { /* update any references to the frame size */
                            if (pi->iRhs && pi->iRhs->type == T_CONST && pi->iRhs->oPSym == ps) {
                                pi->iRhs->oVal += fpBase;
                                pi->iRhs->oPSym = NULL;
                            }
                            if (pi->iLhs && pi->iLhs->type == T_CONST && pi->iLhs->oPSym == ps) {
                                pi->iLhs->oVal += fpBase;
                                pi->iLhs->oPSym = NULL;
                            }
                        } while (pi = pi->pNext);
                        return;
                    }
                    pi->iPSym   = ps;

                    ps->p.pInst = pi;

                    pi->aux     = INT_MAX;
                    if (psect == DATA && ps->label[0] == 'S') { /* compiler generated switch tables start with S */
                        psect           = SWDATA;
                        l2->pNext       = pi;
                        pi              = pi->pAlt;
                        pi->pNext       = NULL;
                        l2->pNext->pAlt = l2;
                        l2              = l2->pNext;
                    }
                    if (psect == DATA || psect == BSS) {
                        pi->type    = T_INVALID;
                        pi->iLhs    = NULL;
                        ps->p.pInst = NULL;
                        pr_psect(psect);
                        printf("%s:\n", ps->label);
                    } else if (ps->label[0] == '_') { /* external name */
                        name_fun = ps->label;
                    }

                    if (tokType == T_COLN) {
                        break;
                    }
                    continue; /* inner loop */

                case 255: /* -1 */
                    pi->type  = T_INVALID;
                    word_6ffc = pi;
                    return;

                case T_DEFW:
                case T_DEFB:
                    if (psect == TEXT) {
                        goto case_default;
                    }
                case T_DEFM:
                case T_DEFS:
                case T_DEFF:
                    if (psect == TEXT) {
                        pr_error("DEF[BMSF] in text psect");
                    }
                    pr_psect(psect);
                /* fall through */
                case T_GLB:
                    printf("%s\t%s\n", yytext, ptr_token());
                    pi->type = T_INVALID;
                    break;

                case T_PSCT:
                    psect    = num_psect(ptr_token()); /* m30: */
                    pi->type = T_INVALID;
                    break;

                case T_JR:
                    pi->type = T_JP; /* convert to jp so it is safe to move code */
                    yytext   = "jp";
                /* fall through */
                default:
case_default:
                    pi->opCode = yytext;
                    pi->aux    = yylval.i;
                    if (tokType == T_JP || tokType == T_CALL) { /* set if can have conditional */
                        expectCond = true;
                    } else {
                        expectCond = false;
                    }
                    getOperands(pi);
                    if ((pi->type == T_JP) || (pi->type == T_CALL)) {
                        if (pi->iLhs->type == T_COND) { /* if cond then hoist condition and remove lhs */
                            pi->aux  = pi->iLhs->aux;
                            pi->iLhs = pi->iRhs;
                            pi->iRhs = NULL;
                        }
                    }
                    if (pi->type == T_JP && pi->aux == 0 && pi->iLhs->type != T_REGREF &&
                            (l3 = pi->pAlt)->type == T_CONST && l3->pAlt->type == T_JP && l3->pAlt->aux == 0) {

                        while (l3 = l3->pAlt) {
                            if (l3->type == T_JP && l3->iLhs->type == T_FWD && l3->iLhs->oVal == pi->pAlt->aux) {
                                *l3->iLhs = *pi->iLhs;
                            } else if (l3->type == T_CONST && l3->aux == pi->pAlt->aux) {
                                break;
                            }
                        }
                        removeInstruction(pi->pAlt);
                        freeOperand(pi->iLhs);
                        pi->type   = T_INVALID;
                        pi->iLhs   = NULL;
                        pi->opCode = NULL;
                    }
                    break;
            }
            break; /* to outer loop */
        }
    }
}

/**************************************************************************
 55 sub_4000    ok++ (PMO)
 **************************************************************************/
bool sub_4000(register inst_t const *pi) {

    return pi->type == T_JP && pi->iLhs->oPSym && strcmp(pi->iLhs->oPSym->label, "cret") == 0;
}
