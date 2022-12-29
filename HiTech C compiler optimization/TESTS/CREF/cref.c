/*
 *
 * The cref.c file is part of the restored P1.COM program
 * from the Hi-Tech CP/M Z80 C v3.09
 *
 * Not a commercial goal of this laborious work is to popularize among
 * potential fans of 8-bit computers the old HI-TECH Z80 C compiler V3.09
 * (HI-TECH Software) and extend its life, outside of the CP/M environment
 * for full operation in windows 32/64 and Unix-like operating systems
 *
 * The HI-TECH Z80 C cross compiler V3.09 is provided free of charge for any use,
 * private or commercial, strictly as-is. No warranty or product support
 * is offered or implied including merchantability, fitness for a particular
 * purpose, or non-infringement. In no event will HI-TECH Software or its
 * corporate affiliates be liable for any direct or indirect damages.
 *
 * You may use this software for whatever you like, providing you acknowledge
 * that the copyright to this software remains with HI-TECH Software and its
 * corporate affiliates.
 *
 * All copyrights to the algorithms used, binary code, trademarks, etc.
 * belong to the legal owner - Microchip Technology Inc. and its subsidiaries.
 * Commercial use and distribution of recreated source codes without permission
 * from the copyright holderis strictly prohibited.
 *
 *
 * See the readme.md file for additional commentary
 *
 * Mark Ogden
 * 25-Aug-2022
 */
/*
 *  Changes
 *  25-Aug-2022
 *      Add support for UZI-180 build as identified by Nikitin Andrey
 *
 */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * CP/M has a very limited command line length, to work around this
 * HiTech supply a function _getargs which allows user entry of
 * multi line command lines.
 * For most other systems this isn't really needed, so by default it isn't
 * used. It can be forced by defining the variable USE_GETARGS and linking
 * with a _getargs implementation
 */
#ifdef CPM
#define USE_GETARGS 1
#endif

#define true 1
#define false 0
#define const
#define _Noreturn
#define vfprintf _doprnt

typedef unsigned char bool;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;
typedef char int8_t;


#define SYMTABSIZE  3001
#define MAX_LINELEN 256

typedef struct _xref {
    uint16_t line;
    struct _sym *sym;
    int8_t flag;
} xref_t;

typedef struct _sym {
    char *name;
    uint16_t cntXrefs;
    uint16_t sizeXrefs;
    xref_t *xrefs;
    struct _sym *next;
    int8_t flags;
} sym_t;

int16_t width   = 80;
int16_t pageLen = 66;
char *header    = "";

bool needNewLine;
int16_t maxNameLen;
char *xPrefixList[30];
sym_t *curSym;
uint16_t pagenum;
uint16_t row;
uint16_t symCount;
uint16_t xPrefixCnt;
char buf[MAX_LINELEN];
bool showIncludes; /* addition to omit include file references */

sym_t *symtab[SYMTABSIZE];
char *o_str;
int16_t maxDefNameLen;

#ifdef USE_GETARGS
extern int _argc_;
char **_getargs(char *_str, const char *_name);
#endif

uint16_t hash(register char *s, uint16_t size);
sym_t *getSymEntry(register char *s, int16_t flag);
void *alloc(size_t size);
int cmpSym(const sym_t **pp1, const sym_t **pp2);
void packSym(void);
void addXref(char *name, uint16_t line, uint8_t flag);
int main(int argc, char **argv);
_Noreturn void fatal(const char *msg, ...);
void loadXref(register char *s);
int cmpXref(register const xref_t *pxref, const xref_t *qxref);
bool chkExclude(register char *name);
void stopList(register char *filename);
void printAll(void);
void eject(void);
void printHeading(void);
void checkIfHeading(void);
void newline(void);
void printXref(register sym_t *sym);
/* gets is unsafe and removed in later versions of C
 * a safer version xgets is provided, which also
 * handles \r\n for unix variants
 */
char *xgets(char *buf, int size);

uint16_t hash(register char *s, uint16_t size) {
    uint16_t val = 0;
    while (*s)
        val += val + *s++;
    return val % size;
}

