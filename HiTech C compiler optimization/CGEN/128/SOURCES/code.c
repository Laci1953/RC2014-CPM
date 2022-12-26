/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File - code.c
 */

#define MINI
#include "cgen.h"

uint16_t   nodesize(node_t *);
node_t    *parseExpr(void);
void       prDefb0s(int);
void prError(char *fmt, ...);
void       prPsect(int);
int        varSize(member_t *);
void       expect(char);
void       prGlobalDef(member_t *);
char      *getToken();
void prWarning(char *fmt, ...);
void       sortCaseLabels(int *pCase, int *pLabel, int nCase);
node_t    *allocNode(void);
int        newLocal();
member_t **gethashptr(char *);
void *allocMem(size_t);
member_t  *sub_265(char *);
int        sub_808(member_t *, node_t *);
void       sub_CAB(member_t *);
void       sub_1420(int);
uint8_t    sub_14F3(node_t *);
int        sub_1CC4(member_t *);
void       sub_2BD0(node_t *);
void       sub_3DC9(node_t *);
node_t    *sub_415E(long);
node_t    *sub_43EF(uint8_t, node_t *, node_t *);
void       sub_475C(node_t *);
node_t    *sub_600E(node_t *);

#include "alloc128.h"

/*********************************************************
 * sub_808 OK++ PMO			 Used in: sub_B19
 * code identical except for where the original code had
 * l = l4b->b_b6 immediately followed
 * by hl = 0. The useless l = l4b->b-b6 code is no longer there
 * Note there appears to be a problem with hitech code generation
 * around << of a long value
 * if the rhs is uint8_t then the calculation is done incorrectly
 *
 *********************************************************/

int sub_808(member_t *sb, register node_t *p2a) {
    int16_t l1;
    int16_t l2;
    member_t *l3b;
    member_t *l4b;
    int l5;
    node_t *l6a FORCEINIT;
    struct _memb * membp;
    int bsize;

    l1 = 0;
    if (p2a->a_c0 == COLON_U) {
        sub_475C(p2a);
        l5 = 0;
        if (/* sb->b_class */ GetByte((char*)sb+OFF_b_class) == (char)STRUCT) {
            l3b = sb;
            while ((p2a = parseExpr())->a_c0 != DOT_DOT) {
		membp = (struct _memb *)GetWord((char*)l3b+OFF_b_memb);
                if ((l2 = sub_1CC4(l4b = /* l3b->b_memb.mlist->vals[l5++] */ membp->vals[l5++] ) ) != 0 &&
		    l1 % ++l2 != 0) {
                    l2 -= l1 % l2;
                    l1 += l2;
                    prDefb0s(l2); /* emit defb 0 to pad to item boundary */
                }
                if (/* l4b->b_sloc */ GetByte((char*)l4b+OFF_b_sloc) & 0x10) {
                    p2a = sub_43EF(BAND, p2a, sub_415E((1L << (int8_t)(/* l4b->b_b5 */ GetByte((char*)l4b+OFF_b_b5))) - 1L));
                    if (/* l4b->b_b6 */ GetByte((char*)l4b+OFF_b_b6) != 0)
                        l6a = sub_43EF(BOR, l6a, sub_43EF(LSHIFT, p2a, sub_415E(0L)));
                    else
                        l6a = p2a;

                    if (l5 != /* l3b->b_memb.mlist->cnt */ membp->cnt &&
(/* l3b->b_memb.mlist->vals[l5]->b_sloc */ GetByte((char*)(membp->vals[l5])+OFF_b_sloc) & 0x10) &&
 /* l3b->b_memb.mlist->vals[l5]->b_off */ GetWord((char*)(membp->vals[l5])+OFF_b_off) == /* l4b->b_off */ GetWord((char*)l4b+OFF_b_off))
                        continue;
                    p2a = l6a;
                }
                l1 += sub_808(l4b, p2a);
            }

            sub_475C(p2a);
        } else {
            l3b = /* sb->b_type */ (member_t *)GetWord((char*)sb+OFF_b_type);
            for (;;) {
                if ((p2a = parseExpr())->a_c0 == COLON_U)
                    l1 += sub_808(l3b, p2a);
                else if (p2a->a_c0 == DOT_DOT) {
                    sub_475C(p2a);
                    break;
                } else {
                    l1 += nodesize(p2a);
                    sub_2BD0(sub_600E(sub_43EF(ATGIGN, p2a, 0)));
                }
            }
        }
    } else {
        p2a = sub_600E(sub_43EF(ATGIGN, p2a, 0));
        l1  = nodesize(p2a);
        sub_2BD0(p2a);
    }
    bsize = GetWord((char*)sb+OFF_b_size);
    if (l1 < /* sb->b_size */ bsize) {
        prDefb0s(/* sb->b_size */ bsize - l1);
        l1 = /* sb->b_size */ bsize;
    } else if (/* sb->b_size */ bsize < l1 && /* sb->b_size */ bsize != 0)
        prError("Too many initializers");

    return l1; /* m15: */
}

