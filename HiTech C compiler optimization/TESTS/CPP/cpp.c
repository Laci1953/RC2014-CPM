/* @(#)cpp.c	1.54 21/05/30 2010-2021 J. Schilling */
/* modified to use standard C headers and stripped out
   non prototype, gcos, scw2 and cpp usage for non CP/M target 6-Jul-2022 Mark Ogden */
#include "cpp.h"
#ifndef lint
static UConst char sccsid[] = "@(#)cpp.c	1.54 21/05/30 2010-2021 J. Schilling";
#endif
/*
 * C command
 * written by John F. Reiser
 * July/August 1978
 */
/*
 * This implementation is based on the UNIX 32V release from 1978
 * with permission from Caldera Inc.
 *
 * Copyright (c) 2010-2021 J. Schilling
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTOR(S) ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTOR(S) BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * Copyright(C) Caldera International Inc. 2001-2002. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code and documentation must retain the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:  This product includes
 *    software developed or owned by Caldera International, Inc.
 *
 * 4. Neither the name of Caldera International, Inc. nor the names of other
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * USE OF THE SOFTWARE PROVIDED FOR UNDER THIS LICENSE BY CALDERA
 * INTERNATIONAL, INC.  AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CALDERA INTERNATIONAL, INC. BE LIABLE FOR
 * ANY DIRECT, INDIRECT INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include <fcntl.h>
#include <io.h>
#else
#ifdef CPM
#include <unixio.h>
#define O_RDONLY 0
#else
#include <fcntl.h>
#include <unistd.h>
#endif
#endif

#include "version.h"

#define STATIC static

#ifdef SMALL
#define SYMLEN 31
#else
#define SYMLEN 128
#endif

#define SALT '#'
#ifndef BUFSIZ
#define BUFSIZ 512
#endif

STATIC char *pbeg;
STATIC char *pbuf;
STATIC char *pend;
char *outptr, *inptr;
char *newp;
STATIC char cinit;

/* some code depends on whether characters are sign or zero extended */
/*	#if '\377' < 0		not used here, old cpp doesn't understand */
#if pdp11 | vax | '\377' < 0
#define COFF 128
#else
#define COFF 0
#endif

#define ALFSIZ 256 /* alphabet size */
STATIC char macbit[ALFSIZ + 11];
STATIC char toktyp[ALFSIZ];
#define BLANK 1 /* white space (" \t\v\f\r") */
#define IDENT 2 /* valid char for identifier names */
#define NUMBR 3 /* chars is of "0123456789." */

/*
 * a superimposed code is used to reduce the number of calls to the
 * symbol table lookup routine.  (if the kth character of an identifier
 * is 'a' and there are no macro names whose kth character is 'a'
 * then the identifier cannot be a macro name, hence there is no need
 * to look in the symbol table.)  'scw1' enables the test based on
 * single characters and their position in the identifier.
 */
#define scw1  1

#if scw1
#define b0 1
#define b1 2
#define b2 4
#define b3 8
#define b4 16
#define b5 32
#define b6 64
#define b7 128
#endif

#define IB 1
#define SB 2
#define NB 4
#define CB 8
#define QB 16
#define WB 32
char fastab[ALFSIZ];
STATIC char slotab[ALFSIZ];
STATIC char *ptrtab;

/*
 * Cast the array index to int in order to avoid GCCs warnings:
 * warning: subscript has type `char'
 */
#define isslo     (ptrtab == (slotab + COFF))
#define isid(a)   ((fastab + COFF)[(int)a] & IB)
#define isspc(a)  (ptrtab[(int)a] & SB)
#define isnum(a)  ((fastab + COFF)[(int)a] & NB)
#define iscom(a)  ((fastab + COFF)[(int)a] & CB)
#define isquo(a)  ((fastab + COFF)[(int)a] & QB)
#define iswarn(a) ((fastab + COFF)[(int)a] & WB)

#define eob(a)    ((a) >= pend)
#define bob(a)    (pbeg >= (a))

#ifdef SMALL
#define BUFFERSIZ 512
#else
#define BUFFERSIZ 8192
#endif
STATIC char buffer[SYMLEN + BUFFERSIZ + BUFFERSIZ + SYMLEN];

/*
 * SBSIZE was static 12000 chars in 1978, we now have a method to
 * malloc more space.
 */
#ifdef SMALL
#define SBSIZE 12000
STATIC char sbf[SBSIZE];
#else
#define SBSIZE 65536
STATIC char *sbf;
#endif
#ifdef SMALL
STATIC char *savch = sbf;
#else
STATIC char *savch;
#endif

#define DROP     0xFE /* special character not legal ASCII or EBCDIC */
#define WARN     DROP
#define SAME     0
#define MAXINC   10 /* max include nesting depth */
#define MAXIDIRS 10 /* max # of -I directories */
#define MAXFRE   14 /* max buffers of macro pushback */
#define MAXFRM   31 /* max number of formals/actuals to a macro */

#define MAXMULT  (INT_MAX / 10)
#define MAXVAL   (INT_MAX)

static char warnc = (char)WARN;

STATIC int mactop;
STATIC int fretop;
STATIC char *instack[MAXFRE];
STATIC char *bufstack[MAXFRE];
STATIC char *endbuf[MAXFRE];

STATIC int plvl;      /* parenthesis level during scan for macro actuals */
STATIC int maclin;    /* line number of macro call requiring actuals */
STATIC char *macfil;  /* file name of macro call requiring actuals */
STATIC char *macnam;  /* name of macro requiring actuals */
STATIC int maclvl;    /* # calls since last decrease in nesting level */
STATIC char *macforw; /* pointer which must be exceeded to decrease nesting level */
STATIC int macdam;    /* offset to macforw due to buffer shifting */

#if tgp
int tgpscan; /* flag for dump(); */
#endif

STATIC int inctop[MAXINC];
STATIC char *fnames[MAXINC];
STATIC char *dirnams[MAXINC]; /* actual directory of #include files */
STATIC FILE *fins[MAXINC];
STATIC int lineno[MAXINC];
#ifndef SMALL
STATIC char *input;
STATIC char *target;
#endif

/*
 * We need:
 *	"" include dir as dirs[0] +
 *	MAXIDIRS +
 *	system default include dir +
 *	a NULL pointer at the end
 */
STATIC char *dirs[MAXIDIRS + 3]; /* -I and <> directories */
STATIC FILE *fin;                /* will be initially set to stdin */
STATIC FILE *fout;               /* Init in main(), Mac OS is nonPOSIX */
#ifndef SMALL
STATIC FILE *mout; /* Output for -M */
#endif
STATIC int nd = 1;
#ifndef SMALL
STATIC int extrawarn; /* warn on extra tokens */
#endif
STATIC int pflag;   /* don't put out lines "# 12 foo.c" */
STATIC int passcom; /* don't delete comments */
STATIC int rflag;   /* allow macro recursion */
#ifndef SMALL
STATIC int hflag; /* Print included filenames */
STATIC int mflag; /* Generate make dependencies */
#endif
#ifndef CPM
STATIC int noinclude; /* -noinclude /usr/include */
#endif
#ifndef SMALL
STATIC int nopredef; /* -undef all */
#endif
STATIC int ifno;
#define NPREDEF 20
STATIC char *prespc[NPREDEF];
STATIC char **predef = prespc;
STATIC char *punspc[NPREDEF];
STATIC char **prund = punspc;
STATIC int exfail;
STATIC struct symtab *lastsym;

