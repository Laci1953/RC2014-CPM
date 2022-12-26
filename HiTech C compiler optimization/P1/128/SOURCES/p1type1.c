/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

uint8_t yylex(void);
void sub_516c(register sym_t *st);
void sub_0493(register sym_t *st);
void sub_3c7e(sym_t *p1);
void expectErr(char *p);
void skipStmt(uint8_t tok);
void prError(int, ...);

/**************************************************
 * 132: 5BE1 PMO +++
 **************************************************/
// st may be in Upper RAM
void sub_5be1(register s8_t *st, bool vst) {
    uint16_t ch;
    int i4;

    if (vst)
	i4 = GetByte((char*)st+OFF_ATTR_i4);
    else
	i4 = st->i4;

    ch = i4;

    if (ch & 0x8000)
        prError(35);

    ch = (ch << 1) | 1;

    if (vst)
	PutByte((char*)st+OFF_ATTR_i4, ch);
    else
    	st->i4 = ch;
}

/**************************************************
 * 133: 5C19 PMO +++
 **************************************************/
void sub_5c19(uint8_t p1) {
    /* register sym_t *st; */ /* may not be needed */
    register uint8_t tok;

    byte_a299 = p1;
    for (;;) {
        ungetTok = tok = yylex();
        if (tok == (char)S_CLASS || tok == (char)S_TYPE ||
            (tok == (char)T_ID &&
	    /* (st = yylval.ySym)->m20 */ GetByte((char*)(yylval.ySym)+OFF_m20) == (char)T_TYPEDEF))
            sub_5c50();
        else
            break;
    }
    byte_a299 = 0;
}

/**************************************************
 * 134: 5C50 PMO +++
 * trivial optimiser differences, use of uint8_t arg
 * and dummy arg
 **************************************************/
void sub_5c50(void) {
    uint8_t scType;
    s8_t var9;
    uint8_t tok;
    uint8_t scFlags;
    uint8_t varc;
    bool vard;
    register sym_t *st;

    scFlags = sub_5dd1(&scType, &var9);

    if (scType != (char)D_6 && scType != (char)T_REGISTER && byte_a299 == (char)D_6) {
        prError(34);
        scType = D_6;
    }

    if ((tok = yylex()) == (char)T_SEMI)
        return;

    ungetTok = tok;

    for (;;) 
    {
        st   = sub_69ca(scType, &var9, scFlags & ~1, 0); /* dummy last param */
        vard = st && (/* st->m18 */ GetWord((char*)st+OFF_m18) & 0x10) &&
		      /* st->attr.c7 */ GetByte((char*)st+OFF_s8_t_c7) == (char)ANODE;
        varc = (scFlags & 1) && scType != (char)D_6 && !vard;

        if ((tok = yylex()) == (char)T_EQ) 
	{
            if (vard || scType == (char)D_6 || scType == (char)T_EXTERN || scType == (char)T_TYPEDEF)
                prError(33);

            if (scType == (char)T_STATIC || scType == (char)T_EXTERN) 
	    {
                sub_516c(st);

                if (depth && scType == (char)T_STATIC)
		{
                    /* st->m18 |= 0x80; */
		    PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) | 0x80);
		}

                sub_0493(st);
                sub_3c7e(st);
            } else 
	    {
                sub_516c(st);
                sub_0493(st);
                sub_6531(st);
            }
            tok = yylex();
        }
	else if (scType != (char)D_6) 
	{
            if (varc)
                sub_516c(st);

            if (depth && scType == (char)T_STATIC)
	    {
                /* st->m18 |= 0x80; */
	        PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) | 0x80);
	    }

            sub_0493(st);
        } /* 5d95 */
        if (tok == (char)T_ID || tok == (char)T_STAR) 
	{
            expectErr(",");
            ungetTok = tok;
        } 
	else if (tok != (char)T_COMMA) 
	{
            if (tok != (char)T_SEMI) 
	    {
                expectErr(";");
                skipStmt(tok);
            }
            return;
        }
    }
}
