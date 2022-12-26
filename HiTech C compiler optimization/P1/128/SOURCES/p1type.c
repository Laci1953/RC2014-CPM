/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

void sub_5c50(void);
void sub_6531(register sym_t *st);
args_t *sub_65e2(uint16_t p1);
void sub_6fab(uint8_t p1);
uint16_t sub_742a(uint16_t n);

/**************************************************
 * 132: 5BE1 PMO +++
 **************************************************/
void sub_5be1(register s8_t *st) {
    uint16_t ch;

    if (IsValid(st))
	ch = GetByte((char*)st+OFF_s8_t_a_i4);
    else
    	ch = st->i4;

    if (ch & 0x8000)
        prError("too much indirection");

    if (IsValid(st))
        PutByte((char*)st+OFF_s8_t_a_i4, (ch << 1) | 1);
    else
    	st->i4 = (ch << 1) | 1;
}

/**************************************************
 * 133: 5C19 PMO +++
 **************************************************/
void sub_5c19(uint8_t p1) {
    register sym_t *st; /* may not be needed */
    uint8_t tok;

    byte_a299 = p1;
    for (;;) {
        ungetTok = tok = yylex();
        if (tok == S_CLASS || tok == S_TYPE ||
            (tok == T_ID && /* (st = yylval.ySym)->m20 */ GetByte((char*)(st = yylval.ySym)+OFF_m20) == (char)T_TYPEDEF))
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
    int tmp;

    scFlags = sub_5dd1(&scType, &var9);

    if (scType != D_6 && scType != T_REGISTER && byte_a299 == D_6) {
        prError("only register storage class allowed");
        scType = D_6;
    }

    if ((tok = yylex()) == T_SEMI)
        return;

    ungetTok = tok;

    for (;;) 
    {
        st   = sub_69ca(scType, &var9, scFlags & ~1, 0); /* dummy last param */
        vard = st && (/* st->m18 */ GetWord((char*)st+OFF_m18) & 0x10) &&
		      /* st->attr.c7 */ GetByte((char*)st+OFF_s8_t_c7) == (char)ANODE;
        varc = (scFlags & 1) && scType != D_6 && !vard;

        if ((tok = yylex()) == T_EQ) 
	{
            if (vard || scType == D_6 || scType == T_EXTERN || scType == T_TYPEDEF)
                prError("illegal initialisation");

            if (scType == T_STATIC || scType == T_EXTERN) 
	    {
                sub_516c(st);

                if (depth && scType == T_STATIC)
		{
                    /* st->m18 |= 0x80; */
		    tmp = GetWord((char*)st+OFF_m18);
		    PutWord((char*)st+OFF_m18, tmp | 0x80);
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
	else if (scType != D_6) 
	{
            if (varc)
                sub_516c(st);

            if (depth && scType == T_STATIC)
	    {
                /* st->m18 |= 0x80; */
	        tmp = GetWord((char*)st+OFF_m18);
	        PutWord((char*)st+OFF_m18, tmp | 0x80);
	    }

            sub_0493(st);
        } /* 5d95 */
        if (tok == T_ID || tok == T_STAR) 
	{
            expectErr(",");
            ungetTok = tok;
        } 
	else if (tok != T_COMMA) 
	{
            if (tok != T_SEMI) 
	    {
                expectErr(";");
                skipStmt(tok);
            }
            return;
        }
    }
}

/**************************************************
 * 135: 5DD1 PMO +++
 * use of uint8_t arg
 * some optimisations different some better some worse
 **************************************************/
uint8_t sub_5dd1(uint8_t *pscType, register s8_t *attr) {
    uint8_t scType;
    uint8_t dataType;
    int16_t sizeIndicator; /* -1 short, 0 int, 1 long */
    bool isUnsigned;
    uint8_t tok;
    uint8_t scFlags; /* storage class */
    sym_t *var9;
    sym_t *ps;
    struct _s8 tmp;

    attr->i4        = 0;
    attr->i_sym     = 0;
    attr->i_nextSym = NULL;
    attr->c7        = 0;
    scType = dataType = 0;
    sizeIndicator     = 0;
    isUnsigned        = false;
    scFlags           = 0;

    for (;;) {
        if ((tok = yylex()) == S_CLASS) {
            if (pscType == NULL)
                prError("storage class illegal");
            else {
                switch (tok = yylval.yVal) {
                case T_REGISTER:
                    scFlags |= 4;
                    break;
                case T_AUTO:
                    if (!depth)
                        prError("bad storage class");
                    /* FALLTHRU */
                case T_STATIC:
                case T_TYPEDEF:
                    scFlags |= 1;
                    /* FALLTHRU */
                case T_EXTERN:
                    if (scType && scType != tok)
                        prError("inconsistent storage class");
                    else
                        scType = tok;
                    break;
                }
            }
        } else if (tok == S_TYPE) { /* 5e78 */
            switch (tok = yylval.yVal) {
            case T_SHORT:
                sizeIndicator--;
                break;
            case T_LONG:
                sizeIndicator++;
                break;
            case T_UNSIGNED:
                isUnsigned = true;
                break;
            case T_UNION:
                dataType        = DT_UNION;
                attr->i_nextSym = sub_60db(D_UNION);
                if (attr->i_nextSym)
                    sub_51cf(attr->i_nextSym);
                break;
            case T_STRUCT:
                dataType        = DT_STRUCT;
                attr->i_nextSym = sub_60db(D_STRUCT);
                if (attr->i_nextSym)
                    sub_51cf(attr->i_nextSym);
                break;
            case T_ENUM:
                dataType        = DT_ENUM;
                attr->i_nextSym = sub_6360();
                sub_51cf(attr->i_nextSym);
                break;
            case T_CHAR:
            case T_DOUBLE:
            case T_FLOAT:
            case T_INT:
            case T_VOID:
                if (dataType)
                    prError("inconsistent type");
                else
                    dataType = tok == T_INT     ? DT_INT
                               : tok == T_CHAR  ? DT_CHAR
                               : tok == T_VOID  ? DT_VOID
                               : tok == T_FLOAT ? DT_FLOAT
                                                : DT_DOUBLE;
                break;
            }
        } else if (tok == T_ID && /* yylval.ySym->m20 */ GetByte((char*)yylval.ySym+OFF_m20) == T_TYPEDEF && dataType == 0) { /* 5f68 */
            ps = yylval.ySym;
            sub_51cf(ps);
            var9 = ps;
            if (/* var9->a_c7 */ GetByte((char*)var9+OFF_s8_t_c7)) {
                dataType        = DT_POINTER;
                attr->i_nextSym = ps;
            } else 
	    {
		ReadMem(&tmp, var9, sizeof(struct _s8));
                dataType = /* var9->a_dataType */ tmp.dataType;
                attr->u1 = /* var9->attr.u1; */ tmp.u1;  
                attr->u2 = /* var9->attr.u2; */ tmp.u2;
                attr->i4 = /* var9->attr.i4; */ tmp.i4;
            }
        } else
            break;

    } /* 6003 */
    ungetTok = tok;
    if (scType == 0) {
        scType = depth ? byte_a299 : T_EXTERN;
        scFlags |= 1;
    }
    if ((scFlags & 4) && scType != T_AUTO && scType != D_6 && scType != D_14 && scType != D_15)
        prError("can't be a register");
    if (dataType == 0)
        dataType = DT_INT;
    if (sizeIndicator > 0) {
        if (dataType == DT_FLOAT || dataType == DT_INT)
            dataType += 2; /* to DT_DOUBLE or DT_LONG*/
        else
            prError("can't be long");
    } else if (sizeIndicator < 0) {
        if (dataType == DT_INT)
            dataType = DT_SHORT;
        else
            prError("can't be short");
    }
    if (isUnsigned) {
        if (dataType < DT_FLOAT)
            dataType |= 1;
        else
            prError("can't be unsigned");
    }
    if (pscType)
        *pscType = scType;
    attr->dataType = dataType;
    return scFlags;
}

/**************************************************
 * 136: 60DB PMO +++
 * differences due to dummy args, use of uint8_t args
 * and equivalent optimiser differences.
 **************************************************/
sym_t *sub_60db(uint8_t p1) {
    sym_t *var2;
    /* sym_t **var4; */
    sym_t *var4;
    uint8_t tok;
    int16_t var7;
    s8_t varF;
    register sym_t *st;
    char tmp;
    s8_t tmp8;
    int tmp16;

    byte_8f85 = true;
    tok       = yylex();
    byte_8f85 = false;

    if (tok == T_ID) 
    {
        st = yylval.ySym;

        if (/* st->m20 */ GetByte((char*)st+OFF_m20) != p1)
            st = sub_4eed(st, p1, 0, 0);

        tok = yylex();
    } 
    else 
    {
        st = sub_4eed(sub_56a4(), p1, 0, 0);

        if (tok != T_LBRACE)
            expectErr("struct/union tag or '{'");
    }

    var4 = 0;

    if (tok == T_LBRACE) 
    {
        if ((/* st->m18 */ GetWord((char*)st+OFF_m18) & 0x81) == 1)
            prError("struct/union redefined: %s", st->nVName);
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
                        prError("members cannot be functions");

                    sub_516c(var2);

                    if (var4) 
		    {
			/*-------------------------
                        *var4 = var2;
                        var4  = &var2->nMemberList;
			--------------------------- */
			PutWord((char*)var4+OFF_nMemberList, var2);
			var4 = var2;
                    }

                    /* var2->m14 = var7++; */
		    PutWord((char*)var2+OFF_m14, var7);
		    var7++;
                }

                if ((tok = yylex()) == T_COLON) 
		{
                    if (!(/* var2->attr.dataType */ GetByte((char*)var2+OFF_s8_t_a_dataType) & DT_UNSIGNED))
		    {
			tmp = GetByte((char*)var2+OFF_s8_t_a_dataType);
			PutByte((char*)var2+OFF_s8_t_a_dataType, tmp | DT_UNSIGNED);
                        /* var2->attr.dataType |= DT_UNSIGNED; */
		    }

		    ReadMem(&tmp8, var2, sizeof(struct _s8));
 
                    if (!sub_5a76(/* &var2->attr */ &tmp8, DT_UINT))
                        prError("bad bitfield type");

                    if ((tok = yylex()) != T_ICONST)
                        prError("integer constant expected");
                    else 
		    {
                        if (!var2) 
			{
                            sub_516c(var2 = sub_4eed(sub_56a4(), D_MEMBER, &varF, st));

                            if (var4) 
			    {
				/* ------------------------
                                *var4 = var2;
                                var4  = &var2->nMemberList;
				----------------------------- */
				PutWord((char*)var4+OFF_nMemberList, var2);
				var4 = var2;
                            }

                            var2->m14 = var7++;
                        } /* 62ce */

                        /* var2->m18 |= 0x400; */
			tmp16 = GetWord((char*)var2+OFF_m18);
			PutWord((char*)var2+OFF_m18, tmp16 | 0x400);

                        /* var2->m16 = (int16_t)yylval.yNum; */
			PutWord((char*)var2+OFF_m16, (int16_t)yylval.yNum);

                        tok       = yylex();
                    }
                }

            } while (tok == T_COMMA); /* 62f2 */

            byte_8f86 = false;

            if (tok != T_SEMI)
                expectErr(";");

            if ((tok = yylex()) == T_RBRACE) 
	    {
                if (!(/* st->m18 */ GetWord((char*)st+OFF_m18) & 1))
                    sub_516c(st);

                if (var4) 
		{
                    /* *var4 = st; */
		    PutWord((char*)var4+OFF_nMemberList, st);

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

    if ((tok = yylex()) == T_ID) {
        st = yylval.ySym;
        if ((tok = yylex()) != T_LBRACE) {
            if (!(/* st->m18 */ GetWord((char*)st+OFF_m18) & 1))
                prError("undefined enum tab: %s", st->nVName);
            ungetTok = tok;
        }
    } else if (tok == T_LBRACE)
        st = sub_56a4();
    else {
        expectErr("enum tag or {");
        st = NULL;
    }
    if (tok == T_LBRACE) {
        sub_516c(st = sub_4eed(st, D_ENUM, 0, 0));
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
            if ((tok = yylex()) != T_ID) {
                expectErr("identifier");
                break;
            } else { /* 6474 */
                if ((vara = sub_4eed(yylval.ySym, DT_CONST, &var8, st)))
                    vara->m14 = vare++;
                if ((tok = yylex()) == T_EQ) {
                    sub_2569(varc);
                    sub_0a83(T_LBRACE);
                    if (!sub_5b08(&varc->attr) || varc->attr.dataType >= DT_LONG)
                        prError("integer expression required");
                    tok = yylex();
                }
                sub_516c(vara);
                sub_05d3(varc);
                if (tok != T_COMMA)
                    break;
                varc = sub_25f7(varc);
            }
        }
        printf(".. ]\n");
        sub_2569(varc);
        if (tok != T_RBRACE) {
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

    if (st && !sub_5aa4(&st->attr)) {
        prError("can't initialise auto aggregates");
        skipToSemi();
    } else {
        if (!(var4 = (tok = yylex()) == T_LBRACE))
            ungetTok = tok;
        if ((var2 = sub_1441(T_60, sub_07f5(T_RBRACE), 0)) && st) {
            var2 = sub_1441(T_EQ, allocId(st), var2);
            sub_042d(var2);
            sub_2569(var2);
        }
        if (var4 && yylex() != T_RBRACE)
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
    s8_t buf8, *varb;
    uint8_t tok;
    sym_t *vare FORCEINIT;
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
    int tmp;

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
        if ((tok = yylex()) == T_3DOT) 
	{
            args.s8array[args.cnt].dataType = DT_VARGS;
            args.s8array[args.cnt].i_expr   = 0;
            args.s8array[args.cnt].c7       = 0;
            args.s8array[args.cnt++].i4     = 0;
            tok                             = yylex();
            break;
        } /* 66db */

        if (tok == T_ID && /* yylval.ySym->m20 */ GetByte((char*)yylval.ySym+OFF_m20) != (char)T_TYPEDEF)
            if (p1)
                var12 = true;
            else
                prError("type specifier reqd. for proto arg");
        else
            var11 = true;

        if (var11 && var12) 
	{
            var12 = false;
            prError("can't mix proto and non-proto args");
        }

        ungetTok = tok;
        scFlags  = sub_5dd1(&scType, &attr);

        if (scType != D_15 && scType != T_REGISTER)
            prError("bad storage class");

        scType = p1 ? D_15 : D_14;
        st     = sub_69ca(scType, &attr, scFlags & ~1, 0);

        /* varb   = &st->attr; */
	ReadMem(&buf8, st, sizeof(struct _s8));
	varb = &buf8;

        if (varb->c7 == ANODE) {
            varb->i_nextSym = sub_4eed(sub_56a4(), T_TYPEDEF, varb, 0);
            varb->dataType  = DT_POINTER;
            varb->c7        = 0;
            varb->i4        = 1;
        }

        if (var11)
            sub_58bd(varb, &args.s8array[args.cnt++]);

        scFlags &= ~1;
        scType = D_6;
        st     = sub_4eed(st, scType, var11 ? varb : &var1a, 0);

	ReadMem(&buf8, varb, sizeof(struct _s8));

        if (p1 && !sub_5a76(/* varb */ &buf8, T_AUTO)) 
	{
            if (!p25_a28f) 
	    {
                p25_a28f = st;
                vare     = st;
                /* st->m18 |= scFlags | 0x20; */
		tmp = GetWord((char*)st+OFF_m18);
		PutWord((char*)st+OFF_m18, tmp | scFlags | 0x20);
            } else if (/* st->m18 */ GetWord((char*)st+OFF_m18) & 0x20) /* 6893 */
                prError("argument redeclared: %s", /*st->nVName */ GetWord((char*)st+OFF_nVName));
            else 
	    {
                /* vare->nMemberList = st; */
		PutWord((char*)vare+OFF_nMemberList, st);

                /* st->m18 |= scFlags | 0x20; */
		tmp = GetWord((char*)st+OFF_m18);
		PutWord((char*)st+OFF_m18, tmp | scFlags | 0x20);

                vare = st;
            }

            if (var11)
	    {
                /* st->m18 |= 0x208; */
		tmp = GetWord((char*)st+OFF_m18);
		PutWord((char*)st+OFF_m18, tmp | 0x208);
	    }

            /* st->nMemberList = 0; */
	    PutWord((char*)st+OFF_nMemberList, 0);
        }
        if ((tok = yylex()) == T_EQ) {
            prError("can't initialize arg");
            skipStmt(tok);
        }
        if (tok == T_ID || tok == S_CLASS || tok == S_TYPE) {
            expectErr(",");
            ungetTok = tok;
        } else if (tok != T_COMMA)
            break;
    } /* 669c */
    byte_a299 = var10;
    if (tok != T_RPAREN) {
        expectErr(")");
        skipStmt(tok);
    }
    if (args.cnt == 0)
        return 0;
    var1c = args.cnt;
    while (var1c--) {
        if (args.s8array[var1c].c7 == ENODE) {
            args.s8array[var1c].c7 = SNODE;
            sub_2569(args.s8array[var1c].i_expr);
            sub_5be1(&args.s8array[var1c]);
        }
    }
    return sub_578d((args_t *)&args);
}

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
    int tmp;
    s8_t buf8;

    var6             = p12_a297;
    p12_a297         = &var12;
    var1b.i_info     = NULL; /* other options */
    var1b.i_nextInfo = NULL; /* other options */
    var1b.c7         = 0;
    var1b.i4         = 0;
    var1b.dataType   = 0;
    p12_a297->p8     = &var1b;
    p12_a297->i6     = 0;
    p12_a297->p25    = NULL;
    p12_a297->p25_1  = NULL;
    p12_a297->uca    = 0;
    p12_a297->uc9    = 0;
    p12_a297->uc8    = 0;
    p12_a297->ucb    = 0;
    sub_6fab(p1);
    ungetTok = tok = yylex();
    if (p12_a297->ucb) {
        if (p1 == T_79 || p1 == D_14 || p1 == D_15 || (p1 == D_MEMBER && tok == T_COLON)) {
            p12_a297->p25 = sub_56a4();
            if (p1 == T_79)
                p1 = T_TYPEDEF;
        } else
            prError("no identifier in declaration");
    } /* 6aaf */
    p12_a297->p8->dataType = p2->dataType;
    p12_a297->p8->u1       = p2->u1;

    for (var2 = p2->i4; var2; var2 >>= 1) {
        if (p12_a297->i6 & 1) {
            p12_a297->uc8 = 1;
            break;
        }
        p12_a297->i6 = (p12_a297->i6 >> 1) | ((var2 & 1) << 15);
    }
    /* ^^^ */
    for (;;) {
        if (p12_a297->p8->c7 == SNODE && p12_a297->p8->dataType == DT_POINTER &&
            (p12_a297->i6 == 0 || p12_a297->p8->i_nextInfo->c7 == SNODE)) {
            for (var2 = p12_a297->p8->i_nextInfo->i4; var2; var2 >>= 1) {
                if (p12_a297->i6 & 1) {
                    p12_a297->uc8 = 1;
                    break;
                }
                p12_a297->i6 = (p12_a297->i6 >> 1) | ((var2 & 1) << 15);
            }
            sub_58bd(p12_a297->p8->i_nextInfo, p12_a297->p8);
        } else
            break;
    } /* 6c13 vvv */
    if (p12_a297->uc8)
        prError("declarator too complex");
    p12_a297->p8->i4 = sub_742a(p12_a297->i6);
    if (p12_a297->p25 && p1 != T_TYPEDEF && p1 != D_14 && p1 != D_15 &&
        sub_5a76(p12_a297->p8, DT_VOID) && p12_a297->p8->c7 != ANODE)
        prError("only functions may be void");
    else if (sub_5a76(p12_a297->p8, DT_POINTER)) {
        if (p12_a297->p8->c7 == ANODE && p12_a297->p8->i_nextInfo->c7 == ENODE)
            prError("functions can't return arrays");
        else if (p12_a297->p8->c7 == ENODE && p12_a297->p8->i_nextInfo->c7 == ANODE)
            prError("can't have array of functions");
    }
    if (p12_a297->p25_1) 
    {
	ReadMem(&buf8, p12_a297->p25_1, sizeof(struct _s8));
        p12_a297->p25_1 = sub_4eed(p12_a297->p25_1, T_TYPEDEF, /* &p12_a297->p25_1->attr */ &buf8, 0);
        sub_516c(p12_a297->p25_1);
        sub_51cf(p12_a297->p25_1);
        if (/* p12_a297->p25_1->attr.c7 */ GetByte((char*)p12_a297->p25_1+OFF_s8_t_c7) != (char)ANODE ||
	    !(/* p12_a297->p25_1->m18 */ GetWord((char*)p12_a297->p25_1+OFF_m18) & 0x80))
            sub_0493(p12_a297->p25_1);
    } /* 6d95 */
    if (p1 != T_TYPEDEF && p12_a297->p8->i4 == 0 &&
        (sub_5a76(p2, DT_STRUCT) || sub_5a76(p2, DT_UNION)) && p2->i_nextSym &&
        !(/* p2->i_nextSym->m18 */ GetWord((char*)p2->i_nextSym+OFF_m18) & 1))
        prError("undefined struct/union: %s", 
	/* p12_a297->p8->i_nextSym->nVName */ GetWord((char*)p12_a297->p8->i_nextSym+OFF_nVName));
    if (p12_a297->p25) { /* 6e0b */
        if (byte_a299 == D_6 && p1 != D_MEMBER) {
            if (/* p12_a297->p25->m18 */ GetWord((char*)p12_a297->p25+OFF_m18) & 8)
                prError("argument redeclared: %s", /* p12_a297->p25->nVName */ GetWord((char*)p12_a297->p25+OFF_nVName));
            else if (!(/* p12_a297->p25->m18 */ GetWord((char*)p12_a297->p25+OFF_m18) & 0x20))
                prError("not an argument: %s", /* p12_a297->p25->nVName */ GetWord((char*)p12_a297->p25+OFF_nVName));
            else {
                /* p12_a297->p25->m18 |= p3 | 8; */
		tmp = GetWord((char*)p12_a297->p25+OFF_m18);
		PutWord((char*)p12_a297->p25+OFF_m18, tmp | p3 | 8);

                /* p12_a297->p25->attr = var1b; */
		WriteMem(&var1b, p12_a297->p25, sizeof(struct _s8));

                if (sub_5a76(&var1b, DT_FLOAT)) {
                    prWarning("float param coerced to double");
                    /* p12_a297->p25->attr.dataType = DT_DOUBLE; */
		    PutByte((char*)p12_a297->p25+OFF_s8_t_a_dataType, (char)DT_DOUBLE);
                }
            }
        } else if (byte_a299 != D_14 && byte_a299 != D_15) { /* 6ecd */
            if (p1 == T_AUTO && var1b.c7 == ANODE)
                p1 = T_EXTERN;
            if ((p12_a297->p25 = sub_4eed(p12_a297->p25, p1, &var1b, p4)))
	    {
                /* p12_a297->p25->m18 |= p3; */
		tmp = GetWord((char*)p12_a297->p25+OFF_m18);
		PutWord((char*)p12_a297->p25+OFF_m18, tmp | p3);
	    }
        } 
	else 
	{
            if (/* p12_a297->p25->m20 */ GetByte((char*)p12_a297->p25+OFF_m20) &&
	        /* p12_a297->p25->m21 */ GetByte((char*)p12_a297->p25+OFF_m21) != depth) /* 6f39 */
                p12_a297->p25 = sub_4eed(p12_a297->p25, 0, &var1b, 0);
            /* p12_a297->p25->attr = var1b; */
	    WriteMem(&var1b, p12_a297->p25, sizeof(struct _s8)); 
        }
    }
    /* 6f91 */
    var4     = p12_a297->p25;
    p12_a297 = var6;
    return var4;
}
/**************************************************
 * 141: 6FAB PMO +++
 * minor equivalent optimiser differences
 * differnces due to dummy & uint8_t args
 **************************************************/
void sub_6fab(uint8_t p1) {
    bool var1;
    uint8_t var2;
    uint8_t tok;
    uint8_t var4;
    uint8_t var5;
    register expr_t *st;
    s8_t buf8;

    byte_a29a = 0;
    var4      = 1;
    var2      = 0;
    while ((tok = yylex()) == T_STAR)
        var2++;
    if (tok == T_ID) {
        p12_a297->p25 = yylval.ySym;
        tok           = yylex();
        var4          = /* p12_a297->p25->m20 */ GetByte((char*)p12_a297->p25+OFF_m20);
        if (/* p12_a297->p25->m20 */ GetByte((char*)p12_a297->p25+OFF_m20) == (char)0)
            /* p12_a297->p25->m20 = p1; */
	    PutByte((char*)p12_a297->p25+OFF_m20, p1);
        byte_a29a = p1 != D_14;
    } else if (tok == T_LPAREN) { /* 701b */
        ungetTok = tok = yylex();
        if (tok == T_RPAREN || tok == S_CLASS || tok == S_TYPE ||
            (tok == T_ID && /* yylval.ySym->m20 */ GetByte((char*)yylval.ySym+OFF_m20) == (char)T_TYPEDEF))
            tok = T_LPAREN;
        else {
            sub_6fab(p1);
            if ((tok = yylex()) != T_RPAREN)
                expectErr(")");
            tok = yylex();
        }
    } /* 707b */
    p12_a297->ucb = p12_a297->p25 == NULL;
    for (;;) { /* 7091 */
        if (tok == T_LPAREN) {
            if (p12_a297->uc9) {
                prError("can't have array of functions");
                p12_a297->uc9 = false;
            }
            if (p12_a297->p8->c7 == ANODE && p12_a297->i6 == 0)
                prError("functions can't return functions");
            if (p12_a297->i6 & 0x8000) { /* 70e7 */
                p12_a297->p8->dataType  = DT_POINTER;
                p12_a297->p8->i4        = sub_742a(p12_a297->i6);
                p12_a297->i6            = 0;
                p12_a297->p8->i_nextSym = sub_56a4();
		ReadMem(&buf8, p12_a297->p25_1, sizeof(struct _s8));
                if (p12_a297->p25_1)
                    p12_a297->p25_1 =
                        sub_4eed(p12_a297->p25_1, T_TYPEDEF, /* &p12_a297->p25_1->attr */ &buf8, 0);
                p12_a297->p25_1 = p12_a297->p8->i_nextSym;
                /* p12_a297->p8    = &p12_a297->p25_1->attr */;
		WriteMem(&buf8, &p12_a297->p8, sizeof(struct _s8));
            } /* 7195 */
            p12_a297->p8->c7 = ANODE;
            byte_a29a        = byte_a29a && !depth;
            if (!byte_a29a) {
                var5 = true;
                depth++;
            } else
                var5 = false;
            /* 71cc */
            depth++;
            if ((tok = yylex()) == S_CLASS || tok == S_TYPE || tok == T_ID) {
                ungetTok             = tok;
                p12_a297->p8->i_args = sub_65e2(byte_a29a);
            } else if (tok != T_RPAREN) {
                expectErr(")");
                skipStmt(tok);
            }
            byte_a29a = false;
            if (var5) {
                checkScopeExit();
                depth--;
            }
            depth--;
        } else if (tok == T_LBRACK) { /* 7248 */
            if (p12_a297->p8->c7 == ANODE)
                prError("functions can't return arrays");
            if (p12_a297->uca || (p12_a297->i6 & 0x8000)) {
                p12_a297->uca           = false;
                p12_a297->p8->dataType  = DT_POINTER;
                p12_a297->p8->i4        = sub_742a(p12_a297->i6);
                p12_a297->i6            = 0;
                p12_a297->p8->i_nextSym = sub_56a4();
		ReadMem(&buf8, p12_a297->p25_1, sizeof(struct _s8));
                if (p12_a297->p25_1)
                    p12_a297->p25_1 =
                        sub_4eed(p12_a297->p25_1, T_TYPEDEF, /* &p12_a297->p25_1->attr */ &buf8, 0);
                p12_a297->p25_1 = p12_a297->p8->i_nextSym;
                /* p12_a297->p8    = &p12_a297->p25_1->attr; */
		WriteMem(&buf8, &p12_a297->p8, sizeof(struct _s8));
            } /* 732a */
            var1      = byte_8f86;
            byte_8f86 = false;
            if ((tok = yylex()) == T_RBRACK) {
                if (p12_a297->uc9)
                    prError("dimension required");
                st = &s13_9d1b;
            } else {
                ungetTok = tok;
                st       = sub_0a83(T_SEMI);
                if ((tok = yylex()) != T_RBRACK) {
                    expectErr("]");
                    skipStmt(tok);
                }
            } /* 738a */
            p12_a297->uca        = true;
            p12_a297->uc9        = true;
            byte_8f86            = var1;
            p12_a297->p8->c7     = 1;
            p12_a297->p8->i_expr = st;
        } else { /* 73c1 */
            ungetTok = tok;
            if (!var4)
                /* p12_a297->p25->m20 = 0; */
		PutByte((char*)p12_a297->p25, 0);
            if (!var2)
                return;
            p12_a297->uc9 = false;
            do {
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
void sub_7454(register s8_t *st) {
    uint16_t var2;
    uint8_t var3;
    bool v;
    sym_t * tmp;

    putchar('`');
    for (;;) 
    {
        v = IsValid((char*)st);

        if (/* st->c7 */ (v ? GetByte((char*)st+OFF_s8_t_c7) : st->c7) == (char)ANODE)
            putchar('(');

        for (var2 = /* st->i4 */ GetWord((char*)st+OFF_s8_t_a_i4); var2; var2 >>= 1)
            if (var2 & 1)
                putchar('*');

        if (/* st->dataType */(v ? GetByte((char*)st+OFF_s8_t_a_dataType) : st->dataType) == (char)DT_POINTER &&
	    /* st->i_nextInfo->c7 */ (v ? ((s8_t *)GetWord((char*)st+OFF_s8_t_nextInfo))->c7 : st->i_nextInfo->c7) == (char)ANODE)
            /* st = st->i_nextInfo; */
	    st = v ? GetWord((char*)st+OFF_s8_t_nextInfo) : st->i_nextInfo;
        else
            break;
    }

    var3 = v ? GetByte((char*)st+OFF_s8_t_a_dataType) : st->dataType;

    switch (var3) 
    {
    case DT_ENUM:
    case DT_POINTER:
        sub_573b(v ? GetWord((char*)st+OFF_s8_t_nextSym) : st->i_nextSym, stdout);
        break;
    case DT_STRUCT:
    case DT_UNION:
	tmp = v ? GetWord((char*)st+OFF_s8_t_nextSym) : st->i_nextSym;
        printf("S%d", /* st->i_nextSym->a_labelId */ GetWord((char*)tmp+OFF_s8_t_labelId));
        break;
    default:
        if (var3 & 1) {
            putchar('u');
            var3 &= ~1;
        }
        putchar("?bcsilxfd?v"[var3 >> 1]);
        break;
    }
}
