/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

int16_t word_9caf; /*9caf */
char ca9CB1[64];   /* 9cb1 */

void sub_573b(register sym_t *st, FILE *fp);
void sub_7454(register s8_t *st, bool vst);
bool sub_5a76(register s8_t *st, bool vst, uint8_t p2);
void prMsg(char *,...);
void prError(char *, ...);
void fatalErr(char *, ...);
void prWarning(char *, ...);

/**************************************************
 * 1: 013D PMO +++
 **************************************************/
void sub_013d(register FILE *p) {

    if (word_9caf != lineNo || strcmp(ca9CB1, srcFile) != 0) {
        fprintf(p, "\"%d", lineNo);
        if (strcmp(srcFile, ca9CB1) != 0)
            fprintf(p, " %s", srcFile);
        fputc('\n', p);
    }
    word_9caf = lineNo;
    strcpy(ca9CB1, srcFile);
    if (s_opt != 0)
        emitSrcInfo();
}

/**************************************************
 * 2: 01C1 PMO +++
 **************************************************/
void sub_01c1(sym_t *p_arg) {
    sym_t buf;
    register sym_t* p = &buf;

    ReadMem((char*)&buf, (char*)p_arg, sizeof(struct _sym));
 
    if (p->a_c7 == (char)ENODE)
        sub_0470(p->a_expr);
    else
        putchar('1');
}

/**************************************************
 * 3: 01EC PMO +++
 **************************************************/
void sub_01ec(register sym_t *p_arg) {
    sym_t *p = /* p_arg->a_nextSym */ (sym_t*)GetWord((char*)p_arg+OFF_s8_t_nextSym);

    if (/* p_arg->a_dataType */ GetByte((char*)p_arg+OFF_s8_t_a_dataType) == (char)DT_POINTER &&
	! /* p_arg->a_i4 */ GetWord((char*)p_arg+OFF_s8_t_a_i4) &&
	/* p->a_c7 */ GetByte((char*)p+OFF_s8_t_c7) != (char)ANODE &&
	!(/* p->m18 */ GetWord((char*)p+OFF_m18) & 0x100))
        sub_0493(p);
}

/**************************************************
 * 4: 022E PMO +++
 * optimiser has basic blocks moved and eliminates
 * a ld a,(ix+6)
 **************************************************/
void prFuncBrace(uint8_t tok) {
    if (tok == (char)T_RBRACE)
        putchar('}');
    else if (tok == (char)T_LBRACE)
        putchar('{');
    putchar('\n');
}

/**************************************************
 * 5: 0258 PMO +++
 **************************************************/
void emitLabelDef(int16_t p) {

    sub_013d(stdout);
    printf("[e :U %d ]\n", p); /* EXPR :U */
}

/**************************************************
 * 6: 0273 PMO +++
 **************************************************/
void sub_0273(register sym_t *st) {

    if (st) {
        sub_013d(stdout);
        printf("[e :U "); /* EXPR :U */
        sub_573b(st, stdout);
        printf(" ]\n");
    }
}

/**************************************************
 * 7: 02A6 PMO +++
 **************************************************/
void sub_02a6(case_t *p1) {
    int16_t caseCnt;
    register s4_t *st;

    if (p1) {
        sub_013d(stdout);
        printf("[\\ "); /* CASE */
        sub_0470(p1->switchExpr);
        putchar('\n');
        caseCnt = p1->caseCnt;
        for (st = p1->caseOptions; caseCnt--; st++) {
            putchar('\t');
            sub_0470(st->caseVal);
            printf(" %d\n", st->caseLabel);
        }
        putchar('\t');
        printf(".. %d\n", p1->defLabel);
        printf("]\n");
    }
}

/**************************************************
 * 8: 0353 PMO +++
 **************************************************/
void sub_0353(sym_t *p_arg, char c) {
    sym_t *st;
    sym_t symbuf;
    register sym_t* ps = &symbuf;

    if (p_arg) 
    {
	ReadMem((char*)&symbuf, (char*)p_arg, sizeof(struct _sym));

        for (st = ps->nMemberList; 
	     st != p_arg; 
	     st = /* st->nMemberList */ (sym_t*)GetWord((char*)st+OFF_nMemberList))
            sub_01ec(st);

        sub_013d(stdout);
        putchar('[');

        if (c == (char)D_STRUCT)
            putchar('s'); /* STRUCT */
        else
            putchar('u'); /* UNION */

        printf(" S%d", ps->a_labelId);

        for (st = ps->nMemberList; 
	     st != p_arg; 
	     st = /* st->nMemberList */ (sym_t*)GetWord((char*)st+OFF_nMemberList)) 
	{
            if ((/* st->m18 */ GetWord((char*)st+OFF_m18) & 0x400))
                printf(" :%d", /* st->m16 */ GetWord((char*)st+OFF_m16));
            putchar(' ');

	    sub_7454(&st->attr, true);

            putchar(' ');
            sub_01c1(st);
        }

        printf(" ]\n");
    }
}

