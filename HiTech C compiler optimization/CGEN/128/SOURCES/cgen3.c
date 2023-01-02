/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File - cgen3.c Created 09.03.2019 Last Modified 30.05.2020
 */

#include "cgen.h"
#include "alloc128.h"

/*********************************************************
 * sub_3564 OK++ PMO			Used in: sub_35E6
 * minor optimisation difference due to last parameter
 * of sub_2D09 being declared as char
 *********************************************************/
void sub_3564(register node_t *sa) {
    char l1;
    struct codeFrag_t *l2z;

    word_AFF8 = 0;
    for (l1 = 0; l1 != sa->a_c1; l1++) {
        if ((l2z = &codeFrag[sa->a_c3[l1]])->p_6) {
            sub_2D09(sa, l2z->p_6, l1);
            fputchar('\n');
        }
    }
}

/*********************************************************
 * sub_35E6 OK++ PMO			Used in: sub_2BD0
 *
 *********************************************************/
void sub_35E6(register node_t *sa) {
    int l1;

    if (dopetab[sa->a_c0] & 0x200)
        word_AFFB++;
    l1 = dopetab[sa->a_c0] & 0xC;
    if (8 == l1) {
        if (sa->a_c2 & 1) {
            sub_35E6(sa->info.np[0]);
            sub_35E6(sa->info.np[1]);
        } else {
            sub_35E6(sa->info.np[1]);
            if (sa->a_c2 & 2)
                prPush(sa->info.np[1]->a_reg[sa->info.np[1]->a_c1 - 1]);
            sub_35E6(sa->info.np[0]);
        }
    }
    if (l1 == 4)
        sub_35E6(sa->info.np[0]);
    if (sa->a_c2 & 2)
        prPop(sa->info.np[1]->a_reg[sa->info.np[1]->a_c1 - 1]);
    sub_3564(sa);
    if (dopetab[sa->a_c0] & 0x200)
        word_AFFB--;
}

/*********************************************************
 * sub_36E0 PMO    Used in: sub_E43,  sub_39CA, sub_3CDF,
 * OK++ 		    sub_3DC9, sub_3EAA, sub_43EF,
 *			    sub_475C, sub_4C8B, sub_4FA8,
 *			    sub_508A, sub_53EE, sub_54B6,
 *			    sub_5DF6, sub_5F52
 *********************************************************/
void freeNode(register node_t *sa) {

    byte_B013 = true;
    if (sa->a_c0 == FCONST)
        free(sa->info.np[0]);
    sa->pm       = nodeFreeList;
    nodeFreeList = sa;
}

/*********************************************************
 * sub_3712  PMO    Used in: sub_E43,  sub_377A, sub_415E,
 * OK++			     sub_4192, sub_43EF, sub_4DA3,
 *			     sub_54B6
 *********************************************************/
node_t *allocNode(void) {
    register node_t *sa;

    byte_B013 = true;
    if (nodeFreeList) {
        sa           = nodeFreeList;
        nodeFreeList = sa->pm;
        blkclr((char *)sa, sizeof(node_t));
    } else { /* create node_t */
        sa = allocMem(sizeof(node_t));
    }
    return sa;
}

/*********************************************************
 *  sub_374C OK++ PMO			Used in: allocMem
 *********************************************************/
bool releaseNodeFreeList(void) {
    register node_t *sa;

    if (nodeFreeList == 0)
        return false;
    while ((sa = nodeFreeList)) {
        nodeFreeList = sa->pm;
        free(sa);
    }
    return true;
}

/*********************************************************
 *  sub_377A OK++ PMO			Used in: sub_3CDF
 *********************************************************/
node_t *sub_377A(node_t *p1a) {
    register node_t *sa;

    sa  = allocNode();
    *sa = *p1a;

    if (dopetab[sa->a_c0] & 0xC)
        sa->info.np[0] = sub_377A(sa->info.np[0]);
    if ((dopetab[sa->a_c0] & 0xC) == 8)
        sa->info.np[1] = sub_377A(sa->info.np[1]);
    return sa;
}

