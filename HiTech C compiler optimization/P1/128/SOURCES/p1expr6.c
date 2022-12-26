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

void *xalloc(size_t size);
bool sub_5aa4(register s8_t *st);
bool sub_5a4a(register s8_t *st /* , bool vst */);
bool sub_5a76(register s8_t *st, bool vst, uint8_t p2);
void sub_5be1(register s8_t *st, bool vst);
bool sub_591d(register s8_t *st, bool vst, s8_t *p2, bool vp2);
bool sub_5ad5(register s8_t *st);
bool sub_5b08(register s8_t *st);
bool sub_5b38(register s8_t *st);
void sub_5b99(register s8_t *st);
void prError(int, ...);
void prWarning(int, ...);

/**************************************************
 * 26: 1B4B PMO +++
 **************************************************/
expr_t *sub_1b4b(long num, uint8_t p2) {
    register expr_t *st;

    st                = allocIConst(num);
    st->attr.dataType = p2;
    return st;
}

/**************************************************
 * 27: 1B70 PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *allocFConst(char *fltStr) {
    register expr_t *st;

    st                = s13Alloc(T_FCONST);
    st->t_s           = fltStr;
    st->attr.dataType = DT_DOUBLE;
    return st;
}

/**************************************************
 * 28: 1B94 PMO +++
 * minor optimiser difference, equivalent code
 **************************************************/
expr_t *sub_1b94(register expr_t *st) {

    if (!sub_5aa4(&st->attr))
        /* prError("logical type required"); */
	prError(57);
    else if (!sub_5a76(&st->attr, false, 2))
        st = sub_1441(T_NE, st, sub_1bf7(&s13_9d1b, &st->attr));

    return st;
}

/**************************************************
 * 29: 1BF7 PMO +++
 * minor optimiser difference, equivalent code
 **************************************************/
expr_t *sub_1bf7(register expr_t *st, s8_t *p2) {
    expr_t *var2;
    char tmpbuf[32];

    if (st->tType == (char)T_61 && (var2 = st->t_next)->tType == (char)T_ID && 
	(/* var2->t_pSym->m18 */ GetWord((char*)(var2->t_pSym)+OFF_m18) & 0x40)) 
    {
	GetString(tmpbuf, (char*)GetWord((char*)(var2->t_pSym)+OFF_nVName));
	prWarning(100+88, tmpbuf);
        /* var2->t_pSym->m18 &= ~0x40; */
	PutWord((char*)(var2->t_pSym)+OFF_m18, GetWord((char*)(var2->t_pSym)+OFF_m18) & ~0x40);
    }

    if (!sub_591d(&st->attr, false, p2, false)) {
        if (st->tType != (char)T_ICONST || inData(st))
            st = sub_23b4(T_124, st, allocSType(p2));
        st->attr = *p2;
    }
    return st;
}

/**************************************************
 * 30: 1CCC PMO +++
 **************************************************/
expr_t *sub_1ccc(expr_t *p1, uint8_t p2) {
    s8_t st;

    st.dataType = p2;
    st.i4       = 0;
    st.i_sym    = 0;
    st.c7       = 0;
    return sub_1bf7(p1, &st);
}

/**************************************************
 * 31: 1D02 PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *sub_1d02(register expr_t *st) {

    if (st->tType == (char)T_COMMA || st->tType == (char)120)
        return st;
    if (!st->attr.c7 && !st->attr.i4) {
        if (st->attr.dataType < (unsigned char)DT_INT || st->attr.dataType == (char)DT_ENUM)
            return sub_1ccc(st, DT_INT);
        if (st->attr.dataType == (char)DT_FLOAT)
            return sub_1ccc(st, DT_DOUBLE);
    }
    return st;
}

/**************************************************
 * 32: 1D5A PMO +++
 **************************************************/
uint8_t sub_1d5a(register s8_t *st, s8_t *p2) {
    bool mkUnsigned;
    uint8_t l2;

    mkUnsigned = (st->dataType & DT_UNSIGNED) || (p2->dataType & DT_UNSIGNED);
    l2         = st->dataType;
    if (l2 < p2->dataType)
        l2 = p2->dataType;
    if (l2 < DT_INT)
        l2 = DT_INT;
    if (l2 == (char)DT_FLOAT || l2 == (char)DT_DOUBLE)
        return DT_DOUBLE;
    if (l2 == (char)DT_ENUM)
        l2 = DT_INT;
    if (mkUnsigned)
        return l2 | 1;
    return l2;
}

/**************************************************
 * 33: 1DF0 PMO +++
 **************************************************/
expr_t *sub_1df0(register expr_t *st) {

    if (st->tType != (char)T_ID || st->attr.c7 != (char)ANODE)
        return st;
    st->attr.dataType = 0x16;
    st->attr.c7       = SNODE;
    st->a_nextSym     = st->t_pSym;
    st->attr.i4       = 0;
    return sub_1e58(st);
}

