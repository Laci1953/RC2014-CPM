#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "optim1.h"

/**************************************************************************
 50 rel_err     ok++
 **************************************************************************/
void rel_err() {

    pr_warning("Relocation error");
}

/**************************************************************************
 51 sub_3856    ok++ (PMO) except for one jp z, that is optimised to jr z,
 **************************************************************************/
    term_t tO;
    int valO;

operand_t *evalOperand() {
register     operand_t *operO;

#ifdef DEBUG
    pr_warning("evalOperand");
#endif

    operO = allocOperand();

    switch (tokType) {
        case T_EOL:
            /* operO->type = T_INVALID; */
	    PutByte((char*)operO, OFF_type, (uint8_t)T_INVALID);
            break;

        case T_REG:
            if (expectCond && yylval.i == REG_C) { /* check if C is used in jp c context */
                tokType  = T_COND;                 /* convert to condition */
                yylval.i = COND_LLT;
            }
        /* fall through */
        case T_COND:
            /* operO->type = tokType; */ /* save type, value and prep for next token */
	    PutByte((char*)operO, OFF_type, (uint8_t)tokType);

            /* operO->aux  = (uint8_t)yylval.i; */
	    PutByte((char*)operO, OFF_aux, (uint8_t)yylval.i);

            tokType    = get_token();
            break;

        case T_OPAR:
            if ((tokType = get_token()) == T_REG) {
                if (yylval.i != REG_C && yylval.i < REG_BC) { /* only (C) and (BC) onwards are valid */
                    oper_err();
                }
                /* operO->type = T_REGREF; */
		PutByte((char*)operO, OFF_type, (uint8_t)T_REGREF);

                /* operO->aux  = (uint8_t)yylval.i;  */        /* save reg id */
		PutByte((char*)operO, OFF_aux, (uint8_t)yylval.i);

                if ((tokType = get_token()) == T_CPAR) { /* simple (reg) so prep for next token */
                    tokType = get_token();
                    break;
                }
                if (/* operO->aux */ GetByte((char*)operO, OFF_aux) < (uint8_t)REG_IX) { /* only IX & IY are allowed expressions */
                    oper_err();
                }
                /* operO->type = T_INDEXED; */ /* is IX/IY +/- d operand */
		PutByte((char*)operO, OFF_type, (uint8_t)T_INDEXED);
            } else {
                /* operO->type = T_ADDRREF; */    /* is a (addr) operand */
		PutByte((char*)operO, OFF_type, (uint8_t)T_ADDRREF);
            }

            /* operO->term = *evalExpr(); */ /* get the expression */
	    tO = *evalExpr();
	    WriteMem((char*)&tO, (char*)operO, OFF_term, sizeof(term_t));

            if (tokType != T_CPAR) { /* should now be the closing ) */
                oper_err();
            }
            tokType = get_token(); /* prep for next token */
            /* IX & IY are only allowed displacements -128 to 127 */
            if (/* operO->type */ GetByte((char*)operO, OFF_type) == (uint8_t)T_INDEXED &&
		 (/* operO->oVal */ (valO = GetWord((char*)operO, OFF_oVal)) < -128 ||
		 /* operO->oVal */ valO >= 128)) {
                pr_warning("Index offset too large");
            }
            break;
        /*
            the operands below are only valid as the last operand on a line
            so there is no preping for the next token
        */
        case T_FWD:
        case T_BWD:
            /* operO->type = tokType; */
	    PutByte((char*)operO, OFF_type, (uint8_t)tokType);

            /* operO->oVal = yylval.i; */ /* save the label reference number */
	    PutWord((char*)operO, OFF_oVal, yylval.i);

            break;
        default:
            /* operO->type = T_CONST; */
	    PutByte((char*)operO, OFF_type, (uint8_t)T_CONST);

            /* operO->term = *evalExpr(); */ /* allow for more complex expression */
	    tO = *evalExpr();
	    WriteMem((char*)&tO, (char*)operO, OFF_term, sizeof(term_t));
    }
    return operO;
}

/**************************************************************************
 52 oper_err    ok++ (PMO) but note jmpbuf not set
 **************************************************************************/
void oper_err() {

    pr_warning("Operand error");
    /* longjmp(jmpbuf, 1); */ /* BUG: jmpbuf not set */
}

/**************************************************************************
 53 sub_39a3    ok++ (PMO)
 **************************************************************************/
void getOperands(register inst_t *pi) {

#ifdef DEBUG
    pr_warning("getOperands");
#endif

    tokType    = get_token();
    cntOperand = 0;
    pi->iLhs   = NULL;
    pi->iRhs   = NULL;
    if (tokType == T_COMM) {
        oper_err();			/* cannot start with a comma */
    }
    if (tokType != T_EOL) {		/* no operands */
        pi->iLhs = evalOperand();
        if (tokType == T_COMM) {	/* if comma then 2nd operand */
            tokType  = get_token();
            pi->iRhs = evalOperand();
            ++cntOperand;
        }
        ++cntOperand;
    }
    clr_len_inbuf();
}