LOCAL void sayline();
LOCAL void dump(void);
LOCAL char *refill(char *);
LOCAL char *cotoken(char *);
EXPORT char *skipbl(char *);
LOCAL char *unfill(char *);
LOCAL char *doincl(char *);
LOCAL int equfrm(char *, char *, char *);
LOCAL char *dodef(char *);
LOCAL void control(char *);
#ifndef SMALL
LOCAL int iscomment(char *);
#endif
LOCAL struct symtab *stsym(char *);
LOCAL struct symtab *ppsym(char *);
EXPORT void pperror(char *fmt, ...);
EXPORT void yyerror(char *fmt, ...);
LOCAL void ppwarn(char *fmt, ...);
EXPORT struct symtab *lookup(char *, int);
LOCAL struct symtab *slookup(char *, char *, int);
LOCAL char *subst(char *, struct symtab *);
LOCAL char *trmdir(char *);
LOCAL char *copy(char *);
#define strdex strchr
EXPORT int yywrap(void);
EXPORT int main(int argc, char **argav);
#ifndef SMALL
LOCAL void newsbf(void);
LOCAL struct symtab *newsym(void);
LOCAL void usage(void);
#endif

#define symsiz  500
#define SYMINCR 340 /* Number of symtab entries to allocate as a block */
#ifdef SMALL
STATIC struct symtab stab[symsiz]; /* limited size, allocate symbols statically avoids ptr */
#else
STATIC struct symtab *stab[symsiz]; /* newer version allocates lists */
#endif

STATIC struct symtab *defloc;
STATIC int defining;
STATIC struct symtab *udfloc;
STATIC struct symtab *incloc;
STATIC struct symtab *ifloc;
STATIC struct symtab *elsloc;
STATIC struct symtab *eifloc;
STATIC struct symtab *elifloc;
STATIC struct symtab *ifdloc;
STATIC struct symtab *ifnloc;
STATIC struct symtab *asmloc;
STATIC struct symtab *varloc;
STATIC struct symtab *lneloc;
STATIC struct symtab *ulnloc;
STATIC struct symtab *easloc;
STATIC struct symtab *uflloc;

STATIC struct symtab *pragmaloc;
STATIC struct symtab *errorloc;
STATIC int trulvl;
int flslvl;
STATIC int elflvl;
STATIC int elslvl;

#if !defined(CPM) && !defined(_WIN32)
#define fread(ptr, size, count, stream) afread(ptr, count, stream)
/* simplified as size is always one */
size_t afread(void *ptr, size_t count, FILE *stream) {
    int c =0;
    int cnt = 0;
    while (cnt < count && (c = getc(stream)) != EOF && c != 0x1a)
        if (c != '\r')
            ((char *)ptr)[cnt++] = c;
    if (c == 0x1a) /* for next time */
        ungetc(c, stream);
    return cnt;
    
}
#endif

/* ARGSUSED */
STATIC void sayline() {
    if (pflag == 0)
        fprintf(fout, "# %d \"%s\"\n", lineno[ifno], fnames[ifno]);
}

/*
 * data structure guide
 *
 * most of the scanning takes place in the buffer:
 *
 *  (low address)                                             (high address)
 *  pbeg                           pbuf                                 pend
 *  |      <-- BUFFERSIZ chars -->   |         <-- BUFFERSIZ chars -->     |
 *  _______________________________________________________________________
 * |_______________________________________________________________________|
 *          |               |               |
 *          |<-- waiting -->|               |<-- waiting -->
 *          |    to be      |<-- current -->|    to be
 *          |    written    |    token      |    scanned
 *          |               |               |
 *          outptr          inptr           p
 *
 *  *outptr   first char not yet written to outptrut file
 *  *inptr    first char of current token
 *  *p      first char not yet scanned
 *
 * macro expansion: write from *outptr to *inptr (chars waiting to be written),
 * ignore from *inptr to *p (chars of the macro call), place generated
 * characters in front of *p (in reverse order), update pointers,
 * resume scanning.
 *
 * symbol table pointers point to just beyond the end of macro definitions;
 * the first preceding character is the number of formal parameters.
 * the appearance of a formal in the body of a definition is marked by
 * 2 chars: the char WARN, and a char containing the parameter number.
 * the first char of a definition is preceded by a zero character.
 *
 * when macro expansion attempts to back up over the beginning of the
 * buffer, some characters preceding *pend are saved in a side buffer,
 * the address of the side buffer is put on 'instack', and the rest
 * of the main buffer is moved to the right.  the end of the saved buffer
 * is kept in 'endbuf' since there may be nulls in the saved buffer.
 *
 * similar action is taken when an 'include' statement is processed,
 * except that the main buffer must be completely emptied.  the array
 * element 'inctop[ifno]' records the last side buffer saved when
 * file 'ifno' was included.  these buffers remain dormant while
 * the file is being read, and are reactivated at end-of-file.
 *
 * instack[0 : mactop] holds the addresses of all pending side buffers.
 * instack[inctop[ifno]+1 : mactop-1] holds the addresses of the side
 * buffers which are "live"; the side buffers instack[0 : inctop[ifno]]
 * are dormant, waiting for end-of-file on the current file.
 *
 * space for side buffers is obtained from 'savch' and is never returned.
 * bufstack[0:fretop-1] holds addresses of side buffers which
 * are available for use.
 */

STATIC void dump() {
    /*
     * stripped down version
     */
    register char *p1;
    register FILE *f;
    if ((p1 = outptr) == inptr || flslvl != 0)
        return;
    f = fout;
    while (p1 < inptr)
        fputc(*p1++, f);
    outptr = p1;
}

STATIC char *refill(register char *p) {
    /*
     * dump buffer.  save chars from inptr to p.  read into buffer at pbuf,
     * contiguous with p.  update pointers, return new p.
     */
    register char *np, *op;
    register int ninbuf;
    dump();
    np = pbuf - (p - inptr);
    op = inptr;
    if (bob(np + 1)) {
        pperror("token too long");
        np = pbeg;
        p  = inptr + BUFFERSIZ;
    }
    macdam += np - inptr;
    outptr = inptr = np;
    while (op < p)
        *np++ = *op++;
    p = np;
    for (;;) {
        if (mactop > inctop[ifno]) { /* retrieve hunk of pushed-back macro text */
            op = instack[--mactop];
            np = pbuf;
            do {
                while ((*np++ = *op++) != '\0')
                    ;
            } while (op < endbuf[mactop]);
            pend = np - 1;
            /* make buffer space avail for 'include' processing */
            if (fretop < MAXFRE)
                bufstack[fretop++] = instack[mactop];
            return (p);
        } else { /* get more text from file(s) */
            maclvl = 0;
            if (0 < (ninbuf = fread(pbuf, 1, BUFFERSIZ, fin))) {
                pend  = pbuf + ninbuf;
                *pend = '\0';
                return (p);
            }
            /* end of #include file */
            if (ifno == 0) { /* end of input */
                if (plvl != 0) {
                    int n = plvl, tlin = lineno[ifno];
                    char *tfil   = fnames[ifno];
                    lineno[ifno] = maclin;
                    fnames[ifno] = macfil;
                    pperror("%s: unterminated macro call", macnam);
                    lineno[ifno] = tlin;
                    fnames[ifno] = tfil;
                    np           = p;
                    *np++        = '\n'; /* shut off unterminated quoted string */
                    while (--n >= 0)
                        *np++ = ')'; /* supply missing parens */
                    pend = np;
                    *np  = '\0';
                    if (plvl < 0)
                        plvl = 0;
                    return (p);
                }
                inptr = p;
                dump();
                exit(exfail);
            }
            fclose(fin);
            fin     = fins[--ifno];
            dirs[0] = dirnams[ifno];
            sayline();
        }
    }
}

