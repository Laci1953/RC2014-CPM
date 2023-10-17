/*      te.c

        Text editor.

        Main module.

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

        Notes:

        Change TE_VERSION in 'te.h' as required, before compilation.

        Changes:

        29 Apr 2015 : Start working.
        02 May 2015 : Added Amstrad PCW version (te_pcw.c).
        03 May 2015 : Added K. Murakami's CP/M emulator for Win32 version (te_em1.c).
        04 May 2015 : Added te.h file. Added forced entry. Nice ruler with tab stops.
        07 May 2015 : While porting it to Pelles C & Windows 32 bit, it was discovered
                      collateral efects in 3 pieces of code. Recoded and marked below.
                      Solved bug in ReadFile() - characters > 0x7F caused 'bad
                      character' errors.
                      Added checking for filenames too long in command line.
        08 May 2015 : v1.00 : Added BackupFile().
        12 May 2015 : v1.01 : Now K_CUT copies the line to the clipboard before deleting it.
        14 May 2015 : v1.02 : Added support for word left & word right cursor movement.
                              You must define K_LWORD and K_RWORD in your CRT_FILE
                              to activate them.
        15 May 2015 : v1.03 : Modified getch & putch to getchr & putchr.
        31 Aug 2015 : v1.04 : Minor changes in comments and ReadLine().
        02 Jun 2016 : v1.05 : Minor changes.
        10 Jun 2016 : v1.06 : Screen optimizations in Menu(). Removed BOX_COL.
                              Removed lp_max, box_cols, ps_fname, ps_lin_cur, ps_lin_now, ps_lin_max,
                              ps_col_cur, ps_col_now, ps_col_max.
        14 Jun 2016 : v1.07 : Hack for SamaruX.
        05 Jul 2017 : v1.08 : Optimizations in NULL comparisons. Include CC_FGETS.
        24 Jan 2018 : v1.09 : Added find string and find next string commands.
        20 Feb 2018 : v1.10 : Added command to execute macro from file. Split te.c in modules. Added go to line #.
                              Disable code for macros from strings, for now.
        22 Feb 2018 : v1.11 : Ask for confirmation only if changes were not saved.
        06 Jan 2019 : Included te_lines module and modified related functions. Added LoopCut(), LoopPaste().
        13 Jan 2019 : Included te_misc module. Minor changes.
        15 Jan 2019 : Added LoopBlkStart(), LoopBlkEnd(), LoopBlkUnset(), LoopCopy().
        18 Jan 2019 : Added K_DELETE, LoopDelete().
        19 Jan 2019 : Added LoopUp(), LoopDown().
        22 Jan 2019 : Added te_keys module. Added support for key bindings.
        27 Jan 2019 : Added support for macros.
        29 Jan 2019 : Added K_CLRCLP. Show clipboard status.
        30 Jan 2019 : Removed support for SamaruX.
        14 Feb 2019 : Added help items layout.
        24 Dec 2019 : Added support for line numbers.
        26 Dec 2019 : Now K_INTRO is K_CR, LoopIntro() is LoopCr().
        01 Mar 2020 : Added fe_forced.
        02 Mar 2020 : Added automatic indentation.
        08 Mar 2020 : Support for CRT_LONG.
        22 Dec 2020 : Add default filetype to macro filenames.
        30 Dec 2020 : Solved bug in LoopCr() - bad column position after line break.
                      Solved bug in LoopDelete() - did nothing if no selection, must delete current line.
                                  Added LoopBlkEx(), LoopDeleteEx().
                                  Added automatic list.
        04 Jan 2021 : Use configuration variables.
        22 Feb 2021 : Removed CRT_ROWS, CRT_COLS.
        04 Apr 2021 : Move key bindings to configuration. Remove customized key names.
        30 Jun 2021 : Get CP/M version. Adjust auto rows and columns configuration values.
        01 Jul 2021 : Check if macro is running to avoid auto-indentation and lists side effects.
        06 Jul 2021 : Optimize LoopCr(), LoopLeftDel(), LoopRightDel() a bit.
        25 Sep 2021 : Add editln variable. Use SysLineEdit() when editing.
           Nov 2021 : (Ladislau Szilagyi) Adapted for RC2014's SC108 128KB RAM memory module
           Mar 2022 : (Ladislau Szilagyi) Adapted for RC2014's 512KB RAM memory module
           Mar 2023 : (Ladislau Szilagyi) Review & created single source for 128/512

        Notes:

        See FIX-ME notes.
*/
/* Libraries
   ---------
*/
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <cpm.h>
#include <dynm.h>

