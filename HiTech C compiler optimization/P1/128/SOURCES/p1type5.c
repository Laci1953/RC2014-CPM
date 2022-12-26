/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

sym_t *sub_56a4(void);
uint8_t yylex(void);
bool sub_5a76(register s8_t *st, bool vst, uint8_t p2);
sym_t *sub_4eed(register sym_t *st, uint8_t p2, s8_t *p3, bool v, sym_t *p4);
void sub_58bd(register s8_t *st, bool vst, s8_t *p2, bool vp2);
void sub_0493(register sym_t *st);
void prError(int, ...);
void prWarning(int, ...);

/**************************************************
 * 140: 69CA PMO +++
 * minor code optimiser differences and deltas
 * due to dummy and uit8_t args
 **************************************************/
sym_t *sub_69ca(uint8_t p1, register s8_t *p2, uint8_t p3, sym_t *p4) {
    int16_t var2;
    sym_t *var4;
    s12_t *var6;
    s12_t var12;
    uint8_t tok;
    s8_t var1b;
    char c7;
    char dataType;
    char nextInfo_c7;
    int nextInfo_i4;
    s8_t *nextInfo;
    bool v;
    bool vnextInfo;
    s8_t buf8;
    sym_t* nextSym;
    int i4;
    char tmpbuf[32];

    var6             = p12_a297;
    p12_a297         = &var12;
    var1b.i_info     = NULL; /* other options */
    var1b.i_nextInfo = NULL; /* other options */
    var1b.c7         = 0;
    var1b.i4         = 0;
    var1b.dataType   = 0;
    p12_a297->p8     = &var1b;
    v                = false;
    p12_a297->i6     = 0;
    p12_a297->p25    = NULL;
    p12_a297->p25_1  = NULL;
    p12_a297->uca    = 0;
    p12_a297->uc9    = 0;
    p12_a297->uc8    = 0;
    p12_a297->ucb    = 0;

    sub_6fab(p1, &v);		// v = IsValid(p12_a297->p8)

    ungetTok = tok = yylex();

    if (p12_a297->ucb) 
    {
        if (p1 == (char)T_79 ||
	    p1 == (char)D_14 ||
	    p1 == (char)D_15 ||
	    (p1 == (char)D_MEMBER && tok == (char)T_COLON)) 
	{
            p12_a297->p25 = sub_56a4();

            if (p1 == (char)T_79)
                p1 = T_TYPEDEF;
        }
	else
            prError(12);

    } /* 6aaf */

    if (v)
    {
	PutByte((char*)(p12_a297->p8)+OFF_ATTR_dataType, p2->dataType);
	PutWord((char*)(p12_a297->p8)+OFF_ATTR_u1, (short)p2->u1._nextSym); /* p2->u1 */
    }
    else
    {
    	p12_a297->p8->dataType = p2->dataType;
    	p12_a297->p8->u1       = p2->u1;
    }

    for (var2 = p2->i4; var2; var2 >>= 1) 
    {
        if (p12_a297->i6 & 1) 
	{
            p12_a297->uc8 = 1;
            break;
        }

        p12_a297->i6 = (p12_a297->i6 >> 1) | ((var2 & 1) << 15);
    }

    /* ^^^ */
    for (;;) 
    {
    	if (v)
    	{
		ReadMem((char*)&buf8, (char*)(p12_a297->p8), sizeof(struct _s8));
		c7 = buf8.c7;
		dataType = buf8.dataType;
		nextInfo = buf8.i_nextInfo;
  	}
    	else
    	{
      		c7 = p12_a297->p8->c7;
        	dataType = p12_a297->p8->dataType;
        	nextInfo = p12_a297->p8->i_nextInfo;
    	}

    	vnextInfo = IsValid((char*)nextInfo);

    	if (vnextInfo)
    	{
		ReadMem((char*)&buf8, (char*)nextInfo, sizeof(struct _s8));
    		nextInfo_c7 = buf8.c7;
    		nextInfo_i4 = buf8.i4;
    	}
    	else
    	{
    		nextInfo_c7 = nextInfo->c7;
    		nextInfo_i4 = nextInfo->i4;
    	}

        if (/* p12_a297->p8->c7 */ c7 == (char)SNODE &&
	    /* p12_a297->p8->dataType */ dataType == (char)DT_POINTER &&
            (p12_a297->i6 == 0 || /* p12_a297->p8->i_nextInfo->c7 */ nextInfo_c7 == SNODE)) 
	{
            for (var2 = /* p12_a297->p8->i_nextInfo->i4 */ nextInfo_i4; var2; var2 >>= 1) 
	    {
                if (p12_a297->i6 & 1) 
		{
                    p12_a297->uc8 = 1;
                    break;
                }

                p12_a297->i6 = (p12_a297->i6 >> 1) | ((var2 & 1) << 15);
            }

            sub_58bd(/* p12_a297->p8->i_nextInfo */ nextInfo, vnextInfo, p12_a297->p8, v);
        }
	else
            break;
    } /* 6c13 vvv */

    if (p12_a297->uc8)
        prError(11);

    if (v)
	PutWord((char*)(p12_a297->p8)+OFF_ATTR_i4, sub_742a(p12_a297->i6));
    else
    	p12_a297->p8->i4 = sub_742a(p12_a297->i6);

    if (p12_a297->p25 && p1 != (char)T_TYPEDEF && p1 != (char)D_14 && p1 != (char)D_15 &&
        sub_5a76(p12_a297->p8, v, DT_VOID) && /* p12_a297->p8->c7 */ c7 != ANODE)
        prError(9);
    else if (sub_5a76(p12_a297->p8, v, DT_POINTER)) 
    {
        if (/* p12_a297->p8->c7 */ c7 == (char)ANODE && /* p12_a297->p8->i_nextInfo->c7 */ nextInfo_c7 == (char)ENODE)
            prError(10);
        else if (/* p12_a297->p8->c7 */ c7 == (char)ENODE && /* p12_a297->p8->i_nextInfo->c7 */ nextInfo_c7 == (char)ANODE)
            prError(4);
    }

    if (p12_a297->p25_1) 
    {
        p12_a297->p25_1 = sub_4eed(p12_a297->p25_1, T_TYPEDEF, &p12_a297->p25_1->attr, true, 0);
        sub_516c(p12_a297->p25_1);
        sub_51cf(p12_a297->p25_1);

        if (GetByte((char*)(p12_a297->p25_1)+OFF_s8_t_c7) != (char)ANODE ||
	    !(GetWord((char*)(p12_a297->p25_1)+OFF_m18) & 0x80))
            sub_0493(p12_a297->p25_1);

    } /* 6d95 */

    if (v)
	i4 = GetWord((char*)(p12_a297->p8)+OFF_ATTR_i4);
    else
	i4 = p12_a297->p8->i4;

    if (p1 != (char)T_TYPEDEF && /* p12_a297->p8->i4 */ i4 == 0 &&
        (sub_5a76(p2, false, DT_STRUCT) || sub_5a76(p2, false, DT_UNION)) && p2->i_nextSym &&
        !(/* p2->i_nextSym->m18 */ GetWord((char*)(p2->i_nextSym)+OFF_m18) & 1))
    {
	if (v)
	    nextSym = (sym_t*)GetWord((char*)(p12_a297->p8)+OFF_ATTR_nextSym);
	else
	    nextSym = p12_a297->p8->i_nextSym;

	GetString(tmpbuf, (char*)GetWord((char*)nextSym+OFF_nVName));
        prError(100+7, /* p12_a297->p8->i_nextSym->nVName */ tmpbuf);
    }

    if (p12_a297->p25) 
    { /* 6e0b */
	if (byte_a299 == D_6 && p1 != D_MEMBER) 
	{
	    GetString(tmpbuf, (char*)GetWord((char*)(p12_a297->p25)+OFF_nVName));

            if (GetWord((char*)(p12_a297->p25)+OFF_m18) & 8)
                prError(100+5, tmpbuf);
            else if (!(GetWord((char*)(p12_a297->p25)+OFF_m18) & 0x20))
                prError(100+6, tmpbuf);
            else 
	    {
		/* p12_a297->p25->m18 |= p3 | 8; */
		PutWord((char*)(p12_a297->p25)+OFF_m18, GetWord((char*)(p12_a297->p25)+OFF_m18) | p3 | 8);
		/* p12_a297->p25->attr = var1b; */
		WriteMem((char*)&var1b, (char*)(p12_a297->p25), sizeof(struct _s8));

                if (sub_5a76(&var1b, false, DT_FLOAT)) 
		{
                    prWarning(79);
		    /* p12_a297->p25->attr.dataType = DT_DOUBLE; */
		    PutByte((char*)(p12_a297->p25)+OFF_s8_t_a_dataType, DT_DOUBLE);
                }
            }
        } 
	else if (byte_a299 != (char)D_14 && byte_a299 != (char)D_15) 
	{ /* 6ecd */

            if (p1 == (char)T_AUTO && var1b.c7 == (char)ANODE)
                p1 = T_EXTERN;

            if ((p12_a297->p25 = sub_4eed(p12_a297->p25, p1, &var1b, false, p4)))
		/* p12_a297->p25->m18 |= p3; */
		PutWord((char*)(p12_a297->p25)+OFF_m18, GetWord((char*)(p12_a297->p25)+OFF_m18) | p3);
        } 
	else 
	{
            if (GetByte((char*)(p12_a297->p25)+OFF_m20) &&
	        GetByte((char*)(p12_a297->p25)+OFF_m21) != depth) /* 6f39 */
                p12_a297->p25 = sub_4eed(p12_a297->p25, 0, &var1b, false, 0);

	    /* p12_a297->p25->attr = var1b; */
	    WriteMem((char*)&var1b, (char*)(p12_a297->p25), sizeof(struct _s8));
        }
    }

    /* 6f91 */
    var4     = p12_a297->p25;
    p12_a297 = var6;

    return var4;
}
