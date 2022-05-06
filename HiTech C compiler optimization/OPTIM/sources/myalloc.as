SC108	equ	1	;1=SC108, 0=32KB ROM + 128KB RAM Phillip Stevens memory module 
;
TRACE	equ	0	;1=print a message when entering a routine
RCHECK	equ	1	;1=checks range of addresses
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
	GLOBAL	_myalloc
	GLOBAL	_myfree

	GLOBAL	_GetString
	GLOBAL	_PutString
	GLOBAL	_GetByte
	GLOBAL	_PutByte
	GLOBAL	_GetWord
	GLOBAL	_PutWord
	GLOBAL	_StringLen
	GLOBAL	_ReadMem
	GLOBAL	_WriteMem
	GLOBAL	_zerofill
	GLOBAL	_IsValid
;
COND	DEBUG

	GLOBAL	_pr_message
	GLOBAL	_SwitchTrace
_SwitchTrace:
	ld	c,1
	call	5
	sub	30h
	ld	(TraceFlag),a
	ret
;
TraceFlag:defb	1

ENDC

COND	RCHECK

msgwrong:
	defm	'Wrong parameter used at %04x'
	defb	0
;
;	HL to be checked
;	return addr is on stack
;
MACRO	CHECK_RANGE
	ld	a,(TOP+1)
	dec	a
	cp	h
	jr	c,999f
	pop	hl		;ret addr
	push	hl
	ld	hl,msgwrong
	push	hl
	call	_pr_message
	jr	$
999:
ENDM

ENDC

COND	TRAP

msgtrap:
	defm	'Trap at %04x'
	defb	0

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
	call	_pr_message
	jr	$
999:
ENDM

ENDC

DYNM_START	equ	1
DYNM_END	equ	0FF63H

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

FreeBHeader	equ	0FFFAH

;
FREE		equ	0
ALLOCATED	equ	0FFH
;
;	allocated/free bloc header structure (1 bytes)
;
HEADERSIZE	equ	1
BLOCKSIZE	equ	7
;
STATUS		equ	0	;+0	byte - status (0=free,FF=allocated)
				;+1	data - 6 bytes
;
TOP:		defw	DYNM_END	;top available space
;
FreeSlotsCount:	defw	0		;free slots counter
;
FreePointer:	defw	DYNM_END	;pointer of first available free slot	
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
;	Total allocated space
;
_TotalSize:
	ld	hl,DYNM_END
	ld	bc,(TOP)
	sbc	hl,bc
	ex	de,hl			;DE=alloc size
	ld	hl,(FreeSlotsCount)
	ld	b,h	
	ld	c,l			;*1
	add	hl,hl			;*2
	add	hl,hl			;*4
	add	hl,hl			;*8
	sbc	hl,bc			;*7
	ex	de,hl			;HL=alloc size, DE=free size
	sbc	hl,de
	ret
;
;	Init dynamic memory
;
_InitDynM:
	push	ix			;save IX
	call	StoreShared		;setup shadow routines
	ld	hl,DYNM_END - 1		;fill with zero
	ld	c,0
	call	$FillZero
	pop	ix			;restore IX
	ret
;	
;	Alloc dynamic memory in Upper 64KB RAM
;
;	returns HL=pointer of data part (or NULL if no more available memory)
;
AllocDyn:
	ld	hl,(TOP)		;get top of available memory
	ld	a,h
	or	a			;if < 100H			
	jr	z,seefree			
					;CARRY=0
	ld	bc,BLOCKSIZE		;size of data + status byte
	sbc	hl,bc
	ld	(TOP),hl
	ld	c,ALLOCATED		;mark status = allocated
	PutByte
	inc	hl
	ret				;return data pointer
seefree:				;try to search free slots
	ld	hl,(FreeSlotsCount)
	ld	a,l
	or	h			;any free slot?
	ret	z			;no any more, return NULL
	dec	hl			;yes, decrement their count
	ld	(FreeSlotsCount),hl
	ld	hl,(FreePointer)	;and start searching for the first available
