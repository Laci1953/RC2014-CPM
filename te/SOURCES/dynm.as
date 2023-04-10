;
;	Dynamic memory routines
;
;	for 128KB / 512KB RAM
;
;	Szilagyi Ladislau
;
;	March 2023

;	only one of ?2X64K , ?512K, ?Z80ALL must be set to 1
;
?2X64K	equ	1	;1 : SC108 or 32KB ROM + 128KB RAM Phillip Stevens memory module
SC108	equ	1	;1 : SC108, 0 : 32KB ROM + 128KB RAM Phillip Stevens memory module 

?512K	equ	0	;1 : 512KB RAM + 512KB EPROM
?Z80ALL equ	0	;1 : Z80ALL 4 x 32KB RAM

	GLOBAL	_InitDynM
	GLOBAL	_GetTotalFree
	GLOBAL	_Alloc
	GLOBAL	_Free
	GLOBAL	_GetString
	GLOBAL	_PutString
	GLOBAL	_StringLen
	GLOBAL	_GetByte
	GLOBAL	_PutByte
	GLOBAL	_GetWord
	GLOBAL	_PutWord

;-------------------------------------------------------------2X64K
IF	?2X64K

	GLOBAL	_malloc
	GLOBAL	_free

DYNM_START	equ	1
DYNM_END	equ	0FFA0H

$ReadByte 	equ 	0FFA0H
$WriteByte	equ 	0FFAAH
$ReadWord	equ 	0FFB4H
$WriteWord	equ 	0FFC0H
$WriteString	equ 	0FFCCH
$ReadString	equ 	0FFDEH
$StringLen	equ	0FFE9H

FreeBHeader	equ	0FFFAH
;	defs	2	;first free+NEXTFREE
;	defs	2	;last free+NEXTFREE
;
FREE		equ	0
ALLOCATED	equ	0FFH
;
;	allocated/free bloc header structure (11 bytes)
;
HEADERSIZE	equ	9
;
NEXTBLOCK	equ	0	;+0	word - link to next block (0=EOL)
PREVBLOCK	equ	2	;+2	word - link to prev block (0=EOL)
NEXTFREE	equ	4	;+4	word - link to next free+NEXTFREE (or FreeBHeader=EOL)
PREVFREE	equ	6	;+6	word - link to prev free+NEXTFREE (or FreeBHeader=EOL)
STATUS		equ	8	;+8	byte - status (0=free,FF=allocated)
				;+9	data
;
;	HL = block
;
;	calculate BC=SIZE of block
;	HL not affected
;	DE affected
;
MACRO	GetSIZE
	call	$ReadWord
	dec	hl
	ld	a,b
	or	c		;carry=0
	jr	nz,1f
	ld	bc,DYNM_END
1:
	ex	de,hl
	ld	h,b
	ld	l,c
	sbc	hl,de
	ld	b,h
	ld	c,l
	ex	de,hl
ENDM

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

;-------------------------------------------------------------2X64K
ELSE
;-------------------------------------------------------------512K or Z80ALL

IF	?512K
Buf16K		equ	8000H	;address of dynamic memory bank
BANKS_CNT	equ	28	;28 banks of 16KB each are available
MAX_SIZE	equ	10	;max block = 4000H
ELSE
Buf32K		equ	0000H	;address of dynamic memory bank
BANKS_CNT	equ	2	;2 banks of 32KB each are available
MAX_SIZE	equ	11	;max block = 8000H
ENDIF

;
; Memory block structure
;
; 	defw	next 	;pointer to next block
;	defw	prev 	;pointer to prev block
;	defb	status 	;0=available, CrtID=allocated
;	defb	size	;block size : from 0(=10H) to MAX_SIZE(=4000H or 8000H)
;	data
;
OFF_STS		equ	4
OFF_SIZE	equ	5
;

LISTS_NR	equ	MAX_SIZE+1
AVAILABLE	equ	0

	psect bss
;
;	Available block list headers
;
L0:	defs	4*BANKS_CNT	;L0 bank0,L0 bank1,...L0 bank27
L1:	defs	4*BANKS_CNT
L2:	defs	4*BANKS_CNT
L3:	defs	4*BANKS_CNT
L4:	defs	4*BANKS_CNT
L5:	defs	4*BANKS_CNT
L6:	defs	4*BANKS_CNT
L7:	defs	4*BANKS_CNT
L8:	defs	4*BANKS_CNT
L9:	defs	4*BANKS_CNT
L10:	defs	4*BANKS_CNT
IF	?Z80ALL
L11:	defs	4*BANKS_CNT
ENDIF
;
Lists:	defs	LISTS_NR*2*BANKS_CNT	;bank0(L0,L1,...L10),bank1(L0+4,L1+4,...L10+4)... 
;
Buddy:	defs	LISTS_NR*2
;
;	Maximum Available
;	contains (Size+1) if available, or zero if unavailable
;
MaxAv:	defs	BANKS_CNT

;-------------------------------------------------------------512K or Z80ALL
ENDIF

IF	?Z80ALL
	psect	top
ELSE
	psect	text
ENDIF
;
;	Init dynamic memory
;
;*****************************************************************
;void	InitDynM(void);
;*****************************************************************
;
;-------------------------------------------------------------2X64K
IF	?2X64K

_InitDynM:
	push	ix			;save IX
	call	StoreShared		;setup shadow routines
	ld	hl,DYNM_START+NEXTFREE	;init free blocks header
	ld	(FreeBHeader),hl
	ld	(FreeBHeader+2),hl
					;init big block
	ld	bc,0			;EOL
	ld	hl,DYNM_START		;stored at NEXTBLOCK
	PutWord		
	inc	hl			;and at PREVBLOCK
	PutWord		
	inc	hl
	ld	bc,FreeBHeader		;EOL
	PutWord				;stored at NEXTFREE
	inc	hl
	PutWord				;and at PREVFREE
	inc	hl
	ld	c,FREE			;0=free
	PutByte				;stored at STATUS
	pop	ix			;restore IX
	ret
