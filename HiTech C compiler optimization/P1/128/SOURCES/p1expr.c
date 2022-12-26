/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

s2_t *p2List      = &s2_9cf3[20]; /* 8bc7 */
int16_t strId     = 0;            /* 8bd7 */
uint8_t byte_8f85 = 0;            /* 8f85 */
bool byte_8f86 = false;        /* 8f86 */
uint8_t byte_968b;                /* 968b */
int16_t word_968c;                /* 968c */
int16_t tmpLabelId;               /* 968e */

expr_t **s13SP;   /* 9cf1 */
s2_t s2_9cf3[20]; /* 9cf3 */
char pad9d00[27];
expr_t s13_9d1b; /* 9d1b */
expr_t s13_9d28; /* 9d28 */

expr_t *s13FreeList; /* 9d35 */
uint8_t byte_9d37;   /* 9d37 */
expr_t *s13Stk[20];  /* 9d38 */

/* expr.c */
expr_t *sub_0817(register s8_t *st);
bool sub_0aed(register expr_t *st);
bool sub_0b93(register expr_t *st);
bool sub_10a8(void);
expr_t *sub_1340(register expr_t *st, expr_t *p2);
expr_t *allocFConst(char *fltStr);
expr_t *sub_1b94(register expr_t *st);
expr_t *sub_1bf7(register expr_t *st, s8_t *p2);
expr_t *sub_1ccc(expr_t *p1, uint8_t p2);
expr_t *sub_1d02(register expr_t *st);
uint8_t sub_1d5a(register s8_t *st, s8_t *p2);
expr_t *sub_1df0(register expr_t *st);
expr_t *sub_1e37(register expr_t *st);
expr_t *sub_1e58(register expr_t *st);
expr_t *sub_1ebd(register expr_t *st);
bool sub_1ef1(register expr_t *st);
expr_t *sub_1f5d(register expr_t *st, s8_t *p2, int16_t p3);
expr_t *s13Alloc(uint8_t tok);
expr_t *sub_225a(uint8_t p1, register expr_t *st, expr_t *p3);
expr_t *sub_23b4(uint8_t tok, register expr_t *st, expr_t *p3);
expr_t *allocSConst(void);
void complexErr(void);
expr_t *popExpr(void);
void sub_2529(uint8_t p1);
uint8_t sub_255d(void);

/**************************************************
 * 17: 07F5 PMO +++
 **************************************************/
expr_t *sub_07f5(char p1) {
    char l1;
    register expr_t *st;

    l1        = byte_9d37;
    byte_9d37 = p1;
    st        = sub_0bfc();
    byte_9d37 = l1;
    return st;
}

/**************************************************
 * 18: 0817 PMO +++
 * Minor differences due to addition of missing
 * parameters and use of uint8_t parameter
 * also one missed optimisation
 * of pop bc, push hl to ex (sp),hl
 **************************************************/
expr_t *sub_0817(register s8_t *st) {
    int16_t var2;
    int16_t var4;
    expr_t *var6;
    uint8_t tok;
    expr_t *arr[128];

#ifdef BUGGY
    if (st && st->c7 != ANODE && st->i4 == 1 && st->dataType == DT_POINTER &&
        st->i_nextInfo->c7 == ANODE)
        st = st->i_nextInfo;
    else if (st->c7 != ANODE)
        prError("function or function pointer required");
#else
    /* fix as if st is NULL the else clause shouldn't be used */
    if (st) {
        if (st->c7 != ANODE && st->i4 == 1 && st->dataType == DT_POINTER &&
            st->i_nextInfo->c7 == ANODE)
            st = st->i_nextInfo;
        else if (st->c7 != ANODE)
            prError("function or function pointer required");
    }
#endif

    if (st && st->c7 == ANODE && st->i_args) {
        var4 = st->i_args->cnt;
        st   = st->i_args->s8array;
    } else {
        st   = NULL;
        var4 = 0;
    }

    var2     = 0;
    ungetTok = tok = yylex();
    for (;;) {
        if (st && st->dataType == DT_VARGS) {
            var4 = 0;
            st   = 0;
        }
        if (tok != T_RPAREN) {
            if (st && sub_5a76(st, DT_VOID)) {
                prError("function does not take arguments");
                var4 = 0;
                st   = NULL;
            }
            var6 = sub_1441(T_60, sub_07f5(3), 0);
            if (var6) {
                if (st && var4-- == 0) {
                    prError("too many arguments");
                    st   = 0;
                    var4 = 0;
                }
                if (st)
                    var6 = sub_1f5d(var6, st++, 1);
                else
                    var6 = sub_1d02(var6);
                arr[var2++] = var6;
                tok         = yylex();
                if (tok == T_COMMA)
                    continue;
                ungetTok = tok;
            }
        }
        break;
    }
    if ((var4 != 1 || var2 != 0 || !sub_5a76(st, DT_VOID)) && var4 && st->dataType != DT_VARGS)
        prError("too few arguments");

    if (var2 == 0)
        return sub_23b4(T_120, NULL, NULL); /* dummy 2nd & 3rd args added */
    var4 = 0;
    while (var4 + 1 != var2) {
        arr[var4 + 1] = sub_23b4(T_COMMA, arr[var4], arr[var4 + 1]);
        ++var4;
    }
    return arr[var4];
}

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
            prError("constant expression required");
        else if (byte_9d37 == 2) {
            if (isValidDimType(&st->attr))
                st = sub_1ccc(st, DT_CONST);
            else
                prError("illegal type for array dimension");
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
    if (type == T_ID)
        return sub_5a76(&st->attr, DT_ENUM);
    if (type == T_SIZEOF)
        return true;
    if (type == D_ADDRESSOF && sub_0b93(st->t_next))
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

    type = st->tType;
    if (type == T_ID)
        return /* st->t_pSym->m20 */ GetByte((char*)(st->t_pSym)+OFF_m20) == (char)T_EXTERN || 
	       /* st->t_pSym->m20 */ GetByte((char*)(st->t_pSym)+OFF_m20) == (char)T_STATIC;
    if (type == T_DOT)
        return sub_0b93(st->t_next);
    return type == T_69 && sub_0aed(st->t_next);
}

