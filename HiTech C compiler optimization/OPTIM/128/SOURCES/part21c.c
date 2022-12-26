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
/* #include <stdlib.h> */
/* #include <string.h> */
#include <setjmp.h>
#include "optim1.h"

/**************************************************************************
 38 sub_24c0    ok++
 **************************************************************************/

    operand_t    *poL,*poR;
    int           reg;
    uint8_t aux, auxR, auxL;
    uint8_t typ, typeR, typeL;
    operand_t    *op;
    bool	 validL, validR;

bool sub_24c0() 
{
register     inst_t 	 *ip;

#ifdef DEBUG
    pr_warning("sub_24c0");
#endif

    if (sub_3053()) 
        return true;

    poL=gPi->iLhs;
    poR=gPi->iRhs;

    if (poR)
    {
        ReadMem((char*)&lo1, (char*)poR, sizeof(operand_t));
        typeR = plo1->type;
        auxR = plo1->aux;
	validR = true;
    }
    else
        validR = false;

    if (poL)
    {
        ReadMem((char*)&lo2, (char*)poL, sizeof(operand_t));
        typeL = plo2->type;
        auxL = plo2->aux;
	validL = true;
    }
    else
        validL = false;

    if (validL && validR && 
	(/* gPi->iLhs->type */ typeL == (uint8_t)T_REG &&
           (	/* gPi->iRhs->type */ typeR == (uint8_t)T_INDEXED ||
	     	/* gPi->iRhs->type */ typeR == (uint8_t)T_ADDRREF ||
	     	/* gPi->iRhs->type */ typeR == (uint8_t)T_CONST	 )	)	)
    {
        if (sub_29c3())
            return true;
    } 
    else if (validL && validR &&
	    /* gPi->iRhs->type */ typeR == (uint8_t)T_CONST &&
	     /* gPi->iLhs->type */ typeL == (uint8_t)T_INDEXED) 
    {
	reg = sub_46b1(poR, REG_A);

        if (reg != -1)
	{
            /* gPi->iRhs->type = T_REG; */ /* m6: */
	    PutByte((char*)(poR)+OFF_type, (uint8_t)T_REG);
            /* gPi->iRhs->aux  = reg; */
	    PutByte((char*)(poR)+OFF_aux, (uint8_t)reg);
            return logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
        }

        sub_44b2(/* gPi->iLhs */ poL);
    }
    else if (validL && validR &&
	    /* gPi->iRhs->type */ typeR == (uint8_t)T_REG &&
	       (	/* gPi->iLhs->type */ typeL == (uint8_t)T_INDEXED ||
			/* gPi->iLhs->type */ typeL == (uint8_t)T_ADDRREF	)	) 
    {
        if (operandsSame(/* gPi->iLhs */ poL, &regValues[/* gPi->iRhs->aux */ auxR]))
	{
kill:
            removeInstr(gPi);
            return logOptimise(O_RED_LD); /* 6fd5 opt_msg[12] = "Redundant loads/stores" */
        }

        sub_44b2(/* gPi->iLhs */ poL);

        if (regValues[/* gPi->iRhs->aux */ auxR].type == (uint8_t)T_INVALID)
	{
            sub_4544(/* gPi->iRhs->aux */ auxR);
            /* regValues[gPi->iRhs->aux] = *gPi->iLhs; */
	    ReadMem((char*)&regValues[auxR], (char*)poL, sizeof(operand_t));
#ifdef PRINT_REG_VAL
 PrintRegVal(auxR);
#endif
        }
    }
    else if (validL && validR && 
	     /* gPi->iLhs->type */ typeL == (uint8_t)T_REG &&
	     /* gPi->iRhs->type */ typeR == (uint8_t)T_REG) 
    { /* 2824 */
        if (/* gPi->iLhs->aux */ auxL == /* poR->aux */ auxR ||
	    operandsSame(&regValues[/* gPi->iLhs->aux */ auxL], &regValues[/* gPi->iRhs->aux */ auxR])) 
            goto kill;

	seq2 = gPi->pNext;

        if (seq2->type == T_LD && seq2->iLhs && seq2->iRhs &&
	    operandsSame(seq2->iLhs, /* gPi->iRhs */ poR) &&
            operandsSame(seq2->iRhs, /* gPi->iLhs */ poL)) 
	{
            removeInstr(seq2);
            logOptimise(O_RED_LD); /* 6fd5 opt_msg[12] = "Redundant loads/stores" */
        }

        if (!sub_47e0(/* gPi->iLhs->aux */ auxL, /* gPi->pNext */ seq2, gPi)) 
            goto kill;

	if (seq2->iRhs && IsValid((char*)seq2->iRhs))
            ReadMem((char*)&lo3, (char*)seq2->iRhs, sizeof(operand_t));
	else
	    plo3->type = 0x7F; /* invalid */

	if (seq2->iLhs && IsValid((char*)seq2->iLhs))
            ReadMem((char*)&lo4, (char*)seq2->iLhs, sizeof(operand_t));
	else
	    plo4->type = 0x7F; /* invalid */

        if (/* gPi->iLhs->aux */ auxL == REG_E &&
	    /* gPi->iRhs->aux */ auxR == REG_L &&
             seq2->type == T_LD &&
             /* seq2->iLhs->type */ plo4->type == (uint8_t)T_REG &&
	     /* seq2->iLhs->aux */ plo4->aux == (uint8_t)REG_D &&
	     /* seq2->iRhs->type */ plo3->type == (uint8_t)T_REG &&
             /* seq2->iRhs->aux */ plo3->aux == (uint8_t)REG_H &&
	    !sub_47e0(REG_HL, seq2->pNext, gPi)) 
	{
            removeInstr(gPi->pNext);
            gPi->type      = T_EX;
            gPi->opCode    = NULL;
            /* gPi->iLhs->aux = REG_DE; */
	    PutByte((char*)(poL)+OFF_aux, (uint8_t)REG_DE);

            /* gPi->iRhs->aux = REG_HL; */
	    PutByte((char*)(poR)+OFF_aux, (uint8_t)REG_HL);

            swapHLDE();
            return logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
        }

        sub_4544(/* gPi->iLhs->aux */ auxL);
        regValues[/* gPi->iLhs->aux */ auxL] = regValues[/* gPi->iRhs->aux */ auxR];
#ifdef PRINT_REG_VAL
 PrintRegVal(auxL); 
#endif
    } 
    else if (validL && /* gPi->iLhs->type */ typeL == (uint8_t)T_REG) 
        sub_4544(/* gPi->iLhs->aux */ auxL);

    if (validL && validR &&
	/* gPi->iLhs->type */ typeL == (uint8_t)T_REG &&
        /* gPi->iRhs->type */ typeR == (uint8_t)T_REG) 
    {
	ip = gPi->pAlt;
	op = ip->iLhs;

	if (op && IsValid((char*)op))
	{
	    typ = (uint8_t)GetByte((char*)op+OFF_type);
	    aux = (uint8_t)GetByte((char*)op+OFF_aux);

            if (ip->type == T_LD &&
	       /* op->type */ typ == (uint8_t)T_REG &&
	       /* op->aux */ aux == /* poR->aux */ auxR &&
               !sub_47e0(/* op->aux */ aux, gPi->pNext, ip) &&
	       sub_1369(ip->iRhs) ) 
	    {
                sub_4544(/* op->aux */ aux);

	        /* op->aux = gPi->iLhs->aux; */ 
	        PutByte((char*)op+OFF_aux, (char)auxL);

                /* regValues[op->aux] = *ip->iRhs; */
                ReadMem((char*)&regValues[auxL], (char*)ip->iRhs, sizeof(operand_t));
#ifdef PRINT_REG_VAL
 PrintRegVal(auxL); 
#endif
                goto kill;
            }
	}

	ip = gPi->pNext;
	op = ip->iRhs;

	if (op && IsValid((char*)op))
	{
	    typ = (uint8_t)GetByte((char*)op+OFF_type);
	    aux = (uint8_t)GetByte((char*)op+OFF_aux);

            if ( ip->type == T_LD &&
	         /* op->type */ typ == (uint8_t)T_REG &&
	         /* op->aux */ aux == /* gPi->iLhs->aux */ auxL &&
                 !sub_47e0(/* op->aux */ aux, ip->pNext, gPi) &&
	         sub_1369(ip->iLhs) ) 
	    {
                sub_4544(/* op->aux */ aux);

                /* op->aux = gPi->iRhs->aux; */ 
	        PutByte((char*)op+OFF_aux, (char)auxR);

                goto kill;
            }
	}
    }

    return false; /* m26: */
}

