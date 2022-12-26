#include "p1.h"

char *keywords[] = { /* 8f87 */
                     "asm",      "auto",   "break",  "case",   "char",   "continue", "default",
                     "do",       "double", "else",   "@@@@@",  "enum",   "extern",   "float",
                     "for",      "goto",   "if",     "int",    "long",   "register", "return",
                     "short",    "sizeof", "static", "struct", "switch", "typedef",  "union",
                     "unsigned", "void",   "while"
};

char lastEmitSrc[64];  /* 9d60 */
bool sInfoEmitted;     /* 9da0 */
int16_t inCnt;         /* 9da1 */
char lastEmitFunc[40]; /* 9da3 */
YYTYPE yylval;         /* 9dcb */
char nameBuf[32];      /* 9dcf */
uint8_t ungetTok;      /* 9def */

int16_t strChCnt;    /* 9df0 */
bool lInfoEmitted;   /* 9df2 */
int16_t startTokCnt; /* 9df3 */
int16_t ungetCh;     /*  9df5 */

uint8_t parseNumber(int16_t ch);
uint8_t parseName(int8_t ch);
void parseAsm(void);
void parseString(int16_t ch);
int16_t getCh(void);
void prErrMsg(void);
int16_t skipWs(void);
int8_t escCh(int16_t ch);


/**************************************************
 * 56: 2671 PMO +++
 * location of two basic blocks swapped, code equivalent
 **************************************************/
uint8_t yylex(void) {
    int16_t ch;
    uint8_t tok;
    char buf[50];
    register char *s;

    if (ungetTok) {
        tok      = ungetTok;
        ungetTok = 0;
        if (tok == T_ID && byte_8f86)
            yylval.ySym = sub_4e90(nameBuf);
        return tok;
    }
    for (;;) {
        ch          = skipWs();
        startTokCnt = inCnt;
        if (Isalpha(ch))
            return parseName((int8_t)ch);
        if (Isdigit(ch))
            return parseNumber(ch);
        switch (ch) {
        case EOF:
            return T_EOF;
        case '#':
            do {
                ch = getCh();
            } while (Isspace(ch) && ch != '\n');
            if (Isdigit(ch) && parseNumber(ch) == T_ICONST) {
                lineNo = (int16_t)(yylval.yNum - 1);
                do {
                    ch = getCh();
                } while (Isspace(ch) && ch != '\n');
                if (ch == '"') {
                    for (s = buf; (ch = getCh()) != '"' && ch != '\n';)
                        *s++ = (char)ch;
                    *s = '\0';
                    if (buf[0])
                        strcpy(srcFile, buf);
                    else if (srcFileArg)
                        strcpy(srcFile, srcFileArg);
                    else
                        *srcFile = '\0';
                    if (crfFp)
                        fprintf(crfFp, "~%s\n", srcFile);
                }
                break;
            } else {
                s = buf;
                do {
                    *s++ = (char)ch;
                    ch   = getCh();
                } while (ch != '\n' && !Isspace(ch));
                *s = '\0';
                while (ch != '\n')
                    ch = getCh();
                if (strcmp(buf, "asm") == 0) {
                    parseAsm();
                    break;
                } else
                    fatalErr("illegal '#' directive");
            }
            /* FALLTHRU */
        case '"':
            parseString('"');
            return T_SCONST;
        case '\'':
            ch          = getCh();
            yylval.yNum = (ch == '\\') ? escCh(getCh()) : ch;
            ch          = getCh();
            if (ch == '\n')
                expectErr("closing quote");
            else if (ch != '\'')
                prError("char const too long");

            while (ch != '\n' && ch != '\'')
                ch = getCh();
            return T_ICONST;
        case ';':
            return T_SEMI;
        case ':':
            return T_COLON;
        case '+':
            ch = getCh();
            if (ch == '+')
                return T_INC;
            ungetCh = ch;
            ch      = skipWs();
            if (ch == '=')
                return T_PLUSEQ;
            ungetCh = ch;
            return T_PLUS;
        case '-':
            ch = getCh();
            if (ch == '-')
                return T_DEC;
            if (ch == '>')
                return T_POINTER;

            ungetCh = ch;
            ch      = skipWs();
            if (ch == '=')
                return T_MINUSEQ;
            ungetCh = ch;
            return T_MINUS;
        case '*':
            ch = skipWs();
            if (ch == '=')
                return T_MULEQ;
            ungetCh = ch;
            return T_STAR; /* deref or multiply */
        case '/':
            ch = skipWs();
            if (ch == '=')
                return T_DIVEQ;
            ungetCh = ch;
            return T_DIV;
        case '%':
            ch = skipWs();
            if (ch == '=')
                return T_MODEQ;
            ungetCh = ch;
            return T_MOD;
        case '&':
            ch = getCh();
            if (ch == '&')
                return T_LAND;
            ungetCh = ch;
            ch      = skipWs();
            if (ch == '=')
                return T_ANDEQ;
            ungetCh = ch;
            return T_BAND;
        case '|':
            ch = getCh();
            if (ch == '|')
                return T_LOR;
            ungetCh = ch;
            ch      = skipWs();
            if (ch == '=')
                return T_OREQ;
            ungetCh = ch;
            return T_BOR;
        case '^':
            ch = skipWs();
            if (ch == '=')
                return T_XOREQ;
            ungetCh = ch;
            return T_XOR;
        case '<':
            ch = getCh();
            if (ch == '<') {
                ch = skipWs();
                if (ch == '=')
                    return T_SHLEQ;
                else {
                    ungetCh = ch;
                    return T_SHL;
                }
            } else if (ch == '=')
                return T_LE;
            ungetCh = ch;
            return T_LT;
        case '>':
            ch = getCh();
            if (ch == '>') {
                ch = skipWs();
                if (ch == '=')
                    return T_SHREQ;
                else {
                    ungetCh = ch;
                    return T_SHR;
                }
            } else if (ch == '=')
                return T_GE;
            ungetCh = ch;
            return T_GT;
        case '=':
            ch = getCh();
            if (ch == '=')
                return T_EQEQ;
            ungetCh = ch;
            return T_EQ;
        case '!':
            ch = getCh();
            if (ch == '=')
                return T_NE;
            ungetCh = ch;
            return T_LNOT;
        case '~':
            return T_BNOT;
        case '(':
            return T_LPAREN;
        case ')':
            return T_RPAREN;
        case '[':
            return T_LBRACK;
        case ']':
            return T_RBRACK;
        case '{':
            return T_LBRACE;
        case '}':
            return T_RBRACE;
        case '.':
            ch = getCh();
            if (Isdigit(ch)) {
                ungetCh = ch;
                return parseNumber('.');
            }
            if (ch == '.') {
                ch = getCh();
                if (ch != '.')
                    prError("'.' expected after '..'");
                return T_3DOT;
            }
            ungetCh = ch;
            return T_DOT;

        case '?':
            return T_QUEST;
        case ',':
            return T_COMMA;
        default:
            prError("illegal character 0%o", ch /* ,ch*/); /* original had ch twice!! */
            break;
        }
    }
}

