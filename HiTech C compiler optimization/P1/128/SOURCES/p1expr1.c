#include "p1.h"

s2_t *p2List      = &s2_9cf3[20]; /* 8bc7 */
int16_t strId     = 0;            /* 8bd7 */
uint8_t byte_8f85 = 0;            /* 8f85 */
bool byte_8f86 = false;        /* 8f86 */
uint8_t byte_968b;                /* 968b */
int16_t word_968c;                /* 968c */
int16_t tmpLabelId;               /* 968e */

expr_t **s13SP;   /* 9cf1 */
s2_t s2_9cf3[20]; /* 9cf3 */
char pad9d00[27];
expr_t s13_9d1b; /* 9d1b */
expr_t s13_9d28; /* 9d28 */

expr_t *s13FreeList; /* 9d35 */
uint8_t byte_9d37;   /* 9d37 */
expr_t *s13Stk[20];  /* 9d38 */

bool sub_5a76(register s8_t *st, bool vst, uint8_t p2);
uint8_t yylex(void);
void prError(int, ...);
void prWarning(int, ...);

/**************************************************
 * 17: 07F5 PMO +++
 **************************************************/
expr_t *sub_07f5(char p1) {
    char l1;
    register expr_t *st;

    l1        = byte_9d37;
    byte_9d37 = p1;
    st        = sub_0bfc();
    byte_9d37 = l1;
    return st;
}

/**************************************************
 * 18: 0817 PMO +++
 * Minor differences due to addition of missing
 * parameters and use of uint8_t parameter
 * also one missed optimisation
 * of pop bc, push hl to ex (sp),hl
 **************************************************/
expr_t *sub_0817(register s8_t *st) {
    int16_t var2;
    int16_t var4;
    expr_t *var6;
    uint8_t tok;
    char inextinfoc7;
    bool v = false;
    bool vnext;
    args_t* stiargs;
    expr_t *arr[128];

    /* fix as if st is NULL the else clause shouldn't be used */
    if (st) {

	if (vnext = IsValid((char*)st->i_nextInfo))
	    inextinfoc7 = GetByte((char*)(st->i_nextInfo)+OFF_ATTR_c7);
	else
	    inextinfoc7 = st->i_nextInfo->c7;

        if (st->c7 != (char)ANODE && st->i4 == 1 && st->dataType == (char)DT_POINTER &&
            /* st->i_nextInfo->c7 */ inextinfoc7 == (char)ANODE)
	{
            st = st->i_nextInfo;
	    v = vnext;
	}
        else if (st->c7 != (char)ANODE)
            /* prError("function or function pointer required"); */
	    prError(91);
    }

    if (
	st && 
	( ( v ? GetByte((char*)st+OFF_ATTR_c7) : st->c7 ) == (char)ANODE ) &&
 	( v ? ( stiargs = (args_t*)GetWord((char*)st+OFF_ATTR_pArgs) ) : ( stiargs = st->i_args ) )
       ) 
    {
        var4 = stiargs->cnt;
        st   = stiargs->s8array;
    } else {
        st   = NULL;
        var4 = 0;
    }

    var2     = 0;
    ungetTok = tok = yylex();
    for (;;) {
        if (st && st->dataType == (char)DT_VARGS) {
            var4 = 0;
            st   = 0;
        }
        if (tok != (char)T_RPAREN) {
            if (st && sub_5a76(st, false, DT_VOID)) {
                /* prError("function does not take arguments"); */
		prError(78);
                var4 = 0;
                st   = NULL;
            }
            var6 = sub_1441(T_60, sub_07f5(3), 0);
            if (var6) {
                if (st && var4-- == 0) {
                    /* prError("too many arguments"); */
		    prError(77);
                    st   = 0;
                    var4 = 0;
                }
                if (st)
                    var6 = sub_1f5d(var6, st++, 1);
                else
                    var6 = sub_1d02(var6);
                arr[var2++] = var6;
                tok         = yylex();
                if (tok == (char)T_COMMA)
                    continue;
                ungetTok = tok;
            }
        }
        break;
    }
    if ((var4 != 1 || var2 != 0 || !sub_5a76(st, false, DT_VOID)) && var4 && st->dataType != (char)DT_VARGS)
        /* prError("too few arguments"); */
	prError(76);

    if (var2 == 0)
        return sub_23b4(T_120, NULL, NULL); /* dummy 2nd & 3rd args added */
    var4 = 0;
    while (var4 + 1 != var2) {
        arr[var4 + 1] = sub_23b4(T_COMMA, arr[var4], arr[var4 + 1]);
        ++var4;
    }
    return arr[var4];
}