/* TE definitions
   --------------
*/
#include <te.h>
#include <tekeys.h>

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

int   tmp;
char* ptmp;
char  tmpbuf[401];
char  b;

/* Operating system
   ----------------
*/
int cpm_ver;  /* CP/M version */

/* Array of text lines
   -------------------
*/
char** lp_arr;                  /* Pointer of Text lines pointers array */
char b_lp_arr;
char* lp_arr_i_b;               /* Pointer of Text lines dynamic memory banks array */
char b_lp_arr_i_b;

int   lp_now; /* How many lines are in the array */
int   lp_cur; /* Current line */
int   lp_chg; /* 0 if no changes are made */

/* Current line
   ------------
*/
char ln_dat[200]; /* Data buffer */
int   ln_max; /* Max. # of characters */

/* Clipboard & block selection
   ---------------------------
*/
#if OPT_BLOCK

int blk_start;   /* Start line # */
int blk_end;     /* End line # */
int blk_count;   /* # of lines */

char* clp_arr[CLP_LINES_MAX];           /* Multi-line clipboard lines pointers array */
char clp_arr_i_b[CLP_LINES_MAX];        /* Multi-line clipboard lines memory banks array */

int   clp_count; /* # of lines */

#else

char *clp_line;  /* Just one line */

#if	Z80ALL
char clipboard[128];
#endif

#endif

/* Filename
   --------
*/
char file_name[FILENAME_MAX];

/* Editor box
   ----------
*/
int offset;   /* How many chars the box is shifted to the right */
int box_rows; /* Height in lines */
int box_shr;  /* Vertical   position of cursor in the box (0..box_rows - 1) */
int box_shc;  /* Horizontal position of cursor in the box (0..cf_cols - 1) */

/* Keyboard forced entry
   ---------------------
*/
int fe_dat[FORCED_MAX];   /* Data buffer */
int fe_now;    /* How many characters are now in the buffer */
int fe_set;    /* Set position */
int fe_get;    /* Get position */
int fe_forced; /* Flag: true if forced character on input */

/* System line
   -----------
*/
int sysln;    /* NZ when written - for Loop() */

/* Edit line
   ---------
*/
int editln;   /* NZ when editing line - for ErrLine() */

#if OPT_FIND

/* Find string
   -----------
*/
char find_str[FIND_MAX];

#endif

#if OPT_MACRO

/* Macros
   ------
*/
FILE *mac_fp;  /* FP for a file macro, or NULL */
/*char *mac_str;*/ /* Address for a string macro, or NULL */

#endif

/* Help items layout
   -----------------
*/
int help_items[] = {
        K_UP,        K_DOWN,    K_TAB,
        K_LEFT,      K_RIGHT,   K_CR,
        K_BEGIN,     K_END,     K_ESC,
        K_TOP,       K_BOTTOM,  K_MACRO,
        K_PGUP,      K_PGDOWN,  0,
        K_LWORD,     K_RWORD,   0,
        K_LDEL,      K_RDEL,    0,
        K_BLK_START, K_BLK_END, K_BLK_UNSET,
        K_CUT,       K_COPY,    K_PASTE,
        K_DELETE,    K_CLRCLP,  0,
        K_FIND,      K_NEXT,    K_GOTO,
        -1
};

