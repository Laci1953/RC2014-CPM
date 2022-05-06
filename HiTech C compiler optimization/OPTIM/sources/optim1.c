/*
 * File optim1.c created 14.08.2021, last modified 11.11.2021.
 *
 * The optim1.c file is part of the restored optimization program
 * from the Hi-Tech C compiler v3.09 package.
 *
 * The C source code was RESTORED by disassembling the original executable
 * file OPTIM.COM from the Hi-Tech v3.09 compiler.
 *
 * This file is compiled by Hi-Tech C compiler v3.09 and the resulting
 * executable file performs all the functions provided. To compile, use
 * the following command:
 *
 *    cc -o optim1.c part21.c part31.c ctype1.c initvar1.c
 *
 * The created executable file almost completely matches the original image.
 *
 * Mark Ogden took part in the work on the restoration of the source code of
 * the optimization program in C language.
 * He assigned names to the variables, gave the structures an understandable
 * look, corrected some C functions to exclude labels in the function body,
 * suggested how to interpret and fix several obscure places in the decompiled
 * code, and found errors in the original optimization program. Mark Ogden's
 * contribution is very large. Without his participation, code restoration
 * would be much slower. Thanks to Mark Ogden for the effort and time spent
 * on this thankless event. In fact, he is a co-author of the recovered code. 
 * 
 * The OPTIM utility tries to perform 18 types of optimizations.
 *
 * OPTIM has 5 options not described in the manual:
 *    -l - Prints additional information on each pass;
 *    -n - Prints statistics: number of iterations and number of
 *         optimization types performed;
 *    -r - disables register load optimisation
 *    -f - use inline frame initialisation
 *    -s - Unbuffered stdout;
 *
 * Options are unknown to ordinary users and are not used when compiling a
 * program using optimization. These options are probably intended for compiler
 * support to find errors while performing optimization.
 *
 * Not a commercial goal of this laborious work is to popularize among potential
 * fans of 8-bit computers the old HI-TECH C compiler V3.09 (HI-TECH Software)
 * and extend its life, outside of the CP/M environment (Digital Research, Inc),
 * for full operation in a Unix-like operating system UZI-180 without using the
 * CP/M emulator.
 *
 *	Andrey Nikitin & Mark Ogden 11.11.2021
 */

#include <stdio.h>
/* #include <assert.h> */
#include <setjmp.h>
/* #include <stdlib.h> */
#include <string.h>

#if defined(__STDC__) || defined(__STDC_VERSION__)
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#endif

#include "optim1.h"

#define	_Z	 0	/* 0000 0000 */
#define	_U	 1	/* 0000 0001 */
#define	_L	 2	/* 0000 0010 */
#define	_D	 4	/* 0000 0100 */
#define	_H	 8	/* 0000 1000 */
#define	_S	16	/* 0001 0000 */

#define ISALPHA(c)     ( ccClass[c] & (_U|_L) )
#define ISUPPER(c)     ( ccClass[c] & _U )
#define ISDIGIT(c)     ( ccClass[c] & _D )
#define ISXDIGIT(c)    ( ccClass[c] & _H )
#define ISSPACE(c)     ( ccClass[c] & _S )
#define ISALNUM(c)     ( ccClass[c] & (_U|_L|_D) )

/****************************************************************
 *	Initialized variables 
 ****************************************************************/
char * conditions[] = {
    0,          /* 63db	 0          */
    "nz",       /* 63dd	 1 COND_NZ  */
    "z",        /* 63df	 2 COND_Z   */
    "nc",       /* 63e1	 3 COND_LGE */
    "c",        /* 63e3	 4 COND_LLT */
    "po",       /* 63e5	 5 COND_PO  */
    "pe",       /* 63e7	 6 COND_PE  */
    "p",        /* 63e9	 7 COND_GE  */
    "m"         /* 63eb	 8 COND_LT  */
};

char * regs[] = {
    "b",	/* 63ed  0 REG_B   */
    "c",	/* 63ef  1 REG_C   */
    "d",	/* 63f1  2 REG_D   */
    "e",	/* 63f3  3 REG_E   */
    "h",	/* 63f5  4 REG_H   */
    "l",	/* 63f7  5 REG_L   */
    "",		/* 63f9  6 REG_F   */
    "a",	/* 63fb  7 REG_A   */
    "i",	/* 63fd  8 REG_I   */
    "r",	/* 63ff  9 REG_R  Memory Refresh Register */
    "bc",	/* 6401 10 REG_BC  */
    "de",	/* 6403 11 REG_DE  */
    "hl",	/* 6405 12 REG_HL  */
    "sp",	/* 6407 13 REG_SP  */
    "af",	/* 6409 14 REG_AF  */
    "af'",	/* 640b 15 REG_AF1 */
    "ix",	/* 640d 16 REG_IX  */
    "iy"	/* 640f 17 REG_IY  */
};

