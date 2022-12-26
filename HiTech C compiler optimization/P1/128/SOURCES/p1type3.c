/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

uint8_t yylex(void);
sym_t *sub_4eed(register sym_t *st, uint8_t p2, s8_t *p3, bool v, sym_t *p4);
void expectErr(char *p);
void sub_516c(register sym_t *st);
bool sub_5a76(register s8_t *st, bool vst, uint8_t p2);
void sub_0353(sym_t *p, char c);
sym_t *sub_56a4(void);
void prError(int, ...);

/**************************************************
 * 136: 60DB PMO +++
 * differences due to dummy args, use of uint8_t args
 * and equivalent optimiser differences.
 **************************************************/
sym_t *sub_60db(uint8_t p1) {
    sym_t *var2;
    /* sym_t **var4; */
    sym_t *var4; // !!! but will be used as &...->nMemberList
    uint8_t tok;
    int16_t var7;
    s8_t varF;
    register sym_t *st;
    char tmpbuf[32];

    byte_8f85 = true;
    tok       = yylex();
    byte_8f85 = false;

    if (tok == (char)T_ID) 
    {
        st = yylval.ySym;

        if (/* st->m20 */ GetByte((char*)st+OFF_m20) != p1)
            st = sub_4eed(st, p1, 0, false, 0);

        tok = yylex();
    } 
    else 
    {
        st = sub_4eed(sub_56a4(), p1, 0, false, 0);

        if (tok != (char)T_LBRACE)
            expectErr("struct/union tag or '{'");
    }

    var4 = 0;

    if (tok == (char)T_LBRACE) 
    {
        if ((/* st->m18 */ GetWord((char*)st+OFF_m18) & 0x81) == 1)
	{
	    GetString(tmpbuf, (char*)GetWord((char*)st+OFF_nVName));
            prError(100+24, tmpbuf);
	}
        else
            /* var4 = &st->nMemberList; */
	    var4 = st;		

        var7 = 0;

        for (;;) 
	{
            sub_5dd1(0, &varF);
            byte_8f86 = true;

            do 
	    {
                var2 = sub_69ca(D_MEMBER, &varF, 0, st);

                if (var2) 
		{
                    if (/* var2->attr.c7 */ GetByte((char*)var2+OFF_s8_t_c7) == (char)ANODE)
                        prError(23);

                    sub_516c(var2);

                    if (var4) 
		    {
			/*-------------------------
                        *var4 = var2;
                        var4  = &var2->nMemberList;
			--------------------------- */
			PutWord((char*)var4+OFF_nMemberList, (short)var2);
			var4 = var2;
                    }

                    /* var2->m14 = var7++; */
		    PutWord((char*)var2+OFF_m14, (short)var7++);
                }

                if ((tok = yylex()) == (char)T_COLON) 
		{
                    if (!(/* var2->attr.dataType */ GetByte((char*)var2+OFF_s8_t_a_dataType) & DT_UNSIGNED))
		    {
                        /* var2->attr.dataType |= DT_UNSIGNED; */
			PutByte((char*)var2+OFF_s8_t_a_dataType, GetByte((char*)var2+OFF_s8_t_a_dataType) | DT_UNSIGNED);
		    }

                    if (!sub_5a76(&var2->attr, true, DT_UINT))
                        prError(22);

                    if ((tok = yylex()) != (char)T_ICONST)
                        prError(21);
                    else 
		    {
                        if (!var2) 
			{
                            sub_516c(var2 = sub_4eed(sub_56a4(), D_MEMBER, &varF, false, st));

                            if (var4) 
			    {
				/* ------------------------
                                *var4 = var2;
                                var4  = &var2->nMemberList;
				----------------------------- */
				PutWord((char*)var4+OFF_nMemberList, (short)var2);
				var4 = var2;
                            }

                    	    /* var2->m14 = var7++; */
		    	    PutWord((char*)var2+OFF_m14, var7);

		    	    var7++;
                        } /* 62ce */

                        /* var2->m18 |= 0x400; */
			PutWord((char*)var2+OFF_m18, GetWord((char*)var2+OFF_m18) | 0x400);

                        /* var2->m16 = (int16_t)yylval.yNum; */
			PutWord((char*)var2+OFF_m16, (int16_t)yylval.yNum);

                        tok       = yylex();
                    }
                }

            } while (tok == (char)T_COMMA); /* 62f2 */

            byte_8f86 = false;

            if (tok != (char)T_SEMI)
                expectErr(";");

            if ((tok = yylex()) == (char)T_RBRACE) 
	    {
                if (!(/* st->m18 */ GetWord((char*)st+OFF_m18) & 1))
                    sub_516c(st);

                if (var4) 
		{
                    /* *var4 = st; */
		    PutWord((char*)var4+OFF_nMemberList, (short)st);

                    sub_0353(st, p1);
                }

                return st;
            }
            ungetTok = tok;

        } /* for */

    } /* if */

    ungetTok = tok;

    return st;
}