loopa:	GetByte
	ld	a,c
	or	a
	jr	z,found	
	ld	bc,BLOCKSIZE
	add	hl,bc
	jr	loopa
found:					;found free slot
	ld	c,ALLOCATED		;mark status = allocated
	PutByte
	inc	hl
	ld	d,h
	ld	e,l			;DE=data pointer
	ld	bc,BLOCKSIZE-1
	add	hl,bc
	ld	(FreePointer),hl	;save next block as pointer of first available free slot
	ex	de,hl			;return HL=data pointer
	ret
;
;	myalloc
;
;void*	myalloc(void)
;
;	returns addr of allocated data (6 bytes) 
;		or NULL
COND	TRACE
msgal:	defm	'alloc returned=%04x'
	defb	0
ENDC
_myalloc:
COND	TRACE
	call	AllocDyn
	ld	a,(TraceFlag)
	or	a
	ret	z
	push	hl
	ld	hl,msgal
	push	hl
	call	_pr_message
	pop	hl
	pop	hl
	ret
ENDC
COND	1-TRACE
	jp	AllocDyn
ENDC
;
;	myfree
;
;	marks block as free
;	then checks next blocks
;	and moves the TOP if possible
;
;void	myfree(void* buf)
;
COND	TRACE
msgfr:	defm	'free %04x'
	defb	0
ENDC
_myfree:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl			;HL=pointer to data
COND	TRACE
	ld	a,(TraceFlag)
	or	a
	jr	z,nomsg
	push	hl
	ld	hl,msgfr
	push	hl
	call	_pr_message
	pop	hl
	pop	hl
nomsg:
ENDC
	dec	hl			;HL=pointer to block
	xor	a			;CARRY=0
	ld	c,a			;mark block as free
	PutByte
	ld	d,h
	ld	e,l			;HL=DE=pointer to block
	ld	bc,(TOP)
	sbc	hl,bc			;block is at top?
	jr	z,loopf
					;CARRY=0
	ld	hl,(FreeSlotsCount)	;not at top, increment free slots counter
	inc	hl
	ld	(FreeSlotsCount),hl
	ld	hl,(FreePointer)	;then adjust Free Pointer
	sbc	hl,de
	ret	c
	ld	(FreePointer),de
	ret
					;freed block was at top
loopf:					;DE=block pointer
	ld	hl,BLOCKSIZE		;yes...
	add	hl,de			;CARRY=0
	ld	(TOP),hl		;set new top
	ex	de,hl
	ld	hl,DYNM_END		;end reached?
	sbc	hl,de
	ret	z			;return if end reached
	ex	de,hl			
	GetByte				;check next block
	ex	de,hl			;DE=pointer to block
	ld	a,c
	or	a			;is it free?
	ret	nz			;not free, return
	ld	hl,(FreeSlotsCount)	;if yes, decrement free slots counter
	dec	hl
	ld	(FreeSlotsCount),hl
	jr	loopf			;and keep checking
;
COND	SC108

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

ENDC

COND	1-SC108

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

ENDC

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
				;move "mover" to 0A000H
	ld	de,0A000H	;to
	ld	hl,mover_start	;from
	ld	bc,mover_end - mover_start
	ldir
	jp	0A000H		;call "mover" to store shared code to Upper RAM and return
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
	ld	hl,retadr - mover_start + 0A000H
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
SHARED		equ	DYNM_END

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
;	Fill with 00H (from DYNM_START=0001H to DYNM_END=0FF63H) Upper 64KB RAM
;
;	HL = DYNM_END - 1 (0FF62H)
;	C = 0
;
$$FillZero:
	UP_RAM
	ld	(hl),c
	dec	hl
	ld	a,h
	or	l
	jr	nz,$$FillZero
	LOW_RAM
	ret
;	
SHARED_END:			;at 0FFF6H
;---------------------------------------------------------------------------------------
;
;	short	StringLen(char* source)
;
;	source is in Upper RAM
;
COND	TRACE
sls:	defs	2
msgsl:	defm	'StringLen s=%04x r %04x'
	defb	0
