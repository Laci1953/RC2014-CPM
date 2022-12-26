/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

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

extern char* conditions[];
extern char* regs[];
extern operator_t operators[];
extern int ccSwap[];
extern char* psectNames[];
extern struct _s1 regHiLoMap[];
extern struct _s2 regHiLoValMap[];
extern operand_t lo1,lo2,lo3,lo4;
extern operand_t *plo1, *plo2, *plo3, *plo4;

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

/*
 *	Common code sequences
 */
#define PEEKCH() (charsLeft > 0 ? *ptr_inbuf : '\n')
#define GETCH()  (--charsLeft >= 0 ? *ptr_inbuf++ : get_line())

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