#define BEG 0
#define LF  1

STATIC char *cotoken(register char *p) {
    register int c, i;
    char quoc;
    int cppcom       = 0;
    static int state = BEG;

    if (state != BEG)
        goto prevlf;
    for (;;) {
    again:
        while (!isspc(*p++))
            ;
        switch (*(inptr = p - 1)) {
        case 0:
            {
                if (eob(--p)) {
                    p = refill(p);
                    goto again;
                } else
                    ++p; /* ignore null byte */
            }
            break;
        case '|':
        case '&':
            for (;;) { /* sloscan only */
                if (*p++ == *inptr)
                    break;
                if (eob(--p))
                    p = refill(p);
                else
                    break;
            }
            break;
        case '=':
        case '!':
            for (;;) { /* sloscan only */
                if (*p++ == '=')
                    break;
                if (eob(--p))
                    p = refill(p);
                else
                    break;
            }
            break;
        case '<':
        case '>':
            for (;;) { /* sloscan only */
                if (*p++ == '=' || p[-2] == p[-1])
                    break;
                if (eob(--p))
                    p = refill(p);
                else
                    break;
            }
            break;
        case '\\':
            for (;;) {
                if (*p++ == '\n') {
                    ++lineno[ifno];
                    break;
                }
                if (eob(--p))
                    p = refill(p);
                else {
                    ++p;
                    break;
                }
            }
            break;
        case '/':
            for (;;) {
                if (*p == '*' || *p == '/') { /* comment */
                    if (*p++ == '/')          /* A new style comment */
                        cppcom++;
                    if (defining || !passcom) {
                        for (inptr = p - 2; (toktyp + COFF)[inptr[-1]] == BLANK && inptr !=  outptr; inptr--)
                            ;
                        dump();
                        ++flslvl;
                    }
                    for (;;) {
                        while (!iscom(*p++))
                            ;
                        if (cppcom && p[-1] == '\n') {
                            p--;
                            goto endcom;
                        }
                        if (p[-1] == '*')
                            for (;;) {
                                if (*p++ == '/')
                                    goto endcom;
                                if (eob(--p)) {
                                    if (defining || !passcom) {
                                        inptr = p;
                                        p     = refill(p);
                                    } else if ((p - inptr) >= BUFFERSIZ) { /* split long comment */
                                        inptr = p;
                                        p     = refill(p); /* last char written is '*' */
                                        fputc('/', fout);  /* terminate first part */
                                        /* and fake start of 2nd */
                                        outptr = inptr = p -= 3;
                                        *p++           = '/';
                                        *p++           = '*';
                                        *p++           = '*';
                                    } else
                                        p = refill(p);
                                } else
                                    break;
                            }
                        else if (p[-1] == '\n') {
                            ++lineno[ifno];
                            if (defining || !passcom)
                                fputc('\n', fout);
                        } else if (eob(--p)) {
                            if (!passcom) {
                                inptr = p;
                                p     = refill(p);
                            } else if ((p - inptr) >= BUFFERSIZ) { /* split long comment */
                                inptr = p;
                                p     = refill(p);
                                fputc('*', fout);
                                fputc('/', fout);
                                outptr = inptr = p -= 2;
                                *p++           = '/';
                                *p++           = '*';
                            } else
                                p = refill(p);
                        } else
                            ++p; /* ignore null byte */
                    }
                endcom:
                    cppcom = 0;
                    if (defining || !passcom) {
                        outptr = inptr = p;
                        --flslvl;
                        goto again;
                    }
                    break;
                } else { /* no comment, skip */
                    p++;
                }
                if (eob(--p))
                    p = refill(p);
                else
                    break;
            }
            break;
        case '"':
        case '\'':
            {
                quoc = p[-1];
                for (;;) {
                    while (!isquo(*p++))
                        ;
                    if (p[-1] == quoc)
                        break;
                    if (p[-1] == '\n') {
                        --p;
                        break;
                    } /* bare \n terminates quotation */
                    if (p[-1] == '\\')
                        for (;;) {
                            if (*p++ == '\n') {
                                ++lineno[ifno];
                                break;
                            } /* escaped \n ignored */
                            if (eob(--p))
                                p = refill(p);
                            else {
                                ++p;
                                break;
                            }
                        }
                    else if (eob(--p))
                        p = refill(p);
                    else
                        ++p; /* it was a different quote character */
                }
            }
            break;
        case '\n':
            {
                ++lineno[ifno];
                if (isslo) {
                    state = LF;
                    return (p);
                }
            prevlf:
                state = BEG;
                for (;;) {
                    if (*p++ == '#')
                        return (p);
                    if (eob(inptr = --p))
                        p = refill(p);
                    else
                        goto again;
                }
            }
            /* NOTREACHED */
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            for (;;) {
                while (isnum(*p++))
                    ;
                if (eob(--p))
                    p = refill(p);
                else
                    break;
            }
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case '_':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':

#define tmac1(c, bit)                                                                              \
    if (!xmac1(c, bit, &))                                                                         \
    goto nomac
#define xmac1(c, bit, op) ((macbit + COFF)[c] op(bit))

            if (flslvl)
                goto nomac;
            for (;;) {
                c = p[-1];
                tmac1(c, b0);
                i = *p++;
                if (!isid(i))
                    goto endid;
                tmac1(i, b1);
                c = *p++;
                if (!isid(c))
                    goto endid;
                tmac1(c, b2);
                i = *p++;
                if (!isid(i))
                    goto endid;
                tmac1(i, b3);
                c = *p++;
                if (!isid(c))
                    goto endid;
                tmac1(c, b4);
                i = *p++;
                if (!isid(i))
                    goto endid;
                tmac1(i, b5);
                c = *p++;
                if (!isid(c))
                    goto endid;
                tmac1(c, b6);
                i = *p++;
                if (!isid(i))
                    goto endid;
                tmac1(i, b7);
                while (isid(*p++))
                    ;
                if (eob(--p)) {
                    refill(p);
                    p = inptr + 1;
                    continue;
                }
                goto lokid;
            endid:
                if (eob(--p)) {
                    refill(p);
                    p = inptr + 1;
                    continue;
                }
            lokid:
                slookup(inptr, p, 0);
                if (newp) {
                    p = newp;
                    goto again;
                } else
                    break;
            nomac:
                while (isid(*p++))
                    ;
                if (eob(--p)) {
                    p = refill(p);
                    goto nomac;
                } else
                    break;
            }
            break;
        } /* end of switch */

        if (isslo)
            return (p);
    } /* end of infinite loop */
}

char *skipbl(register char *p) { /* get next non-blank token */
    do {
        outptr = inptr = p;
        p              = cotoken(p);
    } while ((toktyp + COFF)[(int)*inptr] == BLANK);
    return (p);
}

