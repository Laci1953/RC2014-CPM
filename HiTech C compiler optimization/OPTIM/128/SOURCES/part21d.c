/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "optim1.h"

/**************************************************************************
 42 sub_2ef8    ok++
 **************************************************************************/
bool sub_2ef8() {
    operand_t tmp;
    int       l2;

#ifdef DEBUG
    pr_warning("sub_2ef8");
#endif

    if (gPi->pNext->type == T_INCDEC && operandsSame(gPi->iLhs, gPi->pNext->iLhs) && gPi->pNext->aux != gPi->aux) {
        removeInstr(gPi->pNext);
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
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_TRACKER); 
#endif
        hlDelta                = l2;
        if (gPi->aux == SI_INC) {
            ++hlDelta;
        } else {
            --hlDelta;
        }
        return false;
    }
    removeInstr(gPi);
    return logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
}

/**************************************************************************
 42a    sub_3053    ok+-
 **************************************************************************/

    operand_t *pogL, *pogR, *posL, *posR;
    uint8_t auxV;
    int l2_3053;

bool sub_3053() {

#ifdef DEBUG
    pr_warning("sub_3053");
#endif

    pogL = gPi->iLhs;
    pogR = gPi->iRhs;

    auxV = GetByte((char*)pogL+OFF_aux);

    if (/* gPi->iLhs->type */ GetByte((char*)pogL+OFF_type) != (uint8_t)T_REG ||
	(/* l1 = gPi->iLhs->aux */ auxV != REG_HL && auxV != (uint8_t)REG_L) ) {
        return false;
    }

    if (pogR && !operandsSame(/* gPi->iRhs */ pogR, &regValues[REG_TRACKER])) {
        return false;
    }

    l2_3053 = hlDelta;

    if (/* gPi->iLhs->aux */ auxV == (uint8_t)REG_L) {

        seq2 = gPi->pNext;

	posL = seq2->iLhs;
	posR = seq2->iRhs;

        if (seq2->type != T_LD ||
	    ( posL && (/* seq2->iLhs->type */ GetByte((char*)(posL)+OFF_type) != (uint8_t)T_REG) ) ||
	    ( posL && (/* seq2->iLhs->aux */ GetByte((char*)(posL)+OFF_aux) != (uint8_t)REG_H) ) ||
            ( posR && pogR && (/* seq2->iRhs->oVal */ GetWord((char*)(posR)+OFF_oVal) !=
	    /* gPi->iRhs->oVal */ GetWord((char*)(pogR)+OFF_oVal) + 1)) ) {
            return false;
        }

        sub_4544(REG_HL);
        /* regValues[REG_L] = gPi->iLhs; */
	ReadMem((char*)&regValues[REG_L], (char*)pogL, sizeof(operand_t));
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_L); 
#endif
        /* regValues[REG_H] = *seq2->iLhs; */
	ReadMem((char*)&regValues[REG_H], (char*)posL, sizeof(operand_t));
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_H); 
#endif
        removeInstr(seq2);
    } else {
        sub_4544(REG_HL);
        /* regValues[REG_HL] = *gPi->iLhs; */
	ReadMem((char*)&regValues[REG_HL], (char*)pogL, sizeof(operand_t));
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_HL); 
#endif
    }

    gPi = gPi->pAlt; /* m4: */
    removeInstr(gPi->pNext);
    while (l2_3053 != 0) {
        gPi             = allocInst(gPi);
        gPi->type       = T_INCDEC;
        gPi->iLhs       = allocOperand();
        /* gPi->iLhs->type = T_REG; */
	PutByte((char*)(gPi->iLhs)+OFF_type, (uint8_t)T_REG);
        /* gPi->iLhs->aux  = REG_HL; */
	PutByte((char*)(gPi->iLhs)+OFF_aux, (uint8_t)REG_HL);
        if (l2_3053 < 0) {
            ++l2_3053;
            gPi->aux = SI_INC;
        } else {
            --l2_3053;
            gPi->aux = SI_DEC;
        }
    }
    return logOptimise(O_RED_LD); /* 6fd5 opt_msg[12] = "Redundant loads/stores" */
}

/**************************************************************************
 43 sub_31ee    ok++
 **************************************************************************/
void swapHLDE() {
    operand_t pi;

#ifdef DEBUG
    pr_warning("swapHLDE");
#endif

    pi                          = regValues[REG_HL];
    regValues[REG_HL]           = regValues[REG_DE];
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_HL); 
#endif
    regValues[REG_DE]           = pi;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_DE); 
#endif

    pi                          = regValues[REG_H];
    regValues[REG_H]            = regValues[REG_D];
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_H); 
#endif
    regValues[REG_D]            = pi;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_D); 
#endif

    pi                          = regValues[REG_L];
    regValues[REG_L]            = regValues[REG_E];
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_L); 
#endif
    regValues[REG_E]            = pi;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_E); 
#endif
    regValues[REG_TRACKER].type = T_INVALID;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_TRACKER); 
#endif
};

/**************************************************************************
 44 pr_psect    sub_328a    ok++
 **************************************************************************/
void pr_psect(int psect) {

#ifdef DEBUG
    pr_warning("pr_psect");
#endif

    if (psect == cur_psect) {
        return;
    }
    printf("psect\t%s\n", psectNames[psect]);
    cur_psect = psect;
}

/**************************************************************************
 45 num_psect   sub_32bf    ok++
 **************************************************************************/
int num_psect(char const *fmt) {
    int l1;

#ifdef DEBUG
    pr_warning("num_psect");
#endif

    for (l1 = 0; l1 < 4; ++l1) {
        if (strcmp(fmt, psectNames[l1]) == 0) {
            return l1;
        }
    }
    pr_error("Unknown psect");
}

/**************************************************************************
 46	evalExpr	sub_3313  ok++   (PMO) apart from optimiser changes
 1) one code optimisation to remove jp, putting condition code on call
 2) code block moved with optimisation on shared code
 3) two further code block group moves
 Note the other code is identical and overall the logic is the same
 **************************************************************************/
typedef struct {
    int type; /* 01 s->str */
    int prec; /* 23 s->i_2 */
} op_t;

    char expectOp;
    term_t *pTerm;
    term_t termStack[30];
    op_t opStack[30];
    static term_t exprResult; /* so pointer to term can be returned */

term_t *evalExpr() {
register     op_t *pOp;

#ifdef DEBUG
    pr_warning("evalExpr");
#endif

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
        if (pOp->type == T_OPAR) { 	/* check for matching () */
            if (tokType != T_CPAR) {
                exp_err();    		/* ")" */
            }
            tokType  = get_token(); 	/* prime for next part */
            expectOp = 1;           	/* assuming its a term */
        } else if (T_MARKER >= pOp->type) {
            uconv(pOp->type, pTerm);    /* calculate unary operator */
        } else {
            bconv(pOp->type, pTerm + 1, pTerm); /* calculate binary operator*/
            ++pTerm;
        }
    } while (++pOp != &opStack[30]); 	/* loop till end of operator stack */

    exprResult = *pTerm;
    if (&termStack[29] != pTerm) { 	/* oops outstanding term */
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
 *
 *  Unary operators
 **************************************************************************/
void uconv(int op, register term_t *lhs) {

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
 *
 *  Binary operators
 **************************************************************************/
void bconv(int op, register term_t *lhs, term_t const *rhs) {

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
