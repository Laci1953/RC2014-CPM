/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

uint16_t sub_742a(uint16_t n);
sym_t *sub_56a4(void);
sym_t *sub_4eed(register sym_t *st, uint8_t p2, s8_t *p3, bool v, sym_t *p4);
void expectErr(char *p);
void skipStmt(uint8_t tok);
void checkScopeExit(void);
expr_t *sub_0a83(uint8_t n);
uint8_t yylex(void);
void prError(int, ...);

// changes p12_a297->p8
void common_6fab(bool *v)
{
	if (*v)
	{
	    PutByte((char*)(p12_a297->p8)+OFF_ATTR_dataType, DT_POINTER);
	    PutWord((char*)(p12_a297->p8)+OFF_ATTR_i4, (short)sub_742a(p12_a297->i6));
	    PutWord((char*)(p12_a297->p8)+OFF_ATTR_nextSym, (short)sub_56a4());
	}
	else
	{
            p12_a297->p8->dataType  = DT_POINTER;
            p12_a297->p8->i4        = sub_742a(p12_a297->i6);
            p12_a297->p8->i_nextSym = sub_56a4();
	}

        p12_a297->i6            = 0;
		
	if (p12_a297->p25_1)
            p12_a297->p25_1 = sub_4eed(p12_a297->p25_1, T_TYPEDEF, &p12_a297->p25_1->attr, true, 0);

        p12_a297->p25_1 = *v ? (sym_t*)GetWord((char*)(p12_a297->p8)+OFF_ATTR_nextSym) : p12_a297->p8->i_nextSym;

	p12_a297->p8    = &p12_a297->p25_1->attr;
	*v = true;
}

/**************************************************
 * 141: 6FAB PMO +++
 * minor equivalent optimiser differences
 * differnces due to dummy & uint8_t args
 **************************************************/
void sub_6fab(uint8_t p1, bool *v) {
    uint8_t var2;
    uint8_t tok;
    uint8_t var4;
    bool var1;
    uint8_t var5;
    register expr_t *st;
    char c7;

    byte_a29a = 0;
    var4      = 1;
    var2      = 0;

    while ((tok = yylex()) == (char)T_STAR)
        var2++;

    if (tok == (char)T_ID) 
    {
        p12_a297->p25 = yylval.ySym;

        tok           = yylex();
        var4          = GetByte((char*)(p12_a297->p25)+OFF_m20);

        if (/* p12_a297->p25->m20 */ var4 == (char)0)
	    PutByte((char*)(p12_a297->p25)+OFF_m20, p1);

        byte_a29a = p1 != D_14;
    }
    else if (tok == (char)T_LPAREN) 
    { /* 701b */
        ungetTok = tok = yylex();

        if (tok == (char)T_RPAREN || tok == (char)S_CLASS || tok == (char)S_TYPE ||
            (tok == (char)T_ID && 
	     /* yylval.ySym->m20 */ GetByte((char*)(yylval.ySym)+OFF_m20) == (char)T_TYPEDEF))
            tok = T_LPAREN;
        else 
	{
            sub_6fab(p1, v);

            if ((tok = yylex()) != (char)T_RPAREN)
                expectErr(")");

            tok = yylex();
        }
    } /* 707b */

    p12_a297->ucb = p12_a297->p25 == NULL;

    for (;;) 
    { /* 7091 */
	if (*v)
	    c7 = GetByte((char*)(p12_a297->p8)+OFF_ATTR_c7);
	else
	    c7 = p12_a297->p8->c7;

	if (tok == (char)T_LPAREN) 
	{
            if (p12_a297->uc9) 
	    {
                prError(4);
                p12_a297->uc9 = false;
            }

            if (/* p12_a297->p8->c7 */ c7 == (char)ANODE && p12_a297->i6 == 0)
                prError(3);

            if (p12_a297->i6 & 0x8000) 
	    { /* 70e7 */
		common_6fab(v);	//changes p12_a297->p8
            } /* 7195 */

            /* p12_a297->p8->c7 = ANODE; */
	    if (*v)
		PutByte((char*)(p12_a297->p8)+OFF_ATTR_c7, ANODE);
	    else
		p12_a297->p8->c7 = ANODE;

            byte_a29a        = byte_a29a && !depth;

            if (!byte_a29a) 
	    {
                var5 = true;
                depth++;
            } 
	    else
                var5 = false;

            /* 71cc */
            depth++;

            if ((tok = yylex()) == (char)S_CLASS || tok == (char)S_TYPE || tok == (char)T_ID) 
	    {
                ungetTok             = tok;

		if (*v)
		    PutWord((char*)(p12_a297->p8)+OFF_ATTR_pArgs, (short)sub_65e2(byte_a29a));
		else
		    p12_a297->p8->i_args = sub_65e2(byte_a29a);
            }
	    else if (tok != (char)T_RPAREN) 
	    {
                expectErr(")");
                skipStmt(tok);
            }

            byte_a29a = false;

            if (var5) 
	    {
                checkScopeExit();
                depth--;
            }

            depth--;
        } 
	else if (tok == (char)T_LBRACK) 
	{ /* 7248 */
            if (/* p12_a297->p8->c7 */ c7 == (char)ANODE)
                prError(2);

            if (p12_a297->uca || (p12_a297->i6 & 0x8000)) 
	    {
                p12_a297->uca           = false;
		common_6fab(v);	//changes p12_a297->p8
            } /* 732a */

            var1      = byte_8f86;
            byte_8f86 = false;

            if ((tok = yylex()) == (char)T_RBRACK) 
	    {
                if (p12_a297->uc9)
                    prError(1);

                st = &s13_9d1b;
            } 
	    else 
	    {
                ungetTok = tok;
                st       = sub_0a83(T_SEMI);

                if ((tok = yylex()) != (char)T_RBRACK) 
		{
                    expectErr("]");
                    skipStmt(tok);
                }
            } /* 738a */

            p12_a297->uca        = true;
            p12_a297->uc9        = true;
            byte_8f86            = var1;

	    if (*v)
	    {
		PutByte((char*)(p12_a297->p8)+OFF_ATTR_c7, 1);
		PutWord((char*)(p12_a297->p8)+OFF_ATTR_pExpr, (short)st);
	    }
	    else
	    {
            	p12_a297->p8->c7     = 1;
            	p12_a297->p8->i_expr = st;
	    }
        } 
	else 
	{ /* 73c1 */
            ungetTok = tok;

            if (!var4)
		PutByte((char*)(p12_a297->p25)+OFF_m20, 0);

            if (!var2)
                return;

            p12_a297->uc9 = false;

            do 
	    {
                if (p12_a297->i6 & 1)
                    p12_a297->uc8 = true;
                else
                    p12_a297->i6 = (p12_a297->i6 >> 1) | 0x8000;
            } while (--var2);

            return;
        }

        tok = yylex();
    }
}