/**************************************************
 * 22: 0BFC PMO +++
 * many basic blocks moved around, logic still valid
 * some optimisation differences but equivalent code
 **************************************************/
expr_t *sub_0bfc(void) {
    s8_t var8;
    expr_t *varA FORCEINIT;
    s2_t *varC;
    expr_t **varE;
    uint8_t tok;
    uint8_t tok2;
    uint8_t var11;
    uint8_t var12;
    bool var13;
    uint8_t var14;
    uint8_t var15;
    int16_t var17;
    int16_t var19; /* not used */
    register sym_t *st;

    varE = s13SP;
    varC = p2List;
    sub_2529(T_60);
    var11 = 0;
    var19 = 0;
    for (;;) {
        var13 = (byte_8f86 = p2List->type1 == T_DOT || p2List->type1 == T_POINTER);
        tok               = yylex();
        byte_8f86         = false;
        if (tok < T_60 || tok >= T_128) {
            ungetTok = tok;
            tok      = T_60;
        }
        if (opTable[tok - T_60].uc4 & 1) {
            if (var11)
                goto error;
            switch (tok) {
            case T_ID: /* c9a */
                st = yylval.ySym;
                if (!var13) {
                    if (/* st->m20 */ GetByte((char*)st+OFF_m20) == (char)0) {
                        if (peekCh() == '(') {
                            var8.c7       = 2;
                            var8.dataType = DT_INT;
                            var8.i_sym    = 0;
                            var8.i4       = 0;
                            st            = sub_4eed(st, T_EXTERN, &var8, 0);
                            /* st->m18 |= 0x42; */
			    PutByte((char*)st+OFF_m18, GetByte((char*)st+OFF_m18) | 0x42);
                            /* st->m21 = 0 */ PutByte((char*)st+OFF_m21, 0);
                            sub_0493(st);

                        } else {
                            prError("undefined identifier: %s", st->nVName);
                            /* st->m20           = T_STATIC; */
			    PutByte((char*)st+OFF_m20, T_STATIC);
                            /* st->attr.dataType = DT_INT; */
			    PutByte((char*)st+OFF_s8_t_a_dataType, DT_INT);
                            /* st->m18           = 0x11; */
			    PutWord((char*)st+OFF_m18, 0x11);
                        }
                    } else {
                        var17 = /* st->m18 */ GetWord((char*)st+OFF_m18);
                        if (!(var17 & 0x10))
                            prError("not a variable identifier: %s", 
					/* st->nVName */ GetWord((char*)st+OFF_nVName));
                        else if (/* st->m20 */ GetByte((char*)st+OFF_m20) == (char)T_EXTERN && !(var17 & 0x100))
                            sub_0493(st);
                    }
                    sub_51cf(st);
                } /* d57 */
                varA = allocId(st);
                break;
            case T_ICONST: /* d75 */
                varA = sub_1b4b(yylval.yNum, DT_INT);
                break;
            case T_LCONST: /* d90 */
                varA = sub_1b4b(yylval.yNum, DT_LONG);
                break;
            case T_FCONST: /* da5 */
                varA = allocFConst(yylval.yStr);
                break;
            case T_SCONST: /* dae */
                varA       = allocSConst();
                varA->t_i2 = strChCnt;
                sub_053f(varA, yylval.yStr);
                free(yylval.yStr);
                break;
            case S_TYPE:
                goto error;
            }
            /* d63 */
            pushS13(varA);
            var11 = true;
            continue;
        } /* dfa */
        switch (tok) {
        case T_LPAREN:
            ungetTok = tok2 = yylex();
            if (tok2 == S_TYPE || 
	        (tok2 == T_ID &&
	         /* yylval.ySym->m20 */ GetByte((char*)(yylval.ySym)+OFF_m20) == (char)T_TYPEDEF)) {
                if (var11)
                    goto error;
                sub_5dd1(0, &var8);
                sub_516c(st = sub_69ca(T_79, &var8, 0, 0));
                sub_51cf(st);
                tok2 = yylex();
                if (tok2 != T_RPAREN)
                    goto error;
                /* var8 = st->attr; */ ReadMem((char*)&var8, (char*)st, sizeof(struct _s8));
                pushS13(allocSType(&var8));
                if (p2List->type1 == T_SIZEOF) {
                    p2List->type2 = T_EQ;
                    var11         = true;
                    continue;
                } else
                    tok = T_79;
            } else if (var11) {
                sub_2529(T_61);
                pushS13(sub_0817(&(*s13SP)->attr));
            }
            break;
        case T_60:
            break;
        case T_LBRACK:
            if (!var11)
                goto error;
            tok   = T_64;
            var11 = false;
            break;
        case T_RPAREN:
        case T_RBRACK:
            if (!var11)
                goto error;
            break;
        case T_INC:
        case T_DEC:
            if (var11)
                tok++;
            break;
        default:
            if (opTable[tok - T_60].uc4 & 4 && !var11)
                tok -= 11;
            var14 = (opTable[tok - T_60].uc4 & 2) != 0;
            if (var14 != var11)
                goto error;
            var11 = false;
            break;
        }
        /* f23 */
        var15 = opTable[tok - T_60].c3;
        if ((byte_9d37 >= 3 && tok == T_COMMA) ||
            (byte_9d37 == 1 && tok == T_COLON && p2List->type1 != T_QUEST))
            var15 = 5;
        /* f8e */
        do {
            var12 = 0;
            if (p2List->type2 < var15 ||
                (p2List->type2 == var15 && (opTable[tok - T_60].uc4 & 8))) {
                switch (tok) {
                case T_75:
                case T_77:
                    var15 = 0x1f;
                    break;
                case T_LPAREN:
                case T_64:
                    var15 = 4;
                    break;
                }
                sub_2529(tok);
                p2List->type2 = var15;
            } else {
                if (p2List->type1 == T_60) { /* 1058 */
                    if (tok != T_60)
                        ungetTok = tok;
                    varA = popExpr();
                    if (varA && s13SP == varE)
                        goto done;
                    else
                        goto error;
                } else if (p2List->type1 == T_LPAREN) {
                    if (tok != T_RPAREN) {
                        expectErr(")");
                        ungetTok = tok;
                    }
                } else if (p2List->type1 == T_64) {
                    if (tok != T_RBRACK) {
                        expectErr("]");
                        ungetTok = tok;
                    }
                } else
                    var12 = 1;
                /* 1037 */
                if (sub_10a8())
                    goto error;
            }
        } while (var12);
    }
error:
    prError("expression syntax");
    skipStmt(tok);
    while (s13SP != varE)
        sub_2569(popExpr());
    varA = NULL;

done:
    s13SP  = varE;
    p2List = varC;
    return varA;
}

