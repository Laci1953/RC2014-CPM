#include "te.h"

// Assembler routines for TE

/* Print character on screen X times
   ---------------------------------
*/

/* putchrx(ch, n) */
/* int ch, n;     */
#asm
psect text
global _CrtOut
global _putchrx
_putchrx:
        ld      hl,2
        add     hl,sp
        ld      c,(hl)  ;ch
        inc     hl
        inc     hl
        ld      b,(hl)  ;n
1:      push    bc
        call    _CrtOut
        pop     bc
        djnz    1b
        ret
#endasm

/* Print string on screen
   ----------------------
*/

/* putstr(s) */
/* char *s;  */
#asm
psect text
global _putstr
_putstr:
        ld      hl,2
        add     hl,sp
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        ex      de,hl
1:      ld      a,(hl)
        or      a
        ret     z
        inc     hl
        ld      c,a
        push    hl
        push    bc
        call    _CrtOut
        pop     bc
        pop     hl
        jr      1b
#endasm

/* putstrn(s, len) */
#asm
psect text
global _putstrn
_putstrn:
        ld      hl,2
        add     hl,sp
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        inc     hl
        ld      b,(hl)
        ex      de,hl
1:      ld      a,(hl)
        or      a
        ret     z
        inc     hl
        ld      c,a
        push    hl
        push    bc
        call    _CrtOut
        pop     bc
        pop     hl
        djnz    1b
        ret
#endasm

#asm
global _CrtSetupEx
_CrtSetupEx:
        ld  hl,(1)
        inc hl
        inc hl
        inc hl
        ld  de,BiosConst
        ld  bc,9
        ldir
        ret

BiosConst:  jp 0
BiosConin:  jp 0
BiosConout: jp 0
#endasm

#ifndef Z80ALL
#asm
global _CrtOut
_CrtOut:
        ld      hl,2
        add     hl,sp
        ld      a,(hl)  ;ch
        and     7FH     ;drop high bit
        ld      c,a
        cp   10
        jp   nz,BiosConout
        call BiosConout
        ld   c,13
        jp   BiosConout

#endasm
#endif

#ifdef Z80ALL

#asm
global _CrtInEx
global _SetCursor
global _EraseCursor
_CrtInEx:
        call _SetCursor
        call BiosConin
        ld h,0
        ld l,a
        call _EraseCursor
        ret
#endasm

#else

#asm
global _CrtInEx
_CrtInEx:
        call BiosConin
        ld h,0
        ld l,a
        ret
#endasm

#endif

