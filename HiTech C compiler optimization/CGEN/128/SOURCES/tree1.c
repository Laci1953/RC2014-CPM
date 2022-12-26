/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File - tree1.c Created 09.03.2019 Last Modified 17.06.2020
 */

#include "cgen.h"

/*********************************************************
 * sub_3DC9 OK++ PMO		Used in: sub_6D1, sub_D66
 *********************************************************/
void sub_3DC9(register node_t *sa) {
    uint16_t loc;

    sa = sub_600E(sa);
    if (sa->a_c0 == DOLLAR && sa->info.np[0]->a_c0 == CONST) {
        prWarning("constant conditional branch");
        if (sa->info.np[0]->info.l != 0) {
            sa->a_c0 = DOLLAR_U;
            freeNode(sa->info.np[0]);
            sa->info.np[0] = sa->info.np[1];
        } else {
            sub_475C(sa);
            return;
        }
    }
    byte_B014 = 0;
    if (sa->a_c0 == INCR)
        sa->a_c0 = ASADD;
    else if (sa->a_c0 == DECR)
        sa->a_c0 = ASSUB;

    sa = sub_3CDF(sa);
    if (byte_B014 != 0)
        sa = sub_600E(sa);
    sub_2BD0(sa);
    for (loc = 0; loc < byte_B014; loc++)
        sub_2BD0(array_AFFD[loc]);
}

/*********************************************************
 * sub_3EAA OK++ PMO			Used in: sub_600E
 *********************************************************/
node_t *sub_3EAA(register node_t *sa) {
    node_t *l1a;
    int l2;

    if (l2 = dopetab[sa->a_c0] & 0xC)
        sa->info.np[0] = sub_3EAA(sa->info.np[0]);
    if (l2 == 8)
        sa->info.np[1] = sub_3EAA(sa->info.np[1]);

    if (sa->a_c0 == CONV && sa->info.np[0]->a_c0 == FCONST && sub_14F3(sa) == 3) {
        l1a     = sa->info.np[0];
        l1a->pm = sa->pm;
        freeNode(sa->info.np[1]);
        freeNode(sa);
        return l1a;
    }
    if (l2 == 0 || sa->info.np[0]->a_c0 != CONST)
        return sa;

    if (sa->a_c0 == CONV)
        return sub_39CA(sa);

    if (l2 == 8 && sa->info.np[1]->a_c0 != CONST)
        return sa;

    if (sub_14F3(sa) == 2) {
        switch (sa->a_c0) {
        case LT:
        case LEQ:
        case GT:
        case GEQ:
            warningMsg = "constant relational expression";
            return sa;
        case MOD:
        case DIV:
        case RSHIFT:
            sub_3B65(&sa->info.np[0]->info.ul, sa->info.np[1]->info.l, sa->a_c0);
        finish:
            if (l2 == 8)
                sub_475C(sa->info.np[1]);
            sub_3A79(sa, sa->info.np[0]);
            freeNode(sa);
            return sa->info.np[0];
        }
    }
    switch (sa->a_c0) { /* m10: */
    case NEQL:
    case LT:
    case LEQ:
    case EQL:
    case GT:
    case GEQ:
        warningMsg = "constant relational expression";
        return sa;
    case BAND:
    case LAND:
    case SUB:
    case BXOR:
    case BOR:
    case LOR:
    case ADD:
    case MUL:
    case DIV:
    case MOD:
    case RSHIFT:
    case LSHIFT:
        sub_3BC6(&sa->info.np[0]->info.l, sa->info.np[1]->info.l, sa->a_c0);
        goto finish;
    case MINUS_U:
        sa->info.np[0]->info.l = -sa->info.np[0]->info.l;
        goto finish;
    case NOT:
        sa->info.np[0]->info.l = sa->info.np[0]->info.l == 0;
        goto finish;
    case BNOT:
        sa->info.np[0]->info.l = ~sa->info.np[0]->info.l;
        goto finish;
    default:
        return sa;
    }
}

/*********************************************************
 * sub_415E OK     Used in: sub_808,  sub_E43,  sub_4192,
 *			    sub_4DA3, sub_54B6
 *********************************************************/
node_t *sub_415E(long number) {
    register node_t *sa;

    sa         = allocNode();
    sa->a_c0   = CONST;
    sa->pm     = typeLong;
    sa->info.l = number;

    return sa;
}

/* end of file tree1.c */
