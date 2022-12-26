;
;	Upper 64 KB RAM memory allocator
;
;	Szilagyi Ladislau
;
;	December 2022
;
SC108	equ	1	;1=SC108, 0=32KB ROM + 128KB RAM Phillip Stevens memory module 
;
TRACE	equ	0	;1=print a message when entering a routine
RCHECK	equ	0	;1=checks range of addresses
TRAP	equ	0	;1=traps a write at the TARGET address

DEBUG	equ	TRACE + RCHECK + TRAP

	psect	text
;
;	Dynamic memory allocator for 64KB Upper RAM
;	Data access routines
;	Shadow routines
;

	GLOBAL	_InitDynM
	GLOBAL	_TotalSize
	GLOBAL	_myallocsym
	GLOBAL	_myallocstr
	GLOBAL	_GetByte
	GLOBAL	_PutByte
	GLOBAL	_GetWord
	GLOBAL	_PutWord
	GLOBAL	_ReadMem
	GLOBAL	_WriteMem
	GLOBAL	_PutString
	GLOBAL	_GetString
	GLOBAL	_StringLen
	GLOBAL	_IsValid

	GLOBAL	_breakpoint
	GLOBAL	_leds
	GLOBAL	_calledBy
	GLOBAL	_IsHeader
;
;	bool IsHeader(char* addr)
;
_IsHeader:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=addr
	or	a		;CARRY=0
	ld	bc,HEADERS_START
	sbc	hl,bc
	ld	l,0FFH
	ret	nc
	inc	l
	ret
;
;	void	calledBy(void)

_calledBy:
	ld	l,(ix+4)
	ld	h,(ix+5)
	ret
;

;	void breakpoint(char code);

_breakpoint:
	pop	hl		;drop return
	push	ix
	pop	hl
	ld	(8000H),hl	;store IX at 8000H
	pop	hl		;L = to display
	ld	a,l
	ld	hl,leds
	or	(hl)
	out	(0),a		;display breakpoint code
	jp	$
;

leds:	defb	0

;	void leds(char led)

_leds:
	ld	hl,2
	add	hl,sp
	ld	a,(leds)
	or	(hl)
	ld	(leds),a
	out	(0),a
	ret
	
IF	TRACE

msgwcall:
	defm	'@%04x '
	defb	0

MACRO	WCALL
	pop	hl	;get ret addr
	push	hl	;put it back
	dec	hl
	dec	hl
	dec	hl
	push	hl	;call address
	ld	hl,msgwcall
	push	hl	;msg
	ld	hl,__iob+16
	push	hl	;file=stderr
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
ENDM

ENDIF

IF	RCHECK

msgwrong:
	defm	'Wrong parameter used at %04x'
	defb	0ah,0dh,0
;
;	HL to be checked
;	return addr is on stack
;
MACRO	CHECK_RANGE_SYM
	ld	a,(BOTTOM_SYM+1)
	dec	a
	cp	h
	jr	c,999f
	ld	(8002h),hl	;save wrong addr
	pop	hl		;ret addr
	push	hl
	ld	hl,msgwrong
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	ld	(8000h),ix	;save IX
	jr	$
999:
ENDM
;
;	HL to be checked
;	return addr is on stack
;
MACRO	CHECK_RANGE_STR
	ld	a,(TOP_STRINGS+1)
	cp	h
	jr	nc,888f
	ld	(8002h),hl	;save wrong addr
	pop	hl		;ret addr
	push	hl
	ld	hl,msgwrong
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	ld	(8000h),ix	;save IX
	jr	$
888:
ENDM

ENDIF

IF	TRAP

msgtrap:
	defm	'Trap at %04x'
	defb	0ah,0dh,0

TARGET:	defw	0H		;must be filled with the target address
COUNT:	defb	1		;must be filled with the iterations counter
;
;	HL to be checked
;	return addr is on stack
;
MACRO	CHECK_TRAP
	ld	a,(TARGET)
	cp	l
	jr	nz,999f
	ld	a,(TARGET+1)
	cp	h
	jr	nz,999f
	ld	a,(COUNT)
	dec	a
	ld	(COUNT),a
	jr	nz,999f
	pop	hl		;ret addr
	push	hl
	ld	hl,msgtrap
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	ld	(8000h),ix	;save IX
	jr	$
