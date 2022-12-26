/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File - local2.c
 */

#define MINI
#include "cgen.h"

void       sub_C57(member_t *);
uint8_t    sub_14F3(node_t *);
node_t    *sub_3EAA(node_t *);
node_t    *sub_43EF(uint8_t, node_t *, node_t *);
node_t    *sub_54B6(node_t *);
node_t    *sub_5DF6(node_t *);
node_t    *sub_5F52(node_t *);
bool       sub_61AA(member_t *, int);
bool       sub_628F(node_t *, node_t *);
uint8_t    sub_6589(int, int);
uint16_t   sub_665B(uint16_t, uint8_t);
void       freeNode(node_t *);
void *allocMem(size_t);	
void prWarning(char *fmt, ...);
uint16_t   nodesize(node_t *);

#include "alloc128.h"

/*********************************************************
 * sub_5CF5 OK++ PMO	    Used in: leaveBlock, sub_17E0
 *
 *********************************************************/
void sub_5CF5(member_t *sb_arg, int p2) {
    member_t buf;
    register member_t* sb = &buf;
    char tmp;

    ReadMem((char*)&buf, (char*)sb_arg, sizeof(struct member));

    switch (sb->b_flag) {
    case 1:
        if (sub_61AA(sb_arg, p2) == 0) {
            array_AE13[lvlidx] -= sb->b_size;
	    PutWord((char*)sb_arg+OFF_b_off, array_AE13[lvlidx]);
            if ((/* sb->b_off = */ array_AE13[lvlidx]) < -128 && sb->b_size <= 4)
	    {
		GetString(tmpBuf, sb->b_name);
                prWarning("%s: large offset", tmpBuf /* sb->b_name */);
	    }
        }
        break;
    case 5:
        /* sb->b_off = array_AE97[lvlidx]; */
	PutWord((char*)sb_arg+OFF_b_off, array_AE97[lvlidx]);
        array_AE97[lvlidx] += sb->b_size;
        if (sb->b_size == 1 && 
	    /* sb->b_type->b_class */ (tmp = GetByte((char*)(sb->b_type)+OFF_b_class)) != STRUCT && 
	    /* sb->b_type->b_class */ tmp != UNION)
            array_AE97[lvlidx]++;

        if (!sub_61AA(sb_arg, p2))
            /* sb->b_flag = 1; */
	    PutByte((char*)sb_arg+OFF_b_flag, 1);
        break;
    case 3:
    case 4:
        sub_C57(sb_arg);
        break;
    }
}

/*********************************************************
 * sub_5DF6 OK++ PMO			Used in: sub_600E
 * parameter 1 of sub_43EF optimised
 *********************************************************/
node_t *sub_5DF6(register node_t *sa) {
    char l1;
    char *l2;
    char tmpBufAux[MAXBUF]; 

    l1 = dopetab[sa->a_c0] & 0xC;
    if (l1 == 8)
        sa->info.np[1] = sub_5DF6(sa->info.np[1]);
    if (l1 != 0)
        sa->info.np[0] = sub_5DF6(sa->info.np[0]);

    switch (sa->a_c0) {
    case SUB:
    case ASSUB:
        if (sa->info.np[1]->a_c0 == CONST)
            sa->info.np[1]->info.l = -sa->info.np[1]->info.l;
        else
            sa->info.np[1] = sub_43EF(MINUS_U, sa->info.np[1], 0);
        sa->a_c0 = sa->a_c0 == SUB ? ADD : ASADD;
        break;
    case MINUS_U:
        if (sa->info.np[0]->a_c0 == MINUS_U) {
            freeNode(sa);
            sa = sa->info.np[0];
            freeNode(sa);
            sa = sa->info.np[0];
        }
        if (sa->info.np[0]->a_c0 == FCONST) {
            freeNode(sa);
            sa = sa->info.np[0];
            l2 = MyAllocStr(strlen(sa->info.sv.s) + 2); 
			/* allocMem(strlen(sa->info.sv.s) + 2); */
	    GetString(tmpBufAux, sa->info.sv.s);
	    strcpy(tmpBuf, "-");
	    strcat(tmpBuf, tmpBufAux);
	    PutString(tmpBuf, l2);
            		/* strcat(strcpy(l2, "-"), sa->info.sv.s); */

            /* free(sa->info.sv.s); */
            sa->info.sv.s = l2;
        }
    }
    return sa;
}

/*********************************************************
 * sub_5F52 OK++ PMO			Used in: sub_600E
 *********************************************************/
