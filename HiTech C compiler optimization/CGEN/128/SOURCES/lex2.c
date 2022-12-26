/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File - lex2.c
 */

#include "cgen.h"
#include "alloc128.h"

/*********************************************************
 * char sub_1B2 OK++ PMO	Used in: sub_6D1, sub_4192
 *
 * Token search		(Binary search algorithm is used)
 *********************************************************/
char sub_1B2(register char *target) {
    uint8_t hi, lo, mid;
    char cmp;

    /* clang-format off */
    static char *tnames[] = {
        "",    "!",  "!=",  "#",  "$",  "$U", "%",  "&",  "&&", "&U",
        "(",   ")",  "*",   "*U", "+",  "++", "+U", ",",  "-",  "--",
        "->",  "-U", ".",   "..", "/",  ":",  ":U", ":s", ";",  ";;",
        "<",   "<<", "<=",  "=",  "=%", "=&", "=*", "=+", "=-", "=/",
        "=<<", "==", "=>>", "=^", "=|", ">",  ">=", ">>", "?",  "@",
        "[\\", "[a", "[c",  "[e", "[i", "[s", "[u", "[v", "^",  "{",
        "|",   "||", "}",   "~", "RECIP", "TYPE",
        "ID", "CONST", "FCONST", "REG", "INAREG", "BITFIELD"
    };
    /* clang-format on */

    static char lastTok = 65;

    lo                  = 0;
    hi                  = lastTok;
    do {
        mid = (hi + lo) / 2;
        if ((cmp = strcmp(target, tnames[mid])) == 0)
            return mid;
        if (cmp < 0)
            hi = mid - 1;
        else
            lo = mid + 1;
    } while (hi >= lo);
    return (-1); /* Search terminates as unsuccessful */
}

/*********************************************************
 * gethashptr OK++ PMO		Used in: sub_265, sub_1754
 *
 * Convert name to a hash table ptr (sub_21F)
 *
 * Returns pointer to pointer to structure associated with
 * pointer to token.  for example
 *
 * Input	Hash key  Destination
 * string  	dec hex
 *   "f"  	 1	- float
 *   "i"  	 4	- int
 *   "l"  	 7	- long
 *   "s"  	14 0eh	- short
 *   "v"  	17 11h	- void
 *   "x"  	19 13h	- x ?
 *   "uc" 	30 1eh	- uint8_t
 *   "ui" 	36 24h	- uint16_t
 *   "ul" 	39 27h	- uint32_t
 *   "us" 	46 2eh	- uint16_t short
 *   "ux" 	51 33h	- uint16_t ?
 *   "b"  	98 62h	- b ?
 *   "c"  	99 63h	- char
 **********************************************************/

#define HASHTABSIZE 101

member_t **gethashptr(register char *str) {
    uint16_t key;

    /* calculate hash */
    for (key = 0; *str; str++)
        key = key * 2 + *(uint8_t *)str;

    /* Hash table index is determined by    */
    /* hash function using division method  */
    return &hashtab[key % HASHTABSIZE];
}

/*********************************************************
 * sub_265 OK++ PMO Used in: sub_627,  sub_B19,  sub_E43,
 *			     sub_1680, sub_1754, sub_4192
 *
 * Return pointer to struct associated with pointer to token
 *********************************************************/
member_t *sub_265(char *str) {
    member_t **l1b;
    register member_t *sb;
    char* p;

    /* In hash table, we look for pointer to    */
    l1b = gethashptr(str); /* entry in symbol table associated with    */
    /* scan for the matching name on the hash list */
    for (sb = *l1b; sb; sb = /* sb->b_next */ (member_t*)GetWord((char*)sb+OFF_b_next))
    {
        GetString(tmpBuf, /* sb->b_name */ (char*)GetWord((char*)sb+OFF_b_name));
        if (strcmp(/* sb->b_name */ tmpBuf, str) == 0)
            return sb;
    }

    /* not found so create new record & insert at head of list */
    sb         = (member_t*)MyAllocMbr(); /* allocMem(); */ /* 22 bytes */

    /* sb->b_next = *l1b; */
    PutWord((char*)sb+OFF_b_next, (short)*l1b);

    /* sb->b_name = (char*)MyAllocStr(strlen(str) + 1); */ /* allocMem(strlen(str) + 1); */
    PutWord((char*)sb+OFF_b_name, (short)(p = (char*)MyAllocStr(strlen(str) + 1))); 

						/* effectively strdup */
    PutString(str, /* sb->b_name */ p); /* strcpy(sb->b_name, str); */

    /* sb->b_ndpth = (uint8_t)nstdpth; */
    PutByte((char*)sb+OFF_b_ndpth, (uint8_t)nstdpth);

    *l1b        = sb;

    return sb; /* position and return pointer to record.   */
}