/**************************************************
 * 57: 2CC3 PMO +++
 * two blocks change from ex de,hl ld de,xxx
 * to ld hl,xxx ex de,hl (xxx are to locations on stack)
 * optimiser also uses byte compare for digit >= base
 **************************************************/
uint8_t parseNumber(int16_t ch) {
    long lval;
    uint8_t base;
    char buf[50];
    uint8_t digit;
    register char *s = buf;

    while (Isdigit(ch)) {
        *s++ = (char)ch;
        ch   = getCh();
    }
    if (ch == '.' || ch == 'e' || ch == 'E') {
        if (ch == '.')
            do {
                *s++ = (char)ch;
                ch   = getCh();
            } while (Isdigit(ch));
        if (ch == 'e' || ch == 'E') {
            *s++ = 'e';
            ch   = getCh();
            if (ch == '+' || ch == '-') {
                *s++ = (char)ch;
                ch   = getCh();
            }
            if (Isdigit(ch))
                do {
                    *s++ = (char)ch;
                    ch   = getCh();
                } while (Isdigit(ch));
            else
                prError("exponent expected");
        }
        ungetCh = ch;
        *s++      = 0;
        if (*buf == '.')
            s++;
        yylval.yStr = xalloc(s - buf);
        if (*buf == '.')
            strcat(strcpy(yylval.yStr, "0"), buf);
        else
            strcpy(yylval.yStr,buf);
        return T_FCONST;
    }
    base = 10;

    /* 
        original code would allow invalid numbers such as
        99x123 and 0999X123 both as 0x123
    */
    if ((ch == 'x' || ch == 'X') && s == buf + 1 && *buf == '0') {
        base = 16;
        s    = buf;
        while (Isxdigit(ch = getCh()))
            *s++ = (char)ch;
        if (s == buf)
            prError("hex digit expected");
    } else if (*buf == '0')
        base = 8;
    lval = 0L;
    *s   = 0;
    s    = buf;
    while (*s) {
        if (*s >= 'A')
            digit = (*(uint8_t *)s++ | 0x20) - 'a' + 10;
        else
            digit = *(uint8_t *)s++ - '0';
        if (digit >= base) {
            prError("digit out of range");
            break;
        }
        lval = lval * base  + digit;
    }
    yylval.yNum = lval;
    if (ch == 'l' || ch == 'L')
        return T_LCONST;
    ungetCh = ch;
    return T_ICONST;
}

