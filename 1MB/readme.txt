RC2014 + 512KB RAM/ROM board
----------------------------

A powerful CP/M development & execution platform
------------------------------------------------

The RC2014, if provided with the 512MB RAM + 512MB ROM memory board, can be used as a powerful CP/M development and execution platform.
The needed RC2014 configuration is :
-	any bankplane 
-	any Z80 CPU board
-	any serial board (ACIA,SIO,KIO)
-	512MB RAM + 512MB ROM memory module
-	CF card module provided with a 64MB CF
-	Digital I/O module (optional)
An EEPROM burner is also needed.

I will describe below the advantages of this configuration.
1.	The enhanced CP/M that I suggest to be used offers an extra 2 and half KB TPA space, allowing to compile/assemble large C/assembler sources
2.	The TE editor can be used to handle large files (up to 8000 lines, 400KB)
3.	The CP/M execution platform that will be presented below allows running huge executables (up to 464KB) which may access up to 448KB dynamic memory.

The CP/M development platform
-----------------------------

The API that can be used:

dyn512.h
--------

void	Init512Banks(void);
Initializes the dynamic allocation buffers.

char*	alloc512(short Size, char* BankPointer);
Allocates a buffer with the specified size; the memory bank containing the buffer is returned. When accessing the buffer, setbank must be called first.

void	free512(char* Buf, char Bank);
Frees the allocated buffer.

void	setbank(char bank);
Selects the specified memory bank.

int	GetTotalFree();
Returns the size of the free dynamic memory ( in KB ).

overlays.h
----------

void InitOverlays(void);
Initializes the overlays system.

short CallOverlay0(void* Routine, char CalledRomBank);
Calls the routine from the specified bank of memory, without passing any parameter.

short CallOverlay1(void* Routine, short Par1, char CalledRomBank);
Calls the routine from the specified bank of memory, passing one parameter.

short CallOverlay2(void* Routine, short Par1, short Par2, char CalledRomBank);
Calls the routine from the specified bank of memory, passing two parameters.

void ReturnOverlay0(short ret_val);
Returns a value from an overlay that has been called without any parameter, from a routine that used at least one local variable.

void ReturnOverlay0_(short ret_val);
Returns a value from an overlay that has been called without any parameter, from a routine without any local variables.

void ReturnOverlay1(short ret_val);
Returns a value from an overlay that has been called with one parameter.

void ReturnOverlay2(short ret_val);
Returns a value from an overlay that has been called with two parameters.

The CP/M execution platform
---------------------------

The memory map of such a platform is the following:

0000-3FFFH		base .com area (code,data,buffers)
4000-7FFFH		overlays area (code, data)
8000-BFFFH		dynamic memory buffer
C000-D880H		overlays buffers area
D880-DFFFH	system buffers
E000-E200H		app stack
E200-FFFFH		BDOS,BIOS

All the applications may use a “base” .COM program, which can call (load) up to 28 separate 16KB “overlays”, stored on the 512KB EEPROM. 
An overlay may call any other overlay (overlays calls and returns use the “stack” principle).
The base or overlays can allocate/deallocate dynamic memory ( up to 16 KB for a single alloc, the total size allocated up to 448KB )
The base and overlays can access all the CP/M BDOS/BIOS functions.

An example follows: (BASE calls OVERLAY4 which calls OVERLAY5)

BASE: use makebase.sub
----------------------

#include <stdio.h>
#include <string.h>
#include <dyn512.h>
#include <overlays.h>

char *pBuf;
short ret;
char bank;

#define proc_4		0x4000

void main(void)
{
  int ret;

  InitOverlays();
  Init512Banks();
  pBuf=alloc512(40, &bank);
  ret=CallOverlay2((void*)proc_4, pBuf, (short)bank, 4); /* call overlay 4 */
  setbank(bank);
  printf(pBuf);
  printf("\r\nOverlay4 returned %d",ret);
  free512(pBuf,bank);
}

OVERLAY4: use linkov4.sub
-------------------------

#include <stdio.h>
#include <string.h>
#include <dyn512.h>
#include <overlays.h>

#define proc5	0x4000

void proc(char* p, char bank)
{
  int ret;

  setbank(bank);
  strcpy(p,"\r\nStored by proc_4");
  ret=CallOverlay0((void*)proc5, 5);	/* call overlay 5 */
  printf("\r\nOverlay5 returned %d",ret);
  ReturnOverlay2(1);
}

OVERLAY5: use linkov5.sub
-------------------------

#include <stdio.h>
#include <string.h>
#include <overlays.h>

void proc(void)
{
  int ret;

  ret=2;
  printf("\r\nPrinted by proc_5");
  ReturnOverlay0(ret);
}


HEX file to be burned (commented):

:20000000F33E00D3783E21D3793E22D37A3E23D37B3E01D37C3100D0AFD3003E03D3803E7A     -CP/M boot
:2000200016D380CDA0003E05D312DB12FE05280921C100CDB800C33600CDA0003E01D311B1
:200040003EEFD317CDA0003E82D3113EEFD317CDA00006180E002100D079D313AFD314D30F
:20006000153EE0D3163E01D3123E20D317CDA000DB17CB5F28FA1E04C50E100680EDB21D06
:2000800020F9C10C10D3110080219400010C00EDB0C300803E20D3783E01320300C300F094
:2000A000DB17CB7F20FADB17CB7728F4C9F5DB80CB4F28FAF1D381C97EB7C8CDAD00231885
:1B00C000F70D0A4346206E6F7420666F756E64206F7220646566656374210034
:020000021000EC                                                                 - overlay 4
:20000000CD5712FEFFDD6E08E5CD1D0321020039F9217140E5DD6E06DD6607E5CD5F40216F
:20002000040039F92E05E5210040E5CDAB01D921040039F9D96D64DD75FEDD74FFDD6EFEF0
:20004000DD66FFE5218440E5CDFE0421040039F9210100E5CD170221020039F9C34F12C162
:20006000D1E1E5D5C54B427E121323B720F96960C90D0A53746F7265642062792070726FA6
:1B008000635F34000D0A4F7665726C6179352072657475726E6564202564000F
:020000021400E8                                                                 – overlay 5
:20000000CD5712FEFFDD36FE02DD36FF00212B40E5CDFE0421020039F9DD6EFEDD66FFE583
:1F002000CDE70121020039F9C34F120D0A5072696E7465642062792070726F635F350043
:00000001FF

Notice that the overlays code is very small ( all the C library routines are called from the BASE code, without being included as code in the overlays ! )

The output:
D>base

Printed by proc_5
Overlay5 returned 2
Stored by proc_4
Overlay4 returned 1
D>

The game ZORK was builded using this development platform.

The CP/M booters and PutSys for CP/M (ACIA,SIO,KIO) are included.
