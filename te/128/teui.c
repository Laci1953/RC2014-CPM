#define	SC108	/* un-define-it for MM */

/*	te_ui.c

	Text editor.

	User interface.

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
	22 Feb 2018 : Ask for confirmation only if changes were not saved. INTRO equals Y on confirmation.
	16 Jan 2019 : Modified Refresh() to show block selection. Added RefreshBlock().
	19 Jan 2019 : Added ShowFilename().
	23 Jan 2019 : Refactorized MenuHelp().
	30 Jan 2019 : Added putchrx().
	24 Dec 2019 : Modified some text messages. SysLineKey() is now SysLineCont(). Added support for numbered lines.
	26 Dec 2019 : Now K_INTRO is K_CR. Add SysLineWait(), SysLineBack().
	28 Feb 2020 : Minor changes.
	08 Mar 2020 : Support for CRT_LONG in menu, about and (ejem) help options.
	31 Dec 2020 : Use NUM_SEP instead of space to separate line numbers from text.
	04 Jan 2021 : Use configuration variables.
	22 Feb 2021 : Removed CRT_ROWS, CRT_COLS.
	04 Apr 2021 : Remove customized key names. Use key bindings from configuration.
	05 Apr 2021 : Adapt HELP to last changes.
	06 Apr 2021 : Use special screen characters from configuration instead of macros.
	08 Apr 2021 : Get adaptation (port) name from configuration.
	05 Jul 2021 : Support for OPT_Z80.
	25 Sep 2021 : Added SysLineEdit(). Fix URLs.
	   Nov 2021 : (Ladislau Szilagyi) Adapted for RC2014's SC108 128KB RAM memory module
*/

#include <te.h>
#include <tekeys.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <dynm128.h>

extern int help_items[];
extern char file_name[FILENAME_MAX];
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
extern char** lp_arr; 			/* Pointer of Text lines pointers array */
extern char* lp_arr_i_bank; 		/* Pointer of Text lines dynamic memory banks array */
extern int   lp_now; /* How many lines are in the array */
extern int   lp_cur; /* Current line */
extern int   lp_chg; /* 0 if no changes are made */
extern char ln_dat[]; /* Data buffer */
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
extern char* clp_arr[];		/* Pointer of Multi-line clipboard lines pointers array */
extern char* clp_arr_i_bank;	/* Pointer of Multi-line clipboard lines memory banks array */

#else

extern char *clp_line;  /* Just one line */

#endif

#if OPT_FIND

extern char find_str[FIND_MAX];

#endif

void CrtOut(int);
char* GetKeyWhat(int key);
int WriteFile(char* fn);
int ReadFile(char* fn);
char* CurrentFile(void);
int GetKey(void);
int ReadLine(char* buf, int width);
int GetFirstLine(void);
int GetLastLine(void);
int MenuExit(void);
void MenuAbout(void);
void MenuHelpCh(char ch);
void MenuHelp(void);
int MenuSaveAs(void);
int MenuSave(void);
int MenuOpen(void);
int MenuNew(void);
int Menu(void);
void MenuFreeMemory(void);

/* Read character from keyboard
   ----------------------------
*/
/* **************************** SEE #define
int getchr()
{
	return GetKey();
}
******************************* */

/* Print character on screen
   -------------------------
*/
/* **************************** SEE #define
putchr(ch)
int ch;
{
	CrtOut(ch);
}
******************************* */

/* Print character on screen X times
   ---------------------------------
*/
#if OPT_Z80
/* putchrx(ch, n) */
/* int ch, n;     */
#asm
psect text
global _CrtOut
global _putchrx
_putchrx:
        ld      hl,2
        add     hl,sp
        ld      c,(hl)  ;ch
        inc     hl
        inc     hl
        ld      b,(hl)  ;n
1:      push    bc
        call    _CrtOut
        pop     bc
        djnz    1b
        ret
#endasm
#else
putchrx(ch, n)
int ch, n;
{
	while(n--) 
		putchr(ch);
}
#endif