;
;	Split free block
;
;	HL=block addr
;	DE=to-be-allocated block size (including header)
;
;	if block size - (to-be-allocated block size) <= HEADERSIZE,
;		remove block from free block list
;	else
;		split block in two parts
;		low part remains free, with next block link updated
;		high part is allocated, with block links set, status=allocated
;
;	returns HL=allocated block addr
;	
SplitFree:
	push	de
	GetSIZE				;BC=size
	pop	de		
	push	hl			;block addr on stack
	ld	h,b
	ld	l,c			;HL=block size
	xor	a			;carry=0
	sbc	hl,de			;subtract requested size
	ld	bc,HEADERSIZE+1
	sbc	hl,bc			;if block size - (to-be-allocated block size) < HEADERSIZE,
	jp	c,full			;	 alloc the full block

					;split the block

	add	hl,bc			;HL=remaining size
	ld	b,h
	ld	c,l			;BC=remaining size

	pop	hl			;HL=block addr
	push	hl			;back on stack
	add	hl,bc			;HL=high part of the block, to-be-allocated
	ld	b,h
	ld	c,l			;BC=high part of the block, to-be-allocated
	pop	hl			;HL=block addr
	push	bc			;to-be-allocated on stack
	GetWord				;BC=next block
	ld	d,b
	ld	e,c			;DE=next block
	pop	bc			;BC=high part of the block, to-be-allocated
	dec	hl			;HL=block addr
	PutWord				;set BC as NEXTBLOCK for the low (free) part of the block
					;for the low (free) part of the block,
					;PREVBLOCK, STATUS,NEXTFREE,PREVFREE links remain unchanged
					;now, fix the new to-be-allocated block header (BC=block addr, DE=size)
	dec	hl			;HL=block addr
	push	hl			;exchange BC <--> HL
	push	bc
	pop	hl
	pop	bc			;HL=to-be-allocated, BC=block, DE=next block
	push	hl			;to-be-allocated on stack
	inc	hl
	inc	hl
	PutWord				;set to-be-allocated PREVBLOCK link = block
	dec	hl
	dec	hl
	dec	hl
	ld	b,d
	ld	c,e			;BC=next block
	PutWord				;set to-be-allocated NEXTBLOCK link = next block
					
	ld	a,d			;next block is EOL?
	or	e
	jr	z,isEOL
					;no, must adjust next block prev link

	pop	bc			;BC=to-be-allocated
	push	bc
	ex	de,hl			;HL=next block
	inc	hl
	inc	hl
	PutWord				;set next block.prev = to-be-allocated
isEOL:
	pop	hl			;HL=to-be-allocated
	
setsts:
	push	hl
	ld	bc,STATUS
	add	hl,bc			;HL=to-be-allocated STATUS pointer
	ld	c,ALLOCATED		;allocated
	PutByte				;set to-be-allocated STATUS=allocated
	pop	hl			;HL=to-be-allocated block
	ret

full:					;transform full block from free to allocated

	pop	hl			;HL=block
	push	hl			;block on stack
	call	RemoveFree		;remove block from free list
					;mark block allocated
	pop	hl			;HL=block
	jr	setsts			;go mark allocated
;
;	Remove Free
;
;	removes block from the free blocks list
;
;	HL=block
;
RemoveFree:
	ld	bc,NEXTFREE
	add	hl,bc
	GetWord				;BC=nextfree+NEXTFREE
	ld	d,b
	ld	e,c			;DE=nextfree+NEXTFREE
	inc	hl
	GetWord				;BC=prevfree+NEXTFREE

					;is nextfree==header?

	ld	hl,FreeBHeader		;carry=0
	sbc	hl,de
	jr	nz,seeprev
					;yes, the block is the last one in the list, remove-it
					;BC=prev
	ld	hl,FreeBHeader+2
	ld	(hl),c
	inc	hl
	ld	(hl),b			;header.last=prev

	ld	h,b
	ld	l,c
	ld	bc,FreeBHeader
	PutWord				;prev.next=header
	ret
	
seeprev:				;is prevfree==header?

	ld	hl,FreeBHeader		;carry=0
	sbc	hl,bc
	jr	nz,remove
					;yes, the block is the first one in the list, remove-it
					;DE=next
	ld	hl,FreeBHeader
	ld	(hl),e
	inc	hl
	ld	(hl),d			;header.first=next

	ld	h,d
	ld	l,e
	inc	hl
	inc	hl
	ld	bc,FreeBHeader
	PutWord				;next.prev=header
	ret

remove:					;block is not the first or the last one, simply remove-it

					;store prevfree+NEXTFREE at nextfree+NEXTFREE.prev
	ld	hl,2
	add	hl,de			;HL=nextfree+NEXTFREE+2
	PutWord
					;store nextfree+NEXTFREE at prevfree+NEXTFREE.next
	ld	h,b
	ld	l,c			;HL=prevfree+NEXTFREE
	ld	b,d
	ld	c,e			;BC=nextfree+NEXTFREE
	PutWord
	ret
;
;	AddFree
;
;	adds block to the free blocks list as last one
;
;	HL=block
;
AddFree:
	ld	bc,NEXTFREE
	add	hl,bc		;HL=block+NEXTFREE
	push	hl		;on stack
				;check header
	ld	de,(FreeBHeader+2);DE=lastfree+NEXTFREE (or FreeBHeader)
	ld	hl,FreeBHeader
	xor	a		;carry=0
	sbc	hl,de
	jr	z,empty
				;list is not empty, DE=lastfree+NEXTFREE
				;set lastfree next link
	ld	h,d
	ld	l,e		;HL=lastfree+NEXTFREE
	pop	bc		;BC=block+NEXTFREE
	PutWord
				;set block prev link
	ld	h,b
	ld	l,c
	inc	hl
	inc	hl		;HL=pointer to block.PREVFREE
	push	bc		;block+NEXTFREE on stack
	ld	b,d
	ld	c,e		;BC=lastfree+NEXTFREE
	PutWord
				;set block next link
	pop	hl		;HL=block+NEXTFREE
	ld	bc,FreeBHeader
	PutWord
	dec	hl		;HL=block+NEXTFREE
	ld	(FreeBHeader+2),hl;set FreeBHeader.last
	ret
empty:				;free block list is empty, set new block as the only one
	pop	hl		;HL=block+NEXTFREE
	push	hl		;on stack
	ld	bc,FreeBHeader
	PutWord
	inc	hl
	PutWord
	pop	hl		;hl=block+NEXTFREE
	ld	(FreeBHeader),hl
	ld	(FreeBHeader+2),hl
	ret
;	
;	Alloc dynamic memory in Upper 64KB RAM
;
;	DE=size (without header)
;
;	returns HL (or NULL if no more available memory)
;
AllocDyn:
	ld	hl,HEADERSIZE
	add	hl,de		;add header size
	ex	de,hl		;DE=size+header
	ld	bc,(FreeBHeader);HL=first available block (or FreeBHeader=EOL)