operator_t operators[] = {
    /* 6411 */
    { "&",      T_AND,     P_AND    },  /*   0   0   Bitwise AND */
    { "(",      T_OPAR,    P_OPAR   },  /*   1   1   Open parenthesis */
    { ")",      T_CPAR,    P_CPAR   },  /*   2   2   Closing parenthesis */
    { "*",      T_MUL,     P_MUL    },  /*   3   3   Multiplication */
    { "+",      T_PLUS,    P_ADD    },  /*   4   4   Addition */
    { ",",      T_COMM,    P_NA     },  /*   5   5   Comma */
    { "-",      T_MINUS,   P_SUB    },  /*   6   6   Subtraction */
    { ".and.",  T_AND,     P_AND    },  /*   7   7   Bitwise AND */
    { ".high.", T_HI,      P_HI     },  /*   8   8   Hi byte of operand */
    { ".low.",  T_LOW,     P_LOW    },  /*   9   9   Low byte of operand */
    { ".mod.",  T_MOD,     P_MOD    },  /*  10   a   Modulus */
    { ".not.",  T_NOT,     P_NOT    },  /*  11   b   Bitwise complement */
    { ".or.",   T_OR,      P_OR     },  /*  12   c   Bitwise or */
    { ".res.",  T_RES,     P_RES    },  /*  13   d   */
    { ".shl.",  T_SHL,     P_SHL    },  /*  14   e   Shift left */
    { ".shr.",  T_SHR,     P_SHR    },  /*  15   f   Shift right */
    { ".xor.",  T_XOR,     P_XOR    },  /*  16   10  Exclusive or */
    { "/",      T_DIV,     P_DIV    },  /*  17   11  Divison */
    { ":",      T_COLN,    P_NA     },  /*  18   12  Label separator */
    { "<",      T_GE,      P_GE     },  /*  19   13  Signed less than */
    { "=",      T_EQ,      P_EQU    },  /*  20   14  Equality */
    { ">",      T_LE,      P_LE     },  /*  21   15  Signed greater than */
    { "\\",     T_NOT,     P_NOT    },  /*  22   16  Bitwise complement */
    { "^",      T_OR,      P_OR     },  /*  23   17  Bitwise or */
    { "a",      T_REG,     REG_A    },  /*  24   18  Register */
    { "adc",    T_CARR,    I_ADC    },  /*  25   19 Add with Carry */
    { "add",    T_CADD,    I_ADD    },  /*  26   1a  Add */
    { "af",     T_REG,     REG_AF   },  /*  27   1b  Register */
    { "af'",    T_REG,     REG_AF1  },  /*  28   1c  Register */
    { "age",    T_COND,    COND_GE  },  /*  29   1d  Condition code Arithmetic greater or equal */
    { "alt",    T_COND,    COND_LT  },  /*  30   1e  Condition code Arithmetic less than */
    { "and",    T_3,       I_AND    },  /*  31   1f Logical AND */
    { "anz",    T_COND,    COND_NZ  },  /*  32   20  Condition code */
    { "az",     T_COND,    COND_Z   },  /*  33   21  Condition code */
    { "b",      T_REG,     REG_B    },  /*  34   22  Register b */
    { "bc",     T_REG,     REG_BC   },  /*  35   23  Register bc */
    { "bit",    T_BIT,     I_BIT    },  /*  36   24 Tests if the specified bit is set */
    { "c",      T_REG,     REG_C    },  /*  37   25  Register c */
    { "call",   T_CALL,    P_NA     },  /*  38   26  Call */
    { "ccf",    T_SIMPLE,  I_CCF    },  /*  39   27 Complement Carry Flag */
    { "cp",     T_3,       I_CP     },  /*  40   28 Compare */
    { "cpl",    T_SIMPLE,  I_CPL    },  /*  41   29 Complement */
    { "d",      T_REG,     REG_D    },  /*  42   2a  Register d */
    { "de",     T_REG,     REG_DE   },  /*  43   2b  Register de */
    { "dec",    T_INCDEC,  SI_DEC   },  /*  44   2c Decrement */
    { "defb",   T_DEFB,    P_NA     },  /*  45   2d  Definition byte */
    { "deff",   T_DEFF,    P_NA     },  /*  46   2e  Definition real */
    { "defl",   T_DEFL,    P_NA     },  /*  47   2f  Definition label */
    { "defm",   T_DEFM,    P_NA     },  /*  48   30  Definition a message */
    { "defs",   T_DEFS,    P_NA     },  /*  49   31  memory reservation */
    { "defw",   T_DEFW,    P_NA     },  /*  50   32  Definition word */
    { "di",     T_SIMPLE,  I_DI     },  /*  51   33 Disable Interrupts */
    { "djnz",   T_DJNZ,    I_DJNZ   },  /*  52   34  Dec., Jump Non-Zero */
    { "e",      T_REG,     REG_E    },  /*  53   35  Register */
    { "ei",     T_SIMPLE,  I_EI     },  /*  54   36 Enable Interrupts */
    { "equ",    T_EQU,     P_NA     },  /*  55   37  set value of symbol */
    { "ex",     T_EX,      P_NA     },  /*  56   38  Exchange */
    { "exx",    T_SIMPLE,  I_EXX    },  /*  57   39 Exchange */
    { "fge",    T_COND,    COND_GE  },  /*  58   3a  Condition code ge */
    { "flt",    T_COND,    COND_LT  },  /*  59   3b  Condition code lt */
    { "fnz",    T_COND,    COND_NZ  },  /*  60   3c  Condition code nz */
    { "fz",     T_COND,    COND_Z   },  /*  61   3d  Condition code z */
    { "global", T_GLB,     P_NA     },  /*  62   3e  Global */
    { "h",      T_REG,     REG_H    },  /*  63   3f  Register h */
    { "hl",     T_REG,     REG_HL   },  /*  64   40  Register hl */
    { "inc",    T_INCDEC,  SI_INC   },  /*  65   41 Increment */
    { "ix",     T_REG,     REG_IX   },  /*  66   42  Register ix */
    { "iy",     T_REG,     REG_IY   },  /*  67   43  Register iy */
    { "jp",     T_JP,      P_NA     },  /*  68   44  Absolute jumps to the address */
    { "jr",     T_JR,      I_JR     },  /*  69   45  Relative jumps to the address */
    { "l",      T_REG,     REG_L    },  /*  70   46  Register l */
    { "ld",     T_LD,      P_NA     },  /*  71   47  Load */
    { "ldir",   T_TWOBYTE, I_LDIR   },  /*  72   48 Load, Inc., Repeat */
    { "lge",    T_COND,    COND_LGE },  /*  73   49  Condition code Logical greater or equal */
    { "llt",    T_COND,    COND_LLT },  /*  74   4a  Condition code Logical less than */
    { "lnz",    T_COND,    COND_NZ  },  /*  75   4b  Condition code nz */
    { "lz",     T_COND,    COND_Z   },  /*  76   4c  Condition code z */
    { "m",      T_COND,    COND_LT  },  /*  77   4d  Condition code Arithmetic less than */
    { "nc",     T_COND,    COND_LGE },  /*  78   4e  Condition code lge */
    { "neg",    T_SIMPLE,  I_NEG    },  /*  79   4f Negates the accumulator */
    { "nop",    T_SIMPLE,  I_NOP    },  /*  80   50 No operation */
    { "nz",     T_COND,    COND_NZ  },  /*  81   51  Condition code */
    { "or",     T_3,       I_OR     },  /*  82   52 Logical inclusive OR */
    { "p",      T_COND,    COND_GE  },  /*  83   53  Condition code Arithmetic greater or equal */
    { "pop",    T_STK,     I_POP    },  /*  84   54  Stack operation pop */
    { "psect",  T_PSCT,    P_NA     },  /*  85   55  Psect */
    { "push",   T_STK,     I_PUSH   },  /*  86   56  Stack operation push */
    { "r",      T_REG,     REG_R    },  /*  87   57  Register r */
    { "res",    T_BIT,     I_RESB   },  /*  88   58 Reset bit */
    { "rl",     T_SHIFT,   I_RL     },  /*  89   59 */
    { "rla",    T_SIMPLE,  I_RLA    },  /*  90   5a */
    { "rlc",    T_SHIFT,   I_RLC    },  /*  91   5b */
    { "rlca",   T_SIMPLE,  I_RLCA   },  /*  92   5c */
    { "rld",    T_TWOBYTE, I_RLD    },  /*  93   5d */
    { "rr",     T_SHIFT,   I_RR     },  /*  94   5e */
    { "rra",    T_SIMPLE,  I_RRA    },  /*  95   5f */
    { "rrc",    T_SHIFT,   I_RRC    },  /*  96   60 */
    { "rrca",   T_SIMPLE,  I_RRCA   },  /*  97   61 */
    { "rrd",    T_TWOBYTE, I_RRD    },  /*  98   62 */
    { "rst",    T_RST,     I_RST    },  /*  99   63  Restart Commands */
    { "sbc",    T_CARR,    I_SBC    },  /* 100   64 Subtract with Carry */
    { "scf",    T_SIMPLE,  I_SCF    },  /* 101   65 Set Carry Flag */
    { "set",    T_BIT,     I_SETB   },  /* 102   66 Set bit */
    { "sla",    T_SHIFT,   I_SLA    },  /* 103   67 */
    { "sll",    T_SHIFT,   I_SLL    },  /* 104   68 */
    { "sp",     T_REG,     REG_SP   },  /* 105   69  Register sp */
    { "sra",    T_SHIFT,   I_SRA    },  /* 106   6a */
    { "srl",    T_SHIFT,   I_SRL    },  /* 107   6b */
    { "sub",    T_3,       I_SUB    },  /* 108   6c */
    { "xor",    T_3,       I_XOR    },  /* 109   6d */
    { "z",      T_COND,    COND_Z   }   /* 110   6e  Condition code */
};

