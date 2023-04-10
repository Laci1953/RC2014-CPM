/*	te_misc.c

	Miscelaneous tools and functions.

	Operations with text lines.

	Copyright (c) 2015-2019 Miguel Garcia / FloppySoftware

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

	13 Jan 2019 : Added AllocMem().
	19 Jan 2019 : Added FreeArray().
	30 Jan 2019 : Added MatchStr().
	   Nov 2021 : (Ladislau Szilagyi) Adapted for RC2014's SC108 128KB RAM memory module
	   Mar 2022 : (Ladislau Szilagyi) Adapted for RC2014's 512KB RAM memory module
	   Mar 2023 : (Ladislau Szilagyi) Review & created single source for 128/512
*/

#include <te.h>
#include <stdio.h>
#include <string.h>
#include <dynm.h>

char*	ExpandTabs(char*);
char*	DropBlanks(char*);
void	ErrLine(char*);

extern char** lp_arr;			/* Pointer of Text lines pointers array */
extern char b_lp_arr;
extern char*  lp_arr_i_b; 		/* Pointer of Text lines dynamic memory banks array */
extern char b_lp_arr_i_b;

#if OPT_BLOCK
extern void* clp_arr[];		/* Pointer of Multi-line clipboard lines pointers array */
extern char  clp_arr_i_b[];	/* Pointer of Multi-line clipboard lines memory banks array */
#endif

extern int ln_max;

char localbuf[200];

void Layout(void);
void LoopTop(void);

void	XGetString(char* dest, char* src, char src_flag)
{
	if (src_flag == (unsigned char)LOW64)
		strcpy(dest, ExpandTabs(src));
	else
	{
		GetString(localbuf, src, src_flag);
		strcpy(dest, ExpandTabs(localbuf));
	}
/*-----------------------------------	
	if (strlen(dest) > ln_max)
	{
		dest[ln_max]=0;
		ErrLine("Line was truncated!");
	}
------------------------------------*/
}

void	XPutString(char* source, char* dest, char dest_flag)
{
	PutString(DropBlanks(source), dest, dest_flag);
}

int	XStringLen(char* s, char s_flag)
{
	if (s_flag == (unsigned char)LOW64)
		return(strlen(ExpandTabs(s)));
	else
	{
		GetString(localbuf, s, s_flag);
		return(strlen(ExpandTabs(localbuf)));
	}
}

/* Allocate memory
   ---------------
   Shows error message on failure.
*/
void* AllocMem(short bytes, char* bank)
{
	char *p;

	if(!(p = Alloc(bytes, bank))) 
		ErrLineMem();

	return p;
}

/* Free array
   ----------
   Deallocate array memory.
*/
char* FreeText(int count, int flag)
{
	int i;
	char* p;

	for(i = 0; i < count; ++i) 
	{
		if (p = GetWord(lp_arr, i, b_lp_arr))
		{
			Free(p, GetByte(lp_arr_i_b, i, b_lp_arr_i_b));
			PutWord(lp_arr, i, NULL, b_lp_arr);
		}
	}

	if(flag) 
		Free(lp_arr, b_lp_arr);

	return NULL;
}

#if OPT_BLOCK
char* FreeClipboard(void)
{
	int i;

	for (i=0; i < CLP_LINES_MAX; i++)
	{
		if (clp_arr[i])
		{
			Free(clp_arr[i], clp_arr_i_b[i]);
			clp_arr[i] = NULL;
		}
	}
	return 	NULL;
}
#endif

#if OPT_MACRO

/* Test if two strings are equal
   -----------------------------
*/
int MatchStr(s1, s2)
char *s1, *s2;
{
	return !strcmp(s1, s2);
}

#endif