/* Print string on screen
   ----------------------
*/
#if OPT_Z80
/* putstr(s) */
/* char *s;  */
#asm
psect text
global _putstr
_putstr:
        ld      hl,2
        add     hl,sp
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        ex      de,hl
1:      ld      a,(hl)
        or      a
        ret     z
        inc     hl
        ld      c,a
        push    hl
        push    bc
        call    _CrtOut
        pop     bc
        pop     hl
        jr      1b
#endasm
#else
putstr(s)
char *s;
{
	while(*s)
		putchr(*s++);
}		
#endif		

/* Print string + '\n' on screen
   -----------------------------
*/
putln(s)
char *s;
{
	putstr(s); putchr('\n');
}

/* Print number on screen
   ----------------------
*/
putint(format, value)
char *format; int value;
{
	char r[7]; /* -12345 + ZERO */

	sprintf(r, format, value);
	putstr(r);
}

/* Print program layout
   --------------------
*/
Layout()
{
	int i, k, w;

	/* Clear screen */
	CrtClear();

	/* Header */
	putstr("te:");

	/* Information layout */
	CrtLocate(PS_ROW, PS_INF); putstr(PS_TXT);

	/* Max. # of lines */
	CrtLocate(PS_ROW, PS_LIN_MAX); putint("%04d", cf_mx_lines);

	/* # of columns */
	CrtLocate(PS_ROW, PS_COL_MAX); putint("%02d", 1 + ln_max);

	/* Ruler */
#if CRT_LONG
	CrtLocate(BOX_ROW - 1, cf_num);

	w = cf_cols - cf_num;

	for(i = k = 0; i < w; ++i)
	{
		if(k++)
		{
			putchr(cf_rul_chr);

			if(k == cf_tab_cols)
				k = 0;
		}
		else
			putchr(cf_rul_tab);
	}

	/* System line separator */
	CrtLocate(cf_rows - 2, 0);

	putchrx(cf_horz_chr, cf_cols);
#endif
}

/* Print filename
   --------------
*/
ShowFilename()
{
	char *s;

	CrtLocate(PS_ROW, PS_FNAME);
	putstr((s = CurrentFile()));
	putchrx(' ', FILENAME_MAX - strlen(s) - 1);
}

/* Print message on system line
   ----------------------------
   Message can be NULL == blank line / clear system line.
*/
SysLine(s)
char *s;
{
	CrtClearLine(cf_rows - 1);

	if(s)
		putstr(s);

	/* Set flag for Loop() */
	sysln = 1;
}

/* Print message when editing
   --------------------------
*/
SysLineEdit()
{
	SysLine(GetKeyName(K_ESC));	putstr(" = menu");
}

/* Print message on system line and wait
   for CR and / or ESC key press
   -------------------------------------
   Message can be NULL. Returns NZ if CR, else Z.
*/
int SysLineWait(s, cr, esc)
char *s, *cr, *esc;
{
	int ch;

	SysLine(s);

	if(s)
		putstr(" (");

	if(cr)
	{
		putstr(GetKeyName(K_CR)); putstr(" = "); putstr(cr); putstr(", ");
	}

	if(esc)
	{
		putstr(GetKeyName(K_ESC)); putstr(" = "); putstr(esc);
	}

	if(s)
		putchr(')');

	putstr(": ");

	for(;;)
	{
		ch = getchr();

		if(cr && ch == K_CR)
			break;

		if(esc && ch == K_ESC)
			break;
	}

	SysLine(NULL);

	return (ch == K_CR);
}


/* Print message on system line and wait
   for ESC key press to CONTINUE
   -------------------------------------
   Message can be NULL.
*/
SysLineCont(s)
char *s;
{
	SysLineWait(s, NULL, "continue");
}

/* Print message on system line and wait
   for ESC key press to COMEBACK
   -------------------------------------
   Message can be NULL.
*/
SysLineBack(s)
char *s;
{
	SysLineWait(s, NULL, "back");
}