ENDC
_StringLen:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source in Upper RAM
COND	TRACE
	ld	(sls),hl
	call	$StringLen
	ld	a,(TraceFlag)
	or	a
	ret	z
	push	hl
	ld	hl,(sls)
	push	hl
	ld	hl,msgsl
	push	hl
	call	_pr_message
	pop	hl
	pop	hl
	pop	hl
	ret
ENDC
COND	1-TRACE
	jp	$StringLen
ENDC
;
;	GetString(char* dest, char* source)
;
;	source is in Upper RAM
;	destination is in Lower RAM
;
COND	TRACE
gsd:	defs	2
gss:	defs	2
msggs:	defm	'GetString d=%04x s=%04x'
	defb	0
ENDC
_GetString:
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
COND	TRACE
	ld	(gss),hl
	ld	(gsd),de
	call	$ReadString
	ld	a,(TraceFlag)
	or	a
	ret	z
	ld	hl,(gss)
	push	hl
	ld	hl,(gsd)
	push	hl
	ld	hl,msggs
	push	hl
	call	_pr_message
	pop	hl
	pop	hl
	pop	hl
	ret
ENDC
COND	1-TRACE
	jp	$ReadString
ENDC
;
;	PutString(char* source, char* dest)
;
;	source is in Lower RAM
;	destination is in Upper RAM
;
COND	TRACE
psd:	defs	2
pss:	defs	2
msgps:	defm	'PutString s=%04x d=%04x'
	defb	0
ENDC
_PutString:
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
COND	TRACE
	ld	(psd),hl
	ld	(pss),de
	call	$WriteString
	ld	a,(TraceFlag)
	or	a
	ret	z
	ld	hl,(psd)
	push	hl
	ld	hl,(pss)
	push	hl
	ld	hl,msgps
	push	hl
	call	_pr_message
	pop	hl
	pop	hl
	pop	hl
	ret
ENDC
COND	1-TRACE
	jp	$WriteString
ENDC
;
;	PutByte(char* dest, char b)
;
;	dest is in Upper RAM
;
COND	TRACE
pbd:	defs	2
pbb:	defs	1
msgpb:	defm	'PB d=%04x b=%02x'
	defb	0
ENDC
_PutByte:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	ex	de,hl		;HL=dest, C=byte
COND	TRAP
	CHECK_TRAP
ENDC
COND	RCHECK
	CHECK_RANGE
ENDC
COND	TRACE
	ld	(pbd),hl
	ld	a,c
	ld	(pbb),a
	call	$WriteByte
	ld	a,(TraceFlag)
	or	a
	ret	z
	ld	a,(pbb)
	ld	l,a
	ld	h,0
	push	hl
	ld	hl,(pbd)
	push	hl
	ld	hl,msgpb
	push	hl
	call	_pr_message
	pop	hl
	pop	hl
	pop	hl
	ret
ENDC
COND	1-TRACE
	jp	$WriteByte
ENDC
;
;char	GetByte(char* source)
;
;	source is in Upper RAM
;
COND	TRACE
gbs:	defs	2
gbb:	defs	1
msggb:	defm	'GB s=%04x r %02x'
	defb	0
ENDC
_GetByte:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source
COND	RCHECK
	CHECK_RANGE
ENDC
COND	TRACE
	ld	(gbs),hl
	call	$ReadByte	;C=byte
	ld	l,c
	ld	a,(TraceFlag)
	or	a
	ret	z
	push	bc
	ld	hl,(gbs)
	push	hl
	ld	hl,msggb
	push	hl
	call	_pr_message
	pop	hl
	pop	hl
	pop	hl
	ret
ENDC
COND	1-TRACE
	call	$ReadByte
	ld	l,c
	ret
ENDC
;
;	PutWord(char* dest, short w)
;
;	dest is in Upper RAM
;
COND	TRACE
pwd:	defs	2
pww:	defs	2
msgpw:	defm	'PW d=%04x w=%04x'
	defb	0
