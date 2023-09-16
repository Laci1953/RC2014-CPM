# RC2014-CPM

Why a "custom" CP/M ?
---------------------

Because the "official" RC2014's CP/M is too "fat" ... eating all the available RAM space till 0FFFFH

Why is this a bad thing?

Because this way, it's impossible to make use of the "upper" 64KB RAM available on all the 128KB RAM Z80 systems.

So, the owners of Z80 systems provided with SC108, SC114, SC118, SC150, SC152 or the Phillip Stevens memory module are affected by this.

It is impossible to make use of the "upper" 64KB RAM because at the top adresses, some "shadow" routines must be stored, at the same address,
in both the "lower" and "upper" 64KB RAM, in order to be able to move bytes between these two 64KB banks.

This is why I tried to customize the BIOS, in order to obtain a "thinner" CP/M.
It's a matter of only some dozens of bytes gained...but it's important.

Using this "customized" CP/M, it is now possible to benefit from :

- an enhanced HiTech C Compiler, able to compile larger C source files ( see https://github.com/Laci1953/HiTech-C-compiler-enhanced )
- an enhanced TE text editor, able to edit (in memory) larger text files ( see https://github.com/Laci1953/RC2014-CPM/tree/main/te )

CP/M and some tools & games for RC2014:
---------------------------------------

PutSys - CP/M for RC2014's (serial SIO, KIO or ACIA, CF 64MB or 128MB) - all combinations covered
- allows using the enhanced HiTech C Compiler, for 128KB RAM systems ( see https://github.com/Laci1953/HiTech-C-compiler-enhanced )    
- allows using the TE text editor, for 128KB RAM systems
- offers 2 and half KB extra TPA space for systems provided with 64MB CF

BOOT - CP/M booters (to be burned to EEPROM)

TE - Miguel Garcia's text editor "te", configured for systems with 128KB or 512KB RAM, allowing large text files to be edited in the internal memory

1MB - an "extended" model of CP/M, making possible to build huge executables, using overlays (up to 496KB code, and up to 448 KB memory space to store data),
for systems provided with the 512KB RAM + 512KB ROM memory module.

ZORK - the famous Infocom game, can be played on RC2014+512KB RAM/ROM memory module

DOWNLOAD - tools for moving files to CF

BDOS_BIOS - source file for CP/M BDOS & BIOS

DEBUG - tiny debugger using Digital I/O module

GREP - search files for a string

ERAQ - erase files with query

COPYQ - copy files with query

FASTDUMP - twice as fast as DUMP

FASTTYPE - twice as fast as TYPE

SARGON78 - the famous chess game, including save/load game as added feature

STARTREK - the game, including save/load game as added feature

BACKGMMN - the Backgammon game, including save/load game as added feature

SOKOBAN - the Sokoban game

SUDOKU - the Sudoku game, including save/load game

CONNECT4 - the Connect Four game

TEXTFALL - a 'Matrix digital rain' style console game

BIRDS - the "birds, eagle and kite" game