/**************************************************************************
 39 sub_29c3    ok+ (PMO)
 *
 * Code is identical, except that the optimiser moves the code in the
 * first if block to a different location. The jp conditions are changed to
 * reflect this)
 **************************************************************************/

    operand_t *poLL, *poRR;
    uint8_t auxLL;
    uint8_t auxRR;
    int l1_29c3;

bool sub_29c3() {
register     inst_t *pi1_29c3;

#ifdef DEBUG
    pr_warning("sub_29c3");
#endif

    poLL = gPi->iLhs;
    poRR = gPi->iRhs;
    auxLL = GetByte((char*)(poLL)+OFF_aux);
    auxRR = GetByte((char*)(poRR)+OFF_aux);

    if (operandsSame(/* gPi->iRhs */ poRR, &regValues[/* gPi->iLhs->aux */ auxLL]) ||
	!sub_47e0(/* gPi->iLhs->aux */ auxLL, gPi->pNext, gPi)) {
        /* OPTIMISER: this block is located differently */
        removeInstr(gPi);
        return logOptimise(O_RED_LD); /* 6fd5 opt_msg[12] = "Redundant loads/stores" */
    }

    sub_4544(/* gPi->iLhs->aux */ auxLL);

    if (/* gPi->iLhs->aux */ auxLL <= (uint8_t)REG_HL) {
        if ((l1_29c3 = sub_46b1(/* gPi->iRhs */ poRR, /* gPi->iLhs->aux */ auxLL)) != -1) {
            /* code hikes gPi->iLhs->aux before test !!! */
            /* regValues[gPi->iLhs->aux] = *gPi->iRhs; */
	    ReadMem((char*)&regValues[auxLL], (char*)poRR, sizeof(operand_t));
#ifdef PRINT_REG_VAL
 PrintRegVal(auxLL); 
#endif
            /* gPi                       = gPi; ??? */
            /* gPi->iRhs->type           = T_REG; */
	    PutByte((char*)(poRR)+OFF_type, (uint8_t)T_REG);

            /* gPi->iRhs->aux            = l1_29c3; */
	    PutByte((char*)(poRR)+OFF_aux, (uint8_t)l1_29c3);
	    auxRR = l1_29c3;
	    
            if (/* gPi->iLhs->aux */ auxLL >= REG_BC) {
                pi1_29c3             = allocInst(gPi);
                pi1_29c3->iLhs       = allocOperand();
                pi1_29c3->iRhs       = allocOperand();

                /* pi1_29c3->iLhs->type = T_REG; */
   	        PutByte((char*)(pi1_29c3->iLhs)+OFF_type, (uint8_t)T_REG);

                /* pi1_29c3->iRhs->type = T_REG; */
   	        PutByte((char*)(pi1_29c3->iRhs)+OFF_type, (uint8_t)T_REG);

                /* pi1_29c3->iLhs->aux  = regHiLoMap[gPi->iLhs->aux].hiReg; */
   	        PutByte((char*)(pi1_29c3->iLhs)+OFF_aux, regHiLoMap[/* gPi->iLhs->aux */ auxLL].hiReg);

                /* pi1_29c3->iRhs->aux  = regHiLoMap[gPi->iRhs->aux].hiReg; */
   	        PutByte((char*)(pi1_29c3->iRhs)+OFF_aux, regHiLoMap[/* gPi->iRhs->aux */ auxRR].hiReg);

                /* gPi->iLhs->aux  = regHiLoMap[gPi->iLhs->aux].loReg; */
   	        PutByte((char*)(poLL)+OFF_aux, regHiLoMap[/* gPi->iLhs->aux */ auxLL].loReg);

                /* gPi->iRhs->aux  = regHiLoMap[gPi->iRhs->aux].loReg; */
   	        PutByte((char*)(poRR)+OFF_aux, regHiLoMap[/* gPi->iRhs->aux */ auxRR].loReg);

                pi1_29c3->type       = T_LD;
                gPi             = pi1_29c3;
            }
            return logOptimise(O_SIMPLE_ADDR); /* 6fd7 opt_msg[13] = "Simplified addresses" */
        }
    }
    /* regValues[gPi->iLhs->aux] = *gPi->iRhs; */
    ReadMem((char*)&regValues[auxLL], (char*)poRR, sizeof(operand_t));
#ifdef PRINT_REG_VAL
 PrintRegVal(auxLL); 
#endif
    return false;
}