/**************************************************
 * 23: 10A8 PMO +++
 * Minor differences, use of uint8_ parameter and
 * addition of missing parameters
 **************************************************/
bool sub_10a8(void) {
    expr_t *l1;
    expr_t *l2;
    uint8_t tok;
    register expr_t *st FORCEINIT;

    if ((tok = sub_255d()) == T_LPAREN)
        return false;

    l1 = NULL;
    if (tok != T_120 &&
        (((opTable[tok - T_60].uc4 & 2) && (l1 = popExpr()) == NULL) || (st = popExpr()) == NULL))
        return true;

    switch (tok) {
    case T_64:
        sub_2529(T_69);
        p2List->type2 = T_EQ;
        tok           = T_PLUS;
        if (isValidDimType(&l1->attr))
            l1 = sub_1ccc(l1, DT_CONST);
        else
            prError("illegal type for index expression");
        break;
    case T_INC:
    case T_75:
    case T_DEC:
    case T_77:
        l1  = &s13_9d28;
        tok = tok == T_INC ? T_PLUSEQ : tok == T_DEC ? T_MINUSEQ : tok == T_75 ? T_102 : T_104;
        break;
    case T_POINTER:
        tok = T_DOT;
        st  = sub_1441(T_69, st, 0); /* added dummy 3rd arg */
        break;
    case T_SIZEOF:
        if (st->tType == T_SCONST) {
            l2 = sub_1b4b((long)st->t_i2 + 1, DT_UINT);
            sub_2569(st);
            pushS13(l2);
            return false;
        }
        if (st->tType != S_TYPE && st->tType != T_ID) {
            l2             = allocSType(&st->attr);
            l2->attr.i_sym = NULL;
            l2->attr.c7    = 0;
            if (st->attr.c7 == ENODE) {
                l2  = sub_1441(T_SIZEOF, l2, 0);
                l1  = sub_1ccc(sub_21c7(st->attr.i_expr), DT_UINT);
                tok = T_STAR;
            }
            sub_2569(st);
            st = l2;
        } else if (st->tType == S_TYPE && st->attr.c7 == ENODE) {
            l1  = sub_1ccc(sub_21c7(st->attr.i_expr), DT_UINT);
            st  = sub_1441(T_SIZEOF, st, 0);
            tok = T_STAR;
        }
        break;
    case T_COMMA:
        if (p2List[0].type1 != T_LPAREN || p2List[1].type1 != T_61)
            tok = T_114;
        break;
    case T_79:
        pushS13(sub_1bf7(sub_1441(T_60, l1, 0), &st->attr));
        sub_2569(st);
        return 0;
    }
    if ((tok == T_QUEST) != (l1 && l1->tType == T_COLON))
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

    if (!sub_5a76(&st->attr, DT_STRUCT) && !sub_5a76(&st->attr, DT_UNION))
        prError("struct/union required");
    else if (p2->tType != T_ID)
        prError("struct/union member expected");
    else if ((var4 = st->a_nextSym) == 0)
        ;
    else if (!(/* var4->m18 */ GetWord((char*)var4+OFF_m18) & 1))
        prError("undefined struct/union: %s", /* var4->nVName */ GetWord((char*)var4+OFF_nVName));
    else if ((var2 = findMember(var4, /* p2->t_pSym->nVName */ GetWord((char*)(p2->t_pSym)+OFF_nVName)))) {
        sub_51cf(var2);
        st       = s13Alloc(T_126);
        st->t_i0 = /* var2->m14 */ GetWord((char*)var2+OFF_m14);

        /* st->attr = var2->attr; */
	ReadMem(&st->attr, var2, sizeof(struct _s8));

        sub_2569(p2);
        return st;
    }
    return p2;
}