STATIC char *unfill(register char *p) {
    /*
     * take <= BUFFERSIZ chars from right end of buffer and put them on instack .
     * slide rest of buffer to the right, update pointers, return new p.
     */
    register char *np, *op;
    register int d;
    if (mactop >= MAXFRE) {
        pperror("%s: too much pushback", macnam);
        p = inptr = pend;
        dump(); /* begin flushing pushback */
        while (mactop > inctop[ifno]) {
            p = refill(p);
            p = inptr = pend;
            dump();
        }
    }
    if (fretop > 0)
        np = bufstack[--fretop];
    else {
        np = savch;
        savch += BUFFERSIZ;
        if (savch >= sbf + SBSIZE) {
#ifdef SMALL
            pperror("no space");
            exit(exfail);
#else
            newsbf();
            np = savch;
            savch += BUFFERSIZ;
#endif
        }
        *savch++ = '\0';
    }
    instack[mactop] = np;
    op              = pend - BUFFERSIZ;
    if (op < p)
        op = p;
    for (;;) { /* out with old */
        while ((*np++ = *op++) != '\0')
            ;
        if (eob(op))
            break;
    }
    endbuf[mactop++] = np; /* mark end of saved text */
    np               = pbuf + BUFFERSIZ;
    op               = pend - BUFFERSIZ;
    pend             = np;
    if (op < p)
        op = p;
    while (outptr < op)
        *--np = *--op; /* slide over new */
    if (bob(np))
        pperror("token too long");
    d = np - outptr;
    outptr += d;
    inptr += d;
    macdam += d;
    return (p + d);
}

STATIC char *doincl(register char *p) {
    int filok, inctype;
    register char *cp;
    char **dirp, *nfil;
    char isFixedPath;
    char filname[BUFFERSIZ];

    filname[0] = '\0'; /* Make lint quiet */
    p          = skipbl(p);
    cp         = filname;
    if (*inptr++ == '<') { /* special <> syntax */
        inctype = 1;
        for (;;) {
            outptr = inptr = p;
            p              = cotoken(p);
            if (*inptr == '\n') {
                --p;
                *cp = '\0';
                break;
            }
            if (*inptr == '>') {
                *cp = '\0';
                break;
            }
            while (inptr < p)
                *cp++ = *inptr++;
        }
    } else if (inptr[-1] == '"') { /* regular "" syntax */
        inctype = 0;
        while (inptr < p)
            *cp++ = *inptr++;
        if (*--cp == '"')
            *cp = '\0';
    } else {
        pperror("bad include syntax", 0);
        inctype = 2;
    }
    /* flush current file to \n , then write \n */
    ++flslvl;
    do {
        outptr = inptr = p;
        p              = cotoken(p);
    } while (*inptr != '\n');
    --flslvl;
    inptr = p;
    dump();
    if (inctype == 2)
        return (p);
    /* look for included file */
    if (ifno + 1 >= MAXINC) {
        pperror("Unreasonable include nesting", 0);
        return (p);
    }
    if ((nfil = savch) > sbf + SBSIZE - BUFFERSIZ) {
#ifdef SMALL
        pperror("no space");
        exit(exfail);
#else
        newsbf();
        nfil = savch;
#endif
    }
    filok = 0;
    isFixedPath =
#ifdef DEVSEP
        isFixedPath = ISDIRSEP(filname[0]) || strchr(filname, DEVSEP);
#else
        isFixedPath = ISDIRSEP(filname[0]);
#endif
    for (dirp = dirs + inctype; *dirp; ++dirp) {
        if (isFixedPath || **dirp == '\0')
            strcpy(nfil, filname);
        else {
            strcpy(nfil, *dirp);
#if defined(__linux__) || defined(_WIN32) || defined(__APPLE__)
            strcat(nfil, "/");
#endif
            strcat(nfil, filname);
        }
#ifdef _WIN32
        for (char *s = nfil; s = strchr(s, '\\'); s++) /* normalise dir sep to / */
            *s = '/';
#endif
        if ((fins[ifno + 1] = fopen(nfil, "r"))) {
            filok = 1;
            fin   = fins[++ifno];
            break;
        }
        if (isFixedPath)
            break;
    }
    if (filok == 0) {
        pperror("Can't find include file %s", filname);
    } else {
        lineno[ifno] = 1;
        fnames[ifno] = cp = nfil;
        while (*cp++)
            ;
        savch         = cp;
        dirnams[ifno] = dirs[0] = trmdir(copy(nfil));
        sayline();
#ifndef SMALL
        if (hflag)
            fprintf(stderr, "%s\n", nfil);
        if (mflag)
            fprintf(mout, "%s: %s\n", input, fnames[ifno]);
#endif
        /* save current contents of buffer */
        while (!eob(p))
            p = unfill(p);
        inctop[ifno] = mactop;
    }
    return (p);
}

STATIC int equfrm(register char *a, register char *p1, register char *p2) {
    register char c;
    int flag;
    c    = *p2;
    *p2  = '\0';
    flag = strcmp(a, p1);
    *p2  = c;
    return (flag == SAME);
}

/* modified version of string compare with pre triming */
STATIC int xstrcmp(register char *s, char *t) {
    int cmp;
    char *snws; /* s last none ws if present */
    char *tnws; /* t last none ws if present */
    char slch;  /* s saved ws char */
    char tlch;  /* t saved ws char */
    char *p;
    while (*s && (*s == ' ' || *s == '\t')) /* PMO fixed first *s to compare to space */
        s++;
    snws = NULL;
    for (p = s; *p; p++)
        if (*p != ' ' && *p != '\t')
            snws = p;

    if (snws) { /* trim s */
        slch  = *++snws;
        *snws = 0;
    }
    while (*t && (*t == ' ' || *t == '\t')) /* PMO fixed first *t to compare to space */
        t++;
    tnws = NULL;
    for (p = t; *p; p++)
        if (*p != ' ' && *p != '\t')
            tnws = p;

    if (tnws) { /* trim t */
        tlch  = *++tnws;
        *tnws = 0;
    }
    cmp = strcmp(s, t);
    if (snws)
        *snws = slch;
    if (tnws)
        *tnws = tlch;
    return cmp;
}