/**************************************************
 * 34: 1E37 PMO +++
 **************************************************/
expr_t *sub_1e37(register expr_t *st) {

    if (st->attr.c7 == (char)ENODE)
        st = sub_1e58(st);
    return st;
}

/**************************************************
 * 35: 1E58 PMO +++
 * differences due to added 3rd arg and uint8_t param
 **************************************************/
expr_t *sub_1e58(register expr_t *st) {
    expr_t *pi;

    pi             = sub_23b4(0x46, st, 0); /* PMO missing 3rd arg. added 0 */
    pi->attr       = st->attr;
    pi->attr.i_sym = 0;
    pi->attr.c7    = SNODE;
    sub_5be1(&pi->attr, false);
    return pi;
}

/**************************************************
 * 36: 1EBD PMO +++
 * differences due to added 3rd arg and uint8_t param
 **************************************************/
expr_t *sub_1ebd(register expr_t *st) {
    st = allocSType(&st->attr);
    st->attr.i4 >>= 1;
    return sub_1441(T_SIZEOF, st, 0); /* PMO fixed missing 3rd arg */
}

/**************************************************
 * 37: 1EF1 PMO +++
 **************************************************/
bool sub_1ef1(register expr_t *st) {

    switch (st->tType) {
    case T_69:
        return true;
    case T_ID:
        return (/* st->t_pSym->m18 */ GetWord((char*)(st->t_pSym)+OFF_m18) & 0x10) &&
	        /* st->t_pSym->m20 */ GetByte((char*)(st->t_pSym)+OFF_m20) != (char)D_CONST &&
	        st->attr.c7 == SNODE;
    case T_DOT:
        return st->attr.c7 == (char)SNODE && sub_1ef1(st->t_next);
    }
    return false;
}

/**************************************************
 * 38: 1F5D PMO +++
 * minor differences due to uint8_t param
 **************************************************/
expr_t *sub_1f5d(register expr_t *st, s8_t *p2, int16_t p3) {
    s8_t *pAttr;

    pAttr = &st->attr;
    if (sub_591d(pAttr, false, p2, false) == 0) {
        if (sub_5ad5(pAttr) && sub_5ad5(p2)) {
            if (sub_5b08(p2) && sub_5b38(pAttr))
                /* prWarning("implicit conversion of float to integer"); */
		prWarning(84);
        } else if ((p2->i4 & 1) && p2->c7 == (char)SNODE && sub_5b08(pAttr)) {
            if (p3 == 0 && (sub_5a76(pAttr, false, DT_CONST) || sub_5a76(pAttr, false, DT_UCONST)))
                return st;
            if (!sub_2105(st))
                /* prWarning("illegal conversion of integer to pointer"); */
		prWarning(87);
        } else if ((pAttr->i4 & 1) && pAttr->c7 == (char)SNODE && sub_5b08(p2))
            /* prWarning("illegal conversion of pointer to integer"); */
	    prWarning(86);
        else if ((pAttr->i4 & 1) && pAttr->c7 == (char)SNODE && (p2->i4 & 1) && p2->c7 == (char)SNODE) {
            if (!sub_5a4a(pAttr /*, false */) && !sub_5a4a(p2 /*, false */))
                /* prWarning("illegal conversion between pointer types"); */
		prWarning(85);
        } else
            /* prError("illegal conversion"); */
	    prError(56);
    }
    return sub_1bf7(st, p2);
}

/**************************************************
 * 39: 2105 PMO +++
 **************************************************/
bool sub_2105(register expr_t *st) {

    switch (st->tType) {
    case T_ICONST:
        return st->t_ul == 0L; /* long */
    case T_124:
        if (sub_5b08(&st->attr))
            return sub_2105(st->t_next);
        break;
    }
    return false;
}

/**************************************************
 * 40: 2157 PMO +++
 **************************************************/
bool s13ReleaseFreeList(void) {
    register expr_t *st;

    if (s13FreeList == 0)
        return false;
    while ((st = s13FreeList)) {
        s13FreeList = st->t_next;
        free(st);
    }
    return true;
}

/**************************************************
 * 41: 2186 PMO +++
 **************************************************/
expr_t *s13Alloc(uint8_t tok) {
    register expr_t *st;

    if (s13FreeList != 0) {
        st          = s13FreeList;
        s13FreeList = st->t_next;
        blkclr(st, sizeof(expr_t));
    } else
        st = xalloc(sizeof(expr_t));
    st->tType         = tok;
    st->attr.dataType = DT_VOID;
    return st;
}

/**************************************************
 * 42: 21C7 PMO +++
 * uint8_t paramater
 **************************************************/
expr_t *sub_21c7(register expr_t *st) {
    expr_t *l1;
    uint16_t l2;

    l1  = s13Alloc(0);
    *l1 = *st;
    l2  = opTable[st->tType - 60].uc4;
    if ((l2 & 1) || st->tType == (char)T_120)
        return l1;

    l1->t_next = sub_21c7(st->t_next);
    if (l2 & 2)
        l1->t_alt = sub_21c7(st->t_alt);

    return l1;
}

