/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

// sym_t *word_a291;   /* as91 */
// sym_t *s25FreeList; /* a293 */
// sym_t **hashtab;    /* a295 */

//extern header hashtab[];
//extern header SymList;

s12_t *p12_a297;    /* a297 */
uint8_t byte_a299;  /* a299 */
uint8_t byte_a29a;  /* a29a */

char*	Get_hashtab(int index);
char*	Get_SymList(void);

void InitHash(void);
void InitHashEntry(int index, char* sym);
void AddToList(register char* hdr, char* new);
void RemoveFromList(register char* sym);
char* GetNextSym(char* hdr, register char* crt);
char* GetFirstSym(register char* hdr);

void *xalloc(size_t size);
void sub_2569(register expr_t *st);
void sub_5be1(register s8_t *st, bool vst);
void sub_0493(register sym_t *st);
expr_t *sub_21c7(register expr_t *st);
void prError(int, ...);
void prWarning(int, ...);
void fatalErr(int , ...);

/**************************************************
 * 103: 4D92 PMO +++
 **************************************************/
void sub_4d92(void) {

    /* s25FreeList = 0 */ ;
    // hashtab = xalloc(HASHTABSIZE * sizeof(hashtab[0]));
    InitHash();
}

/**************************************************
 * 104: 4DA7 PMO +++
 **************************************************/

int ComputeHashEntry(char *buf)
{
    uint16_t crc;
    char* s;

    for (crc = 0, s = buf; *s; s++)
       crc += crc + *(uint8_t *)s;

   return crc % HASHTABSIZE;
}

/* sym_t **lookup(char *buf) { */
sym_t *lookup(char *buf) {
/*    sym_t **ps; */
    uint16_t init;
    uint8_t type;
    sym_t *cp;
    char* hdr;
    char tmpbuf[32];

    init = ComputeHashEntry(buf);
    hdr = Get_hashtab(init);
/*-------------------------------
    for (crc = 0, s = buf; *s; s++)
        crc += crc + *(uint8_t *)s;

    for (ps = &hashtab[crc % HASHTABSIZE]; 
	 (cp = *ps); 
	 ps = &cp->m8 )
----------------------------------- */
    for (cp = (sym_t*)GetFirstSym(hdr);
	 cp;
	 cp = (sym_t*)GetNextSym(hdr, (char*)cp)) 
    {
	GetString(tmpbuf, (char*)GetWord((char*)cp+OFF_nVName));

        if (buf && strcmp(/* cp->nVName */ tmpbuf, buf) == 0) 
            if (((byte_8f85 == ((type = /* cp->m20 */ GetByte((char*)cp+OFF_m20)) == (char)D_STRUCT || type == (char)D_UNION)) &&
                 byte_8f86 == (type == (char)D_MEMBER)) ||
                type == 0)
                break;
    }
    /* return ps; */

    return cp;
}

/**************************************************
 * 105: 4E90 PMO +++
 **************************************************/
sym_t *sub_4e90(register char *buf) {
/*----------------------------------
    sym_t **ps = lookup(buf);
    if (*ps == 0)
        *ps = nodeAlloc(buf);
    if (crfFp && buf)
        fprintf(crfFp, "%s %d\n", buf, lineNo);
    return *ps;
-------------------------------------- */
    sym_t *ps = lookup(buf);

    if (!ps)
    {
        ps = nodeAlloc(buf);
	AddToList(Get_hashtab(ComputeHashEntry(buf)), (char*)ps);
    }

    if (crfFp && buf)
        fprintf(crfFp, "%s %d\n", buf, lineNo);

    return ps;
}

/**************************************************
 * 106: 4EED PMO +++
 **************************************************/
