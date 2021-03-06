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
	   Oct 2021 : (Ladislau Szilagyi) Adapted for RC2014's 512KB RAM memory module
*/

#include <te.h>
#include <string.h>
#include <dynm512.h>

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
extern char  lp_arr_bank; 		/* Text lines pointers array's dynamic memory bank */
extern char* lp_arr_i_bank; 		/* Pointer of Text lines dynamic memory banks array */
extern char  lp_arr_i_bank_bank;	/* Text lines dynamic memory banks array's dynamic memory bank */
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
extern char* clp_arr_i_bank;	/* Pointer of Multi-line clipboard lines memory banks array */

#else

extern char *clp_line;  /* Just one line */
extern char clp_line_bank;

#endif

#if OPT_FIND

extern char find_str[FIND_MAX];

#endif

extern int tmp;
extern char* ptmp;
extern char  tmpbuf[];
extern char  bank;

void* AllocMem(short bytes, char* bank);
void ErrLineTooMany(void);
void	Xstrcpy(char* dest, char*src);
void	XGetString(char* dest, char*src);
int	Xstrlen(char* s);
int	XStringLen(char* s);
char*	DropBlanks(char* pbuf);
char*	ExpandTabs(char*);

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
	char bank1;

	if(insert && lp_now >= cf_mx_lines) {
		ErrLineTooMany();
		
		return 0;
	}

	if(!text) 
		text = "";

	/* if((p = AllocMem(strlen(text) + 1))) */
	tmp = strlen(text) + 1;
	strcpy(tmpbuf, text);		/* save text to tmpbuf */

	if ((p = (char*)AllocMem(tmp, &bank)))
	{
		if(insert) 
		{
			for(i = lp_now; i > line; --i)
			{
				setbank(lp_arr_bank);
				lp_arr[i] = lp_arr[i - 1];

				setbank(lp_arr_i_bank_bank);				
				lp_arr_i_bank[i] = lp_arr_i_bank[i - 1];
			}

			++lp_now;
		}
		else 
		{
			setbank(lp_arr_bank);
			ptmp = lp_arr[line];

			if(ptmp)
			{
				setbank(lp_arr_i_bank_bank);
				bank1 = lp_arr_i_bank[line];
				free512(ptmp, bank1); /*free(lp_arr[line]);*/
			}
		}

		/* lp_arr[line] = strcpy(p, text); */
		setbank(bank);
		strcpy(p, DropBlanks(tmpbuf));	/* text */

		setbank(lp_arr_bank);
		lp_arr[line] = p;

		setbank(lp_arr_i_bank_bank);				
		lp_arr_i_bank[line] = bank;

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
	/* no need to setbank because of the NULL */
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
	char *p, *p2;

	/* if((p = AllocMem(pos + 1))) */
	if ((p = (char*)AllocMem(pos+1, &bank))) 
	{
		setbank(lp_arr_bank);
		ptmp = lp_arr[line];

		setbank(lp_arr_i_bank_bank);				
		setbank(lp_arr_i_bank[line]);

		strcpy(tmpbuf, ExpandTabs(ptmp) + pos);

		/* if(AppendLine(line, lp_arr[line] + pos)) */
		if(AppendLine(line, tmpbuf)) 
		{
			/* We don't have strncpy() nor arrays of pointers yet, then... */
			setbank(lp_arr_bank);
			p2 = lp_arr[line];

			setbank(lp_arr_i_bank_bank);
			setbank(lp_arr_i_bank[line]);

			Xstrcpy(tmpbuf, p2);

			p2 = tmpbuf;
			p2[pos] = '\0';

			/* strcpy(p, p2); */
			setbank(lp_arr_i_bank_bank);
			if (lp_arr_i_bank[line] == bank)	/* if same banks */
			{
				setbank(bank);
				strcpy(p, DropBlanks(p2));			/* ...then do direct copy */
			}
			else
			{
				setbank(lp_arr_i_bank[line]);
				strcpy(tmpbuf, p2);		/* else copy p2 to tmpbuf... */
				setbank(bank);
				strcpy(p, DropBlanks(tmpbuf));		/* ... then copy tmpbuf to p */
			}

			/* free(p2); */
			setbank(lp_arr_i_bank_bank);				
			free512((void*)p2, lp_arr_i_bank[line]);

			setbank(lp_arr_bank);
			lp_arr[line] = p;

			setbank(lp_arr_i_bank_bank);				
			lp_arr_i_bank[line] = bank;

			return 1;
		}

		/* free(p); */
		free512((void*)p, bank);
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

	/* free(lp_arr[line]); */
	setbank(lp_arr_bank);
	ptmp = lp_arr[line];

	setbank(lp_arr_i_bank_bank);				
	bank = lp_arr_i_bank[line];
	
	free512(ptmp, bank);

	--lp_now;

	for(i = line; i < lp_now; ++i) 
	{
		/* lp_arr[i] = lp_arr[i + 1]; */
		setbank(lp_arr_bank);
		lp_arr[i] = lp_arr[i + 1];

		setbank(lp_arr_i_bank_bank);
		lp_arr_i_bank[i] = lp_arr_i_bank[i+1];
	}

	setbank(lp_arr_bank);
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
	char oldbank;

	setbank(lp_arr_bank);
	p1 = lp_arr[line];
	p2 = lp_arr[line + 1];

	setbank(lp_arr_i_bank_bank);
	setbank(lp_arr_i_bank[line]);
	s1 = strlen(p1);

	setbank(lp_arr_i_bank_bank);
	setbank(lp_arr_i_bank[line+1]);
	s2 = strlen(p2);

	if(s1 + s2 <= ln_max) 
	{
		/* if((p = AllocMem(s1 + s2 + 1))) */
		if ((p = (char*)AllocMem(s1+s2+1, &bank)))
		{
			/* lp_arr[line] = strcat(strcpy(p, p1), p2); */

			/* first strcpy(p,p1); */
			setbank(lp_arr_i_bank_bank);

			if (lp_arr_i_bank[line] == bank)	/* if same banks */
			{
				setbank(bank);
				strcpy(p, p1);			/* then do direct copy */
			}
			else
			{
				setbank(lp_arr_i_bank[line]);
				strcpy(tmpbuf, p1);		/* else copy first p1 to tmpbuf... */
				setbank(bank);
				strcpy(p, tmpbuf);		/* ...then copy tmpbuf to p */
			}
			
			/* then strcat(p,p2); */
			setbank(lp_arr_i_bank_bank);

			if (lp_arr_i_bank[line+1] == bank)	/* if same banks */
			{
				setbank(bank);
				strcat(p, p2);			/* then do direct strcat */
			}
			else
			{
				setbank(lp_arr_i_bank[line+1]);
				strcpy(tmpbuf, p2);		/* else copy first p2 to tmpbuf... */
				setbank(bank);
				strcat(p, tmpbuf);		/* ...then strcat(p, tmpbuf) */
			}

			setbank(lp_arr_bank);
			lp_arr[line] = p;

			setbank(lp_arr_i_bank_bank);
			oldbank = lp_arr_i_bank[line];
			lp_arr_i_bank[line] = bank;	

			/* free(p1); */
			free512((void*)p1, oldbank);

			DeleteLine(line + 1);

			return 1;
		}
	}

	return 0;
}