/**************************************************
 * 43: 225A PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *sub_225a(uint8_t p1, register expr_t *st, expr_t *p3) {
    expr_t *l1;

    if (p1 == (char)T_124 && st->tType == (char)T_ICONST) {
        st->attr = p3->attr;
        sub_2569(p3);
        return st;
    }

    l1 = sub_23b4(p1, st, p3); /* m1:  */

    switch (opTable[p1 - 60].c7) {
    case 1:
        l1->attr = st->attr;
        break;
    case 2:
        l1->attr = p3->attr;
        break;
    case 4:
        l1->attr = st->attr;
        sub_5be1(&l1->attr, false);
        break;
    case 3:
        l1->attr = st->attr;
        sub_5b99(&l1->attr);
        break;
    case 5:
        l1->attr.dataType = DT_INT;
        break;
    case 6:
        l1->attr.dataType = DT_2;
        break;
    case 7:
        l1->attr.dataType = DT_VOID;
        break;
    }

    return l1;
}

/**************************************************
 * 44: 23B4 PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *sub_23b4(uint8_t tok, register expr_t *st, expr_t *p3) {
    expr_t *pi;

    pi = s13Alloc(tok);
    if (tok != (char)T_120) {
        pi->t_next = st;
        if (opTable[tok - 60].uc4 & 2)
            pi->t_alt = p3;
    }
    return pi;
}

/**************************************************
 * 45: 240E PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *allocId(register sym_t *st) {
    expr_t *pi;

    pi         = s13Alloc(T_ID);
    pi->t_pSym = st;
    if ((/* st->m18 */ GetWord((char*)st+OFF_m18) & 0x10) || 
	 /* st->m20 */ GetByte((char*)st+OFF_m20) == (char)D_MEMBER)
{
        /* pi->attr = st->attr; */
        ReadMem((char*)&pi->attr, (char*)st, sizeof(struct _s8));
}

    return pi;
}

/**************************************************
 * 46: 245D PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *allocSConst(void) {
    register expr_t *st;

    st                = s13Alloc(T_SCONST);
    st->attr.dataType = DT_CHAR;
    st->attr.i4       = 1;
    st->t_i0          = ++strId;
    return st;
}

/**************************************************
 * 47: 248A PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *allocIConst(long p1) {
    register expr_t *st;

    st      = s13Alloc(T_ICONST);
    st->t_l = p1;
    return st;
}

/**************************************************
 * 48: 24B6 PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *allocSType(s8_t *p1) {
    register expr_t *st;

    st       = s13Alloc(S_TYPE);
    st->attr = *p1;

    return st;
}

/**************************************************
 * 49: 24DE PMO +++
 **************************************************/
void complexErr(void) {

    fatalErr("expression too complex");
}

/**************************************************
 * 50: 24E7 PMO +++
 **************************************************/
void pushS13(expr_t *p1) {
    if (s13SP == s13Stk)
        complexErr();
    *(--s13SP) = p1;
}

/**************************************************
 * 51: 250A PMO +++
 **************************************************/
expr_t *popExpr(void) {
    if (s13SP != &s13Stk[20])
        return *(s13SP++);
    return NULL;
}

/**************************************************
 * 52: 2529 PMO +++
 **************************************************/
void sub_2529(uint8_t p1) {
    register s2_t *st;

    if (p2List == s2_9cf3)
        complexErr();
    (--p2List)->type1    = p1;
    (st = p2List)->type2 = opTable[p1 - 60].c3;
}

/**************************************************
 * 53: 255D PMO +++
 **************************************************/
uint8_t sub_255d(void) {

    return (p2List++)->type1;
}

/**************************************************
 * 54: 2569 PMO +++
 **************************************************/
void sub_2569(register expr_t *st) {
    uint8_t type;

    if (st) {
        type = st->tType;
        if (!(opTable[type - 60].uc4 & 1) && type != (char)T_120) {
            sub_2569(st->t_next);
            if (opTable[type - 60].uc4 & 2)
                sub_2569(st->t_alt);
        }
        if (!inData(st)) {
            if (type == (char)T_FCONST)
                free(st->t_s);
            st->t_next  = s13FreeList; /* m2: */
            s13FreeList = st;
        }
    }
}

/**************************************************
 * 55: 25F7 PMO +++
 * uint8_t parameter + minor optimisation difference
 * code is equivalent
 **************************************************/
expr_t *sub_25f7(register expr_t *st) {

    if (st) {
        if (!inData(st) && st->tType == (char)T_ICONST) {
            st->t_ul += 1;
        } else if (st->tType == (char)T_PLUS)
            st->t_alt = sub_25f7(st->t_alt);
        else
            st = sub_1441(T_PLUS, st, sub_1b4b(1L, DT_INT)); /* m3: */
    }
    return st; /* m4: */
}
