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
#if 1

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

#else

/*  This function generates correct code */

void sub_1ec1() {
    int l1;
    register inst_t *pi;

    sub_4601();

    gPi = root;
    while (gPi != 0) {                                                        /* m5: */
        pi = gPi;                                                             /* m1: */
        switch (pi->type) {
            case T_LD:
                if (key_r == 0) {                                             /* m6: */
                    if (sub_24c0() != 0) {
                        goto m4;
                    }

                    if (((pi = gPi)->iLhs->type == T_REG)
                            && (pi->iLhs->aux == REG_A)
                            && (sub_4682(pi->iRhs) != 0)
                            && (sub_47e0(REG_F, (pi = gPi)->pNext, gPi) == 0)) {
                        pi       = gPi;
                        pi->iRhs = 0;
                        pi->type = T_3;
                        pi->aux  = P_XOR;
                        gPi->opCode = 0;
                        ++optimiseCnters[14]; /* 6fd9 opt_msg[14] = "Xor a's used" m7: */
                        hasChanged = true;
                    }
                }
                goto m2;

            case T_SYMBOL:
                while (((pi = gPi)->type == T_SYMBOL) && (pi->aux == 0)) {      /* m9: */
                    removeInstr(gPi);                     		/* m8: */
                    optimiseCnters[4]++; /* 6fc5 opt_msg[4] = "Unref'ed labels" */
                    hasChanged = true;
                    gPi = (pi = gPi)->pNext;
                }
                /* fall through */

            case T_TWOBYTE:
            case T_CALL:
m10:
                sub_4601();                                                     /* m10: */
                goto m2;

            case T_EX:
                if ((pi = gPi)->iLhs->type == T_REG) {                          /* m11: */
                    if (sub_23c1() != 0) {
                        goto m3;
                    }
                    swapHLDE();
                    goto m2;
                }
                sub_4544((pi = gPi)->iRhs->aux);                                /* m13: */
                goto m2;

            case T_ONEBYTE:
                switch ((pi = gPi)->aux) {                                      /* m15: */
                    case P_NOP:
                    case P_SCF:
                    case P_CCF:
                    case P_HALT:
                    case P_DI:
                    case P_EI:
                        goto m2;
                    case P_EXX:
                        goto m10;
                    default:
                        sub_4544(REG_AF);
                        break;                                                  /* m16: */
                }
                goto m2;

            case T_BIT:
                if ((pi = gPi)->aux == P_BIT) {
                    goto m2;    						/* m17: */
                }
                if (pi->iRhs->type == T_REG) {
                    sub_4544(pi->iRhs->aux);
                    goto m2;
                }
                if ((l1 = sub_46b1((pi = gPi)->iRhs, 0)) == -1) {
                    goto m2;    						/* m18: */
                }
                sub_4544(l1);
                goto m2;

            case T_CARR:
m19:
                pi = gPi;							/* m19: */
m20:
m21:
                if ((pi = gPi)->iLhs->type == T_REG) {
                    if (sub_47e0(pi->iLhs->aux, (pi = gPi)->pNext, gPi) != 0) {
                        goto m26;
                    }

                    pi = gPi;
                    if ((pi->type != T_INCDEC) || (pi->iLhs->aux < REG_BC)) {
                        if (sub_47e0(REG_F, (pi = gPi)->pNext, gPi) == 0) {
                            goto m23;    					/* m22: */
                        }
                        goto m25;
                    }
m23:
                    removeInstr(gPi);
m24:
                    optimiseCnters[11]++; /* 6fd3 opt_msg[11] = "Redundant operations" */
                    hasChanged = true;
                    goto m3;
                }
                /* fall through */
            case T_6:
            case T_0xE:
m25:
                pi = gPi;
m26:
                if ((pi = gPi)->iLhs->type == T_REG) {
                    sub_4544((pi = gPi)->iLhs->aux);                            /* m27: */
                } else {
                    sub_44b2((pi = gPi)->iLhs);                                 /* m28: */
                }
                goto m2;

            case T_CADD:

                if (sub_47e0(pi->iLhs->aux, (pi = gPi)->pNext, gPi) == 0) {     /* m29: */
m30:
                    removeInstr(gPi);
                    goto m24;
                }
                if ((pi = gPi)->iRhs->type != T_CONST) {
                    goto m20;                                                   /* m31: */
                }
                if (abs(pi->iRhs->oVal) != 1) {
                    goto m20;
                }
                if ((pi = gPi)->iRhs->oPOperand != 0) {
                    goto m20;
                }
                pi->type = T_INCDEC;
                (pi = gPi)->aux = (pi->iRhs->oVal  == 1) ? REG_H : REG_L;       /* m33: */
                gPi->opCode = 0;
                freeOperand(pi->iRhs);
                (pi = gPi)->iRhs = 0;
                optimiseCnters[13]++; /* 6fd7 opt_msg[13] = "Simplified addresses" */
                hasChanged = true;
                /* fall through */

            case T_INCDEC:
                if ((pi = gPi)->iLhs->type != T_REG) {
                    goto m21;    //m34:
                }
                if (pi->iLhs->aux          != REG_HL) {
                    goto m19;
                }
                if (sub_2ef8() != 0) {
                    goto m3;                                                    /* m35: */
                }
                goto m2;

            case T_DJNZ:
                sub_4544(0); //REG_B                                            /* m36: */
                goto m2;

            case T_STK:
                if ((pi = gPi)->aux == P_PUSH) {                                /* m37: */
                    if ((pi = gPi)->iLhs->aux == REG_IY) {
                        l1 = sub_46b1(&regValues[17], REG_IY);
                        if (l1 != -1) {
                            if (l1 !=  REG_IY) {
                                (pi = gPi)->iLhs->aux = pi->type;
                                optimiseCnters[13]++; /* 6fd7 opt_msg[13] = "Simplified addresses" */
                                hasChanged = true;
                            }
                        }
                    }
                    /* Check for pop */
                    if ((pi = gPi)->pNext->type != T_STK) {
                        goto m2;    						/* m38: */
                    }
                    if (pi->pNext->aux != P_POP) {
                        goto m2;
                    }
                    if (pi->iLhs->aux != pi->pNext->iLhs->aux) {
                        goto m2;
                    }
                    removeInstr(pi->pNext);
                    goto m30;
                }
                sub_4544(pi->iLhs->aux);                                        /* m39: */
                if ((pi = gPi)->pAlt->type != T_STK) {
                    goto m2;
                }
                if (pi->pAlt->aux          != P_PUSH) {
                    goto m2;
                }

                /* Ambiguious code - pi could be modified on rhs before using on lhs
                 * on hitech generated code gets lhs before rhs
                 */
                regValues[(pi = gPi)->iLhs->aux] = regValues[pi->pAlt->iLhs->aux];
                if ((pi = gPi)->iLhs->aux != REG_IY) {
                    goto m2;
                }
                if (pi->pNext->type       != T_STK) {
                    goto m2;
                }
                if (pi->pNext->iLhs->aux  != REG_IY) {
                    goto m2;
                }
                pi->pNext->iLhs->aux = pi->pAlt->iLhs->aux;
                ++optimiseCnters[13]; /* 6fd7 opt_msg[13] = "Simplified addresses" */
                hasChanged = true;
                goto m2;

            case T_3:   //   -   -   -   -   -   -   -   -   -   -   -   -   -
                if (sub_2d3b() != 0) {
                    goto m3;                                                    //m40: */
                }
        }
m2:
        sub_2bdb();
m3:
        pi = gPi;
m4:
        gPi = (pi = gPi)->pNext;
    }
}

#endif

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
