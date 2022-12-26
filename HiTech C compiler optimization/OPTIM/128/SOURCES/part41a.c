/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "optim1.h"

extern int regTestMasks[];

/**************************************************************************
 58 sub_436e    ok++ (PMO)
 Same except optimiser misses the optimisation of fputc(')', stdout)
 **************************************************************************/
void sub_436e(register operand_t const *pi_arg) {
    operand_t local_pi;
    operand_t *pi = &local_pi;

#ifdef DEBUG
    pr_warning("sub_436e");
#endif

    ReadMem((char*)&local_pi, (char*)pi_arg, sizeof(operand_t));

    /* HEAP(pi); */
    switch (pi->type) {
        case T_INDEXED:
        case T_ADDRREF:
            fputc('(', stdout);
            if (pi->type == T_INDEXED) {
                if (pi->aux == REG_IX) {
                    printf("ix");
                } else {
                    printf("iy");
                }
                fputc('+', stdout);
            }
        case T_CONST:
            if (pi->oPSym) {
                if (pi->oPSym->label[0]) {
                    printf("%s", pi->oPSym->label);
                } else {
                    printf("L%d", pi->oPSym->p.pInst->iSymId);
                }
                if (0 < pi->oVal) {
                    fputc('+', stdout);
                }
            }
            if (pi->oVal != 0 || !pi->oPSym) {
                printf("%d", pi->oVal);
            }
            if (pi->type != T_CONST) {
                fputc(')', stdout);
            }
            break;
        case T_REGREF:
            fputc('(', stdout);
        case T_REG:
            printf("%s", regs[pi->aux]);
            if (pi->type == T_REGREF) {
                fputc(')', stdout);    /* OPTIMISER[1]: misses optimising htis with same fputc above */
            }
            break;
        case T_FWD:
            printf("%df", pi->oVal);
            break;
        default:
            pr_error("Bad operand pi=%04x type=%02x", pi_arg, pi->type);
            break;
    }
    /* HEAP(pi); */
}

/**************************************************************************
 59 sub_44b2    ok++ (PMO)
 Optimiser saves some code with minor changes to code for regValues[18]

 original                   replacement
 ld a,(regValues + 6ch)     ld  hl,regvalues + 6ch  ; test regValues[18].type
 or a                       ld  a,(hl)
 jp z,cret                  or  a
 push   iy                  jp  z,cret
 ld hl,regVales + 6ch       push    iy
 push   hl                  push    hl              ; &regValues[18]

 The replacement code is slightly short
 **************************************************************************/
void sub_44b2(register operand_t const *po) {
    int n;
/* po is checked !!! */
#ifdef DEBUG
    pr_warning("sub_44b2");
#endif

    while ((n = sub_46b1(po, REG_B)) != -1) {
        sub_4544(n);
    }
    while ((n = sub_46b1(po, REG_BC)) != -1) {
        sub_4544(n);
    }

    if (regValues[17].type && po && operandsSame(po, &regValues[17])) {
        sub_4544(17);
    }

    if (regValues[REG_TRACKER].type && po && operandsSame(&regValues[REG_TRACKER], po)) {
        regValues[REG_TRACKER].type = T_INVALID;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_TRACKER);
#endif
    }
}

/**************************************************************************
 60 sub_4544    ok++ (PMO)
 *
 * Optimiser generates marginally less efficient code for regValues[18]
 * access it chooses to use hl and (hl) rather than the load directly
 * to/from a code functionally is the same
 **************************************************************************/
void sub_4544(int reg) {
    register operand_t *pi;     /* NOT allocated !!! */

#ifdef DEBUG
    pr_warning("sub_4544");
#endif

    regValues[reg].type = T_INVALID;
#ifdef PRINT_REG_VAL
 PrintRegVal(reg);
#endif
    if (regTestMasks[REG_HL] & regTestMasks[reg]) {
        hlDelta                     = 0;
        regValues[REG_TRACKER].type = T_INVALID;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_TRACKER);
#endif
    }
    if (pi = regHiLoValMap[reg].pHiRegVal) {
        pi->type = T_INVALID;
        if (pi = regHiLoValMap[reg].pLoRegVal) {
            pi->type = T_INVALID;
        }
    }
    if (reg != 17) {
        return;
    }
    for (pi = regValues; pi < &regValues[REG_TRACKER]; ++pi)
        if (pi->type == T_INDEXED && pi->aux == REG_IY) {
            pi->type = T_INVALID;
        }

    if (regValues[REG_TRACKER].type == T_INDEXED && regValues[REG_TRACKER].aux == REG_IY) {
        regValues[REG_TRACKER].type = T_INVALID;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_TRACKER);