#define NOPERATORS (sizeof(operators) / sizeof(operators[0]))

char * opt_msg[] = {				/* 68a3 */
    "Redundant labels",		/*  0	 0 */
    "Jumps to jumps",		/*  1	 1 */
    "Stack adjustments",	/*  2	 2 */
    "Temporary labels",		/*  3	 3 */
    "Unref'ed labels",		/*  4	 4 */
    "Unreachable code",		/*  5	 5 */
    "Jumps to .+1",		/*  6	 6 */
    "Skips over jumps",		/*  7	 7 */
    "Common code seq's",	/*  8	 8 */
    "Redundant exx's",		/*  9	 9 */
    "Ex (sp),hl's used",	/* 10	 a */
    "Redundant operations",	/* 11	 b */
    "Redundant loads/stores",	/* 12	 c */
    "Simplified addresses",	/* 13	 d */
    "Xor a's used",		/* 14	 e */
    "Redundant ex de,hl's",	/* 15	 f */
    "Code motions",		/* 16	10 */
    "Loops inverted"		/* 17	11 */
};

#define NOPTIM     (sizeof(opt_msg) / sizeof(opt_msg[0]))

int ccSwap[] = {				/* 68c7 */
    0, 2, 1, 4, 3, 6, 5, 8, 7
};

char * psectNames[] = {				/* 6a59 */
    "",		/*  0 */
    "text",	/*  1 */
    "data",	/*  2 */
    "bss",	/*  3 */
    "data"	/*  4 */
};

struct _s1 regHiLoMap[] = {
    { 0,     0     },				/* 6c50	REG_B  */
    { 0,     0     },				/* 6c52	REG_C  */
    { 0,     0     },				/* 6c54	REG_D  */
    { 0,     0     },				/* 6c56	REG_E  */
    { 0,     0     },				/* 6c58	REG_H  */
    { 0,     0     },				/* 6c5a	REG_L  */
    { 0,     0     },				/* 6c5c	REG_F  */
    { 0,     0     },				/* 6c5e	REG_A  */
    { 0,     0     },				/* 6c60	REG_I  */
    { 0,     0     },				/* 6c62	REG_R  */
    { REG_B, REG_C },				/* 6c64	REG_BC */
    { REG_D, REG_E },				/* 6c66	REG_DE */
    { REG_H, REG_L },				/* 6c68	REG_HL */
};

struct _s2 regHiLoValMap[] = {
    { &regValues[REG_BC], NULL	           },	/* 6c6a	REG_B   */
    { &regValues[REG_BC], NULL	           },	/* 6c6e REG_C   */
    { &regValues[REG_DE], NULL	           },	/* 6c72 REG_D   */
    { &regValues[REG_DE], NULL	           },	/* 6c76 REG_E   */
    { &regValues[REG_HL], NULL	           },	/* 6c7a REG_H   */
    { &regValues[REG_HL], NULL	           },	/* 6c7e REG_L   */
    { &regValues[REG_AF], NULL	           },	/* 6c82 REG_F   */
    { &regValues[REG_AF], NULL	           },	/* 6c86 REG_A   */
    { NULL,               NULL	           },	/* 6c8a REG_I   */
    { NULL,               NULL	           },	/* 6c8e REG_R   */
    { &regValues[REG_B], &regValues[REG_C] },	/* 6c92 REG_BC  */
    { &regValues[REG_D], &regValues[REG_E] },	/* 6c96 REG_DE  */
    { &regValues[REG_H], &regValues[REG_L] },	/* 6c9a REG_HL  */
    { NULL,               NULL	           },	/* 6c9e REG_SP  */
    { &regValues[REG_A], &regValues[REG_F] },	/* 6ca2 REG_AF  */
    { NULL,               NULL	           },	/* 6ca6 REG_AF1 */
    { NULL,               NULL	           },	/* 6caa REG_IX  */
    { NULL,               NULL	           }    /* 6cae REG_IY  */
};

operand_t lo1,lo2,lo3,lo4;
operand_t *plo1=&lo1, *plo2=&lo2, *plo3=&lo3, *plo4=&lo4;

/*
 *	Common code sequences
 */
#define PEEKCH() (charsLeft > 0 ? *ptr_inbuf : '\n')
#define GETCH()  (--charsLeft >= 0 ? *ptr_inbuf++ : get_line())

/**************************************************************************
 1	strtoi	sub_013d	ok++	Used in const_value
 *
 *	Converts a numeric string, in various bases, to a integer
 **************************************************************************/
int strtoi(char const *s, int base) {
    int val;   /* number */
    int digit; /* digit */

    val = 0;
    while (*s) {
        val *= base;
        if (ISDIGIT(*s)) {
            digit = *(s++) - '0';
        } else {
            if (ISXDIGIT(*s)) {
                digit = (ISUPPER(*s) ? (char)(*s | 0x20) : (char) * s) - ('a' - 10);
            }
        }
        /* #pragma warning(suppress : 6001) /* function only called after verified there is a digit */
        if (digit >= base) {
            pr_warning("Digit out of range");
            return 0;
        }
        val += digit;
    }
    return val;
}

/**************************************************************************
 2	ptr_token	sub_020f	ok++
 *
 *	Returns a pointer to a token
 **************************************************************************/
char *ptr_token() {
    register char *s;

#ifdef DEBUG
    pr_warning("ptr_token");
#endif

    s = yyline;
    while (PEEKCH() != 0 && PEEKCH() != '\n') {
        *s++ = GETCH();
    }
    *s        = '\0';
    charsLeft = 0;
    return yyline;
}

/**************************************************************************
 3	const_value	sub_0289	ok+
 *
 *	Rerurn integer value numeric string in various bases
 *	Generates correct code, but in a different sequence
 **************************************************************************/
