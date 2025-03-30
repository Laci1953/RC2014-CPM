#include <stdio.h>
#include <setjmp.h>
#include <string.h>

#include "optim1.h"

void topmain(int argc, char **argv);

/**************************************************************************
 7  main    sub_07b1    ok++ (PMO)
 *
 *  In the switch statement the location of the code blocks for 's' and
 *  default are swapped by the optimiser, otherwise the code is identical
 **************************************************************************/
int main(int argc, char **argv) {

    topmain(argc, argv);

    /* optimise(); */
    CallOverlay0(optimise, 32+4);

    if (fclose(stdout) == -1) {
        pr_warning("Error closing output file");
        ++num_warn;
    }

    return (num_warn != 0);
}

/**************************************************************************
 8	pr_error	sub-0941	ok++
 **************************************************************************/
void pr_error(fmt, p2, p3) char * fmt;
{
    pr_message(fmt, p2, p3);
    fclose(stdout);
    quit(1);
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
    fprintf(stderr, fmt, p2, p3);
    fputc('\n', stderr);
}

/**************************************************************************
 14	freeOperand	sub_0b6a	ok++
 **************************************************************************/
void freeOperand(register operand_t *po) {

    if (!po) 
        return;

/* --------------------------------------------
    ((list_t *)po)->pNext = freeOperandLst;
    freeOperandLst       = (list_t *)po;
------------------------------------------------*/

    free512((char*)po);
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

    return (operand_t *) my_alloc_mem(sizeof(operand_t));
}

/**************************************************************************
 69 sub_4c6b    ok+
 **************************************************************************/
void freeSymbol(register sym_t *ps) {

#ifdef DEBUG
    pr_warning("freeSymbol");
#endif

    if (strlen(ps->label) >= sizeof(sym_t)) 
    { 
	/* if string could be reused as a symbol at it to the free list*/
        ((list_t *)(ps->label))->pNext = freeItemList;
        freeItemList                   = (list_t *)(ps->label);
    }

    ((list_t *)ps)->pNext = freeItemList; /* add the symbol to the free list */
    freeItemList          = (list_t *)ps;
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

    if (allocs + size > alloct)
    {
        if ((allocs = sbrk(512)) == (char *) -1) 
            pr_error("Out of Lower RAM memory in %s", name_fun);
        
	alloct = allocs + 512;
    }

    pi = allocs;
    allocs += size;
    for (p = pi; size-- != 0;) {
        *p++ = 0;    /* Clearing allocated memory area */
    }

    return pi;
}

char* my_alloc_mem(int size) {
    char* p;

    p = alloc512(size);

    if (!p)
        pr_error("Out of Upper RAM memory in %s", name_fun);

    ZeroFill(p, size);

    return p;
}
