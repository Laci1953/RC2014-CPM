/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File - sym3.c
 */

#include "cgen.h"

/*********************************************************
 * sub_283E OK++ PMO			Used in: sub_2B2A
 *
 * apart from one minor optimisation difference to save
 * a pop bc, the code is identical
 *********************************************************/
int sub_283E(register node_t *sa, int par) {
    char *s;
    struct codeFrag_t *l2z;
    node_t *l3a FORCEINIT;
    char *t;
    uint8_t ch;
    int l6;
    int l7;

    l2z = &codeFrag[sa->a_c3[par]];
    l7  = 0;

    for (s = l2z->p_8; *s;) {
        switch (ch = *(s++)) {
        case 'X':
        case 'G':
            if (isdigit(*s)) {
                t = s;
                while (isdigit(*s))
                    s++;
            } else
                t = 0;

            switch (*s++) {
            case 'N':
                l6  = par;
                l3a = sa;
                break;
            case 'L':
                if (par > 0) {
                    l3a = sa;
                    l6  = par - 1;
                } else {
                    l3a = sa->info.np[0];
                    l6  = l3a->a_c1 - 1;
                }
                break;
            case 'R':
                l3a = sa->info.np[1];
                l6  = l3a->a_c1 - 1;
                break;
            }
            if (ch == 'S') {
                if (l3a->a_c0 == USEREG)
                    ch = (uint8_t)l3a->info.l;
                else
                    ch = 0;
            } else {
                ch = ch == 'G' ? l3a->a_reg[l6] : l3a->a_uc9[l6];
                if (t)
                    while (isdigit(*t) != 0)
                        ch = array_AB24[ch * 2 + (*t++ - '0')];
            }
            l7 |= array_AAE8[ch];
            if (ch != 0)
                byte_AFFA = ch;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'F':
        case 'M':
        case 'O':
        case 'P':
        case 'Q':
        case 'S':
        case 'T':
        case 'V':
        case 'W':
        case 'Z':
            while (*s && *s != 'N' && *s != 'L' && *s != 'R')
                s++;
            break;
        case '~':
            while (*s && *s != 'L' && *s != 'R')
                s++;
            break;
        case 'L':
            if (par > 0)
                l7 |= sub_283E(sa, par - 1);
            else
                l7 |= sub_2B2A(sa->info.np[0]);

            break;
        case 'R':
            l7 |= sub_2B2A(sa->info.np[1]);
            break;
        case 'D':
        case 'U':
            break;
        }
    }
    return l7 & word_B017;
}

/*********************************************************
 * sub_2B2A OK++ PMO Used in: sub_1F4B, sub_283E, sub_2B79
 * optimiser improves on code for sa->a_c1 - 1
 *********************************************************/
int sub_2B2A(register node_t *sa) {

    if (sa->a_c1 == 0) {
        if ((dopetab[sa->a_c0] & 0xC) == 0)
            return 0;
        return sub_2B2A(sa->info.np[0]); /* LOGOP & QUEST */
    }
    return sub_283E(sa, sa->a_c1 - 1);
}

/*********************************************************
 * sub_2B79 OK++ PMO			 Used in: sub_1F4B
 *********************************************************/
uint8_t sub_2B79(register node_t *sa) {
    uint8_t l1;
    int l2;

    l1        = sa->a_uc9[sa->a_c1 - 1];
    byte_AFFA = 0;
    l2        = sub_2B2A(sa);
    if (l1 != 0 && sub_6589(l2, l1) == l1)
        return l1;
    return byte_AFFA;
}
/* end of file sym3.c */