/* Print message on system line and wait
   for CONFIRMATION
   -------------------------------------
   Message can be NULL. Returns NZ if YES, else Z.
*/
SysLineConf(s)
char *s;
{
	return SysLineWait(s, "continue", "cancel");
}

/* Ask for a string
   ----------------
   Return NZ if entered, else Z.
*/
SysLineStr(what, buf, maxlen)
char *what, *buf; int maxlen;
{
	int ch;

	SysLine(what);
	putstr(" (");
	putstr(GetKeyName(K_ESC));
	putstr(" = cancel): ");

	ch = ReadLine(buf, maxlen);

	SysLine(NULL);

	if(ch == K_CR && *buf)
			return 1;

	return 0;
}

/* Ask for a filename
   ------------------
   Return NZ if entered, else Z.
*/
SysLineFile(fn)
char *fn;
{
	return SysLineStr("Filename", fn, FILENAME_MAX - 1);
}

/* Ask for confirmation on changes not saved
   -----------------------------------------
   Returns NZ if YES, else Z.
*/
SysLineChanges()
{
	return SysLineConf("Changes will be lost!");
}

/* Read simple line
   ----------------
   Returns last character entered: INTRO or ESC.
*/
int ReadLine(char* buf, int width)
{
	int len;
	int ch;

	putstr(buf); len=strlen(buf);

	while(1)
	{
		switch((ch = getchr()))
		{
			case K_LDEL :
				if(len)
				{
					putchr('\b'); putchr(' '); putchr('\b');

					--len;
				}
				break;
			case K_CR :
			case K_ESC :
				buf[len] = 0;
				return ch;
			default :
				if(len < width && ch >= ' ')
					putchr(buf[len++] = ch);
				break;
		}
	}
}

/* Return name of current file
   ---------------------------
*/
char* CurrentFile()
{
	return (file_name[0] ? file_name : "-");
}

/* Clear the editor box
   --------------------
*/
ClearBox()
{
	int i;

	for(i = 0; i < box_rows; ++i)
		CrtClearLine(BOX_ROW + i);
}

/* Print centered text on the screen
   ---------------------------------
*/
CenterText(row, txt)
int row; char *txt;
{
	CrtLocate(row, (cf_cols - strlen(txt)) / 2);

	putstr(txt);
}

#if OPT_BLOCK

/* Refresh block selection in editor box
   -------------------------------------
   Set 'sel' to NZ for reverse print, else Z for normal print.
*/
RefreshBlock(row, sel)
int row, sel;
{
	int i, line;

	line = GetFirstLine() + row;

	for(i = row; i < box_rows; ++i) 
	{
		if(line >= blk_start) 
		{
			if(line <= blk_end) 
			{
#if CRT_CAN_REV
				CrtLocate(BOX_ROW + i, cf_num);
				CrtClearEol();

				if(sel) 
					CrtReverse(1);
				
				if (lp_arr_i_bank[line]==0)
					strcpy(tmpbuf, lp_arr[line]);
				else
					GetString(tmpbuf, lp_arr[line]);

				putstr(tmpbuf);
				putchr(' ');

				if(sel)
					CrtReverse(0);
#else
				CrtLocate(BOX_ROW + i, cf_cols - 1); putchr(sel ? BLOCK_CHR : ' ');
#endif
			}
			else 
				break;
		}
		++line;
	}
}

#endif