// p3 may be in Upper RAM, but may be also NULL
sym_t *sub_4eed(sym_t *st, uint8_t p2, s8_t *p3, bool v, sym_t *p4) {
    /* sym_t **ppSym; */
    sym_t *tmp1, *tmp2;
    char *var4;
    int16_t var6;
    int index;
    char m20;
    args_t* p3iargs, *stiargs;
    expr_t* p3iexpr, *stiexpr;
    char c7;
    s8_t buf8;
    char tmpbuf[32];

    if (m20 = /* st->m20 */ GetByte((char*)st+OFF_m20)) 
    {
        if (depth == /* st->m21 */ GetByte((char*)st+OFF_m21) &&
            (p2 != D_MEMBER || (/* st->m20 */ m20 == D_MEMBER &&
			        /* st->nMemberList */ (sym_t*)GetWord((char*)st+OFF_nMemberList) == p4))) 
	{
            var4 = 0;

	    if (p3)
	    {
	    	if (v)
	    	{
		    p3iargs = (args_t*)GetWord((char*)p3+OFF_ATTR_pArgs);
		    p3iexpr = (expr_t*)GetWord((char*)p3+OFF_ATTR_pExpr);
		    c7 = GetByte((char*)p3+OFF_ATTR_c7);
	        }
	    	else
	    	{
		    p3iargs = p3->i_args;
		    p3iexpr = p3->i_expr;
		    c7 = p3->c7;
	    	}
	    }
	    else
		c7 = 0;	// keep p3 access impossible

	    stiargs = (args_t*)GetWord((char*)st+OFF_s8_t_pArgs);
	    stiexpr = (expr_t*)GetWord((char*)st+OFF_s8_t_pExpr);

            if (p2 != /* st->m20 */ m20)
                var4 = "storage class";
            else if (/* st->m18 */ GetWord((char*)st+OFF_m18) & 0x10) 
	    {
                if (p3 && !sub_591d(p3, v, &st->attr, true))
                    var4 = "type";

                if (/* p3->c7 */ c7 == (char)ANODE) 
		{
                    if (/* p3->i_args */ p3iargs && ! /* st->attr.i_args */ stiargs)
                        var4 = "arguments";
                    else if (/* p3->i_args */ p3iargs) 
		    {
                        if (/* p3->i_args->cnt */ p3iargs->cnt != /* st->attr.i_args->cnt */ stiargs->cnt)
                            var4 = "no. of arguments";
                        else 
			{
                            var6 = /* p3->i_args->cnt */ p3iargs->cnt;
                            while (var6--) 
			    {
                                if (!sub_591d(&p3iargs->s8array[var6], false, &stiargs->s8array[var6], false)) 
				{
                                    var4 = "arguments";
                                    break;
                                }
                            }
                        }
                    }
                }
            } /* 4fea */

            if (var4)
	    {
		GetString(tmpbuf, (char*)GetWord((char*)st+OFF_nVName));
                prError(300, "%s: %s redeclared", /* st->nVName */ tmpbuf, var4);
	    }
            else if (/* p3->c7 */ c7 == ENODE &&
		     /* p3->i_expr */ p3iexpr &&
		     /* p3->i_expr */ p3iexpr != /* st->attr.i_expr */ stiexpr) 
	    {
                sub_2569(/* st->attr.i_expr */ stiexpr);
                /* st->attr.i_expr = p3->i_expr; */
		PutWord((char*)st+OFF_s8_t_pExpr, (short)p3iexpr);
            } 
	    else if (/* p3->c7 */ c7 == (char)ANODE) 
	    {
                if (! /* st->attr.i_args */ stiargs)
		{
                    /* st->attr.i_args = p3->i_args; */
		    PutWord((char*)st+OFF_s8_t_pArgs, (short)p3iargs); 
		}
		else if (/* p3->i_args */ p3iargs &&
			 /* p3->i_args */ p3iargs != /* st->attr.i_args */ stiargs)		
		{
                    sub_583a(/* st->attr.i_args */ stiargs);
                    /* st->attr.i_args = p3->i_args; */
		    PutWord((char*)st+OFF_s8_t_pArgs, (short)p3iargs); 
                }
            } /* 50d1 */

            return st;
        } /* 50d7 */
/*-----------------------------------------------
        ppSym        = lookup(st->nVName);
        *ppSym       = nodeAlloc(st->nVName);
        (*ppSym)->m8 = st;
        st           = *ppSym;
------------------------------------------------*/
	GetString(tmpbuf, (char*)GetWord((char*)st+OFF_nVName));

	index = ComputeHashEntry(tmpbuf);

	tmp1 = lookup(tmpbuf); 

	tmp2 = nodeAlloc(tmpbuf);

	if (!tmp1)
		InitHashEntry(index, (char*)tmp2);
	else
       		AddToList(Get_hashtab(index), (char*)tmp2);

	st = tmp2;

    } /* 5116 */
    
    PutByte((char*)st+OFF_m20, p2);

    switch (/* st->m20 = p2 */ p2) 
    {
    case DT_USHORT:
    case DT_INT:
    case DT_UINT:
        /* st->attr.i_labelId = newTmpLabel(); */
	PutWord((char*)st+OFF_s8_t_labelId, (short)newTmpLabel());
        return st;
    case D_CONST:
        /* st->m18 |= 0x10; */
	PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) | 0x10);
        /* FALLTHRU */
    case DT_LONG:
        /* st->nMemberList = p4; */
	PutWord((char*)st+OFF_nMemberList, (short)p4);
        break;
    case DT_ULONG:
        return st;
    case T_TYPEDEF:
        break;
    default:
        /* st->m18 |= 0x10; */
	PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) | 0x10);
        break;
    }

    /* st->attr = *p3; */
    if (p3)
    {
    	if (v)
    	{
	    ReadMem((char*)&buf8, (char*)p3, sizeof(struct _s8));
	    WriteMem((char*)&buf8, (char*)st, sizeof(struct _s8));
        }
        else
    	    WriteMem((char*)p3, (char*)st, sizeof(struct _s8));
    }

    return st;
}