/*********************************************************
 * sub_B19 OK++	PMO			 Used in: sub_6D1
 *********************************************************/
void parseInit() {
    register member_t *sb;
    int l1;
    int l2;

    sb = sub_265(getToken());
    prPsect(P_DATA);
    sub_CAB(sb); /* Emit "symbolic_name:" (identifier label) */
    l1 = sub_808(sb, parseExpr());
    l2 = /* sb->b_refl */ GetWord((char*)sb+OFF_b_refl) ? 2 : /* sb->b_type->b_size */ GetWord((char*)GetWord((char*)sb+OFF_b_type)+OFF_b_size);
    if (/* sb->b_nelem */ GetWord((char*)sb+OFF_b_nelem) == 0) { /* make sure nelem is set */
        /* sb->b_nelem = l1 / l2; */
	PutWord((char*)sb+OFF_b_nelem, (short)(l1 / l2));
        /* sb->b_size  = varSize(sb); */
	PutWord((char*)sb+OFF_b_size, (short)varSize(sb)); 
    }
    expect(']');
}

/*********************************************************
 * sub_B96 OK++ PMO			Used in: sub_2BD0
 * emit_call_ncsv
 *********************************************************/
void prFrameHead(int fId) {
    static bool frameGlobalsEmitted; /* First call ncsv */

    prPsect(P_TEXT);
    if (frameGlobalsEmitted == false) /* if first call	*/
        printf("global\tncsv, cret, indir\n");

    printf("call\tncsv\n");
    frameGlobalsEmitted = true; /* first call is done	*/
    printf("defw\tf%d\n", fId);
}

/*********************************************************
 * sub_BCC OK++ PMO		       Used in: leaveBlock
 *********************************************************/
void prFrameTail(int fId, int fSize) {

    prPsect(P_TEXT);
    printf("jp\tcret\n"
           "f%d\tequ\t%d\n",
           fId, fSize);
}

/*********************************************************
 * sub_BEE OK++ PMO		       Used in: leaveBlock
 *********************************************************/
void sub_BEE(int kId, int size) {

    prPsect(P_TEXT);
    printf("ld\tde,k%d\n"
           "ld\tbc,%d\n"
           "ldir\n"
           "ld\thl,k%d\n",
           kId, size, kId);
    prPsect(P_BSS);
    printf("k%d:defs\t%d\n", kId, size);
}

/*********************************************************
 * sub_C36 OK++ PMO    Used in: sub_C57, sub_CAB, sub_4192
 *********************************************************/
void prGlobalDef(register member_t *sb) {

    GetString(tmpBuf, /* sb->b_name */ (char*)GetWord((char*)sb+OFF_b_name));
    printf("global\t%s\n", tmpBuf /* sb->b_name */);
    /* sb->b_sloc |= 2; */
    PutByte((char*)sb+OFF_b_sloc, GetByte((char*)sb+OFF_b_sloc) | 2);
}

