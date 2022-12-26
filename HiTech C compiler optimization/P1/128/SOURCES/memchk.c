/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 *
 * The memchk.c file is part of the restored P1.COM program
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

/*
    Hitech P1.COM makes use of _Hbss to determine whether a memory location is above or below
    the data areas.
    Whilst this works for CP/M & MSDOS where the image is ordered as
    text, data, bss with heap and stack above this, it may not work for other OS and compilers
    in particular, even thought the user code is often still often in the same order, dynamic
    libraries will not fit this model and it is quite possible for stack and heap to be below the
    text area causing the simple tests in P1.COM to fail

    As only P1.COM allocated addresses are used in the memory checks, this code sets _Ldata and _Hbss
    to define the boundaries of the data area.
    It assumes read only data, data and bss are adjacent blocks and heap/stack are not interleaved
    For linux / GCC the compiler labels etext and end are used and mapped as follows
    etext ->  _Ldata
    end   ->  _Hbss
    For windows only the .rdata and .data segments are considered, others are assumed not to
    contain P1.COM specific data.
    start .rdata ->_Ldata
    end   .data + sizeof(.data) - 1  ->_Hbss
    Note this file is not needed for compilation on CP/M and will fail to build due to the
    #pragma
*/

#ifdef __GNUC__
#ifndef __APPLE__
extern char etext, end;
const char *_Ldata = &etext;
const char *_Hbss = &end;
#endif
#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
/* force windows.h first */
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

const char *_Ldata;
const char *_Hbss;

void initMemAddr(void) {
    HANDLE hModule = GetModuleHandle(NULL); // module handle

    // get the location of the module's IMAGE_NT_HEADERS structure
    IMAGE_NT_HEADERS *pNtHdr = ImageNtHeader(hModule);

    // section table immediately follows the IMAGE_NT_HEADERS
    IMAGE_SECTION_HEADER *pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);

    const char *imageBase             = (const char *)hModule;

    for (int scn = 0; scn < pNtHdr->FileHeader.NumberOfSections; ++scn) {
        if (strcmp(pSectionHdr->Name, ".rdata") == 0)
            _Ldata = imageBase + pSectionHdr->VirtualAddress;
        else if (strcmp(pSectionHdr->Name, ".data") == 0)
            _Hbss = imageBase + pSectionHdr->VirtualAddress + pSectionHdr->Misc.VirtualSize - 1;
        ++pSectionHdr;
    }
}
#else
#error code behind inData missing
#endif
