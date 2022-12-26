/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

extern s2_t *p2List;  /* 8bc7 */
extern int16_t strId ;            /* 8bd7 */
extern uint8_t byte_8f85 ;            /* 8f85 */
extern bool byte_8f86 ;        /* 8f86 */
extern uint8_t byte_968b;                /* 968b */
extern int16_t word_968c;                /* 968c */
extern int16_t tmpLabelId;               /* 968e */

extern expr_t **s13SP;   /* 9cf1 */
extern s2_t s2_9cf3[20]; /* 9cf3 */
extern char pad9d00[27];
extern expr_t s13_9d1b; /* 9d1b */
extern expr_t s13_9d28; /* 9d28 */

extern expr_t *s13FreeList; /* 9d35 */
extern uint8_t byte_9d37;   /* 9d37 */
extern expr_t *s13Stk[20];  /* 9d38 */

bool sub_5ad5(register s8_t *st);
bool sub_5b08(register s8_t *st);
bool sub_5a4a(register s8_t *st /*, bool vst */);
bool sub_591d(register s8_t *st, bool vst, s8_t *p2, bool vp2);
bool sub_5aa4(register s8_t *st);
bool sub_5a76(register s8_t *st, bool vst, uint8_t p2);
void prError(int, ...);
void prWarning(int, ...);

/**************************************************
 * 25: 1441 PMO +++
 * Minor optimiser differences, but equivalent code
 * Also uint8_t parameter differences
 **************************************************/
