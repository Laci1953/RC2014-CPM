/*
 *
 * The tok.h file is part of the restored P1.COM program
 * from the Hi-Tech CP/M Z80 C v3.09
 *
 * Not a commercial goal of this laborious work is to popularize among
 * potential fans of 8-bit computers the old HI-TECH Z80 C compiler V3.09
 * (HI-TECH Software) and extend its life, outside of the CP/M environment
 * for full operation in windows 32/64 and Unix-like operating systems
 *
 * The HI-TECH Z80 C cross compiler V3.09 is provided free of charge for any use,
 * private or commercial, strictly as-is. No warranty or product support
 * is offered or implied including merchantability, fitness for a particular
 * purpose, or non-infringement. In no event will HI-TECH Software or its
 * corporate affiliates be liable for any direct or indirect damages.
 *
 * You may use this software for whatever you like, providing you acknowledge
 * that the copyright to this software remains with HI-TECH Software and its
 * corporate affiliates.
 *
 * All copyrights to the algorithms used, binary code, trademarks, etc.
 * belong to the legal owner - Microchip Technology Inc. and its subsidiaries.
 * Commercial use and distribution of recreated source codes without permission
 * from the copyright holderis strictly prohibited.
 *
 *
 * See the readme.md file for additional commentary
 *
 * Mark Ogden
 * 09-Jul-2022
 */
#ifndef _TOK_H
#define _TOK_H

#define T_EOF       1
#define T_SEMI      2
#define T_RBRACE    3
#define T_LBRACE    4
#define S_CLASS     5
#define D_6         6
#define D_LABEL     7
#define D_STRUCT    8
#define D_UNION     9
#define D_MEMBER    10
#define D_ENUM      11
#define D_CONST     12

#define T_3DOT      13
#define D_14        14
#define D_15        15
#define T_18        18
#define T_ASM       19
#define T_AUTO      20
#define T_BREAK     21
#define T_CASE      22
#define T_CHAR      23
#define T_CONTINUE  24
#define T_DEFAULT   25
#define T_DO        26
#define T_DOUBLE    27
#define T_ELSE      28

#define T_ENUM      30
#define T_EXTERN    31
#define T_FLOAT     32
#define T_FOR       33
#define T_GOTO      34
#define T_IF        35
#define T_INT       36
#define T_LONG      37
#define T_REGISTER  38
#define T_RETURN    39
#define T_SHORT     40
#define _T_SIZEOF   41
#define T_STATIC    42
#define T_STRUCT    43
#define T_SWITCH    44
#define T_TYPEDEF   45
#define T_UNION     46
#define T_UNSIGNED  47
#define T_VOID      48
#define T_WHILE     49
#define T_60        60
#define T_61        61
#define T_LPAREN    62
#define T_RPAREN    63
#define T_64        64
#define T_LBRACK    65
#define T_RBRACK    66
#define T_DOT       67
#define T_POINTER   68
#define T_69        69
#define D_ADDRESSOF 70
#define T_LNOT      72
#define T_BNOT      73
#define T_INC       74
#define T_75        75 /* ? post / prefix variant */
#define T_DEC       76
#define T_77        77 /* ? post/prefix variant */
#define T_SIZEOF    78
#define T_79        79
#define T_STAR      80
#define T_BAND      81
#define T_MINUS     82
#define T_DIV       83
#define T_MOD       84
#define T_PLUS      85
#define T_SHR       86
#define T_SHL       87
#define T_LT        88
#define T_GT        89
#define T_LE        90
#define T_GE        91
#define T_EQEQ      92
#define T_NE        93
#define T_XOR       94
#define T_BOR       95
#define T_LAND      96
#define T_LOR       97
#define T_QUEST     98
#define T_COLON     99
#define T_EQ        100
#define T_PLUSEQ    101
#define T_102       102 /* ? postfix variant */
#define T_MINUSEQ   103
#define T_104       104 /* ? post fix variant */
#define T_MULEQ     105
#define T_DIVEQ     106
#define T_MODEQ     107
#define T_SHREQ     108
#define T_SHLEQ     109
#define T_ANDEQ     110
#define T_XOREQ     111
#define T_OREQ      112
#define T_COMMA     113
#define T_114       114
#define T_ID        115
#define T_ICONST    116
#define T_LCONST    117
#define T_SCONST    118
#define S_TYPE      119
#define T_120       120
#define T_121       121
#define T_122       122
#define T_123       123
#define T_124       124
#define T_125       125
#define T_126       126
#define T_FCONST    127
#define T_128       128

#define DT_UNSIGNED 1 /* bit to indicate unsigned data type*/
#define DT_2        2
#define DT_CHAR     4
#define DT_UCHAR    5
#define DT_SHORT    6
#define DT_USHORT   7
#define DT_INT      8
#define DT_UINT     9
#define DT_LONG     10
#define DT_ULONG    11
#define DT_CONST    12
#define DT_UCONST   13
#define DT_FLOAT    14
#define DT_DOUBLE   16
#define DT_ENUM     18
#define DT_VOID     20
#define DT_POINTER  22
#define DT_VARGS    24
#define DT_STRUCT   26
#define DT_UNION    28

/* node types */
#define ANODE       2 /* function args node */
#define ENODE       1 /* expression node */
#define SNODE       0 /* symbol node */

#endif
