/* @(#)cpp.h	1.5 15/02/12 Copyright 2010-2015 J. Schilling */
/*
 * Copyright (c) 2010-2015 J. Schilling
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

#ifndef _CPP_H
#define _CPP_H
#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_DECLARE_NONSTDC_NAMES 1
#define _CRT_NONSTDC_NO_WARNINGS 1

#define LOCAL static
#define EXPORT

#ifdef CPM
int _doprnt();
#define vfprintf _doprnt
#define UConst
#define signed
#define SMALL
#else
#define UConst const
#endif
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

/* support Windows/MSDOS & unix variants */
#ifdef _WIN32
#define NULDEV       "NUL"
#define DIRSEP       "/\\"
#define DEVSEP       ':'
#define ISDIRSEP(ch)    ((ch) == '/' || (ch) == '\\' || (ch) == DEVSEP)
#define HOST_OS      "Windows"
#else
#ifdef CPM
#define DIRSEP      ":"
#define DEVSEP      ':'
#define ISDIRSEP(ch)    ((ch) == DEVSEP)
#define HOST_OS     "CP/M"
#else
#define NULDEV      "/dev/null"
#define DIRSEP       "/"
#define ISDIRSEP(ch)    ((ch) == '/')
#define HOST_OS     "Unix"
#endif
#endif

#define HOST_CPU    "Z80"
#define HOST_VENDOR "Hi-Tech"

struct symtab {
    char *name;
    char *value;
    struct symtab *next;
};

extern char *skipbl(char *);
extern void pperror(char *fmt, ...);
extern void yyerror(char *fmt, ...);
extern int yyparse(void);
extern int yylex(void);
extern struct symtab *lookup(char *, int);

/* get args support */
extern int _argc_;
char **_getargs(char *_str, char *_name);
char *fname(char *name);
#endif /* _CPP_H */