alloc_loop:
	ld	hl,FreeBHeader
	xor	a		;carry=0
	sbc	hl,bc		;if equal to header,
	ret	z		;return-it, no more memory
				;BC=free block+NEXTFREE
	ld	hl,-NEXTFREE
	add	hl,bc		;HL=free block
	push	hl		;free block on stack
	push	de		;size on stack
	GetSIZE			;BC=block size
	pop	de		;DE=size
	ld	h,b
	ld	l,c		;HL=free block size
	xor	a		;carry=0
	sbc	hl,de
	pop	hl		;HL=free block
	jr	c,trynext
	jp	SplitFree	;free block is big enough, split-it
trynext:			;free block is too small, try to get next one
	ld	bc,NEXTFREE
	add	hl,bc
	GetWord			;BC=next free block+NEXTFREE (or FreeBHeader+NEXTFREE)
	jr	alloc_loop
;
;	Free dynamic memory
;
;	HL=addr of the header of block to be deallocated
;
FreeDyn:
	push	hl		;save block addr
	ld	bc,STATUS	;get block status
	add	hl,bc
	GetByte
	pop	hl		;restore block addr
	ld	a,c		;should be 0FFH
	inc	a
	ret	nz		;return if status is NOT ALLOCATED
	GetWord			;BC=next block
	dec	hl		;HL=block
	ld	a,c
	or	b
	jr	z,tryprev	;next block is EOL, so try to join with prev block
				;try to join with next block
	push	hl		;block on stack
	ld	h,b
	ld	l,c		;HL=next block, check its status
	push	hl		;next on stack
	ld	bc,STATUS
	add	hl,bc		;HL=next status pointer
	GetByte		
	ld	a,c
	or	a
	jp	z,nextisfree
				;next block not free
	pop	hl		;drop next
	pop	hl		;HL=block
	jp	tryprev		;so try to join the prev block
;
nextisfree:			;is free, so join with next free block
				;next link must be updated
				;prev link is not affected

	pop	hl		;HL=next
	push	hl		;back on stack
	call	RemoveFree	;remove next from free blocks list
	pop	hl		;HL=next
	GetWord			;BC=next.nextblock

	pop	hl		;HL=block
	PutWord			;set block next link	
	dec	hl		;HL=block		
	ld	a,b		;next.nextblock is null?
	or	c		
	jr	z,tryprev	;yes, see prev block
				;not null, set next block prev link (BC=next, HL=block)
	push	hl		;block on stack
	ld	d,b
	ld	e,c		;DE=next
	ld	b,h
	ld	c,l		;BC=block
	ex	de,hl		;HL=next
	inc	hl
	inc	hl
	PutWord			;next.prev = block	
	pop	hl		;HL=block
	
tryprev:			;try to join with prev block

	push	hl		;block on stack
	inc	hl
	inc	hl
	GetWord			;BC=prev block
	ld	a,b
	or	c
	jr	nz,prevnotnull
				;prev block is NULL
addtofree:
	pop	hl		;HL=block
	push	hl		;back on stack
	call	AddFree		;add block to free block's list as the last one
	pop	hl		;HL=block
	jp	markfree
;
prevnotnull:			;prev block is not null, check its status
	push	bc		;prev on stack
	ld	h,b
	ld	l,c		;HL=prev block
	ld	bc,STATUS
	add	hl,bc		;HL=prev status pointer
	GetByte		
	ld	a,c
	or	a
	pop	de		;DE=prev
	jr	nz,addtofree	;prev not free, so add block to the free list, mark-it free	

				;is free, so join with the previous free block
				;prev.next link must be updated
				;prev.prev link is not affected
				;prev free links remain unchanged
				;...so no need to add the block to the free block's list

				;DE=prev block 
	pop	hl		;HL=block
	GetWord			;BC=next
	ex	de,hl		;HL=prev
	PutWord			;set prev.next = next
	dec	hl		;HL=prev=block
	ld	a,b
	or	c		;next is null?
	jr	z,markfree	;yes, go mark free
	push	hl		;block on stack

				;no, set next block prev link (BC=next, HL=block)

	ld	d,b
	ld	e,c		;DE=next
	ld	b,h
	ld	c,l		;BC=block
	ex	de,hl		;HL=next
	inc	hl
	inc	hl
	PutWord			;next.prev = block	
	pop	hl		;HL=block

markfree:
	ld	bc,STATUS
	add	hl,bc
	ld	c,FREE		;free
	PutByte			;set status=free
	ret
;
;	stored at 0FFA0H in both Lower & Upper RAM
;
;ReadByte 	at FFA0H
;WriteByte	at FFAAH
;ReadWord	at FFB4H
;WriteWord	at FFC0H
;WriteString	at FFCCH
;ReadString	at FFDEH
;StringLen	at FFE9H

SHARED		equ	0FFA0H

;------------------------------------------------------SC108
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

;------------------------------------------------------SC108
ELSE
;------------------------------------------------------1-SC108

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
;------------------------------------------------------1-SC108
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
;	call	$$WriteByte - SHARED_START + SHARED
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
	call	$$ReadByte - SHARED_START + SHARED
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
$$loop:	call	$$ReadByte - SHARED_START + SHARED
	inc	hl
	inc	b
	ld	a,c
	or	a
	jr	nz,$$loop
	ld	h,0
	ld	l,b
	ret
SHARED_END:			;at 0FFF8H

;-------------------------------------------------------------2X64K
ELSE
;-------------------------------------------------------------512K or Z80ALL

_InitDynM:
	push	iy
				;init list headers for each bank
	ld	bc,BANKS_CNT * LISTS_NR	;total lists counter
	ld	hl,L0		;HL=first list header
initL:	
	ld	d,h		;DE=HL
	ld	e,l
	ld	(hl),e
	inc	hl
	ld	(hl),d
	inc	hl
	ld	(hl),e
	inc	hl
	ld	(hl),d
	inc	hl
	dec	bc
	ld	a,b
	or	c
	jr	nz,initL
				;init lists pointers for each bank
	ld	iy,Lists
	ld	a,BANKS_CNT
	ld	bc,4 * BANKS_CNT;delta for list headers
	ld	de,0		;offset in lists headers
initLHB:			;for each bank
	push	af
	ld	hl,L0
	add	hl,de
	ld	a,LISTS_NR
