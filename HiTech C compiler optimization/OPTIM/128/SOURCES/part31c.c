/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include <stdio.h>
/* #include <stdlib.h> */
#include <string.h>
#include <setjmp.h>
#include "optim1.h"

extern int regTestMasks[];

/**************************************************************************
 55 sub_4000    ok++ (PMO)
 **************************************************************************/
bool sub_4000(register inst_t const *pi) {
   sym_t *p;

#ifdef DEBUG
    pr_warning("sub_4000");
#endif

    return pi->type == T_JP &&
           /* pi->iLhs->oPSym */ (p = (sym_t*)GetWord((char*)(pi->iLhs)+OFF_oPSym) ) &&
           strcmp(/* pi->iLhs->oPSym->label */ p->label, "cret") == 0;
}

/*************************************************************************/

#ifdef PRINT_REG_VAL
void PrintOp(operand_t* o)
{
  if (IsValid((char*)o))
    fprintf(stderr, "%04x type %02x aux %02x term.val %04x term.p %04x \n",
                o,
                GetByte((char*)o+OFF_type),
                GetByte((char*)o+OFF_aux),
                GetWord((char*)o+OFF_oVal),
                GetWord((char*)o+OFF_oPSym));
  else
    fprintf(stderr, "%04x type %02x aux %02x term.val %04x term.p %04x \n", o, o->type, o->aux, o->oVal, o->oPSym);
}

void PrintRegVal(short n)
{
  fprintf(stderr, "regValues[%02x] ",n);
  PrintOp(&regValues[n]);
}
#endif

#ifdef PRINT_LISTS

#ifndef PRINT_REG_VAL
void PrintOp(operand_t* o)
{
  if (IsValid((char*)o))
    fprintf(stderr, "%04x type %02x aux %02x term.val %04x term.p %04x \n",
                o,
                GetByte((char*)o+OFF_type),
                GetByte((char*)o+OFF_aux),
                GetWord((char*)o+OFF_oVal),
                GetWord((char*)o+OFF_oPSym));
  else
    fprintf(stderr, "%04x type %02x aux %02x term.val %04x term.p %04x \n", o, o->type, o->aux, o->oVal, o->oPSym);
}
#endif

void PrintSym(int id, sym_t* s)
{
  fprintf(stderr, "Symbol symId %04x ", id);

  if (s->label)
    fprintf(stderr, "label %s ", s->label);
  else
    fprintf(stderr, "label NULL ");

  fprintf(stderr, "p %04x\n", (char*)s->p);
}

void PrintPi(void)
{
  inst_t* pi;

  for (pi = root->pNext; pi; pi = pi->pNext)
  {
    fprintf(stderr, "PI %04x ", pi);
    fprintf(stderr, "type %02x ", pi->type);

    if (pi->opCode)
      fprintf(stderr, "opCode %s ", pi->opCode);
    else
      fprintf(stderr, "opCode NULL ");

    fprintf(stderr, "aux %02x \n", pi->aux);

    if (pi->type == T_SYMBOL)
      PrintSym(pi->iSymId, pi->iPSym);
    else
    {
      fprintf(stderr, "Operand Lhs ");

      if (pi->iLhs)
        PrintOp(pi->iLhs);
      else
        fprintf(stderr, "NULL\n");

      fprintf(stderr, "Operand Rhs ");

      if (pi->iRhs)
        PrintOp(pi->iRhs);
      else
        fprintf(stderr, "NULL\n");
    }
  }

  getch();
  /* SwitchTrace(); */
}
#endif

/***************************************************************************/

/**************************************************************************
 56 sub_404d    ok++ (PMO)  Used in: optimise
 **************************************************************************/

    int v;
    sym_t* ps;
    operand_t *pop;
    bool valid;