void* AllocMem(short bytes, char* bank);
void ErrLineMem(void);
int ReadFile(char* p);
int BfEdit(void);
int Menu(void);
int GetFirstLine(void);
int GetLastLine(void);
int SplitLine(int,int);
int AppendLine(int,char*);
int InsertLine(int,char*);
int MacroRunning(void);
int LoopCopyEx(void);
int JoinLines(int);
int DeleteLine(int);
int SysLineStr(char* what, char* buf, int maxlen);
char* FreeClipboard(void);
void    XGetString(char* dest, char* src, char src_flag);
void    XPutString(char* source, char* dest, char dest_flag);
int     XStringLen(char* s, char s_flag);

int topmain(int argc, char** argv);

void type(char* text)
{
       sprintf(ln_dat, text);
       putstr(ln_dat);
}

/* Program entry
   -------------
*/
int main(int argc, char** argv)
{
        int i;

        CrtSetup();
	CrtClear();

        /* Check command line */
        if (argc == 1)
        {
                /* display help */
                type("TE text editor\r\n");
                type("Copyright (c) 2015-2021 Miguel Garcia / FloppySoftware\r\n");
#ifdef  M128
                type("Adapted for 2x64KB RAM by Ladislau Szilagyi\r\n");
#endif
#ifdef  M512
                type("Adapted for 512KB RAM by Ladislau Szilagyi\r\n");
#endif
#ifdef  Z80ALL
                type("Adapted for 4x32KB RAM by Ladislau Szilagyi\r\n");
#endif
                type("\r\nUse: >TE [options] [filename]\r\n");
                type("\r\nwhere options (lowercase or uppercase) are:\r\n");
                type(" -Tn (n = 4 or 8) sets the tabs to n spaces, default is 8\r\n");
#ifdef Z80ALL
                type(" -Hnn sets the screen height (up to 48, default is 48)\r\n");
                type(" -Wnnn sets the screen width (up to 64, default is 64)\r\n");
#else
                type(" -Hnn sets the screen height (up to 99, default is 24)\r\n");
                type(" -Wnnn sets the screen width (up to 200, default is 80)\r\n");
#endif
                sprintf(ln_dat, "\r\nText files can have lines of up to %d characters\r\n", LINE_SIZE_MAX);
		putstr(ln_dat);
                type("\r\nContinue? (N/n = quit) :");

                if ((i=CrtInEx()) == 'N' || i == 'n')
                        exit(1);
        }
        else
        {
                while(*++argv)
                {
                        if((argv)[0][0] == '-')
                        {
                                /* options T,H,W */

                                if(islower(i = argv[0][1]))
                                        i = toupper(i);

                                switch(i)
                                {
                                case    'T':    //tabs
                                        cf_tab_cols = argv[0][2];
                                        if (cf_tab_cols == '4' || cf_tab_cols == '8')
                                                cf_tab_cols = cf_tab_cols - '0';
                                        break;
                                case    'H':    //screen height
                                        cf_rows = atoi(argv[0] + 2);
                                        if (!cf_rows || cf_rows > CRT_MAX_ROWS)
                                                goto wrong;
                                        break;
                                case    'W':    //screen width
                                        cf_cols = atoi(argv[0] + 2);
                                        if (!cf_cols || cf_cols > CRT_MAX_COLS)
                                                goto wrong;
                                        break;
                                default:
wrong:
                                        printf("Wrong option!");
                                        exit(1);
                                }
                        }
                        else
                                break;  /* must be a file name... */
                }
        }

        /* Setup some globals */
#if CRT_LONG
        box_rows = cf_rows - 4;
#else
        box_rows = cf_rows - 2;
#endif

        /* Max. number of lines */
#ifdef  M64
        cf_mx_lines = 2048;
#else
#ifdef  M128
        cf_mx_lines = 4093;
#else
        cf_mx_lines = 8189;
#endif
#endif

        /* Max. width of lines */
        ln_max = cf_cols - cf_num - 1;

        /* Get CP/M version */
        cpm_ver = bdos(0x0C, 0x0000);

        InitDynM();     /* init 64KB Upper RAM dynamic memory */

        /* alloc text lines pointers array */
        /* For 128KB version & Z80ALL version - in lower 64KB RAM */
#ifdef Z80ALL
        lp_arr = (int*)HEAP2;
        b_lp_arr = LOW64;
#else
        lp_arr = Alloc(cf_mx_lines * 2, &b_lp_arr);
#endif

        /* alloc text lines dynamic memory banks array */
        /* For 128KB version & Z80ALL version - in lower 64KB RAM */
#ifdef Z80ALL
        lp_arr_i_b = (char*)HEAP1;
        b_lp_arr_i_b = LOW64;
#else
        lp_arr_i_b = Alloc(cf_mx_lines, &b_lp_arr_i_b);
#endif

        /* Setup clipboard */
#if OPT_BLOCK
        clp_count = 0;
#else
#ifdef  Z80ALL
        clp_line = clipboard;
#else
        clp_line = malloc(200);
#endif
        *clp_line = '\0';
#endif

        /* init line pointers */
        for(i = 0; i < cf_mx_lines; ++i)
                PutWord(lp_arr, i, NULL, b_lp_arr);

#if OPT_BLOCK
        /* init clipboard line pointers */
        for(i = 0; i < CLP_LINES_MAX; ++i)
                clp_arr[i] = NULL;
#endif

        /* Print layout */
        Layout();

        if(argc == 1 || *argv == 0)
                NewFile();
        else
        {
                if(strlen(argv[0]) > FILENAME_MAX - 1)
                {
                        ErrLine("Filename too long");
                        NewFile();
                }
                else if(ReadFile(argv[0]))
                        NewFile();
                else
                        strcpy(file_name, argv[0]);
        }

        /* Main loop */
        Loop();

        /* Clear & reset CRT */
        CrtClear();
        CrtReset();

        /* Exit */
        return 0;
}

