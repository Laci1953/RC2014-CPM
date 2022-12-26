/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 *	Initial setup for stdio
 */

#include	<stdio.h>

#define	BUFSIZ	512
#define	MAXFILE	5	/* max number of files open */

/*
#define	FILL	0, "        ", "   ", 0, {0}, 0, {0}, 0, {0}, 0
*/

#define	FILL	0, "\t    ", "   ", 0, {0}, 0, {0}, 0, {0},(long)4,0

struct fcb {
    uchar	dr;		/*   +0 drive code				*/
    char	name[8];	/*   +1 file name				*/
    char	ft[3];		/*   +9 file type				*/
    uchar	ex;		/*  +12 file extent				*/
    char	fil[2];		/*  +13 not used				*/
    char	rc;		/*  +15 number of records in present extent	*/
    char	dm[16];		/*  +16 CP/M disk map				*/
    char	nr;		/*  +32 next record to read or write		*/
    uchar	ranrec[3];	/*  +33 random record number (24 bit no. )	*/
    long	rwp;		/*  +36 read/write pointer in bytes		*/
    uchar	use;		/*  +40 use flag				*/
};

/*	 flag values in fcb.use */

#define	U_READ	1	/* file open for reading   */
#define	U_WRITE	2	/* file open for writing   */
#define	U_RDWR	3	/* open for read and write */
#define	U_CON	4	/* device is console 	   */
#define	U_RDR	5	/* device is reader	   */
#define	U_PUN	6	/* device is punch	   */
#define	U_LST	7	/* list device		   */

extern char	_sibuf[BUFSIZ];

FILE	_iob[MAXFILE] =
{
	{
		_sibuf,
		0,
		_sibuf,
		_IOREAD|_IOMYBUF,
		0			/* stdin */
	},
	{
		(char *)0,
		0,
		(char *)0,
		_IOWRT|_IONBF,
		1			/* stdout */
	},
	{
		(char *)0,
		0,
		(char *)0,
		_IOWRT|_IONBF,
		2			/* stderr */
	},
};


struct fcb	_fcb[MAXFILE] =
{
	{ FILL },	/* stdin */
	{ FILL },	/* stdout */
	{ FILL },	/* stderr */
};

/* End of file setup.c */