int const_value() {
    register char *pc;
    uint8_t base;

    pc   = yyline + 1;
    base = 0;
    while (ISXDIGIT(PEEKCH())) {
        *pc++ = GETCH();
    }

    switch (PEEKCH()) { /* m17: */
        case 'H':
        case 'h':
            base = 16;
            /* #pragma warning(suppress : 6269) /* deference ignored - original code did the dreference */
            GETCH();
            break;

        case 'O':
        case 'o':
        case 'Q':
        case 'q':
            base = 8;
            /* #pragma warning(suppress : 6269) /* deference ignored - original code did the dreference */
            GETCH();
            break;
        default:
            if (pc[-1] == 'f' || pc[-1] == 'b') {
                yytype   = *--pc == 'f' ? T_FWD : T_BWD;
                *pc      = '\0';
                yylval.i = (int16_t)strtoi(yyline, 10);
                return yytype;
            }
            if (pc[-1] == 'B') {
                pc--;
                base = 2;
            }
            break;
    }
    *pc = '\0';
    if (base == 0) {
        base = 10;
    }
    yylval.i = (int16_t)strtoi(yyline, base);
    return T_CONST;
}

/**************************************************************************
 4  get_token   sub_03c7    ok++ (PMO)
 **************************************************************************/
int get_token() {
    int c;
    register char *pc;

#ifdef DEBUG
    pr_warning("get_token");
#endif

    for (;;) {
        pc = yyline;

        switch (c = GETCH()) {
            case -1:
                return -1;
            case '\t':
            case '\f':
            case ' ':
                continue;
            case 0:
            case ';':
                return T_EOL;
            case '\'': /* Single quote (apostrophe) */
                while (PEEKCH() && PEEKCH() != '\'') {
                    *pc++ = GETCH();
                }
                if (PEEKCH() == '\'')
                    /* #pragma warning(suppress : 6269) /* deference ignored - original code did the dreference */
                {
                    GETCH();
                } else {
                    pr_warning("Unterminated string");
                }
                *pc          = '\0';
                yylval.pChar = yyline;
                return T_STRING;

            case '.':
                *pc++ = c;
                while (ISALPHA(PEEKCH())) {
                    *pc++ = GETCH();
                }
                if (PEEKCH() == '.') {			/* m27: */
                    *pc++ = GETCH();
                }
                break;
            case ',':
                return T_COMM;				/* m31: */

            case ':':
                return T_COLN;				/* m32: */

            case '(':
                yylval.i = P_OPAR;			/* m33: */
                return T_OPAR;

            case ')':
                yylval.i = P_CPAR;			/* m34: */
                return T_CPAR;

            case '+':
                yylval.i = P_ADD;			/* m35: */
                return T_PLUS;

            case '-':
                yylval.i = P_SUB;			/* m36: */
                return T_MINUS;

            case '*':
                if (ptr_inbuf == inp_buf + 1) {
                    printf("%s\n", inp_buf);
                    charsLeft = 0;
                    continue;
                }
            default:
                *pc++ = c;
                if (ISALPHA(c)) {
                    while (ISALNUM(PEEKCH())) {		/* m41: */
                        *pc++ = GETCH();
                    }
                    if (pc == yyline + 2) {
                        if (PEEKCH() == '\'') {
                            *pc++ = GETCH();
                        }
                    }
                } else if (ISDIGIT(c)) {
                    return const_value();
                }
                break;
        }
        while (ISSPACE(PEEKCH()))
            /* #pragma warning(suppress : 6269) /* deference ignored - original code did the dreference */
        {
            GETCH();
        }

        *pc = '\0';
        if (yyline[0] == '_' || ISDIGIT(pc[-1]) || ((yytype = num_token(yyline)) == -1)) {
            if (ISALPHA(yyline[0])) {			/* m57: */
                yylval.pSym = lookupSym(yyline);
                return T_SYMBOL;
            }
            pr_warning("Lexical error");		/* m58: */
        } else {
            return yytype;				/* m59: */
        }
    }
}

/**************************************************************************
 5	get_line	sub_0758	ok++ (PMO)
 *	Optimiser eliminates a jump otherwise code is identical
 **************************************************************************/
/* PMO this code contains a bug. If strlen returns a 0 it accesses inp_buf[-1]
 * and could overwrite with a 0, this could occur if there is a '\0' in the input stream
 * it is also not clear whether a blank line i.e. one with just a '\n' should
 * return with a '\0' as it currently does or try another line
 *
 * Note needs fixing for linux as '\r' will not be removed on input
 *
 * The compiler can generate comment lines > 80 chars, this causes this function
 * to split the line which may cause the optimiser to get confused
 * a simple option would be to extend the size of inp_buf alternatively as the only
 * long lines appear to be comment ones. A better option would be to detect for a missing '\n'
 * and junk the rest of the line
 *
 * WARNING: The Hitech fgets contains a bug in that if line read has length >= size
 * it will write a 0 at buf[size], for optim this is the -l option flag
 *
 */
#ifdef CPM
int get_line() {

#ifdef DEBUG
    pr_warning("get_line");
#endif

    for (;;) {
        if (fgets(inp_buf, sizeof(inp_buf), stdin) == NULL) {
            return -1;
        }
        charsLeft = (int)strlen(ptr_inbuf = inp_buf);
        if (inp_buf[charsLeft - 1] == '\n') { /* If penultimate character is */
            inp_buf[charsLeft - 1] = '\0';    /* '\n' then replace it with 0 */
        }
        if (--charsLeft >= 0) {             /* got something */
            return *ptr_inbuf++;            /* otherwise, return first character of string */
        }
    }
}

#else
#define CPMEOF  0x1A

int get_line() {
    int c;

    charsLeft = 0;
    while ((c = getchar()) != '\n' && c != EOF && c != CPMEOF)
        if (charsLeft < sizeof(inp_buf) - 1 && c != '\r') {
            inp_buf[charsLeft++] = c;
        }
    inp_buf[charsLeft] = '\0';
    ptr_inbuf          = inp_buf;
    return charsLeft || (c != EOF && c != CPMEOF) ? *ptr_inbuf++ : EOF;
}
#endif

/**************************************************************************
 6	clr_len_inbuf	sub_0758  ok++	Used in: getOperands, loadFunction
 **************************************************************************/
void clr_len_inbuf() {

    charsLeft = 0;
}

/**************************************************************************
 7  main    sub_07b1    ok++ (PMO)
 *
 *  In the switch statement the location of the code blocks for 's' and
 *  default are swapped by the optimiser, otherwise the code is identical
 **************************************************************************/
