/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

int8_t depth;       /* a288 */
uint8_t byte_a289;  /* a289 */
bool unreachable;   /* a28a */
int16_t word_a28b;  /* a28b */
sym_t *curFuncNode; /* a28d */
sym_t *p25_a28f;    /* ad8f */

expr_t *allocIConst(long p1);
uint8_t sub_5dd1(uint8_t *pscType, register s8_t *attr);
uint8_t yylex(void);
sym_t *sub_69ca(uint8_t p1, register s8_t *p2, uint8_t p3, sym_t *p4);
bool sub_2105(register expr_t *st);
expr_t *allocSType(s8_t *p1);
expr_t *sub_1441(uint8_t p1, register expr_t *lhs, expr_t *rhs);
expr_t *sub_0a83(uint8_t n);
void sub_05b5(expr_t *p1);
void prError(int, ...);

/**************************************************
 * 81: 3ADF +++
 * minor differences due to adding missing arg and use
 * of uint8_t paramater
 * one use of add a,255 vs sub a,1 i.e. equivalent
 **************************************************/
void sub_3adf(void) {
    uint8_t tok;
    uint8_t scType;
    s8_t attr;
    uint8_t varb;
    uint8_t scFlags;
    register sym_t *st;
    sym_t symbuf;
    sym_t* ps = &symbuf;

    scFlags = sub_5dd1(&scType, &attr);
 
    if ((tok = yylex()) == (char)T_SEMI)
        return;
    ungetTok = tok;
    varb     = true;

    for (;;) {
        p25_a28f = 0;
        st       = sub_69ca(scType, &attr, scFlags & ~1, 0);
        tok      = yylex();

	if (st)
	    ReadMem((char*)&symbuf, (char*)st, sizeof(struct _sym));
 
        if (st && (/* st->m18 */ ps->m18 & 0x10) &&
		   /* st->a_c7 */ ps->a_c7 == (char)ANODE) 
	{
            if (varb && tok != (char)T_COMMA && tok != (char)T_SEMI && scType != (char)T_TYPEDEF) 
	    {
                byte_a289 = /* st->a_c7 */ ps->a_c7 == (char)ANODE &&
			    /* st->a_i4 */ ps->a_i4 == (char)0 &&
                            (/* st->a_dataType */ ps->a_dataType == (char)DT_VOID ||
			     /* st->a_dataType */ ps->a_dataType == (char)DT_INT);
                ungetTok = tok;
                sub_516c(st);
                sub_0493(st);
                curFuncNode = st;
                sub_409b();
                return;
            }
            if (p25_a28f && !(/* p25_a28f->m18 */ GetWord((char*)p25_a28f+OFF_m18) & 8))
                expectErr("function body");
            ++depth;
            checkScopeExit();
            --depth;
            sub_0493(st);
        } else if (tok == (char)T_EQ) {
            if (scType == (char)T_TYPEDEF)
                /* prError("illegal initialisation"); */
		prError(47);
            sub_516c(st);
            sub_0493(st);
            sub_3c7e(st);
            tok = yylex();
        } else if (st) {
            if ((scFlags & 1) || /* st->m20 */ ps->m20 != (char)T_EXTERN) {
                sub_516c(st);
                sub_0493(st);
            } else
                sub_01ec(st);
        }
        if (tok == (char)T_ID || tok == (char)T_STAR) {
            expectErr(",");
            ungetTok = tok;
        } else if (tok != (char)T_COMMA)
            break;
        varb = false;
    }

    if (tok != (char)T_SEMI) {
        expectErr(";");
        while (tok > (char)T_RBRACE)
            tok = yylex();
    }
}

/**************************************************
 * 82: 3C7E PMO +++
 * use of uint8_t param
 **************************************************/
void sub_3c7e(sym_t *p1) {
    int16_t var2;
    sym_t *st;
    expr_t* ex;

    if (p1) {
        printf("[i ");
        sub_573b(p1, stdout);
        putchar('\n');
        st = p1;
        if ((var2 = sub_3d24(st, 1)) < 0) {
            /* prError("initialisation syntax"); */
	    prError(46);
            skipToSemi();
        } else if (/* st->a_c7 */ GetByte((char*)st+OFF_s8_t_c7) == (char)ENODE &&
		   /* st->a_expr */ (ex = (expr_t*)GetWord((char*)st+OFF_s8_t_pExpr)) &&
		   sub_2105(/* st->a_expr */ ex)) 
	{
            sub_2569(/* st->a_expr */ ex);
            /* st->a_expr = allocIConst(var2); */
	    PutWord((char*)st+OFF_s8_t_pExpr, (short)allocIConst(var2));
        }
        printf("]\n");
    } else
        skipToSemi();
}

/**************************************************
 * 83: 3D24 +++
 * minor optimiser differences including moving basic
 * blocks. Use of uint8_t parameter
 **************************************************/
