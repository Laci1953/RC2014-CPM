#include <stdio.h>
#include <setjmp.h>

#include "optim1.h"

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

#ifdef STATISTICS
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
#endif

int regTestMasks[] = {
    0x01,                              		/* 6cb2 REG_B   */
    0x02,                              		/* 6cb4 REG_C   */
    0x04,                              		/* 6cb6 REG_D   */
    0x08,                              		/* 6cb8 REG_E   */
    0x10,                              		/* 6cba REG_H   */
    0x20,                              		/* 6cbc REG_L   */
    0x40,                              		/* 6cbe REG_F   */
    0x80,                              		/* 6cc0 REG_A   */
    0x00,                              		/* 6cc2 REG_I   */
    0x00,                              		/* 6cc4 REG_R   */
    0x03,                              		/* 6cc6 REG_BC  */
    0x0C,                              		/* 6cc8 REG_DE  */
    0x30,                              		/* 6cca REG_HL  */
    0x00,                              		/* 6ccc REG_SP  */
    0xC0,                              		/* 6cce REG_AF  */
    0x00,                              		/* 6cd0 REG_AF1 */
    0x100,                             		/* 6cd2 REG_IX  */
    0x200,                             		/* 6cd4 REG_IY  */
};

operand_t *plo1=&lo1, *plo2=&lo2, *plo3=&lo3, *plo4=&lo4;

