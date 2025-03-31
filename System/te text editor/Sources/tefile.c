/*      te_file.c

        Text editor.

        File I/O.

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
        22 Feb 2018 : Check for buffer changes.
        09 Jan 2018 : Optimize ReadFile().
        16 Jan 2019 : Added support for block selection.
        19 Jan 2019 : Optimize WriteFile().
        04 Jan 2021 : Use configuration variables.
           Nov 2021 : (Ladislau Szilagyi) Adapted for RC2014's SC108 128KB RAM memory module
	   Mar 2022 : (Ladislau Szilagyi) Adapted for RC2014's 512KB RAM memory module
	   Mar 2023 : (Ladislau Szilagyi) Review & created single source for 128/512
*/

#include <te.h>
#include <stdio.h>
#include <string.h>
#include <dynm.h>

void* AllocMem(short bytes, char* bank);
char* FreeText(int count, int flag);

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
extern char** lp_arr;                   /* Pointer of Text lines pointers array */
extern char b_lp_arr;
extern char* lp_arr_i_b;             /* Pointer of Text lines dynamic memory banks array */
extern char b_lp_arr_i_b;
extern int   lp_now; /* How many lines are in the array */
extern int   lp_cur; /* Current line */
extern int   lp_chg; /* 0 if no changes are made */
extern char  ln_dat[]; /* Data buffer */
extern int   ln_max; /* Max. # of characters */
extern int  fe_dat[];   /* Data buffer */
extern int fe_now;    /* How many characters are now in the buffer */
extern int fe_set;    /* Set position */
extern int fe_get;    /* Get position */
extern int fe_forced; /* Flag: true if forced character on input */
extern int sysln;    /* NZ when written - for Loop() */
extern int editln;   /* NZ when editing line - for ErrLine() */

extern int tmp;
extern char* ptmp;
extern char  tmpbuf[];
extern char  b;

#if OPT_BLOCK

extern int blk_start;   /* Start line # */
extern int blk_end;     /* End line # */
extern int blk_count;   /* # of lines */
extern char* clp_arr[];         /* Pointer of Multi-line clipboard lines pointers array */
extern char* clp_arr_i_b;    /* Pointer of Multi-line clipboard lines memory banks array */

#else

extern char *clp_line;  /* Just one line */

#endif

#if OPT_FIND

extern char find_str[FIND_MAX];

#endif

/* Reset lines array
   -----------------
*/
void ResetLines()
{
        FreeText(cf_mx_lines, 0);

        lp_cur = lp_now = lp_chg = box_shr = box_shc = 0;

#if OPT_BLOCK

        blk_start = blk_end = -1;
        blk_count = 0;

#endif

}

/* New file
   --------
*/
void NewFile()
{
        /* Free current contents */
        ResetLines();

        /* No filename */
        file_name[0] = '\0';

        /* Build first line */
        InsertLine(0, NULL);
}

/* Backup the previous file with the same name
   -------------------------------------------
   Return NZ on error.
*/
void BackupFile(char *fn)
{
        FILE *fp;
        char *bkp;

        /* Check if file exists */
        /* if((fp = fopen(fn, "r")) != NULL) */
        if((fp = fopen(fn, "r")))
        {
                fclose(fp);

                bkp = "te.bkp";

                /* Remove the previous backup file, if exists */
                remove(bkp);

                /* Rename the old file as backup */
                rename(fn, bkp);
        }
}

/* Refresh count of lines read or written
   --------------------------------------
*/
void RefreshLineNr(int i)
{
	char buf[5];

	sprintf(buf, "%d", i);
	CrtLocate(cf_rows - 1, 22);
	putstr(buf);
}

/* Read text file
   --------------
   Returns NZ on error.
*/
int ReadFile(char* fn)
{
        FILE *fp;
	int i, len;
        unsigned char err, oversize;
        char *p;

        /* Free current contents */
        ResetLines();

        /* Setup some things */
        err = oversize = 0;

        SysLine("Reading file... Line# ");

        /* Open the file */
        if(!(fp = fopen(fn, "r")))
        {
                ErrLineOpen(); return -1;
        }

        /* Read the file */
        for(i = 0; i < 32000; ++i)
        {
		/* refresh number of lines read */
		RefreshLineNr(i);

                if(!fgets(ln_dat, LINE_SIZE_MAX + 1, fp))
                        break;

                if(lp_now == cf_mx_lines)
                {
                        ErrLineTooMany(); err = 1; break;
                }

                len = strlen(ln_dat);

                if(ln_dat[len - 1] == '\n')
                        ln_dat[--len] = 0;

		p = ln_dat;

		if ((len=strlen(p)) > LINE_SIZE_MAX)
		{
			p[LINE_SIZE_MAX]=0;
			len = LINE_SIZE_MAX;
			oversize = 1;
		}

                ptmp = (char*)AllocMem(len+1, &b);

                if (ptmp)
                {
                        PutWord(lp_arr, lp_now, ptmp, b_lp_arr);
                        PutByte(lp_arr_i_b, lp_now, b, b_lp_arr_i_b);

                        PutString(p, ptmp, b);

                        lp_now++;
                }
                else
                {
                        err = 1; break;
                }
        }

        /* Close the file */
        fclose(fp);

        /* Check errors */
        if(err)
                return -1;

        /* Check if empty file */
        if(!lp_now)
        {
                /* Build first line */
                InsertLine(0, NULL);
        }

	/* Check truncated lines */
	if(oversize)
		ErrLine("Some lines were truncated!");

        /* Success */
        return 0;
}

/* Write text file
   ---------------
   Returns NZ on error.
*/
int WriteFile(char* fn)
{
        FILE *fp;
        int i;

        SysLine("Writing file... Line# ");

        /* Backup old file */
        BackupFile(fn);

        /* Open the file */

        if(!(fp = fopen(fn, "w")))
        {
                ErrLineOpen(); return -1;
        }

        /* Write the file */
        for(i = 0; i < lp_now; ++i)
        {
		/* refresh number of lines written */
		RefreshLineNr(i);

                GetString(tmpbuf, GetWord(lp_arr, i, b_lp_arr), GetByte(lp_arr_i_b, i, b_lp_arr_i_b));

                if(fputs(tmpbuf, fp) == EOF || fputc('\n', fp) == EOF)
                {
                        fclose(fp); remove(fn);

                        ErrLine("Can't write");

                        return -1;
                }
        }

        /* Close the file */
        if(fclose(fp) == EOF)
        {
                remove(fn);

                ErrLine("Can't close");

                return -1;
        }

        /* Success */
        return (lp_chg = 0);
}

