/*      te_conf.c

        Text editor.

        Configuration variables.

        Copyright (c) 2020-2021 Miguel Garcia / FloppySoftware

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

        22 Oct 2020 : Start.
        22 Feb 2021 : Added screen rows and columns.
        04 Apr 2021 : Added key bindings.
        06 Apr 2021 : Added screen characters for various purposes.
        11 May 2021 : Default values.
           Nov 2021 : (Ladislau Szilagyi) Adapted for RC2014's SC108 128KB RAM memory module

        Notes:

        This file is shared with TECF, the TE configuration tool.
*/

#include <tekeys.h>

/* Configuration variables
   -----------------------
*/
#asm
        psect data
global _cf_version
global _cf_name
global _cf_rows
global _cf_cols
global _cf_mx_lines
global _cf_tab_cols
global _cf_num
global _cf_clang
global _cf_indent
global _cf_list
global _cf_list_chr
global _cf_rul_chr
global _cf_rul_tab
global _cf_vert_chr
global _cf_horz_chr
global _cf_lnum_chr
global _cf_cr_name
global _cf_esc_name
global _cf_keys
global _cf_keys_ex
global _cf_bytes

cf_start:
                  defm 'TE_CONF'
                defb 0     ;  8 bytes > Identifier + ZERO for the configuration block in memory and COM file.

_cf_version:   defb 2                ;  1 byte  > Configuration version >= 0. It's not the TE version.

_cf_name:        ; 32 bytes > Configuration name + ZERO.
                defm 'WS & VT100'
              defb 0,0,0,0,0,0
              defb 0,0,0,0,0,0,0,0
              defb 0,0,0,0,0,0,0,0

_cf_rows:      defb 50               ;  1 byte  > Screen rows or 0 for auto.
_cf_cols:      defb 104               ;  1 byte  > Screen columns or 0 for auto.

_cf_mx_lines:  defw 4096               ;  2 bytes > Max. number of lines in editor.
                                    ;            Each line takes 2 bytes (1 word) of memory.

_cf_tab_cols:  defb 4                ;  1 byte  > How many spaces a tabulation inserts.

_cf_num:       defb 5                ;  1 byte  > Number of digits for line numbers (see cf_mx_lines) plus 1
                                    ;            for the separator. Set to ZERO for no line numbers.

_cf_clang:     defb 1                ;  1 byte  > C language auto completion: [], {}, (), "", '', /**/.

_cf_indent:    defb 1                ;  1 byte  > Automatic indent according to the indent of previous line.

_cf_list:      defb 1                ;  1 byte  > Automatic list.
_cf_list_chr:  defm '-*>'
                defb 0,0,0,0,0  ;  8 byte  > Legal characters for automatic lists + ZERO.

_cf_rul_chr:   defm '.'              ;  1 byte  > Ruler: Character.
_cf_rul_tab:   defm '!'              ;  1 byte  > Ruler: Tab stop character.

_cf_vert_chr:  defm '|'              ;  1 byte  > Char. for vertical draws -- ie: for line numbers.
_cf_horz_chr:  defm '-'              ;  1 byte  > Char. for horizontal draws -- ie: for status line.

_cf_lnum_chr:  defm '|'              ;  1 byte  > Char. between line numbers and text.

_cf_cr_name:   defm     'CR'
                defb 0,0,0,0,0,0  ;  8 byte  > CR key name
_cf_esc_name:  defm     'ESC'
                defb 0,0,0,0,0  ;  8 byte  > ESC key name


_cf_keys:       ; 29 bytes > 1st character in key binding. Must to be equal to KEYS_MAX.
        defb    CTL_E   ;key.up = ^E
        defb    CTL_X   ;key.down = ^X
        defb    CTL_S   ;key.left = ^S
        defb    CTL_D   ;key.right = ^D
        defb    CTL_Q   ;key.begin = ^QS
        defb    CTL_Q   ;key.end = ^QD
        defb    CTL_Q   ;key.top = ^QR
        defb    CTL_Q   ;key.bottom = ^QC
        defb    CTL_R   ;key.pgUp = ^R
        defb    CTL_C   ;key.pgDown = ^C
        defb    CTL_I   ;key.indent = ^I
        defb    CTL_M   ;key.newLine = ^M
        defb    ESC     ;key.escape = ^[
        defb    CTL_G   ;key.delRight = ^G
        defb    CTL_H   ;key.delLeft = ^H
        defb    CTL_Y   ;key.cut = ^Y
        defb    CTL_O   ;key.copy = ^O
        defb    CTL_W   ;key.paste = ^W
        defb    CTL_K   ;key.delete = ^KY
        defb    CTL_T   ;key.clearClip = ^T
        defb    CTL_Q   ;key.find = ^QF
        defb    CTL_L   ;key.findNext = ^L
        defb    CTL_J   ;key.goLine = ^J
        defb    CTL_A   ;key.wordLeft = ^A
        defb    CTL_F   ;key.wordRight = ^F
        defb    CTL_K   ;key.blockStart = ^KB
        defb    CTL_K   ;key.blockEnd = ^KK
        defb    CTL_K   ;key.blockUnset = ^KU
        defb    CTL_Q   ;key.macro = ^QM

_cf_keys_ex:    ; 29 bytes > 2nd character in key binding. Must to be equal to KEYS_MAX.
        defb    0       ;key.up = ^E
        defb    0       ;key.down = ^X
        defb    0       ;key.left = ^S
        defb    0       ;key.right = ^D
        defm    'S'     ;key.begin = ^QS
        defm    'D'     ;key.end = ^QD
        defm    'R'     ;key.top = ^QR
        defm    'C'     ;key.bottom = ^QC
        defb    0       ;key.pgUp = ^R
        defb    0       ;key.pgDown = ^C
        defb    0       ;key.indent = ^I
        defb    0       ;key.newLine = ^M
        defb    0       ;key.escape = ^[
        defb    0       ;key.delRight = ^G
        defb    0       ;key.delLeft = ^H
        defb    0       ;key.cut = ^Y
        defb    0       ;key.copy = ^O
        defb    0       ;key.paste = ^W
        defm    'Y'     ;key.delete = ^KY
        defb    0       ;key.clearClip = ^T
        defm    'F'     ;key.find = ^QF
        defb    0       ;key.findNext = ^L
        defb    0       ;key.goLine = ^J
        defb    0       ;key.wordLeft = ^A
        defb    0       ;key.wordRight = ^F
        defm    'B'     ;key.blockStart = ^KB
        defm    'K'     ;key.blockEnd = ^KK
        defm    'U'     ;key.blockUnset = ^KU
        defm    'M'     ;key.macro = ^QM

_cf_bytes:     defw $ - cf_start + 2 ;  2 bytes > Block configuration size in bytes.

#endasm

