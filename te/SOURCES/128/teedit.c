/*      te_edit.c

        Text editor.

        Edit line.

        Copyright (c) 2015-2021 Miguel Garcia / FloppySoftware

        This program is free software; you can redistribute it and/or modify it
        under the terms of the GNU General Public License as published by the
        Free Software Foundation; either version 2, or (at your option) any
        later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

        Changes:

        30 Jan 2018 : Extracted from te.c.
        04 Feb 2018 : Support for macros. Go to line #.
        22 Feb 2018 : Check for buffer changes.
        26 Dec 2018 : Use TAB_COLS instead of 8.
        06 Jan 2019 : Speed improvements. Now cut deletes the line. Now paste inserts a new line before the current one.
        08 Jan 2019 : BfEdit() now uses ModifyLine().
        13 Jan 2019 : Minor optimizations.
        18 Jan 2019 : Unselect block if needed.
        18 Jan 2018 : Added K_DELETE.
        27 Jan 2019 : Added support for macros.
        29 Jan 2019 : Added K_CLRCLP.
        24 Dec 2019 : Added support for line numbers.
        26 Dec 2019 : Now K_INTRO is K_CR.
        01 Mar 2020 : Added CLANG support. Set fe_forced flag in ForceGetCh().
        04 Jan 2021 : Use configuration variables.
        25 Sep 2021 : Ignore '\0' characters comming from macro comments. Use editln variable.
           Nov 2021 : (Ladislau Szilagyi) Adapted for RC2014's SC108 128KB RAM memory module
*/

#include <te.h>
#include <tekeys.h>
#include <string.h>
#include <stdlib.h>
#include <dynm128.h>

extern char cf_name[];
extern unsigned char cf_rows;
extern unsigned char cf_cols;
extern int cf_mx_lines;
extern unsigned char cf_tab_cols;
extern unsigned char cf_num;
extern unsigned char cf_clang;
extern unsigned char cf_indent;
extern unsigned char cf_list;
extern char cf_list_chr[];
extern char cf_cr_name[];
extern char cf_esc_name[];
extern unsigned char cf_keys[];
extern unsigned char cf_keys_ex[];
extern unsigned char cf_rul_chr;
extern unsigned char cf_rul_tab;
extern unsigned char cf_vert_chr;
extern unsigned char cf_horz_chr;
extern unsigned char cf_lnum_chr;
extern unsigned int cf_start;
extern unsigned char cf_version;
extern int cf_bytes;
extern int box_rows; /* Height in lines */
extern int box_shr;  /* Vertical   position of cursor in the box (0..box_rows - 1) */
extern int box_shc;  /* Horizontal position of cursor in the box (0..cf_cols - 1) */
extern char** lp_arr;                   /* Pointer of Text lines pointers array */
extern char* lp_arr_i_bank;             /* Pointer of Text lines dynamic memory banks array */
extern int   lp_now; /* How many lines are in the array */
extern int   lp_cur; /* Current line */
extern int   lp_chg; /* 0 if no changes are made */
extern char  ln_dat[]; /* Data buffer */
extern int   ln_max; /* Max. # of characters */
extern int fe_dat[];   /* Data buffer */
extern int fe_now;    /* How many characters are now in the buffer */
extern int fe_set;    /* Set position */
extern int fe_get;    /* Get position */
extern int fe_forced; /* Flag: true if forced character on input */
extern int sysln;    /* NZ when written - for Loop() */
extern int editln;   /* NZ when editing line - for ErrLine() */

extern int tmp;
extern char* ptmp;
extern char  tmpbuf[];
extern char  bank;

#if OPT_BLOCK

extern int blk_start;   /* Start line # */
extern int blk_end;     /* End line # */
extern int blk_count;   /* # of lines */
extern char* clp_arr[];         /* Pointer of Multi-line clipboard lines pointers array */
extern char* clp_arr_i_bank;    /* Pointer of Multi-line clipboard lines memory banks array */

#else

extern char *clp_line;  /* Just one line */

#endif

#if OPT_FIND

