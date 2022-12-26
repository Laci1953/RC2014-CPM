/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

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

sym_t *sub_4e90(register char *buf);
void expectErr(char *p);
void prError(int, ...);
void fatalErr(int, ...);

/**************************************************
 * 56: 2671 PMO +++
 * location of two basic blocks swapped, code equivalent
 **************************************************/
uint8_t tmp_yylex(void)
{
    int16_t ch;
    char buf[50];
    register char *s;

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
            } while (Isspace(ch) && (char)ch != (char)'\n');
            if (Isdigit(ch) && parseNumber(ch) == T_ICONST) {
                lineNo = (int16_t)(yylval.yNum - 1);
                do {
                    ch = getCh();
                } while (Isspace(ch) && (char)ch != (char)'\n');
                if (ch == '"') {
                    for (s = buf; (char)(ch = getCh()) != (char)'"' && (char)ch != (char)'\n';)
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
                } while ((char)ch != (char)'\n' && !Isspace(ch));
                *s = '\0';
                while ((char)ch != (char)'\n')
                    ch = getCh();
                if (strcmp(buf, "asm") == 0) {
                    parseAsm();
                    break;
                } else
                    fatalErr(96);
            }
            /* FALLTHRU */
        case '"':
            parseString('"');
            return T_SCONST;
        case '\'':
            ch          = getCh();
            yylval.yNum = ((char)ch == (char)'\\') ? escCh(getCh()) : ch;
            ch          = getCh();
            if ((char)ch == (char)'\n')
                expectErr("closing quote");
            else if ((char)ch != (char)'\'')
                /* prError("char const too long"); */
		prError(54);

            while ((char)ch != (char)'\n' && (char)ch != (char)'\'')
                ch = getCh();
            return T_ICONST;
        case ';':
            return T_SEMI;
        case ':':
            return T_COLON;
        case '+':
            ch = getCh();
            if ((char)ch == (char)'+')
                return T_INC;
            ungetCh = ch;
            ch      = skipWs();
            if ((char)ch == (char)'=')
                return T_PLUSEQ;
            ungetCh = ch;
            return T_PLUS;
        case '-':
            ch = getCh();
            if ((char)ch == (char)'-')
                return T_DEC;
            if ((char)ch == (char)'>')
                return T_POINTER;
            ungetCh = ch;
            ch      = skipWs();
            if ((char)ch == (char)'=')
                return T_MINUSEQ;
            ungetCh = ch;
            return T_MINUS;
        case '*':
            ch = skipWs();
            if ((char)ch == (char)'=')
                return T_MULEQ;
            ungetCh = ch;
            return T_STAR; /* deref or multiply */
        case '/':
            ch = skipWs();
            if ((char)ch == (char)'=')
                return T_DIVEQ;
            ungetCh = ch;
            return T_DIV;
        case '%':
            ch = skipWs();
            if ((char)ch == (char)'=')
                return T_MODEQ;
            ungetCh = ch;
            return T_MOD;
        case '&':
            ch = getCh();
            if ((char)ch == (char)'&')
                return T_LAND;
            ungetCh = ch;
            ch      = skipWs();
            if ((char)ch == (char)'=')
                return T_ANDEQ;
            ungetCh = ch;
            return T_BAND;
        case '|':
            ch = getCh();
            if ((char)ch == (char)'|')
                return T_LOR;
            ungetCh = ch;
            ch      = skipWs();
            if ((char)ch == (char)'=')
                return T_OREQ;
            ungetCh = ch;
            return T_BOR;
        case '^':
            ch = skipWs();
            if ((char)ch == (char)'=')
                return T_XOREQ;
            ungetCh = ch;
            return T_XOR;
        case '<':
            ch = getCh();
            if ((char)ch == (char)'<') {
                ch = skipWs();
                if ((char)ch == (char)'=')
                    return T_SHLEQ;
                else {
                    ungetCh = ch;
                    return T_SHL;
                }
            } else if ((char)ch == (char)'=')
                return T_LE;
            ungetCh = ch;
            return T_LT;
        case '>':
            ch = getCh();
            if ((char)ch == (char)'>') {
                ch = skipWs();
                if ((char)ch == (char)'=')
                    return T_SHREQ;
                else {
                    ungetCh = ch;
                    return T_SHR;
                }
            } else if ((char)ch == (char)'=')
                return T_GE;
            ungetCh = ch;
            return T_GT;
        case '=':
            ch = getCh();
            if ((char)ch == (char)'=')
                return T_EQEQ;
            ungetCh = ch;
            return T_EQ;
        case '!':
            ch = getCh();
            if ((char)ch == (char)'=')
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
            if ((char)ch == (char)'.') {
                ch = getCh();
                if ((char)ch != (char)'.')
		    prError(53);
                return T_3DOT;
            }
            ungetCh = ch;
            return T_DOT;

        case '?':
            return T_QUEST;
        case ',':
            return T_COMMA;
        default:
	    buf[0] = ch;
	    buf[1] = 0;
	    prError(155, buf);
            break;
        }
    }
}

uint8_t yylex(void) 
{
    uint8_t tok;

    if (ungetTok) {
        tok      = ungetTok;
        ungetTok = 0;
        if (tok == T_ID && byte_8f86)
            yylval.ySym = sub_4e90(nameBuf);
        return tok;
    }

    return tmp_yylex();
}