/**************************************************
 * 107: 516C PMO +++
 **************************************************/
void sub_516c(register sym_t *st) {
    int tmp;
    char* p;
    char buf[32];

    if (st && !(/* st->m18 */(tmp = GetWord((char*)st+OFF_m18)) & 0x80)) 
    {
	p = (char*)GetWord((char*)st+OFF_nVName);
	
	if (p)
	    GetString(buf, p);

        if (/* st->m18 */ tmp & 1)
            prError(100+37, /* st->nVName */ buf);

        /* st->m18 |= 1; */
	PutWord((char*)st+OFF_m18, tmp = tmp | 1);

        if (crfFp &&
	    /* st->nVName */ p &&
	    !(/* st->m18 */ tmp & 0x80)) 
            fprintf(crfFp, "#%s %d\n", /* st->nVName */ buf, lineNo);
    }
}

/**************************************************
 * 108: 51CF PMO +++
 **************************************************/
void sub_51cf(register sym_t *st) {

    if (st)
    {
        /* st->m18 |= 2; */
	PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) | 2);
    }
}

/**************************************************
 * 109: 51E7 PMO +++
 * uin8_t param
 **************************************************/
void sub_51e7(void) {
    int16_t var2;
    int16_t var4;
    args_t *var6;
    register sym_t *st;

    var6 = /* curFuncNode->a_args; */ (args_t*)GetWord((char*)curFuncNode+OFF_s8_t_pArgs); 
    
    st   = p25_a28f;
    while (st) {
        /* st->m18 &= ~0x28; */
	PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) & ~0x28);
        if (var6)
	{
            /* st->m18 |= 0x200; */
	    PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) | 0x200);
	}
        if (/* st->a_c7 */ GetByte((char*)st+OFF_s8_t_c7) == (char)1) {
            sub_2569(/* st->a_expr */ (expr_t*)GetWord((char*)st+OFF_s8_t_pExpr));
            /* st->a_expr = 0; */
	    PutWord((char*)st+OFF_s8_t_pExpr, 0);
            /* st->a_c7   = 0; */
	    PutByte((char*)st+OFF_s8_t_c7, 0);

            /* sub_5be1(&st->attr); */
	    sub_5be1(&st->attr, true);
        } /* 523d */
        sub_516c(st);
        sub_0493(st);
        /* st = st->nMemberList; */
	st = (sym_t*)GetWord((char*)st+OFF_nMemberList);
    }
    if (!var6)
        return;
    var4 = 0;
    var2 = var6->cnt;
    if (var2 == 1 && sub_5a76(var6->s8array, false, DT_VOID) && !p25_a28f)
        return;
    st = p25_a28f;
    while (st && var2--) {
        if (var6->s8array[var4].dataType == (char)DT_VARGS) {
            st   = NULL;
            var2 = 0;
            break;
        } else if (!sub_591d(&var6->s8array[var4], false, &st->attr, true))
            break;
        /* st = st->nMemberList; */
	st = (sym_t*)GetWord((char*)st+OFF_nMemberList);
        var4++;
    }
    if (st || (var2 && var6->s8array[var4].dataType != (char)DT_VARGS))
        prError(36);
    var2 = 1;
}