int main(int argc, char **argv) {

    InitDynM();

    --argc, ++argv; /* would be cleaner as a for loop */
    while (0 < argc && argv[0][0] == '-') {
        switch (argv[0][1]) {
            case 'N':
            case 'n':
                key_n = true;
                break; /* Enables statistics printing */

            case 'L':
            case 'l':
                key_l = true;
                break; /* Enables printing of additional information */

            case 'R':
            case 'r':
                key_r = true;
                break; /* Disables register load optimisation */

            case 'F':
            case 'f':
                key_f = true;
                break; /* use inline frame initialisation */

            case 's':
                key_s = true;
                break; /* Unbuffered stdout */
            default:
                pr_error("Illegal switch %c", argv[0][1]);
                break;
        }
        ++argv, --argc;
    }
    if (argc > 0) {
        if (!freopen(*argv, "r", stdin)) {
            pr_error("Can't open %s", *argv);
        }

        if (argc > 1 && !freopen(*(argv + 1), "w", stdout)) {
            pr_error("Can't create %s", *(argv + 1));
        }
    }
    if (key_s) {
        setbuf(stdout, 0);    /* Unbuffered stdout (Turns off buffering) */
    }

    optimise();

    if (fclose(stdout) == -1) {
        pr_warning("Error closing output file");
        ++num_warn;
    }
    exit(num_warn != 0);
}

/**************************************************************************
 8	pr_error	sub-0941	ok++
 **************************************************************************/
#ifdef CPM
void pr_error(fmt, p2, p3) char * fmt;
{
    pr_message(fmt, p2, p3);
    fclose(stdout);
    exit(1);
}

/**************************************************************************
 9	pr_warning	sub_096f	ok++
 **************************************************************************/
void pr_warning(fmt, p2, p3) char *fmt;
{
    pr_message(fmt, p2, p3);
    ++num_warn;
}

/**************************************************************************
 10	pr_message	sub_0994	ok++
 **************************************************************************/
void pr_message(fmt, p2, p3) char * fmt;
{
    /* fprintf(stderr, "optim: "); */
    fprintf(stderr, fmt, p2, p3);
    fputc('\n', stderr);
}

#else

void pr_error(char const * fmt, ...) {
    va_list args;

    va_start(args, fmt);
    pr_message(fmt, args);
    va_end(args);
    exit(1);
}

void pr_warning(char const * fmt, ...) {
    va_list args;

    va_start(args, fmt);
    pr_message(fmt, args);
    va_end(args);
    ++num_warn;
}

void pr_message(char const * fmt, va_list args) {

    fprintf(stderr, "optim: ");
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
}

#endif

/**************************************************************************
 11	find_token	sub_09d0	ok++	Used in: num_token
 *
 *	Binary search is used
 * Minor difference after strcmp. the code sequence
 * original           new
 * ld (ix-1),l        ld a,l
 * ld a,l             ld (ix-1),a
**************************************************************************/
int find_token(register char const *str, operator_t const *p2, int p3) {
    char    cmp;
    uint8_t high, low, mid;

#ifdef DEBUG
    pr_warning("find_token");
#endif

    tableBase = p2;
    low       = 0;
    high      = p3 - 1;
    do {
        mid = (high + low) / 2;
        cmp = strcmp(str, tableBase[mid].str);
        if (cmp == 0) {
            yylval.i = tableBase[mid].aux;
            yytext   = tableBase[mid].str;
            return tableBase[mid].type;
        }
        if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    } while (high >= low);
    return -1;
}

/**************************************************************************
 12	num_token	sub_0a97	ok++	Used in
 **************************************************************************/
int num_token(char const *fmt) {

    return find_token(fmt, operators, NOPERATORS);
}

/**************************************************************************
 13	pr_token	sub_0ab2	ok++ (PMO)
 Only difference is the new code jumps to a location that cleans up
 stack after a call before jumping to cret. The direct jp cret also works
 **************************************************************************/
void pr_token(register inst_t const *pi) {
    operator_t const *po;

#ifdef DEBUG
    pr_warning("pr_token");
#endif

    if (pi->opCode) {
        printf("%s", pi->opCode);
        return;							/* m1: */
    }

    switch (pi->type) {						/* m2: */
        case T_JP:
            printf("jp");
            return;
        case T_CALL:
            printf("call");
            return;
        case T_RET:
            printf("ret");
            return;
    }

    po = operators + NOPERATORS; /* PMO should not be + 1 */
    do {
        if (--po < operators) {					/* m3: */
            pr_error("Can't find op");
            return;
        }
    } while ((pi->type != po->type) || (po->aux != pi->aux));	/* m8: */

    printf("%s", po->str);
    return;
}

/**************************************************************************
 14	freeOperand	sub_0b6a	ok++
 **************************************************************************/
void freeOperand(register operand_t *po) {

    if (!po) {
        return;
    }

/* --------------------------------------------
    ((list_t *)po)->pNext = freeOperandLst;
    freeOperandLst       = (list_t *)po;
------------------------------------------------*/
    myfree(po);
}

/**************************************************************************
 15	freeInst	sub_0b8b	ok++
 **************************************************************************/
void freeInst(register inst_t *pi) {

#ifdef DEBUG
    pr_warning("freeInst");
#endif

    pi->pAlt     = freeInstList;
    freeInstList = pi;
}

/**************************************************************************
 16	allocOperand	sub_0ba7	ok++
 *			Used in: sub_1795, sub_1c67, sub_1d94, sub_29c3,
 *				 sub_2bdb, sub_3053, evalOperand
 **************************************************************************/
operand_t *allocOperand() {

/*----------------------------------------------------
    register operand_t *pi;

    if (freeOperandLst) {
        pi = (operand_t *)freeOperandLst;
        HEAP(pi);
        freeOperandLst = ((list_t *)pi)->pNext;
        pi->type = pi->aux = 0;
        pi->oVal           = 0;
        pi->oPSym          = NULL;
        return pi;
    }
    return (operand_t *)alloc_mem(sizeof(operand_t));
----------------------------------------------------------*/
#ifdef DEBUG
    pr_warning("alloc_mem");
#endif

    return (operand_t *) my_alloc_mem();
}

/**************************************************************************
 17	allocInst	sub_0be2	ok++		Used in:
 **************************************************************************/
inst_t *allocInst(register inst_t *pi) {
    inst_t *l1;

#ifdef DEBUG
    pr_warning("allocInst");
#endif

    HEAP(pi->pNext);

    if ((l1 = freeInstList)) {
        HEAP(l1);
        freeInstList = l1->pAlt;
        l1->type = l1->aux = 0;
        l1->iLhs = l1->iRhs = NULL;
        l1->opCode          = NULL;
    } else {
        l1 = alloc_mem(sizeof(inst_t));
    }
    l1->pNext = pi->pNext;
    l1->pAlt  = pi;
    if (pi->pNext) {
        HEAP(pi->pNext);
        pi->pNext->pAlt = l1;
    }
    HEAP(l1);
#ifdef DEBUG
    pr_warning("allocInst returns %04x", l1);
#endif
    return pi->pNext = l1;
}

/**************************************************************************
 18	syntheticLabel	sub_0ca2	ok++		Used in: 
 **************************************************************************/