/*********************************************************
 * sub_306 v1 OK++ PMO Used in: sub_6D1,  sub_793,  sub_B19,
 *			        sub_E43,  sub_17E0, sub_19C1,
 *			        sub_4192
 * get_token
 * Optimiser one basic blocked moved between original code
 *  and new code. Behaviour unchanged
 *********************************************************/
char *getToken() {
    int ch, expectName;
    register char *ptr;
    static char buffer[MAXBUF];

    expectName = false;
    for (;;) {
        ptr = buffer;

        while ((ch = fgetchar()) != EOF && isspace(ch))
            ;
        if (ch == EOF)
            return (char *)~0;
        *ptr++ = ch;
        do {
            *ptr++ = ch = fgetchar();
        } while (ch != EOF && !isspace(ch));
        *--ptr = 0;
        if (buffer[0] == '"') {
            lineno     = atoi(buffer + 1);
            expectName = ch != '\n';
        } else if (expectName) {
            strncpy(progname, buffer, sizeof(progname) - 1);
            expectName = false;
        } else if (buffer[0] == ';' && buffer[1] == ';') {
            do {
                fputchar(ch = fgetchar());
            } while (ch != '\n');
        } else
            return buffer;
    }
}

/*********************************************************
 * sub_406 v3 OK++ PMO			 Used in: sub_6D1
 *
 * Notes:
 *   Added 0 parameter to sub_5CF5 function call to
 *   match number and types actual and formal parameters
 *   this function.
 *   Modified code to get sb->b_next before freeing sb
 *   Other than code changes to handle the above, the code
 *   is identical
 *********************************************************/
/* remove and free up items which have got out of scope */

void leaveBlock() {
    member_t **pHashEntry;
    int l2;
    member_t *newlist;
    member_t *next;
//    member_t **l5;
//    int l6;
    member_t *sb, *tmp;
    member_t buf;
    register member_t* p = &buf;

    l2         = HASHTABSIZE;
    pHashEntry = hashtab;

    do 
    {
        newlist = 0;

        /* note with the fixed handling of free, this could be converted to a for loop */
        sb = *pHashEntry;

        while (sb && /* sb->b_ndpth */ GetByte((char*)sb+OFF_b_ndpth) == nstdpth) 
	{
	    ReadMem((char*)&buf, (char*)sb, sizeof(struct member));

            if (/* sb->b_class */ p->b_class == VAR) 
	    {
                if (/* sb->b_flag */ p->b_flag == 2)
                    word_B017 |= array_AAE8[/* sb->b_memb.i */ p->b_memb.i];
                if (/* sb->b_flag */ p->b_flag == 4 || /* sb->b_flag */ p->b_flag == 3)
                    sub_5CF5(sb, 0); /* Add parameter 0! */
                if (0 < nstdpth && /* sb->b_flag */ p->b_flag == 3) 
		{
                    next       = /* sb->b_next */ p->b_next;
                    /* sb->b_next = newlist; */
		    PutWord((char*)sb+OFF_b_next, (short)newlist);
                    newlist    = sb;
                    /* sb->b_ndpth--; */
		    PutByte((char*)sb+OFF_b_ndpth, GetByte((char*)sb+OFF_b_ndpth) - 1);
                    sb = next;
                    continue;
                }
            }

            /* free(sb->b_name); */

	    /* --------------------------------------------------
            if (sb->b_class == UNION || sb->b_class == STRUCT) {
                l6 = sb->b_memb.mlist->cnt;
                l5 = sb->b_memb.mlist->vals;
                while (l6-- != 0)
                    free(*l5++);
                free(sb->b_memb.mlist);
            }
	    ---------------------------------------------------- */

            /*  code modified to get sb->b_next before freeing sb !!!*/
            next = /* sb->b_next */ p->b_next;

            /* free(sb); */

            /* sb = sb->b_next original code relied on b_next not being at start of block !!!*/
            sb = next;
        }

        if (newlist) {                 /* if l3b list exists add to end */
            *(pHashEntry++) = newlist; /* set hash table to the l3b list */
            while ( tmp = /* newlist->b_next */ (member_t*)GetWord((char*)newlist+OFF_b_next))
                newlist = /* newlist->b_next */ tmp; /* m14: */
            /* newlist->b_next = sb; */
	    PutWord((char*)newlist+OFF_b_next, (short)sb);
        } else
            *(pHashEntry++) = sb; /* else initialise hash table entry with new item  */
    } while (--l2 != 0);

    nstdpth--;

    if (lvlidx >= 0) 
    {
        if (array_AE57[lvlidx] == nstdpth) 
	{
            sb = array_AEDB[lvlidx];
	    ReadMem((char*)&buf, (char*)sb, sizeof(struct member));

            if (/* sb->b_type->b_class */ GetByte((char*)(p->b_type)+OFF_b_class) == STRUCT && 
		(/* sb->b_refl */ p->b_refl & 4) == 0)
                sub_BEE(/* sb->b_off */ p->b_off, /* sb->b_size */ p->b_size);

            prFrameTail(/* sb->b_off */ p->b_off, array_AE13[lvlidx]);
            lvlidx--;
            word_B017 = 0x17F;
        }
    }
}