initLH:				
	ld	(iy+0),l
	ld	(iy+1),h
	inc	iy
	inc	iy
	add	hl,bc
	dec	a
	jr	nz,initLH
				;next bank
	inc	de		;increment offset in list headers
	inc	de
	inc	de
	inc	de	
	pop	af
	dec	a
	jr	nz,initLHB
				;init Buddy
	ld	hl,Buddy
	ld	de,10H
	ld	a,LISTS_NR
initBDY:ld	(hl),e
	inc	hl
	ld	(hl),d
	inc	hl
	ex	de,hl
	add	hl,hl
	ex	de,hl
	dec	a
	jr	nz,initBDY
				;init largest block list headers for each bank
	ld	b,BANKS_CNT
	xor	a
IF	?512K
	ld	hl,L10		;HL=L10 lists header
ELSE
	ld	hl,L11		;HL=L11 lists header
ENDIF
initLMAX:
	push	af
	push	bc
	push	hl
IF	?512K
	add	a,32+4
	out	(7AH),a		;select physical RAM bank number A in logical bank 2 (8000-C000)
	ld	de,Buf16K
	call	__AddToL	;add DE to HL header
	ld	hl,Buf16K+OFF_STS;HL=pointer of block status
ELSE
	inc	a
	out	(1FH),a		;select physical RAM bank number A in logical bank 0000-8000
	ld	de,Buf32K
	call	__AddToL	;add DE to HL header
	ld	hl,Buf32K+OFF_STS;HL=pointer of block status
ENDIF
	ld	(hl),AVAILABLE	;set block status = free
	inc	hl		;HL=pointer of block size
	ld	(hl),MAX_SIZE	;set size = 16KB
				;next bank
	pop	hl
	pop	bc
	pop	af
	inc	hl		;next header
	inc	hl
	inc	hl
	inc	hl
	inc	a		;next bank
	djnz	initLMAX
				;init MaxAv vector
	ld	b,BANKS_CNT
	ld	hl,MaxAv
	ld	a,MAX_SIZE+1
loopav:	ld	(hl),a
	inc	hl
	djnz	loopav

	pop	iy
IF	?Z80ALL
	xor	a		;bank 0 back in 0000-8000
	out	(1FH),a
ENDIF
	ret	

ENDIF
;-------------------------------------------------------------512K or Z80ALL
;
;	Get available memory size in KB
;
;*****************************************************************
;int	GetTotalFree(void);
;*****************************************************************
;
;	returns HL=total amount of free memory in KB
;
_GetTotalFree:
;-------------------------------------------------------------2X64K
IF	?2X64K

	call	__GetTotalFree	;DE=total free bytes
	srl	d
	srl	d		;D=#KB in upper RAM
	ld	l,d
	ld	h,0		;HL=#KB in lower & upper RAM
	ret
;
;	Returns DE=total size of Upper RAM free memory
;
__GetTotalFree:
	ld	de,0		;total
	ld	bc,(FreeBHeader);HL=first available block (or FreeBHeader=EOL)
get_loop:
	ld	hl,FreeBHeader
	xor	a		;carry=0
	sbc	hl,bc		;if equal to header,
	ret	z		;return, no more memory
				;BC=free block+NEXTFREE
	ld	hl,-NEXTFREE
	add	hl,bc		;HL=free block
	push	de
	GetSIZE			;BC=size
	pop	de
	ex	de,hl
	add	hl,bc		;add to total
	ex	de,hl
	ld	bc,NEXTFREE
	add	hl,bc
	GetWord			;BC=next free block+NEXTFREE (or FreeBHeader+NEXTFREE)
	jr	get_loop
;-------------------------------------------------------------2X64K
ELSE
;-------------------------------------------------------------512K
	push	ix
	push	iy
	call	__GetTotalFree
	pop	iy
	pop	ix
	ret
;
__GetTotalFree:
	ld	iy,0		;IY=grand total
	ld	bc,BANKS_CNT	;B = CARRY = 0, C = banks counter
bigloop:
	ld	ix,0		;IX=crt bank total
	push	bc		;save BC
	ld	a,BANKS_CNT
	sub	c		;A=crt bank (0,1...)
IF	?512K
	add	a,32+4
	out	(7AH),a		;select physical RAM bank number A in logical bank 2 (8000-C000)
	sub	32+4		;A=crt bank (0,1...)
ELSE
	inc	a
	out	(1FH),a
	dec 	a
ENDIF
	ld	de,LISTS_NR*2	;DE=LISTS_NR*2
				;compute HL=Lists+(crt.bank)*(LISTS_NR*2)
	ld	hl,Lists
1:	or	a
	jr	z,2f
	add	hl,de
	dec	a
	jr	1b
2:
	ld	a,LISTS_NR	;A=counter of lists
	ld	bc,10H		;BC=size of blocks
nextl:	push	af
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=crt list header
	inc	hl
	push	hl		;pointer of lists on stack
	ex	de,hl		;HL=crt list header
	push	hl		;on stack
nextb:	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=first
	pop	hl		;HL=header
	push	hl
	or	a
	sbc	hl,de
	jr	z,1f
	add	ix,bc
	ex	de,hl
	jr	nextb
1:	sla	c
	rl	b		;BC=BC*2
	pop	hl		;drop header
	pop	hl		;HL=pointer of lists
	pop	af		;A=counter
	dec	a
	jr	nz,nextl
				;done with crt bank
	pop	bc		;B = CARRY, C = banks counter
	push	ix
	pop	de		;DE=crt bank total
	add	iy,de		;add to grand total		
	jr	nc,2f
	inc	b		;increment carry if needed
2:
	dec	c
	jp	nz,bigloop

	push	iy
	pop	de
				;rotate (B,D) right 2 times
	srl	b
	rr	d
	srl	b
	rr	d

	ld	h,b
	ld	l,d
IF	?Z80ALL
	xor	a
	out	(1FH),a		;bank 0 back in 0000-8000
ENDIF
	ret