/* Main loop
   ---------
*/
Loop()
{
        int run, ch;

        /* Setup forced entry */
        fe_now = fe_get = fe_set = 0;

        /* Setup more things */
        run = sysln = 1;
        offset = 0;

        /* Print filename */
        ShowFilename();

        /* Refresh editor box */
        RefreshAll();

        /* Loop */
        while(run)
        {
                /* Refresh system line message if it changed */
                if(sysln)
                {
                        SysLineEdit();
                        sysln = 0;
                }

                /* Print clipboard status */
                CrtLocate(PS_ROW, PS_CLP);

#if OPT_BLOCK
        putstr(clp_count ? "CLP" : "---");
#else
        putstr(*clp_line ? "CLP" : "---");
#endif

                /* Print current line number, etc. */
                CrtLocate(PS_ROW, PS_LIN_CUR); putint("%04d", lp_cur + 1);
                CrtLocate(PS_ROW, PS_LIN_NOW); putint("%04d", lp_now);

                /* Edit the line */
                ch = BfEdit();

                /* Note: BfEdit() does previous checks for following
                   actions, to not waste time when an action is not
                   possible */

                /* Check returned control character */
                switch(ch)
                {
                        case K_UP :    /* Up one line --------------------- */
                                LoopUp();
                                break;
                        case K_DOWN :  /* Down one line ------------------- */
                                LoopDown();
                                break;
                        case K_CR :    /* Insert CR ----------------------- */
                                LoopCr();
                                break;
                        case K_LDEL :  /* Delete CR on the left ----------- */
                                LoopLeftDel();
                                break;
                        case K_RDEL :  /* Delete CR on the right ---------- */
                                LoopRightDel();
                                break;
                        case K_PGUP :  /* Page up ------------------------- */
                                LoopPgUp();
                                break;
                        case K_PGDOWN :/* Page down ----------------------- */
                                LoopPgDown();
                                break;
                        case K_TOP :   /* Top of document ----------------- */
                                LoopTop();
                                break;
                        case K_BOTTOM :/* Bottom of document -------------- */
                                LoopBottom();
                                break;
                        case K_COPY :   /* Copy line or block ------------- */
                                LoopCopy();
                                break;
                        case K_CUT :    /* Copy and delete line or block -- */
                                LoopCut();
                                break;
                        case K_PASTE :  /* Paste line or block ------------ */
                                LoopPaste();
                                break;
                        case K_DELETE : /* Delete line or block ----------- */
                                LoopDelete();
                                break;
                        case K_CLRCLP : /* Clear the clipboard ------------ */
                                LoopClrClp();
                                break;
#if OPT_BLOCK
                        case K_BLK_START : /* Set block start ------------- */
                                LoopBlkStart();
                                break;
                        case K_BLK_END :   /* Set block end --------------- */
                                LoopBlkEnd();
                                break;
                        case K_BLK_UNSET : /* Unset block ----------------- */
                                LoopBlkUnset();
                                break;
#endif
#if OPT_FIND
                        case K_FIND :  /* Find string --------------------- */
                                LoopFindFirst();
                                break;
                        case K_NEXT :  /* Find next string ---------------- */
                                LoopFindNext();
                                break;
#endif
#if OPT_GOTO
                        case K_GOTO :  /* Go to line # -------------------- */
                                LoopGoLine();
                                break;
#endif
#if OPT_MACRO
                        case K_MACRO : /* Execute macro from file --------- */
                                LoopMacro();
                                break;
#endif
                        case K_ESC :   /* Show the menu ------------------- */
                                if(Menu())
                                        run = 0;
                                else
                                {
                                        ShowFilename(); /* Refresh filename */
                                        RefreshAll();   /* Refresh editor box */
                                }
                                break;
                        case K_REFRESH:
                                RefreshAll();
                                break;
                }
        }
}