/**************************************************
 * 58: 2F75 PMO +++
 * uses ld hl,xxx ex de,hl compared to ex de,hl ld de,xxx
 * equivalent code
 **************************************************/
uint8_t parseName(int8_t ch) {
    int16_t len;
    uint8_t mid;
    uint8_t lo;
    uint8_t hi;
    int16_t cmp;
    register char *s = nameBuf;

    len              = 0;
    do {
        if (len != sizeof(nameBuf) - 1) {
            *s++ = ch;
            len++;
        }
        ch = (int8_t)getCh();
    } while (Isalnum(ch));
    ungetCh = ch;
    *s      = 0;
    lo      = T_ASM;
    hi      = T_WHILE;
    do {
        mid = (lo + hi) / 2;
        cmp = (int16_t)strcmp(nameBuf, keywords[mid - T_ASM]);
        if (cmp <= 0)
            hi = mid - 1;
        if (cmp >= 0)
            lo = mid + 1;
    } while (hi >= lo);
    if (hi < lo - 1) {
        switch (mid) {
        case T_AUTO:
        case T_EXTERN:
        case T_REGISTER:
        case T_STATIC:
        case T_TYPEDEF:
            yylval.yVal = mid;
            return S_CLASS;
        case T_CHAR:
        case T_DOUBLE:
        case T_ENUM:
        case T_FLOAT:
        case T_INT:
        case T_LONG:
        case T_SHORT:
        case T_STRUCT:
        case T_UNION:
        case T_UNSIGNED:
        case T_VOID:
            yylval.yVal = mid;
            return S_TYPE;
        case _T_SIZEOF:
            return T_SIZEOF;
        }
        return mid;
    }
    yylval.ySym = sub_4e90(nameBuf);
    return T_ID;
}

/**************************************************
 * 59: 308B PMO +++
 **************************************************/
void parseAsm(void) {
    int16_t ch;
    char buf[512];
    register char *s;

    for (;;) {
        s = buf;
        while ((ch = getCh()) != '\n' && ch != EOF)
            *s++ = (char)ch;
        *s = 0;
        if (ch == EOF)
            fatalErr("EOF in #asm");
        if (strncmp(buf, "#endasm", 7) == 0)
            return;
        printf(";; %s\n", buf);
    }
}

/**************************************************
 * 60: 310B PMO +++
 * one basic block relocated. Code equivalent
 **************************************************/
void parseString(int16_t ch) {
    char *var2;
    char *var4;
    char buf[1024];
    register char *s = buf;

    while (ch == '"') {
        while ((ch = getCh()) != '"') {
            if (ch == '\n') {
                expectErr("closing quote");
                break;
            }
            if (ch == '\\') {
                if ((ch = getCh()) != '\n')
                    *s++ = escCh(ch);
            } else
                *s++ = (char)ch;
        }
        ch = skipWs();
    }
    ungetCh  = ch;
    *s       = 0;
    strChCnt = (int16_t)(s - buf);
    var2 = var4 = xalloc(strChCnt + 1);
    ch          = strChCnt + 1; /* unwound memcpy. Note strcpy cannot handle embedded '\0' */
    s           = buf;
    while (ch--)
        *var2++ = *s++;
    yylval.yStr = var4;
}

/**************************************************
 * 61: 320D PMO +++
 * move of 2 basic blocks code equivalent
 **************************************************/
int16_t getCh(void) {
    int16_t ch;
#if !defined(CPM) && !defined(_WIN32)
    do {
#endif
        if (ungetCh) {
            ch      = ungetCh;
            ungetCh = 0;
        } else if ((ch = inBuf[inCnt++]) == 0) {
            if (s_opt)
                emitSrcInfo();
            sInfoEmitted = false;
            lInfoEmitted = false;

            if (!fgets(inBuf, /* 512 */, stdin))
                return EOF;
            ch          = inBuf[0];
            inCnt       = 1;
            startTokCnt = 0;
            lineNo++;
            if (l_opt)
                prErrMsg();
        }
#if !defined(CPM) && !defined(_WIN32)
        if (ch == 0x1a)
            return EOF;
    } while (ch != '\r');
#endif
    return ch;
}

