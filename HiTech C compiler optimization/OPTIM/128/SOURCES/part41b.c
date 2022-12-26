/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include <stdio.h>
/* #include <stdlib.h> */
#include <string.h>
#include <setjmp.h>
#include "optim1.h"

extern int regTestMasks[];

/**************************************************************************
 67 sub_47e0    ok++ (PMO)
 *
 *  Optimiser differences
 * 1) The code for case 0 is detected as the same as the code in case T_JP
 *    and reused
 * 2) There are two instances where the test cases are reversed along with
 *    the associated jumps. Leaving the logic the same
 **************************************************************************/
bool sub_47e0(int reg, register inst_t const *pi1, inst_t const *pi2) {
    uint16_t msk;
    sym_t   *ps;
    inst_t  *l3;
    int      n; /* number of iterations */
    operand_t *po;
    uint8_t typ,aux;

#ifdef DEBUG
    pr_warning("sub_47e0");
#endif

    if (reg == REG_SP || reg == REG_IX)
        return true;

    if (REG_BC <= reg && reg <= REG_HL)
    {
        if (sub_47e0(regHiLoMap[reg].hiReg, pi1, pi2))
            return true;

        reg = regHiLoMap[reg].loReg;
    }

    if (reg >= regTestMasks_cnt /* sizeof(regTestMasks) / sizeof(regTestMasks[0] */)
        fprintf(stderr, "%d\n", reg);

    /* #pragma warning(suppress : 6385) /* reg has limited values */
    msk = regTestMasks[reg]; /* m3: */
    n   = 40;

    do {

        if (IsValid((char*)pi1->iLhs))
        {
            po = pi1->iLhs;
            ps = (sym_t*)GetWord((char*)po+OFF_oPSym);
            typ = GetByte((char*)(pi1->iLhs)+OFF_type);
            aux = GetByte((char*)(pi1->iLhs)+OFF_aux);
        }
        else
        {
            po = NULL;
            ps = NULL;
            typ = 0x7F;
            aux = 0x7F;
        }

        switch (pi1->type) {

            case T_CALL:
                if (pi1->aux != 0 && (msk & 0x40))
                    return true;

                if (reg == REG_IY)
                    break;

                if (! /* pi1->iLhs->oPSym */ ps ||
                    /* pi1->iLhs->oPSym */ ps->label[0] == '_')
                    return false;

                if (!(msk & 0xBF))
                    return false;
                else
                    return true;

            case T_JP:
                if (pi1->aux != 0 ||
                    /* pi1->iLhs->type */ typ == (uint8_t)T_REGREF ||
                    !ps )
                    return true;

                /* ps is not NULL !!! */

                if (!(l3 = ps->p.pInst))
                {
                    if (strcmp(ps->label, "cret") != 0)
                        return true;
                    else if (msk & 0x3C)  /* code reused */
                        return true;
                    else
                        return false;
                }
                pi1 = l3;
                break;

            case T_SIMPLE:
                switch (pi1->aux)
                { /* m14: */
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
                            if (pi1->type != T_LD && pi1->type != T_STK && pi1->type != T_CADD)
                                return false;

                        if (!pi1)
                            return false;

                        break;
                    default:
                        if (msk & 0x80)
                            return true;

                        break;
                }
                break;
            case T_TWOBYTE:
                if (reg != 17)
                    return true;

                break;

            case T_3:
                if (pi1->aux == I_XOR) /* m22: */
                {
                    if (/* pi1->iLhs->type */ typ == (uint8_t)T_REG &&
                        /* pi1->iLhs->aux */ aux == (uint8_t)REG_A &&
                        reg == REG_A)
                        return false;
                }

                if (msk & 0x80)
                    return true;    /* m23: */

                if (msk & 0x40)
                    return false;

                if (po && sub_475c(/* pi1->iLhs */ po, msk))  /* OPTIMISER[1]: see below */
                    return true;

                break;

            case T_SHIFT:
                if ((pi1->aux & 0xFFE7) != 0x20 && (msk & 0x40))
                    return true;

            case T_INCDEC:
                if ((msk & 0x40) &&
                    (/* pi1->iLhs->type */ typ != (uint8_t)T_REG ||
                     /* pi1->iLhs->aux */ aux < (uint8_t)REG_BC))
                    return false;

                if (/* pi1->iLhs->type */ typ == (uint8_t)T_REG ||
                    (po && sub_475c(/* pi1->iLhs */ po, msk)))
                    return true;

                break;

            case T_BIT: /* "set", "res", "bit" */
                if (pi1->aux == 0x40 && (msk & 0x40))
                    return false;

            case T_0xF: /* 0xF */
                if (sub_475c(pi1->iRhs, msk) || (po && sub_475c(/* pi1->iLhs */ po, msk)))
                    return true;

                break;

            case T_5:
                break;

            case T_DJNZ:
                if (msk & 1)
                    return true;

                break;

            case T_0xE:
                if (sub_475c(pi1->iRhs, msk))
                    return true;

                if (po && sub_47a2(/* pi1->iLhs */ po, msk))
                    return false;

                break;

            case T_STK:
                if (pi1->aux == I_PUSH &&
                   (msk & regTestMasks[/* pi1->iLhs->aux */ aux]))
                    return true;

                if (msk & regTestMasks[/* pi1->iLhs->aux */ aux])
                    return false;

                break;

            case T_EX:
                if (/* pi1->iLhs->type */ typ == (uint8_t)T_REGREF &&
                    (msk & regTestMasks[/* pi1->iLhs->aux */ aux]))
                    return true;

                if (msk & 0x3C)
                    return true;

                break;

            case T_CADD:
                if (msk & 0x40)
                    return false;

            case T_CARR: /* Add, sub with Carry */
                if ((regTestMasks[/* pi1->iLhs->aux */ aux] | 0x40) & msk)
                    return true;

                if (sub_475c(pi1->iRhs, msk))
                    return true;

                break;
            case T_LD:
                if (po && !operandsSame(/* pi1->iLhs */ po, pi1->iRhs))
                {
                    if (sub_475c(pi1->iRhs, msk))
                        return true;
                    else if (sub_47a2(/* pi1->iLhs */ po, msk))
                        return false;
                    else if (sub_475c(/* pi1->iLhs */ po, msk))
                        return true;
                }
                break;

            case 0: /* OPTIMISER[2]: optimised to reuse code in T_JP */
                if (msk & 0x3C)
                    return true;
                else
                    return false;
            case T_JR:
            case T_RET:
            case T_RST:
                break;
        }

        pi1 = pi1->pNext;

        if (pi2 == pi1)
            return false;

    } while (n-- != 0);

    return true;
}