sym_t *getSymEntry(register char *s, int16_t flag) {
    sym_t *p;
    sym_t **pslot;
    int16_t len;

    pslot = &symtab[hash(s, SYMTABSIZE)];
    for (p = *pslot; p && (strcmp(p->name, s) != 0 || (p->flags & 3) != flag); p = p->next)
        ;
    if (p)
        return p;

    if (++symCount == SYMTABSIZE)
        fatal("Too many symbols");
    p       = alloc(sizeof(sym_t));
    p->next = *pslot;
    *pslot  = p;
    len     = (int16_t)strlen(s);
    p->name = alloc(len + 1);
    strcpy(p->name, s);
#if 0
    /* moved to loadXref as path names caused width of name field to also increase */
    if (len > maxNameLen)
        maxNameLen = len;
#endif
    p->xrefs     = alloc(20 * sizeof(xref_t));
    p->sizeXrefs = 20;
    p->flags     = (int8_t)flag;

    return p;
}

void *alloc(size_t size) {
    register void *p;
    /* void *unused; */
    if ((p = calloc(size, 1)) == NULL)
        fatal("Out of memory");
    /* unused = p; */
    return p;
}

int cmpSym(const sym_t **pp1, const sym_t **pp2) {
    register const sym_t *p1 = *pp1;
    const sym_t *p2          = *pp2;
    if (p1 == p2)
        return 0;
    if (p1 == 0)
        return 1;
    if (p2 == 0)
        return -1;
    return strcmp(p1->name, p2->name);
}

void packSym() {
    sym_t **var2;
    sym_t **var4;
    sym_t *tmp;
    register sym_t *p;

    for (var4 = var2 = symtab; var2 < symtab + SYMTABSIZE; var2++) {
        p = *var2;
        while (p && p->next) {
            while (*var4)
                var4++;
            *var4     = p->next;
            tmp       = p;
            p         = p->next;
            tmp->next = 0;
        }
    }
    for (var4 = var2 = symtab; var2 != symtab + SYMTABSIZE; var2++) {
        if (*var2)
            *var4++ = *var2;
    }
    qsort(symtab, var4 - symtab, sizeof(sym_t *), (int (*)(void *, void *))cmpSym);
}

void addXref(char *name, uint16_t line, uint8_t flag) {
    xref_t *xref;
    register sym_t *p;

    p = getSymEntry(name, 1);
    if (p->cntXrefs) {
        xref = &p->xrefs[p->cntXrefs - 1];
        if (xref->sym == curSym && xref->line == line) {
            xref->flag |= flag;
            return;
        }
    }
    if (p->cntXrefs == p->sizeXrefs &&
        (p->xrefs = realloc(p->xrefs, (p->sizeXrefs += 20) * sizeof(xref_t))) == NULL)
        fatal("out of memory");
    xref       = &(p->xrefs[p->cntXrefs++]);
    xref->line = line;
    xref->flag = flag;
    xref->sym  = curSym;
}

int main(int argc, char **argv) {
    --argc, ++argv;
#ifdef USE_GETARGS
    if (argc == 0) {
        argv = _getargs(NULL, "cref");
        argc = _argc_;
    }
#endif
    while (argc && argv[0][0] == '-') {
        switch (argv[0][1]) {
        case 'O':
        case 'o':
            o_str = argv[0] + 2;
            break;
        case 'P':
        case 'p':
            width = atoi(argv[0] + 2);
            break;
        case 'L':
        case 'l':
            pageLen = atoi(argv[0] + 2);
            break;
        case 'H':
        case 'h':
            header = argv[0] + 2;
            break;
        case 'X':
        case 'x':
            xPrefixList[xPrefixCnt++] = argv[0] + 2;
            break;
        case 'S':
        case 's':
            stopList(argv[0] + 2);
            break;
        case 'I':
        case 'i':
            showIncludes = true;
            break;
        default:
            fprintf(stderr, "Unrocognize switch %s\n", argv[0]);
            break;
        }
        --argc, ++argv;
    }

    if (argc == 0) {
        fprintf(stderr, "Usage: cref [switches] [input_file]+\n");
        exit(1);
    }
    if (!header)
        header = argv[0];
    while (argc--) {
        if (!freopen(argv[0], "r", stdin))
            fatal("Can't open %s for input", argv[0]);
        while (xgets(buf, MAX_LINELEN))
            loadXref(buf);
        argv++;
    }

    packSym();
    if (o_str && *o_str && !freopen(o_str, "w", stdout))
        fatal("Can't open %s for  output\n", o_str);
    printAll();
    exit(0);
}

void fatal(const char *msg, ...) {
    va_list args;
    fprintf(stderr, "CREF ERROR: ");

    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "\nExecution terminated\n");
    exit(1);
}