/**************************************************
 * 110: 5356 PMO +++
 **************************************************/
bool releaseNodeFreeList(void) {
//    register sym_t *st;
//
//    if (!s25FreeList)
//        return false;
// 
//    while ((st = s25FreeList)) {
//        /* s25FreeList = st->nMemberList; */
//	s25FreeList = GetWord((char*)st+OFF_nMemberList);
//        myfree(st);
//    }
//    return true;

    return false;
}

/**************************************************
 * 111: 5384 PMO +++
 **************************************************/
char blank[] = "";
int16_t nodeCnt = 0;

sym_t *nodeAlloc(char *s) {
    register sym_t *pn;
    char* tmp;
 
//    if (s25FreeList) {
//        pn          = s25FreeList;
//        /* s25FreeList = pn->nMemberList; */
//	s25FreeList = GetWord((char*)pn+OFF_nMemberList);
//        /* blkclr(pn, sizeof(sym_t)); */
//	ZeroFill(pn);
//    } else {
//        /* pn = xalloc(sizeof(sym_t)); */
//	pn = (sym_t *)myalloc();
//    }

    pn = (sym_t *)myallocsym();

    if (!s) 
	s = blank;

    tmp = myallocstr(strlen(s) + 1);

    if (!pn || !tmp)
	fatalErr(100, "Out of memory");

    /* pn->m21     = depth; */
    PutByte((char*)pn+OFF_m21, depth);

    /* pn->nRefCnt = 1; */
    PutByte((char*)pn+OFF_nRefCnt, 1);

    /* pn->nodeId  = ++nodeCnt; */
    PutWord((char*)pn+OFF_nodeId, ++nodeCnt); 

    PutString(s, tmp);

    PutWord((char*)pn+OFF_nVName, (short)tmp);

    return pn;
}

/**************************************************
 * 112: 540C PMO +++
 * optimiser has better code for --pn->nRefCnt
 **************************************************/
void reduceNodeRef(sym_t *pn_par) {
    sym_t buf;
    register sym_t *pn = &buf;
    char tmp;

/*    if (--pn->nRefCnt == 0) { */

    PutByte((char*)pn_par+OFF_nRefCnt, tmp = (GetByte((char*)pn_par+OFF_nRefCnt) - 1));
    ReadMem((char*)&buf, (char*)pn_par, sizeof(struct _sym));

    if (tmp == 0) 
    {    
        if (pn->m20 != 0 &&
	    pn->m20 != D_LABEL &&
	    pn->m20 != D_STRUCT &&
	    pn->m20 != D_UNION &&
            pn->m20 != D_ENUM) 
	{
            if (pn->a_c7 == (char)ANODE)
                sub_583a(pn->a_args);
            else if (pn->a_c7 == (char)ENODE)
                sub_2569(pn->a_expr);
            if (pn->a_dataType == (char)DT_POINTER)
                reduceNodeRef(pn->a_nextSym);
        }

//        if (pn->nVName != blank)
//            free(pn->nVName);
 
        /* pn->nMemberList = s25FreeList; */
        /* s25FreeList     = pn_par; */
	
	RemoveFromList((char*)pn_par);
	// myfree(pn_par);
    }
}