#endif
    }
}

/**************************************************************************
 61 sub_4601    ok+
 *
 *  Generates correct code, but different from original
 **************************************************************************/
void sub_4601() {
    register operand_t *po;     /* NOT allocated */

#ifdef DEBUG
    pr_warning("sub_4601");
#endif

    for (po = regValues; po < &regValues[REG_TRACKER]; ++po) {
        po->type = T_INVALID;
    }
    regValues[REG_TRACKER].type = T_INVALID;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_TRACKER);
#endif
}

/**************************************************************************
 62 sub_4625    ok+
 *
 *  Generates correct code, but in a sequence different from original
 **************************************************************************/
bool sub_4625(register inst_t *pi_arg) {
    operand_t local_pi;
    operand_t *pi = &local_pi;
    uint8_t t,a;

#ifdef DEBUG
    pr_warning("sub_4625");
#endif

/* -------------------------------------------------------------------------- */
    if (IsValid((char*)pi_arg->iLhs))
    {
        ReadMem((char*)&local_pi, (char*)pi_arg->iLhs, sizeof(operand_t));
        t = pi->type;
        a = pi->aux;
    }
    else
    {
        t = pi_arg->iLhs->type;
        a = pi_arg->iLhs->aux;
    }
/* -------------------------------------------------------------------------- */

    switch (pi_arg->type) {
        case T_LD:
        case T_STK:
            return true;
        case T_INCDEC:
            return /* pi_arg->iLhs->type */ t == (uint8_t) T_REG && /*pi_arg->iLhs->aux*/ a >= (uint8_t) REG_BC;
        case T_EX:
            return /* pi_arg->iLhs->aux */ a != (uint8_t) REG_AF;
    }
    return false;
}

/**************************************************************************
 63 sub_4682    ok++
 **************************************************************************/
bool sub_4682(register operand_t const *pi_arg) {
    operand_t pi_local;
    operand_t* pi = &pi_local;

#ifdef DEBUG
    pr_warning("sub_4682");
#endif

    if (!pi_arg)
        return false;

    if (IsValid((char*)pi_arg))
        ReadMem((char*)&pi_local, (char*)pi_arg, sizeof(operand_t));
    else
        pi = pi_arg;

    return pi->type == T_CONST && !pi->oPSym && pi->oVal == 0;
}

/**************************************************************************
 64 sub_46b1    ok++
 **************************************************************************/
int sub_46b1(register operand_t const *opr, int reg) {
    operand_t *po;
    int        i;
/* opr is checked !!! */
#ifdef DEBUG
    pr_warning("sub_46b1");
#endif

    po = reg < REG_BC ? &regValues[REG_B] : &regValues[REG_BC];
    do {
        if (po->type)
        {
            if (opr && operandsSame(po, opr))
            {
                i = (int)(po - regValues);

                if (i >= REG_BC && reg < REG_BC)
                    return regHiLoMap[i].loReg;

                return i;
            }
        }
    } while (++po < &regValues[REG_SP]);

    return -1;
}

/**************************************************************************
 65 sub_475c    ok++
 **************************************************************************/
int sub_475c(register operand_t const *po_arg, int p2) {
    operand_t po_local;
    operand_t* po = &po_local;

#ifdef DEBUG
    pr_warning("sub_475c");
#endif

     if (!po_arg) {
        return false;
    }

    ReadMem((char*)&po_local, (char*)po_arg, sizeof(operand_t));

    if (po->type != (uint8_t)T_REG && po->type != (uint8_t)T_INDEXED && po->type != (uint8_t)T_REGREF) {
        return false;
    }

    if (p2 & regTestMasks[po->aux]) {
        return true;
    }

    return false;
}

/**************************************************************************
 66 sub_47a2    ok++
 **************************************************************************/
int sub_47a2(register operand_t const *po_arg, int p2) {
    operand_t po_local;
    operand_t* po = &po_local;

#ifdef DEBUG
    pr_warning("sub_47a2");
#endif

    if (!po_arg) {
        return false;
    }

    ReadMem((char*)&po_local, (char*)po_arg, sizeof(operand_t));

    if (po->type != (uint8_t)T_REG) {
        return false;
    }
    if (p2 & regTestMasks[po->aux]) {
        return true;
    }
    return false;
}

