#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "optim1.h"

/**************************************************************************
 46	evalExpr	sub_3313  ok++   (PMO) apart from optimiser changes
 1) one code optimisation to remove jp, putting condition code on call
 2) code block moved with optimisation on shared code
 3) two further code block group moves
 Note the other code is identical and overall the logic is the same
 **************************************************************************/
typedef struct {
    int type; /* 01 s->str */
    int prec; /* 23 s->i_2 */
} op_t;

    char expectOp;
    term_t *pTerm;
    term_t termStack[30];
    op_t opStack[30];

term_t *evalExpr() {
register     op_t *pOp;

#ifdef DEBUG
    pr_warning("evalExpr");
#endif

    pTerm     = &termStack[30];
    pOp       = &opStack[29];
    pOp->type = T_MARKER;
    pOp->prec = 0;
    expectOp  = false;
    do {                                 /* REDUCE loop */
        for (;; tokType = get_token()) { /* SHIFT loop */
            if (tokType == T_STRING) {   /* in expressions "A" is treated as ascii value of A */
                if (strlen(yylval.pChar) != 1) {
                    pr_warning("Bad character const");
                }
                yylval.i = *yylval.pChar;
                tokType  = T_CONST;
            }
            if (T_FWD <= tokType && tokType <= T_CONST) { /* get the term, note two terms together is an error */
                if (expectOp) {
                    exp_err();
                }
                expectOp = true; /* flag as expect operator next */
                --pTerm;         /* where to push the term */
                switch (tokType) {
                    case T_SYMBOL: /* its reocatable */
                        pTerm->tPSym = yylval.pSym;
                        pTerm->val   = 0;
                        break;
                    case T_CONST: /* its a constant */
                        pTerm->val   = yylval.i;
                        pTerm->tPSym = NULL;
                        break;
                }
                continue;
            } else if (T_UPLUS <= tokType && tokType <= T_LASTOP) {           /* get the operator */
                if (!expectOp && (tokType == T_PLUS || tokType == T_MINUS)) { /* map unary +/- */
                    tokType  = tokType - 7;
                    yylval.i = 8; /* set its precedence */
                }
                if (tokType <= T_MARKER) {
                    if (expectOp) {
                        exp_err();
                    }
                } else {
                    if (!expectOp && tokType != T_OPAR) { /* binary op only when expecting op*/
                        exp_err();
                    }
                    if (pOp->prec >= yylval.i) { /* pick up precedence */
                        break;
                    }
                }
                if (tokType != T_CPAR) { /* not a closing ) */
                    --pOp;
                    pOp->type = tokType;   /* push its type */
                    if (tokType == T_OPAR) { /* if it was a ( then set prec to 1 */
                        yylval.i = 1;
                    } else {                                 /* OPTIMISER[2]: code moved to here */
                        expectOp = false;    /* now expecting a term */
                    }
                    pOp->prec = yylval.i; /* set the prec */ /* OPTIMISER[3] code block moved / shared */
                } else if (pOp->type ==
                           T_MARKER) /* ) with nothing on stack */ { /* OPTIMISER[4]: test code block moved */
                    break;
                }
            } else {
                break;
            }
        }

        /* REDUCE phase */
        if (pOp->type == T_OPAR) { 	/* check for matching () */
            if (tokType != T_CPAR) {
                exp_err();    		/* ")" */
            }
            tokType  = get_token(); 	/* prime for next part */
            expectOp = 1;           	/* assuming its a term */
        } else if (T_MARKER >= pOp->type) {
            uconv(pOp->type, pTerm);    /* calculate unary operator */
        } else {
            bconv(pOp->type, pTerm + 1, pTerm); /* calculate binary operator*/
            ++pTerm;
        }
    } while (++pOp != &opStack[30]); 	/* loop till end of operator stack */

    exprResult = *pTerm;
    if (&termStack[29] != pTerm) { 	/* oops outstanding term */
        exp_err();
    }
    return &exprResult;
}

/**************************************************************************
 47 exp_err     sub_32bf    ok++ (PMO)
 **************************************************************************/
void exp_err() {

    pr_error("Expression error");
}

/**************************************************************************
 48 sub_359e    ok++ (PMO)
 *
 *  Unary operators
 **************************************************************************/
void uconv(int op, register term_t *lhs) {

    switch (op) {
        case T_UMINUS:
            lhs->val = -lhs->val;
            break;
        case T_NOT:
            lhs->val = ~lhs->val;
            break;
        case T_HI:
            lhs->val = (lhs->val >> 8) & 0xff;
            break;
        case T_LOW:
            lhs->val &= 0xff;
            break;
        case T_UPLUS:
            return;
        case T_MARKER:
            return;
        default:
            pr_error("uconv - bad op");
    }
    if (lhs->tPSym) {
        rel_err();
    }
    return;
}