/**************************************************
 * 25: 1441 PMO +++
 * Minor optimiser differences, but equivalent code
 * Also uint8_t parameter differences
 **************************************************/
expr_t *sub_1441(uint8_t p1, register expr_t *lhs, expr_t *rhs) {
    s8_t tmpExpr;
    expr_t *savedLhs;
    expr_t *minusLhs FORCEINIT;
    bool hasRhs;
    bool minusLhsValid;
    int16_t opFlags;
    char *opStr;
    uint8_t var13;

    hasRhs = (opTable[p1 - 60].uc4 & 2) != 0;
    opStr  = opTable[p1 - 60].s0;
    if (!lhs || (hasRhs && rhs == 0)) {
        sub_2569(lhs);
        if (hasRhs)
            sub_2569(rhs);
        return NULL;
    }
    minusLhsValid = false;
    opFlags       = opTable[p1 - 60].i5;
    if (p1 == D_ADDRESSOF && lhs->tType == T_ID && 
	(/* lhs->t_pSym->m18 */ GetWord((char*)(lhs->t_pSym)+OFF_m18) & 4))
        prError("can't take address of register variable");

    if (!(opFlags & 0x100))
        lhs = sub_1e37(lhs);
    if (hasRhs && !(opFlags & 0x800))
        rhs = sub_1e37(rhs);

    if (p1 == T_61) {
        if ((lhs->attr.i4 & 1) && lhs->attr.c7 == SNODE)
            lhs = sub_1441(T_69, lhs, 0); /* dummy 3rd arg */
    } else
        lhs = sub_1df0(lhs);

    if (hasRhs)
        rhs = sub_1df0(rhs);
    if ((opFlags & 0x2000) && sub_1ef1(lhs) == 0) {
        if (p1 == D_ADDRESSOF) {
            if (lhs->tType == D_ADDRESSOF && lhs->t_next->attr.c7 == ENODE)
                return lhs;
            else
                prError("can't take this address");
        } else
            prError("only lvalues may be assigned to or modified");
    }
    if ((opFlags & 0x4000) && (!(lhs->attr.i4 & 1) || lhs->attr.c7 != SNODE))
        prError("pointer required");
    if (!(opFlags & 3)) {
        if (sub_5a76(&lhs->attr, DT_2))
            lhs = sub_1ccc(lhs, DT_INT);
        if (hasRhs && sub_5a76(&rhs->attr, DT_2))
            rhs = sub_1ccc(rhs, DT_INT);
    }
    switch (p1) {
    case T_60:
        return lhs;
    case T_DOT:
        rhs = sub_1340(lhs, rhs);
        break;
    case T_121:
        /* tmpExpr    = curFuncNode->attr; */
	ReadMem(&tmpExpr, curFuncNode, sizeof(struct _s8));
        tmpExpr.c7 = SNODE;
        if (sub_5a76(&tmpExpr, DT_VOID))
            prError("void function cannot return value");
        else
            lhs = sub_1f5d(lhs, &tmpExpr, 1);
        break;
    }
    if ((opFlags & 3)) {
        if ((opFlags & 2))
            lhs = sub_1b94(lhs);
        if ((opFlags & 1))
            rhs = sub_1b94(rhs);
    } else if ((opFlags & 4) && (lhs->attr.i4 & 1) && lhs->attr.c7 == SNODE &&
               sub_5b08(&rhs->attr)) /* 16e1 */
        rhs = sub_1ccc(sub_1441(T_STAR, rhs, sub_1ebd(lhs)),
                       (rhs->attr.dataType & DT_UNSIGNED) ? DT_UCONST : DT_CONST);
    else if (p1 == T_PLUS && (rhs->attr.i4 & 1) && rhs->attr.c7 == SNODE &&
             sub_5b08(&lhs->attr)) { /* 1740 */
        savedLhs = lhs;
        lhs      = rhs;
        rhs      = sub_1ccc(sub_1441(T_STAR, savedLhs, sub_1ebd(lhs)),
                       (rhs->attr.dataType & DT_UNSIGNED) ? DT_UCONST : DT_CONST);
    } else if ((opFlags & 8) && (lhs->attr.i4 & 1) && lhs->attr.c7 == SNODE &&
               (!hasRhs || ((rhs->attr.i4 & 1) && rhs->attr.c7 == SNODE))) { /* 17ab */
        if (!(opFlags & 0x8000) || (!sub_5a4a(&lhs->attr) && !sub_5a4a(&rhs->attr))) {
            if (hasRhs && !sub_591d(&lhs->attr, &rhs->attr))
                prWarning("operands of %.3s not same pointer type", opStr);
            else if (p1 == T_MINUS) {
                minusLhs      = lhs;
                minusLhsValid = true;
                lhs           = sub_1ccc(lhs, DT_CONST);
                rhs           = sub_1ccc(rhs, DT_CONST);
            }
        }
    } else if ((opFlags & 0x30) && sub_5ad5(&lhs->attr) &&
               (!hasRhs || sub_5ad5(&rhs->attr))) { /* 187a */
        if (opFlags & 0x40) {
            var13 = sub_1d5a(&lhs->attr, &rhs->attr);
            lhs   = sub_1ccc(lhs, var13);
            rhs   = sub_1ccc(rhs, var13);
        } /* 18fa */
        if (!(opFlags & 0x10) && (!sub_5b08(&lhs->attr) || (hasRhs && !sub_5b08(&rhs->attr))))
            prError("integral type required");
    } else if (opFlags & 0x80) { /* 1937 */
        if (sub_5a76(&lhs->attr, DT_VOID) || (hasRhs && sub_5a76(&rhs->attr, DT_VOID))) {
            if (p1 == T_124 && lhs->tType == T_61 && sub_5a76(&rhs->attr, DT_VOID)) {
                lhs->attr = rhs->attr;
                sub_2569(rhs);
                return lhs;
            }
            prError("illegal use of void expression");
        } else if (!(opFlags & 0x200) &&
                   (!sub_5aa4(&lhs->attr) || (hasRhs && !sub_5aa4(&rhs->attr)))) /* 19cc */
            prError("simple type required for %.3s", opStr);
        else if (opFlags & 0x1000) { /* 1a11 */
            if ((opFlags & 0x8000)) {
                if (sub_2105(lhs) && (rhs->attr.i4 & 1) && rhs->attr.c7 == SNODE)
                    lhs = sub_1bf7(lhs, &rhs->attr);
                else if (sub_2105(rhs) && (lhs->attr.i4 & 1) && lhs->attr.c7 == SNODE)
                    rhs = sub_1bf7(rhs, &lhs->attr);
            } /* 1a95 */
            if (!sub_591d(&lhs->attr, &rhs->attr))
                prWarning("operands of %.3s not same type", opStr);
        }
    } else if (!(opFlags & 0x200)) /* 1ac3 */
        prError("type conflict");
    /* 1ad1 */
    if (opFlags & 0x400)
        rhs = sub_1f5d(rhs, &lhs->attr, (opFlags & 4) == 0);

    savedLhs = sub_225a(p1, lhs, rhs);
    if (minusLhsValid)
        savedLhs = sub_1441(T_DIV, savedLhs, sub_1ebd(minusLhs));
    return savedLhs;
}