inst_t *syntheticLabel(register inst_t *pi) {

#ifdef DEBUG
    pr_warning("syntheticLabel");
#endif

    pi                = allocInst(pi);
    pi->iPSym         = allocBlankSym();
    pi->iSymId        = ++symbolId;
    pi->iPSym->tPInst = pi;
    pi->type          = T_SYMBOL;
    return pi;
}

/**************************************************************************
19  optimise    sub-0ce4    ok++ (PMO)
Apart from bug fix change, the code is the same
**************************************************************************/
void optimise() {
    int   l1;
    int   l2;
    int   iteration;
    char *l4;
    bool  l5;
    char *l6;

#ifdef DEBUG
    pr_warning("optimise");
#endif

    l6 = l4   = sbrk(0); /* PMO bug fix set l6 */
    iteration = 0;
    while (!feof(stdin)) { /* not eof */
        freeInstList    = NULL;
        freeOperandLst = NULL;
        resetHeap();
        loadFunction();

#ifdef PRINT_LISTS
 PrintPi();  
#endif

        freeHashtab();
        sub_0ed1();
        chkIXYUsage();
        sub_122f();

        l2 = 0;
        do {
            if (iteration < ++l2) {
                iteration = l2;
            }

            if (key_l) {
                printf("***** Pass %d\n", l2);
                sub_404d();
            }
            l5 = false;
            do {
                hasChanged = false; /* m5: */
                sub_15ad();
                l5 |= hasChanged;
            } while (hasChanged);
            do {
                hasChanged = false; /* m6: */
                sub_1ec1();
            } while (hasChanged);

        } while ((l5 |= hasChanged));

        sub_404d();

        if (l6 < (char *)sbrk(0)) {
            l6 = sbrk(0);
        }
    } /* end while */

    if (!key_n) {
        return;
    }
			/*    / This statement results in an error when */
    /* fclose(stdout); */	/* <-+  standard output is closed again in main */
			/*    \ program. It can be removed.             */

    fprintf(stderr, "%dK, %d iterations\n", ((int)(l6 - l4) + 0x3ff) / 0x400, iteration);

    for (l1 = 0; l1 < NOPTIM; l1++)
        if (optimiseCnters[l1] != 0) {
            fprintf(stderr, "%d %s\n", optimiseCnters[l1], opt_msg[l1]);
        }

    if (key_n)
	fprintf(stderr, "%d KB used from the upper 64KB RAM\r\n", (unsigned short)TotalSize() / 1024);
}

/**************************************************************************
 20	chkIXYUsage	sub_0e67	ok++	Used in: optimise
 **************************************************************************/
void chkIXYUsage() {
    register inst_t const *pi;
    uint8_t typeL;
    bool vL, vR;

#ifdef DEBUG
    pr_warning("chkIXYUsage");
#endif

    usesIXorIY = false;
    for (pi = root; pi; pi = pi->pNext) 
    {
        vL = IsValid((char*)pi->iLhs);
        vR = IsValid((char*)pi->iRhs);

        if (pi->type != T_SYMBOL &&
                ( (pi->iLhs && vL &&
		  /* pi->iLhs->type */ (typeL = GetByte((char*)(pi->iLhs)+OFF_type)) == (uint8_t)T_INDEXED) || 
		 (pi->iRhs && vR && 
			/* pi->iRhs->type */ GetByte((char*)(pi->iRhs)+OFF_type) == (uint8_t)T_INDEXED) ||
                 (pi->iLhs && vL && 
			/* pi->iLhs->type */ typeL == (uint8_t)T_REG &&
		  	/* pi->iLhs->aux */ GetByte((char*)(pi->iLhs)+OFF_aux) >= (uint8_t)REG_IX))) 
	{
            usesIXorIY = true;
            return;
        }
    }
}

/**************************************************************************
 21	sub_0ed1			ok++	used in optimise
 **************************************************************************/

    operand_t *po;
    bool       newLabel; /* Flag of the sub_0ca2 call */
    inst_t *pi1;
    uint8_t ty;
    bool valid_po;

void sub_0ed1() {
register     inst_t    *pi2;

#ifdef DEBUG
    pr_warning("sub_0ed1");
#endif

    for (pi1 = word_6ffc; pi1; pi1 = pi1->pAlt) 
    {
        if (pi1->type == T_CONST) 
	{
            newLabel = false;
            logOptimise(O_TMP_LAB); /* 6fc3 opt_msg[3] = "Temporary labels" */
            for (pi2 = pi1->pAlt; pi2; pi2 = pi2->pAlt) {
                if (pi2->type == T_CONST && pi2->aux == pi1->aux) 
                    break;

		po = pi2->iLhs;

		valid_po = IsValid((char*)po);

		if (valid_po)
		    ty = GetByte((char*)po+OFF_type);
		else
		    ty = 0x7F;

                if (valid_po && 
		     /* po->type */ ty == (uint8_t)T_FWD && 
		     /* po->oVal */ (int)GetWord((char*)po+OFF_oVal) == pi1->aux) 
		{
                    if (!newLabel) 
		    {
                        syntheticLabel(pi1);
                        newLabel = true;
                    }

                    /* po->type      = T_CONST; */
		    PutByte((char*)po+OFF_type, (uint8_t)T_CONST);

                    /* po->oPOperand = pi1->pNext->iLhs; */
		    PutWord((char*)po+OFF_oPOperand, (short)pi1->pNext->iLhs);

                    /* po->oVal      = 0; */
		    PutWord((char*)po+OFF_oVal, (short)0);
                }
            }
            if (pi1->aux < REG_BC) 
	    { /* m7: */
                if (!newLabel) 
                    syntheticLabel(pi1);

                for (pi2 = pi1->pNext; pi2; pi2 = pi2->pNext) 
		{

		    po = pi2->iLhs;

		    valid_po = IsValid((char*)po);

		    if (valid_po)
		        ty = GetByte((char*)po+OFF_type);
		    else
		        ty = 0x7F;
		    
                    if (valid_po && 
			/* po->type */ ty == (uint8_t)T_BWD && 
			/* po->oVal */ (int)GetWord((char*)po+OFF_oVal) == pi1->aux) 
		    {

                        /* po->type      = T_CONST; */
		        PutByte((char*)po+OFF_type, (uint8_t)T_CONST);

                        /* po->oPOperand = pi1->pNext->iLhs; */
		        PutWord((char*)po+OFF_oPOperand, (short)pi1->pNext->iLhs);

                        /* po->oVal      = 0; */
		        PutWord((char*)po+OFF_oVal, (short)0);

                        break;
                    }
                }
            }

            pi1 = pi1->pNext; /* m12: */
            removeInstr(pi1->pAlt);
        }
    }
}

/**************************************************************************
 22	sub_1071			ok++	used in sub_15ad
 **************************************************************************/
    inst_t *pi_1;
    inst_t *pi_2;
    inst_t *pi_3;
    sym_t * s_1071;

