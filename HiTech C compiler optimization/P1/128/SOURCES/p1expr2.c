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

bool isValidDimType(register s8_t *st);
bool sub_5a76(register s8_t *st, bool vst, uint8_t p2);
void prError(int, ...);

/**************************************************
 * 19: 0A83 PMO +++
 * Minor differences due to addition of missing
 * parameters and use of uint8_t parameter
 **************************************************/
expr_t *sub_0a83(uint8_t n) {
    register expr_t *st;

    byte_9d37 = n;
    if ((st = sub_1441(T_60, sub_0bfc(), 0))) { /* PMO added dummy arg3 */
        if (!sub_0aed(st))
            /* prError("constant expression required"); */
	    prError(75);
        else if (byte_9d37 == 2) {
            if (isValidDimType(&st->attr))
                st = sub_1ccc(st, DT_CONST);
            else
                /* prError("illegal type for array dimension"); */
		prError(74);
        }
    }
    byte_9d37 = 0;
    return st;
}

/**************************************************
 * 20: 0AED PMO +++
 * Locations of code to set return  values 1/0 swapped
 * rest of code adjusted accordingly
 **************************************************/
bool sub_0aed(register expr_t *st) {
    uint8_t type, flags;

    if (st == 0)
        return true;
    type = st->tType;
    if (type == (char)T_ID)
        return sub_5a76(&st->attr, false, DT_ENUM);
    if (type == (char)T_SIZEOF)
        return true;
    if (type == (char)D_ADDRESSOF && sub_0b93(st->t_next))
        return true;
    flags = opTable[(type - 60)].uc4;
    if (!(flags & 0x10))
        return false;
    if (flags & 1)
        return true;
    return sub_0aed(st->t_next) && (!(flags & 2) || sub_0aed(st->t_alt));
}

/**************************************************
 * 21: 0B93 PMO +++
 **************************************************/
bool sub_0b93(register expr_t *st) {
    uint8_t type;
    char tmp;

    type = st->tType;
    if (type == (char)T_ID)
    {
	tmp = GetByte((char*)(st->t_pSym)+OFF_m20);
        return /* st->t_pSym->m20 */ tmp == (char)T_EXTERN ||
               /* st->t_pSym->m20 */ tmp == (char)T_STATIC;
    }

    if (type == (char)T_DOT)
        return sub_0b93(st->t_next);

    return type == (char)T_69 && sub_0aed(st->t_next);
}