/**************************************************************************
 49 sub_3630    ok++ (PMO)
 *
 *  Binary operators
 **************************************************************************/
void bconv(int op, register term_t *lhs, term_t const *rhs) {

    switch (op) {
        case T_PLUS:
            if (lhs->tPSym && rhs->tPSym) {
                rel_err();
            }
            lhs->val += rhs->val;
            if (!lhs->tPSym) {
                lhs->tPSym = rhs->tPSym;
            }
            return;
        case T_MINUS:
            if (rhs->tPSym) {
                rel_err();
            }
            lhs->val -= rhs->val;
            return;
        case T_MUL:
            lhs->val *= rhs->val;
            break;
        case T_DIV:
            lhs->val /= rhs->val;
            break;
        case T_MOD:
            lhs->val %= rhs->val;
            break;
        case T_SHR:
            lhs->val >>= rhs->val;
            break;
        case T_SHL:
            lhs->val <<= rhs->val;
            break;
        case T_AND:
            lhs->val &= rhs->val;
            break;
        case T_OR:
            lhs->val |= rhs->val;
            break;
        case T_XOR:
            lhs->val ^= rhs->val;
            break;
        case T_EQ:
            lhs->val = lhs->val == rhs->val;
            break;
        case T_LE:
            lhs->val = rhs->val < lhs->val;
            break;
        case T_GE:
            lhs->val = lhs->val < rhs->val;
            break;
        case T_ULE:
            lhs->val = (uint16_t)rhs->val < (uint16_t)lhs->val;
            break;
        case T_UGE:
            lhs->val = (uint16_t)lhs->val < (uint16_t)rhs->val;
            break;
        default:
            pr_error("Bconv - bad op");
            break;
    }

    if (lhs->tPSym || rhs->tPSym) {
        rel_err();
    }
}

/**************************************************************************
 25     sub_1397        ok++ (PMO)
 **************************************************************************/
/* note there are occasions when pi is accessed after this is called so
 * freeInst has to preserve at least pi->pNext
 */
void removeInstr(register inst_t *pi) {
    sym_t *s;

#ifdef DEBUG
    pr_warning("removeInstr pi=%04x", pi);
#endif

    if (pi->type == T_JP &&
        /* pi->iLhs->type */ GetByte((char*)(pi->iLhs), OFF_type) == (uint8_t)T_CONST &&
        /* pi->iLhs->oPSym */ (s = (sym_t*)GetWord((char*)(pi->iLhs), OFF_oPSym)) ) {
        removeLabelRef(/* pi->iLhs->oPSym */ s );
    }

    pi->pAlt->pNext = pi->pNext;
    pi->pNext->pAlt = pi->pAlt;
    if (pi->type != T_SYMBOL) {
        freeOperand(pi->iLhs);
        freeOperand(pi->iRhs);
    }
    freeInst(pi);
}

/**************************************************************************
 29     sub_153d        ok++
 **************************************************************************/
void removeLabelRef(register sym_t *ps) {
    inst_t *pi;

#ifdef DEBUG
    pr_warning("removeLabelRef");
#endif

    if (!(pi = ps->p.pInst)) {
        return;
    }
    if (pi->aux == 0) {
        pr_error("Refc == 0");
    }
    if (--pi->aux != 0) {
        return;
    }
    removeInstr(pi);
    ps->p.pInst = NULL;
    freeSymbol(ps);
    logOptimise(O_UNREF_LAB); /* 6fc5 opt_msg[4] = "Unref'ed labels" */
}

/**************************************************************************
 45 num_psect   sub_32bf    ok++
 **************************************************************************/
int num_psect(char const *fmt) {
    int l1;

#ifdef DEBUG
    pr_warning("num_psect");
#endif

    for (l1 = 0; l1 < 4; ++l1) {
        if (strcmp(fmt, psectNames[l1]) == 0) {
            return l1;
        }
    }
    pr_error("Unknown psect");
}

/**************************************************************************
 1	strtoi	sub_013d	ok++	Used in const_value
 *
 *	Converts a numeric string, in various bases, to a integer
 **************************************************************************/
int strtoi(char const *s, int base) {
    int val;   /* number */
    int digit; /* digit */

    val = 0;
    while (*s) {
        val *= base;
        if (ISDIGIT(*s)) {
            digit = *(s++) - '0';
        } else {
            if (ISXDIGIT(*s)) {
                digit = (ISUPPER(*s) ? (char)(*s | 0x20) : (char) * s) - ('a' - 10);
            }
        }
        /* #pragma warning(suppress : 6001) /* function only called after verified there is a digit */
        if (digit >= base) {
            pr_warning("Digit out of range");
            return 0;
        }
        val += digit;
    }
    return val;
}