expr_t *sub_1441(uint8_t p1, register expr_t *lhs, expr_t *rhs) {
    s8_t tmpExpr;
    bool hasRhs;
    int16_t opFlags;
    expr_t *savedLhs;
    bool minusLhsValid;
    expr_t *minusLhs;
    char *opStr;
    uint8_t var13;

    hasRhs = (opTable[p1 - 60].uc4 & 2) != 0;
 
    if (!lhs || (hasRhs && rhs == 0)) 
    {
        sub_2569(lhs);

        if (hasRhs)
            sub_2569(rhs);

        return NULL;
    }

    opFlags       = opTable[p1 - 60].i5;

    if (p1 == (char)D_ADDRESSOF && lhs->tType == (char)T_ID && 
	(/* lhs->t_pSym->m18 */ GetWord((char*)(lhs->t_pSym)+OFF_m18) & 4))
        /* prError("can't take address of register variable"); */
	prError(66);

    if (!(opFlags & 0x100))
        lhs = sub_1e37(lhs);

    if (hasRhs && !(opFlags & 0x800))
        rhs = sub_1e37(rhs);

    if (p1 == T_61) 
    {
        if ((lhs->attr.i4 & 1) && lhs->attr.c7 == (char)SNODE)
            lhs = sub_1441(T_69, lhs, 0); /* dummy 3rd arg */
    } 
    else
        lhs = sub_1df0(lhs);

    if (hasRhs)
        rhs = sub_1df0(rhs);

    if ((opFlags & 0x2000) && sub_1ef1(lhs) == 0) 
    {
        if (p1 == (char)D_ADDRESSOF) 
	{
            if (lhs->tType == (char)D_ADDRESSOF && lhs->t_next->attr.c7 == (char)ENODE)
                return lhs;
            else
                /* prError("can't take this address"); */
		prError(65);
        } 
	else
            /* prError("only lvalues may be assigned to or modified"); */
	    prError(64);
    }

    if ((opFlags & 0x4000) && (!(lhs->attr.i4 & 1) || lhs->attr.c7 != (char)SNODE))
        /* prError("pointer required"); */
	prError(63);

    if (!(opFlags & 3)) 
    {
        if (sub_5a76(&lhs->attr, false, DT_2))
            lhs = sub_1ccc(lhs, DT_INT);

        if (hasRhs && sub_5a76(&rhs->attr, false, DT_2))
            rhs = sub_1ccc(rhs, DT_INT);
    }

    switch (p1) 
    {
    case T_60:
        return lhs;
    case T_DOT:
        rhs = sub_1340(lhs, rhs);
        break;
    case T_121:
        /* tmpExpr    = curFuncNode->attr; */
	ReadMem((char*)&tmpExpr, (char*)curFuncNode, sizeof(struct _s8));
        tmpExpr.c7 = SNODE;
        if (sub_5a76(&tmpExpr, false, DT_VOID))
            /* prError("void function cannot return value"); */
	    prError(62);
        else
            lhs = sub_1f5d(lhs, &tmpExpr, 1);
        break;
    }

    opStr  = opTable[p1 - 60].s0;

    minusLhsValid = false;

    if ((opFlags & 3)) 
    {
        if ((opFlags & 2))
            lhs = sub_1b94(lhs);
        if ((opFlags & 1))
            rhs = sub_1b94(rhs);
    } 
    else if ((opFlags & 4) && (lhs->attr.i4 & 1) && lhs->attr.c7 == (char)SNODE &&
               sub_5b08(&rhs->attr)) /* 16e1 */
    {
        rhs = sub_1ccc(sub_1441(T_STAR, rhs, sub_1ebd(lhs)),
                       (rhs->attr.dataType & DT_UNSIGNED) ? DT_UCONST : DT_CONST);
    }
    else if (p1 == T_PLUS && (rhs->attr.i4 & 1) && rhs->attr.c7 == (char)SNODE &&
             sub_5b08(&lhs->attr)) 
    { /* 1740 */
        savedLhs = lhs;
        lhs      = rhs;
        rhs      = sub_1ccc(sub_1441(T_STAR, savedLhs, sub_1ebd(lhs)),
                       (rhs->attr.dataType & DT_UNSIGNED) ? DT_UCONST : DT_CONST);
    } 
    else if ((opFlags & 8) && (lhs->attr.i4 & 1) && lhs->attr.c7 == (char)SNODE &&
               (!hasRhs || ((rhs->attr.i4 & 1) && rhs->attr.c7 == (char)SNODE))) 
    { /* 17ab */
        if (!(opFlags & 0x8000) || (!sub_5a4a(&lhs->attr /*, false */) && !sub_5a4a(&rhs->attr /*, false */))) 
	{
            if (hasRhs && !sub_591d(&lhs->attr, false, &rhs->attr, false))
                /* prWarning("operands of %.3s not same pointer type", opStr); */
		prWarning(100+90, opStr);
            else if (p1 == T_MINUS) 
	    {
                minusLhs      = lhs;
                minusLhsValid = true;
                lhs           = sub_1ccc(lhs, DT_CONST);
                rhs           = sub_1ccc(rhs, DT_CONST);
            }
        }
    } 
    else if ((opFlags & 0x30) && sub_5ad5(&lhs->attr) &&
               (!hasRhs || sub_5ad5(&rhs->attr))) 
    { /* 187a */
        if (opFlags & 0x40) 
	{
            var13 = sub_1d5a(&lhs->attr, &rhs->attr);
            lhs   = sub_1ccc(lhs, var13);
            rhs   = sub_1ccc(rhs, var13);
        } /* 18fa */

        if (!(opFlags & 0x10) && (!sub_5b08(&lhs->attr) || (hasRhs && !sub_5b08(&rhs->attr))))
            /* prError("integral type required"); */
	    prError(60);
    } 
    else if (opFlags & 0x80) 
    { /* 1937 */
        if (sub_5a76(&lhs->attr, false, DT_VOID) || (hasRhs && sub_5a76(&rhs->attr, false, DT_VOID))) 
	{
            if (p1 == T_124 && lhs->tType == (char)T_61 && sub_5a76(&rhs->attr, false, DT_VOID)) 
	    {
                lhs->attr = rhs->attr;
                sub_2569(rhs);
                return lhs;
            }

            /* prError("illegal use of void expression"); */
	    prError(61);
        } 
	else if (!(opFlags & 0x200) &&
                   (!sub_5aa4(&lhs->attr) || (hasRhs && !sub_5aa4(&rhs->attr)))) /* 19cc */
            /* prError("simple type required for %.3s", opStr); */
	    prError(100+59, opStr);
        else if (opFlags & 0x1000) 
	{ /* 1a11 */
            if ((opFlags & 0x8000)) 
	    {
                if (sub_2105(lhs) && (rhs->attr.i4 & 1) && rhs->attr.c7 == (char)SNODE)
                    lhs = sub_1bf7(lhs, &rhs->attr);
                else if (sub_2105(rhs) && (lhs->attr.i4 & 1) && lhs->attr.c7 == (char)SNODE)
                    rhs = sub_1bf7(rhs, &lhs->attr);
            } /* 1a95 */

            if (!sub_591d(&lhs->attr, false, &rhs->attr, false))
                /* prWarning("operands of %.3s not same type", opStr); */
		prWarning(100+89, opStr);
        }
    } 
    else if (!(opFlags & 0x200)) /* 1ac3 */
        /* prError("type conflict"); */
	prError(58);

    /* 1ad1 */
    if (opFlags & 0x400)
        rhs = sub_1f5d(rhs, &lhs->attr, (opFlags & 4) == 0);

    savedLhs = sub_225a(p1, lhs, rhs);

    if (minusLhsValid)
        savedLhs = sub_1441(T_DIV, savedLhs, sub_1ebd(minusLhs));

    return savedLhs;
}