node_t *sub_5F52(register node_t *sa) {
    char l1;
    node_t *l2;

    l1 = dopetab[sa->a_c0] & 0xC;

    if (l1 == 8)
        sa->info.np[1] = sub_5F52(sa->info.np[1]);

    if (l1 != 0)
        sa->info.np[0] = sub_5F52(sa->info.np[0]);

    switch (sa->a_c0) {
    case ADD:
        if (sa->info.np[0]->a_c0 == MINUS_U) {
            l2             = sa->info.np[0];
            sa->info.np[0] = sa->info.np[1];
            sa->info.np[1] = l2;
        }
    case ASADD:
        if (sa->info.np[1]->a_c0 == MINUS_U) {
            freeNode(sa->info.np[1]);
            sa->info.np[1] = sa->info.np[1]->info.np[0];
            sa->a_c0       = sa->a_c0 == ASADD ? ASSUB : SUB;
        }
    }

    return sa;
}

/*********************************************************
 * sub_600E OK++ PMO Used in: sub_793,  sub_808,  sub_E43,
 *                            sub_17E0, sub_19C1, sub_1B0C,
 *			      sub_3DC9
 *********************************************************/
node_t *sub_600E(register node_t *sa) {

     /* int l1; Not used */

    warningMsg = 0;
    sa         = sub_5DF6(sa);

    /* l1         = 0; Not used */
    do {
        byte_B013 = false;
        /*
              Excluded part optimization code
              present in the DOS version (3.06)
        */
        sa = sub_3EAA(sub_54B6(sa));
    } while (byte_B013);

    if (warningMsg)
        prWarning(warningMsg);

    return sub_5F52(sa);
}

/*********************************************************
 * sub_605E OK++ PMO			Used in: sub_4C8B
 *
 *********************************************************/

int invertTest(int op) {

#ifdef DEBUG
    printf("\tsub_605E(%d)\n", op);
#endif

    switch (op) {
    case EQL:
        return NEQL;
    case NEQL:
        break;
    case LT:
        return GEQ;
    case LEQ:
        return GT;
    case GEQ:
        return LT;
    case GT:
        return LEQ;
    }
    return EQL;
}

/*********************************************************
 * sub_60A8 OK++ PMO			Used in: sub_54B6
 *
 *********************************************************/
bool sub_60A8(register node_t *sa, node_t *p2a) {
    long l1;

    if (sa->a_c0 != 'C')
        return 0;
    if (nodesize(p2a) >= 4)
        return sa->info.l >= 0 || sub_14F3(p2a) == 1;

    l1 = (uint16_t)(1 << (nodesize(p2a) * 8));
    if (sub_14F3(p2a) == 2) {
        if (sa->info.l < 0)
            return 0;
    } else {
        l1 /= 2;
        if (sa->info.l < 0)
            return sa->info.l >= -l1;
    }
    return sa->info.l < l1;
}

/*********************************************************
 * sub_61AA OK++ PMO			Used in: sub_5CF5
 *********************************************************/
bool sub_61AA(member_t *sb_arg, int p2) {
    member_t buf;
    register member_t* sb = &buf;
    int16_t tmp;

    ReadMem((char*)&buf, (char*)sb_arg, sizeof(struct member));

    if (!rflag && isupper(p2) && sb->b_refl == 1 && sb->b_nelem <= 1) {
        if (sub_6589(word_B017, 9) == 0)
            return false;
        if (sb->b_flag == 5)
            /* sb->b_sloc |= 4; */
	    PutByte((char*)sb_arg+OFF_b_sloc, GetByte((char*)sb_arg+OFF_b_sloc) | 4);
        /* sb->b_flag   = 2; */
	PutByte((char*)sb_arg+OFF_b_flag, 2);
        /* sb->b_memb.i = sub_6589(word_B017, 9); */
	PutWord((char*)sb_arg+OFF_b_memb, tmp = sub_6589(word_B017, 9));
        word_B017 &= ~array_AAE8[/* sb->b_memb.i */ tmp];
        return true;
    }

    return false;
}

/*********************************************************
 * sub_6246 OK++ PMO	      Used in: sub_508A, sub_54B6
 *********************************************************/
bool sub_6246(node_t *p1a, node_t *p2a) {

    return nodesize(p1a) == nodesize(p2a) && sub_14F3(p1a) == sub_14F3(p2a);
}

/*********************************************************
 * sub_628F OK++ PMO
 *********************************************************/
bool sub_628F(node_t *p1a, node_t *p2a) {
    return sub_14F3(p1a) == 1 && sub_14F3(p2a) == 1;
}

/*********************************************************
 * sub_62BE OK++ PMO			Used in: sub_54B6
 *********************************************************/
bool sub_62BE(register node_t *sa, node_t *p2a) {
    long l1, l2;
    char l3;

    l3 = nodesize(sa->info.np[0]) * 8;
    if (l3 >= 32)
        return true;
    l1 = 1L << l3;
    l2 = 0;
    if (sub_628F(sa, sa->info.np[0])) {
        l1 /= 2;
        l2 = -l1;
    }
    return l2 <= p2a->info.l && p2a->info.l < l1;
}