/**************************************************************************
 2	ptr_token	sub_020f	ok++
 *
 *	Returns a pointer to a token
 **************************************************************************/
char *ptr_token() {
    register char *s;

#ifdef DEBUG
    pr_warning("ptr_token");
#endif

    s = yyline;
    while (PEEKCH() != 0 && PEEKCH() != '\n') {
        *s++ = GETCH();
    }
    *s        = '\0';
    charsLeft = 0;
    return yyline;
}

/**************************************************************************
 3	const_value	sub_0289	ok+
 *
 *	Rerurn integer value numeric string in various bases
 *	Generates correct code, but in a different sequence
 **************************************************************************/
int const_value() {
    register char *pc;
    uint8_t base;

    pc   = yyline + 1;
    base = 0;
    while (ISXDIGIT(PEEKCH())) {
        *pc++ = GETCH();
    }

    switch (PEEKCH()) { /* m17: */
        case 'H':
        case 'h':
            base = 16;
            /* #pragma warning(suppress : 6269) /* deference ignored - original code did the dreference */
            GETCH();
            break;

        case 'O':
        case 'o':
        case 'Q':
        case 'q':
            base = 8;
            /* #pragma warning(suppress : 6269) /* deference ignored - original code did the dreference */
            GETCH();
            break;
        default:
            if (pc[-1] == 'f' || pc[-1] == 'b') {
                yytype   = *--pc == 'f' ? T_FWD : T_BWD;
                *pc      = '\0';
                yylval.i = (int16_t)strtoi(yyline, 10);
                return yytype;
            }
            if (pc[-1] == 'B') {
                pc--;
                base = 2;
            }
            break;
    }
    *pc = '\0';
    if (base == 0) {
        base = 10;
    }
    yylval.i = (int16_t)strtoi(yyline, base);
    return T_CONST;
}

/**************************************************************************
 4  get_token   sub_03c7    ok++ (PMO)
 **************************************************************************/
int get_token() {
    int c;
    register char *pc;

#ifdef DEBUG
    pr_warning("get_token");
#endif

    for (;;) {
        pc = yyline;

        switch (c = GETCH()) {
            case -1:
                return -1;
            case '\t':
            case '\f':
            case ' ':
                continue;
            case 0:
            case ';':
                return T_EOL;
            case '\'': /* Single quote (apostrophe) */
                while (PEEKCH() && PEEKCH() != '\'') {
                    *pc++ = GETCH();
                }
                if (PEEKCH() == '\'')
                    /* #pragma warning(suppress : 6269) /* deference ignored - original code did the dreference */
                {
                    GETCH();
                } else {
                    pr_warning("Unterminated string");
                }
                *pc          = '\0';
                yylval.pChar = yyline;
                return T_STRING;

            case '.':
                *pc++ = c;
                while (ISALPHA(PEEKCH())) {
                    *pc++ = GETCH();
                }
                if (PEEKCH() == '.') {			/* m27: */
                    *pc++ = GETCH();
                }
                break;
            case ',':
                return T_COMM;				/* m31: */

            case ':':
                return T_COLN;				/* m32: */

            case '(':
                yylval.i = P_OPAR;			/* m33: */
                return T_OPAR;

            case ')':
                yylval.i = P_CPAR;			/* m34: */
                return T_CPAR;

            case '+':
                yylval.i = P_ADD;			/* m35: */
                return T_PLUS;

            case '-':
                yylval.i = P_SUB;			/* m36: */
                return T_MINUS;

            case '*':
                if (ptr_inbuf == inp_buf + 1) {
                    printf("%s\n", inp_buf);
                    charsLeft = 0;
                    continue;
                }
            default:
                *pc++ = c;
                if (ISALPHA(c)) {
                    while (ISALNUM(PEEKCH())) {		/* m41: */
                        *pc++ = GETCH();
                    }
                    if (pc == yyline + 2) {
                        if (PEEKCH() == '\'') {
                            *pc++ = GETCH();
                        }
                    }
                } else if (ISDIGIT(c)) {
                    return const_value();
                }
                break;
        }
        while (ISSPACE(PEEKCH()))
            /* #pragma warning(suppress : 6269) /* deference ignored - original code did the dreference */
        {
            GETCH();
        }

        *pc = '\0';
        if (yyline[0] == '_' || ISDIGIT(pc[-1]) || ((yytype = num_token(yyline)) == -1)) {
            if (ISALPHA(yyline[0])) {			/* m57: */
                yylval.pSym = lookupSym(yyline);
                return T_SYMBOL;
            }
            pr_warning("Lexical error");		/* m58: */
        } else {
            return yytype;				/* m59: */
        }
    }
}

/**************************************************************************
 5	get_line	sub_0758	ok++ (PMO)
 *	Optimiser eliminates a jump otherwise code is identical
 **************************************************************************/
