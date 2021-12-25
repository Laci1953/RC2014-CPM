ZORK for Z80 on RC2014 
----------------------

ZORK is a famous adventure game, introduced by Infocom 40 years ago.

This folder contains ZORK versions for RC2014 provided with a 512KB ROM + 512KB RAM module, with a serial SIO, ACIA and KIO board; it works with 64MB CF's.

The present project is an adaptation of the game, ported to the RC2014 Z80 homebrew computer, 
from the Donnie Russel II ZORK1 C version published on GitHub ( https://github.com/donnierussellii/zork1c ).

An example of an RC2014 system configuration on which ZORK can run:

- SC112 mother board
- any Z80 CPU board (without RAM or ROM !)
- SC110 SIO board ( or any ACIA or KIO - serial board - take care of which zork_rom_serial.hex you burn!)
- 512KB RAM + 512KB ROM, with the "zork_rom_SIO.hex" EEPROM burned
- Digital I/O module (optional)
- CF board with CP/M installed on a 64MB CF

You must have also an EEPROM burner, to burn the zork_rom_serial.hex file on the 512KB EEPROM. 
This EEPROM will contain a CP/M booter and the ZORK overlays. 
The booter will boot a CP/M from a 64MB CF.
You must first copy CP/M sytem files to the 64MB CF, using the appropriate PutSys.hex file.

The PutSys hex files needed to install CP/M on a 64MB CF are included, for ACIA, SIO and KIO serial interfaces.

The necessary files to play ZORK are: 

zork.hex (to be converted to zork.com using LOAD & SAVE CP/M commands), 
zorkcomm.hex (will be loaded by zork.com) ,
zork_rom_serial.hex (serial=acia or sio or kio) to be burned to the 512MB EEPROM.

At startup, the zorkcomm.hex (which contains code & must be stored on the same disk as zork.com) will be loaded in memory, just below the BDOS. 

If a Digital I/O module is present, it will "display" the flow of execution (the "index" of the current overlay is showed on the leds).

The architecture of the implementation
--------------------------------------

The Donnie Russel II C version is huge (13273 C & H lines, with a total size of 454935 bytes).
Just take a look at those files!

Mission impossible? Not quite so...but it took me a month to "slice" the sources!

I choosed to port these files to the RC2014 Z80 computer using the HiTech C v3.09 compiler & linker.

The choice proved to be good enough, apart some unpleasant surprises:
- memset is implemented in the HiTech C library with the last two arguments switched
- strchr(0) does not return a valid pointer (as it does in unix's gcc) 

I re-wrote in assembler the file-accessing routines (for the ZORK Save/Restore commands) and modified small parts of the text parser.

The 512KB RAM + 512KB ROM module allows the use of 16KB memory-mapped segments (32 RAM & 32 ROM segments).

I used some 20 of the RAM segments for storing the BASE + OVERLAYS code segments (copied at start-up from ROM).

I used the first 16KB (0000-3FFF) to load the BASE program and then the OVERLAYS called by the BASE or by other OVERLAYS (there are 20 of them).

The next two 16KB (4000-BFFF) segments contain the data used by the game (texts, tables, pointers to routines). 

All the texts used are compressed at a ~60% ratio and will be decompressed at run-time. 

The last 16KB contains the COMMON code part (overlays.as, zorkrand.as, comprss.c, common.c), + some C library routines + stack + CP/M BDOS & BIOS
(all BDOS & BIOS functions are accessible from BASE & OVERLAYS, including disk file routines).

The OVERLAYS manager (overlays.as) handles calling an overlay (optionally passing a parameter) and returning a value from an OVERLAY.

Some details concerning the architecture follows below:

				      512 module BANK
OVERLAY					ROM	RAM	SOURCE FILES									
---------------------------------------------------------------------------------------------------------------------------
BASE				(0+cpm boot)	32	zorkbase.c, loadhex.c
				(1 not used)	33
				(2 not used)	34
COMMON				(3 not used)	35	overlays.as, zorkrand.as, comprss.c, common.c	
OVR_PARSE				4	36	zorkpars.c, miscpars.c							
OVR_EVENT_ROUTINES 			5	37	gamevent.c, misceven.c, tables2.c					
OVR_OVERRIDE_OBJECT_DESC 		6	38	ovobdesc.c								
OVR_OVERRIDE_ROOM_DESC 			7	39	ovrmdesc.c								
OVR_SAVE_RESTORE			8	40	saverest.c								
OVR_DO_MISC				9	41	domisc.c dogofrom.c miscdomi.c						
OVR_INTERCEPT				10	42	interact.c								
OVR_GO_FROM				11	43	gofrom.c gofrmisc.c								
OVR_DO_MISC_WITH_TO			12	44	domiscwt.c miscwt.c							
OVR_DO_PARSE_ACTION			13	45	psactdir.c, miscpsa1.c, tables2.c					
OVR_VILLAINS				14	46	villain1.c, villain2.c, villain3.c, villmisc.c				
OVR_VILLAINS_BLOW			15	47	villblow.c, villbusy.c							
OVR_DO_MISC_EXT				16	48	domiscex.c								
OVR_DO_MISC_TAKE_TO			17	49	domitake.c								
OVR_DO_MISC_TO				18	50	domgithr.c								
OVR_DO_MISC_WITH_TO_EXT			19	51	domwtext.c, miscwtex.c							
OVR_DO_MISC_DO				20	52	miscdomi.c, domiscdo.c							
OVR_DO_PARSE_ACTION_EX1			21	53	parsdrop.c, miscdrop.c							
OVR_DO_PARSE_ACTION_EX2			22	54	parstake.c, misctak1.c, misctak2.c					
OVR_DO_PARSE_ACTION_EX3			23	55	actthrow.c, miscactt.c			
OVR_DO_PARSE_ACTION_EX2A 		24	56	takeallr.c, miscallr.c			
DATA1					25	57	data.c, gametab1.c, gametb2a.c, gametab3.c, gametb2b.c, gametab4.c	
DATA2					26	58	tables1.c, tables3.c, tables4.c, tables5.c, tables6.c, tables7.c	

OVERLAYS LIST
-------------
#define OVR_PARSE			36
#define OVR_EVENT_ROUTINES 		37
#define OVR_OVERRIDE_OBJECT_DESC	38
#define OVR_OVERRIDE_ROOM_DESC 		39
#define OVR_SAVE_RESTORE		40
#define OVR_DO_MISC			41
#define OVR_INTERCEPT			42
#define OVR_GO_FROM			43
#define OVR_DO_MISC_WITH_TO		44
#define OVR_DO_PARSE_ACTION		45
#define OVR_VILLAINS			46
#define OVR_VILLAINS_BLOW		47
#define OVR_DO_MISC_EXT			48
#define OVR_DO_MISC_TAKE_TO		49
#define OVR_DO_MISC_TO			50
#define OVR_DO_MISC_WITH_TO_EXT		51
#define OVR_DO_MISC_DO			52
#define OVR_DO_PARSE_ACTION_EX1		53
#define OVR_DO_PARSE_ACTION_EX2		54
#define OVR_DO_PARSE_ACTION_EX3		55
#define OVR_DO_PARSE_ACTION_EX2A	56

CALL MAP
---------
BASE
-->36
   -->40
   -->41
      -->48
      -->49
   -->42
      -->38
      -->39
         -->38
   -->43
      -->38
      -->39
         -->38
   -->44
      -->51
   -->45
      -->41
         -->48
         -->49
      -->50
         -->38
         -->39
            -->38
      -->53
      -->54
         -->56
      -->55
         -->41
            -->48
            -->49
         -->44
            -->51
         -->50
            -->38
            -->39
               -->38
   -->52
      -->38
      -->39
         -->38
-->37
   -->46
      -->47
-->38
-->39
   -->38
-->40
   -->38
   -->39
      -->38

List of routines called in overlays
-----------------------------------

BASE (zorkbase.c, loadhex.c) 					- linkbase
-----------------------------------

OVR_PARSE (zorkpars.c, miscpars.c) 				- linkov36
-----------------------------------
Routines:
Parse

OVR_EVENT_ROUTINES (gamevent.c, misceven.c, tables2.c) 		- linkov37
-----------------------------------
Routines:
RunEventRoutines

OVR_OVERRIDE_OBJECT_DESC (ovobdesc.c)				- linkov38
-----------------------------------
Routines:
PrintDesc_*

OVR_OVERRRIDE_ROOM_DESC (ovrmdesc.c)				- linkov39
-----------------------------------
Routines:
PrintDesc_*

OVR_SAVE_RESTORE (saverest.c) 					- linkov40
-----------------------------------
Routines:
DoRestore
DoSave

OVR_DO_MISC (domisc.c, dogofrom.c, miscdomi.c)			- linkov41
-----------------------------------
Routines:
DM_*

OVR_INTERCEPT (interact.c)					- linkov42
-----------------------------------
Routines:
InterceptAction

OVR_GO_FROM (gofrom.c, gofrmisc.c)				- linkov43
-----------------------------------
Routines:
GoFrom_*

OVR_DO_MISC_WITH_TO (domiscwt.c, miscwt.c)			- linkov44
-----------------------------------
Routines:
DMWT_*

OVR_DO_PARSE_ACTION (psactdir.c, miscpsa1.c, tables2.c)		- linkov45
-----------------------------------
Routines:
ParseActionRestartOrQuit
ParseActionWhereIs
ParseActionExamine
ParseActionGive
ParseActionDirection
DoActionOnObject
ParseActionTake 
ParseActionDropPut
ParseActionThrow

OVR_VILLAINS (villain1.c, villain2.c, villain3.c, villmisc.c) 	- linkov46
-----------------------------------
TRoutines:
VillainsRoutine

OVR_VILLAINS_BLOW (villblow.c, villbusy.c) 			- linkov47
-----------------------------------
Routines:
BlowRemark
VillainBusy
CureRoutine
FindWeapon

OVR_DO_MISC_EXT (domiscex.c)					- linkov48
-----------------------------------
Routines:
DM_*_ext

OVR_DO_MISC_TAKE_TO (domitake.c)				- linkov49
-----------------------------------
Routines:
DM_*_tk

OVR_DO_MISC_TO	(domgithr.c )					- linkov50
-----------------------------------
Routines:
DoMiscGiveTo_*
DoMiscThrowTo_*

OVR_DO_MISC_WITH_TO_EXT (domwtext.c, miscwtex.c)		- linkov51
-----------------------------------
Routines:
DMWT_*_ext

OVR_DO_MISC_DO (miscdomi.c, domiscdo.c)				- linkov52
-----------------------------------
Routines:
DoOdysseus
DoCommandActor
DoTalkTo
DoGreet
DoSay
DoJump
DoSleep
DoDisembark
DoLaunch
DoLand
DoEcho
DoPray
DoVersion
DoDiagnose
DoSwim

OVR_DO_PARSE_ACTION_EX1	(parsdrop.c, miscdrop.c)		- linkov53
-----------------------------------
Routines:
ParseActionDropPut_ext


OVR_DO_PARSE_ACTION_EX2 (parstake.c, misctak1.c, misctak2.c)	- linkov54
-----------------------------------
Routines:
ParseActionTake_ext 

OVR_DO_PARSE_ACTION_EX3	(actthrow.c miscactt.c)			- linkov55
-----------------------------------
Routines:
ParseActionThrow_ext
DoActionOnObject_ext

#define OVR_DO_PARSE_ACTION_EX2A (takeallr.c, miscallr.c)	- linkov56
-----------------------------------
Routines:
TakeOutOfRoutine
TakeAllRoutine


Memory map
----------

16K (0000H-3FFFH) Base / Overlays (text,data,bss) - loaded as 16K RAM page
16K (4000H-7FFFH) DATA1 - loaded as 16K RAM page
16K (8000H-BFFFH) DATA2 - loaded as 16K RAM page
16K (C000H-FFFFH) COMMON - 16K RAM (compress,overlays manager,rand,basic parser,C lib), STACK(~200H), BDOS,BIOS


COMMON routines
------
DecompressText
PrintNewLine
PrintText
PrintLine
PrintCompText
PrintCompLine
GetWords
MatchCurWord

Overlays manager
----------------
RAM
bank32		= (0000H-3FFFH)	; (base + overlays) buffer
bank33		= (4000H-7FFFH) ; DATA1
bank34		= (8000H-BFFFH) ; DATA2
bank35		= (C000H-FFFFH)	; COMMON code & data, stack, BDOS,BIOS
bank36...63	= overlays

ROM
bank0		= CP/M boot
bank1		= DATA1
bank2		= DATA2
bank3		= (nothing)
bank4...31	= overlays

RAM MEMORY BANKS
--------
0	BASE: Parser main + C libs (text,data,bss)
1	DATA1 + strings buf la 7D80H !!!
2	DATA2 + alloc buf la BB00H !!!
3	COMMON: rand, OVERLAYS MANAGER, compress, parser(basic) + C libs(text,data,bss), stack, BDOS, BIOS
4...31	OVERLAYS: Parser parts + game + villains + C libs (text,data,bss)

DATA1
-----
data.c:
	RoomDesc
	BlockMsg
	ObjectDesc
gametabs.c:
	DoMiscWithTo
	DoMisc
	DoMiscGiveTo
	DoMiscThrowTo
	GoFrom
	OverrideRoomDesc
	OverrideObjectDesc

data.obj        data         4000     35F8
gametab1.obj    data         75F8      19D
gametb2a.obj    data         7795      276
gametb2b.obj    data         7A0B      27D
gametab3.obj    data         7C88      149
gametab4.obj    data         7DD1       F6

TOTAL           Name         Link     Load   Length
                (abs)           0        0        0
                data         4000     4000     3D7C

DATA2
---------
tables1.c:
	Room
tables3.c:
	Obj
tables4.c:
	VerbToAction
tables5.c:
	NounPhraseToObj
tables6.c:
	NounPhraseToFixedObj
tables7.c:
	TreasureScore
	RoomScore

tables1.obj     data         8000      2A8
tables3.obj     data         82A8      460
tables4.obj     data         8708      D8F
tables5.obj     data         9497      DF0
tables6.obj     data         A287     1453
tables7.obj     data         B6DA       73

TOTAL           Name         Link     Load   Length
                (abs)           0        0        0
                data         8000     8000     374D
COMMON_BSS (385H) at B750H-BA85H
ALLOC_BUF (500H) at BB00H-BFFFH

==>DATADEFS.OBJ
==>COMMDEFS.OBJ

NOTE: the table below must be included in OVERLAYS
tables2.c:
	RoomPassages


How ZORK was built for RC2014
-----------------------------

(symtoas.com & symtoh.com are provided as .hex files)

1.
- fill in (gametab1.c, gametb2a.c, gametb2b.c, gametab3.c, gametab4.c) with fictive routine addresses (0)
>submit makedata
>submit linkdat1 
==> data1.sym

2.
>submit linkdat2 
==> data2.sym 
										
>objtohex data2.obj data2.hex
- merge data1.sym, data2.sym to data.sym
>symtoas datadefs.as data.sym 
>zas datadefs.as 
==>datadefs.obj

3.
>submit makecomm
>submit linkcomm  
==> comm.sym, comm.obj

>objtohex comm.obj zorkcomm.hex
>symtoas commdefs.as comm.sym 
>zas commdefs.as

>pip base.sym=comm.sym
- edit base.sym (erase libc routines)
>symtoas basedefs.as base.sym
>zas basedefs.as

4. Now we must store the "real" routine addresses to the DATA segments, and build the overlays...

Building order  : (use submit makeovN, then submit linkovN)
---------------------------------------------------------
COMMON	(makecomm,linkcomm)		==> zorkcomm.hex, commdefs.obj , basedefs.obj
38 OVR_OVERRIDE_OBJECT_DESC 		==> ovr38.h, ovr38.hex
39 OVR_OVERRIDE_ROOM_DESC		==> ovr39.h, ovr39.hex
50 OVR_DO_MISC_TO			==> ovr50.h, ovr50.hex
56 OVR_DO_PARSE_ACTION_EX2A		==> ovr56.h, ovr56.hex
55 OVR_DO_PARSE_ACTION_EX3		==> ovr55.h, ovr55.hex
54 OVR_DO_PARSE_ACTION_EX2		==> ovr54.h, ovr54.hex
53 OVR_DO_PARSE_ACTION_EX1		==> ovr53.h, ovr53.hex
45 OVR_DO_PARSE_ACTION			==> ovr45.h, ovr45.hex
52 OVR_DO_MISC_DO			==> ovr52.h, ovr52.hex
51 OVR_DO_MISC_WITH_TO_EXT		==> ovr51.h, ovr51.hex
49 OVR_DO_MISC_TAKE_TO			==> ovr49.h, ovr49.hex
48 OVR_DO_MISC_EXT			==> ovr48.h, ovr48.hex
47 OVR_VILLAINS_BLOW			==> ovr47.h, ovr47.hex
46 OVR_VILLAINS				==> ovr46.h, ovr46.hex
44 OVR_DO_MISC_WITH_TO			==> ovr44.h, ovr44.hex
43 OVR_GO_FROM				==> ovr43.h, ovr43.hex
42 OVR_INTERCEPT			==> ovr42.h, ovr42.hex
41 OVR_DO_MISC				==> ovr41.h, ovr41.hex
57 DATA1	(makedata,linkdat1)	==> data1.hex
58 DATA2	(linkdat2)		==> data2.hex
40 OVR_SAVE_RESTORE			==> ovr40.h, ovr40.hex
37 OVR_EVENT_ROUTINES			==> ovr37.h, ovr37.hex
36 OVR_PARSE				==> ovr36.h, ovr36.hex
BASE (makebase, linkbase)		==> zork.com

The ovrN.H files must be builded using symtoh.com

How to use symtoh:
-----------------
Let's say we must "define" all the symbols having as prefix "xyz", for the ovrN.
We edit ovrN.sym that was builded after linkovN (it contains the values: 1234H for _xyzABC and 3456H for __xyzXYZ).
We insert _xyz as the first line.
Then, we use:
>symtoh ovrN.h ovrN.sym
...which will build the ovrN.h file, containing: 
#define xyzABC 0x1234
#define xyzXYZ 0x3456

OUTPUT:
-------
zork.com
zorkcomm.hex (to be moved to the CP/M disk containing zork.com)
zork_rom.hex (cpm boot + ovrNs.hex merged) --> burned on 512KB EEPROM (use "extended segment address" records!)

Starting zork.com will first read zorkcomm.hex, then initialize the overlays support (copy all ROM segments to RAM), then will display the introductory message.

See the Infocom manual to learn how to play the game.

Enjoy!
