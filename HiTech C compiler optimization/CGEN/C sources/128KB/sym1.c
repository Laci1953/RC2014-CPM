/*
 * File - sym1.c
 */

#include "cgen.h"
#include "alloc128.h"

/*********************************************************
 * sub_17E0 OK++ PMO		Used in: sub_6D1, sub_4192
 *
 * Two related Basic Blocks moved and one optimisation
 * loading a from l rather then sb->b_size
 *********************************************************/
void parseVariable() {
    node_t *l1a;
    int l2;
    register member_t *sb;

    sb         = sub_1754(getToken(), VAR);
    sb->b_type = sub_627(getToken(), &sb->b_refl);
    l1a        = sub_600E(parseExpr());
    if (l1a->a_c0 != CONST)
        fatalErr("Bad element count expr");
    sb->b_nelem = (uint16_t)l1a->info.l;
    sub_475C(l1a);
    l2 = *getToken();
    expect(']');

    switch (l2) {
    case 'A': /* auto */
    case 'a':
        sb->b_flag = 1;
        break;
    case 'R': /* register */
    case 'r':
        sb->b_sloc |= 8;
    case 'P': /* pointer */
    case 'p':
        sb->b_flag = 5;
        break;
    case 'S': /* static */
    case 's':
        sb->b_flag = 4;
        break;
    case 'E': /* extern	*/
    case 'e':
        sb->b_flag = 3;
        break;
    case 'T': /* typedef */
    case 't':
        sb->b_class = TYPE;
        if (sb->b_refl != 0) {
            sb->b_off  = 0;
            sb->b_flag = 2;
        } else {
            sb->b_off  = sb->b_type->b_off;
            sb->b_flag = sb->b_type->b_flag;
        }
        if (sb->b_nelem != 1)
            sb->b_flag = 0;
        break;
    default:
        fatalErr("Bad storage class");
    }
    sb->b_size = varSize(sb);
    if (sb->b_class != TYPE) {
        if (sb->b_refl == 0 && sb->b_size == 0 && sb->b_type->b_size == 0)
            prError("Bad dimensions");

        if (sb->b_flag == 5 || sb->b_flag == 1)
            sub_5CF5(sb, l2);
        else
            sb->b_off = newLocal(); /* swTableCnt++ */

        if ((sb->b_refl & 0x2) && sb->b_nelem != 0) {
            if (++lvlidx == MAXFUN)
                fatalErr("Functions nested too deep");

            array_AE57[lvlidx] = nstdpth; /* Save current nesting depth */
            array_AE97[lvlidx] = 6;
            array_AE13[lvlidx] = 0;
            array_AEDB[lvlidx] = sb;
        }
    }
}

typedef struct {
    uint16_t cnt;
    member_t *vals[257];
} memberb_t;

typedef struct {
    uint16_t cnt;
    int16_t vals[257];
} memberi_t;

/*********************************************************
 * sub_19C1 OK++ PMO	       Used in: sub_6D1, sub_4192
 *
 * One minor optimisation as sub_1754 now declared as taking
 * a uint8_t rather than int for last parameter
 *********************************************************/
void parseMembers(int p1) {

    size_t size;
    member_t *l2b;
    memberb_t *pMember;
    node_t *l4a;
    char *l5;
    memberb_t members;
    register member_t *sb;

    l2b          = sub_1754(getToken(), p1);
    pMember      = &members;
    pMember->cnt = 0;
    for (;;) {
        l5 = getToken();
        if (*l5 == ']')
            break;
        sb          = (member_t *)allocMem(sizeof(member_t)); /* Create member_t */
        sb->b_class = MEMBER;
        if (*l5 == ':') {
            sb->b_b5 = atoi(l5 + 1);
            sb->b_sloc |= 0x10;
            l5 = getToken();
        }
        sb->b_type = sub_627(l5, &sb->b_refl);
        l4a        = sub_600E(parseExpr());
        if (l4a->a_c0 != CONST)
            fatalErr("Strucdecl - bad\tnelem");
        sb->b_nelem = (uint16_t)l4a->info.l;
        sub_475C(l4a);
        sb->b_size                    = varSize(sb);
        pMember->vals[pMember->cnt++] = sb;
    }
    /* note in the size calculation below an extra member_t pointer is reserved
     * in the decompilation I have assumed struct _memb (defined in struct bbb)
     * used a dummy array size of [1] for the vals, this gives the extra pointer
     */
    size    = (pMember->cnt - 1) * sizeof(member_t *) + sizeof(struct _memb);
    pMember = allocMem(size);
    bmove(&members, pMember, size);
    l2b->b_memb.mlist = (struct _memb *)pMember;
    sub_1CEF(l2b);
}

