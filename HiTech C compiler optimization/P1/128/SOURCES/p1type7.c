/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

void sub_573b(register sym_t *st, FILE *fp);

/**************************************************
 * 142: 742A PMO +++
 **************************************************/
uint16_t sub_742a(uint16_t n) {
    if (n)
        while (!(n & 1))
            n >>= 1;
    return n;
}

/**************************************************
 * 143: 7454 PMO +++
 **************************************************/
// st may be in Upper RAM
void sub_7454(register s8_t *st, bool vst) {
    uint16_t var2;
    uint8_t var3;
    char stc7;
    int sti4;
    char stdatatype;
    s8_t *stinextinfo;
    sym_t *stinextsym;
    char stinextinfoc7;
    bool vstinextinfo;
    s8_t buf8;

    putchar('`');
    for (;;) 
    {
    	if (vst)
    	{
	    ReadMem((char*)&buf8, (char*)st, sizeof(struct _s8));
	    stc7 = buf8.c7;
	    stdatatype = buf8.dataType;
	    sti4 = buf8.i4;
	    stinextsym = buf8.i_nextSym;
	    stinextinfo = buf8.i_nextInfo;
    	}
    	else
    	{
	    stc7 = st->c7;
	    stdatatype = st->dataType;
	    sti4 = st->i4;
	    stinextsym = st->i_nextSym;
	    stinextinfo = st->i_nextInfo;
    	}

        if (stc7 == (char)ANODE)
            putchar('(');

        for (var2 = sti4; var2; var2 >>= 1)
            if (var2 & 1)
                putchar('*');

	if (stdatatype == (char)DT_POINTER)
	{
	    vstinextinfo = IsValid((char*)stinextinfo);

    	    stinextinfoc7 = vstinextinfo ? GetByte((char*)stinextinfo+OFF_ATTR_c7) : stinextinfo->c7;

	    if (stinextinfoc7 == (char)ANODE)
	    {
            	st = stinextinfo;
		vst = vstinextinfo;
	    }
            else
            	break;
	}
	else
	    break;
    }

    var3 = stdatatype;

    switch (var3) 
    {
    case DT_ENUM:
    case DT_POINTER:
        sub_573b(stinextsym, stdout);
        break;
    case DT_STRUCT:
    case DT_UNION:
        printf("S%d", /* st->i_nextSym->a_labelId */ GetWord((char*)(stinextsym)+OFF_s8_t_labelId));
        break;
    default:
        if (var3 & 1) 
	{
            putchar('u');
            var3 &= ~1;
        }
        putchar("?bcsilxfd?v"[var3 >> 1]);
        break;
    }
}