/**************************************************
 * 62: 329A PMO +++
 **************************************************/
void prErrMsg(void) {
    register char *iy;
    if (!lInfoEmitted) {
        iy = depth ? curFuncNode->nVName : "";

        if (!l_opt && (strcmp(srcFile, lastEmitSrc) || strcmp(iy, lastEmitFunc))) {
            fprintf(stderr, "%s:", srcFile);
            if (*iy)
                fprintf(stderr, " %s()\n", iy);
            else
                fputc('\n', stderr);
            strcpy(lastEmitSrc, srcFile);
            strcpy(lastEmitFunc, iy);
        }
        fprintf(stderr, "%6d:\t%s", lineNo, inBuf);
        lInfoEmitted = true;
    }
}

/**************************************************
 * 63: 3350 PMO +++
 **************************************************/
void prMsgAt(register char *buf) {
    int16_t i;
    uint16_t col;
    prErrMsg();
    if (!*inBuf)
        fputs(buf, stderr);
    else {
        fputc('\t', stderr);
        for (col = i = 0; i < startTokCnt - 1; i++)
            if (inBuf[i] == '\t')
                col = (col + 8) & 0xfff8;
            else
                col++;
        if (strlen(buf) + 1 < col)
            fprintf(stderr, "%*s ^ ", col - 1, buf);
        else
            fprintf(stderr, "%*c %s", col + 1, '^', buf);
    }
}

/**************************************************
 * 64: 3429 PMO +++
 **************************************************/
void emitSrcInfo(void) {
    register char *s;

    if (!sInfoEmitted && inBuf[0]) {
        for (s = inBuf; *s && Isspace(*s); s++)
            ;
        if (*s && *s != '#')
            printf(";; ;%s: %d: %s", srcFile, lineNo, inBuf);
    }
    sInfoEmitted = true;
}

/**************************************************
 * 65: 347A PMO +++
 * equivalent code
 * uses ex de,hl ld de,xxx, cf. ld hl,xxx ex de,hl
 **************************************************/
int16_t skipWs(void) {
    int16_t ch;
    while (Isspace(ch = getCh()))
        ;
    return ch;
}

/**************************************************
 * 66: 3495 PMO +++
 * basic block move and different equivalent code
 * and optimisations
 **************************************************/
int8_t escCh(int16_t ch) {
    int16_t val;
    int8_t cnt;
    if (Isdigit(ch)) {
        val = ch - '0';
        ch  = getCh();
        if (Isdigit(ch)) {
            val = val * 8 + ch - '0';
            if (Isdigit(ch = getCh()))
                val = val * 8 + ch - '0';
            else
                ungetCh = ch;
        } else
            ungetCh = ch;
        return (int8_t)val;
    }
    switch (ch) {
    case 'n':
        return '\n';
    case 'b':
        return '\b';
    case 'r':
        return '\r';
    case 't':
        return '\t';
    case 'f':
        return '\f';
    case 'a':
        return '\a';
    case 'v':
        return '\v';
    case 'x':
        val = 0;
        cnt = 3;
        do {
            ch = getCh();
            if (!Isxdigit(ch)) {
                ungetCh = ch;
                return (int8_t)val;
            }
            val *= 16;
            if (Isupper(ch))
                ch |= 0x20;
            if (Islower(ch))
                val += ch - 'a' + 10;
            else
                val += ch - '0';
        } while (cnt--);
        return (int8_t)val;
    }
    return (int8_t)ch;
}

/**************************************************
 * 67: 35F7 PMO +++
 **************************************************/
int16_t peekCh(void) {
    int16_t ch;
    ungetCh = ch = skipWs();
    return ch;
}

/**************************************************
 * 68: 3610 PMO +++
 **************************************************/
void skipStmt(uint8_t tok) {

    while (tok > T_RBRACE)
        tok = yylex();
    if (tok == T_EOF)
        fatalErr("unexpected EOF");
    ungetTok = tok;
}

/**************************************************
 * 69: 363F PMO +++
 * uint8_t parameter
 **************************************************/
void expect(uint8_t etok, char *msg) {
    uint8_t tok;

    if ((tok = yylex()) == etok)
        return;
    expectErr(msg);
    skipStmt(tok);
}

/**************************************************
 * 70: 3666 PMO +++
 **************************************************/
void skipToSemi(void) {
    uint8_t tok;

    do {
        tok = yylex();
    } while (tok != T_SEMI);
    ungetTok = T_SEMI;
}