/*********************************************************
 * sub_627 OK++ PMO  Used in: sub_17E0, sub_19C1, sub_4192
 *********************************************************/
/* encode the accessor prefix */
member_t *sub_627(register char *ch, uint16_t *par) {
    uint16_t loc;

    loc = 0;
    ch++;
    do {
        loc <<= 2;
        if (*ch == '(') {
            loc |= 2;
            ch++;
        } else if (*ch == '*') {
            loc |= 1;
            ch++;
        }
    } while (!isalnum(*ch) && *ch != '_');
    /* invert them */
    *par = 0;
    while (loc != 0) {
        *par <<= 2;
        *par |= (loc & 3);
        loc >>= 2;
    }
    return sub_265(ch);
}

/*********************************************************
 * sub_6C8 OK++ PMO		Used in: sub_6D1, expect
 *********************************************************/
void badIntCode() {

    fatalErr("Bad int. code");
}

/*********************************************************
 * sub_6D1 OK++				    Used in: main
 *
 * Compiling intermediate code
 *
 * note optimiser uses hl in jmp table calculations
 * then exchanges with de. The original code uses de
 * the effect is the same and the rest of the code is identical
 *********************************************************/
void parseStmt() {
    register char *ch;
    int tok;

    word_B017 = 0x17F;
    while ((ch = getToken()) != (char *)~0) { /* get_token    */
        tok = sub_1B2(ch);
        switch (tok) { /* search_token */
        case LBRACE:   /* "{" opening block */
            nstdpth++; /* Increase the current nesting depth */
            break;
        case RBRACE: /* "}" end of block */
            leaveBlock();
            break;
        case EXPR: /* "[e" expression	*/
            prPsect(P_TEXT);
            sub_3DC9(parseExpr());
            expect(']');
            break;
        case VAR: /* "[v" variable	*/
            parseVariable();
            break;
        case STRUCT: /* "[s" struct	*/
        case UNION:  /* "[u" union	*/
            parseMembers(tok);
            break;
        case CASE: /* "[\\" */
            parseSwitch();
            break;
        case ENUM: /* "[c" enum		*/
            parseEnum();
            break;
        case INIT: /* "[i" initialization */
            parseInit();
            break;
        case UNKNOWN: /* "[a" 		*/
            parseData();
            break;
        case BXOR: /* "^" */
        case BOR:  /* "|" */
        case LOR:  /* "||" */
        default:
            badIntCode();
        }
    }
    leaveBlock();
}

/*********************************************************
 * sub_76F OK	      Used in: sub_6D1,  sub_B19, sub_E43,
 * 			       sub_17E0, sub_1B0C
 ;
 * Search for expected character (closing square bracket)
 *********************************************************/
void expect(char par) {
    char ch;

    do {
        ch = fgetchar();
    } while (isspace(ch)); /* Skip white-space characters */

    if (ch != par)
        badIntCode();
}

/*********************************************************
 * sub_793 OK++ 			 Used in: sub_6D1
 * minor variance in code as sub_43EF the first parameter
 * is explicitly declared uint8_t, rather than the default int
 *********************************************************/
void parseData() {
    char *loc1;
    char buf[1024];
    register char *ptr;

    prPsect(P_DATA);
    sub_2BD0(sub_600E(sub_43EF(COLON_U, sub_43EF(COLON_S, parseExpr(), 0), 0)));
    ptr = buf;
    for (;;) {
        if (*(loc1 = getToken()) == ']') /* parse non-terminating tokens */
            break;
        *ptr = atoi(loc1); /* Convert string to int value and place it in buffer */
        ptr++;
    }

    prDefb(buf, (int)(ptr - buf)); /* Emit data "defb byte1, ..." ("ptr-buf" bytes from "buf")*/
}
/* end of lex2.c */