/**************************************************
 * 113: 549C PMO +++
 * use of uint8_t param
 **************************************************/
void enterScope(void) {

    prFuncBrace(T_LBRACE);
    ++depth;
}

/**************************************************
 * 114: 54AC PMO +++
 * use of uint8_t param
 **************************************************/
void exitScope(void) {

    checkScopeExit();
    --depth;
    prFuncBrace(T_RBRACE);
}

/**************************************************
 * 115: 54C0 PMO +++
 **************************************************/
void checkScopeExit(void) {
    /* sym_t **ppSym; */
    // header *ppSym; 
    /* sym_t **var4; */
    sym_t *var4;
    uint8_t var5;
    char *var7;
    sym_t buf;
    register sym_t *st_tmp = &buf;
    sym_t *st;
    int n;
    char* hdr;
    char tmpbuf[32];

    for (n = 0; n < HASHTABSIZE; n++)
    {
        var4 = (sym_t*)GetFirstSym((char*)(hdr = Get_hashtab(n)));

        while (st = var4)
	{
	    ReadMem((char*)&buf, (char*)st, sizeof(struct _sym));

            if (st_tmp->m21 == (char)depth) 
	    {
                var7 = 0;
                var5 = st_tmp->m20;

                if ((st_tmp->m18 & 3) == 2) 
		{
                    switch (var5) 
		    {
                    case D_LABEL:
                        var7 = "label";
                        break;
                    case D_STRUCT:
                    case D_UNION:
                    case T_EXTERN:
                        break;
                    default:
                        var7 = "variable";
                        break;
                    }

                    if (var7)
		    {
			GetString(tmpbuf, st_tmp->nVName);
                        prError(300, "undefined %s: %s", var7, tmpbuf);
		    }
                } else if ((depth || var5 == (char)T_STATIC) &&
			   !(st_tmp->m18 & 2)) { /* 5555  */
                    switch (var5) 
 		    {
                    case D_LABEL:
                        var7 = "label";
                        break;
                    case D_STRUCT:
                        var7 = "structure";
                        break;
                    case D_UNION:
                        var7 = "union";
                        break;
                    case D_MEMBER:
                        var7 = "member";
                        break;
                    case D_ENUM:
                        var7 = "enum";
                        break;
                    case D_CONST:
                        var7 = "constant";
                        break;
                    case T_TYPEDEF:
                        var7 = "typedef";
                        break;
                    case D_6:
                        var7 = 0;
                        break;
                    default:
                        if (var5) 
			{
                            if (st_tmp->m18 & 1)
                                var7 = "variable definition";
                            else
                                var7 = "variable declaration";
                        }
                        break;
                    }

                    if (var7)
		    {
			GetString(tmpbuf, st_tmp->nVName);
                        prWarning(300, "unused %s: %s", var7, tmpbuf);
		    }

                } /* 55d2 */

                /* *var4 */ var4 = (sym_t*)GetNextSym(hdr, (char*)var4);
                reduceNodeRef(st);
            } else
                var4 = (sym_t*)GetNextSym(hdr, (char*)var4); 
        }
    }

    /* var4 = &word_a291; */
    var4 = (sym_t*)GetFirstSym(hdr = Get_SymList());

    while ((st = /* *var4 */ var4))
    {
	ReadMem((char*)&buf, (char*)st, sizeof(struct _sym));
	st_tmp = &buf;

        if (st_tmp->m21 == (char)depth) 
	{
            /* *var4 = st->m8; */
	    var4 = (sym_t*)GetNextSym(hdr, (char*)var4);
            reduceNodeRef(st);
        } else
            /* var4 = &st->m8; */
	    var4 = (sym_t*)GetNextSym(hdr, (char*)var4); 
    }
}

