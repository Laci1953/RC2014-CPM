#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "optim1.h"

/**************************************************************************
 42 sub_2ef8    ok++
 **************************************************************************/
bool sub_2ef8() {
    operand_t tmp;
    int       l2;

#ifdef DEBUG
    pr_warning("sub_2ef8");
#endif

    if (gPi->pNext->type == T_INCDEC && operandsSame(gPi->iLhs, gPi->pNext->iLhs) && gPi->pNext->aux != gPi->aux) {
        removeInstr(gPi->pNext);
    }

    else if (sub_47e0(REG_HL, gPi->pNext, gPi)) {
        tmp = regValues[REG_TRACKER];
        l2  = hlDelta;
        if (regValues[REG_HL].type != T_INVALID ||
                (regValues[REG_L].type == T_INDEXED && regValues[REG_H].type == T_INDEXED &&
                 regValues[REG_L].aux == regValues[REG_H].aux && regValues[REG_L].oVal + 1 == regValues[REG_H].oVal)) {
            if (regValues[REG_HL].type != T_INVALID) {
                tmp = regValues[REG_HL];
            } else {
                tmp = regValues[REG_L];
            }
            l2 = 0;
        }
        sub_4544(REG_HL);
        regValues[REG_TRACKER] = tmp;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_TRACKER); 
#endif
        hlDelta                = l2;
        if (gPi->aux == SI_INC) {
            ++hlDelta;
        } else {
            --hlDelta;
        }
        return false;
    }
    removeInstr(gPi);
    return logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
}

/**************************************************************************
 42a    sub_3053    ok+-
 **************************************************************************/

    operand_t *pogL, *pogR, *posL, *posR;
    uint8_t auxV;
    int l2_3053;

bool sub_3053() {

#ifdef DEBUG
    pr_warning("sub_3053");
#endif

    pogL = gPi->iLhs;
    pogR = gPi->iRhs;

    auxV = GetByte((char*)pogL, OFF_aux);

    if (/* gPi->iLhs->type */ GetByte((char*)pogL, OFF_type) != (uint8_t)T_REG ||
	(/* l1 = gPi->iLhs->aux */ auxV != REG_HL && auxV != (uint8_t)REG_L) ) {
        return false;
    }

    if (pogR && !operandsSame(/* gPi->iRhs */ pogR, &regValues[REG_TRACKER])) {
        return false;
    }

    l2_3053 = hlDelta;

    if (/* gPi->iLhs->aux */ auxV == (uint8_t)REG_L) {

        seq2 = gPi->pNext;

	posL = seq2->iLhs;
	posR = seq2->iRhs;

        if (seq2->type != T_LD ||
	    ( posL && (/* seq2->iLhs->type */ GetByte((char*)(posL), OFF_type) != (uint8_t)T_REG) ) ||
	    ( posL && (/* seq2->iLhs->aux */ GetByte((char*)(posL), OFF_aux) != (uint8_t)REG_H) ) ||
            ( posR && pogR && (/* seq2->iRhs->oVal */ GetWord((char*)(posR), OFF_oVal) !=
	    /* gPi->iRhs->oVal */ GetWord((char*)(pogR), OFF_oVal) + 1)) ) {
            return false;
        }

        sub_4544(REG_HL);
        /* regValues[REG_L] = gPi->iLhs; */
	ReadMem((char*)&regValues[REG_L], (char*)pogL, 0, sizeof(operand_t));
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_L); 
#endif
        /* regValues[REG_H] = *seq2->iLhs; */
	ReadMem((char*)&regValues[REG_H], (char*)posL, 0, sizeof(operand_t));
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_H); 
#endif
        removeInstr(seq2);
    } else {
        sub_4544(REG_HL);
        /* regValues[REG_HL] = *gPi->iLhs; */
	ReadMem((char*)&regValues[REG_HL], (char*)pogL, 0, sizeof(operand_t));
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_HL); 
#endif
    }

    gPi = gPi->pAlt; /* m4: */
    removeInstr(gPi->pNext);
    while (l2_3053 != 0) {
        gPi             = allocInst(gPi);
        gPi->type       = T_INCDEC;
        gPi->iLhs       = allocOperand();
        /* gPi->iLhs->type = T_REG; */
	PutByte((char*)(gPi->iLhs), OFF_type, (uint8_t)T_REG);
        /* gPi->iLhs->aux  = REG_HL; */
	PutByte((char*)(gPi->iLhs), OFF_aux, (uint8_t)REG_HL);
        if (l2_3053 < 0) {
            ++l2_3053;
            gPi->aux = SI_INC;
        } else {
            --l2_3053;
            gPi->aux = SI_DEC;
        }
    }
    return logOptimise(O_RED_LD); /* 6fd5 opt_msg[12] = "Redundant loads/stores" */
}

/**************************************************************************
 43 sub_31ee    ok++
 **************************************************************************/
void swapHLDE() {
    operand_t pi;

#ifdef DEBUG
    pr_warning("swapHLDE");
#endif

    pi                          = regValues[REG_HL];
    regValues[REG_HL]           = regValues[REG_DE];
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_HL); 
#endif
    regValues[REG_DE]           = pi;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_DE); 
#endif

    pi                          = regValues[REG_H];
    regValues[REG_H]            = regValues[REG_D];
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_H); 
#endif
    regValues[REG_D]            = pi;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_D); 
#endif

    pi                          = regValues[REG_L];
    regValues[REG_L]            = regValues[REG_E];
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_L); 
#endif
    regValues[REG_E]            = pi;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_E); 
#endif
    regValues[REG_TRACKER].type = T_INVALID;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_TRACKER); 
#endif
};

/**************************************************************************
 44 pr_psect    sub_328a    ok++
 **************************************************************************/
void pr_psect(int psect) {

#ifdef DEBUG
    pr_warning("pr_psect");
#endif

    if (psect == cur_psect) {
        return;
    }
    printf("psect\t%s\n", psectNames[psect]);
    cur_psect = psect;
}

/**************************************************************************
 45 num_psect   sub_32bf    ok++
 **************************************************************************/
int num_psect(char const *fmt) {
    int l1;

#ifdef DEBUG
    pr_warning("num_psect");
#endif

    for (l1 = 0; l1 < 4; ++l1) {
        if (strcmp(fmt, psectNames[l1]) == 0) {
            return l1;
        }
    }
    pr_error("Unknown psect");
}