/*********************************************************
 * sub_1B0C OK++ PMO		Used in: sub_6D1, sub_4192
 *
 * There are three minor differences of equivalent code
 * 1) optimisation of last parameter passed to sub_1754 as it
 *    is declared a uint8_t
 * 2) Slightly inferior code to calculate (pMember->cnt - 1)
 *    using explicity add -1 rather than dec hl
 * 3) code generated following expect includes popping the
 *    passed in paramater, before jp cret. As cret will
 *    clean up the stack this isn't strictly necessary
 *
 *********************************************************/
void parseEnum() {
    node_t *l1a;
    memberi_t *pMember;
    int lobnd;
    int hibnd;
    size_t size;
    memberi_t members;
    register member_t *sb;

    lobnd        = MININT; /* arithmetic overflow in constant expression */
    hibnd        = MAXINT;
    sb           = sub_1754(getToken(), ENUM);
    pMember      = &members;
    pMember->cnt = 0;
    for (;;) {
        l1a = parseExpr();
        if (l1a->a_c0 == DOT_DOT) {
            sub_475C(l1a);
            size    = (pMember->cnt - 1) * sizeof(uint16_t) + sizeof(struct _memi);
            pMember = allocMem(size);
            bmove(&members, pMember, size);
            sb->b_memb.ilist = (struct _memi *)pMember;
            sub_15FB(sb, hibnd, lobnd);
            expect(']');
            return;
        }
        l1a                           = sub_600E(l1a);

        pMember->vals[pMember->cnt++] = (uint16_t)l1a->info.l;

        if (lobnd < l1a->info.l)
            lobnd = l1a->info.l;
        if (l1a->info.l < hibnd)
            hibnd = l1a->info.l;
        sub_475C(l1a);
    }
}

/*********************************************************
 * sub_1C6D OK++ PMO			Used in: sub_1CEF
 *********************************************************/
int sub_1C6D(int p1, int p2) {

    return (p1 + p2) & ~p2;
}

/*********************************************************
 * sub_1C8E OK++ PMO   Used in: sub_B19, sub_17E0, sub_19C1
 *********************************************************/
int varSize(register member_t *sb) {

    if (sb->b_refl & 1)
        return sb->b_nelem * 2;
    return sb->b_type->b_size * sb->b_nelem;
}

/*********************************************************
 * sub_1CC4 OK++ PMO	       Used in: sub_808, sub_1CEF
 *********************************************************/
int sub_1CC4(register member_t *sb) {

    if (sb->b_refl != 0)
        return 0;
    return sb->b_type->b_off;
}

/*********************************************************
 * sub_1CEF OK++ PMO			Used in: sub_19C1
 *********************************************************/
void sub_1CEF(register member_t *sb) {
    int l1;
    int l2;
    int l3;
    member_t *l4b;
    l1 = 0;

    for (l2 = l3 = 0; l2 != sb->b_memb.mlist->cnt; l2++) {

        l4b = sb->b_memb.mlist->vals[l2];
        if (sb->b_class == UNION) {
            l1         = max(l1, l4b->b_size);
            l4b->b_off = 0;
        } else {
            l1 = l4b->b_off = sub_1C6D(l1, sub_1CC4(l4b));
            if (l4b->b_sloc & 0x10) {
                if (16 < l3 + l4b->b_b5 || l4b->b_b5 == 0) {
                    if (l3 != 0)
                        l4b->b_off = (l1 += 2);
                    l3 = 0;
                }
                l4b->b_b6 = l3;
                l3 += l4b->b_b5;
                if (l3 == 32) {
                    l3 = 0;
                    l1 += 2;
                }
            } else {
                if (l3 != 0) {
                    l3         = 0;
                    l4b->b_off = (l1 += 2);
                }
                l1 += l4b->b_size;
            }
        }
    }
    if (l3 != 0) {
        l3 = 0;
        l1 += 2;
    }
    sb->b_size  = sub_1C6D(l1, 0); /* m10: */
    sb->b_off   = 0;
    sb->b_class = STRUCT;
}

/*********************************************************
 * sub_1EDF OK++ PMO			Used in: sub_2BD0
 *********************************************************/
void sub_1EDF() {
    member_t **l1b;
    register member_t *sb;

    l1b = hashtab;
    do {
        for (sb = *l1b; sb != 0 && sb->b_ndpth == nstdpth; sb = sb->b_next)
            if (sb->b_class == VAR && sb->b_flag == 2 && (sb->b_sloc & 4))
                prIXnPush(sb);
    } while (++l1b != &hashtab[HASHTABSIZE]);
}

/* end of file sym1.c */