/**************************************************
 * 116: 56A4 PMO +++
 **************************************************/
sym_t *sub_56a4(void) {
    register sym_t *st;

    st = nodeAlloc(0);

    AddToList(Get_SymList(), (char*)st);

    /* st->m18 |= 0x83; */
    PutWord((char*)st+OFF_m18, GetWord((char*)st+OFF_m18) | 0x83);

    /* st->m8    = word_a291; */

    return st;
}

/**************************************************
 * 117: 56CD PMO +++
 **************************************************/
sym_t *findMember(sym_t *p1, char *p2) {
    register sym_t *st;
    char p1buf[32];
    char stbuf[32];

    st = /* p1->nMemberList */ (sym_t*)GetWord((char*)p1+OFF_nMemberList);

    GetString(p1buf, (char*)GetWord((char*)p1+OFF_nVName));

    for (;;) 
    {
        if (p1 == st) 
	{
	    prError(300, "%s is not a member of the struct/union %s", p2, p1buf);
            return NULL;
        }

	GetString(stbuf, (char*)GetWord((char*)st+OFF_nVName));

        if (strcmp(/* st->nVName */ stbuf, p2) == 0)
            return st;

        /* st = st->nMemberList; */
	st = (sym_t*)GetWord((char*)st+OFF_nMemberList);
    }
}

/**************************************************
 * 118: 573B PMO +++
 **************************************************/
void sub_573b(sym_t *st_par, FILE *fp) {
    sym_t buf;
    register sym_t *st;
    char tmpbuf[32];

    if (st_par) 
    {
	ReadMem((char*)&buf, (char*)st_par, sizeof(struct _sym));
	st = &buf;

        if (st->m18 & 0x80)
            fprintf(fp, "F%d", st->nodeId);
        else
	{
	    GetString(tmpbuf, st->nVName);
            fprintf(fp, "_%s", tmpbuf);
	}
    }
}

/**************************************************
 * 119: 5785 PMO +++
 **************************************************/
int16_t newTmpLabel(void) {

    return ++tmpLabelId;
}

/**************************************************
 * 120: 578D PMO +++
 * trivial optimiser differences
 **************************************************/
args_t *sub_578d(register args_t *p) {
    args_t *var2;
    s8_t *var4;
    int16_t var6;

    if (!p)
        return p;

    var2 = (args_t *)xalloc(sizeof(args_t) + (p->cnt - 1) * sizeof(s8_t));
    var6 = var2->cnt = p->cnt;

    while (var6--) 
    {
        var4  = &var2->s8array[var6];
        *var4 = p->s8array[var6];
        if (var4->dataType == DT_POINTER)
	{
            /* var4->i_nextSym->nRefCnt++; */
	    PutByte((char*)(var4->i_nextSym)+OFF_nRefCnt, GetByte((char*)(var4->i_nextSym)+OFF_nRefCnt)+1);
	}
    }
    return var2;
}

/**************************************************
 * 121: 583A PMO +++
 **************************************************/
void sub_583a(register args_t *st) {
    s8_t *var2;

    if (st) 
    {
        for (var2 = st->s8array; st->cnt--; var2++) 
   	{
            if (var2->c7 == (char)ANODE && var2->i_args)
                sub_583a(var2->i_args);

            if (var2->dataType == (char)DT_POINTER)
                reduceNodeRef(var2->i_nextSym);
        }

        free(st); 
    }
}

/**************************************************
 * 122: 58BD PMO +++
 **************************************************/