/* Go one line up
   --------------
*/
LoopUp()
{
        --lp_cur; /* FIXME -- check if we are on the 1st line? */

        if(box_shr)
                --box_shr;
        else
                Refresh(0, lp_cur);
}

/* Go one line down
   ----------------
*/
LoopDown()
{
        ++lp_cur; /* FIXME -- check if we are on the last line? */

        if(box_shr < box_rows - 1)
                ++box_shr;
        else
                Refresh(0, lp_cur - box_rows + 1);
}

/* Go to document top
   ------------------
*/
LoopTop()
{
        int first;

        first = GetFirstLine();

        lp_cur = box_shr = box_shc = offset = 0;

        if(first > 0)
                RefreshAll();
}

/* Go to document bottom
   ---------------------
*/
LoopBottom()
{
        int first, last;

        first = GetFirstLine();
        last = GetLastLine();

        lp_cur = lp_now - 1;
        box_shc = offset = 0;

        if(last < lp_now - 1)
        {
                box_shr = box_rows - 1;
                RefreshAll();
        }
        else
                box_shr = last - first;
}

/* Page up
   -------
*/
LoopPgUp()
{
        int first, to;

        first = GetFirstLine();

        if(first)
        {
                if((to = first - box_rows) < 0) /* max(to, 0) */
                        to = 0;

                lp_cur = to; box_shr = box_shc = offset = 0;

                RefreshAll();
        }
        else
                LoopTop();
}

/* Page down
   ---------
*/
LoopPgDown()
{
        int to;

        if(GetLastLine() < lp_now - 1)
        {
                to = GetFirstLine() + box_rows;

                if(to >= lp_now)
                        to = lp_now - 1;

                lp_cur = to; box_shr = box_shc = offset = 0;

                RefreshAll();
        }
        else
                LoopBottom();
}