999:
ENDM

ENDIF

;
;	Upper 64KB RAM structure
;
;<         strings>		<symbols       ><hashtab headers>	<shadow code>
;                  ^		^               ^                       ^
;		   |            |		|			|
;        	   TOP_STRINGS	BOTTOM_SYM	HEADERS_START		HEADERS_END
;
HEADERS_COUNT	equ	271

HEADERS_END	equ	0FF63H
HEADERS_START	equ	HEADERS_END - ((HEADERS_COUNT + 1) * 4)

DYNM_START	equ	1
DYNM_END	equ	HEADERS_START

$ReadByte 	equ 	0FF63H
$WriteByte	equ 	0FF6DH
$ReadWord	equ 	0FF77H
$WriteWord	equ 	0FF83H
$WriteString	equ 	0FF8FH
$ReadString	equ 	0FFA1H
$StringLen	equ	0FFB2H
$ReadMem	equ	0FFC7H
$WriteMem	equ	0FFD7H
$FillZero	equ	0FFE7H

FREE		equ	0
ALLOCATED	equ	0FFH

;
;	allocated/free bloc header structure
;
STATUS		equ	0	;+0	byte - status (0=free,FF=allocated)
				;+1	data - 27 bytes
;
HEADERSIZE	equ	1
BLOCKSIZE	equ	28	;1+27
;
TOP_STRINGS:	defw	DYNM_START	;top strings area (must be < BOTTOM_SYM)
BOTTOM_SYM:	defw	DYNM_END	;bottom symbols area (must be > TOP_STRINGS)

;

MACRO	PutByte
	call	$WriteByte
ENDM

MACRO	GetByte
	call	$ReadByte
ENDM

MACRO	PutWord
	call	$WriteWord
ENDM

MACRO	GetWord
	call	$ReadWord
ENDM

;
;	Get_hashtab(int index)
;
	GLOBAL	_Get_hashtab
;
_Get_hashtab:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL = index
gethdr:	add	hl,hl
	add	hl,hl		;HL=index*4
	ld	bc,HEADERS_START
	add	hl,bc		;HL=hashtab addr
	ret
;
;	Get_SymList(void)
;
	GLOBAL	_Get_SymList
;
_Get_SymList:
	ld	hl,HEADERS_COUNT
	jr	gethdr
;
;	Total allocated space
;
_TotalSize:
	ld	hl,DYNM_END
	ld	bc,(BOTTOM_SYM)
	sbc	hl,bc			;HL=symbols alloc size
	ex	de,hl			;DE=symbols alloc size
	ld	hl,(TOP_STRINGS)	;HL=strings alloc size
	add	hl,de			;HL=total alloc size
	ret
;	
;	Alloc symbol in Upper 64KB RAM
;
;	returns HL=pointer of data part (or NULL if no more available memory)
;
AllocDynSym:
	ld	a,(TOP_STRINGS+1)	;get top of allocated strings area
	inc	a
	ld	hl,(BOTTOM_SYM)		;compare to bottom of allocated symbols area
	cp	h
	jr	z,full			
	or	a			;CARRY=0
	ld	bc,BLOCKSIZE		;size of data
	sbc	hl,bc
	ld	(BOTTOM_SYM),hl		;update bottom of allocated symbols area
	ld	c,ALLOCATED		;mark status = allocated
	PutByte
	inc	hl
	ret				;return data pointer
full:
	ld	hl,0			;return zero
	ret	
;
;	myalloc
;
;void*	myallocsym(void)
;
;	returns addr of allocated data block
;		or NULL
IF	TRACE
msgal:	defm	'alloc symbol returned=%04x'
	defb	0ah,0dh,0
ENDIF
_myallocsym:
IF	TRACE
	WCALL
	call	AllocDynSym
	push	hl
	ld	hl,msgal
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	ret
ELSE
	jp	AllocDynSym
