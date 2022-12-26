/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File part21.c created 14.08.2021, last modified 11.11.2021.
 *
 * The part21.c file is part of the restored optimization program
 * from the Hi-Tech C compiler v3.09 package.
 *
 *	Andrey Nikitin & Mark Ogden 11.11.2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "optim1.h"

/**************************************************************************
 36 sub_1ec1    ok++    Used in: optimize
 **************************************************************************/
void sub_1ec1() {
    int l1;
    bool vL,vR;

#ifdef DEBUG
    pr_warning("sub_1ec1");
#endif

    sub_4601();
    for (gPi = root; gPi; gPi = gPi->pNext) {

        vL = IsValid((char*)gPi->iLhs);
        vR = IsValid((char*)gPi->iRhs);

        switch (gPi->type) {
            case T_LD:

                if (!key_r) {
                    if (sub_24c0()) {
                        continue;
                    }
                    if (vL &&
			/* gPi->iLhs->type */ GetByte((char*)(gPi->iLhs)+OFF_type) == (uint8_t)T_REG &&
			/* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux) == (uint8_t)REG_A &&
			sub_4682(gPi->iRhs) &&
                        !sub_47e0(REG_F, gPi->pNext, gPi)) {
                        gPi->iRhs   = NULL;
                        gPi->type   = T_3;
                        gPi->aux    = I_XOR; /* xor */
                        gPi->opCode = NULL;
                        logOptimise(O_XOR_A); /* 6fbf opt_msg[14] = "xor a's used" */
                    }
                }
                break;

            case T_SYMBOL:
                for (; gPi->type == T_SYMBOL && gPi->aux == 0; gPi = gPi->pNext) {
                    removeInstr(gPi);
                    logOptimise(O_UNREF_LAB); /* 6fc5 opt_msg[4] = "Unref'ed labels" */
                }
            /* fall through */

            case T_TWOBYTE:
            case T_CALL:
                sub_4601();
                break;

            case T_EX:
                if (vL && /* gPi->iLhs->type */ GetByte((char*)(gPi->iLhs)+OFF_type) == (uint8_t)T_REG) {
                    if (sub_23c1()) {
                        continue;
                    }
                    swapHLDE();
                } else {
                    if (vR)
			sub_4544(/* gPi->iRhs->aux */ GetByte((char*)(gPi->iRhs)+OFF_aux));
                }
                break;

            case T_SIMPLE:
                switch (gPi->aux) {
                    case I_NOP:
                    case I_SCF:
                    case I_CCF:
                    case I_HALT:
                    case I_DI:
                    case I_EI:
                        break;
                    case I_EXX:
                        sub_4601();
                        break;
                    default:
                        sub_4544(REG_AF);
                        break;
                }
                break;

            case T_BIT:
                if (gPi->aux != 0x40)
                    if (vR && /* gPi->iRhs->type */ GetByte((char*)(gPi->iRhs)+OFF_type) == (uint8_t)T_REG) {
                        sub_4544(/* gPi->iRhs->aux */ GetByte((char*)(gPi->iRhs)+OFF_aux));
                    } else if ((l1 = sub_46b1(gPi->iRhs, 0)) != -1) {
                        sub_4544(l1);
                    }
                break;
caseCommon:
            case T_CARR:
                if (vL && /* gPi->iLhs->type */ GetByte((char*)(gPi->iLhs)+OFF_type) == (uint8_t)T_REG &&
		    !sub_47e0(/* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux), gPi->pNext, gPi)) {
                    if ((gPi->type == T_INCDEC &&
			 /* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux) >= (uint8_t)REG_BC) ||
			 !sub_47e0(REG_F, gPi->pNext, gPi)) {
                        removeInstr(gPi);
                        logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
                        continue;
                    }
                }
                /* fall through */ /* m25 */

            case T_SHIFT:
            case T_0xE:
                if (vL && /* gPi->iLhs->type */ GetByte((char*)(gPi->iLhs)+OFF_type) == (uint8_t)T_REG) {
                    sub_4544(/* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux));
                } else {
                    sub_44b2(gPi->iLhs);
                }
                break;

            case T_CADD:
                if (vL && !sub_47e0(/* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux), gPi->pNext, gPi)) {
                    removeInstr(gPi);
                    logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
                    continue;
                } else if (vR && /* gPi->iRhs->type */ GetByte((char*)(gPi->iRhs)+OFF_type) == (uint8_t)T_CONST &&
			 abs(/* gPi->iRhs->oVal */ GetWord((char*)(gPi->iRhs)+OFF_oVal)) == 1 &&
			 !/* gPi->iRhs->oPOperand */ GetWord((char*)(gPi->iRhs)+OFF_oPOperand)) {
                    gPi->type   = T_INCDEC;
                    gPi->aux    = /* gPi->iRhs->oVal */ GetWord((char*)(gPi->iRhs)+OFF_oVal) != 1 ? SI_DEC : SI_INC;
                    gPi->opCode = NULL;
                    freeOperand(gPi->iRhs);
                    gPi->iRhs = NULL;
                    logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
                } else {
                    goto caseCommon;
                }
            /* fall through */

            case T_INCDEC:
                if (vL && /* gPi->iLhs->type */ GetByte((char*)(gPi->iLhs)+OFF_type) != (uint8_t)T_REG) {
                    goto caseCommon;
                }
                if (vL && /* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux) != (uint8_t)REG_HL) {
                    goto caseCommon;
                }

                if (sub_2ef8()) {
                    continue;
                }
                break;

            case T_DJNZ:
                sub_4544(0);
                break;

            case T_STK:
		if (gPi->aux == I_PUSH) {
                    if (vL && /* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux) == (uint8_t)REG_IY &&
			 (l1 = sub_46b1(&regValues[17], REG_IY)) != -1 && l1 != REG_IY) {
                        /* gPi->iLhs->aux = l1; */
			PutByte((char*)(gPi->iLhs)+OFF_aux, (char)l1);
                        logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
                    }
                    /* check for pop */
                    /* OTIMISER: fails to optimise the gPi->pNext-aux below */
                    if (gPi->pNext->type == T_STK &&
			gPi->pNext->aux == I_POP &&
			vL && IsValid((char*)gPi->pNext->iLhs) &&
			 /* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux) ==
			 /* gPi->pNext->iLhs->aux */ GetByte((char*)(gPi->pNext->iLhs)+OFF_aux)) {
                        removeInstr(gPi->pNext);
                        removeInstr(gPi);
                        logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
                    } else {
                        break;
                    }
                } else {
                    if (vL)
			sub_4544(/* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux)); /* m39: */
                    if (vL && IsValid((char*)gPi->pAlt->iLhs) && gPi->pAlt->type == T_STK && gPi->pAlt->aux == I_PUSH) {
                        regValues[/* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux)] = 
			regValues[/* gPi->pAlt->iLhs->aux */ GetByte((char*)(gPi->pAlt->iLhs)+OFF_aux)];
#ifdef PRINT_REG_VAL
 PrintRegVal(GetByte((char*)(gPi->iLhs)+OFF_aux)); 
#endif
                        if (vL && IsValid((char*)gPi->pAlt->iLhs) && /* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux) == (uint8_t)REG_IY &&
			    gPi->pNext->type == T_STK &&
			    /* gPi->pNext->iLhs->aux */ GetByte((char*)(gPi->pNext->iLhs)+OFF_aux) == (uint8_t)REG_IY) {
                            /* gPi->pNext->iLhs->aux = gPi->pAlt->iLhs->aux; */
			    PutByte((char*)(gPi->pNext->iLhs)+OFF_aux, GetByte((char*)(gPi->pAlt->iLhs)+OFF_aux));
                            logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
                        }
                    }
                    break;
                }
                continue;

            case T_3:
                if (sub_2d3b()) {
                    continue;
                }
                break;
        }
        sub_2bdb();
    }
}

/**************************************************************************
 37 sub_23c1    ok++
 **************************************************************************/
bool sub_23c1() {
    register inst_t *pi;
    uint8_t aux = GetByte((char*)(gPi->iLhs)+OFF_aux);

#ifdef DEBUG
    pr_warning("sub_23c1");
#endif

    if ((pi = gPi->pNext)->type == T_STK &&
	 pi->aux == I_PUSH &&
	 /* pi->iLhs->aux */ GetByte((char*)(pi->iLhs)+OFF_aux) == /* gPi->iLhs->aux */ aux &&
         !sub_47e0(/* gPi->iLhs->aux */ aux, pi->pNext, gPi) &&
	 !sub_47e0(/* gPi->iRhs->aux */ GetByte((char*)(gPi->iRhs)+OFF_aux), pi, gPi)) {
        /* pi->iLhs->aux = gPi->iRhs->aux; */
	PutByte((char*)(pi->iLhs)+OFF_aux, GetByte((char*)(gPi->iRhs)+OFF_aux));
        removeInstr(gPi);
        gPi = pi;
    } else if (gPi->pNext->type == T_EX &&
	       /* gPi->pNext->iLhs->type */ GetByte((char*)(gPi->pNext->iLhs)+OFF_type) == (uint8_t)T_REG) {
        removeInstr(gPi->pNext);
        removeInstr(gPi);
    } else {
        return false;
    }
    return logOptimise(O_RED_EX_DEHL); /* 6fdb opt_msg[15] = "Redundant ex de,hl's" */
}