/* Insert CR
   ---------
*/
LoopCr()
{
        int ok;
        int i, k;

        if(offset + box_shc)
        {
                if(ln_dat[offset + box_shc])
                {
                        /* Cursor is in the middle of the line */
                        if((ok = SplitLine(lp_cur, offset + box_shc)))
                        {
                                if (offset == 0)
                                        CrtClearEol();
                        }
                }
                else
                        ok = AppendLine(lp_cur, NULL); /* Cursor is at the end of the line */

                if (offset)
                {
                        offset = 0;
                        RefreshAll();
                }
        }
        else
        {
                /* Cursor is in first column */
                if((ok = InsertLine(lp_cur, NULL)))
                {
                        if(ln_dat[0])
                                CrtClearEol();  /* Line is not empty */
                }
        }

        if(ok)
        {
                ++lp_cur;
                k = 0;
#if OPT_MACRO
                if(!MacroRunning())
                {
#endif
                        if(box_shc)
                        {
                                i = 0;

                                if(cf_indent)
                                {
                                        while(ln_dat[i] == ' ')
                                                ++i;
                                }

                                if(cf_list)
                                {
                                        if(strchr(cf_list_chr, ln_dat[i]) && ln_dat[i + 1] == ' ')
                                                i += 2;
                                }

                                if(i)
                                {
                                        k = i;
                                        XGetString(ln_dat + i, GetWord(lp_arr, lp_cur, b_lp_arr), GetByte(lp_arr_i_b, lp_cur, b_lp_arr_i_b));
                                        ModifyLine(lp_cur, ln_dat);
                                }
                        }
#if OPT_MACRO
                }
#endif
                if(box_shr < box_rows - 1)
                {
                        ++box_shr;
                        Refresh(box_shr, lp_cur);
                }
                else
                        Refresh(0, lp_cur - box_rows + 1);

                box_shc = k;
                lp_chg = 1;
        }
}

#if OPT_BLOCK
/* Set block start
   ---------------
*/
LoopBlkStart()
{
        if(blk_start != -1 || (blk_end != -1 && lp_cur > blk_end))
                LoopBlkUnset();

        blk_start = lp_cur;

        if(blk_end != -1)
        {
                RefreshBlock(box_shr, 1);
                blk_count = blk_end - blk_start + 1;
        }
}

/* Set block end
   -------------
*/
LoopBlkEnd()
{
        if(blk_end != -1 || (blk_start != -1 && lp_cur < blk_start) )
                LoopBlkUnset();

        blk_end = lp_cur;

        if(blk_start != -1)
        {
                RefreshBlock(0, 1);  /* FIXME -- optimize */
                blk_count = blk_end - blk_start + 1;
        }
}

/* Unset block
   -----------
*/
LoopBlkUnset()
{
        if(blk_count)
        {
                if(blk_start <= GetLastLine() && blk_end >= GetFirstLine())
                        RefreshBlock(0, 0);  /* FIXME -- optimize */
        }

        blk_start = blk_end = -1;
        blk_count = 0;
}

LoopBlkEx()
{
        if(!blk_count)
        {
                blk_start = blk_end = lp_cur;
                blk_count = 1;
        }
}
#endif

/* Copy line
   ---------
*/
LoopCopy()
{
#if OPT_BLOCK
        LoopBlkEx();

        if(LoopCopyEx())
                LoopBlkUnset();
#else
        strcpy(clp_line, ln_dat);
#endif
}

#if OPT_BLOCK

LoopCopyEx()
{
        int i;

        LoopClrClp();

        if (blk_count > CLP_LINES_MAX)
          return 0;

        for(i = 0; i < blk_count; ++i)
        {
                tmp = XStringLen(GetWord(lp_arr, blk_start + i, b_lp_arr), GetByte(lp_arr_i_b, blk_start + i, b_lp_arr_i_b));
                ptmp = AllocMem(tmp+1, &b);

                if (ptmp)
                {
                        clp_arr_i_b[i] = b;
                        clp_arr[i] = ptmp;
                        XGetString(tmpbuf, GetWord(lp_arr, blk_start + i, b_lp_arr), GetByte(lp_arr_i_b, blk_start + i, b_lp_arr_i_b));
                        XPutString(tmpbuf, clp_arr[i], clp_arr_i_b[i]);
                }
                else
                {
                        FreeClipboard();
                        return 0;
                }
        }

        clp_count = blk_count;

        return 1;
}

#endif

