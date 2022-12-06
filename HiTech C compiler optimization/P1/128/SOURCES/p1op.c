/*
 *
 * The op.c file is part of the restored P1.COM program
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
#include "p1.h"
/* clang-format off */
t8_t opTable[68] = {
{"",    6,    0,    0x200,  7}, /*  60 */
{"(",   0x1e, 2,    0x200,  3}, /*  61 */
{"",    0x1f, 0,    0,      0}, /*  62 */
{"",    2,    0,    0,      0}, /*  63 */
{"",    0x1e, 2,    0,      0}, /*  64 */
{"",    0x1e, 0,    0,      0}, /*  65 */
{"",    2,    0,    0,      0}, /*  66 */
{".",   0x1e, 2,    0xa00,  2}, /*  67 */
{"",    0x1e, 2,    0,      0}, /*  68 */
{"*U",  0x1c, 0x1c, 0x4200, 3}, /*  69 */
{"&U",  0x1c, 0x1c, 0x2200, 4}, /*  70 */
{"-U",  0x1c, 0x1c, 0x30,   1}, /*  71 */
{"!",   0x1c, 8,    2,      6}, /*  72 */
{"~",   0x1c, 0x18, 0x20,   1}, /*  73 */
{"",    0x1c, 8,    0,      0}, /*  74 */
{"",    0x1c, 8,    0,      0}, /*  75 */
{"",    0x1c, 8,    0,      0}, /*  76 */
{"",    0x1c, 8,    0,      0}, /*  77 */
{"#",   0x1c, 0x18, 0x300,  5}, /*  78 */
{"()",  0x1c, 0xa,  0,      0}, /*  79 */
{"*",   0x1a, 0x16, 0x70,   1}, /*  80 */
{"&",   0x11, 0x16, 0x60,   1}, /*  81 */
{"-",   0x18, 0x16, 0x7c,   1}, /*  82 */
{"/",   0x1a, 0x12, 0x70,   1}, /*  83 */
{"%",   0x1a, 0x12, 0x60,   1}, /*  84 */
{"+",   0x18, 0x12, 0x74,   1}, /*  85 */
{">>",  0x16, 0x12, 0x60,   1}, /*  86 */
{"<<",  0x16, 0x12, 0x60,   1}, /*  87 */
{"<",   0x14, 0x12, 0x10f8, 6}, /*  88 */
{">",   0x14, 0x12, 0x10f8, 6}, /*  89 */
{"<=",  0x14, 0x12, 0x10f8, 6}, /*  90 */
{">=",  0x14, 0x12, 0x10f8, 6}, /*  91 */
{"==",  0x12, 0x12, 0x90f8, 6}, /*  92 */
{"!=",  0x12, 0x12, 0x90f8, 6}, /*  93 */
{"^",   0x10, 0x12, 0x60,   1}, /*  94 */
{"|",   0xf,  0x12, 0x60,   1}, /*  95 */
{"&&",  0xe,  2,    3,      6}, /*  96 */
{"||",  0xd,  2,    3,      6}, /*  97 */
{"?",   0xc,  0x1a, 2,      2}, /*  98 */
{":",   0xc,  0x1a, 0x12f8, 1}, /*  99 */
{"=",   0xa,  0xa,  0x2600, 1}, /* 100 */
{"=+",  0xa,  0xa,  0x2434, 1}, /* 101 */
{"++",  0,    0xa,  0x2434, 1}, /* 102 */
{"=-",  0xa,  0xa,  0x2434, 1}, /* 103 */
{"--",  0,    0xa,  0x2434, 1}, /* 104 */
{"=*",  0xa,  0xa,  0x2430, 1}, /* 105 */
{"=/",  0xa,  0xa,  0x2430, 1}, /* 106 */
{"=%",  0xa,  0xa,  0x2420, 1}, /* 107 */
{"=>>", 0xa,  0xa,  0x2020, 1}, /* 108 */
{"=<<", 0xa,  0xa,  0x2020, 1}, /* 109 */
{"=&",  0xa,  0xa,  0x2420, 1}, /* 110 */
{"=^",  0xa,  0xa,  0x2420, 1}, /* 111 */
{"=|",  0xa,  0xa,  0x2420, 1}, /* 112 */
{",",   7,    2,    0x200,  7}, /* 113 */
{";",   7,    2,    0x200,  2}, /* 114 */
{"",    0,    1,    0,      0}, /* 115 */
{"",    0,    0x11, 0,      0}, /* 116 */
{"",    0,    0x11, 0,      0}, /* 117 */
{"",    0,    0x11, 0,      0}, /* 118 */
{"",    0,    0x11, 0,      0}, /* 119 */
{"..",  0,    0,    0,      0}, /* 120 */
{")",   0,    0,    0x200,  7}, /* 121 */
{"$U",  0,    0,    0x200,  7}, /* 122 */
{"$",   0,    2,    2,      7}, /* 123 */
{"->",  0,    0x12, 0x880,  2}, /* 124 */
{"@",   0,    2,    0x480,  7}, /* 125 */
{"",    0,    1,    0,      0}, /* 126 */
{"",    0,    0x11, 0,      0}, /* 127 */


}; /* 9271 */
