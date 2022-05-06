#include <stdio.h>
/* #include <stdlib.h> */
/* #include <string.h> */
#include <setjmp.h>
#include "optim1.h"

/**************************************************************************
 41 sub_2d3b    ok++ (PMO)
 *
 * six of the basic blocks are in different positions but the code in the
 * block is the same and the logic flow is maintained.
 **************************************************************************/
    uint8_t type_2d3b;
    uint8_t aux_2d3b;

bool sub_2d3b() {
register     operand_t *po_2d3b;

#ifdef DEBUG
    pr_warning("sub_2d3b");
#endif

    po_2d3b = gPi->iLhs;
    type_2d3b = GetByte((char*)po_2d3b+OFF_type);
    aux_2d3b = GetByte((char*)po_2d3b+OFF_aux);

    switch (gPi->aux) {
        default:
            if (sub_4682(/* gPi->iLhs */ po_2d3b)) 
	    {
                gPi->aux = I_OR;
            } 
	    else if (/*(po_2d3b = gPi->iLhs)->type */ type_2d3b != (uint8_t)T_REG || 
			/* po_2d3b->aux */ aux_2d3b != (uint8_t)REG_A) 
	    {
                sub_4544(REG_AF);
                return false;
            } else 
	    {
                break;
            }
        /* fall through */
        case I_OR:
            			 /* case 0xB0 */
            if (sub_4682(po_2d3b)) {
                /* po_2d3b->type = T_REG; */
		PutByte((char*)po_2d3b+OFF_type, (uint8_t)T_REG);

                /* po_2d3b->aux  = REG_A; */
		PutByte((char*)po_2d3b+OFF_aux, (uint8_t)REG_A);
            }
        /* fall through */
        case I_SUB:
	caseP_SUB: /* common */
            if (/* gPi->iLhs->type */ type_2d3b == (uint8_t)T_REG &&
		/* gPi->iLhs->aux */ aux_2d3b == (uint8_t)REG_A) 
	    {
                if (gPi->aux == I_SUB) 
		{
                    break;
                }
		else if (!sub_47e0(REG_F, gPi->pNext, gPi)) 
		{
                    removeInstr(gPi);
                    return logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
                }
                return false;
            }
            sub_4544(REG_AF);
            return false;
        case I_AND:
            if (sub_4682(/* gPi->iLhs */ po_2d3b)) {
                break;
            }
            if (/* po_2d3b->type */ type_2d3b == (uint8_t)T_CONST &&
		/* !po_2d3b->oPOperand */ !GetWord((char*)po_2d3b+OFF_oPOperand) && 
		/* (po_2d3b->oVal & 0xff) == 255 ) { */
		( (uint8_t)GetByte((char*)po_2d3b+OFF_oVal) == 255 ) /* -1 */) 
	    {
                gPi->aux = I_OR;
                /* po_2d3b->type = T_REG; */
		PutByte((char*)po_2d3b+OFF_type, (uint8_t)T_REG);

                /* po_2d3b->aux  = REG_A; */
		PutByte((char*)po_2d3b+OFF_aux, (uint8_t)REG_A);
            }
            goto caseP_SUB;

        case I_CP:
            return false;
    }

    if (!sub_47e0(REG_F, gPi->pNext, gPi) && sub_4682(&regValues[REG_A])) { 
        removeInstr(gPi);
        return logOptimise(O_RED_OPS); /* 6fd3 opt_msg[11] = "Redundant operations" */
    }

    gPi->aux                   = I_XOR;
    gPi->opCode                = NULL;

    /* gPi->iLhs->type            = T_REG; */
    PutByte((char*)(gPi->iLhs)+OFF_type, (uint8_t)T_REG);

    /* gPi->iLhs->aux             = REG_A; */
    PutByte((char*)(gPi->iLhs)+OFF_aux, (uint8_t)REG_A);

    regValues[REG_A].type      = T_CONST;
    regValues[REG_A].oPOperand = NULL;
    regValues[REG_A].oVal      = 0;
#ifdef PRINT_REG_VAL
 PrintRegVal(REG_A); 
#endif
    return false;
}