/* Refresh editor box
   ------------------
   Starting from box row 'row', line 'line'.
*/
Refresh(row, line)
int row, line;
{
	int i;
	char *format;

#if OPT_BLOCK
	int blk, sel;

	blk = (blk_count && blk_start <= GetLastLine() && blk_end >= GetFirstLine());
	sel = 0;
#endif
	if(cf_num) 
	{
		format = "%?d";
		format[1] = '0' + cf_num - 1;
	}

	for(i = row; i < box_rows; ++i)
	{
		CrtClearLine(BOX_ROW + i);

		if(line < lp_now) 
		{
			if(cf_num) 
			{
				putint(format, line + 1);
				putchr(cf_lnum_chr);
			}
#if OPT_BLOCK
			if(blk) 
			{
				if(line >= blk_start) 
				{
					if(line <= blk_end) 
					{
#if CRT_CAN_REV
						CrtReverse((sel = 1));
#else
						sel = 1;
#endif
					}
				}
			}
#endif
			if (lp_arr_i_bank[line]==0)
				strcpy(tmpbuf, lp_arr[line]);
			else
				GetString(tmpbuf, lp_arr[line]);

			putstr(tmpbuf);
			line++;
#if OPT_BLOCK
			if(sel) 
			{
#if CRT_CAN_REV
				putchr(' ');

				CrtReverse((sel = 0));
#else
				sel = 0;

				CrtLocate(BOX_ROW + i, cf_cols - 1); putchr(BLOCK_CHR);
#endif
			}

#endif
		}
	}
}

/* Refresh editor box
   ------------------
*/
RefreshAll()
{
	Refresh(0, lp_cur - box_shr);
}

/* Show the menu
   -------------
   Return NZ to quit program.
*/
int Menu(void)
{
	int run, row, stay, menu, ask;
	int tmp;

	/* Setup some things */
	run = stay = menu = ask = 1;

	/* Loop */
	while(run)
	{
		/* Show the menu */
		if(menu)
		{
			row = BOX_ROW + 1;

			ClearBox();

			CenterText(row++, "OPTIONS");
			row++;
#if CRT_LONG
			CenterText(row++, "New");
			CenterText(row++, "Open");
			CenterText(row++, "Save");
			CenterText(row++, "save As");
			CenterText(row++, "Help");
			CenterText(row++, "aBout te");
			CenterText(row++, "available Memory");
			CenterText(row  , "eXit te");
#else
			CenterText(row++, "New   Open      Save     Save As");
			CenterText(row++, "Help  aBout te  eXit te         ");
#endif

			menu = 0;
		}

		/* Ask for option */
		if(ask)
		{
			SysLine("Option (");
			putstr(GetKeyName(K_ESC));
			putstr(" = back): ");
		}
		else
			ask = 1;

		/* Do it */
		tmp=getchr();

	      	if (isalpha(tmp))
      		{
        	  if (islower(tmp))
          	    tmp=toupper(tmp);
      		}

		switch(tmp)
		{
			case 'N'   : run = MenuNew(); break;
			case 'O'   : run = MenuOpen(); break;
			case 'S'   : run = MenuSave(); break;
			case 'A'   : run = MenuSaveAs(); break;
			case 'B'   : MenuAbout(); menu = 1; break;
			case 'H'   : MenuHelp(); menu = 1; break;
			case 'X'   : run = stay = MenuExit(); break;
			case 'M'   : MenuFreeMemory(); menu = 1; break;
			case K_ESC : run = 0; break;
			default    : ask = 0; break;
		}
	}

	/* Clear editor box */
	ClearBox();

	SysLine(NULL);

	/* Return NZ to quit the program */
	return !stay;
}

/* Menu option: New
   ----------------
   Return Z to quit the menu.
*/
int MenuNew(void)
{
	if(lp_chg)
	{
		if(!SysLineChanges())
			return 1;
	}

	NewFile();

	return 0;
}

/* Menu option: Open
   -----------------
   Return Z to quit the menu.
*/
int MenuOpen(void)
{
	char fn[FILENAME_MAX];

	if(lp_chg)
	{
		if(!SysLineChanges())
			return 1;
	}

	fn[0] = 0;

	if(SysLineFile(fn))
	{
		if(ReadFile(fn))
			NewFile();
		else
			strcpy(file_name, fn);

		return 0;
	}

	return 1;
}

/* Menu option: Save
   -----------------
   Return Z to quit the menu.
*/
int MenuSave(void)
{
	if(!file_name[0])
		return MenuSaveAs();

	WriteFile(file_name);

	return 1;
}