extern char find_str[FIND_MAX];

#endif

int ForceCh(char ch);
int ForceStr(char* s);
int ForceGetCh(void);
int ForceChLeft(int len, char ch);
int MacroRunning(void);
int GetKey(void);

/* Edit current line
   -----------------
   Returns last character entered.
*/

int BfEdit()
{
        int i, ch, len, run, upd_lin, upd_col, upd_now, upd_cur, spc, old_len;

        /* Tell we are editing */
        editln = 1;

        /* Get current line contents */

        if (lp_arr_i_bank[lp_cur]==0)
                strcpy(ln_dat, lp_arr[lp_cur]);
        else
                GetString(ln_dat, lp_arr[lp_cur]);

        /* Setup some things */

        len = old_len = strlen(ln_dat);

        run = upd_col = upd_now = upd_cur = 1; upd_lin = spc = 0;

        /* Adjust column position */
        if(box_shc > len)
                box_shc = len;

        /* Loop */
        while(run)
        {
                /* Print line? */
                if(upd_lin)
                {
                        upd_lin = 0;

                        putstr(ln_dat + box_shc);

                        /* Print a space? */
                        if(spc)
                        {
                                putchr(' '); spc = 0;
                        }
                }

                /* Print length? */
                if(upd_now)
                {
                        upd_now = 0;

                        CrtLocate(PS_ROW, PS_COL_NOW); putint("%02d", len);
                }

                /* Print column #? */
                if(upd_col)
                {
                        upd_col = 0;

                        CrtLocate(PS_ROW, PS_COL_CUR); putint("%02d", box_shc + 1);
                }

                /* Locate cursor? */
                if(upd_cur)
                {
                        upd_cur = 0;

                        CrtLocate(BOX_ROW + box_shr, box_shc + cf_num);
                }

                /* Get character and do action */
#if OPT_MACRO
                while((ch = ForceGetCh()) == 0)
                        ;
#else
                ch = ForceGetCh();
#endif

#if OPT_BLOCK
                /* Unselect block if needed: don't check blk_count, we want to unselect start and / or end block selection */
                if(blk_start != -1 || blk_end != -1)
                {
                        if(ch < 1000)
                                upd_cur = 1;
                        else
                        {
                                switch(ch)
                                {
                                        case K_CR :
                                        case K_TAB :
                                        case K_LDEL :
                                        case K_RDEL :
                                        case K_PASTE :
                                        case K_MACRO :
                                                upd_cur = 1;
                                                break;
                                }
                        }

                        if(upd_cur)
                        {
                                LoopBlkUnset();

                                CrtLocate(BOX_ROW + box_shr, box_shc + cf_num);

                                upd_cur = 0;
                        }
                }
#endif
                if(ch < 1000)
                {
                        if(len < ln_max)
                        {
                                putchr(ch);

                                for(i = len; i > box_shc; --i)
                                        ln_dat[i] = ln_dat[i - 1];

                                ln_dat[box_shc++] = ch; ln_dat[++len] = 0;

                                ++upd_lin; ++upd_now; ++upd_col;

                                if(cf_clang)
                                {
#if OPT_MACRO
                                        if(!MacroRunning())
                                        {
#endif
                                                switch(ch)
                                                {
                                                        case '[' : ForceChLeft(len, ']'); break;
                                                        case '{' : ForceChLeft(len, '}'); break;
                                                        case '(' : ForceChLeft(len, ')'); break;
                                                        case '"' : ForceChLeft(len, '"'); break;
                                                        case '\'' : ForceChLeft(len, '\''); break;
                                                        case '*' :
                                                                if(box_shc > 1 && ln_dat[box_shc - 2] == '/' && len + 1 < ln_max)
                                                                { ForceStr("*/"); ForceCh(K_LEFT); ForceCh(K_LEFT);} break;
                                                }
#if OPT_MACRO
                                        }
#endif
                                }
                        }

                        ++upd_cur;
                }
                else
                {
                        /* Note: This function does preliminary checks in some
                           keys for Loop(), to avoid wasted time. */
                        switch(ch)
                        {
                                case K_LEFT :    /* Move one character to the left -------- */
                                        if(box_shc)
                                        {
                                                --box_shc; ++upd_col;
                                        }
                                        else if(lp_cur)
                                        {
                                                box_shc = 9999 /* strlen(lp_arr[lp_cur - 1]) */ ;

                                                ch = K_UP;

                                                run = 0;
                                        }
                                        ++upd_cur;
                                        break;
                                case K_RIGHT :   /* Move one character to the right ------- */
                                        if(box_shc < len)
                                        {
                                                ++box_shc; ++upd_col;
                                        }
                                        else if(lp_cur < lp_now - 1)
                                        {
                                                ch = K_DOWN;

                                                box_shc = run = 0;
                                        }
                                        ++upd_cur;
                                        break;
                                case K_LDEL :   /* Delete one character to the left ------- */
                                        if(box_shc)
                                        {
                                                strcpy(ln_dat + box_shc - 1, ln_dat + box_shc);

                                                --box_shc; --len; ++upd_now; ++upd_lin; ++spc; ++upd_col;

                                                putchr('\b');
                                        }
                                        else if(lp_cur)
                                                run = 0;
                                        ++upd_cur;
                                        break;
                                case K_RDEL :   /* Delete one character to the right ----- */
                                        if(box_shc < len)
                                        {
                                                strcpy(ln_dat + box_shc, ln_dat + box_shc + 1);

                                                --len; ++upd_now; ++upd_lin; ++spc;
                                        }
                                        else if(lp_cur < lp_now -1)
                                                run = 0;
                                        ++upd_cur;
                                        break;
                                case K_UP :     /* Up one line --------------------------- */
                                        if(lp_cur)
                                                run = 0;
                                        ++upd_cur;
                                        break;
                                case K_DOWN :   /* One line down ------------------------- */
                                        if(lp_cur < lp_now - 1)
                                                run = 0;
                                        ++upd_cur;
                                        break;
#if OPT_BLOCK
                                case K_BLK_START : /* Set block start -------------------- */
                                case K_BLK_END :   /* Set block end ---------------------- */
                                case K_BLK_UNSET : /* Unset block  ----------------------- */
#endif

#if OPT_GOTO
                                case K_GOTO :  /* Go to line # -------------------------- */
#endif

#if OPT_MACRO
                                case K_MACRO :  /* Execute macro from file -------------- */
#endif
                                case K_COPY :   /* Copy block/line to the clipboard ------ */
                                case K_CUT :    /* Copy and delete block/line ------------ */
                                case K_PASTE :  /* Paste clipboard before the current line */
                                case K_DELETE : /* Delete block/line --------------------- */
                                case K_CLRCLP : /* Clear the clipboard ------------------- */
                                case K_ESC :    /* Escape: Show the menu ----------------- */
                                case K_CR :     /* Insert CR (split the line) ------------ */
                                        run = 0;
                                        break;
                                case K_PGUP :   /* Page up ------------------------------- */
                                case K_TOP :    /* Document top -------------------------- */
                                        if(lp_cur || box_shc)
                                                run = 0;
                                        ++upd_cur;
                                        break;

                                case K_PGDOWN : /* Page down ----------------------------- */
                                case K_BOTTOM : /* Document bottom ----------------------- */
                                        if(lp_cur < lp_now - 1 || box_shc != len)
                                                run = 0;
                                        ++upd_cur;
                                        break;

                                case K_BEGIN :  /* Begin of line ------------------------- */
                                        if(box_shc)
                                        {
                                                box_shc = 0; ++upd_col;
                                        }
                                        ++upd_cur;
                                        break;
                                case K_END :    /* End of line --------------------------- */
                                        if(box_shc != len)
                                        {
                                                box_shc = len; ++upd_col;
                                        }
                                        ++upd_cur;
                                        break;
                                case K_TAB :    /* Insert TAB (spaces) ------------------- */
                                        i = cf_tab_cols - box_shc % cf_tab_cols;

                                        while(i--)
                                        {
                                                if(ForceCh(' '))
                                                        break;
                                        }
                                        break;
#if OPT_LWORD
                                case K_LWORD :  /* Move one word to the left ------------ */

                                        if(box_shc)
                                        {
                                                /* Skip the current word if we are at its begining */
                                                if(ln_dat[box_shc] != ' ' && ln_dat[box_shc - 1] == ' ')
                                                        --box_shc;

                                                /* Skip spaces */
                                                while(box_shc && ln_dat[box_shc] == ' ')
                                                        --box_shc;

                                                /* Find the beginning of the word */
                                                while(box_shc && ln_dat[box_shc] != ' ')
                                                {
                                                        /* Go to the beginning of the word */
                                                        if(ln_dat[--box_shc] == ' ')
                                                        {
                                                                ++box_shc; break;
                                                        }
                                                }

                                                ++upd_col;
                                        }

                                        ++upd_cur;

                                        break;
#endif

#if OPT_RWORD
                                case K_RWORD :  /* Move one word to the right ----------- */

                                        /* Skip current word */
                                        while(ln_dat[box_shc] && ln_dat[box_shc] != ' ')
                                                ++box_shc;

                                        /* Skip spaces */
                                        while(ln_dat[box_shc] == ' ')
                                                ++box_shc;

                                        ++upd_col; ++upd_cur;

                                        break;
#endif

#if OPT_FIND
                                case K_FIND :   /* Find string -------------------------- */
                                        run = 0;
                                        break;

                                case K_NEXT :   /* Find next string --------------------- */
                                        if(find_str[0])
                                                run = 0;
                                        break;
#endif
                        }
                }
        }

        /* Update changes if any */

        if(len == old_len)
        {
                /* Check for changes */

                if (lp_arr_i_bank[lp_cur]==0)
                        strcpy(tmpbuf, lp_arr[lp_cur]);
                else
                        GetString(tmpbuf, lp_arr[lp_cur]);

                if (memcmp(tmpbuf, ln_dat, len))
                {
                        /* Update the changes */

                        if (lp_arr_i_bank[lp_cur]==0)
                                strcpy(lp_arr[lp_cur], ln_dat);
                        else
                                PutString(ln_dat, lp_arr[lp_cur]);

                        /* Changes are not saved */
                        lp_chg = 1;
                }
        }
        else
        {
                ModifyLine(lp_cur, ln_dat); /* FIX-ME: Re-print the line with old contents in case of error? */

                /* Changes are not saved */
                lp_chg = 1;
        }

        /* Tell we are not editing */
        editln = 0;

        /* Return last character entered */
        return ch;
}