/* PMO this code contains a bug. If strlen returns a 0 it accesses inp_buf[-1]
 * and could overwrite with a 0, this could occur if there is a '\0' in the input stream
 * it is also not clear whether a blank line i.e. one with just a '\n' should
 * return with a '\0' as it currently does or try another line
 *
 * Note needs fixing for linux as '\r' will not be removed on input
 *
 * The compiler can generate comment lines > 80 chars, this causes this function
 * to split the line which may cause the optimiser to get confused
 * a simple option would be to extend the size of inp_buf alternatively as the only
 * long lines appear to be comment ones. A better option would be to detect for a missing '\n'
 * and junk the rest of the line
 *
 * WARNING: The Hitech fgets contains a bug in that if line read has length >= size
 * it will write a 0 at buf[size], for optim this is the -l option flag
 *
 */
int get_line() {

#ifdef DEBUG
    pr_warning("get_line");
#endif

    for (;;) {
        if (fgets(inp_buf, sizeof(inp_buf), stdin) == NULL) {
            return -1;
        }
        charsLeft = (int)strlen(ptr_inbuf = inp_buf);
        if (inp_buf[charsLeft - 1] == '\n') { /* If penultimate character is */
            inp_buf[charsLeft - 1] = '\0';    /* '\n' then replace it with 0 */
        }
        if (--charsLeft >= 0) {             /* got something */
            return *ptr_inbuf++;            /* otherwise, return first character of string */
        }
    }
}

/**************************************************************************
 6	clr_len_inbuf	sub_0758  ok++	Used in: getOperands, loadFunction
 **************************************************************************/
void clr_len_inbuf() {

    charsLeft = 0;
}

/**************************************************************************
 11	find_token	sub_09d0	ok++	Used in: num_token
 *
 *	Binary search is used
 * Minor difference after strcmp. the code sequence
 * original           new
 * ld (ix-1),l        ld a,l
 * ld a,l             ld (ix-1),a
**************************************************************************/
int find_token(register char const *str, operator_t const *p2, int p3) {
    char    cmp;
    uint8_t high, low, mid;

#ifdef DEBUG
    pr_warning("find_token");
#endif

    tableBase = p2;
    low       = 0;
    high      = p3 - 1;
    do {
        mid = (high + low) / 2;
        cmp = strcmp(str, tableBase[mid].str);
        if (cmp == 0) {
            yylval.i = tableBase[mid].aux;
            yytext   = tableBase[mid].str;
            return tableBase[mid].type;
        }
        if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    } while (high >= low);
    return -1;
}

/**************************************************************************
 12	num_token	sub_0a97	ok++	Used in
 **************************************************************************/
int num_token(char const *fmt) {
    return find_token(fmt, operators, NOPERATORS);
}

/**************************************************************************
 13	pr_token	sub_0ab2	ok++ (PMO)
 Only difference is the new code jumps to a location that cleans up
 stack after a call before jumping to cret. The direct jp cret also works
 **************************************************************************/
void pr_token(register inst_t const *pi) {
    operator_t const *po;

#ifdef DEBUG
    pr_warning("pr_token");
#endif

    if (pi->opCode) {
        printf("%s", pi->opCode);
        return;							/* m1: */
    }

    switch (pi->type) {						/* m2: */
        case T_JP:
            printf("jp");
            return;
        case T_CALL:
            printf("call");
            return;
        case T_RET:
            printf("ret");
            return;
    }

    po = operators + NOPERATORS; /* PMO should not be + 1 */
    do {
        if (--po < operators) {					/* m3: */
            pr_error("Can't find op");
            return;
        }
    } while ((pi->type != po->type) || (po->aux != pi->aux));	/* m8: */

    printf("%s", po->str);
    return;
}

/**************************************************************************
 70 hash_index  sub_4cab    ok++    Used in: sub_4cf0
 **************************************************************************/
int hash_index(register char const *s, int size) {
    uint16_t hash;

    for (hash = 0; *s != 0; ++s) {
        hash += *(uint8_t *)s + hash;
    }
    return hash % size;
}

/**************************************************************************
 71 sub_4cf0    ok++ (PMO)          Used in: get_token
 **************************************************************************/
sym_t *lookupSym(register char const *s) {
    sym_t **pps;
    sym_t *ps;

#ifdef DEBUG
    pr_warning("lookupSym");
#endif

    pps = &hashtab[hash_index(s, HASHSIZE)];

    while (*pps && strcmp((*pps)->label, s))
        if (++pps == &hashtab[HASHSIZE]) 
            pps = hashtab;
    
    if (ps = *pps) 
        return ps;
    
    *pps = ps = allocItem();

    ps->label = alloc_mem((int)strlen(s) + 1); 

    strcpy(ps->label, s);

    return ps;
}

