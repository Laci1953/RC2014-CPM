/*	te_lines.c

	Text editor.

	Operations with text lines.

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

	06 Jan 2019 : Extracted from te.c.
	07 Jan 2019 : Added InsertLine(), AppendLine(), SplitLine(), DeleteLine(), JoinLines().
	08 Jan 2019 : Added SetLine(), ModifyLine(), ClearLine(). Modified InsertLine().
	04 Jan 2021 : Use configuration variables.
	   Nov 2021 : (Ladislau Szilagyi) Adapted for RC2014's SC108 128KB RAM memory module
	   Mar 2022 : (Ladislau Szilagyi) Adapted for RC2014's 512KB RAM memory module
	   Mar 2023 : (Ladislau Szilagyi) Review & created single source for 128/512
*/

#include <te.h>
#include <string.h>
#include <dynm.h>

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
extern char* lp_arr[]; 			/* Text lines pointers array */
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

#if OPT_BLOCK

extern int blk_start;   /* Start line # */
extern int blk_end;     /* End line # */
extern int blk_count;   /* # of lines */
extern char* clp_arr[];		/* Pointer of Multi-line clipboard lines pointers array */

#else

extern char *clp_line;  /* Just one line */

#endif

#if OPT_FIND

extern char find_str[FIND_MAX];

#endif

extern int tmp;
extern char* ptmp;
extern char  tmpbuf[];
extern char  b;

void* AllocMem(short bytes);
void ErrLineTooMany(void);
void	XGetString(char* dest, char* src);
void	XPutString(char* source, char* dest);
int	XStringLen(char* s);
char*	DropBlanks(char* pbuf);
char*	ExpandTabs(char*);

char  tmpbuf1[201];

/* Return line # of first line printed on the editor box
   -----------------------------------------------------
*/
int GetFirstLine(void)
{
	return lp_cur - box_shr;
}

/* Return line # of last line printed on the editor box
   ----------------------------------------------------
*/
int GetLastLine(void)
{
	int last;

	last = GetFirstLine() + box_rows - 1;

	return last >= lp_now - 1 ? lp_now - 1 : last; /* min(lp_now - 1, last) */
}

/* Set text in line #
   ------------------
   Set 'text' to NULL for empty lines. Return NZ on success, else Z.

*/
int SetLine(line, text, insert)
int line; char *text; int insert;
{
	char *p;
	int i;

	if(insert && lp_now >= cf_mx_lines) {
		ErrLineTooMany();
		
		return 0;
	}

	if(!text) 
		text = "";

	tmp = strlen(text) + 1;

	if ((p = (char*)AllocMem(tmp)))
	{
		if(insert) 
		{
			for(i = lp_now; i > line; --i)
				lp_arr[i] = lp_arr[i - 1];

			++lp_now;
		}
		else 
		{
			if(lp_arr[line])
				Free(lp_arr[line]); 
		}

		XPutString(text, p);

		lp_arr[line] = p;

		return 1;
	}

	return 0;
}

/* Modify text in line #
   ---------------------
	Set 'text' to NULL for empty lines. Return NZ on success, else Z.
*/

int ModifyLine(line, text)
int line; char *text;
{
	return SetLine(line, text, 0);
}

/* Clear text in line #
   --------------------
   Return NZ on success, else Z.
*/

int ClearLine(line)
int line;
{
	return ModifyLine(line, NULL);
}

/* Insert new line before line #
   -----------------------------
   Set 'text' to NULL for empty lines. Return NZ on success, else Z.
*/
int InsertLine(line, text)
int line; char *text;
{
	return SetLine(line, text, 1);
}

/* Append new line after line #
   -----------------------------
   Set 'text' to NULL for empty lines. Return NZ on success, else Z.
*/
int AppendLine(line, text)
int line; char *text;
{
	return InsertLine(line + 1, text);
}

/* Split line # into two lines
   ---------------------------
   Return NZ on success, else Z.
*/
int SplitLine(line, pos)
int line, pos;
{
	char *p;

	if ((p = (char*)AllocMem(pos+1))) 
	{
		XGetString(tmpbuf1, lp_arr[line]);
		strcpy(tmpbuf, tmpbuf1 + pos);  

		if(AppendLine(line, DropBlanks(tmpbuf))) 
		{
			/* We don't have strncpy() nor arrays of pointers yet, then... */
			XGetString(tmpbuf, lp_arr[line]);
			tmpbuf[pos] = '\0';
			XPutString(tmpbuf, p);
			Free(lp_arr[line]);
			lp_arr[line] = p;
			return 1;
		}

		Free((void*)p);
	}

	return 0;
}

/* Delete line #
   -------------
   Return NZ on success, else Z.
*/
int DeleteLine(line)
int line;
{
	int i;

	Free(lp_arr[line]);

	--lp_now;

	for(i = line; i < lp_now; ++i) 
		lp_arr[i] = lp_arr[i + 1];

	lp_arr[lp_now] = NULL;

	return 1;
}

/* Join two consecutive lines (line & line+1)
   --------------------------
   Return NZ on success, else Z.
*/
int JoinLines(line)
int line;
{
	char *p, *p1, *p2;
	int s1, s2;

	p1 = lp_arr[line];
	p2 = lp_arr[line + 1];

	s1 = StringLen(p1);
	s2 = StringLen(p2);

	if(s1 + s2 <= LINE_SIZE_MAX) 
	{
		if ((p = (char*)AllocMem(s1+s2+1)))
		{
			/* first strcpy(tmpbuf,p1); */
			GetString(tmpbuf, p1);
			
			/* then strcat(tmpbuf,p2); */
			ptmp = tmpbuf + strlen(tmpbuf);
			GetString(ptmp, p2);

			/* now, tmpbuf contains strcat(strcpy(p, p1), p2) */
			/* so, we must copy tmpbuf to p */
			PutString(tmpbuf, p);

			lp_arr[line] = p;

			Free((void*)p1);

			DeleteLine(line + 1);

			return 1;
		}
	}

	return 0;
}