/**************************************************************************
 40 sub_2bdb    ok++ (PMO)
 **************************************************************************/
bool sub_2bdb() {
    register inst_t *pi;

#ifdef DEBUG
    pr_warning("sub_2bdb");
#endif

    if (gPi->type == T_STK &&
	/* gPi->iLhs->aux */ GetByte((char*)(gPi->iLhs)+OFF_aux) == REG_BC &&
	gPi->aux == I_POP &&
	!sub_47e0(REG_BC, gPi->pNext, gPi)) {
        for (pi = gPi->pNext; pi && (pi->type != T_JP && pi->type != T_CALL && pi->type != T_SYMBOL); pi = pi->pNext) {
            if (pi->type == T_STK)
                if (pi->aux != I_PUSH || pi->pNext->type != T_STK || pi->pNext->aux != I_POP) {
                    break;
                } else {
                    pi = pi->pNext;
                }
            if (pi->type == T_EX ||
		 (pi->type == T_LD &&
		  /* pi->iLhs->type */ GetByte((char*)(pi->iLhs)+OFF_type) == (uint8_t)T_REG))
                if (/* pi->iLhs->aux */ GetByte((char*)(pi->iLhs)+OFF_aux) == (uint8_t)REG_SP) {
                    break;
                }
        }

        if (pi->type == T_STK && 
	    pi->aux == I_PUSH &&
	    /* pi->iLhs->aux */ GetByte((char*)(pi->iLhs)+OFF_aux) == (uint8_t)REG_HL &&
	   !sub_47e0(REG_HL, pi->pNext, pi)) {
            removeInstr(gPi);
            pi->opCode     = NULL;
            pi->type       = T_EX;
            pi->aux        = 0;
            pi->iRhs       = pi->iLhs;
            pi->iLhs       = allocOperand();

            /* pi->iLhs->type = T_REGREF; */
	    PutByte((char*)(pi->iLhs)+OFF_type, (uint8_t)T_REGREF);

            /* pi->iLhs->aux  = REG_SP; */
	    PutByte((char*)(pi->iLhs)+OFF_aux, (uint8_t)REG_SP);

            return logOptimise(O_EX_SPHL); /* 6fd1 opt_msg[10] = "Ex (sp),hl'pi used" */
        }
    }
    return false;
}

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