STATIC char *dodef(char *p) { /* process '#define' */
    register char *pin, *psav, *cf;
    char **pf, **qf;
    int b, c, params;
    struct symtab *np;
    char quoc; /* moved here to avoid large offset */
    char *oldval, *oldsavch;
    char *formal[MAXFRM];    /* formal[n] is name of nth formal */
    char formtxt[BUFFERSIZ]; /* space for formal names */

    formtxt[0] = '\0'; /* Make lint quiet */

    if (savch > sbf + SBSIZE - BUFFERSIZ) {
#ifdef SMALL
        pperror("too much defining");
        return p;
#else
        newsbf();
#endif
    }
    oldsavch = savch; /* to reclaim space if redefinition */
    ++flslvl;         /* prevent macro expansion during 'define' */
    p   = skipbl(p);
    pin = inptr;
    if ((toktyp + COFF)[(int)*pin] != IDENT) {
        ppwarn("illegal macro name");
        while (*inptr != '\n')
            p = skipbl(p);
        return (p);
    }
    np = slookup(pin, p, 1);
    if ((oldval = np->value) != NULL)
        savch = oldsavch; /* was previously defined */
    b  = 1;
    cf = pin;
    while (cf < p) { /* update macbit */
        c = *cf++;
        xmac1(c, b, |=);
        b = (b + b) & 0xFF;
    }
    params = 0;
    outptr = inptr = p;
    p              = cotoken(p);
    pin            = inptr;
    formal[0]      = "";     /* Prepare for hack at next line... */
    pf             = formal; /* Make gcc/lint quiet, pf only used with params!=0 */
    if (*pin == '(') {       /* with parameters; identify the formals */
        cf = formtxt;
        pf = formal;
        for (;;) {
            p   = skipbl(p);
            pin = inptr;
            if (*pin == '\n') {
                --lineno[ifno];
                --p;
                pperror("%s: missing )", np->name);
                break;
            }
            if (*pin == ')')
                break;
            if (*pin == ',')
                continue;
            if ((toktyp + COFF)[(int)*pin] != IDENT) {
                c  = *p;
                *p = '\0';
                pperror("bad formal: %s", pin);
                *p = c;
            } else if (pf >= &formal[MAXFRM]) {
                c  = *p;
                *p = '\0';
                pperror("too many formals: %s", pin);
                *p = c;
            } else {
                *pf++ = cf;
                while (pin < p)
                    *cf++ = *pin++;
                *cf++ = '\0';
                ++params;
            }
        }
        if (params == 0)
            --params; /* #define foo() ... */
    } else if (*pin == '\n') {
        --lineno[ifno];
        --p;
    }
    /*
     * remember beginning of macro body, so that we can
     * warn if a redefinition is different from old value.
     */
    oldsavch = psav = savch;
    defining        = 1;
    for (;;) { /* accumulate definition until linefeed */
        outptr = inptr = p;
        p              = cotoken(p);
        pin            = inptr;
        if (*pin == '\\' && pin[1] == '\n')
            continue; /* ignore escaped lf */
        if (*pin == '\n')
            break;
        if (params) { /* mark the appearance of formals in the definiton */
            if ((toktyp + COFF)[(int)*pin] == IDENT) {
                for (qf = pf; --qf >= formal;) {
                    if (equfrm(*qf, pin, p)) {
                        *psav++ = qf - formal + 1;
                        *psav++ = WARN;
                        pin     = p;
                        break;
                    }
                }
            } else if (*pin == '"' || *pin == '\'') { /* inside quotation marks, too */
                quoc = *pin;
                for (*psav++ = *pin++; pin < p && *pin != quoc;) {
                    while (pin < p && !isid(*pin))
                        *psav++ = *pin++;
                    cf = pin;
                    while (cf < p && isid(*cf))
                        ++cf;
                    for (qf = pf; --qf >= formal;) {
                        if (equfrm(*qf, pin, cf)) {
                            *psav++ = qf - formal + 1;
                            *psav++ = WARN;
                            pin     = cf;
                            break;
                        }
                    }
                    while (pin < cf)
                        *psav++ = *pin++;
                }
            }
        }
        while (pin < p) {
            if (*pin == warnc) {
                *psav++ = WARN;
            }
            *psav++ = *pin++;
        }
    }
    *psav++  = params;
    *psav++  = '\0';
    defining = 0;
    if ((cf = oldval) != NULL) { /* redefinition */
        --cf;                    /* skip no. of params, which may be zero */
        while (*--cf)
            ;                               /* go back to the beginning */
        if (0 != xstrcmp(++cf, oldsavch)) { /* redefinition different from old */
            --lineno[ifno];
            ppwarn("%s redefined", np->name);
            ++lineno[ifno];
            np->value = psav - 1;
        } else
            psav = oldsavch; /* identical redef.; reclaim space */
    } else
        np->value = psav - 1;
    --flslvl;
    inptr = pin;
    savch = psav;
    return (p);
}

#define fasscan() ptrtab = fastab + COFF
#define sloscan() ptrtab = slotab + COFF

STATIC void control(register char *p) { /* find and handle preprocessor control lines */
    register struct symtab *np;
#ifndef SMALL
    int didwarn;
#endif
    for (;;) {
        fasscan();
        p = cotoken(p);
        if (*inptr == '\n')
            ++inptr;
        dump();
        sloscan();
        p        = skipbl(p);
        *--inptr = SALT;
        outptr   = inptr;
        ++flslvl;
        np = slookup(inptr, p, 0);
        --flslvl;
        if (np == defloc) { /* define */
            if (flslvl == 0) {
                p = dodef(p);
                continue;
            }
        } else if (np == incloc) { /* include */
            if (flslvl == 0) {
                p = doincl(p);
                continue;
            }
        } else if (np == ifnloc) { /* ifndef */
            ++flslvl;
            p  = skipbl(p);
            np = slookup(inptr, p, 0);
            --flslvl;
            if (flslvl == 0 && np->value == 0)
                ++trulvl;
            else
                ++flslvl;
        } else if (np == ifdloc) { /* ifdef */
            ++flslvl;
            p  = skipbl(p);
            np = slookup(inptr, p, 0);
            --flslvl;
            if (flslvl == 0 && np->value != 0)
                ++trulvl;
            else
                ++flslvl;
        } else if (np == eifloc) { /* endif */
            if (flslvl) {
                if (--flslvl == 0)
                    sayline();
            } else if (trulvl)
                --trulvl;
            else
                pperror("If-less endif", 0);

            if (flslvl == 0)
                elflvl = 0;
            elslvl = 0;
        } else if (np == elifloc) { /* elif */
            if (flslvl == 0)
                elflvl = trulvl;
            if (flslvl) {
                if (elflvl > trulvl) {
                    ;
                } else if (--flslvl != 0) {
                    ++flslvl;
                } else {
                    newp = p;
                    if (yyparse()) {
                        ++trulvl;
                        sayline();
                    } else {
                        ++flslvl;
                    }
                    p = newp;
                }
            } else if (trulvl) {
                ++flslvl;
                --trulvl;
            } else
                pperror("If-less elif");

        } else if (np == elsloc) { /* else */
            if (flslvl) {
                if (elflvl > trulvl)
                    ;
                else if (--flslvl != 0)
                    ++flslvl;
                else {
                    ++trulvl;
                    sayline();
                }
            } else if (trulvl) {
                ++flslvl;
                --trulvl;
            } else
                pperror("If-less else", 0);

            if (elslvl == trulvl + flslvl)
                pperror("Too many #else's");
            elslvl = trulvl + flslvl;

        } else if (np == udfloc) { /* undefine */
            if (flslvl == 0) {
                ++flslvl;
                p = skipbl(p);
                slookup(inptr, p, DROP);
                --flslvl;
            }
        } else if (np == ifloc) { /* if */
            newp = p;
            if (flslvl == 0 && yyparse())
                ++trulvl;
            else
                ++flslvl;
            p = newp;
        } else if (np == pragmaloc) { /* pragma */
            ppwarn("#pragma ignored");
            ++flslvl;

            while (*inptr != '\n') {
                /* skip line */
                outptr = inptr = p;
                p              = cotoken(p);
            }
            --flslvl;

        } else if (np == errorloc) { /* error */
            /*
             * Write a message to stderr and exit immediately, but only
             * if #error was in a conditional "active" part of the code.
             */
            if (flslvl == 0) {
                char ebuf[BUFFERSIZ];

                p = ebuf;
                while (*inptr != '\n') {
                    if (*inptr == '\0')
                        if (eob(--inptr)) {
                            inptr = refill(inptr);
                            continue;
                        }
                    *p++ = *inptr++;
                    if (p >= &ebuf[BUFFERSIZ - 1])
                        break;
                }
                *p = '\0';
                pperror(ebuf);
                exit(exfail);
            }
        } else if (np == lneloc) { /* line */
            if (flslvl == 0 && pflag == 0) {
#ifndef SMALL
                register char *s;
                register int n;
#endif

                outptr = inptr = p;
#ifndef SMALL
            was_line:
                /*
                 * Enforce the rest of the line to be in the buffer
                 */
                s = p;
                while (*s && *s != '\n')
                    s++;
                if (eob(s))
                    p = refill(s);

                s = inptr;
                while ((toktyp + COFF)[(int)*s] == BLANK)
                    s++;
                /*
                 * Now read the new line number...
                 */
                n = 0;
                while (isdigit(*s)) {
                    register int c;

                    if (n > MAXMULT) {
                        pperror("bad number for #line");
                        n = MAXVAL;
                        break;
                    }
                    n *= 10;
                    c = *s++ - '0';
                    if ((MAXVAL - n) < c) {
                        pperror("bad number for #line");
                        n = MAXVAL;
                        break;
                    }
                    n += c;
                }
                if (n == 0)
                    pperror("bad number for #line");
                else
                    lineno[ifno] = n - 1;

                while ((toktyp + COFF)[(int)*s] == BLANK)
                    s++;

                /*
                 * In case there is an optional file name...
                 */
                if (*s != '\n') {
                    register char *f;

                    f = s;
                    while (*f && *f != '\n')
                        f++;

                    if (savch >= sbf + SBSIZE - (f - s)) {
                        newsbf();
                    }
                    f = savch;
                    if (*s != '"')
                        *f++ = *s;
                    s++;
                    while (*s) {
                        if (*s == '"' || *s == '\n')
                            break;
                        *f++ = *s++;
                    }
                    *f++ = '\0';
                    if (strcmp(savch, fnames[ifno])) {
                        fnames[ifno] = savch;
                        savch        = f;
                    }
                }
#endif
                /*
                 * Finally outout the rest of the directive.
                 */
                *--outptr = '#';
                while (*inptr != '\n')
                    p = cotoken(p);
                continue;
            }
        } else if (np == asmloc || np == easloc) {
            while (*inptr != '\n')
                p = cotoken(p);
        } else if (*++inptr == '\n')
            outptr = inptr; /* allows blank line after # */
#ifndef SMALL
        else if (isdigit(*inptr)) { /* allow cpp output "#4711" */
            /*
             * Step back before this token in case it was a number.
             */
            outptr = p = inptr;
            goto was_line;
        }
#endif
        else
            pperror("undefined control", 0);
            /* flush to lf */
#ifndef SMALL
        didwarn = 0;
#endif
        ++flslvl;
        while (*inptr != '\n') {
            outptr = inptr = p;
            p              = cotoken(p);
#ifndef SMALL
            if (extrawarn && !didwarn && *inptr != '\n' && *p != '\n' &&
                (toktyp + COFF)[(int)*p] != BLANK && !iscomment(p)) {
                ppwarn("extra tokens (ignored) after directive");
                didwarn++;
            }
#endif
        }
        --flslvl;
    }
}