bool sub_1071(register inst_t *pi) {

#ifdef DEBUG
    pr_warning("sub_1071");
#endif

    if (pi->type != T_JP || pi->aux != 0 ||
	 /* pi->iLhs->type */ GetByte((char*)(pi->iLhs)+OFF_type) != (uint8_t)T_CONST) {
        return false;
    }

    /* pi_1 = pi->iLhs->oPSym->p.pInst; */
    s_1071 = (sym_t*)GetWord((char*)(pi->iLhs)+OFF_oPSym);

    if (!s_1071)
	pr_error("oPSym is NULL");

    pi_1 = s_1071->p.pInst;

    while (pi_1->type == T_SYMBOL) {
        pi_1 = pi_1->pAlt;
    }

    if (pi_1 == pi) {
        removeInstr(pi);
        logOptimise(O_JMP_TO_PLUS1); /* 6fc9 opt_msg[6] = "Jumps to .+1" */
        return false;
    }
    if (pi_1->type != T_JP || pi_1->aux != 0) {
        return false;
    }

    for (pi_3 = pi_1->pNext; pi_3->type && (pi_3->type != T_JP || pi_3->aux != 0); pi_3 = pi_3->pNext)
        ;

    if (pi_3 == pi_1->pNext || pi_3->type == T_INVALID || pi_3 == pi) {
        return false;
    }

    pi->pNext->pAlt  = pi_3;
    pi_1->pNext->pAlt = pi;

    pi_3->pNext->pAlt = pi_1;

    pi_2              = pi_1->pNext;
    pi_1->pNext       = pi_3->pNext;
    pi_3->pNext       = pi->pNext;
    pi->pNext        = pi_2;
    logOptimise(O_CODE_MOTIONS); /* 6fdd opt_msg[16] = "Code motions" */
    removeInstr(pi);
    return logOptimise(O_JMP_TO_PLUS1); /* 6fc9 opt_msg[6] = "Jumps to .+1" */
}

/**************************************************************************
 23	sub_122f	ok++	Used in: optimise
 **************************************************************************/
    operand_t *po_122f;
    sym_t* ps_122f;

void sub_122f() {
register     inst_t *pi_122f;
#ifdef DEBUG
    pr_warning("sub_122f");
#endif

    for (pi_122f = root->pNext; pi_122f; pi_122f = pi_122f->pNext) /* set initial values for symbols */
        if (pi_122f->type == T_SYMBOL)
            if (pi_122f->iPSym->label[0] == '_') { /* check for public name */
                pi_122f->aux = INT_MAX;
            } else {
                pi_122f->aux = 0;
            }

    for (pi_122f = root->pNext; pi_122f; pi_122f = pi_122f->pNext) { /* update reference counts */
        if (pi_122f->type == T_JP || pi_122f->type == T_DJNZ) {
            if ((po_122f = pi_122f->iLhs) &&
		 /* po_122f->type */ (uint8_t)GetByte((char*)po_122f+OFF_type) == (uint8_t)T_CONST &&
		 /* po_122f->oPSym */ (ps_122f = (sym_t*)GetWord((char*)po_122f+OFF_oPSym)) && 
		 /* po_122f->oPSym->p.pInst */ ps_122f->p.pInst ) {

                /* po_122f->oPSym->p.pInst->aux++; */
		ps_122f->p.pInst->aux++;
            }
        }
    }
    for (pi_122f = switchVectors; pi_122f; pi_122f = pi_122f->pNext) { /* do the same for the jump tables */
        if (pi_122f->type == T_DEFW) {
            if (pi_122f->iLhs &&
		 (po_122f = pi_122f->iLhs) &&
		 /* po_122f->type */  (uint8_t)GetByte((char*)po_122f+OFF_type) == (uint8_t)T_CONST &&
		 /* po_122f->oPSym */ (ps_122f = (sym_t*)GetWord((char*)po_122f+OFF_oPSym)) && 
		 /* po_122f->oPSym->p.pInst */ ps_122f->p.pInst) {

                /* po_122f->oPSym->p.pInst->aux++; */
		ps_122f->p.pInst->aux++;
            }
        }
    }
}

/**************************************************************************
 24	sub_1369	ok++ (PMO)
 **************************************************************************/
bool sub_1369(register operand_t const *pi_arg) {
/* pi_arg is checked !!! */
#ifdef DEBUG
    pr_warning("sub_1369");
#endif

    if (pi_arg && IsValid((char*)pi_arg))
    {
        ReadMem((char*)&lo1, (char*)pi_arg, sizeof(operand_t));

        return (plo1->type == (uint8_t)T_CONST) || 
		(plo1->type == (uint8_t)T_INDEXED) ||
		 (plo1->type == (uint8_t)T_REGREF && plo1->aux == (uint8_t)REG_HL);
    }
    else
	return false;
}

/**************************************************************************
 25	sub_1397	ok++ (PMO)
 **************************************************************************/
/* note there are occasions when pi is accessed after this is called so
 * freeInst has to preserve at least pi->pNext
 */
void removeInstr(register inst_t *pi) {
    sym_t *s;

#ifdef DEBUG
    pr_warning("removeInstr pi=%04x", pi);
#endif

    if (pi->type == T_JP && 
	/* pi->iLhs->type */ GetByte((char*)(pi->iLhs)+OFF_type) == (uint8_t)T_CONST &&
	/* pi->iLhs->oPSym */ (s = (sym_t*)GetWord((char*)(pi->iLhs)+OFF_oPSym)) ) {
        removeLabelRef(/* pi->iLhs->oPSym */ s );
    }

    pi->pAlt->pNext = pi->pNext;
    pi->pNext->pAlt = pi->pAlt;
    if (pi->type != T_SYMBOL) {
        freeOperand(pi->iLhs);
        freeOperand(pi->iRhs);
    }
    freeInst(pi);
}

/**************************************************************************
 26	sub_140b	ok++
 **************************************************************************/
inst_t *getNxtRealInst(register inst_t *pi) {

#ifdef DEBUG
    pr_warning("getNxtRealInst");
#endif

    while (pi->type == T_SYMBOL) {
        pi = pi->pNext;
    }
    return pi;
}

/**************************************************************************
 27	sub_142f	ok++
 **************************************************************************/
bool operandsSame(register operand_t const *po1_arg, operand_t const *po2_arg) {
    operand_t *po1, *po2;

#ifdef DEBUG
    pr_warning("operandsSame");
#endif

    if (!po1_arg && !po2_arg) {
        return true;
    }

    if (!po1_arg || !po2_arg) {
        return false;
    }

    if (!IsValid((char*)po1_arg))
	po1 = po1_arg;
    else
    {
        ReadMem((char*)&lo1, (char*)po1_arg, sizeof(operand_t));
	po1 = plo1;
    }

    if (!IsValid((char*)po2_arg))
	po2 = po2_arg;
    else
    {
        ReadMem((char*)&lo2, (char*)po2_arg, sizeof(operand_t));
	po2 = plo2;
    }

    if (po1->type != po2->type || po1->aux != po2->aux || po1->type == (uint8_t)T_INVALID) {
        return false;
    }

    return po1->oPSym == po2->oPSym && po1->oVal == po2->oVal;
}