int16_t sub_3d24(sym_t *st, uint8_t p2) {
    int16_t var2;
    uint8_t tok;
    char *var5;
    bool haveLbrace;
    sym_t *var8;
    sym_t *vara;
    bool varb;
    expr_t *vard;
    bool vare;
    sym_t symbuf;
    register sym_t* ps = &symbuf;

    /* manual string optimisation */
    static char Ustr[] = ":U ..\n";
    static char Dstr[] = "..\n";

    ReadMem((char*)&symbuf, (char*)st, sizeof(struct _sym));

    var2 = -1;

    if (p2 &&
       /* st->a_c7 */ ps->a_c7 == (char)ENODE &&
       /* st->a_expr */ ps->a_expr) 
    {
        printf(":U ..\n");

        if ((haveLbrace = ((tok = yylex()) == (char)T_LBRACE)))
            tok = yylex();

        if (tok == (char)T_SCONST &&
	    /* st->attr.i4 */ ps->attr.i4 == 0 &&
	    (/* st->attr.dataType */ ps->attr.dataType & ~1) == 4) 
	{
            var2 = 0;
            var5 = yylval.yStr;

            while (var2 < strChCnt) 
	    {
                printf("-> %d `c\n", *var5++);
                ++var2;
            }

            free(yylval.yStr);

            if (sub_2105(/* st->a_expr */ ps->a_expr)) 
	    {
                printf("-> %d `c\n", 0);
                ++var2;
            }

            if (haveLbrace)
                tok = yylex();

        } 
	else if (!haveLbrace) /* 3e4a */
            expectErr("{");
        else 
	{
            ungetTok = tok;

            if (/* st->a_i4 */ ps->a_i4 == 0 &&
	        /* st->a_dataType */ ps->a_dataType == (char)DT_POINTER)
	    {
                st = /* st->a_nextSym */ ps->a_nextSym;
	    }
            else
                p2 = 0;

            var2 = 0;

            for (;;) 
	    {
                if (sub_3d24(st, p2) < 0)
                    break;

                var2++;

                if ((tok = yylex()) == (char)T_RBRACE ||
		     tok != (char)T_COMMA ||
		    (tok = yylex()) == (char)T_RBRACE)
                    break;
                ungetTok = tok;
            }
        }
        /* 3e22 */

        if (haveLbrace && tok != (char)T_RBRACE) 
	{
            expectErr("}");
            var2 = -1;
        }

        printf(Dstr);
    } 
    else if ((p2 == 0 ||
	       /* st->a_c7 */ ps->a_c7 != (char)ENODE) &&
	       /* st->a_i4 */ ps->a_i4 == (char)0 &&
               /* st->a_dataType */ ps->a_dataType == (char)DT_STRUCT) 
    { /* 3ec6 */
        if (p2)
            printf(Ustr);

        printf(Ustr);

        if ((var8 = /* st->a_nextSym */ ps->a_nextSym)) 
	{
            varb = (tok = yylex()) == (char)T_LBRACE;

            if (!varb)
                ungetTok = tok;

            vara = /* var8->nMemberList */ (sym_t*)GetWord((char*)var8+OFF_nMemberList);

            do {
                if ((sub_3d24(vara, 1) < 0))
                    break;

                if ((vara = /* vara->nMemberList */ (sym_t*)GetWord((char*)vara+OFF_nMemberList)) == var8)
                    break;

                if ((tok = yylex()) != (char)T_COMMA) 
		{
                    ungetTok = tok;
                    break;
                }
            } 
	    while (!varb || (ungetTok = tok = yylex()) != (char)T_RBRACE);

            /* 3f7c: */
            if (varb) 
	    {
                if ((tok = yylex()) == (char)T_COMMA)
                    tok = yylex();

                if (tok != (char)T_RBRACE)
                    expectErr("}");
                else
                    var2 = 1;
            } else
                var2 = 1;
        } /* 3fcd */

        printf(Dstr);

        if (p2)
            printf(Dstr);

    } 
    else if ((p2 &&
	       /* st->attr.c7 */ ps->attr.c7 == (char)ENODE) ||
 	       /* st->attr.c7 */ ps->attr.c7 == (char)ANODE ||
               (!(/* st->a_i4 */ ps->a_i4 & 1) &&
		  /* st->attr.dataType */ ps->attr.dataType >= (unsigned char)T_AUTO))
        /* prError("illegal initialisation"); */
	prError(45);
    else 
    {
        vare = (tok = yylex()) == (char)T_LBRACE;

        if (!vare)
            ungetTok = tok;

	vard          = allocSType(/* &st->attr */ &symbuf.attr);
        vard->attr.c7 = SNODE;

        if ((vard = sub_1441(T_125, vard, sub_0a83(3))))
            sub_05b5(vard->t_alt);

        if (vare && yylex() != (char)T_RBRACE)
            expectErr("}");

        sub_2569(vard);
        var2 = 1;
    }

    return var2;
}