/* Menu option: Save as
   --------------------
   Return Z to quit the menu.
*/
int MenuSaveAs(void)
{
	char fn[FILENAME_MAX];

	strcpy(fn, file_name);

	if(SysLineFile(fn))
	{
		if(!WriteFile(fn))
			strcpy(file_name, fn);

		return 0;
	}

	return 1;
}

/* Menu option: available Memory
   -----------------
*/
void MenuFreeMemory(void)
{
	int freeMem;
	int row;

	ClearBox();
	row = BOX_ROW + 1;
	freeMem = GetTotalFree();
	sprintf(tmpbuf, "%u KB available memory", freeMem);
	CenterText(row, tmpbuf);
	SysLineBack(NULL);
}

/* Menu option: Help
   -----------------
*/
void MenuHelp(void)
{
	int i, k;
	char *s;

	ClearBox();

	CrtLocate(BOX_ROW + 1, 0);

	putln("HELP:\n");

#if CRT_LONG

	for(i = 0; help_items[i] != -1; ++i) 
	{
		/* 123456789012345 (15 characters) */
		/* BlkEnd     ^B^E */

		if((k = help_items[i])) 
		{
			if(*(s = GetKeyWhat(k)) == '?')
				k = 0;
		}

		if(k) 
		{
			putstr(s); putchrx(' ', 11 - strlen(s));

			k -= 1000;

			MenuHelpCh(cf_keys[k]);
			MenuHelpCh(cf_keys_ex[k]);
		}
		else 
			putchrx(' ', 15);

		if((i + 1) % 3)
		{ 
			putchr(' '); putchr(cf_vert_chr); putchr(' ');
		}
		else 
			putchr('\n');
	}

#else

	putstr("Sorry, no help is available.");

#endif

	SysLineBack(NULL);
}

void MenuHelpCh(char ch)
{
	if(ch) {
		if(ch < 32 || ch == 0x7F) 
		{
			putchr('^'); putchr(ch != 0x7F ? '@' + ch : '?');
		}
		else 
		{
			putchr(ch); putchr(' ');
		}
	}
	else 
	{
		putchr(' '); putchr(' ');
	}
}

/* Menu option: About
   ------------------
*/
void MenuAbout(void)
{
	int row;

#if CRT_LONG
	row = BOX_ROW + 1;

	ClearBox();

	CenterText(row++, "te - Text Editor");
	CenterText(row++, VERSION);
	CenterText(row++, "Configured for");
	CenterText(row++, cf_name);
	CenterText(row++, COPYRIGHT);
	CenterText(row++, "http://www.floppysoftware.es");
	CenterText(row++, "https://cpm-connections.blogspot.com");
	CenterText(row++  , "floppysoftware@gmail.com");
#ifdef SC108
	CenterText(row++, "Adapted for RC2014's SC108 128KB RAM module");	
#else
	CenterText(row++, "Adapted for RC2014's 32KB ROM + 128KB RAM memory module");	
#endif
	CenterText(row++, "by Ladislau Szilagyi");
	CenterText(row, "http://www.euroqst.ro");
#else
	row = BOX_ROW;

	ClearBox();

	CenterText(row++, "te - Text Editor");
	CenterText(row++, VERSION);
	CenterText(row++, "Configured for");
	CenterText(row++, cf_name);
	CenterText(row++, COPYRIGHT);
	CenterText(row++, "www.floppysoftware.es");
#ifdef SC108
	CenterText(row++, "Adapted for RC2014's SC108 128KB RAM module");	
#else
	CenterText(row++, "Adapted for RC2014's 32KB ROM + 128KB RAM memory module");	
#endif
	CenterText(row++, "by Ladislau Szilagyi");
	CenterText(row, "http://www.euroqst.ro");
#endif

	SysLineBack(NULL);
}

/* Menu option: Quit program
   -------------------------
*/
int MenuExit(void)
{
	if(lp_chg)
		return !SysLineChanges();

	/* Quit program */
	return 0;
}