ENDC
_PutWord:
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
COND	TRAP
	CHECK_TRAP
ENDC
COND	RCHECK
	CHECK_RANGE
ENDC
COND	TRACE
	ld	(pwd),hl
	ld	(pww),bc
	call	$WriteWord
	ld	a,(TraceFlag)
	or	a
	ret	z
	ld	hl,(pww)
	push	hl
	ld	hl,(pwd)
	push	hl
	ld	hl,msgpw
	push	hl
	call	_pr_message
	pop	hl
	pop	hl
	pop	hl
	ret
ENDC
COND	1-TRACE
	jp	$WriteWord
ENDC
;
;short	GetWord(char* source)
;
;	source is in Upper RAM
;
COND	TRACE
gws:	defs	2
gww:	defs	2
msggw:	defm	'GW s=%04x r %04x'
	defb	0
ENDC
_GetWord:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source
COND	RCHECK
	CHECK_RANGE
ENDC
COND	TRACE
	ld	(gws),hl
	call	$ReadWord	;BC=word
	ld	h,b
	ld	l,c
	ld	a,(TraceFlag)
	or	a
	ret	z
	push	bc
	ld	hl,(gws)
	push	hl
	ld	hl,msggw
	push	hl
	call	_pr_message
	pop	hl
	pop	hl
	pop	hl
	ret
ENDC
COND	1-TRACE
	call	$ReadWord
	ld	h,b
	ld	l,c
	ret
ENDC
;
;	ReadMem(char* dest, char* source, short count)
;
;	source is in Upper RAM
;	destination is in Lower RAM
;	count <= 256
;
COND	TRACE
rms:	defs	2
rmd:	defs	2
msgrm:	defm	'RM d=%04x s=%04x'
	defb	0
ENDC
_ReadMem:
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
COND	RCHECK
	CHECK_RANGE
ENDC
COND	TRACE
	ld	(rms),hl
	ld	(rmd),de
	call	$ReadMem
	ld	a,(TraceFlag)
	or	a
	ret	z
	ld	hl,(rms)
	push	hl
	ld	hl,(rmd)
	push	hl
	ld	hl,msgrm
	push	hl
	call	_pr_message
	pop	hl
	pop	hl		
	pop	hl
	ret
ENDC
COND	1-TRACE
	jp	$ReadMem
ENDC
;
;	WriteMem(char* source, char* dest, short count)
;
;	source is in Lower RAM
;	destination is in Upper RAM
;	count <= 256
;
COND	TRACE
wms:	defs	2
wmd:	defs	2
msgwm:	defm	'WM s=%04x d=%04x'
	defb	0
ENDC
_WriteMem:
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
COND	TRAP
	CHECK_TRAP
ENDC
COND	RCHECK
	CHECK_RANGE
ENDC
COND	TRACE
	ld	(wms),de
	ld	(wmd),hl
	call	$WriteMem
	ld	a,(TraceFlag)
	or	a
	ret	z
	ld	hl,(wmd)
	push	hl
	ld	hl,(wms)
	push	hl
	ld	hl,msgwm
	push	hl
	call	_pr_message
	pop	hl
	pop	hl
	pop	hl
	ret
ENDC
COND	1-TRACE
	jp	$WriteMem
ENDC
;
;	Fill with zero a buffer
;
;	zerofill(char* buf, short len)
;
_zerofill:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=buf
	inc	hl
	ld	b,(hl)		;B=len
	ex	de,hl		;HL=buf
	xor	a
fzero:	ld	(hl),a
	inc	hl
	djnz	fzero
	ret
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
	ld	l,a
	dec	hl
	call	$ReadByte	;C=0FFH if valid
	ld	a,c
	ld	hl,0FFFFH	;prepare return=TRUE
	inc	a		;zero if valid
	ret	z		;return HL=0FFFFH if valid
	inc	hl
	ret			;else return HL=zero
;