ENDIF
;	
;	Alloc string in Upper 64KB RAM
;
;	BC=string len + 1
;	returns HL=pointer of data part (or NULL if no more available memory)
;
AllocDynStr:
	ld	hl,(TOP_STRINGS)	;get top of allocated strings area
	ld	a,(BOTTOM_SYM+1)	;compare to bottom of allocated symbols area
	dec	a
	cp	h
	jr	z,fullstr
	push	hl			
	add	hl,bc
	ld	(TOP_STRINGS),hl	;update top of allocated strings area
	pop	hl
	ret				;return data pointer
fullstr:
	ld	hl,0			;return zero
	ret	
;
;	myallocstr
;
;void*	myallocstr(int size)
;
;	returns addr of allocated data block
;		or NULL
IF	TRACE
msgalstr:defm	'alloc string returned=%04x'
	defb	0ah,0dh,0
ENDIF
_myallocstr:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=size
IF	TRACE
	call	AllocDynStr
	push	hl
	ld	hl,msgalstr
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	ret
ELSE
	jp	AllocDynStr
ENDIF

IF	SC108

MEMP_PORT       equ     38H

;       ROM     0000 to 8000H
;
ROM_OUT_CMD     equ     00000001B
ROM_IN_CMD      equ     00000000B
LOWER_64RAM     equ     00000000B
UPPER_64RAM     equ     10000000B
;
MACRO	LOW_RAM
	ld	a,LOWER_64RAM .or. ROM_OUT_CMD
	out	(MEMP_PORT),a
ENDM

MACRO	UP_RAM
	ld	a,UPPER_64RAM .or. ROM_OUT_CMD
	out	(MEMP_PORT),a	
ENDM

MACRO	ROM_IN
	ld	a,LOWER_64RAM .or. ROM_IN_CMD
	out	(MEMP_PORT),a
ENDM

MACRO	ROM_OUT
	ld	a,LOWER_64RAM .or. ROM_OUT_CMD
	out	(MEMP_PORT),a
ENDM

ELSE

MM_RAM_P	equ	30H

MM_UP_RAM	equ	1
MM_LOW_RAM	equ	0

MACRO	LOW_RAM
	ld	a,MM_LOW_RAM
	out	(MM_RAM_P),a
ENDM

MACRO	UP_RAM
	ld	a,MM_UP_RAM
	out	(MM_RAM_P),a
ENDM

MM_ROM_P	equ	38H

MM_ROM_IN	equ	0
MM_ROM_OUT	equ	1

MACRO	ROM_IN
	ld	a,MM_ROM_IN
	out	(MM_ROM_P),a
ENDM

MACRO	ROM_OUT
	ld	a,MM_ROM_OUT
	out	(MM_ROM_P),a
ENDM

ENDIF

	psect	top
;
;	Init dynamic memory
;
_InitDynM:
	push	ix			;save IX
	call	StoreShared		;setup shadow routines
					;init upper ram with 0
	ld	hl,DYNM_START
	ld	de,DYNM_END-DYNM_START
	ld	c,0
loopi:	PutByte
	inc	hl
	dec	de
	ld	a,d
	or	e
	jr	nz,loopi
	pop	ix			;restore IX
	ret

;
;	Store shared code to Upper & Lower RAM
;
;	SP must be set at top of TPA 
;
StoreShared:
				;move shared code to Lower RAM
	ld	de,SHARED	;to
	ld	hl,SHARED_START	;from
	ld	bc,SHARED_END - SHARED_START
	ldir
				;move "mover" to 0D000H
	ld	de,0D000H	;to
	ld	hl,mover_start	;from
	ld	bc,mover_end - mover_start
	ldir
	jp	0D000H		;call "mover" to store shared code to Upper RAM and return
;
mover_start:
				;move shared code to Upper RAM
	ROM_IN
				;first search at 7F00H
	ld	hl,7F00H
	ld	a,(hl)
	cp	0c3H
	jr	nz,searchscm
	inc	hl
	inc	hl
	inc	hl
	ld	a,(hl)
	cp	0c3H
	jr	nz,searchscm
				;found "mover" in CPM BOOT EPROM
	inc	hl
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=address of "move 1 byte to upper RAM" routine
	jp	moveit