/*********************************************************
 * sub_C57 OK++ PMO			Used in: sub_5CF5
 *********************************************************/
void sub_C57(member_t *sb_arg) {
    member_t buf;
    register member_t* sb = &buf;

    ReadMem((char*)&buf, (char*)sb_arg, sizeof(struct member));

    if ((sb->b_sloc & 1) == 0 && sb->b_nelem != 0 && (sb->b_refl & 2) == 0) {
        prPsect(P_BSS);
        if (sb->b_flag == 3 && (sb->b_sloc & 2) == 0)
            prGlobalDef(sb_arg); /* Emit "global name" */
        sub_CAB(sb_arg);         /* Emit "symbolic_name:" (identifier label) */
        printf("\tdefs\t%u\n", sb->b_size);
    }
}

/*********************************************************
 * sub_CAB OK++ PMO		Used in: sub_B19, sub_C57
 *
 * Emit "symbolic_name:" (identifier label)
 *********************************************************/
void sub_CAB(member_t *sb_arg) {
    member_t buf;
    register member_t* sb = &buf;

    ReadMem((char*)&buf, (char*)sb_arg, sizeof(struct member));

    if ((sb->b_sloc & 2) == 0 && sb->b_flag == 3)
        prGlobalDef(sb_arg); /* Emit "global name" */
    /* sb->b_sloc |= 1; */
    PutByte((char*)sb_arg+OFF_b_sloc, GetByte((char*)sb_arg+OFF_b_sloc) | 1);
    GetString(tmpBuf, sb->b_name);
    printf("%s:\n", tmpBuf /* sb->b_name */);
}

/*********************************************************
 * sub_CDF OK++ PMO			Used in: sub_808
 * Emit "defb 0, ..." (num bytes)
 *********************************************************/
void prDefb0s(int num) {
    char cnt;

    cnt = 0;             /* Reset counter bytes printed	       */
    while (num-- != 0) { /* While data is available	       */
        if (cnt == 0)
            printf("defb\t0"); /* Initially output "defb 0",   */
        else
            printf(",0");   /* later ",0"		       */
        cnt++;              /* and update number bytes output  */
        if (cnt == 16) {    /* If number bytes in string is 16 */
            cnt = 0;        /* Reset counter and	       */
            fputchar('\n'); /* continue output in next line    */
        }
    } /* continue processing */
    if (cnt != 0)
        fputchar('\n'); /* If line is incomplete, new line */
}

/*********************************************************
 * prPsect OK++ PMO  Used in: sub_6D1,  sub_793,  sub_B19,
 * 			      prFrameHead,  prFrameTail,  sub_BEE,
 * Select psect		      sub_C57,  sub_E43
 *********************************************************/
void prPsect(int section) {
    static int curPsect;
    static char *psectNames[] = { "", "bss", "text", "data" };

    if (section != curPsect) /* Only when changing section */
        printf("psect\t%s\n", psectNames[curPsect = section]);
}

/*********************************************************
 * sub_D66 OK++ PMO			Used in: sub_E43
 *
 *********************************************************/
void sortCaseLabels(int *pCase, int *pLabel, int nCase) {
    bool changed;
    int *pl;
    int cnt;
    int tCase;
    int tLabel;
    register int *pc;

    do {
        changed = false;
        pc      = pCase;
        pl      = pLabel;
        for (cnt = nCase; --cnt > 0; pc++, pl++) {
            if (pc[1] < pc[0]) {
                changed = true;
                tCase   = pc[0];
                pc[0]   = pc[1];
                pc[1]   = tCase;
                tLabel  = pl[0];
                pl[0]   = pl[1];
                pl[1]   = tLabel;
            } else if (pc[0] == pc[1]) {
                prError("Duplicate case label");
                return;
            }
        }
    } while (changed != 0);
    return;
}