// st, p2 may be in upper RAM
void sub_58bd(s8_t *st, bool vst, register s8_t *p2, bool vp2) {
    s8_t buf;
    char c7;
    expr_t* iexpr;
    args_t* iargs;

    if (vp2)
    {
	if (vst)
	{
	    ReadMem((char*)&buf, (char*)st, sizeof(struct _s8));
	    WriteMem((char*)&buf, (char*)p2, sizeof(struct _s8));
	}
	else
	    WriteMem((char*)st, (char*)p2, sizeof(struct _s8));
    }
    else
    {
	if (vst)
	{
	    ReadMem((char*)&buf, (char*)st, sizeof(struct _s8));
	    *p2 = buf;	    
	}
	else
    	    *p2 = *st;
    }	
	
    if (vp2)
    {
	c7 = GetByte((char*)p2+OFF_ATTR_c7);
	iexpr = (expr_t*)GetWord((char*)p2+OFF_ATTR_pExpr);
	iargs = (args_t*)GetWord((char*)p2+OFF_ATTR_pArgs);
    }
    else
    {
	c7 = p2->c7;
	iexpr = p2->i_expr;
	iargs = p2->i_args;
    }

    if (/* p2->c7 */ c7 == (char)ENODE)
    {
	if (vp2)
	    PutWord((char*)p2+OFF_ATTR_pExpr, (short)sub_21c7(iexpr));
	else
            p2->i_expr = sub_21c7(iexpr);
    }
    else if (/* p2->c7 */ c7 == (char)ANODE)
    {
	if (vp2)
	    PutWord((char*)p2+OFF_ATTR_pArgs, (short)sub_578d(iargs));
	else
            p2->i_args = sub_578d(iargs);
    }
}

/**************************************************
 * 123: 591D PMO +++
 * some optimiser differences including movement
 * of some basic blocks
 **************************************************/
// st, p2 may be in Upper RAM
bool sub_591d(register s8_t *st, bool vst, s8_t *p2, bool vp2) {
    int16_t var2;
    char stc7, p2c7, stdatatype, p2datatype;    
    int sti4, p2i4;
    sym_t *stinextsym, *p2inextsym;
    s8_t *stinextinfo, *p2inextinfo;
    args_t *stiargs, *p2iargs;
    s8_t buf8;

    if (st == p2)
        return true;

    if (vst)
    {
	ReadMem((char*)&buf8, (char*)st, sizeof(struct _s8));
	stc7 = buf8.c7;
	stdatatype = buf8.dataType;
	sti4 = buf8.i4;
	stinextsym = buf8.i_nextSym;
	stinextinfo = buf8.i_nextInfo;
	stiargs = buf8.i_args;
    }
    else
    {
	stc7 = st->c7;
	stdatatype = st->dataType;
	sti4 = st->i4;
	stinextsym = st->i_nextSym;
	stinextinfo = st->i_nextInfo;
	stiargs = st->i_args;
    }

    if (vp2)
    {
	ReadMem((char*)&buf8, (char*)p2, sizeof(struct _s8));
	p2c7 = buf8.c7;
	p2datatype = buf8.dataType;
	p2i4 = buf8.i4;
	p2inextsym = buf8.i_nextSym;
	p2inextinfo = buf8.i_nextInfo;
	p2iargs = buf8.i_args;
    }
    else
    {
	p2c7 = p2->c7;
	p2datatype = p2->dataType;
	p2i4 = p2->i4;
	p2inextsym = p2->i_nextSym;
	p2inextinfo = p2->i_nextInfo;
	p2iargs = p2->i_args;
    }

    if (stc7 != p2c7 ||
	stdatatype != p2datatype ||
	sti4 != p2i4)
        return false;

    switch (stdatatype) {
    case DT_ENUM:
    case DT_STRUCT:
    case DT_UNION:
        return stinextsym == p2inextsym;
    case DT_POINTER:
        return sub_591d(stinextinfo, IsValid((char*)stinextinfo), p2inextinfo, IsValid((char*)p2inextinfo));
    }

    if (stc7 != (char)ANODE || !stiargs || !p2iargs)
        return true;

    if (stiargs->cnt != p2iargs->cnt)
        return false;

    var2 = stiargs->cnt;

    do {
        if (var2-- == 0)
            return true;
    } while (sub_591d(&stiargs->s8array[var2], false, &p2iargs->s8array[var2], false));

    return false;
}

/**************************************************
 * 124: 5A4A PMO +++
 **************************************************/
