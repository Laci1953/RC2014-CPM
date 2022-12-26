/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * The ctype1.c file is part of the restored optimization program
 * from the Hi-Tech C compiler v3.09 package.
 *
 *	Andrey Nikitin 04.11.2021
 */

#define	_Z	 0	/* 0000 0000 */
#define	_U	 1	/* 0000 0001 */
#define	_L	 2	/* 0000 0010 */
#define	_D	 4	/* 0000 0100 */
#define	_H	 8	/* 0000 1000 */
#define	_S	16	/* 0001 0000 */

/*
 *	Definitions of valid characters and their types in oprtmizer 
 */
char ccClass[] = {	/* 62cc */
       _Z,    _Z,    _Z,    _Z,    _Z,    _Z,    _Z,    _Z,
       _S,    _S,    _Z,    _Z,    _S,    _S,    _Z,    _Z,
       _Z,    _Z,    _Z,    _Z,    _Z,    _Z,    _Z,    _Z,
       _Z,    _Z,    _Z,    _Z,    _Z,    _Z,    _Z,    _Z,
       _S,    _Z,    _Z,    _Z,    _L,    _Z,    _Z,    _Z,
       _Z,    _Z,    _Z,    _Z,    _Z,    _Z,    _Z,    _Z,
    _D|_H, _D|_H, _D|_H, _D|_H, _D|_H, _D|_H, _D|_H, _D|_H,
    _D|_H, _D|_H,    _Z,    _Z,    _Z,    _Z,    _Z,    _L,
       _Z, _U|_H, _U|_H, _U|_H, _U|_H, _U|_H, _U|_H,    _U,
       _U,    _U,    _U,    _U,    _U,    _U,    _U,    _U,
       _U,    _U,    _U,    _U,    _U,    _U,    _U,    _U,
       _U,    _U,    _U,    _Z,    _Z,    _Z,    _Z,    _L,
       _Z, _L|_H, _L|_H, _L|_H, _L|_H, _L|_H, _L|_H,    _L,
       _L,    _L,    _L,    _L,    _L,    _L,    _L,    _L,
       _L,    _L,    _L,    _L,    _L,    _L,    _L,    _L,
       _L,    _L,    _L,    _Z,    _Z,    _Z,    _Z,    _Z 
};