#ifndef SMALL
STATIC int iscomment(register char *s) {
    return s[0] == '/' && (s[1] == '*' || s[1] == '/');
}
#endif

STATIC struct symtab *stsym(register char *s) {
    register char *p;
    char buf[BUFFERSIZ];

    /* make definition look exactly like end of #define line */
    /* copy to avoid running off end of world when param list is at end */
    p = buf;
    while ((*p++ = *s++) != '\0')
        ;
    p = buf;
    while (isid(*p++))
        ; /* skip first identifier */
    if (*--p == '=') {
        *p++ = ' ';
        while (*p++)
            ;
    } else {
        s = " 1";
        while ((*p++ = *s++) != '\0')
            ;
    }
    pend = p;
    *--p = '\n';
    sloscan();
    dodef(buf);
    return (lastsym);
}

STATIC struct symtab *ppsym(char *s) { /* kluge */
    register struct symtab *sp;
    cinit    = SALT;
    *savch++ = SALT;
    sp       = stsym(s);
    --sp->name;
    cinit = 0;
    return (sp);
}

EXPORT void _pperror(char *fmt, va_list args) {

    if (fnames[ifno] && fnames[ifno][0])
        fprintf(stderr, "%s: line ", fnames[ifno]);
    fprintf(stderr, "%d: ", lineno[ifno]);

    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    exfail++;
}

EXPORT void pperror(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    _pperror(fmt, args);
    va_end(args);
}

EXPORT void yyerror(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);

    _pperror(fmt, args);
    va_end(args);
}

STATIC void ppwarn(char *fmt, ...) {
    va_list args;
    int fail = exfail;
    exfail   = -1;

    va_start(args, fmt);

    _pperror(fmt, args);
    va_end(args);

    exfail = fail;
}

struct symtab *lookup(char *namep, int enterf) {
    register char *np, *snp;
    register unsigned int c, i;
    register struct symtab *sp;
#ifdef SMALL
    int around = 0;
#else
    struct symtab *prev;
    static struct symtab nsym; /* Hack: Dummy nulled symtab */
#endif

    np = namep;
    i  = cinit;
    /* calc crc on at most 8 chars */
    while (np < namep + 8 && (c = *(unsigned char *)np++))
        i += i + c;
    c = i % symsiz;
#ifdef SMALL
    sp = &stab[c];
    while (snp = sp->name) {
        np = namep;
        while (*snp++ == *np)
            if (*np++ == '\0') {
                if (enterf == DROP) {
                    sp->name[0] = DROP;
                    sp->value   = 0;
                }
                return (lastsym = sp);
            }
        if (--sp < &stab[0])
            if (around) {
                pperror("too many defines", 0);
                exit(exfail);
            } else {
                ++around;
                sp = &stab[symsiz - 1];
            }
    }
    if (enterf > 0)
        sp->name = namep;
    return (lastsym = sp);
#else

    sp   = stab[c];
    prev = sp;

    while (sp != NULL) {
        snp = sp->name;
        np  = namep;
        while (*snp++ == *np) {
            if (*np++ == '\0') {
                if (enterf == DROP) {
                    sp->name[0] = DROP;
                    sp->value   = 0;
                }
                return (lastsym = sp);
            }
        }
        prev = sp;
        sp   = sp->next;
    }
    if (enterf > 0) {
        sp        = newsym();
        sp->name  = namep;
        sp->value = NULL;
        sp->next  = NULL;
        if (prev)
            prev->next = sp;
        else
            stab[c] = sp;
    }
    /*
     * Hack: emulate the behavior of the old code with static stab[], where
     * a non-matching request returns a zeroed (because previously empty)
     * struct symtab.
     */
    if (sp == NULL)
        sp = &nsym;
    return (lastsym = sp);
#endif
}

STATIC struct symtab *slookup(register char *p1, register char *p2, int enterf) {
    register char *p3;
    char c2, c3;
    struct symtab *np;
    c2  = *p2;
    *p2 = '\0'; /* mark end of token */
    if ((p2 - p1) > SYMLEN)
        p3 = p1 + SYMLEN;
    else
        p3 = p2;
    c3  = *p3;
    *p3 = '\0'; /* truncate to symlen chars or less */
    if (enterf == 1)
        p1 = copy(p1);
    np  = lookup(p1, enterf);
    *p3 = c3;
    *p2 = c2;
    if (np->value != 0 && flslvl == 0)
        newp = subst(p2, np);
    else
        newp = 0;
    return (np);
}