// st may be in Upper RAM ???
bool sub_5a4a(register s8_t *st /* , bool vst */) {
    char stdatatype;    
    int sti4;

//    if (vst)
//    {
//	stdatatype = GetByte((char*)st+OFF_ATTR_dataType);
//	sti4 = GetWord((char*)st+OFF_ATTR_i4);
//    }
//    else
//    {
	stdatatype = st->dataType;
	sti4 = st->i4;
//    }

    return stdatatype == (char)DT_VOID && sti4 == 1;
}

/**************************************************
 * 125: 5A76 PMO +++
 **************************************************/
// st may be in UpperRAM
bool sub_5a76(register s8_t *st, bool vst, uint8_t p2) {
    char c7;
    int i4;
    char dataType;

    if (vst)
    {
	c7 = GetByte((char*)st+OFF_ATTR_c7);
	i4 = GetWord((char*)st+OFF_ATTR_i4);
	dataType = GetByte((char*)st+OFF_ATTR_dataType);
    }
    else
    {
	c7 = st->c7;
	i4 = st->i4;
	dataType = st->dataType;
    }

    return /* st->dataType */ dataType == p2 && /* st->i4 */ i4 == 0 && /* st->c7 */ c7 == (char)SNODE;
}

/**************************************************
 * 126: 5AA4 PMO +++
 **************************************************/
bool sub_5aa4(register s8_t *st) {
    return st->c7 == (char)SNODE && (((st->i4 & 1) && st->c7 == (char)SNODE) || st->dataType < (unsigned char)DT_VOID);
}

/**************************************************
 * 127: 5AD5 PMO +++
 **************************************************/
bool sub_5ad5(register s8_t *st) {
    return st->c7 == (char)SNODE && st->i4 == 0 &&  st->dataType <= (unsigned char)DT_ENUM;
}

/**************************************************
 * 128: 5B08 PMO +++
 **************************************************/
bool sub_5b08(register s8_t *st) {
    return (st->c7 == (char)SNODE && st->i4 == 0 && st->dataType < (unsigned char)DT_FLOAT);
}

/**************************************************
 * 129: 5B38 PMO +++
 **************************************************/
bool sub_5b38(register s8_t *st) {
    return st->c7 == (char)SNODE && st->i4 == 0 &&
           (st->dataType == (char)DT_FLOAT || st->dataType == (char)DT_DOUBLE);
}

/**************************************************
 * 130: 5B69 PMO +++
 **************************************************/
bool isValidDimType(register s8_t *st) {
    return sub_5b08(st) || sub_5a76(st, false, DT_ENUM);
}

/**************************************************
 * 131: 5B99 PMO +++
 * uint8_t parameter
 **************************************************/
void sub_5b99(register s8_t *st) {

    if (st->c7 == (char)ANODE)
        st->c7 = 0;
    else
        st->i4 >>= 1;

    if (sub_5a76(st, false, 0x16))
    {
	if (IsValid((char*)st->i_nextInfo))
		ReadMem((char*)st, (char*)(st->i_nextInfo), sizeof(struct _s8));
	else
        	*st = *(st->i_nextInfo);
    }
}

/*-----------------------------------------------------------------------------------------
void CheckSym(register sym_t* s, bool check_nextSym, bool check_pSym, bool check_i_nextSym)
{
    sym_t* t;

    if (s && !IsValid(s))
	breakpoint(0x80);

    if (s && (t = GetWord((char*)s+OFF_nMemberList)) && !IsValid(t))
	breakpoint(0x40);

    if (s && check_nextSym && (t = GetWord((char*)s+OFF_s8_t_nextSym)) && !IsValid(t))
	breakpoint(0x20);

    if (s && check_pSym && (t = GetWord((char*)s+OFF_s8_t_pSym)) && !IsValid(t))
	breakpoint(0x20);

    if (s && check_i_nextSym && (t = GetWord((char*)s+OFF_s8_t_nextSym)) && !IsValid(t))
	breakpoint(0x10);

}
-------------------------------------------------------------------------------------------*/