/* Delete line
   -----------
*/
LoopDelete()
{

#if OPT_BLOCK

        LoopBlkEx();
        LoopDeleteEx();

#else

        if(lp_cur != lp_now - 1)
                DeleteLine(lp_cur);
        else
                ClearLine(lp_cur);

        offset = 0;

        Refresh(box_shr, lp_cur);

        box_shc = 0;

        lp_chg = 1;

#endif

}

#if OPT_BLOCK

LoopDeleteEx()
{
        LoopGo(blk_start);

        while(blk_count--)
        {
                if(blk_start != lp_now - 1)
                        DeleteLine(blk_start);
                else
                        ClearLine(blk_start);

                --blk_end;

                Refresh(box_shr, lp_cur);
        }

        blk_start = blk_end = -1;
        blk_count = 0;

        box_shc = 0;

        lp_chg = 1;
}

#endif

/* Copy and delete line
   --------------------
*/
LoopCut()
{
#if OPT_BLOCK
        LoopBlkEx();

        if(LoopCopyEx())
                LoopDeleteEx();
#else
        strcpy(clp_line, ln_dat);

        LoopDelete();
#endif
}

/* Paste line
   ----------
*/
LoopPaste()
{
#if OPT_BLOCK

        int i;

        if(clp_count)
        {
                for(i = 0; i < clp_count; ++i)
                {
                        XGetString(tmpbuf, clp_arr[i], clp_arr_i_b[i]);

                        if(InsertLine(lp_cur, tmpbuf))
                        {
                                Refresh(box_shr, lp_cur);
                                LoopDown();
                        }
                        else
                                break;
                }

                box_shc = 0;
                lp_chg = 1;
        }
#else
        if((InsertLine(lp_cur, clp_line)))
        {
                Refresh(box_shr, lp_cur);
                LoopDown();

                box_shc = 0;

                lp_chg = 1;

        }
#endif
}

/* Clear the clipboard
   -------------------
*/
LoopClrClp()
{
#if OPT_BLOCK
        if(clp_count)
        {
                FreeClipboard();
                clp_count = 0;
        }
#else

        *clp_line = '\0';
#endif
}

/* Delete CR on the left
   ---------------------
*/
LoopLeftDel()
{
        char *p;
        int ok, rs, pos;

        if(ln_dat[0])
        {
                /* Line is not empty */

                XGetString(tmpbuf, GetWord(lp_arr, lp_cur - 1, b_lp_arr), GetByte(lp_arr_i_b, lp_cur - 1, b_lp_arr_i_b));
                p = tmpbuf;

                if(*p) {
                        /* Previous line is not empty */

                        pos = strlen(p);

                        if((ok = JoinLines(lp_cur - 1)))
                                rs = 0;
                }
                else
                {
                        /* Previous line is empty */
                        if((ok = DeleteLine(lp_cur - 1)))
                        {
                                rs = 0;
                                pos = 0;
                        }
                }
        }
        else
        {
                /* Line is empty */
                if((ok = DeleteLine(lp_cur)))
                {
                        rs = 1;
                        pos = 999;
                }
        }

        if(ok)
        {
                --lp_cur;

                if(box_shr)
                {
                        --box_shr;
                        Refresh(box_shr + rs, lp_cur + rs);
                }
                else
                        Refresh(0, lp_cur);

                box_shc = pos;
                lp_chg = 1;
        }
}

/* Delete CR on the right
   ----------------------
*/
LoopRightDel()
{
        char *p;
        int ok, rs;

        XGetString(tmpbuf, GetWord(lp_arr, lp_cur + 1, b_lp_arr), GetByte(lp_arr_i_b, lp_cur + 1, b_lp_arr_i_b));
        p = tmpbuf;

        if(ln_dat[0])
        {
                /* Line is not empty */
                if(*p)
                {
                        /* Next line is not empty */
                        if((ok = JoinLines(lp_cur)))
                                rs = 0;
                }
                else
                {
                        /* Next line is empty */
                        if((ok = DeleteLine(lp_cur + 1)))
                                rs = 1;
                }
        }
        else
        {
                /* Line is empty */
                if((ok = DeleteLine(lp_cur)))
                {
                        if(*p)
                                rs = 0; /* Next line is not empty */
                        else
                                rs = 1; /* Next line is empty */
                }
        }

        if(ok)
        {
                if(box_shr + rs < box_rows)
                        Refresh(box_shr + rs, lp_cur + rs);

                lp_chg = 1;
        }
}