/**************************************************
 * 9: 042D PMO +++
 **************************************************/
void sub_042d(register expr_t *p) {

    sub_013d(stdout);
    printf("[e "); /* EXPR */
    while (p && p->tType == (char)T_124)
        p = p->t_next;
    sub_0470(p);
    printf(" ]\n");
}

/**************************************************
 * 10: 0470 PMO +++
 **************************************************/
void sub_0470(expr_t *p) {

    if (p)
        sub_05f1(p);
    else
        putchar('1');
}

/**************************************************
 * 11: 0493 PMO +++
 **************************************************/
void sub_0493(sym_t *st) {
    char c;
    int m18;
    unsigned char m20;

    if (st) 
    {
        sub_013d(stdout);
        sub_01ec(st);

        /* st->m18 |= 0x100; */
	PutWord((char*)st+OFF_m18, (m18 = GetWord((char*)st+OFF_m18) | 0x100));

        printf("[v "); /* VAR */
        sub_573b(st, stdout);
        putchar(' ');

        sub_7454(&st->attr, true);

        putchar(' ');

        if (/* st->m18 */ m18 & 1)
            sub_01c1(st);
        else
            putchar('0');

        if (/* st->m20 */ (m20 = GetByte((char*)st+OFF_m20)) == (char)D_6)
            c = (/* st->m18 */ m18 & 0x200) ? 'r' : 'p';
        else
            c = *keywords[/* (unsigned)st->m20 */ m20 - 19];

        if (/* st->m18 */ m18 & 4)
            c += 0xE0;

        printf(" %c ]\n", c);
    }
}

/**************************************************
 * 12: 053F PMO ++
 **************************************************/
void sub_053f(register expr_t *st, char *pc) {
    int16_t var2;

    sub_013d(tmpFp);
    fprintf(tmpFp, "[a %d", st->t_i0);
    var2 = st->t_i2;
    do {
        fprintf(tmpFp, " %d", *pc++);
    } while (var2--);
    fprintf(tmpFp, " ]\n");
}

/**************************************************
 * 13: 05B5 PMO +++
 **************************************************/
void sub_05b5(expr_t *p1) {

    sub_013d(stdout);
    sub_0470(p1);
    putchar('\n');
}

/**************************************************
 * 14: 05D3 PMO +++
 **************************************************/
void sub_05d3(expr_t *p1) {

    sub_013d(stdout);
    sub_0470(p1);
    putchar('\n');
}

/**************************************************
 * 15: 05F1 PMO +++
 * Optimiser passes only uint8_t to sub_5a76 as it
 * has the function prototype
 **************************************************/
void sub_05f1(register expr_t *st) {
    t8_t *var2;
    sym_t *var4;
    expr_t *var6;

    var2 = &opTable[st->tType - T_60];
    if (var2->uc4 & 1) {
        switch (st->tType) {
        case T_ID:
            var4 = st->t_pSym;
            if (/* var4->m20 */ GetByte((char*)var4+OFF_m20) == (char)D_CONST) {
                printf(". `");
                sub_573b(/* var4->nMemberList */ (sym_t*)GetWord((char*)var4+OFF_nMemberList), stdout);
                printf(" %d", /* var4->m14 */ GetWord((char*)var4+OFF_m14));
            } else
                sub_573b(var4, stdout);
            break;
        case T_ICONST:
            if (sub_5a76(&st->attr, false, DT_VOID) != 0) {
                printf("%ld", st->t_ul);
                break;
            } else
                printf("-> %ld ", st->t_ul);
        /* FALLTHRU */
        case S_TYPE:
            sub_7454(&st->attr, false);
            break;
        case T_FCONST:
            printf(".%s", st->t_s);
            break;
        case T_SCONST:
            printf(":s %d", st->t_i0);
            break;
        case T_126:
            printf("%d", st->t_i0); /* m12: */
            break;
        }
    } else {
        if (st->tType == (char)T_SIZEOF)
            printf("-> ");
        if (st->tType == (char)T_SIZEOF && (var6 = st->t_next)->tType == (char)T_ID &&
            !( /* var6->t_pSym->m18 */ GetWord((char*)(var6->t_pSym)+OFF_m18) & 1)) {
            printf("* # ");
            sub_7454(&var6->attr, false);
            putchar(' ');
            sub_01c1(var6->t_pSym);
            putchar(' ');
            sub_7454(&st->attr, false);
        } else {
            printf("%.3s ", var2->s0);
            if (st->tType != (char)T_120) {
                sub_05f1(st->t_next);
                if (var2->uc4 & 2) {
                    putchar(' ');
                    sub_05f1(st->t_alt);
                } else if (st->tType == (char)T_SIZEOF) {
                    putchar(' ');
                    sub_7454(&st->attr, false);
                }
            }
        }
    }
}

/**************************************************
 * 16: 07E3 PMO +++
 * optimiser removes call csv & jp cret
 **************************************************/
void sub_07e3(void) {
    s13SP       = &s13Stk[20];
    s13FreeList = 0;
}