/**************************************************
 * 26: 1B4B PMO +++
 **************************************************/
expr_t *sub_1b4b(long num, uint8_t p2) {
    register expr_t *st;

    st                = allocIConst(num);
    st->attr.dataType = p2;
    return st;
}

/**************************************************
 * 27: 1B70 PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *allocFConst(char *fltStr) {
    register expr_t *st;

    st                = s13Alloc(T_FCONST);
    st->t_s           = fltStr;
    st->attr.dataType = DT_DOUBLE;
    return st;
}

/**************************************************
 * 28: 1B94 PMO +++
 * minor optimiser difference, equivalent code
 **************************************************/
expr_t *sub_1b94(register expr_t *st) {

    if (!sub_5aa4(&st->attr))
        prError("logical type required");
    else if (!sub_5a76(&st->attr, 2))
        st = sub_1441(T_NE, st, sub_1bf7(&s13_9d1b, &st->attr));

    return st;
}

/**************************************************
 * 29: 1BF7 PMO +++
 * minor optimiser difference, equivalent code
 **************************************************/
expr_t *sub_1bf7(register expr_t *st, s8_t *p2) {
    expr_t *var2;
    int tmp;

    if (st->tType == T_61 && (var2 = st->t_next)->tType == T_ID && 
	(/* var2->t_pSym->m18 */ GetWord((char*)(var2->t_pSym)+OFF_m18) & 0x40)) {
        prWarning("%s() declared implicit int", /* var2->t_pSym->nVName */ GetWord((char*)(var2->t_pSym)+OFF_nVName));
        /* var2->t_pSym->m18 &= ~0x40; */
        tmp = GetWord((char*)(var2->t_pSym)+OFF_m18);
	PutWord((char*)(var2->t_pSym)+OFF_m18, tmp & ~0x40);
    }

    if (!sub_591d(&st->attr, p2)) {
        if (st->tType != T_ICONST || inData(st))
            st = sub_23b4(T_124, st, allocSType(p2));
        st->attr = *p2;
    }
    return st;
}

