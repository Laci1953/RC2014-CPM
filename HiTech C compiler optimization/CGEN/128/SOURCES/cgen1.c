/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File - cgen1.c
 */

#include "cgen.h"
#include "alloc128.h"

/*********************************************************
 * sub_2BD0 OK++ PMO  Used in: sub_793, sub_808, sub_3CDF
 *			       sub_3DC9
 *********************************************************/
void sub_2BD0(register node_t *sa) {
    int tmp;

    sa->a_c1 = 0;

    if (sub_1F4B(sa, 0x48, word_B017, 0, &tmp) <= 0)
        fatalErr("Expression too complicated");

    sub_35E6(sa);
    if (sa->a_c0 == COLON_U) {
        if (sa->a_i15 & 2) {
            prFrameHead(/* array_AEDB[lvlidx]->b_off */ GetWord((char*)(array_AEDB[lvlidx])+OFF_b_off)); /* emit_call_ncsv */
            sub_1EDF();
        }
        if (sa->info.np[0]->a_c0 == IDOP)
            /* sa->info.np[0]->info.mp[0]->b_sloc |= 1; */ /* may ba struct aaa a_c3[0] */
	    PutByte((char*)(sa->info.np[0]->info.mp[0])+OFF_b_sloc, GetByte((char*)(sa->info.np[0]->info.mp[0])+OFF_b_sloc) | 1);
    }
    sub_475C(sa);
}

/*********************************************************
 * sub_2C5E OK++ PMO   	      Used in: sub_2CE0, sub_47B2
 *
 *********************************************************/
long sub_2C5E(node_t *sa, long p2) {
    char loc;

    if ((loc = nodesize(sa) * 8) >= 32)
        return p2;
    if (p2 & (1L << (loc - 1)))
        p2 |= ~((1L << loc) - 1L);
    return p2;
}

/*********************************************************
 *  sub_2CE0 OK++ PMO			Used in: sub_2D09
 *********************************************************/
void sub_2CE0(node_t *p1a, long p2) {

    printf("%ld", sub_2C5E(p1a, p2));
}
/* end of cgen1.c */