;
searchscm:
				;search "move 1 byte to upper RAM" routine in SCM
	ld	hl,69H
searchjp:
	ld	a,(hl)
	cp	0c3H
	jr	z,jpfound
	inc	hl
	jr	searchjp
jpfound:
	ld	b,6
searchj7thjp:
	inc	hl
	inc	hl
	inc	hl
	ld	a,(hl)
	cp	0c3H
	jr	nz,searchjp
	djnz	searchj7thjp
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl
	ld	bc,2bH
	add	hl,bc
	add	hl,bc
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl		;HL=LTOUP_SCM
moveit:				
	push	hl
	pop	iy		;IY=SCM function to move A --> UpperRAM in (DE)
	ld	ix,SHARED	;from
	ld	de,SHARED	;to
	ld	a,SHARED_END - SHARED_START
looptoup:
	push	af
	ld	a,(ix+0)
	ld	hl,retadr - mover_start + 0D000H
	push	hl
	jp	(iy)
retadr:	inc	ix
	inc	de
	pop	af
	dec	a
	jr	nz,looptoup
	ROM_OUT
	ret
;
mover_end:

;---------------------------------------------------------------------------------------
;	stored at 0FF63H in both Lower & Upper RAM
;
SHARED		equ	HEADERS_END

SHARED_START:
;
;	Read one byte from Upper 64KB RAM
;
;	HL=address of the byte
;
;	returns C=byte
;
;	affects reg A
;
$$ReadByte:
	UP_RAM
	ld	c,(hl)
	LOW_RAM
	ret
;
;	Write one byte to Upper 64KB RAM
;
;	HL=address of the byte
;	C=byte
;
;	affects reg A
;
$$WriteByte:
	UP_RAM
	ld	(hl),c
	LOW_RAM
	ret
;
;	Read one word from Upper 64KB RAM
;
;	HL=address of the byte
;
;	returns BC=word
;
;	affects reg A,HL
;
$$ReadWord:
	UP_RAM
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	LOW_RAM
	ret
;
;	Write one word to Upper 64KB RAM
;
;	HL=address of the byte
;	BC=word
;
;	affects reg A
;
$$WriteWord:
	UP_RAM
	ld	(hl),c
	inc	hl
	ld	(hl),b
	LOW_RAM
	ret
;
;	WriteString to Upper 64KB RAM
;
;	DE=source (in Lower RAM)
;	HL=destination (in Upper RAM)
;
;	Reg C affected
;
$$WriteString:
	ld	a,(de)
	ld	c,a
	UP_RAM
	ld	(hl),c
	LOW_RAM
	ld	a,c
	or	a
	ret	z
	inc	de
	inc	hl
	jr	$$WriteString
;
;	ReadString from Upper 64KB RAM
;
;	HL=source (in Upper RAM)
;	DE=destination (in Lower RAM)
;
;	Reg C affected
;
$$ReadString:
	UP_RAM
	ld	c,(hl)
	LOW_RAM
	ld	a,c
	ld	(de),a
	or	a
	ret	z
	inc	de
	inc	hl
	jr	$$ReadString
;
;	StringLen from Upper 64KB RAM
;
;	HL = string (in Upper RAM)
;
$$StringLen:
	ld	b,0FFH
$$loop:	UP_RAM
	ld	c,(hl)
	LOW_RAM
	inc	hl
	inc	b
	ld	a,c
	or	a
	jr	nz,$$loop
	ld	h,0
	ld	l,b
	ret
;
;	Read up to 256 bytes from the Upper 64KB RAM
;
;	HL = source (in Upper RAM)
;	DE = dest (in Lower RAM)
;	B = count (256 if 0)
;
;	returns HL=HL+B
;		DE=DE+B
;
$$ReadMem:
	UP_RAM
	ld	c,(hl)
	LOW_RAM
	ld	a,c
	ld	(de),a
	inc	hl
	inc	de
	djnz	$$ReadMem
	ret