/**************************************************
 * 30: 1CCC PMO +++
 **************************************************/
expr_t *sub_1ccc(expr_t *p1, uint8_t p2) {
    s8_t st;

    st.dataType = p2;
    st.i4       = 0;
    st.i_sym    = 0;
    st.c7       = 0;
    return sub_1bf7(p1, &st);
}

/**************************************************
 * 31: 1D02 PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *sub_1d02(register expr_t *st) {

    if (st->tType == T_COMMA || st->tType == 120)
        return st;
    if (!st->attr.c7 && !st->attr.i4) {
        if (st->attr.dataType < DT_INT || st->attr.dataType == DT_ENUM)
            return sub_1ccc(st, DT_INT);
        if (st->attr.dataType == DT_FLOAT)
            return sub_1ccc(st, DT_DOUBLE);
    }
    return st;
}

/**************************************************
 * 32: 1D5A PMO +++
 **************************************************/
uint8_t sub_1d5a(register s8_t *st, s8_t *p2) {
    bool mkUnsigned;
    uint8_t l2;

    mkUnsigned = (st->dataType & DT_UNSIGNED) || (p2->dataType & DT_UNSIGNED);
    l2         = st->dataType;
    if (l2 < p2->dataType)
        l2 = p2->dataType;
    if (l2 < DT_INT)
        l2 = DT_INT;
    if (l2 == DT_FLOAT || l2 == DT_DOUBLE)
        return DT_DOUBLE;
    if (l2 == DT_ENUM)
        l2 = DT_INT;
    if (mkUnsigned)
        return l2 | 1;
    return l2;
}

/**************************************************
 * 33: 1DF0 PMO +++
 **************************************************/
expr_t *sub_1df0(register expr_t *st) {

    if (st->tType != T_ID || st->attr.c7 != ANODE)
        return st;
    st->attr.dataType = 0x16;
    st->attr.c7       = SNODE;
    st->a_nextSym     = st->t_pSym;
    st->attr.i4       = 0;
    return sub_1e58(st);
}

/**************************************************
 * 34: 1E37 PMO +++
 **************************************************/