STATIC char *subst(register char *p, struct symtab *sp) {
    static char match[] = "%s: argument mismatch";
    register char *ca, *vp;
    int params;
    register char **pa;     /* moved here to avoid large offset */
    char *actual[MAXFRM];   /* actual[n] is text of nth actual */
    char acttxt[BUFFERSIZ]; /* space for actuals */

    if (0 == (vp = sp->value))
        return (p);
    if ((p - macforw) <= macdam) {
        if (++maclvl > symsiz && !rflag) {
            pperror("%s: macro recursion", sp->name);
            return (p);
        }
    } else
        maclvl = 0; /* level decreased */
    macforw = p;
    macdam  = 0; /* new target for decrease in level */
    macnam  = sp->name;
    dump();
    if (sp == ulnloc) {
        vp    = acttxt;
        *vp++ = '\0';
        sprintf(vp, "%d", lineno[ifno]);
        while (*vp++)
            ;
    } else if (sp == uflloc) {
        vp    = acttxt;
        *vp++ = '\0';
        ca    = fnames[ifno];
        *vp++ = '"';
        while (*ca) { /* original didn't expand \ to \\ in string */
            if (*ca == '\\')
                *vp++ = '\\';
            *vp++ = *ca++;
        }
        *vp++ = '"';
        *vp++ = '\0';
    }
    if (0 != (params = *--vp & 0xFF)) { /* definition calls for params */
        ca = acttxt;
        pa = actual;
        if (params == 0xFF)
            params = 1; /* #define foo() ... */
        sloscan();
        ++flslvl; /* no expansion during search for actuals */
        plvl = -1;
        do
            p = skipbl(p);
        while (*inptr == '\n'); /* skip \n too */
        if (*inptr == '(') {
            maclin = lineno[ifno];
            macfil = fnames[ifno];
            for (plvl = 1; plvl != 0;) {
                *ca++ = '\0';
                for (;;) {
                    outptr = inptr = p;
                    p              = cotoken(p);
                    if (*inptr == '(')
                        ++plvl;
                    if (*inptr == ')' && --plvl == 0) {
                        --params;
                        break;
                    }
                    if (plvl == 1 && *inptr == ',') {
                        --params;
                        break;
                    }
                    while (inptr < p)
                        *ca++ = *inptr++;
                    if (ca > &acttxt[BUFFERSIZ])
                        pperror("%s: actuals too long", sp->name);
                }
                if (pa >= &actual[MAXFRM])
                    ppwarn(match, sp->name);
                else
                    *pa++ = ca;
            }
        }
        if (params != 0)
            ppwarn(match, sp->name);
        while (--params >= 0)
            *pa++ = "" + 1; /* null string for missing actuals */
        --flslvl;
        fasscan();
    }
    for (;;) {                   /* push definition onto front of input stack */
        while (!iswarn(*--vp)) { /* Terminates with '\0' also */
            if (bob(p)) {
                outptr = inptr = p;
                p              = unfill(p);
            }
            *--p = *vp;
        }
        if (*vp == warnc) { /* insert actual param */
            if (vp[-1] ==
                warnc) { /* allow for 0xfe as an actual character, encoded as warnc warnc */
                *--p = *--vp;
                continue;
            }
            ca = actual[*--vp - 1];
            while (*--ca) {
                if (bob(p)) {
                    outptr = inptr = p;
                    p              = unfill(p);
                }
                *--p = *ca;
            }
        } else
            break;
    }
    outptr = inptr = p;
    return (p);
}

/* note hitech original trimmed to / if present */
STATIC char *trmdir(register char *s) {
    register char *p;

    p = strchr(s, '\0'); /* let the compiler use builtins to find end of s */
    while (--p > s && !ISDIRSEP(*p))
        ;

#ifndef CPM
    if (p == s) /* windows & unix use dot as current directory */
        *p++ = '.';
    else
#endif
        if (*p == ':') { /* windows & cpm support drive (cpm may also  user number before the : */
        p++;
#ifdef _WIN32
        *p++ = '.'; /* on windows convert c: to c:. so added dirsep selects current dir */
#endif
    }
    *p = '\0';
    return (s);
}

STATIC char *copy(register char *s) {
    register char *old;

    old = savch;
    while ((*savch++ = *s++))
        ;
    return (old);
}

int yywrap() {
    return (1);
}

