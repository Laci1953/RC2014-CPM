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

expr_t *popExpr(void);
sym_t *findMember(sym_t *p1, char *p2);
bool isValidDimType(register s8_t *st);
bool sub_5a76(register s8_t *st, bool vst, uint8_t p2);
void sub_51cf(register sym_t *st);
void prError(int, ...);

/**************************************************
 * 23: 10A8 PMO +++
 * Minor differences, use of uint8_ parameter and
 * addition of missing parameters
 **************************************************/
bool sub_10a8(void) {
    expr_t *l1;
    expr_t *l2;
    uint8_t tok;
    register expr_t *st = 0;

    if ((tok = sub_255d()) == (char)T_LPAREN)
        return false;

    l1 = NULL;
    if (tok != T_120 &&
        (((opTable[tok - T_60].uc4 & 2) && (l1 = popExpr()) == 0) || (st = popExpr()) == 0))
        return true;
    switch (tok) {
    case T_64:
        sub_2529(T_69);
        p2List->type2 = T_EQ;
        tok           = T_PLUS;
        if (isValidDimType(&l1->attr))
            l1 = sub_1ccc(l1, DT_CONST);
        else
            /* prError("illegal type for index expression"); */
	    prError(70);
        break;
    case T_INC:
    case T_75:
    case T_DEC:
    case T_77:
        l1  = &s13_9d28;
        tok = tok == (char)T_INC ? T_PLUSEQ : tok == (char)T_DEC ? T_MINUSEQ : tok == (char)T_75 ? T_102 : T_104;
        break;
    case T_POINTER:
        tok = T_DOT;
        st  = sub_1441(T_69, st, 0); /* added dummy 3rd arg */
        break;
    case T_SIZEOF:
        if (st->tType == (char)T_SCONST) {
            l2 = sub_1b4b((long)st->t_i2 + 1, DT_UINT);
            sub_2569(st);
            pushS13(l2);
            return false;
        }

        if (st->tType != (char)S_TYPE && st->tType != (char)T_ID) {
            l2             = allocSType(&st->attr);
            l2->attr.i_sym = NULL;
            l2->attr.c7    = 0;
            if (st->attr.c7 == (char)ENODE) {
                l2  = sub_1441(T_SIZEOF, l2, 0);
                l1  = sub_1ccc(sub_21c7(st->attr.i_expr), DT_UINT);
                tok = T_STAR;
            }

            sub_2569(st);
            st = l2;
        } else if (st->tType == (char)S_TYPE && st->attr.c7 == (char)ENODE) {
            l1  = sub_1ccc(sub_21c7(st->attr.i_expr), DT_UINT);
            st  = sub_1441(T_SIZEOF, st, 0);
            tok = T_STAR;
        }
        break;
    case T_COMMA:
        if (p2List[0].type1 != (char)T_LPAREN || p2List[1].type1 != (char)T_61)
            tok = T_114;
        break;
    case T_79:
        pushS13(sub_1bf7(sub_1441(T_60, l1, 0), &st->attr));
        sub_2569(st);
        return 0;
    }
    if ((tok == (char)T_QUEST) != (l1 && l1->tType == (char)T_COLON))
        return true;

    pushS13(sub_1441(tok, st, l1));
    return false;
}

/**************************************************
 * 24: 1340 PMO +++
 * Optimised uint8_t param
 **************************************************/
expr_t *sub_1340(register expr_t *st, expr_t *p2) {
    sym_t *var2;
    sym_t *var4;
    char tmpbuf[32];

    if (!sub_5a76(&st->attr, false, DT_STRUCT) && !sub_5a76(&st->attr, false, DT_UNION))
        /* prError("struct/union required"); */
	prError(69);
    else if (p2->tType != (char)T_ID)
        /* prError("struct/union member expected"); */
	prError(68);
    else if ((var4 = st->a_nextSym) == 0)
        ;
    else if (!(/* var4->m18 */ GetWord((char*)var4+OFF_m18) & 1))
    {
	GetString(tmpbuf, (char*)GetWord((char*)var4+OFF_nVName));
	prError(100+67, tmpbuf);
    }
    else
    {
        GetString(tmpbuf, (char*)GetWord((char*)(p2->t_pSym)+OFF_nVName));

        if ((var2 = findMember(var4, /* p2->t_pSym->nVName */ tmpbuf)))     
        {
            sub_51cf(var2);
            st       = s13Alloc(T_126);
            st->t_i0 = /* var2->m14 */ GetWord((char*)var2+OFF_m14);

            /* st->attr = var2->attr; */
	    ReadMem((char*)&st->attr, (char*)var2, sizeof(struct _s8));

            sub_2569(p2);
            return st;
	}
    }
    return p2;
}