expr_t *sub_1e37(register expr_t *st) {

    if (st->attr.c7 == ENODE)
        st = sub_1e58(st);
    return st;
}

/**************************************************
 * 35: 1E58 PMO +++
 * differences due to added 3rd arg and uint8_t param
 **************************************************/
expr_t *sub_1e58(register expr_t *st) {
    expr_t *pi;

    pi             = sub_23b4(0x46, st, 0); /* PMO missing 3rd arg. added 0 */
    pi->attr       = st->attr;
    pi->attr.i_sym = 0;
    pi->attr.c7    = SNODE;
    sub_5be1(&pi->attr);
    return pi;
}

/**************************************************
 * 36: 1EBD PMO +++
 * differences due to added 3rd arg and uint8_t param
 **************************************************/
expr_t *sub_1ebd(register expr_t *st) {
    st = allocSType(&st->attr);
    st->attr.i4 >>= 1;
    return sub_1441(T_SIZEOF, st, 0); /* PMO fixed missing 3rd arg */
}

/**************************************************
 * 37: 1EF1 PMO +++
 **************************************************/
bool sub_1ef1(register expr_t *st) {

    switch (st->tType) {
    case T_69:
        return true;
    case T_ID:
        return (/* st->t_pSym->m18 */ GetWord((char*)(st->t_pSym)+OFF_m18) & 0x10) &&
	        /* st->t_pSym->m20 */ GetByte((char*)(st->t_pSym)+OFF_m20) != (char)D_CONST &&
	        st->attr.c7 == SNODE;
    case T_DOT:
        return st->attr.c7 == SNODE && sub_1ef1(st->t_next);
    }
    return false;
}

/**************************************************
 * 38: 1F5D PMO +++
 * minor differences due to uint8_t param
 **************************************************/
expr_t *sub_1f5d(register expr_t *st, s8_t *p2, int16_t p3) {
    s8_t *pAttr;

    pAttr = &st->attr;
    if (sub_591d(pAttr, p2) == 0) {
        if (sub_5ad5(pAttr) && sub_5ad5(p2)) {
            if (sub_5b08(p2) && sub_5b38(pAttr))
                prWarning("implicit conversion of float to integer");
        } else if ((p2->i4 & 1) && p2->c7 == SNODE && sub_5b08(pAttr)) {
            if (p3 == 0 && (sub_5a76(pAttr, DT_CONST) || sub_5a76(pAttr, DT_UCONST)))
                return st;
            if (!sub_2105(st))
                prWarning("illegal conversion of integer to pointer");
        } else if ((pAttr->i4 & 1) && pAttr->c7 == SNODE && sub_5b08(p2))
            prWarning("illegal conversion of pointer to integer");
        else if ((pAttr->i4 & 1) && pAttr->c7 == SNODE && (p2->i4 & 1) && p2->c7 == SNODE) {
            if (!sub_5a4a(pAttr) && !sub_5a4a(p2))
                prWarning("illegal conversion between pointer types");
        } else
            prError("illegal conversion");
    }
    return sub_1bf7(st, p2);
}

/**************************************************
 * 39: 2105 PMO +++
 **************************************************/
bool sub_2105(register expr_t *st) {

    switch (st->tType) {
    case T_ICONST:
        return st->t_ul == 0L; /* long */
    case T_124:
        if (sub_5b08(&st->attr))
            return sub_2105(st->t_next);
        break;
    }
    return false;
}

/**************************************************
 * 40: 2157 PMO +++
 **************************************************/
bool s13ReleaseFreeList(void) {
    register expr_t *st;

    if (s13FreeList == 0)
        return false;
    while ((st = s13FreeList)) {
        s13FreeList = st->t_next;
        free(st);
    }
    return true;
}

/**************************************************
 * 41: 2186 PMO +++
 **************************************************/
expr_t *s13Alloc(uint8_t tok) {
    register expr_t *st;

    if (s13FreeList != 0) {
        st          = s13FreeList;
        s13FreeList = st->t_next;
        blkclr(st, sizeof(expr_t));
    } else
        st = xalloc(sizeof(expr_t));
    st->tType         = tok;
    st->attr.dataType = DT_VOID;
    return st;
}

/**************************************************
 * 42: 21C7 PMO +++
 * uint8_t paramater
 **************************************************/
expr_t *sub_21c7(register expr_t *st) {
    expr_t *l1;
    uint16_t l2;

    l1  = s13Alloc(0);
    *l1 = *st;
    l2  = opTable[st->tType - 60].uc4;
    if ((l2 & 1) || st->tType == T_120)
        return l1;

    l1->t_next = sub_21c7(st->t_next);
    if (l2 & 2)
        l1->t_alt = sub_21c7(st->t_alt);

    return l1;
}