int main(int argc, char *argv[]) {
    register int i, c;
    register char *p;
    char *tf, **cp2;
#ifndef CPM
    char *sysdir = NULL;
#endif
    if (argc == 1) {
        argv = _getargs(NULL, "cpp");
        argc = _argc_;
    }
    fin  = stdin; /* Mac OS X is not POSIX compliant (stdout nonconst.) */
    fout = stdout;

    p    = "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    i    = 0;
    while ((c = *p++)) {
        (fastab + COFF)[c] |= IB | NB | SB;
        (toktyp + COFF)[c] = IDENT;
    }
    p = "0123456789.";
    while ((c = *p++) != '\0') {
        (fastab + COFF)[c] |= NB | SB;
        (toktyp + COFF)[c] = NUMBR;
    }
    p = "\n\"'/\\";
    while ((c = *p++) != '\0')
        (fastab + COFF)[c] |= SB;
    p = "\n\"'\\";
    while ((c = *p++) != '\0')
        (fastab + COFF)[c] |= QB;
    p = "*\n";
    while ((c = *p++) != '\0')
        (fastab + COFF)[c] |= CB;
    (fastab + COFF)[(int)warnc] |= WB;
    (fastab + COFF)['\0'] |= CB | QB | SB | WB;
    for (i = ALFSIZ; --i >= 0;)
        slotab[i] = fastab[i] | SB;
    p = " \t\013\f\r"; /* note no \n;	\v not legal for vertical tab? */
    while ((c = *p++))
        (toktyp + COFF)[c] = BLANK;

    fnames[ifno = 0] = ""; /* Allow pperror() to work correctly	  */
#ifndef SMALL
    newsbf(); /* Must be called before copy() / ppsym() */
#endif
#if defined(unix) || defined(_WIN32)
    dirs[0] = dirnams[0] = ".";
#endif
    for (i = 1; i < argc && argv[i][0] == '-'; i++) {
        switch (argv[i][1]) {
        case 'P':
            pflag++;
        case 'E':
            break;
        case 'R':
            ++rflag;
            break;
        case 'C':
            passcom++;
            break;
        case 'D':
            if (predef > prespc + NPREDEF) {
                pperror("too many -D options, ignoring %s", argv[i]);
                break;
            }
            /* ignore plain "-D" (no argument) */
            if (*(argv[i] + 2))
                *predef++ = argv[i] + 2;
            break;
        case 'U':
            if (prund > punspc + NPREDEF) {
                pperror("too many -U options, ignoring %s", argv[i]);
                break;
            }
            *prund++ = argv[i] + 2;
            break;
#ifndef CPM
        case 'n':
            if (strcmp(argv[i], "-noinclude") == 0)
                noinclude = 1;
            else
                goto unknown;
            break;
#endif
#ifndef SMALL
        case 'u':
            if (strcmp(argv[i], "-undef") == 0)
                nopredef = 1;
            else
                goto unknown;
            break;
        case 'v':
            if (strcmp(argv[i], "-version") == 0) {
                printf("cpp version %s %s (%s-%s-%s)\n", VERSION, VERSION_DATE, HOST_CPU,
                       HOST_VENDOR, HOST_OS);
                printf("\n");
                printf("Copyright (C) 1978 AT&T (John F. Reiser)\n");
                printf("Copyright (C) 2010-2021 Joerg Schilling\n");
                printf("Changes Copyright (C) 2022 Mark Ogden\n");
                exit(0);
            }
            goto unknown;
#endif
        case 'I':
            if (nd >= MAXIDIRS)
                pperror("excessive -I file (%s) ignored", argv[i]);
            else
                dirs[nd++] = argv[i] + 2;
            break;
#ifndef SMALL
        case 'p': /* -T + extra tokens warn */
            extrawarn++;
            break;
        case 'H': /* Print included filenames */
            hflag++;
            break;

        case 'M': /* Generate make dependencies */
            mflag++;
            break;
        case 'Y': /* Replace system include dir */
            sysdir = argv[i] + 2;
            break;
        case '\0':
            break;
        case '-':
            if (strcmp(argv[i], "--help") == 0)
                usage();
            else
                goto unknown;
        case 'h':
            if (strcmp(argv[i], "-help") == 0)
                usage();
/* FALLTHRU */
#endif
#ifndef CPM
        unknown:
#endif
        default:
            pperror("unknown flag %s", argv[i]);
            break;
        }
    }

    if (i < argc) {
        if ((fin = freopen(argv[i], "r", stdin)) == NULL) {
            pperror("No source file %s", argv[i]);
            exit(8);
        }
        fnames[ifno] = copy(argv[i]);
#ifndef SMALL
        input        = copy(argv[i]);
#endif
        dirs[0] = dirnams[ifno] = trmdir(argv[i]);
        if (++i < argc) {
            if ((fout = freopen(argv[i], "w", stdout)) == NULL) {
                pperror("Can't create %s", argv[i]);
                exit(8);
            }
        }
    }

#ifndef SMALL
    /* CPM doesn't support the NULL device, so omit support rather than
     * make changes where fout is used. also omit for SMALL build
     */
    if (mflag) {
        if (input == NULL) {
            pperror("cpp: no input file specified with -M flag");
            exit(8);
        }
        p = strrchr(input, '.');
        if (p == NULL || p[1] == '\0') {
            pperror("cpp: no filename suffix");
            exit(8);
        }
        p[1]  = 'o';
        input = fname(input);
        mout  = fout;
        if (NULL == (fout = fopen(NULDEV, "w"))) {
            pperror("Can't create " NULDEV);
            exit(8);
        }
    }
#endif

    fins[ifno] = fin;
    exfail     = 0;
/* after user -I files here are the standard include libraries */
#ifndef CPM
    if (sysdir != NULL) {
        dirs[nd++] = sysdir;
    } else if (!noinclude) {
        /* use environment variable to reflect where the cross compiler includes are
         * if INCDIR80 is defined then use it as the include directory
         */
        char *s;
        if (s = getenv("INCDIR80")) {
            dirs[nd] = strdup(s);
            s        = strchr(dirs[nd++], '\0') - 1;
            if (ISDIRSEP(*s))
                *s = '\0';
        }
    }
#endif
    dirs[nd++] = 0;
    defloc     = ppsym("define");
    udfloc     = ppsym("undef");
    incloc     = ppsym("include");
    elsloc     = ppsym("else");
    eifloc     = ppsym("endif");
    elifloc    = ppsym("elif"); /* new */
    ifdloc     = ppsym("ifdef");
    ifnloc     = ppsym("ifndef");
    ifloc      = ppsym("if");
    lneloc     = ppsym("line");
    asmloc     = ppsym("asm");    /* from hitech */
    easloc     = ppsym("endasm"); /* from hitech */
    pragmaloc  = ppsym("pragma"); /* new ignored */
    errorloc   = ppsym("error");  /* new */

    memset(macbit, 0, sizeof(macbit));

#ifndef SMALL
    if (!nopredef)
#endif
        varloc = stsym("z80"); /* this is a cross compiler cpp so only define z80 */

    ulnloc       = stsym("__LINE__");
    uflloc       = stsym("__FILE__");

    tf           = fnames[ifno];
    fnames[ifno] = "command line";
    lineno[ifno] = 1;
    cp2          = prespc;
    while (cp2 < predef)
        stsym(*cp2++);
    cp2 = punspc;
    while (cp2 < prund) {
        if ((p = strdex(*cp2, '=')))
            *p++ = '\0';
        lookup(*cp2++, DROP);
    }
    fnames[ifno] = tf;
    pbeg         = buffer + SYMLEN;
    pbuf         = pbeg + BUFFERSIZ;
    pend         = pbuf + BUFFERSIZ;

    trulvl       = 0;
    flslvl       = 0;
    lineno[0]    = 1;
    sayline();
    outptr = inptr = pend;
#ifndef SMALL
    if (mflag)
        fprintf(mout, "%s: %s\n", input, fnames[ifno]);
#endif
    control(pend);
    if (fclose(stdout) == -1) {
        fprintf(stderr, "CPP: Error closing output file\n");
        return 1;
    }
    return (exfail);
}

#ifndef SMALL
STATIC void newsbf() {
    /*
     * All name space in symbols is obtained from sbf[] via copy(), so we
     * cannot use realloc() here as this might relocate sbf[]. We instead
     * throw away the last part of the current sbf[] and allocate new space
     * for new symbols.
     */

    if ((sbf = malloc(SBSIZE)) == NULL) {
        pperror("no buffer space");
        exit(exfail);
    }
    savch = sbf; /* Start at new buffer space */
}

STATIC struct symtab *newsym() {
    static int nelem           = 0;
    static struct symtab *syms = NULL;

    if (nelem <= 0) {
        syms = malloc(SYMINCR * sizeof(struct symtab));
        if (syms == NULL) {
            pperror("too many defines");
            exit(exfail);
        }
        nelem = SYMINCR;
    }
    nelem--;
    return (syms++);
}

STATIC void usage() {
    fprintf(stderr, "Usage: cpp [options] [input-file [output-file]]\n");
    fprintf(stderr, "Options:\n");

    fprintf(stderr, "	-C	Pass all comments.\n");
    fprintf(stderr, "	-H	Print the path names of include files on standard error.\n");
    fprintf(stderr, "	-M	Generate a list of dependencies and write them to the output.\n");
    fprintf(stderr, "	-p	Warn of extra tokens in directives.\n");
    fprintf(stderr, "	-P	Do not include line control information in the preprocessor output.\n");
    fprintf(stderr, "	-R	Allow recursive macros.\n");
    fprintf(stderr, "	-noinclude Ignore standard system include path.\n");
    fprintf(stderr, "	-undef	Remove all initially predefined macros.\n");
    fprintf(stderr, "	-Dname	Defines name as 1.\n");
    fprintf(stderr, "	-Dname=var Defines name as val.\n");
    fprintf(stderr, "	-Idirectory Adds directory to the search path.\n");
    fprintf(stderr, "	-Uname	Remove an initial definition of name.\n");

    fprintf(stderr,
            "	-Ydirectory Uses directory instead of the standard system include directory.\n");

    exit(0);
}
#endif