/* macros to make reading the code easier */
/* clang-format off */
/* don't reformat as hitech cpp includes the extra spaces
   which can cause line overflow
*/
#ifndef TOPBIT
#define TOPBIT 0x8000
#define TopBitSet(n) ((n)&TOPBIT)
#define MapVal(n) (TopBitSet(n)?~(n):array_AAE8[n])
#endif
/* clang-format on */
/*********************************************************
 * sub_63B8 OK++ PMO			Used in: sub_1F4B
 *
 *********************************************************/
uint8_t sub_63B8(int p1, int p2, int p3) {
    uint8_t l1;
    uint8_t l0;
    int l2;
    register uint8_t *st;

    p3 &= ~0x40;
    l1 = 0;

    if (p2 == 0)
        l1 = sub_6589(p1, p3);
    else if (p2 & 0x8000)
        l1 = sub_6589(MapVal(p2) & p1, p3);
    else if (p3 < 24) {
        if ((l1 = sub_6589(MapVal(p2) & p1, p3)) == 0 && (l2 = sub_6589(p1, p3)) != 0 &&
            sub_6589(MapVal(l2), p2) != 0)
            l1 = l2;
    } else {
        l2 = 6;
        st = &array_AB54[(p3 - 24) * 6];
        while (*st && l2-- != 0)
            if ((l0 = sub_6589(p1, *st++)) != 0 &&
                (sub_6589(array_AAE8[l0], p2) != 0 || sub_6589(array_AAE8[p2], l0) != 0)) {
                l1 = l0;
                break;
            }
    }
    return l1;
}

/*********************************************************
 * sub_6589 OK++ PMO Used in: sub_1F4B, sub_2B79, sub_61AA,
 *			      sub_63B8, sub_66BC
 * Optimiser improves on
 *      ld a,(ix-1) ! add a,255 ! ld (ix-1),a ! or a
 * with dec (ix-1)
 *********************************************************/
uint8_t sub_6589(int p1, int p2) {
    char l1;
    uint8_t *l2;

    p2 &= ~0x40; /* Clear bit 6 */
    if (p2 < 24)
        if ((p1 & array_AAE8[p2]) == array_AAE8[p2])
            return p2;
        else
            return 0;
    p2 -= 24;
    l1 = 6;
    l2 = &array_AB54[p2 * 6];
    do {
        if (*l2 != 0 && (p1 & array_AAE8[*l2]) == array_AAE8[*l2])
            return *l2;
    } while (*++l2 && --l1 != 0);
    return 0;
}

/*********************************************************
 * sub_665B OK++ PMO			Used in: sub_66BC
 *********************************************************/
uint16_t sub_665B(uint16_t p1, uint8_t p2) {
    uint16_t loc;

    if (p1 == 0)
        return 0;
    p2 -= 48;
    for (loc = 1; loc < 24; loc++)
        if (array_AB24[loc * 2 + p2] == p1)
            return loc;
    return 0;
}

/*********************************************************
 * sub_66BC OK++ PMO			Used in: sub_1F4B
 * Optimiser passes param 2 as uint8_t rather than promoting
 * to int
 *********************************************************/

int sub_66BC(int p1, int p2, int p3, char *p4) {
    int16_t l1, retval;
    char *l3;

    retval = 0;
    if ((p2 & 0x8000) != 0) {
        if (p1 == 0)
            retval = p2;
        else if ((l1 = sub_6589(((p2 & 0x8000) ? ~p2 : array_AAE8[p2]) & p3, p1)) != 0)
            retval = l1;
        else
            retval = sub_6589(p3, p1);
    } else if (p1 == 0) {
        if (p2 == 0 || p4 == 0)
            retval = p2;
        else {
            for (l3 = p4; isdigit(l3[1]); l3++)
                ;
            if (p2 >= 24 && (p2 = sub_6589(p3, p2)) == 0)
                retval = 0;
            else {
                do {
                    p2 = sub_665B(p2, (uint8_t)*l3);
                } while (l3-- != p4);
                retval = p2;
            }
        }
    } else if (p2 == 0)
        retval = p1;
    else {
        l1 = ((p1 & 0x8000) ? ~p1 : array_AAE8[p1]) & ((p2 & 0x8000) ? ~p2 : array_AAE8[p2]);
        if (((p1 & 0x8000) ? ~p1 : array_AAE8[p1]) == l1)
            retval = p1;
        else if (((p2 & 0x8000) ? ~p2 : array_AAE8[p2]) == l1)
            retval = p2;
        else
            retval = sub_6589(l1, p1);
    }
    return retval;
}
/* end of local2.c*/