/**************************************************
 * 43: 225A PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *sub_225a(uint8_t p1, register expr_t *st, expr_t *p3) {
    expr_t *l1;

    if (p1 == T_124 && st->tType == T_ICONST) {
        st->attr = p3->attr;
        sub_2569(p3);
        return st;
    }
    l1 = sub_23b4(p1, st, p3); /* m1:  */

    switch (opTable[p1 - 60].c7) {
    case 1:
        l1->attr = st->attr;
        break;
    case 2:
        l1->attr = p3->attr;
        break;
    case 4:
        l1->attr = st->attr;
        sub_5be1(&l1->attr);
        break;
    case 3:
        l1->attr = st->attr;
        sub_5b99(&l1->attr);
        break;
    case 5:
        l1->attr.dataType = DT_INT;
        break;
    case 6:
        l1->attr.dataType = DT_2;
        break;
    case 7:
        l1->attr.dataType = DT_VOID;
        break;
    }
    return l1;
}

/**************************************************
 * 44: 23B4 PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *sub_23b4(uint8_t tok, register expr_t *st, expr_t *p3) {
    expr_t *pi;

    pi = s13Alloc(tok);
    if (tok != T_120) {
        pi->t_next = st;
        if (opTable[tok - 60].uc4 & 2)
            pi->t_alt = p3;
    }
    return pi;
}

/**************************************************
 * 45: 240E PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *allocId(register sym_t *st) {
    expr_t *pi;

    pi         = s13Alloc(T_ID);
    pi->t_pSym = st;
    if ((/* st->m18 */ GetWord((char*)st+OFF_m18) & 0x10) || 
	 /* st->m20 */ GetByte((char*)st+OFF_m20) == (char)D_MEMBER)
        pi->attr = st->attr;
    return pi;
}

/**************************************************
 * 46: 245D PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *allocSConst(void) {
    register expr_t *st;

    st                = s13Alloc(T_SCONST);
    st->attr.dataType = DT_CHAR;
    st->attr.i4       = 1;
    st->t_i0          = ++strId;
    return st;
}

/**************************************************
 * 47: 248A PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *allocIConst(long p1) {
    register expr_t *st;

    st      = s13Alloc(T_ICONST);
    st->t_l = p1;
    return st;
}

/**************************************************
 * 48: 24B6 PMO +++
 * uint8_t parameter
 **************************************************/
expr_t *allocSType(s8_t *p1) {
    register expr_t *st;

    st       = s13Alloc(S_TYPE);
    st->attr = *p1;
    return st;
}

/**************************************************
 * 49: 24DE PMO +++
 **************************************************/
void complexErr(void) {

    fatalErr("expression too complex");
}

/**************************************************
 * 50: 24E7 PMO +++
 **************************************************/
void pushS13(expr_t *p1) {

    if (s13SP == s13Stk)
        complexErr();
    *(--s13SP) = p1;
}

/**************************************************
 * 51: 250A PMO +++
 **************************************************/
expr_t *popExpr(void) {

    if (s13SP != &s13Stk[20])
        return *(s13SP++);
    return NULL;
}

/**************************************************
 * 52: 2529 PMO +++
 **************************************************/
void sub_2529(uint8_t p1) {
    register s2_t *st;

    if (p2List == s2_9cf3)
        complexErr();
    (--p2List)->type1    = p1;
    (st = p2List)->type2 = opTable[p1 - 60].c3;
}

/**************************************************
 * 53: 255D PMO +++
 **************************************************/
uint8_t sub_255d(void) {

    return (p2List++)->type1;
}

/**************************************************
 * 54: 2569 PMO +++
 **************************************************/
void sub_2569(register expr_t *st) {
    uint8_t type;

    if (st) {
        type = st->tType;
        if (!(opTable[type - 60].uc4 & 1) && type != T_120) {
            sub_2569(st->t_next);
            if (opTable[type - 60].uc4 & 2)
                sub_2569(st->t_alt);
        }
        if (!inData(st)) {
            if (type == T_FCONST)
                free(st->t_s);
            st->t_next  = s13FreeList; /* m2: */
            s13FreeList = st;
        }
    }
}

/**************************************************
 * 55: 25F7 PMO +++
 * uint8_t parameter + minor optimisation difference
 * code is equivalent
 **************************************************/
expr_t *sub_25f7(register expr_t *st) {

    if (st) {
        if (!inData(st) && st->tType == T_ICONST) {
            st->t_ul += 1;
        } else if (st->tType == T_PLUS)
            st->t_alt = sub_25f7(st->t_alt);
        else
            st = sub_1441(T_PLUS, st, sub_1b4b(1L, DT_INT)); /* m3: */
    }
    return st; /* m4: */
}
