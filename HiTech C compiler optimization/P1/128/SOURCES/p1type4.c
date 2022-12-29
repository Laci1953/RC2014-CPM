/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

void sub_5be1(register s8_t *st, bool vst);
uint8_t yylex(void);
sym_t *sub_56a4(void);
void sub_516c(register sym_t *st);
void sub_573b(register sym_t *st, FILE *fp);
expr_t *sub_1b4b(long num, uint8_t p2);
void expectErr(char *p);
sym_t *sub_4eed(register sym_t *st, uint8_t p2, s8_t *p3, bool v, sym_t *p4);
void sub_2569(register expr_t *st);
expr_t *sub_0a83(uint8_t n);
bool sub_5b08(register s8_t *st);
void sub_05d3(expr_t *p1);
expr_t *sub_25f7(register expr_t *st);
void skipStmt(uint8_t tok);
bool sub_5aa4(register s8_t *st);
void skipToSemi(void);
expr_t *sub_1441(uint8_t p1, register expr_t *lhs, expr_t *rhs); 
expr_t *sub_07f5(char p1);
void sub_042d(register expr_t *p);
void sub_58bd(register s8_t *st, bool vst, s8_t *p2, bool vp2);
bool sub_5a76(register s8_t *st, bool vst, uint8_t p2);
args_t *sub_578d(register args_t *p);
expr_t *allocId(register sym_t *st);
void prError(int, ...);

/**************************************************
 * 137: 6360 PMO +++
 * differences due to dummy and uint8_t args
 **************************************************/
sym_t *sub_6360(void) {
    s8_t var8;
    sym_t *vara;
    expr_t *varc;
    int16_t vare;
    uint8_t tok;
    register sym_t *st;
    char tmpbuf[32];

    if ((tok = yylex()) == (char)T_ID) {
        st = yylval.ySym;
        if ((tok = yylex()) != (char)T_LBRACE) {
            if (!(/* st->m18 */ GetWord((char*)st+OFF_m18) & 1))
	    {
		GetString(tmpbuf, (char*)GetWord((char*)st+OFF_nVName));
                prError(100+20, /* st->nVName */ tmpbuf);
	    }
            ungetTok = tok;
        }
    } else if (tok == (char)T_LBRACE)
        st = sub_56a4();
    else {
        expectErr("enum tag or {");
        st = NULL;
    }
    if (tok == (char)T_LBRACE) {
        sub_516c(st = sub_4eed(st, D_ENUM, 0, false, 0));
        var8.dataType  = DT_ENUM;
        var8.i_nextSym = st;
        var8.i4        = 0;
        var8.i_info    = 0;
        var8.c7        = 0;
        printf("[c ");
        sub_573b(st, stdout);
        putchar('\n');
        vare = 0;
        varc = sub_1b4b(0, DT_INT);
        for (;;) {
            if ((tok = yylex()) != (char)T_ID) {
                expectErr("identifier");
                break;
            } else { /* 6474 */
                if ((vara = sub_4eed(yylval.ySym, DT_CONST, &var8, false, st)))
                    /* vara->m14 = vare++; */
		    PutWord((char*)vara+OFF_m14, vare++);
                if ((tok = yylex()) == (char)T_EQ) {
                    sub_2569(varc);
                    sub_0a83(T_LBRACE);
                    if (!sub_5b08(&varc->attr) || varc->attr.dataType >= (char)DT_LONG)
                        prError(19);
                    tok = yylex();
                }
                sub_516c(vara);
                sub_05d3(varc);
                if (tok != (char)T_COMMA)
                    break;
                varc = sub_25f7(varc);
            }
        }
        printf(".. ]\n");
        sub_2569(varc);
        if (tok != (char)T_RBRACE) {
            expectErr("}");
            skipStmt(tok);
        }
    }

    return st;
}
/**************************************************
 * 138: 6531 PMO +++
 * differences due to dummy and uint8_t args
 **************************************************/
void sub_6531(register sym_t *st) {
    expr_t *var2;
    uint8_t tok;
    bool var4;
    s8_t buf8;

    ReadMem((char*)&buf8, (char*)st, sizeof(struct _s8));

    if (st && !sub_5aa4(/* &st->attr */ &buf8)) {
        prError(18);
        skipToSemi();
    } else {
        if (!(var4 = (tok = yylex()) == (char)T_LBRACE))
            ungetTok = tok;
        if ((var2 = sub_1441(T_60, sub_07f5(T_RBRACE), 0)) && st) {
            var2 = sub_1441(T_EQ, allocId(st), var2);
            sub_042d(var2);
            sub_2569(var2);
        }
        if (var4 && yylex() != (char)T_RBRACE)
            expectErr("}");
    }
}

/**************************************************
 * 139: 65E2 PMO +++
 * equivalent optimiser differences including basic
 * block moves
 * differences due to dummy and uint8_t args
 **************************************************/
