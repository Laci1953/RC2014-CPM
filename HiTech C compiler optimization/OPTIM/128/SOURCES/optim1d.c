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

#define _Z       0      /* 0000 0000 */
#define _U       1      /* 0000 0001 */
#define _L       2      /* 0000 0010 */
#define _D       4      /* 0000 0100 */
#define _H       8      /* 0000 1000 */
#define _S      16      /* 0001 0000 */

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

/*
 *      Common code sequences
 */
#define PEEKCH() (charsLeft > 0 ? *ptr_inbuf : '\n')
#define GETCH()  (--charsLeft >= 0 ? *ptr_inbuf++ : get_line())

/**************************************************************************
 23     sub_122f        ok++    Used in: optimise
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
 24     sub_1369        ok++ (PMO)
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
 25     sub_1397        ok++ (PMO)
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
 26     sub_140b        ok++
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
 27     sub_142f        ok++
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
 28     instrSame       sub_14ac        ok++
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
 29     sub_153d        ok++
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
 30     sub_15ad        ok++    (PMO) Used in optimize
 *
 * code optimised over original as noted below, otherwise identical
 * 1) for loop iteration expression moved
 * 2) code to increment an optiminse counter & set hasChanged = true shared
 * 3) sub_1d94() == 0, test removed as code falls through in either case
 **************************************************************************/
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

