/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File - local1s.c
 */

#define MINI
#include "cgen.h"

node_t *allocNode(void);
uint16_t nodesize(node_t *);
uint8_t sub_14F3(node_t *);
node_t *sub_415E(long);
node_t *sub_43EF(uint8_t, node_t *, node_t *);
uint8_t sub_46F7(long);
bool sub_4B89(node_t *);
bool sub_4BE5(node_t *);
int sub_4C12(node_t *);
node_t *sub_4C8B(node_t *);
node_t *sub_4DA3(node_t *);
node_t *sub_4E8D(node_t *);
node_t *sub_4FA8(node_t *);
node_t *sub_4FCE(node_t *);
node_t *sub_508A(node_t *);
node_t *sub_53EE(node_t *);
bool sub_60A8(node_t *, node_t *);
bool sub_6246(node_t *, node_t *);
bool sub_62BE(node_t *, node_t *);

/*********************************************************
 * sub_54B6 - OK++ PMO			Used in: sub_600E
 * needs ported optimiser to build
 * Optimiser differences
 * 1) in the GT ? GEQ : LT the GEQ assignment is moved
 * 2) in MUL_U, the optimiser uses bc rather than hl, then
 *    does hl = bc
 *********************************************************/
node_t *sub_54B6(register node_t *sa) {
    node_t *l1a;
    node_t *l2a;
    int l3;
    uint32_t l4;

    l3 = dopetab[sa->a_c0] & 0xC;

    if (l3 != 0)
        sa->info.np[0] = sub_54B6(sa->info.np[0]);

    if (l3 == 8)
        sa->info.np[1] = sub_54B6(sa->info.np[1]);

    if (dopetab[sa->a_c0] & 0x40) {
        if (sub_4C12(sa->info.np[0]) < sub_4C12(sa->info.np[1])) {
            byte_B013      = true;
            l1a            = sa->info.np[0];
            sa->info.np[0] = sa->info.np[1];
            sa->info.np[1] = l1a;
        }
        if ((dopetab[sa->a_c0] & 0x2000) && sa->a_c0 == sa->info.np[0]->a_c0) {
            if (sub_4B89(sa->info.np[1]) && sub_4B89((l1a = sa->info.np[0])->info.np[1])) {
                byte_B013       = true;
                sa->info.np[0]  = l1a->info.np[1];
                l1a->info.np[1] = sa;
                sa              = l1a;
            } else if (sub_4C12(sa->info.np[0]->info.np[1]) < sub_4C12(sa->info.np[1])) {
                byte_B013                  = true;
                l1a                        = sa->info.np[1];
                sa->info.np[1]             = sa->info.np[0]->info.np[1];
                sa->info.np[0]->info.np[1] = l1a;
            }
        }
    }

    if ((dopetab[sa->a_c0] & 0xC00) == 0x800 && sa->info.np[0]->a_c0 == CONV &&
        nodesize(sa->info.np[0]->info.np[0]) < nodesize(sa) &&
        sub_60A8(sa->info.np[1], sa->info.np[0]->info.np[0])) {

        l2a                   = sa->info.np[0];
        sa->info.np[0]        = l2a->info.np[0];
        sa->pm                = sa->info.np[0]->pm;
        sa->a_i15             = sa->info.np[0]->a_i15;
        sa->info.np[1]->pm    = sa->pm;
        sa->info.np[1]->a_i15 = sa->a_i15;
        l2a->info.np[0]       = sa;
        sa                    = l2a;
    }

    if ((dopetab[sa->a_c0] & 0x420) == 0x420 && (l1a = sa->info.np[0])->a_c0 == CONV &&
        nodesize(l1a->info.np[0]) != 0 && nodesize(l1a->info.np[0]) < nodesize(l1a)) {
        if (sa->info.np[1]->a_c0 == CONST &&
            (sub_14F3(l1a) == 1 || sub_14F3(l1a->info.np[0]) == 2)) {
            if (sub_62BE(sa->info.np[0], sa->info.np[1])) {
                freeNode(sa->info.np[0]);
                freeNode(sa->info.np[0]->info.np[1]);
                sa->info.np[0]        = sa->info.np[0]->info.np[0];
                sa->info.np[1]->pm    = sa->info.np[0]->pm;
                sa->info.np[1]->a_i15 = sa->info.np[0]->a_i15;
            } else
                warningMsg = "mismatched comparision"; /* m8: */
        } else if (sa->info.np[1]->a_c0 == CONV &&
                   sub_6246(sa->info.np[0]->info.np[0], sa->info.np[1]->info.np[0])) {
            l1a            = sa->info.np[0];
            sa->info.np[0] = l1a->info.np[0];
            freeNode(l1a->info.np[1]);
            freeNode(l1a);
            l1a            = sa->info.np[1];
            sa->info.np[1] = l1a->info.np[0];
            freeNode(l1a->info.np[1]);
            freeNode(l1a);
        }
    }

    switch (sa->a_c0) {
    case IDOP:
        return sub_4DA3(sa);

    case DOT:
        return sub_4E8D(sa);

    case NOT:
        return sub_4C8B(sa);

    case LT:
    case GEQ:
        if (sub_14F3(sa->info.np[0]) == 2 && sub_4BE5(sa->info.np[1])) {
            warningMsg = "degenerate unsigned comparision";
            sub_475C(sa);
            sa = sub_415E(sa->a_c0 == GEQ ? 1L : 0L);
        }
        break;
    case LEQ:
    case GT:
        if (sub_14F3(sa->info.np[0]) == 2 && sub_4BE5(sa->info.np[1]))
            sa->a_c0 = sa->a_c0 == GT ? NEQL : EQL;
        else if (sa->info.np[1]->a_c0 == CONST) {
            sa->info.np[1]->info.l += 1;
            sa->a_c0 = sa->a_c0 == GT ? GEQ : LT;
        } else {
            l1a            = sa->info.np[0];
            sa->info.np[0] = sa->info.np[1];
            sa->info.np[1] = l1a;
            sa->a_c0       = sa->a_c0 != GT ? GEQ : LT;
        }
        break;
    case ASEOR:
    case BOR:
        if (sa->info.np[1]->a_c0 == CONST && sa->info.np[1]->info.l == -1L) {
            sub_475C(sa->info.np[0]);
            freeNode(sa);
            return sa->info.np[1];
        }
        /* fall through */
    case ADD:
    case SUB:
    case LSHIFT:
    case ASADD:
    case ASSUB:
    case ASLSHIFT:
    case ASRSHIFT:
    case RSHIFT:
        if (sa->info.np[1]->a_c0 == CONST && sa->info.np[1]->info.l == 0L)
            return sub_4FA8(sa);

        if ((dopetab[sa->a_c0] & 0x80) && nodesize(sa->info.np[1]) != 1) {
            l1a            = allocNode();
            l1a->pm        = sub_14F3(sa->info.np[1]) == 2 ? typeUChar : typeChar;
            l1a->a_c0      = TYPE;
            sa->info.np[1] = sub_43EF(CONV, sa->info.np[1], l1a);
        }
        break;
    case MUL:
    case DIV:
    case ASMUL:
    case ASDIV:
        if (sa->info.np[1]->a_c0 == CONST && sa->info.np[1]->info.l == 1L)
            return sub_4FA8(sa);
        /* fall through */
    case MOD:
    case ASMOD:
        if (sa->info.np[1]->a_c0 == CONST && sub_46F7(sa->info.np[1]->info.l) != 0)
            sub_4FCE(sa);
        /* fall through */
    case BAND:
    case ASAND:
        if (sa->info.np[1]->a_c0 == CONST && sa->info.np[1]->info.l == 0L)
            switch (sa->a_c0) {
            case MOD:
            case DIV:
            case ASMOD:
            case ASDIV:
                prWarning("Division by zero");
                return sa;
            case BAND:
            case MUL:
                sub_475C(sa->info.np[0]);
                sub_475C(sa->info.np[1]);
                sa->a_c0   = CONST;
                sa->info.l = 0;
                return sa;
            case ASAND:
            case ASMUL:
                sa->a_c0 = ASSIGN;
                return sa;
            }
        l4 = nodesize(sa) >= 4 ? -1L : (1L << (nodesize(sa) * 8)) - 1;
        if ((sa->a_c0 == BAND || sa->a_c0 == ASAND) && sa->info.np[1]->a_c0 == CONST &&
            l4 == (sa->info.np[1]->info.l & l4))
            return sub_4FA8(sa);
        break;
    case CONV:
        return sub_508A(sa);
    case PLUS_U:
        freeNode(sa);
        sa = sa->info.np[0];
        break;
    case GADDR:
        if (sa->info.np[0]->a_c0 == MUL_U)
            return sub_53EE(sa);
        break;
    case MUL_U:
        if (sa->info.np[0]->a_c0 == GADDR) {
            l1a = sa->info.np[0]->info.np[0];
            if (l1a->a_c0 != LPAREN && l1a->a_c0 != ASSIGN && l1a->a_c0 != QUEST)
            	return sub_53EE(sa);
        }
        break;
    }
    return sa;
}

/* end of file local1.c */