/* Add a character to forced entry buffer
   --------------------------------------
   Return Z on success, NZ on failure.
*/
int ForceCh(char ch)
{
        if(fe_now < FORCED_MAX)
        {
                ++fe_now;

                if(fe_set == FORCED_MAX)
                        fe_set = 0;

                fe_dat[fe_set++] = ch;
                return 0;
        }

        return -1;
}

/* Add a string to forced entry buffer
   -----------------------------------
   Return Z on success, NZ on failure.
*/
int ForceStr(char* s)
{
        while(*s)
        {
                if(ForceCh(*s++))
                        return -1;
        }

        return 0;
}

/* Return character from forced entry buffer, or keyboard
   ------------------------------------------------------
*/
int ForceGetCh()
{
        if(fe_now)
        {
                --fe_now;

                if(fe_get == FORCED_MAX)
                        fe_get = 0;

                fe_forced = 1;
                return fe_dat[fe_get++];
        }

#if OPT_MACRO

        if(MacroRunning())
        {
                MacroGet();

                if(fe_now)
                        return ForceGetCh();
        }

#endif

        fe_forced = 0;

        return getchr();
}

/* Single character completion for C language, etc.
   ------------------------------------------------
*/
int ForceChLeft(int len, char ch)
{
        if(!fe_forced)
        {
                if(len < ln_max)
                {
                        ForceCh(ch); ForceCh(K_LEFT);
                }
        }
}

