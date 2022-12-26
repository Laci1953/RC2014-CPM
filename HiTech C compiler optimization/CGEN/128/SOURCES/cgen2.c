/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File - cgen2s.c	BIG size - problems compiling v3.09
 */

/* compiling this function with Hitech C runs out of memory
   so here the external functions are declared explicitly
   so that the normal function declarations and some of the
   unused include files are not processed.
*/

#define MINI
#include "cgen.h"
#include "alloc128.h"

int atoi(char *);
uint8_t sub_14F3(node_t *);
uint16_t sub_387A(node_t *);
uint16_t nodesize(node_t *);
uint8_t sub_46F7(long);

/*.......................................................*
 *  sub_2D09_v2 OK++			Used in: sub_3564
 * minor differences
 * 1) block of 5 instructions has different order but same effect
 * 2) one optimisation missed on loading a from lc10, possibly
 *    related to note 1
 * 3) optimisation because p3 is passed recursively as a char
 *.......................................................*/
void sub_2D09(register node_t *sa, char *p2, char p3) {
    struct codeFrag_t *lz1 FORCEINIT;
    node_t *la2 FORCEINIT;
    char *li3 FORCEINIT;
    uint8_t lc4;
    char lc5;
    char lc6;
    char lc7;
    char /* int */ ch;
    long ll9;
    char lc10;
    char lc11;
    char *li12;
    uint32_t ll13;
    bool lc14;

    li12 = p2;
    lc11 = lc14 = false;
    for (;;) {
        if ((lc7 = *p2++) == 0)
            return;
        switch (lc7) {
        case ' ':
            if (!lc14) {
                fputchar('\t');
                lc14 = true;
            } else
                fputchar(lc7);
            continue;
        case ';':
            return;
        case '\n':
        case '\r':
            fputchar('\n');
            lc14 = false;
            continue;
        case 'G':
        case 'X':
            if (isdigit(*p2)) {
                li3 = p2;
                while (isdigit(*p2))
                    p2++;
            } else
                li3 = 0;
            lc6 = *p2++;
            ch  = 'G';
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
        case '~':
            if ((ch = *p2) == '-' || ch == '+') {
                if (isdigit(*++p2)) {
                    lc5 = atoi(p2);
                    if (ch == '-')
                        lc5 = -lc5;
                    ch = 0;
                } else
                    lc5 = 0;
                while (isdigit(*p2) || *p2 == '+' || *p2 == '-')
                    p2++;
            } else if (ch == '>') {
                for (lc5 = 0; *p2 == '>'; p2++, lc5++)
                    ;
            } else {
                ch  = 0;
                lc5 = 0;
            }

            lc6 = *p2++;
            break;
        case 'L':
        case 'R':
        case 'U':
            lc6 = lc7;
            lc7 = '~';
            ch  = 0;
            lc5 = 0;
            break;
        case 'D':
            lc6 = lc7;
            ch  = 0;
            lc5 = 0;
            break;

        default:
            fputchar(lc7);
            continue;
        }

        switch (lc6) {
        case 'L':
            if (p3 > 0) { /* m27: */
                la2  = sa;
                lc10 = p3 - 1;
                lz1  = &codeFrag[sa->a_c3[lc10]];
            } else {
                la2  = sa->info.np[0];
                lc10 = la2->a_c1 - 1;
                lz1  = &codeFrag[la2->a_c3[lc10]];
            }
            break;
        case 'R':
            la2  = sa->info.np[1];
            lc10 = la2->a_c1 - 1;
            lz1  = &codeFrag[la2->a_c3[lc10]];
            break;
        case 'N':
            la2  = sa;
            lc10 = p3;
            break;
        }
        switch (lc7) {
        case 'D':
            if (word_AFF8 != 0) {
                printf("%d", word_AFF8);
                word_AFF8 = 0;
            }
            break;
        case 'S':
            if (la2->a_c0 == USEREG)
                printf("%s", regNames[la2->info.l]);
            else if (la2->a_c0 == CONST)
                sub_2CE0(la2, la2->info.l);
            else if (la2->a_c0 == FCONST)
	    {
		GetString(tmpBuf, la2->info.sv.s);
                printf("%s", tmpBuf);
	    }
            else
	    {
		GetString(tmpBuf, /* la2->info.mp[0]->b_name */ (char*)GetWord((char*)(la2->info.mp[0])+OFF_b_name));
                printf("%s", tmpBuf);
	    }
	    
        proff:
            if (ch == 0) {
                lc5 += word_AFF8;
                word_AFF8 = 0;
                if (lc5 > 0)
                    printf("+%d", lc5);
                else if (lc5 < 0)
                    printf("%d", lc5);
            }
            break;
        case 'F':
            printf("%d", la2->info.sv.v);
            goto proff;

        case 'Z':
            ll9 = nodesize(la2);
            goto m46;
        case 'B':
            ll9 = sub_387A(la2);
        m46:
            if (ch == '-') {
                if (lc11 == 0) {
                    ll13      = ll9 + (long)word_AFF8;
                    word_AFF8 = 0;
                    lc11      = 1;
                }
                goto m54;
            }
            printf("%lu", ll9);
            break;
        case 'O':
            printf("%s", otherFrag[la2->a_c0]);
            break;
        case 'A':
            sub_153A(la2);
            break;
        case 'V':
            if (ch == '-' || ch == '+') {
                if (lc11 == 0) {
                    ll13      = la2->info.l + (long)word_AFF8;
                    word_AFF8 = 0;
                    lc11      = 1;
                }
            m54:
                ll13 += ch == '-' ? -1L : 1L;
                if (sub_14F3(la2) == 2 && nodesize(la2) < 4)
                    ll13 &= (1 << (int)(nodesize(la2) * 8)) - 1;

                if (ll13 != 0) {
                    while (li12 < p2 && p2[-1] != '\n')
                        p2--;
                    fputchar('\n');
                } else
                    lc11 = 0;
            } else {
                lc5 += word_AFF8;
                word_AFF8 = 0;
                sub_2CE0(la2, la2->info.l >> (lc5 * 8));
            }
            break;

        case 'C':
            if (la2->a_c0 == BFIELD)
                printf("%d", /* la2->info.mp[1]->b_b6 */ GetByte((char*)(la2->info.mp[1])+OFF_b_b6));
            else {
                ll9 = la2->info.l;
                if (sub_46F7(ll9) == 0)
                    ll9 = ~ll9;
                word_AFF8 += (sub_46F7(ll9) - 1) / 8;
            }
            break;
        case 'T':
            if (la2->a_c0 == BFIELD)
                printf("%d", /* la2->info.mp[1]->b_b5 */ GetByte((char*)(la2->info.mp[1])+OFF_b_b5));
            else {
                ll9 = la2->info.l;
                if (sub_46F7(ll9) == 0)
                    ll9 = ~ll9;
                printf("%d", sub_46F7(ll9) - 1);
            }
            break;
        case 'G':
        case 'X':
            lc4 = lc7 == 'G' ? la2->a_reg[lc10] : la2->a_uc9[lc10];

            if (li3 != 0)
                while (isdigit(*li3))
                    lc4 = array_AB24[lc4 * 2 + (*li3++ - '0')];

            if (*p2 != '+') {
                lc4 += word_AFF8;
                word_AFF8 = 0;
            }
            printf("%s", regNames[lc4]);
            break;
        case '~':
            if (lc6 == 'U') {
                lc5 += word_AFFB;
                if ((dopetab[sa->a_c0] & 0x200) && p3 != 0)
                    lc5--;
                printf("%d", lc5);
            } else {
                word_AFF8 += lc5;
                sub_2D09(la2, lz1->p_8, lc10);
            }
            break;
        }
    }
}

/* end of file cgen2s.c */