/*********************************************************
 * sub_E43 OK++	PMO		 	Used in: sub_6D1
 * Only minor code differences due to the declaration of
 * functions with char / uint8_t parameters rather than
 * defaulting to int.
 * One other change is where the compiler saves l to the high
 * and low byte, but as the optimiser detected that h and l
 * were both 0 this is not a problem
 *********************************************************/
void parseSwitch() {
    node_t *l1a;
    int codeLabel, swTableLabel, caseRange, caseCnt, defaultCodeLabel;
    int16_t l7, l8;
    bool loTest, hiTest;
    register node_t *sa;
    int caseVals[255], codeLabels[255];

    l1a     = parseExpr();
    caseCnt = 0;
    for (;;) {
        sa        = sub_600E(parseExpr());
        codeLabel = atoi(getToken());
        if (sa->a_c0 == DOT_DOT) { /* end of switch block */
            defaultCodeLabel = codeLabel;
            freeNode(sa);
            expect(']');
            if (caseCnt != 0)
                break;
            prWarning("No case\tlabels");
            sub_475C(l1a);
            printf("jp\tl%d\n", defaultCodeLabel);
            return;
        }
        if (sa->a_c0 == CONST) { /* only constants allowed */
            caseVals[caseCnt]     = sa->info.l;
            codeLabels[caseCnt++] = codeLabel;
        } else
            prError("Non-constant case label");
        sub_475C(sa);
    }
    sortCaseLabels(caseVals, codeLabels, caseCnt); /* m6:  */
    caseRange = caseVals[caseCnt - 1] - caseVals[0];
    if (0 <= caseRange && caseRange < 16000 &&
        caseRange * 2 + 20 < caseCnt * 5) { /* if jmp table is shorter */
        /* jump table option is smaller so use it
           note the alternative cmp/jp option may be slower
           even if it is shorter
        */
        sa        = allocNode();
        sa->a_c0  = TYPE;
        sa->pm    = l1a->pm;
        sa->a_i15 = l1a->a_i15;
        sa        = sub_43EF(CONV, sub_415E(caseVals[0]), sa);
        sa        = sub_43EF(SUB, l1a, sa);
        if (nodesize(sa) != 2) {
            l1a       = allocNode();
            l1a->a_c0 = TYPE;
            l1a->pm   = sub_265("us");
            sa        = sub_43EF(CONV, sa, l1a);
        }
        prPsect(P_TEXT);
        sub_3DC9(sub_43EF(RPAREN, sa, 0));
        swTableLabel = newLocal(); /* swTableCnt++ */
        /*
            with the switch value in hl, the code
            emitted is effectively
            ld  a,.high. caseRange
            cp  h
            jp  c,l{defaultCodeLabel}
            jp  nz,1f
            ld  a,.low. caseRange
            cp  l
            jp  c,l{defaultCodeLabel}
         1: add hl,hl
            ld  de,S{swTableLabel}
            ld  a,(hl)
            inc hl
            ld  h,(hl)
            ld  l,a
            jp  (hl)
            where {xxx} indicates the respective label number of xxx
        */
        printf("ld\ta,%u\n"
               "cp\th\n"
               "jp\tc,l%d\n"
               "jp\tnz,1f\n"
               "ld\ta,%u\n"
               "cp\tl\n"
               "jp\tc,l%d\n"
               "1:add\thl,hl\n"
               "ld\tde,S%u\n"
               "add\thl,de\n"
               "ld\ta,(hl)\n"
               "inc\thl\n"
               "ld\th,(hl)\n"
               "ld\tl,a\n"
               "jp\t(hl)\n",
               caseRange >> 8, defaultCodeLabel, caseRange & 0xff, defaultCodeLabel, swTableLabel);
        prPsect(P_DATA);
        printf("S%d:\n", swTableLabel);
        codeLabel = 0;
        caseRange = caseVals[0];
        do {
            if (caseRange == caseVals[codeLabel]) {
                printf("defw\tl%d\n", codeLabels[codeLabel]);
                codeLabel++;
            } else
                printf("defw\tl%d\n", defaultCodeLabel); /* fill holes with default */

            caseRange++;
        } while (codeLabel < caseCnt);
        return;
    }
    if (nodesize(l1a) == 2) {
        prPsect(P_TEXT);
        sub_3DC9(sub_43EF(RPAREN, l1a, 0));
        l7 = -1;
        l8 = (int)(loTest = hiTest = false);

        for (codeLabel = 0; codeLabel < caseCnt; codeLabel++) {
            l7 &= caseVals[codeLabel];
            l8 |= caseVals[codeLabel];
            if (l7 >> 8 != l8 >> 8 && (l7 & 0xFF) != (l8 & 0xFF)) {
                if (hiTest | loTest)
                    printf("1:\n"); /* target of previous hi/lo test */
                loTest = hiTest = false;
                l7 = l8 = caseVals[codeLabel];
            }
            if (l7 >> 8 == l8 >> 8) {
                if (!hiTest) {
                    printf("ld\ta,h\n");
                    sub_1420(l7 >> 8);
                    printf("jp\tnz,1f\n"
                           "ld\ta,l\n");
                    hiTest = true;
                }
                caseVals[codeLabel] &= 0xFF;
            } else {
                if (!loTest) { /* m16: */
                    if (hiTest)
                        printf("1:\n");
                    printf("ld\ta,l\n"); /* m17: */
                    sub_1420(l7 & 0xFF);
                    printf("jp\tnz,1f\n"
                           "ld\ta,h\n");
                    loTest = true;
                }
                caseVals[codeLabel] >>= 8;
                caseVals[codeLabel] &= 0xFF;
            }
            sub_1420(caseVals[codeLabel]); /* m19: */
            printf("jp\tz,l%d\n", codeLabels[codeLabel]);
        }
        if (hiTest | loTest)
            printf("1:\n");
        printf("jp\tl%d\n", defaultCodeLabel); /* m21: */
        return;
    }

    prPsect(P_TEXT);
    sub_3DC9(sub_43EF(INAREG, l1a, 0));
    for (codeLabel = 0; codeLabel < caseCnt; codeLabel++) {
        if (caseVals[codeLabel] < 256 && caseVals[codeLabel] >= -128) {
            sub_1420(caseVals[codeLabel]);
            printf("jp\tz,l%d\n", codeLabels[codeLabel]);
        }
    }
    printf("jp\tl%d\n", defaultCodeLabel);
}

