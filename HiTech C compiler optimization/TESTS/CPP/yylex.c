/* @(#)yylex.c	1.9 19/08/29 2010-2019 J. Schilling */
/* modified to use standard C headers 6-Jul-2022 Mark Ogden */
#include "cpp.h"

#ifndef lint
static UConst char sccsid[] = "@(#)yylex.c	1.9 19/08/29 2010-2019 J. Schilling";
#endif
/*
 * This implementation is based on the UNIX 32V release from 1978
 * with permission from Caldera Inc.
 *
 * Copyright (c) 2010-2019 J. Schilling
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

#include <string.h>

/*
 * XXX This block should be moved to cpp.h, it is douplicated in cpp.c
 */
#define isid(a) ((fastab + COFF)[(int)a] & IB)
#define IB      1
/*	#if '\377' < 0		it would be nice if this worked properly!!!!! */
#if pdp11 | vax | '\377' < 0
#define COFF 128
#else
#define COFF 0
#endif

EXPORT int yylex(void);
LOCAL int tobinary(char *, int);

int yylex() {
    static int ifdef   = 0;
    static char *op2[] = { "||", "&&", ">>", "<<", ">=", "<=", "!=", "==" };
    static int val2[]  = { OROR, ANDAND, RS, LS, GE, LE, NE, EQ };
    static char *opc   = "b\bt\tn\nf\fr\r\\\\";
    extern char fastab[];
    extern char *outptr, *inptr, *newp;
    extern int flslvl;
    register char savc, *s;
    int val;
    register char **p2;
    struct symtab *sp;

    for (;;) {
        newp = skipbl(newp);
        if (*inptr == '\n')
            return (stop); /* end of #if */
        savc  = *newp;
        *newp = '\0';
        for (p2 = op2 + 8; --p2 >= op2;) /* check 2-char ops */
            if (0 == strcmp(*p2, inptr)) {
                val = val2[p2 - op2];
                goto ret;
            }
        s = "+-*/%<>&^|?:!~(),"; /* check 1-char ops */
        while (*s)
            if (*s++ == *inptr) {
                val = *--s;
                goto ret;
            }
        if (*inptr <= '9' && *inptr >= '0') { /* a number */
            if (*inptr == '0')
                yylval = (inptr[1] == 'x' || inptr[1] == 'X') ? tobinary(inptr + 2, 16)
                                                              : tobinary(inptr + 1, 8);
            else
                yylval = tobinary(inptr, 10);
            val = number;
        } else if (isid(*inptr)) {
            if (0 == strcmp(inptr, "defined")) {
                ifdef = 1;
                ++flslvl;
                val = DEFINED;
            } else {
                sp = lookup(inptr, -1);
                if (ifdef != 0) {
                    ifdef = 0;
                    --flslvl;
                }
                yylval = (sp->value == 0) ? 0 : 1;
                val    = number;
            }
        } else if (*inptr == '\'') { /* character constant */
            val = number;
            if (inptr[1] == '\\') { /* escaped */
                signed char c;

                if (newp[-1] == '\'')
                    newp[-1] = '\0';
                s = opc;
                while (*s)
                    if (*s++ != inptr[2])
                        ++s;
                    else {
                        yylval = *s;
                        goto ret;
                    }
                if (inptr[2] <= '9' && inptr[2] >= '0')
                        yylval = c = tobinary(inptr + 2, 8);
                else
                    yylval = inptr[2];
            } else
                yylval = inptr[1];
        } else if (0 == strcmp("\\\n", inptr)) {
            *newp = savc;
            continue;
        } else {
            *newp = savc;
            pperror("Illegal character %c in preprocessor if", *inptr);
            continue;
        }
    ret:
        *newp  = savc;
        outptr = inptr = newp;
        return (val);
    }
}

LOCAL int tobinary(char *st, int b) {
    int n, c, t;
    char *s;
    n = 0;
    s = st;
    while ((c = *s++)) {
        switch (c) {
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
            t = c - '0';
            break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            t = c - 'a' + 10;
            if (b > 10)
                break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            t = c - 'A' + 10;
            if (b > 10)
                break;
        default:
            t = -1;
            if (c == 'l' || c == 'L')
                if (*s == '\0')
                    break;
            pperror("Illegal number %s", st);
        }
        if (t < 0)
            break;
        n = n * b + t;
    }
    return (n);
}