void loadXref(register char *s) {
    char *p;
    int16_t n;

    if (*s == '~') {
        if (!showIncludes && ((p = strrchr(s, '.')) && strcmp(p, ".h") == 0))
            return;
        curSym = getSymEntry(s + 1, 0);
        n      = (int16_t)strlen(s + 1);
        if (n > maxDefNameLen)
            maxDefNameLen = n;
    } else {
        p = *s == '#' ? s + 1 : s;
        while (!isspace(*p++))
            ;
        p[-1] = 0;
        n     = (int16_t)strlen(s);
        if (n > maxNameLen) /* more appropriate place to set field width */
            maxNameLen = n;
        n = atoi(p);
        if (*s == '#')
            addXref(s + 1, n, 8);
        else
            addXref(s, n, 0);
    }
}

int cmpXref(register const xref_t *pxref, const xref_t *qxref) {
    if (pxref->sym != qxref->sym)
        return strcmp(pxref->sym->name, qxref->sym->name);
    return pxref->line - qxref->line;
}

bool chkExclude(register char *name) {
    uint16_t pindex;
    uint16_t sindex;

    for (pindex = 0; pindex != xPrefixCnt; pindex++) {
        if (*xPrefixList[pindex]) {
            for (sindex = 0;; sindex++) {
                if (xPrefixList[pindex][sindex] == 0)
                    return true;
                if (xPrefixList[pindex][sindex] != name[sindex] &&
                    (!isdigit(xPrefixList[pindex][sindex]) || !isdigit(name[sindex])))
                    break;
            }
        }
    }
    return false;
}

void stopList(register char *filename) {
    char buf[80];
    if (!freopen(filename, "r", stdin))
        fprintf(stderr, "Can't open stoplist file %s\n", filename);
    else
        while (scanf("%s", buf) == 1)
            getSymEntry(buf, 1)->flags |= 4;
}

void printAll() {
    sym_t **ppsym;
    register sym_t *psym;

    for (ppsym = symtab; *ppsym; ppsym++) {
        psym = *ppsym;
        if ((psym->flags & 7) == 1 && !chkExclude(psym->name)) {
            qsort(psym->xrefs, psym->cntXrefs, sizeof(xref_t), (int (*)(void *, void *))cmpXref);
            printXref(psym);
        }
    }
    eject();
}

void eject() {
    if (row)
        fputc('\f', stdout);
    row = 0;
}

void printHeading() {
    printf("\n\nCross reference listing: %s", header);
    printf("%*s %3d\n\n\n", (int)(width - strlen(header) - 38), "Page", ++pagenum);
    row = 5;
}

void checkIfHeading() {
    if (row == 0)
        printHeading();
}

void newline() {
    if (row) {
        fputc('\n', stdout);
        if (++row == pageLen - 5)
            eject();
    }
}

void printXref(register sym_t *sym) {
    int16_t col = 0;
    bool showSymName;
    sym_t *xSym;
    uint16_t line;
    uint16_t i;

    if (needNewLine)
        newline();
    needNewLine = true;
    checkIfHeading();
    showSymName = true;
    xSym        = NULL;
    line        = 0xffff;
    for (i = 0; i < sym->cntXrefs; i++) {
        if (sym->xrefs[i].sym != xSym) {
            if (col) {
                newline();
                col = 0;
                checkIfHeading();
            }
            xSym = sym->xrefs[i].sym;
            if (showSymName)
                printf("%-*s", maxNameLen + 2, sym->name);
            else
                printf("%-*s", maxNameLen + 2, "");

            printf("%-*s", maxDefNameLen + 2, sym->xrefs[i].sym->name);
            col         = maxDefNameLen + maxNameLen + 4;
            showSymName = false;
        } else if (sym->xrefs[i].line == line)
            continue;
        if (col + 8 > width) {
            newline();
            checkIfHeading();
            printf("%-*s", col = maxDefNameLen + maxNameLen + 4, "");
        }
        printf("%5d%c", line = sym->xrefs[i].line, sym->xrefs[i].flag & 8 ? '#' : ' ');
        col += 6;
    }
    if (col)
        newline();
}


char *xgets(char *buf, int size) {
    char *s = fgets(buf, size, stdin);
    char *t;
    if (s && (t = strchr(s, '\n'))) {
        *t = '\0';
#if !defined(CPM) && !defined(_WIN32)
        /* handle \r\n */
        if (s != t && *--t == '\r')
            *t = 0;
#endif
    }
    return s;
}
