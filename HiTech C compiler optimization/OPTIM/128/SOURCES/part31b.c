/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File part31.c created 14.08.2021, last modified 11.11.2021.
 *
 * The part31.c file is part of the restored optimization program
 * from the Hi-Tech C compiler v3.09 package.
 *
 *      Andrey Nikitin & Mark Ogden 11.11.2021
 */

#include <stdio.h>
/* #include <stdlib.h> */
/* #include <string.h> */
#include <setjmp.h>
#include "optim1.h"

extern int regTestMasks[];

/**************************************************************************
 54 sub_3a15    ok++ (PMO)
 **************************************************************************/
/* void PrintOp(operand_t* o); */

    sym_t  *psF;
    inst_t *l2;
    inst_t *l3;
    int     fpBase;
    int vF;
    operand_t tmpOp;

void loadFunction() {
register     inst_t *piF;

#ifdef DEBUG
    pr_warning("loadFunction");
#endif

    piF = root = (inst_t *)alloc_mem(sizeof(inst_t));
    piF->pNext = (inst_t *)alloc_mem(sizeof(inst_t));
    HEAP(piF->pNext);
    piF->pNext->pAlt = piF;
    piF              = piF->pNext;
    l2 = switchVectors = word_6fee = (inst_t *)alloc_mem(sizeof(inst_t));

    for (;; HEAP(piF->iRhs))
    {
        tokType = get_token();
        HEAP(piF->iRhs);

        for (;; HEAP(piF->pNext))
        {
            if (tokType == T_EOL)
            {
                clr_len_inbuf();
                break;
            }

            if (piF->type != T_INVALID)
                piF = allocInst(piF);    /* m3: */

            if (tokType == -1)
            {    /* m4: */
                word_6ffc = piF;
                word_6fee = l2;
                return;
            }

            piF->type = tokType; /* m5: */

            if (psect == SWDATA)
            {
                if (tokType == T_DEFW)
                { /* collect the switch table */
                    piF->opCode = yytext;
                    getOperands(piF);
                    l2->pNext       = piF;
                    piF              = piF->pAlt;
                    piF->pNext       = 0; /* NULL */ ;
                    l2->pNext->pAlt = l2;
                    l2              = l2->pNext;
                    break;
                }

                psect = DATA; /* revert to normal data handling */
            }

            switch (piF->type = tokType)
            { /* m7: */
                case T_CONST:
                    if ((psect == DATA) || (psect == BSS))
                    {
                        piF->type = T_INVALID;
                        pr_psect(psect);
                        printf("%d:\n", yylval.i);
                    }
                    else
                    {
                        piF->aux = yylval.i; /* m10: */
                        l3      = piF->pAlt;

                        if (piF->pAlt->type == T_JP &&
                            /* l3->iLhs->type */ GetByte((char*)(l3->iLhs)+OFF_type) == (uint8_t)T_FWD &&
                            /* l3->iLhs->oVal */ GetWord((char*)(l3->iLhs)+OFF_oVal) == piF->aux)
                            removeInstr(l3);
                    }

                    tokType = get_token(); /* m11: */

                    if (tokType != T_COLN)
                        pr_error("Bad temp label");

                    break;

                case T_SYMBOL:
                    psF      = yylval.pSym;
                    piF->aux = 0;
                    tokType = get_token();

                    if (tokType == T_EQU)
                    {
                        if (psF->label[0] != 'f')
                            pr_error("Unknown EQU"); /* compiler generated equ names begin with f */

                        piF->type = T_INVALID;
                        tokType  = get_token();
                        piF->iLhs = evalOperand();
                        /* check is constant with no unresolved symbol ref */

                        if (/* piF->iLhs->type */ GetByte((char*)(piF->iLhs)+OFF_type) != (uint8_t)T_CONST ||
                            /* piF->iLhs->oPSym */ GetWord((char*)(piF->iLhs)+OFF_oPSym) )
                            pr_error("Bad arg to EQU");

                        fpBase    = /* piF->iLhs->oVal */ GetWord((char*)(piF->iLhs)+OFF_oVal);
                                                 /* the frame pointer offset to lowest local (will be 0 or -ve) */
                        word_6ffc = piF;
                        word_6fee = l2;
                        piF        = root;

                        do
                        { /* update any references to the frame size */

                            if (piF->iRhs &&
                                IsValid((char*)piF->iRhs) &&
                                /* piF->iRhs->type */ GetByte((char*)(piF->iRhs)+OFF_type) == (uint8_t)T_CONST &&
                                /* piF->iRhs->oPSym */ (sym_t*)GetWord((char*)(piF->iRhs)+OFF_oPSym) == psF)
                            {
                                /* piF->iRhs->oVal += fpBase; */
                                vF = GetWord((char*)(piF->iRhs)+OFF_oVal);
                                PutWord((char*)(piF->iRhs)+OFF_oVal, vF + fpBase);
                                /* piF->iRhs->oPSym = NULL; */
                                PutWord((char*)(piF->iRhs)+OFF_oPSym, (short)0);
                            }

                            if (piF->iLhs &&
                                IsValid((char*)piF->iLhs) &&
                                /* piF->iLhs->type */ GetByte((char*)(piF->iLhs)+OFF_type) == (uint8_t)T_CONST &&
                                /* piF->iLhs->oPSym */ (sym_t*)GetWord((char*)(piF->iLhs)+OFF_oPSym) == psF)
                            {
                                /* piF->iLhs->oVal += fpBase; */
                                vF = GetWord((char*)(piF->iLhs)+OFF_oVal);
                                PutWord((char*)(piF->iLhs)+OFF_oVal, vF + fpBase);
                                /* piF->iLhs->oPSym = NULL; */
                                PutWord((char*)(piF->iLhs)+OFF_oPSym, (short)0);
                            }

                        }
                        while (piF = piF->pNext);

                        return;
                    }

                    piF->iPSym   = psF;

                    psF->p.pInst = piF;

                    piF->aux     = INT_MAX;

                    if (psect == DATA && psF->label[0] == 'S')
                    { /* compiler generated switch tables start with S */
                        psect           = SWDATA;
                        l2->pNext       = piF;
                        piF              = piF->pAlt;
                        piF->pNext       = /* NULL */ 0;
                        l2->pNext->pAlt = l2;
                        l2              = l2->pNext;
                    }

                    if (psect == DATA || psect == BSS) {
                        piF->type    = T_INVALID;
                        piF->iLhs    = /* NULL */ 0;
                        psF->p.pInst = /* NULL */ 0;
                        pr_psect(psect);
                        printf("%s:\n", psF->label);
                    }
                    else if (psF->label[0] == '_')
                        name_fun = psF->label; /* external name */

                    if (tokType == T_COLN)
                        break;

                    continue; /* inner loop */

                case 255: /* -1 */
                    piF->type  = T_INVALID;
                    word_6ffc = piF;
                    return;

                case T_DEFW:
                case T_DEFB:
                    if (psect == TEXT)
                        goto case_default;

                case T_DEFM:
                case T_DEFS:
                case T_DEFF:
                    if (psect == TEXT)
                        pr_error("DEF[BMSF] in text psect");

                    pr_psect(psect);
                /* fall through */
                case T_GLB:
                    printf("%s\t%s\n", yytext, ptr_token());
                    piF->type = T_INVALID;
                    break;

                case T_PSCT:
                    psect    = num_psect(ptr_token()); /* m30: */
                    piF->type = T_INVALID;
                    break;

                case T_JR:
                    piF->type = T_JP; /* convert to jp so it is safe to move code */
                    yytext   = "jp";
                /* fall through */
                default:
case_default:
                    piF->opCode = yytext;
                    piF->aux    = yylval.i;
                    if (tokType == T_JP || tokType == T_CALL)
                        expectCond = true; /* set if can have conditional */
                    else
                        expectCond = false;

                    getOperands(piF);

                    if ((piF->type == T_JP) || (piF->type == T_CALL))
                    {
                        if (/* piF->iLhs->type */ GetByte((char*)(piF->iLhs)+OFF_type) == (uint8_t)T_COND)
                        {
                                                /* if cond then hoist condition and remove lhs */
                            piF->aux  = /* piF->iLhs->aux */ GetByte((char*)(piF->iLhs)+OFF_aux);
                            piF->iLhs = piF->iRhs;
                            piF->iRhs = /* NULL */ 0;
                        }
                    }

                    if (piF->type == T_JP &&
                        piF->aux == 0 &&
                        /* piF->iLhs->type */ GetByte((char*)(piF->iLhs)+OFF_type) != (uint8_t)T_REGREF &&
                        (l3 = piF->pAlt)->type == T_CONST && l3->pAlt->type == T_JP && l3->pAlt->aux == 0)
                    {

                        while (l3 = l3->pAlt)
                        {
                            if (l3->type == T_JP &&
                                /* l3->iLhs->type */ GetByte((char*)(l3->iLhs)+OFF_type) == (uint8_t)T_FWD &&
                                /* l3->iLhs->oVal */ GetWord((char*)(l3->iLhs)+OFF_oVal) == piF->pAlt->aux)
                            {
                             /* *l3->iLhs = *piF->iLhs; */
                                ReadMem((char*)&tmpOp, (char*)piF->iLhs, sizeof(operand_t));
                                WriteMem((char*)&tmpOp, (char*)l3->iLhs, sizeof(operand_t));
                            }
                            else if (l3->type == T_CONST && l3->aux == piF->pAlt->aux)
                                break;
                        }

                        removeInstr(piF->pAlt);
                        freeOperand(piF->iLhs);
                        piF->type   = T_INVALID;
                        piF->iLhs   = /* NULL */ 0;
                        piF->opCode = /* NULL */ 0;
                    }
                    break;
            }
            break; /* to outer loop */
        }
/* -------------------------------------------------------
        fprintf(stderr, "PI %04x was processed...", piF);
        fprintf(stderr, "OPERAND ");
        PrintOp((operand_t*)0xFB4A);
 ---------------------------------------------------------- */
    }
}