/*********************************************************
 * sub_37ED v1 OK++ OK	      Used in: sub_387A, sub_43EF
 *
 * missed optimisation on reloading l1b but otherwise
 * identical
 *********************************************************/
void sub_37ED(register node_t *sa) {
    member_t *l1b;
    member_t buf;
    member_t* p = &buf;

    l1b = sa->pm;

    if (sa->a_i15 != 0)
        sa->a_i15 >>= 2;
    else {
        while (/* l1b->b_refl */ GetWord((char*)l1b+OFF_b_refl) == 0)
            l1b = /* l1b->b_type */ (member_t*)GetWord((char*)l1b+OFF_b_type);

        ReadMem((char*)&buf, (char*)l1b, sizeof(struct member));

        sa->pm    = /* l1b->b_type */ p->b_type;
        sa->a_i15 = /* l1b->b_refl */ p->b_refl >> 2;
    }
}

/*********************************************************
 * sub_385B  PMO    Used in: sub_387A, sub_43EF, sub_4DA3,
 * OK++			     sub_4E8D
 *********************************************************/
void sub_385B(register node_t *sa) {
    sa->a_i15 = (sa->a_i15 * 4) | 1;
}

/*********************************************************
 * sub_387A OK++ PMO			Used in: sub_2D09
 *********************************************************/
uint16_t sub_387A(register node_t *sa) {
    uint8_t loc;
    member_t buf;
    member_t* p = &buf;

    ReadMem((char*)&buf, (char*)(sa->pm), sizeof(struct member));

    if (sa->a_i15 == 0 && 
	/* sa->pm->b_type */ p->b_type == 0 && 
	/* sa->pm->b_refl */ p->b_refl == 0)
        return 1;

    sub_37ED(sa);
    loc = (uint8_t)nodesize(sa);
    sub_385B(sa);
    return loc;
}

/*********************************************************
 * sub_38CA OK++ PMO	      Used in: sub_393C, sub_3952
 * minor difference as function declared as bool
 *********************************************************/
bool sub_38CA(node_t *p1a, int p2) {
    register member_t *sb;
    member_t buf;
    member_t* p = &buf;

    if (p1a->a_i15 & p2)
        return true;

    if (p1a->a_i15 != 0)
        return false;

    sb = p1a->pm;

    do 
    {
        ReadMem((char*)&buf, (char*)sb, sizeof(struct member));

        if (/* sb->b_refl */ p->b_refl & p2)
            return true;

        if (/* sb->b_refl */ p->b_refl != 0)
            return false;

    } while ((sb = /* sb->b_type */ p->b_type) != 0);

    return false;
}

/*********************************************************
 * sub_393C OK++ PMO			Used in: Not used
 *********************************************************/
bool sub_393C(node_t *sa) {
    return sub_38CA(sa, 1);
}

/*********************************************************
 * sub_3952 OK++ PMO			Used in: Not used
 *********************************************************/
bool sub_3952(node_t *sa) {
    return sub_38CA(sa, 2);
}

/*********************************************************
 * sub_3968 OK++ PMO			Used in: sub_47B2
 *********************************************************/
bool sub_3968(register node_t *sa) {
    return /* sa->pm->b_class */ GetByte((char*)(sa->pm)+OFF_b_class) == STRUCT && (sa->a_i15 & 1) == 0;
}

/*********************************************************
 * nodesize  PMO    Used in: sub_808,  sub_E43,  sub_2C5E,
 * OK++			     sub_2D09, sub_387A, sub_3A79,
 *			     sub_43EF, sub_47B2, sub_508A,
 *			     sub_54B6, sub_60A8, sub_6246,
 *			     sub_62BE
 *********************************************************/
uint16_t nodesize(register node_t *sa) {

    if (sa->a_i15 & 2)
        prError("can\'t take sizeof func");

    if (sa->a_i15 & 1)
        return 2;

    return /* sa->pm->b_size */ GetWord((char*)(sa->pm)+OFF_b_size);
}

