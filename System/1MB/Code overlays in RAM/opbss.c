#include <stdio.h>
#include <setjmp.h>
#include "optim1.h"

/****************************************************************
 *      Descriptions of uninitialized variables and arrays
 ****************************************************************/

char               yyline[80];                  /* 6f00 Working buffer */
int                charsLeft;                   /* 6f50 Length string in input buffer */
char             * ptr_inbuf;                   /* 6f52 Pointer to input buffer  */
int                yytype;                      /* 6f54 */
char               inp_buf[80];                 /* 6f56 Input buffer */
bool               key_l;                       /* 6fa6 Prints additional information on each pass */
#ifdef STATISTICS
bool               key_n;                       /* 6fa7 Prints statistics */
#endif
bool               key_f;                       /* 6fa8 The action is not clear */
bool               key_s;                       /* 6fa9 Key_s = 1 Unbuffered stdout */
int                num_warn;                    /* 6faa Number of warnings */
bool               key_r;                       /* 6fac */
operator_t const * tableBase;                   /* 6fad Pointer to keyword[] */
char             * yytext;                      /* 6faf */
YYSTYPE            yylval;                      /* 6fb1 Contains different types of data */
int                symbolId;                    /* 6fb3 */
sym_t            * gPs;                         /* 6fb5 */
inst_t           * gPi;                         /* 6fb7 */
bool               hasChanged;                  /* 6fbb */
bool               usesIXorIY;                  /* 6fbc */
#ifdef STATISTICS
int                optimiseCnters[NOPTIM];  	/* 6fbd Array of counters types of optimizations */
#endif
list_t           * freeOperandLst;              /* 6fe1 ptr to struct size 6 */
inst_t           * seq1;                        /* 6fe3 */
inst_t           * seq2;                        /* 6fe5 */
inst_t           * freeInstList;                /* 6fe7 */
char               psect;                       /* 6fe9 Program section */
int                cur_psect;                   /* 6fea Current program section */
int                expectCond;                  /* 6fec */
inst_t           * word_6fee;                   /* 6fee */
int                tokType;                     /* 6ff0 Token value */
inst_t           * switchVectors; 		/* word_6ff2; */      
inst_t           * root;                        /* 6ff8 */
int                cntOperand;                  /* 6ffa Used only in sub_39a3 */
inst_t           * word_6ffc;                   /* 6ffc */
jmp_buf            jmpbuf;                      /* 6ffe [8] */
int                hlDelta; 			/* word_7006;*/     
operand_t          regValues[19];               /* 7008 */
char             * alloct;                      /* 707a is the top of the current region */
char             * name_fun;                    /* 707c Function name */
list_t           * freeItemList;                /* 7080 */
char             * allocs;                      /* 7082 is the current next allocated heap location */
char             * programBreak;                /* 7084 ok */
term_t 		   exprResult; 			/* so pointer to term can be returned */
sym_t            * hashtab[HASHSIZE];           /* 7086 [622] */

operand_t lo1;
operand_t lo2;
operand_t lo3;
operand_t lo4;