/*********************************************************
 * sub_1420 OK++ PMO			 Used in: sub_E43
 *********************************************************/
void sub_1420(int par) {

    printf((uint16_t)par ? "cp\t%d\n" : "or\ta\n", par);
}

/*********************************************************
 * sub_143F OK++ PMO			Used in: sub_35E6
 *********************************************************/
void prPush(uint8_t reg) {

    if (reg == REG_DEHL) {
        printf("push\thl\n"
               "push\tde\n");
        return;
    }
    if (reg <= REG_H)
        reg = reg / 2 + REG_AF;
    printf("push\t%s\n", regNames[reg]);
}

/*********************************************************
 * sub_1489 OK++ PMO			Used in: sub_35E6
 *********************************************************/
void prPop(uint8_t reg) {

    if (reg == REG_DEHL) {
        printf("pop\tde\n"
               "pop\thl\n");
        return;
    }
    if (reg <= REG_H)
        reg = reg / 2 + REG_AF;
    printf("pop\t%s\n", regNames[reg]);
}

/*********************************************************
 * sub_14D3 OK++ PMO			Used in: sub_1EDF
 *
 * Assigning register "IY" value formal parameter with
 * type register
 *********************************************************/
void prIXnPush(register member_t *sb) {
    int off = GetWord((char*)sb+OFF_b_off);

    printf("ld\tl,(ix+%d)\n"
           "ld\th,(ix+%d)\n"
           "push\thl\n"
           "pop\tiy\n",
           /* sb->b_off */ off, /* sb->b_off */ off + 1);
}