/*********************************************************
 * sub_39CA OK++ PMO			Used in: sub_3EAA
 *********************************************************/
node_t *sub_39CA(register node_t *sa) {
    char buf[50];

    freeNode(sa->info.np[1]);
    if (sub_14F3(sa) == 3) {
        sprintf(buf, "%ld.", sa->info.np[0]->info.l);
        freeNode(sa->info.np[0]);
        sa->info.sv.s = (char*)MyAllocStr(strlen(buf) + 1); /* allocMem(strlen(buf) + 1); */
						 /* create string */
        PutString(buf, sa->info.sv.s); /* strcpy(sa->info.sv.s, buf); */
        sa->info.sv.v = newLocal();
        sa->a_c0      = FCONST;
        return sa;
    }
    sub_3A79(sa, sa->info.np[0]);
    freeNode(sa);
    return sa->info.np[0];
}

/*********************************************************
 * sub_3A79 OK++ PMO	       Used in: sub_3EAA, sub_39CA
 *
 * Compiler generates code that performs expected action,
 * but differs from original image
 *********************************************************/
void sub_3A79(register node_t *sa, node_t *p2a) {
    int l1;

    l1 = nodesize(sa);
    if (l1 != 0 && l1 < 4) {
        l1 <<= 3;
        p2a->info.l &= (1L << l1) - 1L;
        if (sub_14F3(sa) == 1) {
            if (p2a->info.l & (long)(1 << (l1 - 1))) { /* sign extend */
                p2a->info.l |= ~((1L << l1) - 1L);
            }
        }
    }
    p2a->pm    = sa->pm;
    p2a->a_i15 = sa->a_i15;
}

/*********************************************************
 * sub_3B65 OK++ PMO			Used in: sub_3EAA
 *********************************************************/
void sub_3B65(register unsigned long *ptr, long p2, int operator) {

    switch (operator) {
    case DIV:
        *ptr /= p2;
        break;
    case MOD:
        *ptr %= p2;
        break;

    case RSHIFT:
        *ptr >>= p2;
        break;
    }
}

/*********************************************************
 * sub_3BC6 v1 OK++ PMO			Used in: sub_3EAA
 *
 *********************************************************/
void sub_3BC6(register long *ptr, long p2, int operator) {

    switch (operator) {
    case ADD:
        *ptr += p2;
        break;
    case SUB:
        *ptr -= p2;
        break;
    case MUL:
        *ptr *= p2;
        break;
    case DIV:
        *ptr /= p2;
        break;
    case MOD:
        *ptr %= p2;
        break;
    case BOR:
        *ptr |= p2;
        break;
    case BAND:
        *ptr &= p2;
        break;
    case BXOR:
        *ptr ^= p2;
        break;
    case LSHIFT:
        *ptr <<= p2;
        break;
    case RSHIFT:
        *ptr >>= p2;
        break;
    }
}

/*********************************************************
 * sub_3CDF OK++ PMO			Used in: sub_3DC9
 *********************************************************/
node_t *sub_3CDF(register node_t *sa) {
    int loc;

    if (sa->a_c0 == SCOLON) {
        sub_2BD0(sa->info.np[0]);
        freeNode(sa);
        sa = sa->info.np[1];
    }
    if (byte_B014 != 10) {
        if (dopetab[sa->a_c0] & 0x4000)
            return sa;
        if ((sa->a_c0 == INCR || sa->a_c0 == DECR) && sub_4C6D(sa)) {
            array_AFFD[byte_B014++] = sa;
            sa->a_c0                = sa->a_c0 == INCR ? ASADD : ASSUB;
            sa                      = sub_377A(sa->info.np[0]);
        }
        loc = dopetab[sa->a_c0];
        if (loc & 0xC)
            sa->info.np[0] = sub_3CDF(sa->info.np[0]);
        if ((loc & 0xC) == 8)
            sa->info.np[1] = sub_3CDF(sa->info.np[1]);
    }
    return sa;
}

/* end of file cgen3.c */