;
;	Write up to 256 bytes from the Upper 64KB RAM
;
;	HL = dest (in Upper RAM)
;	DE = source (in Lower RAM)
;	B = count (256 if 0)
;
;	returns HL=HL+B
;		DE=DE+B
;
$$WriteMem:
	ld	a,(de)
	ld	c,a
	UP_RAM
	ld	(hl),c
	LOW_RAM
	inc	hl
	inc	de
	djnz	$$WriteMem
	ret
;
;	Fill with 00H in Upper 64KB RAM
;
;	HL = start
;	B = count
;
$$FillZero:
	UP_RAM
	xor	a
1:	ld	(hl),a
	inc	hl
	djnz	1b
	LOW_RAM
	ret
;	
SHARED_END:			;at 0FFF6H
;---------------------------------------------------------------------------------------

	psect	text
;
;	short	StringLen(char* source)
;
;	source is in Upper RAM
;
IF	TRACE
sls:	defs	2
msgsl:	defm	'StringLen s=%04x r %04x'
	defb	0ah,0dh,0
ENDIF
_StringLen:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source in Upper RAM
IF	RCHECK
	CHECK_RANGE_STR
ENDIF
IF	TRACE
	ld	(sls),hl
	call	$StringLen
	push	hl
	ld	hl,(sls)
	push	hl
	ld	hl,msgsl
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	pop	hl
	ret
ELSE
	jp	$StringLen
ENDIF
;
;	GetString(char* dest, char* source)
;
;	source is in Upper RAM
;	destination is in Lower RAM
;
IF	TRACE
gsd:	defs	2
gss:	defs	2
msggs:	defm	'GetString d=%04x s=%04x'
	defb	0ah,0dh,0
ENDIF
_GetString:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=dest in Lower RAM
	inc	hl
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source in Upper RAM
IF	RCHECK
	CHECK_RANGE_STR
ENDIF
IF	TRACE
	ld	(gss),hl
	ld	(gsd),de
	call	$ReadString
	ld	hl,(gss)
	push	hl
	ld	hl,(gsd)
	push	hl
	ld	hl,msggs
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	pop	hl
	ret
ELSE
	jp	$ReadString
ENDIF
;
;	PutString(char* source, char* dest)
;
;	source is in Lower RAM
;	destination is in Upper RAM
;
IF	TRACE
psd:	defs	2
pss:	defs	2
msgps:	defm	'PutString s=%04x d=%04x'
	defb	0ah,0dh,0
ENDIF
_PutString:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=source in Lower RAM
	inc	hl
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=dest in Upper RAM
IF	RCHECK
	CHECK_RANGE_STR
ENDIF
IF	TRACE
	ld	(psd),hl
	ld	(pss),de
	call	$WriteString
	ld	hl,(psd)
	push	hl
	ld	hl,(pss)
	push	hl
	ld	hl,msgps
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	pop	hl
	ret
ELSE
	jp	$WriteString
ENDIF
;
;	PutByte(char* dest, char b)
;
;	dest is in Upper RAM
;
IF	TRACE
pbd:	defs	2
pbb:	defs	1
msgpb:	defm	'PB d=%04x b=%02x'
	defb	0ah,0dh,0
ENDIF
_PutByte:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	ex	de,hl		;HL=dest, C=byte
IF	TRAP
	CHECK_TRAP
ENDIF
IF	RCHECK
	CHECK_RANGE_SYM
ENDIF
IF	TRACE
	ld	(pbd),hl
	ld	a,c
	ld	(pbb),a
	call	$WriteByte
	ld	a,(pbb)
	ld	l,a
	ld	h,0
	push	hl
	ld	hl,(pbd)
	push	hl
	ld	hl,msgpb
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	pop	hl
	ret
ELSE
	jp	$WriteByte
ENDIF
;
;char	GetByte(char* source)
;
;	source is in Upper RAM
;
IF	TRACE
gbs:	defs	2
gbb:	defs	1
msggb:	defm	'GB s=%04x r %02x'
	defb	0ah,0dh,0
ENDIF
_GetByte:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source
IF	RCHECK
	CHECK_RANGE_SYM
ENDIF
IF	TRACE
	ld	(gbs),hl
	call	$ReadByte	;C=byte
	ld	l,c
	push	bc
	ld	hl,(gbs)
	push	hl
	ld	hl,msggb
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	pop	hl
	ret
