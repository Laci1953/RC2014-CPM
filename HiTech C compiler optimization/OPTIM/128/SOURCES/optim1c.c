/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include <setjmp.h>

#include "optim1.h"

/**************************************************************************
 20	chkIXYUsage	sub_0e67	ok++	Used in: optimise
 **************************************************************************/
void chkIXYUsage() {
    register inst_t *pi;
    uint8_t typeL;
    bool vL, vR;

#ifdef DEBUG
    pr_warning("chkIXYUsage");
#endif

    usesIXorIY = false;
    for (pi = root; pi; pi = pi->pNext) 
    {
        vL = pi->iLhs && IsValid((char*)pi->iLhs);
        vR = pi->iRhs && IsValid((char*)pi->iRhs);

        if (pi->type != T_SYMBOL &&
            ((vL && (typeL = GetByte((char*)(pi->iLhs)+OFF_type)) == (uint8_t)T_INDEXED) || 
	     (vR && GetByte((char*)(pi->iRhs)+OFF_type) == (uint8_t)T_INDEXED) ||
              (vL && typeL == (uint8_t)T_REG && GetByte((char*)(pi->iLhs)+OFF_aux) >= (uint8_t)REG_IX))) 
	{
            usesIXorIY = true;
            return;
        }
    }
}

/**************************************************************************
 21	sub_0ed1			ok++	used in optimise
 **************************************************************************/

    operand_t *po;
    bool       newLabel; /* Flag of the sub_0ca2 call */
    inst_t *pi1;
    uint8_t ty;
    bool valid_po;

void sub_0ed1() {
register     inst_t    *pi2;

#ifdef DEBUG
    pr_warning("sub_0ed1");
#endif

    for (pi1 = word_6ffc; pi1; pi1 = pi1->pAlt) 
    {
        if (pi1->type == T_CONST) 
	{
            newLabel = false;
            logOptimise(O_TMP_LAB); /* 6fc3 opt_msg[3] = "Temporary labels" */
            for (pi2 = pi1->pAlt; pi2; pi2 = pi2->pAlt) {
                if (pi2->type == T_CONST && pi2->aux == pi1->aux) 
                    break;

		po = pi2->iLhs;

		valid_po = IsValid((char*)po);

		if (valid_po)
		    ty = GetByte((char*)po+OFF_type);
		else
		    ty = 0x7F;

                if (valid_po && 
		     /* po->type */ ty == (uint8_t)T_FWD && 
		     /* po->oVal */ (int)GetWord((char*)po+OFF_oVal) == pi1->aux) 
		{
                    if (!newLabel) 
		    {
                        syntheticLabel(pi1);
                        newLabel = true;
                    }

                    /* po->type      = T_CONST; */
		    PutByte((char*)po+OFF_type, (uint8_t)T_CONST);

                    /* po->oPOperand = pi1->pNext->iLhs; */
		    PutWord((char*)po+OFF_oPOperand, (short)pi1->pNext->iLhs);

                    /* po->oVal      = 0; */
		    PutWord((char*)po+OFF_oVal, (short)0);
                }
            }
            if (pi1->aux < REG_BC) 
	    { /* m7: */
                if (!newLabel) 
                    syntheticLabel(pi1);

                for (pi2 = pi1->pNext; pi2; pi2 = pi2->pNext) 
		{

		    po = pi2->iLhs;

		    valid_po = IsValid((char*)po);

		    if (valid_po)
		        ty = GetByte((char*)po+OFF_type);
		    else
		        ty = 0x7F;
		    
                    if (valid_po && 
			/* po->type */ ty == (uint8_t)T_BWD && 
			/* po->oVal */ (int)GetWord((char*)po+OFF_oVal) == pi1->aux) 
		    {

                        /* po->type      = T_CONST; */
		        PutByte((char*)po+OFF_type, (uint8_t)T_CONST);

                        /* po->oPOperand = pi1->pNext->iLhs; */
		        PutWord((char*)po+OFF_oPOperand, (short)pi1->pNext->iLhs);

                        /* po->oVal      = 0; */
		        PutWord((char*)po+OFF_oVal, (short)0);

                        break;
                    }
                }
            }

            pi1 = pi1->pNext; /* m12: */
            removeInstr(pi1->pAlt);
        }
    }
}

/**************************************************************************
 22	sub_1071			ok++	used in sub_15ad
 **************************************************************************/
    inst_t *pi_1;
    inst_t *pi_2;
    inst_t *pi_3;
    sym_t * s_1071;

bool sub_1071(register inst_t *pi) {

#ifdef DEBUG
    pr_warning("sub_1071");
#endif

    if (pi->type != T_JP || pi->aux != 0 ||
	 /* pi->iLhs->type */ GetByte((char*)(pi->iLhs)+OFF_type) != (uint8_t)T_CONST) {
        return false;
    }

    /* pi_1 = pi->iLhs->oPSym->p.pInst; */
    s_1071 = (sym_t*)GetWord((char*)(pi->iLhs)+OFF_oPSym);

    if (!s_1071)
	pr_error("oPSym is NULL");

    pi_1 = s_1071->p.pInst;

    while (pi_1->type == T_SYMBOL) {
        pi_1 = pi_1->pAlt;
    }

    if (pi_1 == pi) {
        removeInstr(pi);
        logOptimise(O_JMP_TO_PLUS1); /* 6fc9 opt_msg[6] = "Jumps to .+1" */
        return false;
    }
    if (pi_1->type != T_JP || pi_1->aux != 0) {
        return false;
    }

    for (pi_3 = pi_1->pNext; pi_3->type && (pi_3->type != T_JP || pi_3->aux != 0); pi_3 = pi_3->pNext)
        ;

    if (pi_3 == pi_1->pNext || pi_3->type == T_INVALID || pi_3 == pi) {
        return false;
    }

    pi->pNext->pAlt  = pi_3;
    pi_1->pNext->pAlt = pi;

    pi_3->pNext->pAlt = pi_1;

    pi_2              = pi_1->pNext;
    pi_1->pNext       = pi_3->pNext;
    pi_3->pNext       = pi->pNext;
    pi->pNext        = pi_2;
    logOptimise(O_CODE_MOTIONS); /* 6fdd opt_msg[16] = "Code motions" */
    removeInstr(pi);
    return logOptimise(O_JMP_TO_PLUS1); /* 6fc9 opt_msg[6] = "Jumps to .+1" */
}