void sub_404d() {
register     inst_t *pi;

#ifdef DEBUG
    pr_warning("sub_404d");
#endif

    if (root->pNext)
    {
        pr_psect(TEXT);

#ifdef PRINT_LISTS
 PrintPi();
#endif

        for (pi = root->pNext; pi; pi = pi->pNext)
        {

/* !!! */
            pop = pi->iLhs;

            if (pop)
                valid = IsValid((char*)pop);
            else
                valid = false;

            if (valid)
                ps = (sym_t*)GetWord((char*)pop+OFF_oPSym);
/* !!! */

            if (valid &&
                pi->type == T_CALL &&
                strcmp(/* pi->iLhs->oPSym */ ps->label, "ncsv") == 0)
            {
                pi = pi->pNext;
                pop = pi->iLhs;

                if (pop)
                    valid = IsValid((char*)pop);
                else
                    valid = false;

                if (pi->type != T_DEFW)
                    pr_error("Expecting defw after call ncsv"); /* "defw" */

                if (valid)
                {
                    v = GetWord((char*)pop+OFF_oVal);

                    if (/* pi->iLhs->oVal */ v == 0)
                    {
                        if (usesIXorIY)
                            printf("global csv\ncall csv\n");
                    }
                    else
                    {
                        usesIXorIY = true;

                        if (/* pi->iLhs->oVal */ v >= -4)
                        {
                            printf("global csv\ncall csv\npush hl\n");

                            if (/* pi->iLhs->oVal */ v < -2)
                                printf("push hl\n");
                        }
                        else
                            printf("call ncsv\ndefw %d\n", /* pi->iLhs->oVal */ v);
                    }
                }
            }
            else if (!usesIXorIY && sub_4000(pi))
            {
                pi->type   = T_RET;
                pi->opCode = /* NULL */ 0;
                pr_instruction(pi);
            }
            else if (valid && !usesIXorIY && pi->type == T_CALL && pi->aux == 0 && pi->pNext->aux == 0 &&
                       sub_4000(pi->pNext) && /* pi->iLhs->oPSym */ ps->label[0] == '_')
            {
                pi->type   = T_JP; /* "jp" */
                pi->opCode = /* NULL */ 0;
                pr_instruction(pi);
                pi = pi->pNext;
            }
            else
                pr_instruction(pi);
        }
    }

    if (switchVectors->pNext)
    {
        pr_psect(DATA);

        for (pi = switchVectors->pNext; pi; pi = pi->pNext)
            pr_instruction(pi);
    }
}

/**************************************************************************
 57 pr_instruction      sub_420a    ok++ (PMO)
 *
 * benign differences
 *   1) printf call/return code is shared
 *   2) fputc('\n', stdout) is located differently
 **************************************************************************/
    operand_t *po_i;
    bool valid_i;
    sym_t *ps_i;

void pr_instruction(register inst_t *pi) {

#ifdef DEBUG
    pr_warning("pr_instruction pi=%04x", pi);
#endif

    po_i = pi->iLhs;

    valid_i = IsValid((char*)po_i);

    if (valid_i)
        ps_i = (sym_t*)GetWord((char*)po_i+OFF_oPSym);

    if (pi->type == T_INVALID)
        ;
    else if (pi->type == T_SYMBOL)
    {
        if ((pi->iPSym->label[0]))
            printf("%s:\n", pi->iPSym->label);
        else
            printf("L%d:\n", pi->iSymId);
    }
    else if (pi->type == T_CONST)
        printf("%d:\n", pi->aux);     /* OPTIMISER[1]: shares printf call with above*/ /* m4: */
    else if (  valid_i &&
               key_f &&
               pi->type == T_CALL &&
               strcmp(/* pi->iLhs->oPSym */ ps_i->label, "csv") == 0)
        printf("push\tiy\npush\tix\nld\tix,0\nadd\tix,sp\n");
    else
    {
        if (key_n)
            fputc('\t', stdout);                /* m7: */

        pr_token(pi);

        if (pi->type == T_JP || pi->type == T_CALL || pi->type == T_RET)
        {
            fputc('\t', stdout);

            if (pi->aux != 0)
                printf("%s", conditions[pi->aux]);

            if (pi->type != T_RET)
            {
                if (pi->aux != 0)
                    fputc(',', stdout);

                if (valid_i)
                    sub_436e(po_i);             /* m11: */
            }
        }
        else if (valid_i)
        {                       /* m14: */
            fputc('\t', stdout);

            sub_436e(po_i);

            if (pi->iRhs && IsValid((char*)pi->iRhs))
            {
                fputc(',', stdout);
                sub_436e(pi->iRhs);
            }
        }

        fputc('\n', stdout); /* OPTIMISER: minor movement in where this is located also optimises return */
    }
}

