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
*/

#include <te.h>
#include <string.h>
#include <dynm128.h>

extern char** lp_arr;			/* Pointer of Text lines pointers array */
extern char*  lp_arr_i_bank; 		/* Pointer of Text lines dynamic memory banks array */

extern void* clp_arr[];		/* Pointer of Multi-line clipboard lines pointers array */
extern char  clp_arr_i_bank[];	/* Pointer of Multi-line clipboard lines memory banks array */

extern char* ptmp;

/* Allocate memory
   ---------------
   Shows error message on failure.
*/
void* AllocMem(short bytes, char* bank)
{
	char *p;

	if(!(p = Alloc128(bytes, bank))) 
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

	for(i = 0; i < count; ++i) 
	{
		if (lp_arr[i])
		{
			Free128(lp_arr[i], lp_arr_i_bank[i]);
			lp_arr[i] = NULL;
		}
	}

	if(flag) 
		free(lp_arr);

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
			Free128(clp_arr[i], clp_arr_i_bank[i]);
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