/*********************************************************
 * sub_14F3  PMO   Used in: sub_153A, sub_2D09, sub_39CA,
 * OK++			    sub_3A79, sub_3EAA, sub_47B2,
 *			    sub_4FCE, sub_508A, sub_54B6,
 *			    sub_60A8, sub_6246, sub_628F
 *********************************************************/
uint8_t sub_14F3(register node_t *sa) {
    member_t buf;
    member_t* p = &buf;

    if (sa->a_i15 & 2)
        return 0;
    if (sa->a_i15 != 0)
        return 2;

    ReadMem((char*)&buf, (char*)(sa->pm), sizeof(struct member));

    if (/* sa->pm->b_nelem */ p->b_nelem > 1)
        return 0;
    return /* sa->pm->b_flag */ p->b_flag;
}

/*********************************************************
 * sub_153A OK++ PMO			Used in: sub_2D09
 *********************************************************/
void sub_153A(register node_t *sa) {

    static char array_A542[] = { 0, 'a', 'l', 'f' };

    fputchar(array_A542[sub_14F3(sa)]);
}

/*********************************************************
 * sub_155D OK++ PMO			 Used in: sub_793
 *
 * Emit "defb byte1, ..." (from ptr num bytes)
 *********************************************************/
void prDefb(register char *ptr, int num) {
    char cnt;

    cnt = 0;             		/* Reset counter bytes printed	   */
    while (num-- != 0) { 		/* While data is available	   */
        if (cnt == 0)
            printf("defb\t"); 		/* Initially output "defb",	   */
        else
            fputchar(',');             	/* later "," 			   */
        printf("%d", (uint8_t)*ptr++); 	/* Output byte and advance pointer */
        cnt++;                         	/* update number bytes output 	   */
        if (cnt == 16) {               	/* max 16 bytes per line 	   */
            fputchar('\n');
            cnt = 0;
        }
    }
    if (cnt != 0)
        fputchar('\n'); /* If line is incomplete, new line */
}

/*********************************************************
 * xx1	 OK++ PMO	Used in: Explicit calls are absent
 *********************************************************/
void prJmpLabel(int p) {
    printf("j%d:\n", p);
}

/*********************************************************
 * xx2	 OK++ PMO	Used in: Explicit calls are absent
 *********************************************************/
void prJump(int p) {
    printf("jp\tj%d\n", p);
}

/*********************************************************
 * sub_15FB OK++ PMO			Used in: sub_1B0C
 *********************************************************/
void sub_15FB(register member_t *sb, int p2, int p3) {
    bool f = (p2 >= 0);

    PutByte((char*)sb+OFF_b_flag, f ? 2 : 1);

    PutWord((char*)sb+OFF_b_size, f ? ( (p3 <= 255) ? 1 : 2 ) : ( (p3 <= 127 && p2 >= -128) ? 1 : 2 ) );
}

/*********************************************************
 * sub_1659 OK++ PMO			Used in: sub_1CEF
 * Find maximum between two numbers
 *********************************************************/
int max(int num1, int num2) {
    return (num1 > num2) ? num1 : num2;
}

#define NVARS 14

struct type {
    char *t_str;
    int t_size;
    int t_alig;
    char t_flag;
};

/*********************************************************
 * sub_1680 OK++ PMO			    Used in: main
 *
 * first_init
 *********************************************************/