ENDIF
;-------------------------------------------------------------512K
;
;	Allocate dynamic memory
;
;*****************************************************************
;void*	Alloc(short size, char* type);
;*****************************************************************
;
;	returns addr of allocated block &
;		 	type=0FFH:	block is in Lower 64KB RAM (128KB RAM) or
;			type=0:		block is in Upper 64KB RAM (128KB RAM) or
;			type=36...63:	bank # in 512KB RAM or
;			type=2 or 3:	bank # in 4 x 32KB RAM
;		or NULL if no memory available
;
;-------------------------------------------------------------2X64K
IF	?2X64K
_Alloc:
	ld	hl,2
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC = size
	inc	hl		;HL=pointer of pointer of type
	push	hl		;on stack
	push	bc		;size on stack
	push	bc		;size on stack	
	call	_malloc		;try to allocate in the Lower 64KB RAM
	pop	bc		;drop this one (it seems that malloc, when fails, alters-it!!!)
	pop	de		;DE=size
	ld	a,h
	or	l
	jr	nz,inlower
				;malloc failed, try to allocate in the Upper 64KB RAM
	call	AllocDyn
	ld	a,h
	or	l
	jr	nz,inupper
				;AllocDyn failed, no more available memory
	pop	hl		;drop pointer of pointer of type
	ld	hl,0
	ret
inlower:			;malloc ok
	ld	e,0FFH
settype:			;E=type to be set
	ld	b,h
	ld	c,l		;BC=buffer
	pop	hl		;HL=pointer of pointer of type
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=pointer of type
	ld	(hl),e		;set type
	ld	h,b
	ld	l,c		;HL=buffer
	ret
inupper:			;AllocDyn ok
	ld	bc,HEADERSIZE	;add the size of header
	add	hl,bc
	ld	e,0
	jr	settype
;-------------------------------------------------------------2X64K
ELSE
;-------------------------------------------------------------512K or Z80ALL
_Alloc:
	ld	hl,2
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC = size
	inc	hl		;HL=pointer of pointer of type
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=target bank pointer
	inc	bc		;add 6 to size
	inc	bc
	inc	bc
	inc	bc
	inc	bc
	inc	bc
	push	de		;target bank pointer on stack
	call	__allocS	;C=bSize
	ld	b,BANKS_CNT	;B=banks counter
	ld	hl,MaxAv	;HL=MaxAv vector
loop:
				;compare bSize ? MaxAv[bank]
	ld	a,c
	cp	(hl)		;bSize ? MaxAv[bank]
	jr	c,try		;if < , try to allocate in this bank
	jr	toobig		;if >= , it's too big, try to allocate in the next bank
try:
	ld	a,BANKS_CNT
	sub	b		;try to allocate in this bank (A=bank nr)
IF	?512K
	add	a,32+4 		;select physical RAM bank number A in logical bank 2 (8000-C000)
	out	(7AH),a
	push	hl		;save MaxAv
	push	bc		;B=banks counter, C=bSize on stack
	sub	32+4		;A=current bank nr. (0,1,...27)
ELSE
	inc	a 		;select physical RAM bank number A in 0000-8000
	out	(1FH),a
	push	hl		;save MaxAv
	push	bc		;B=banks counter, C=bSize on stack
	dec	a		;A=current bank nr. (0,1)
ENDIF
	call	__alloc		;try to alloc
IF	?Z80ALL
	ld	a,0
	out	(1FH),a		;bank 0 back in 0000-8000
ENDIF
	jr	nz,alloc_ok
				;local alloc failed
	pop	bc		;B=banks counter, C=bSize
	pop	hl		;HL=MaxAv
	ld	(hl),c		;store failed bSize
toobig:
	inc	hl		;increment MaxAv pointer
	djnz	loop
				;global alloc failed
	pop	de		;drop target bank pointer
	ld	hl,0		;return NULL
	ret
;
alloc_ok:			;HL=allocated block
	pop	bc		;B=banks counter
	pop	de		;drop MaxAv
	pop	de		;DE=target bank pointer
	ld	a,BANKS_CNT
	sub	b		;A=current bank nr.
IF	?512K
	add	a,32+4		;A=physical RAM bank number
ELSE
	inc	a
ENDIF
	ld	(de),a		;store target bank (36...63)
	ret			;return HL=allocated block
;	
;	allocS
;
;	BC=memory size (must be <= 4000H or 8000H)
;	Affected regs: A,BC
;	Returns C=bElement size
;
__allocS:
	dec	bc		;bc = memory size-1
	ld	a,b
IF	?512K
	and	3FH		;keep it <= 3FH
ELSE
	and	7FH		;keep it <= 7FH
ENDIF
	or	a
	jr	z,1f
				;high part != 0
IF	?Z80ALL
	ld	c,11		;prepare bSize for 8000H
	bit	6,a
	ret	nz		;if bit#14=1 return 11 for 8000H
	dec	c		;C=10
ELSE
	ld	c,10		;prepare bSize for 4000H
ENDIF
	bit	5,a
	ret	nz		;if bit#13=1 return 10 for 4000H
	dec	c		;C=9
	bit	4,a
	ret	nz		;if bit#12=1 return 9 for 2000H
	dec	c		;C=8
	bit	3,a
	ret	nz		;if bit#11=1 return 8 for 1000H
	dec	c		;C=7
	bit	2,a
	ret	nz		;if bit#10=1 return 7 for 800H
	dec	c		;C=6
	bit	1,a
	ret	nz		;if bit#9=1 return 6 for 400H
	dec	c		;C=5
	ret			;else return 5 for 200H
1:	ld	a,c		;high part == 0
	ld	c,4		;C=4
	bit	7,a
	ret	nz		;if bit#7=1 return 4 for 100H
	dec	c		;C=3
	bit	6,a
	ret	nz		;if bit#6=1 return 3 for 80H
	dec	c		;C=2
	bit	5,a
	ret	nz		;if bit#5=1 return 2 for 40H
	dec	c		;C=1
	bit	4,a		
	ret	nz		;if bit#4=1 return 1 for 20H
	dec	c		;C=0
	ret			;else return 0 for 10H
;
;	Allocate a memory block of given size
;
;	called under DI
;	A=current bank nr. (0,1,...27)
;	returns Z=0 and HL=pointer to memory block if available, 
;		else Z=1 if no memory available
;	Local variables: DE = Element, 
;			BC on stack (B=Size, C=CrtSize), 
;			IY=Lists[crt.bank],
;	Affected regs: A,BC,DE,HL
;	IX not affected
;
__alloc:
				;compute IY=Lists+(crt.bank)*(LISTS_NR*2)
				;A=current bank nr. (0,1,...27)
	ld	de,LISTS_NR*2	;DE=LISTS_NR*2
	ld	iy,Lists
	ld	b,5		;how many times to shift A