ELSE
	call	$ReadByte
	ld	l,c
	ret
ENDIF
;
;	PutWord(char* dest, short w)
;
;	dest is in Upper RAM
;
IF	TRACE
pwd:	defs	2
pww:	defs	2
msgpw:	defm	'PW d=%04x w=%04x'
	defb	0ah,0dh,0
ENDIF
_PutWord:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)	
	ex	de,hl		;HL=dest, BC=word
IF	TRAP
	CHECK_TRAP
ENDIF
IF	RCHECK
	CHECK_RANGE_SYM
ENDIF
IF	TRACE
	ld	(pwd),hl
	ld	(pww),bc
	call	$WriteWord
	ld	hl,(pww)
	push	hl
	ld	hl,(pwd)
	push	hl
	ld	hl,msgpw
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	pop	hl
	ret
ELSE
	jp	$WriteWord
ENDIF
;
;short	GetWord(char* source)
;
;	source is in Upper RAM
;
IF	TRACE
gws:	defs	2
gww:	defs	2
msggw:	defm	'GW s=%04x r %04x'
	defb	0ah,0dh,0
ENDIF
_GetWord:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source
IF	RCHECK
	CHECK_RANGE_SYM
ENDIF
IF	TRACE
	ld	(gws),hl
	call	$ReadWord	;BC=word
	ld	h,b
	ld	l,c
	push	bc
	ld	hl,(gws)
	push	hl
	ld	hl,msggw
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	pop	hl
	ret
ELSE
	call	$ReadWord
	ld	h,b
	ld	l,c
	ret
ENDIF
;
;	ReadMem(char* dest, char* source, short count)
;
;	source is in Upper RAM
;	destination is in Lower RAM
;	count <= 256
;
IF	TRACE
rms:	defs	2
rmd:	defs	2
msgrm:	defm	'RM d=%04x s=%04x'
	defb	0ah,0dh,0
ENDIF
_ReadMem:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=dest
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=source
	inc	hl
	ld	a,(hl)		;A=count
	ld	h,b
	ld	l,c		;HL=source
	ld	b,a		;B=count
IF	RCHECK
	CHECK_RANGE_SYM
ENDIF
IF	TRACE
	ld	(rms),hl
	ld	(rmd),de
	call	$ReadMem
	ld	hl,(rms)
	push	hl
	ld	hl,(rmd)
	push	hl
	ld	hl,msgrm
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl		
	pop	hl
	ret
ELSE
	jp	$ReadMem
ENDIF
;
;	WriteMem(char* source, char* dest, short count)
;
;	source is in Lower RAM
;	destination is in Upper RAM
;	count <= 256
;
IF	TRACE
wms:	defs	2
wmd:	defs	2
msgwm:	defm	'WM s=%04x d=%04x'
	defb	0ah,0dh,0
ENDIF
_WriteMem:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=source
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=dest
	inc	hl
	ld	a,(hl)		;A=count
	ld	h,b
	ld	l,c		;HL=dest
	ld	b,a		;B=count
IF	TRAP
	CHECK_TRAP
ENDIF
IF	RCHECK
	CHECK_RANGE_SYM
ENDIF
IF	TRACE
	ld	(wms),de
	ld	(wmd),hl
	call	$WriteMem
	ld	hl,(wmd)
	push	hl
	ld	hl,(wms)
	push	hl
	ld	hl,msgwm
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	pop	hl
	ret
ELSE
	jp	$WriteMem
ENDIF
;
;	IsValid - is the address allocated?
;
;	short IsValid(char* addr)
;
_IsValid:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=addr
	ld	a,(BOTTOM_SYM+1)	;is addr in the allocated area?
	dec	a
	cp	h
	jr	nc,invalid	;no, so is invalid...
				;yes, see marker
	dec	hl
	call	$ReadByte	;C=0FFH if valid
	ld	a,c
	ld	l,0FFH		;prepare return=TRUE
	inc	a		;zero if valid
	ret	z		;return L=0FFH if valid
invalid:
	ld	l,0
	ret			;return L=zero if invalid
;
