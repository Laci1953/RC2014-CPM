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

expr_t *sub_0817(register s8_t *st);
int16_t peekCh(void);
sym_t *sub_69ca(uint8_t p1, register s8_t *p2, uint8_t p3, sym_t *p4);
uint8_t yylex(void);
sym_t *sub_4eed(register sym_t *st, uint8_t p2, s8_t *p3, bool v, sym_t *p4);
void sub_516c(register sym_t *st);
void sub_51cf(register sym_t *st);
uint8_t sub_5dd1(uint8_t *pscType, register s8_t *attr);
void expectErr(char *p);
void skipStmt(uint8_t tok);
void prError(int, ...);

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
//    int16_t var19; /* not used */
    register sym_t *st;
    char tmpbuf[32];

    varE = s13SP;
    varC = p2List;
    sub_2529(T_60);
    var11 = 0;
//    var19 = 0;
    for (;;) {
        var13 = (byte_8f86 = p2List->type1 == (char)T_DOT || p2List->type1 == (char)T_POINTER);
        tok               = yylex();
        byte_8f86         = false;
        if (tok < (unsigned char)T_60 || tok >= (unsigned char)T_128) {
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
                            st            = sub_4eed(st, T_EXTERN, &var8, false, 0);
                            /* st->m18 |= 0x42; */
			    PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) | 0x42);
                            /* st->m21 = 0 */ 
			    PutByte((char*)st+OFF_m21, 0);
                            sub_0493(st);
                        } else 
			{
			    GetString(tmpbuf, (char*)GetWord((char*)st+OFF_nVName));
			    prError(100+72, tmpbuf); 
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
			{
			    GetString(tmpbuf, (char*)GetWord((char*)st+OFF_nVName));
			    prError(100+71, tmpbuf);
			}
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
            if (tok2 == (char)S_TYPE || 
	        (tok2 == (char)T_ID &&
	         /* yylval.ySym->m20 */ GetByte((char*)(yylval.ySym)+OFF_m20) == (char)T_TYPEDEF)) {
                if (var11)
                    goto error;
                sub_5dd1(0, &var8);
                sub_516c(st = sub_69ca(T_79, &var8, 0, 0));
                sub_51cf(st);
                tok2 = yylex();
                if (tok2 != (char)T_RPAREN)
                    goto error;
                /* var8 = st->attr; */ 
		ReadMem((char*)&var8, (char*)st, sizeof(struct _s8));
                pushS13(allocSType(&var8));
                if (p2List->type1 == (char)T_SIZEOF) {
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
        if ((byte_9d37 >= (unsigned char)3 && tok == (char)T_COMMA) ||
            (byte_9d37 == (char)1 && tok == (char)T_COLON && p2List->type1 != (char)T_QUEST))
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
                if (p2List->type1 == (char)T_60) { /* 1058 */
                    if (tok != (char)T_60)
                        ungetTok = tok;
                    varA = popExpr();
                    if (varA && s13SP == varE)
                        goto done;
                    else
                        goto error;
                } else if (p2List->type1 == (char)T_LPAREN) {
                    if (tok != (char)T_RPAREN) {
                        expectErr(")");
                        ungetTok = tok;
                    }
                } else if (p2List->type1 == (char)T_64) {
                    if (tok != (char)T_RBRACK) {
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
    /* prError("expression syntax"); */
    prError(73);
    skipStmt(tok);
    while (s13SP != varE)
        sub_2569(popExpr());
    varA = NULL;

done:
    s13SP  = varE;
    p2List = varC;
    return varA;
}
