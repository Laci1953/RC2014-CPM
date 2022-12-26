/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File - main.c
 */

#include "cgen.h"
#include "alloc128.h"

void top_main(int argc, char **argv)
{
    InitDynM();	/* Init upper 64KB support */

#ifdef CPM
    baseHeap = sbrk(0); /* Current highest memory */
#endif
    --argc, ++argv;
    while (argc > 0 && **argv == '-') { /* Parsing options */
        switch (argv[0][1]) {
        case 'N':
        case 'n':
            nflag = true;
            break; /* Enables statistics printing */
        case 'P':
        case 'p':
            pflag = true;
            break; /* Not use */
        case 'W':
        case 'w':
            wflag = true;
            break; /* Displaying warnings */
        case 'R':
            rflag = true;
            break;
        case 'B':
            bflag = true;
            break; /* Not use */
        case 'E':
            eflag = true;
            break; /* Not use */
        case 'H':
            hflag = true;
            break; /* Not use */
        default:
            fatalErr("Illegal\tswitch %c", argv[0][1]);
            break;
        }
        argv++, argc--;
    }

    if (argc-- > 0) {
        if (freopen(*argv, "r", stdin) == NULL) /* Open input file */
            fatalErr("Can't open %s", *argv);
        else if (argc > 0 && freopen(argv[1], "w", stdout) == NULL) /* Open output file */
            fatalErr("Can't create %s", argv[1]);
    }
}

/*********************************************************
 * main OK++ PMO
 *
 *********************************************************/
int main(int argc, char **argv) {

    top_main(argc, argv);

    sub_1680();                  /* First_init */
    parseStmt();                 /* Compiling intermediate code */

    if (fclose(stdout) == EOF) { /* Close output file */
        prError("Error closing output file");
    }

    if (nflag)
	fprintf(stderr, "%d KB used from the upper 64KB RAM\r\n", (unsigned short)TotalSize() / 1024);

    /* Exit with error code */
    exit(errcnt != 0); /* Generated code is not significantly different */
}

/*********************************************************
 * sub_6AA2 OK PMO Used in: badIntCode,  sub_17E0, sub_19C1,
 *			    sub_2BD0, sub_4192, sub_6B1D,
 * fatalErr error  	    sub_6B9B,
 * Difference due to change to use stdarg
 *********************************************************/
_Noreturn void fatalErr(fmt, p2, p3) char* fmt;
{
    prMsg(fmt, p2, p3);
    fclose(stdout);
    exit(2);
}
/*********************************************************
 * sub_6AD0  OK PMO  Used in: sub_E43,  sub_3DC9, sub_43EF,
 * Warning message	      sub_54B6, sub_5CF5, sub_600E
 * Difference due to change to use stdarg
 *********************************************************/
void prWarning(fmt, p2, p3) char *fmt;
{
        if (wflag == 0) 
	    prMsg(fmt, p2,p3);
}

/*********************************************************
 * prError OK PMO
 * Nonfatal error
 * Difference due to use of stdarg
 *********************************************************/
void prError(fmt, p2, p3) char * fmt;
{
    prMsg(fmt, p2, p3);

    if (++errcnt >= MAXERR)
        fatalErr("Too many errors");
}

/*********************************************************
 * prMsg OK PMO	      	  Used in: ferror, prError
 * Difference due to use of stdarg
 *********************************************************/
void prMsg(fmt, p2, p3) char *fmt;
{
    fprintf(stderr, "%s:%d:\t", progname, lineno);
    fprintf(stderr, fmt, p2, p3);
    fputc('\n', stderr);
}

/*********************************************************
 * allocMem OK    Used in: sub_265,  sub_1754, sub_19C1,
 *			     sub_1B0C, allocNode, sub_39CA,
 * alloc_mem & return ptr    sub_4192, sub_508A, sub_5DF6
 *********************************************************/
void *allocMem(size_t size) {
    register char *ptr;

//    do {
//        if (ptr = malloc(size))
//            goto done;
//    } while (releaseNodeFreeList());

    if (!(ptr = malloc(size)))
    	fatalErr("No room");

    blkclr(ptr, size);

    return ptr;
}

char* MyAllocStr(size_t size)
{
    register char *ptr;

    if (ptr = myallocstr(size))
      return ptr;

    fatalErr("Upper RAM full");
    return 0;
}

char* MyAllocMbr(void)
{
    register char *ptr;

    if (ptr = myallocmbr())
      return ptr;

    fatalErr("Upper RAM full");
    return 0;
}

/* end of file main.c */