/**************************************************************************
 68 sub_4c33    ok++ (PMO)  Used in: sub_4cf0, sub_4da7
 **************************************************************************/
sym_t *allocItem() {
    register sym_t *ps;

#ifdef DEBUG
    pr_warning("allocItem");
#endif

    ps = (sym_t *)freeItemList; /* check the free list*/
    if (ps) {                   /* if there is an entry release it*/
        freeItemList = ((list_t *)ps)->pNext;
        ps->label    = /* NULL */ 0;
        ps->p.pInst  = /* NULL */ 0;
        return ps;
    }
    return (sym_t *)alloc_mem(sizeof(sym_t)); /* else allocate a new one */
}

/**************************************************************************
 69 sub_4c6b    ok+
 **************************************************************************/
void freeSymbol(register sym_t *ps) {

#ifdef DEBUG
    pr_warning("freeSymbol");
#endif

    if (strlen(ps->label) >= sizeof(sym_t)) { /* if string could be reused as a symbol at it to the free list*/
        ((list_t *)(ps->label))->pNext = freeItemList;
        freeItemList                   = (list_t *)(ps->label);
    }
    ((list_t *)ps)->pNext = freeItemList; /* add the symbol to the free list */
    freeItemList          = (list_t *)ps;
}

/**************************************************************************
 70 hash_index  sub_4cab    ok++    Used in: sub_4cf0
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

#ifdef DEBUG
    pr_warning("lookupSym");
#endif

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
 72 sub_4da7    ok++
 **************************************************************************/
sym_t *allocBlankSym() {
    register sym_t *ps;

#ifdef DEBUG
    pr_warning("allocBlankSym");
#endif

    ps        = allocItem();
    ps->label = "";
    return ps;
}

/**************************************************************************
 73 sub_4dbf    ok++            Used in: optimise
 **************************************************************************/
void resetHeap() {
    int *p;

#ifdef DEBUG
    pr_warning("resetHeap");
#endif

    if (programBreak == 0) {
        programBreak = sbrk(0);    /* current base of heap */
    } else {
        brk(programBreak);         /* reset the heap */
    }
    alloct = allocs = programBreak;

    for (p = (int *)hashtab; p < (int *)&hashtab[HASHSIZE];) {
        *p++ = 0;
    }
    freeItemList = /* NULL */ 0;
}

/**************************************************************************
 74 sub_4e20    ok++            Used in: optimise
 **************************************************************************/
void freeHashtab() {

    allocs = (char *)&hashtab;
    alloct = (char *)&hashtab[HASHSIZE];
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

#ifdef DEBUG
    pr_warning("alloc_mem");
#endif

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

char* my_alloc_mem(void) {
    char* p;

    p = myalloc();

    if (!p)
        pr_error("Out of Upper 64KB RAM memory in %s", name_fun);

    return p;
}

/*
 * Simple sbrk & brk implementations
 */
#ifndef CPM

//#define MAXHEAP 0xff00
//static char *heapBase;
//static char *heapTop;

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

#endif

#ifdef _DEBUG
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

