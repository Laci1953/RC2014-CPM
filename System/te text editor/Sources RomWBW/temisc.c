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

extern char* lp_arr[];			/* Text lines pointers array */

#if OPT_BLOCK
extern void* clp_arr[];		/* Pointer of Multi-line clipboard lines pointers array */
#endif

extern int ln_max;

char localbuf[200];

void Layout(void);
void LoopTop(void);

void	XGetString(char* dest, char* src)
{
	GetString(localbuf, src);
	strcpy(dest, ExpandTabs(localbuf));
}

void	XPutString(char* source, char* dest)
{
	PutString(DropBlanks(source), dest);
}

int	XStringLen(char* s, char s_flag)
{
	GetString(localbuf, s);
	return(strlen(ExpandTabs(localbuf)));
}

/* Allocate memory
   ---------------
   Shows error message on failure.
*/
void* AllocMem(short bytes, char* bank)
{
	char *p;

	if(!(p = Alloc(bytes))) 
		ErrLineMem();

	return p;
}

/* Free array
   ----------
   Deallocate array memory.
*/
char* FreeText(int count)
{
	int i;
	char* p;

	for(i = 0; i < count; ++i) 
	{
		if (p = lp_arr[i])
		{
			Free(p);
			lp_arr[i] = NULL;
		}
	}

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
			Free(clp_arr[i]);
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