/**************************************************************************
 28	instrSame	sub_14ac	ok++
 **************************************************************************/
bool instrSame(register inst_t const *pi1, inst_t const *pi2) {

#ifdef DEBUG
    pr_warning("instrSame");
#endif

    if (pi1->type == T_INVALID || pi1->type == T_SYMBOL || pi2->type == T_INVALID || pi2->type == T_SYMBOL) {
        return false;
    }

    if (pi1->type != pi2->type || pi1->aux != pi2->aux) {
        return false;
    }

    return operandsSame(pi1->iLhs, pi2->iLhs) && operandsSame(pi1->iRhs, pi2->iRhs);
}

/**************************************************************************
 29	sub_153d	ok++
 **************************************************************************/
void removeLabelRef(register sym_t *ps) {
    inst_t *pi;

#ifdef DEBUG
    pr_warning("removeLabelRef");
#endif

    if (!(pi = ps->p.pInst)) {
        return;
    }
    if (pi->aux == 0) {
        pr_error("Refc == 0");
    }
    if (--pi->aux != 0) {
        return;
    }
    removeInstr(pi);
    ps->p.pInst = NULL;
    freeSymbol(ps);
    logOptimise(O_UNREF_LAB); /* 6fc5 opt_msg[4] = "Unref'ed labels" */
}

/**************************************************************************
 30	sub_15ad	ok++	(PMO) Used in optimize
 *
 * code optimised over original as noted below, otherwise identical
 * 1) for loop iteration expression moved
 * 2) code to increment an optiminse counter & set hasChanged = true shared
 * 3) sub_1d94() == 0, test removed as code falls through in either case
 **************************************************************************/
#if 1

    inst_t *pi_15ad;
    sym_t *s_15ad;
    uint8_t t_15ad;

void sub_15ad() {

#ifdef DEBUG
    pr_warning("sub_15ad");
#endif

    for (gPi = root; gPi; gPi = gPi->pNext) 
    {
	if (IsValid((char*)gPi->iLhs))
	{
            s_15ad = (sym_t*)GetWord((char*)(gPi->iLhs)+OFF_oPSym);
	    t_15ad = GetByte((char*)(gPi->iLhs)+OFF_type);
	}
	else
	{
	    s_15ad = NULL;
	    t_15ad = 0x7F;
	}

	if (s_15ad)
            pi_15ad = s_15ad->p.pInst;
	else
	    pi_15ad = NULL;
            
        if (!sub_1795() && gPi->type == T_JP && !sub_1aec() && !sub_1b86()) 
	{
            if (/* gPi->iLhs->type */ t_15ad == (uint8_t)T_CONST &&
	 	pi_15ad &&
	        /* gPi->iLhs->oPSym->p.pInst */ pi_15ad == gPi->pNext) 
	    {
                removeInstr(gPi);
                logOptimise(O_JMP_TO_PLUS1); /* 6fc9 opt_msg[6] = "Jumps to .+1" */
            } 
	    else 
	    {
                if (gPi->aux == 0) 
		{ /* 1648 / 164B */
                    while (gPi->pNext->type != T_INVALID && gPi->pNext->type != T_SYMBOL) 
		    {
                        removeInstr(gPi->pNext);
                        logOptimise(O_UNREACH_LAB); /* 6fc7 opt_msg[5] = "Unreachable code" */
                    }

                    if (/* gPi->iLhs->type */ t_15ad != (uint8_t)T_REGREF &&
		        /* gPi->iLhs->oPSym->p.pInst */ pi_15ad &&
		        sub_1071(gPi)) 
                        continue;
                }

                if (/* gPi->iLhs->type */ t_15ad == T_CONST &&
		   (gPi->pNext->type == T_CALL || gPi->pNext->type == T_JP) &&
                    gPi->pNext->aux == 0 &&
		    pi_15ad &&
		    /* gPi->iLhs->oPSym->p.pInst */ pi_15ad == gPi->pNext->pNext) 
		{
                    gPi->pNext->aux = ccSwap[gPi->aux]; /* swap condition code */
                    removeInstr(gPi);
                    logOptimise(O_SKIP_OVER_JMP); /* 6fcb opt_msg[7] = "Skips over jumps" */
                } 
		else if (sub_1c67() || !sub_1d94())
                    ;
            }
        }
    }
}

#else

void sub_15ad() {
    register inst_t * pi;

#ifdef DDEBUG
    printf("\n\tsub_15ad\n");
#endif
    gPi = root;
    while(gPi != 0) {
	if(sub_1795() == 0) {
	    if(gPi->type == T_JP) {
		if(sub_1aec() == 0) {
		    if(sub_1b86() == 0) {
			if(gPi->iLhs->type           !=    T_CONST) goto m5;
			if(gPi->iLhs->oPSym->p.pInst != gPi->pNext) goto m5;
			removeInstr(gPi);
			optimiseCnters[6]++;	/* 6fc9 opt_msg[6] = "Jumps to .+1" */
			hasChanged = true;
		    }
		}
	    }
	}
m2:	gPi = gPi->pNext;
    }
    return;

m5: if(gPi->aux == 0) {
	while((gPi->pNext->type != 0)					/* m8: */
	&&    (gPi->pNext->type != T_SYMBOL)) {
	    pi = gPi->pNext;						/* m7: */
	    removeInstr(pi);
	    optimiseCnters[5]++; /* 6fc7 opt_msg[5] = "Unreachable code" */
	    hasChanged = true;
	}
	if(gPi->iLhs->type != T_REGREF) {				/* m9: */

	    if(gPi->iLhs->oPSym->p.pInst != 0) {

		if(sub_1071(gPi) != 0) goto m2;
	    }
	}
    }
    if(gPi->iLhs->type == T_CONST) {					/* m10: */
	if(gPi->pNext->type == T_CALL) goto m12;
	if(gPi->pNext->type == T_JP) {
m12:	    if(gPi->pNext->aux == 0) {
		if(gPi->iLhs->oPSym->p.pInst == gPi->pNext->pNext) {
		    
		    gPi->pNext->aux = ccSwap[gPi->aux]; /* swap condition code */
		    
		    removeInstr(gPi);
		    optimiseCnters[7]++; /* 6fcb opt_msg[7] = "Skips over jumps" */
		    hasChanged = true;
		    goto m2;
		}
	    }
	}
    }
    if(sub_1c67() != 0) goto m2;					/* m13: */
    if(sub_1d94() == 0) goto m2;
    goto m2;
}

#endif