void sub_1680() {
    member_t *sb;
    int16_t cnt;
    register struct type *tp;

    /*
     *	Initializaion of type pointers
     */
    static struct type vars[NVARS] = { /* sub_1680 */
                                       { "i", 2, 0, 1 },  { "s", 2, 0, 1 },  { "c", 1, 0, 1 },
                                       { "l", 4, 0, 1 },  { "ui", 2, 0, 2 }, { "us", 2, 0, 2 },
                                       { "uc", 1, 0, 2 }, { "ul", 4, 0, 2 }, { "f", 4, 0, 3 },
                                       { "d", 4, 0, 3 },  { "x", 2, 0, 1 },  { "ux", 2, 0, 2 },
                                       { "b", 0, 0, 0 },  { "v", 0, 0, 0 }
    };

    /* Clear hash table */

    blkclr((char *)hashtab, sizeof(hashtab));

    /* Create a hash table of templates for standard types */

    cnt = NVARS;
    tp  = vars;
    do {
        sb          = sub_265(tp->t_str);
        /* sb->b_class = TYPE; */
	PutByte((char*)sb+OFF_b_class, TYPE);
        /* sb->b_off   = tp->t_alig; */
	PutWord((char*)sb+OFF_b_off, tp->t_alig);
        /* sb->b_size  = tp->t_size; */
	PutWord((char*)sb+OFF_b_size, tp->t_size);
        /* sb->b_flag  = tp->t_flag; */
        PutByte((char*)sb+OFF_b_flag, tp->t_flag); 
        tp++;
    } while (--cnt != 0);

    /* Additional patterns for types */

    typeLong   = sub_265("l");  /* long	  	*/
    typeDouble = sub_265("d");  /* double 	*/
    typeB      = sub_265("b");  /* b   	  	*/
    typeVar    = sub_265("v");  /* variable 	*/
    typeChar   = sub_265("c");  /* char   	*/
    typeUChar  = sub_265("uc"); /* uint8_t  	*/
    typeX      = sub_265("x");  /* x      	*/
    lvlidx     = -1;
}

/*********************************************************
 * sub_174C OK++ PMO    Used in: sub_E43,  sub_17E0, sub_39CA,
 *			     sub_4192, sub_508A
 *********************************************************/
int newLocal() {
    static int localLabelCnt;
    return ++localLabelCnt;
}

/*********************************************************
 * sub_1754 OK++ PMO    Used in: sub_17E0, sub_19C1, sub_1B0C
 *********************************************************/
member_t *sub_1754(char *token, uint8_t cls) {
    member_t **l1b;
    register member_t *sb;
    char* p;

    sb = sub_265(token);                          /* Set pointer to struct associated with   */
                                                  /* pointer to token.			     */
    if (/* sb->b_ndpth */ GetByte((char*)sb+OFF_b_ndpth) != nstdpth) {                 /* If nesting depth not correspond current,*/
        l1b        = gethashptr(token);           /* Get pointer to pointer to struct from   */
        sb         = /* allocMem(sizeof(member_t)) */ (member_t*)MyAllocMbr();  /* hash table. Create new struct and save  */
        /*sb->b_next = *l1b; */                        /* pointer to struct from hash table in    */
	PutWord((char*)sb+OFF_b_next, (short)*l1b);
                                                  /* struct as associated with it. 	     */
        *l1b = sb;                                /* Save pointer to this struct in current  */
                                                  /* position hash table.		     */
        /* sb->b_name = (char*)MyAllocStr(strlen(token) + 1); */ /* allocMem(strlen(token) + 1); */ 
	PutWord((char*)sb+OFF_b_name, (short)(p = (char*)MyAllocStr(strlen(token) + 1)));
						/* Get memory address allocated for token  */
                                                  /* and assign it to corresponding member.  */
        PutString(token, /* sb->b_name */ p); /* strcpy(sb->b_name, token); */                
						/* Copy specified token to this address.   */
        /* sb->b_ndpth = (uint8_t)nstdpth; */           /* Save current nesting depth in struct    */
	PutByte((char*)sb+OFF_b_ndpth, nstdpth);
                                                  /*
                                                        if(++symcnt > dynmem)   dynmem = symcnt;
                                                   */
    }
    /* sb->b_class = cls; */
    PutByte((char*)sb+OFF_b_class, cls);

    return sb;
}
/* end of code.c */
