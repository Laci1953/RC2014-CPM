/*	te_keys.c

	Text editor.

	Key bindings.

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

	22 Jan 2019 : Start.
	23 Jan 2019 : Added GetKeyWhat().
	29 Jan 2019 : Added K_CLRCLP.
	26 Dec 2019 : Now K_INTRO is K_CR. Add GetKeyName().
	04 Apr 2021 : Remove customized key names. Use key bindings from configuration.
	11 May 2021 : Update keys purposes.
	10 Jul 2021 : Remove SetKey().
	   Oct 2021 : (Ladislau Szilagyi) Adapted for RC2014's 512KB RAM memory module
	   Mar 2022 : (Ladislau Szilagyi) Adapted for RC2014's 512KB RAM memory module
	   Mar 2023 : (Ladislau Szilagyi) Review & created single source for 128/512
*/

#include <te.h>
#include <tekeys.h>
#include <ctype.h>

int CrtIn(void);
extern unsigned char cf_keys[];
extern unsigned char cf_keys_ex[];
extern char cf_cr_name[];
extern char cf_esc_name[];

/* Return key name
   ---------------
*/
char* GetKeyName(key)
int key;
{
	switch(key)
	{
		case K_CR:  return cf_cr_name;
		case K_ESC: return cf_esc_name;
	}
	
	return "?";
}

/* Return key purpose
   ------------------
*/
char* GetKeyWhat(int key)
{
	/* Max. length of 8 chars., see MenuHelp() */
	 
	switch(key)
	{
		case K_UP:      return "Up";
		case K_DOWN:    return "Down";
		case K_LEFT:    return "Left";
		case K_RIGHT:   return "Right";
		case K_BEGIN:   return "Begin";
		case K_END:     return "End";
		case K_TOP:     return "Top";
		case K_BOTTOM:  return "Bottom";
		case K_PGUP:    return "PgUp";
		case K_PGDOWN:  return "PgDown";
		case K_TAB:     return "Indent";
		case K_CR:      return "NewLine";
		case K_ESC:     return "Escape";
		case K_RDEL:    return "DelRight";
		case K_LDEL:    return "DelLeft";
		case K_CUT:     return "Cut";
		case K_COPY:    return "Copy";
		case K_PASTE:   return "Paste";
		case K_DELETE:  return "Delete";
		case K_CLRCLP:  return "ClearClip";
#if OPT_FIND
		case K_FIND:    return "Find";
		case K_NEXT:    return "FindNext";
#endif
#if OPT_GOTO
		case K_GOTO:    return "GoLine";
#endif
#if OPT_LWORD
		case K_LWORD:   return "WordLeft";
#endif
#if OPT_RWORD
		case K_RWORD:   return "WordRight";
#endif
#if OPT_BLOCK
		case K_BLK_START:  return "BlockStart";
		case K_BLK_END:    return "BlockEnd";
		case K_BLK_UNSET:  return "BlockUnset";
#endif
#if OPT_MACRO
		case K_MACRO:   return "Macro";
#endif
	}

	return "?";
}

/* Return key from keyboard, according to key bindings
   ---------------------------------------------------
*/

/*
ARROW L 	ESC [ D
ARROW R 	ESC [ C
ARROW U 	ESC [ A
ARROW D 	ESC [ B
PG UP 		ESC [ 5 ~
PG DOWN		ESC [ 6 ~
HOME		ESC [ 1 ~
END		ESC [ 4 ~
CTRL Z		1AH
*/

#define ESCAPE	0x1B
#define CTRL_Z	0x1A
#define CR	0x0D
#define DELETE	0x7F
#define CTRL_G	0x07

char	ESCseq = 0;
int	last = 0;

int GetKey(void)
{
        char c, x, tmp;
        int i,k;

	if (last == K_LDEL)
	{
		last = 0;
		return K_RDEL;
	}
back:
       	c = CrtIn();

	if (c == DELETE)
		c = CTRL_G;		//^G

	last = c;

	if (c == ESCAPE)
	{
		ESCseq = 1;

		c = CrtIn();
try2:
		if (c == '[')
		{
			c = CrtIn();
try:
			switch(c)
			{
				case 'A': ESCseq = 0; return K_UP;
				case 'B': ESCseq = 0; return K_DOWN;
				case 'C': ESCseq = 0; return K_RIGHT;
				case 'D': ESCseq = 0; return K_LEFT;
				case '5':
					c = CrtIn();
					if (c == '~')
					{
						ESCseq = 0;
						return K_PGUP;	//PgUp
					}
					else goto back;
				case '6':
					c = CrtIn();
					if (c == '~')
					{
						ESCseq = 0;
						return K_PGDOWN; //PgDn
					}
					else goto back;
				case '1':
					c = CrtIn();
					if (c == '~')
					{
						ESCseq = 0;
						return K_TOP;	//HOME (begin doc)
					}
					else goto back;
				case '4':
					c = CrtIn();
					if (c == '~')
					{
						ESCseq = 0;
						return K_BOTTOM; //END (end doc)
					}
					else goto back;

				default:  ESCseq = 0; goto back;
			}
		}
		else
			goto try;	//in case '[' was missed, try to identify next char after '['
	}
	else if (ESCseq)
		goto try2;		//in case char after '[' was missed, try to identify next '['

        if(c > 31 && c != 127)
                return c;
	
	if (c == CTRL_Z)
		return K_ESC;

        for(i = 0; i < KEYS_MAX; ++i)
        {
                if(cf_keys[i])
                {
                        if(c == cf_keys[i])
                        {
                                if(cf_keys_ex[i])
                                {
       					tmp = CrtIn();

                                        if (isalpha(tmp))
                                        {
                                          if (islower(tmp))
                                                tmp=toupper(tmp);
                                        }

                                        x = tmp;

                                        /* TODO: try to optimize the following */

                                        for(k = i; k < KEYS_MAX; ++k)
                                        {
                                                if(c == cf_keys[k])
                                                {
                                                        if(x == cf_keys_ex[k])
                                                        {
								last = k + 1000;
                                                                return last;
                                                        }
                                                }
                                        }
                                        break;
                                }
                                else
                                {
					last = i + 1000;

					if (last == K_LDEL)
						return K_LEFT;
					else
                                        	return last;
                                }
                        }
                }
        }

        goto back;
}