args_t *sub_65e2(uint16_t p1) {
    uint8_t scType;
    s8_t attr;
    s8_t *varb;
    uint8_t tok;
    sym_t *vare=0;
    uint8_t scFlags;
    uint8_t var10;
    bool var11;
    bool var12;
    s8_t var1a;
    int16_t var1c;
    struct {
        int16_t cnt;
        s8_t s8array[128];
    } args;
    register sym_t *st;
    char tmpbuf[32];

    var10          = byte_a299;
    byte_a299      = D_15;
    args.cnt       = 0;
    var12          = false;
    var11          = false;
    var1a.dataType = DT_INT;
    var1a.i4       = 0;
    var1a.i_expr   = 0;
    var1a.c7       = 0;

    for (;;) 
    { /* 6619 */
        if ((tok = yylex()) == (char)T_3DOT) 
	{
            args.s8array[args.cnt].dataType = DT_VARGS;
            args.s8array[args.cnt].i_expr   = 0;
            args.s8array[args.cnt].c7       = 0;
            args.s8array[args.cnt++].i4     = 0;
            tok                             = yylex();
            break;
        } /* 66db */
        if (tok == (char)T_ID &&
	    /* yylval.ySym->m20 */ GetByte((char*)(yylval.ySym)+OFF_m20) != (char)T_TYPEDEF)
            if (p1)
                var12 = true;
            else
                prError(17);
        else
            var11 = true;

        if (var11 && var12) 
	{
            var12 = false;
            prError(16);
        }

        ungetTok = tok;
        scFlags  = sub_5dd1(&scType, &attr);

        if (scType != (char)D_15 && scType != (char)T_REGISTER)
            prError(15);

        scType = p1 ? D_15 : D_14;
        st     = sub_69ca(scType, &attr, scFlags & ~1, 0);

        varb   = &st->attr;

        if (/* varb->c7 */ GetByte((char*)varb+OFF_ATTR_c7) == (char)ANODE) 
	{
            /* varb->i_nextSym = sub_4eed(sub_56a4(), T_TYPEDEF, varb, false, 0); */
	    PutWord((char*)varb+OFF_ATTR_nextSym, (short)sub_4eed(sub_56a4(), T_TYPEDEF, varb, true, 0));
            /* varb->dataType  = DT_POINTER; */
	    PutByte((char*)varb+OFF_ATTR_dataType, DT_POINTER);
            /* varb->c7        = 0; */
	    PutByte((char*)varb+OFF_ATTR_c7, 0);
            /* varb->i4        = 1; */
	    PutWord((char*)varb+OFF_ATTR_i4, 1);
        }

        if (var11)
            sub_58bd(varb, true, &args.s8array[args.cnt++], false);

        scFlags &= ~1;
        scType = D_6;
        st     = sub_4eed(st, scType, var11 ? varb : &var1a, var11 ? true : false, 0);

        if (p1 && !sub_5a76(varb, true, T_AUTO)) 
	{
            if (!p25_a28f) 
	    {
                p25_a28f = st;
                vare     = st;
                /* st->m18 |= scFlags | 0x20; */
		PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) | scFlags | 0x20);
            } else if (/* st->m18 */ GetWord((char*)st+OFF_m18) & 0x20) /* 6893 */
	    {
		GetString(tmpbuf, (char*)GetWord((char*)st+OFF_nVName));
                prError(100+14, /*st->nVName */ tmpbuf);
	    }
            else 
	    {
                /* vare->nMemberList = st; */
		if (vare)
			PutWord((char*)vare+OFF_nMemberList, (short)st);

                /* st->m18 |= scFlags | 0x20; */
		PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) | scFlags | 0x20);

                vare = st;
            }

            if (var11)
	    {
                /* st->m18 |= 0x208; */
		PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) | 0x208);
	    }

            /* st->nMemberList = 0; */
	    PutWord((char*)st+OFF_nMemberList, 0);
        }
        if ((tok = yylex()) == (char)T_EQ) {
            prError(13);
            skipStmt(tok);
        }
        if (tok == (char)T_ID || tok == (char)S_CLASS || tok == (char)S_TYPE) {
            expectErr(",");
            ungetTok = tok;
        } else if (tok != (char)T_COMMA)
            break;
    } /* 669c */
    byte_a299 = var10;
    if (tok != (char)T_RPAREN) {
        expectErr(")");
        skipStmt(tok);
    }
    if (args.cnt == 0)
        return 0;
    var1c = args.cnt;
    while (var1c--) {
        if (args.s8array[var1c].c7 == (char)ENODE) {
            args.s8array[var1c].c7 = SNODE;
            sub_2569(args.s8array[var1c].i_expr);
            sub_5be1(&args.s8array[var1c], false);
        }
    }
    return sub_578d((args_t *)&args);
}