2:
	rra			;shift right A
	jr	nc,1f
	add	iy,de
1:
	sla	e		;DE=DE*2
	rl	d
	djnz	2b
				;IY=Lists+(crt.bank)*(LISTS_NR*2)
;CrtSize=Size
	ld	b,c		;CrtSize=Size
5:	
;do {
;  Element=FirstFromL(Lists[Lists[CrtSize])
				;
	push	bc		;B=Size, C=CrtSize on stack
	ld	a,c
	add	a,a		;A=CrtSize*2
	push	iy
	pop	hl		;HL=Lists+(crt.bank)*(LISTS_NR*2)		
	ld	d,0
	ld	e,a
	add	hl,de		;HL=Lists+(crt.bank)*(LISTS_NR*2)+(CrtSize*2)
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=CrtL=Lists[CrtSize]
	call	__GetFromL	;HL=GetFromL(CrtL)
	jr	z,7f
				;
;  if (Element) {
				;HL=Element
	ld	d,h
	ld	e,l		;save DE=Element
;    Element->Status=ALLOCATED
	ld	a,OFF_STS
	add	a,l
	ld	l,a		;HL=&El.Status
	ld	a,0FFH
	ld	(hl),a		;ALLOCATED
				;
	pop	bc		;B=Size, C=CrtSize
	inc	l		;HL=&El.Size
;    Element->Size=Size
	ld	(hl),b		;set ElSize = Size
	ld	a,c
;    if ( CrtSize == Size)
	cp	b	
;      return Element
	jr	nz,6f
	ex	de,hl		;HL=Element
	ld	bc,6
	add	hl,bc		;+6
	inc	a		;Z=0
	ret			;return HL=Element
6:	
;    do {
;      CrtSize--
	dec	c		;decrement CrtSize
				;
	push	bc		;B=Size, C=CrtSize back on stack
				;
;      ElementBuddy = Element XOR Buddy_XOR_Mask
	ld	a,c
	add	a,a		;A=CrtSize*2
	ld	hl,Buddy
	add	a,l
	ld	l,a
	jr	nc,1f
	inc	h		
1:				;HL=pointer to Buddy_XOR_mask
	push	de		;DE=Element on stack
	ld	a,e
	xor	(hl)
	ld	e,a
	inc	l
	ld	a,d
	xor	(hl)
	ld	d,a		;DE = ElementBuddy = Element XOR Buddy_XOR_Mask
;      CrtL=Lists[CrtSize]
	ld	a,c		;C=CrtSize
	add	a,a		;A=CrtSize*2
	push	iy
	pop	hl		;HL=Lists+(crt.bank)*(LISTS_NR*2)		
	ld	b,0
	ld	c,a
	add	hl,bc		;HL=Lists+(crt.bank)*(LISTS_NR*2)+(CrtSize*2)
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=CrtL=Lists[CrtSize]
;      AddToL(CrtL, ElementBuddy)
	call	__AddToL	;AddToL(HL=CrtL, DE=ElementBuddy)
				;returned HL=ElementBuddy
	pop	de		;DE=Element
;      ElementBuddy->Status=AVAILABLE
	ld	a,OFF_STS
	add	a,l
	ld	l,a
	ld	(hl),AVAILABLE	
;      ElementBuddy->Size=CrtSize
				;
	pop	bc		;B=Size, C=CrtSize
				;
	inc	l
	ld	a,c
	ld	(hl),a		;set ElBuddy Size = CrtSize	
;    while (--CrtSize != Size)
	cp	b
	jr	nz,6b
	inc	a		;Z=0
	ex	de,hl		;HL=Element
	ld	bc,6
	add	hl,bc		;+6
	ret			;return HL=Element	
;
;   end if (Element) }
7:	
;}
;while (CrtSize++ < MAX_SIZE)
	pop	bc		;B=Size, C=CrtSize
	inc	c		;++CrtSize
	ld	a,c
	cp	MAX_SIZE+1
	jr	nz,5b
;				;alloc failed
	xor	a		;Z=1, no available memory
	ret	
;

ENDIF
;-------------------------------------------------------------512K or Z80ALL
;
;	Free dynamic memory
;
;*****************************************************************
;void	Free(void* buf, char flag);
;*****************************************************************
;
;-------------------------------------------------------------2X64K
IF	?2X64K
_Free:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=buf
	inc	hl
	ld	a,(hl)		;A=type (FF : in lower RAM)
	cp	0FFH
	jr	nz,upfree
	push	de		;dealloc from lower RAM
	call	_free
	pop	de
	ret
upfree:				;dealloc from upper RAM
	or	a		;carry=0			
	ex	de,hl		;HL=buf
	ld	bc,HEADERSIZE	
	sbc	hl,bc		;HL=addr of block header
	jp	FreeDyn
;-------------------------------------------------------------2X64K
ELSE
;-------------------------------------------------------------512K or Z80ALL
_Free:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=buf
	inc	hl
	ld	a,(hl)		;A=physical RAM bank counter (36...63 or 2,3)
IF	?512K
	out	(7AH),a		;select physical RAM bank number A in logical bank 2 (8000-C000)
	sub	32+4		;A=current bank nr. (0,1,...27)
ELSE
	out	(1FH),a
	dec	a
ENDIF
	ld	hl,MaxAv	;HL=MaxAv vector
	ld	b,0
	ld	c,a
	add	hl,bc
	ld	(hl),MAX_SIZE+1	;set availability at full range...first alloc will settle-it
	ex	de,hl		;HL=Buf
;
;	Deallocate a memory block of given size
;
;	HL=memory block addr
;	A=current bank nr. (0,1,...27)
;	Local variables : DE = Element, 
;			C=CrtSize, 
;			IY=Lists[crt.bank],
;	Affected regs: A,BC,DE,HL,IY
;	IX not affected
;
	push	iy		;save IY
	ld	bc,-6
	add	hl,bc		;decrement HL by -6
				;compute IY=Lists+(crt.bank)*(LISTS_NR*2)
				;A=current bank nr. (0,1,...27)
	ld	de,LISTS_NR*2	;DE=LISTS_NR*2
	ld	iy,Lists
	ld	b,5		;how many times to shift A
2:
	rra			;shift right A
	jr	nc,1f
	add	iy,de
1:
	sla	e		;DE=DE*2
	rl	d
	djnz	2b
				;IY=Lists+(crt.bank)*(LISTS_NR*2)
	ld	d,h
	ld	e,l		;DE=HL=Element
	ld	a,OFF_SIZE
	add	a,l
	ld	l,a
	ld	c,(hl)		;C=CrtSize=Size
	ld	b,0		;B=0
;CrtSize=Size
;while (CrtSize < MAX_SIZE) {
10:	
	push	de		;DE=Element on stack
	ld	a,c		;A=CrtSize
	cp	MAX_SIZE
	jr	z,11f		;quit while loop when CrtSize == MAX_SIZE
				;
;  ElementBuddy = Element XOR Buddy_XOR_Mask
	ld	a,c
	add	a,a		;A=CrtSize*2
	ld	hl,Buddy
	add	a,l
	ld	l,a		
	jr	nc,1f
	inc	h		
1:				;HL=pointer to Buddy_XOR_mask
	ld	a,e
	xor	(hl)
	ld	e,a
	inc	l
	ld	a,d
	xor	(hl)
	ld	d,a		;DE = ElementBuddy = Element XOR Buddy_XOR_Mask
	ex	de,hl		;HL=ElementBuddy
	ld	a,OFF_STS
	add	a,l
	ld	l,a
	ld	a,(hl)		;A=BuddyStatus
	or	a		;is AVAILABLE ?
	jr	nz,11f		;break if ElementBuddy->Status != AVAILABLE
	inc	l
	ld	a,(hl)		;A=BuddySize
	cp	c		;is BuddySize == CrtSize ?
	jr	nz,11f		;break if ElementBuddy->Size != CrtSize
				;CARRY=0
	ld	a,l
	sub	OFF_SIZE
	ld	l,a		;HL=ElementBuddy
;  RemoveFromL(ElementBuddy)
	push	bc		;save CrtSize
	call	__RemoveFromL	;HL is still = ElementBuddy
	pop	bc		;restore CrtSize
	ex	de,hl		;DE=ElementBuddy
	pop	hl		;HL=Element
				;must set Element = min (Element,Buddy)
;  if (ElementBuddy < Element)
				;compare DE(buddy) ? HL(element)
	ld	a,d
	cp	h
	jr	c,12f
				;buddy high (D) >= element high (H)
	ld	a,e
	cp	l
	jr	c,12f
				;buddy low (E) >= element low (L)
				;buddy (DE) >= HL (element)...
;    then Element = ElementBuddy
	ex	de,hl		;so set DE (element) = HL
12:
	inc	c		;C=CrtSize++
	jr	10b
				;
;} end while (CrtSize < MAX_SIZE)
11:
	pop	de		;DE=Element
	ld	h,d
	ld	l,e		;HL=DE=Element
	inc	l
	inc	l
	inc	l
	inc	l
;Element->Status=AVAILABLE
	ld	(hl),AVAILABLE	
	inc	l
;Element->Size = CrtSize;
	ld	(hl),c		;set Element Size=CrtSize
	ld	a,c
	add	a,a		;A=CrtSize*2
	push	iy
	pop	hl		;HL=Lists+(crt.bank)*(LISTS_NR*2)		
	ld	b,0
	ld	c,a
	add	hl,bc		;HL=Lists+(crt.bank)*(LISTS_NR*2)+(CrtSize*2)
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=CrtL=Lists[CrtSize]
;CrtL=Lists[CrtSize]
;AddToL(CrtL, Element);
	call	__AddToL
	pop	iy		;restore IY
IF	?Z80ALL
	xor	a
	out	(1FH),a		;bank 0 back to 0000-8000
ENDIF
	ret
;
;	__AddToL
;
;	must be called under interrupts DISABLED
;	HL=list header, DE=new
;	return HL=new
;	affected regs: A,BC,DE,HL
;	IX,IY not affected
;
__AddToL:
        ld      a,l
        ld      (de),a
        inc     de
        ld      a,h
        ld      (de),a
        dec     de              ;New.Next=ListHeader
        inc     hl
        inc     hl
        ld      c,(hl)
        ld      (hl),e
        inc     hl
        ld      b,(hl)
        ld      (hl),d          ;BC=Last, ListHeader.Last=New
        ld      a,e
        ld      (bc),a
        inc     bc
        ld      a,d
        ld      (bc),a
        dec     bc              ;Last.Next=New
        ld      l,e
        ld      h,d             ;return HL=New
        inc     de
        inc     de
        ld      a,c
        ld      (de),a
        inc     de
        ld      a,b
        ld      (de),a          ;New.Prev=Last
	ret
;
;	__RemoveFromL
;
;	must be called under interrupts DISABLED
;	HL=elem to be removed
;	Returns HL=Element
;	affected regs: A,BC,DE,HL
;	IX,IY not affected
;
__RemoveFromL:
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        inc     hl              ;DE=Next
        ld      c,(hl)
        inc     hl
        ld      b,(hl)          ;BC=Prev
        ld      a,e
        ld      (bc),a
        inc     bc
        ld      a,d
        ld      (bc),a          ;Prev.Next=Next
        dec     bc
        inc     de
        inc     de
        ld      a,c
        ld      (de),a
        inc     de
        ld      a,b
        ld      (de),a          ;Next.Prev=Prev
	dec	hl
	dec	hl
	dec	hl		;HL=element
	ret
;
;	__GetFromL
;
;	must be called under interrupts DISABLED
;	HL=list header
;	returns (HL=elem and Z=0) or (HL=0 and Z=1 if list empty)
;	affected regs: A,BC,DE,HL
;	IX,IY not affected
;
__GetFromL:
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        dec     hl		;DE=First, HL=ListHeader
				;compare HL ? DE 
	or	a		;CARRY=0
	sbc	hl,de
        ret	z	        ;list empty, return HL=0
	ex	de,hl		;HL will be returned after removing element from list
        ld      e,(hl)		;Remove HL=Element
        inc     hl
        ld      d,(hl)
        inc     hl              ;DE=Next
        ld      c,(hl)
        inc     hl
        ld      b,(hl)          ;BC=Prev
        ld      a,e
        ld      (bc),a
        inc     bc
        ld      a,d
        ld      (bc),a          ;Prev.Next=Next
        dec     bc
        inc     de
        inc     de
        ld      a,c
        ld      (de),a
        inc     de
        ld      a,b
        ld      (de),a          ;Next.Prev=Prev
	dec	hl
	dec	hl
	dec	hl		;HL=element to be returned
	or	h		;Z=0
	ret

ENDIF
;-------------------------------------------------------------512K

;	Get string (destination is in Lower RAM)
;
;*****************************************************************
;void	GetString(char* dest, char* source, char source_flag);
;*****************************************************************
;	
_GetString:
	ld	hl,2
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=dest
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=source
	inc	hl
;-------------------------------------------------------------2X64K
IF	?2X64K

	ld	a,(hl)		;A=flag (0FF if in low 64KB RAM, 0 if in upper 64KB RAM)
	or	a
	jr	nz,1f
				;source in upper 64KB RAM
	ex	de,hl		;HL=source
	ld	d,b
	ld	e,c		;DE=dest
	jp	$ReadString
1:				;source in lower RAM
;-------------------------------------------------------------2X64K
ELSE
;-------------------------------------------------------------512K or Z80ALL
	ld	a,(hl)		;A=block #
IF	?512K
	out	(7AH),a
ELSE
	out	(1FH),a	
ENDIF
ENDIF
;-------------------------------------------------------------512K or Z80ALL
2:				;DE=source, BC=dest
	ld	a,(de)
	ld	(bc),a
	inc	de
	inc	bc
	or	a		;if end-of-string
	jr	nz,2b
				;return
IF	?Z80ALL
	out	(1FH),a		;bank 0 back to 0000-8000
ENDIF
	ret
;
;	Put string (source is in Lower RAM)
;
;*****************************************************************
;void	PutString(char* source, char* dest, char dest_flag);
;*****************************************************************
;	
_PutString:
	ld	hl,2
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=source
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=dest
	inc	hl
;-------------------------------------------------------------2X64K
IF	?2X64K

	ld	a,(hl)		;A=flag (0FF if in low 64KB RAM, 0 if in upper 64KB RAM)
	or	a
	jr	nz,1f
				;dest in upper 64KB RAM
	ex	de,hl		;HL=dest
	ld	d,b
	ld	e,c		;DE=source
	jp	$WriteString
1:				;dest in lower RAM
;-------------------------------------------------------------2X64K
ELSE
;-------------------------------------------------------------512K or Z80ALL
	ld	a,(hl)		;A=block #
IF	?512K
	out	(7AH),a
ELSE
	out	(1FH),a		
ENDIF
ENDIF
;-------------------------------------------------------------512K or Z80ALL
2:				;DE=dest, BC=source
	ld	a,(bc)
	ld	(de),a
	inc	de
	inc	bc
	or	a		;if end-of-string
	jr	nz,2b
				;return
IF	?Z80ALL
	out	(1FH),a		;bank 0 back to 0000-8000
ENDIF
	ret
;
;	Get string length
;
;*****************************************************************
;short	StringLen(char* s, char s_flag);
;*****************************************************************
;
_StringLen:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=string
	inc	hl
;-------------------------------------------------------------2X64K
IF	?2X64K

	ld	a,(hl)		;A=flag (0FF if in low 64KB RAM, 0 if in upper 64KB RAM)
	or	a
	jr	nz,1f
				;in upper 64KB
	ex	de,hl		;HL=string
	jp	$StringLen
1:				;in low 64KB RAM
;-------------------------------------------------------------2X64K
ELSE
;-------------------------------------------------------------512K or Z80ALL
	ld	a,(hl)		;A=block #
IF	?512K
	out	(7AH),a
ELSE
	out	(1FH),a		
ENDIF
ENDIF
;-------------------------------------------------------------512K or Z80ALL
	ld	b,0FFH
2:	ld	a,(de)
	inc	de
	inc	b
	or	a
	jr	nz,2b
	ld	h,a
	ld	l,b
IF	?Z80ALL
	out	(1FH),a		;bank 0 back to 0000-8000
ENDIF
	ret
;
;	Get vector[index] as byte
;	For 2X64K & 4x32K vector is allways in lower 64KB RAM
;*****************************************************************
;char	GetByte(char* vector, short index, char flag);
;*****************************************************************
;
_GetByte:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=vector
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=index
;-------------------------------------------------------------512K
IF	?512K
	inc	hl
	ld	a,(hl)		;A=16KB RAM block#
	out	(7AH),a
ENDIF
;-------------------------------------------------------------512K
	ex	de,hl		;HL=vector
	add	hl,bc		;HL=vector+index
	ld	l,(hl)		;get L=byte
	ld	h,0
	ret
;
;	Get vector[index] as word
;	For 2X64K & 4x32K vector is allways in lower 64KB RAM
;*****************************************************************
;char*	GetWord(char* vector, short index, char flag);
;*****************************************************************
;
_GetWord:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=vector
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=index
	sla	c
	rl	b		;BC=BC*2
;-------------------------------------------------------------512K
IF	?512K
	inc	hl
	ld	a,(hl)		;A=16KB RAM block#
	out	(7AH),a
ENDIF
;-------------------------------------------------------------512K
	ex	de,hl		;HL=vector
	add	hl,bc		;HL=vector+index
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl		;get HL=word
	ret
;
;	Put vector[index] as byte
;	For 2X64K & 4x32K vector is allways in lower 64KB RAM
;*****************************************************************
;char	PutByte(char* vector, short index, char byte, char flag);
;*****************************************************************
;
_PutByte:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=vector
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=index
	inc	hl
	ld	a,(hl)		;A=byte
;-------------------------------------------------------------512K
IF	?512K
	push	af
	inc	hl
	inc	hl
	ld	a,(hl)		;A=16KB RAM block#
	out	(7AH),a
	pop	af
ENDIF
;-------------------------------------------------------------512K
	ex	de,hl		;HL=vector
	add	hl,bc		;HL=vector+index
	ld	(hl),a		;Put byte
	ret
;
;	Put vector[index] as word
;	For 2X64K & 4x32K vector is allways in lower 64KB RAM
;*****************************************************************
;char	PutWord(char* vector, short index, short word, char flag);
;*****************************************************************
;
_PutWord:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=vector
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=index
	sla	c
	rl	b		;BC=BC*2
	ex	de,hl
	add	hl,bc
	ex	de,hl		;DE=vector+(index*2)
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=word
;-------------------------------------------------------------512K
IF	?512K
	inc	hl
	ld	a,(hl)		;A=16KB RAM block#
	out	(7AH),a
ENDIF
;-------------------------------------------------------------512K
	ex	de,hl		;HL=vector+(index*2)
	ld	(hl),c		;put word
	inc	hl
	ld	(hl),b
	ret
;