#if OPT_FIND
/* Find string
   -----------
*/
LoopFind()
{
        int slen, flen, line, row, col, i;
        char *p;

        row  = box_shr;
        col  = box_shc + offset;
        flen = strlen(find_str);

        for(line = lp_cur; line < lp_now; ++line)
        {
                XGetString(tmpbuf, GetWord(lp_arr, line, b_lp_arr), GetByte(lp_arr_i_b, line, b_lp_arr_i_b));
                p = tmpbuf;

                for(slen = strlen(p); flen <= (slen - col) && col < slen; ++col)
                {
                        for(i = 0; i < flen && find_str[i] == p[col + i]; ++i)
                                ;

                        if(i == flen)
                        {
                                /* Found, set new cursor position and refresh the screen if needed */
                                lp_cur = line;

                                if(row < box_rows)
                                        box_shr = row;
                                else
                                        Refresh((box_shr = 0), lp_cur);

                                if (offset == 0)
                                {
                                        if (col >= ln_max)
                                        {
                                                offset = ln_max & 0xFFF8;
                                                box_shc = col - offset;
                                                RefreshAll();
                                        }
                                        else
                                                box_shc = col;
                                }
                                else
                                {
                                        if (col < ln_max)
                                        {
                                                offset = 0;
                                                box_shc = col;
                                                RefreshAll();
                                        }
                                        else
                                                box_shc = col - offset;
                                }

                                return 1;
                        }
                }

                ++row; col = 0;
        }

        /* Not found */
        return 0;
}

/* Find first string
   -----------------
*/
LoopFindFirst()
{
        find_str[0] = '\0';

        if(SysLineStr("Find", find_str, FIND_MAX - 1))
                LoopFind();
}

/* Find next string
   ----------------
*/
LoopFindNext()
{
        int old_box_shc;

        if(find_str[0])
        {
                old_box_shc = box_shc;

                /* Skip current character */
                tmp = XStringLen(GetWord(lp_arr, lp_cur, b_lp_arr), GetByte(lp_arr_i_b, lp_cur, b_lp_arr_i_b));

                if(box_shc < tmp)
                        ++box_shc;

                /* Set old cursor position on find failure */
                if(!LoopFind())
                        box_shc = old_box_shc;
        }
}

#endif

#if OPT_GOTO
/* Go to line # (1..X)
   -------------------
*/
LoopGoLine()
{
        char buf[6];
        int line;

        buf[0] = '\0';

        if(SysLineStr("Go to line #", buf, 5))
        {
                line = atoi(buf);

                if(line > 0 && line <= lp_now)
                        LoopGo(line - 1);
        }
}

#endif

/* Go to line # (0..X)
   -------------------
*/
LoopGo(line)
int line;
{
        int first, last;

        first = GetFirstLine();
        last = GetLastLine();

        lp_cur = line;
        box_shc = 0;

        if(lp_cur >= first && lp_cur <= last)
        {
                box_shr = lp_cur - first;

                if (offset)
                {
                        offset = 0;
                        RefreshAll();
                }
        }
        else
        {
                offset = 0;
                Refresh((box_shr = 0), lp_cur);
        }
}

#if OPT_MACRO

/* Execute macro from file
   -----------------------
*/
LoopMacro()
{
        char fn[FILENAME_MAX];
        int len_type;

        fn[0] = '\0';

        if(SysLineStr("Macro", fn, FILENAME_MAX - 1))
        {
                len_type = (strchr(fn, '.') ? 0 : strlen(MAC_FTYPE));

                if(len_type)
                {
                        if(strlen(fn) + len_type < FILENAME_MAX)
                                strcat(fn, MAC_FTYPE);
                }

                MacroRunFile(fn);
        }
}

#endif

