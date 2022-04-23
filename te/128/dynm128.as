DEBUG	equ	0
SC108	equ	1	;1=SC108, 0=32KB ROM + 128KB RAM Phillip Stevens memory module 
;
;	Dynamic memory allocator for 64KB Upper RAM
;	Data access routines
;	Shadow routines
;

COND	1-DEBUG
	GLOBAL	_InitDynM
	GLOBAL	_GetTotalFree
	GLOBAL	_Alloc128
	GLOBAL	_Free128
	GLOBAL	_malloc
	GLOBAL	_free
ENDC
COND	DEBUG
	GLOBAL	_InitDynM
	GLOBAL	AllocDyn
	GLOBAL	FreeDyn
	GLOBAL	_GetTotalFree
ENDC

COND	DEBUG
DYNM_START	equ	4000H
DYNM_END	equ	6000H
ENDC
COND	1-DEBUG
DYNM_START	equ	1
DYNM_END	equ	0FFA0H
ENDC

$ReadByte 	equ 	0FFA0H
$WriteByte	equ 	0FFAAH
$ReadWord	equ 	0FFB4H
$WriteWord	equ 	0FFC0H
$WriteString	equ 	0FFCCH
$ReadString	equ 	0FFDEH
$StringLen	equ	0FFE9H

	psect	bss

COND	DEBUG
	org	1000H
ENDC

COND	1-DEBUG
FreeBHeader	equ	0FFFAH
ENDC
COND	DEBUG
FreeBHeader:
	defs	2	;first free+NEXTFREE
	defs	2	;last free+NEXTFREE
ENDC
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
COND	DEBUG
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
ENDC
COND	1-DEBUG
	call	$ReadWord
ENDC
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
COND	DEBUG
	ld	(hl),c
ENDC
COND	1-DEBUG
	call	$WriteByte
ENDC
ENDM

MACRO	GetByte
COND	DEBUG
	ld	c,(hl)
ENDC
COND	1-DEBUG
	call	$ReadByte
ENDC
ENDM

MACRO	PutWord
COND	DEBUG
	ld	(hl),c
	inc	hl
	ld	(hl),b
ENDC
COND	1-DEBUG
	call	$WriteWord
ENDC
ENDM

MACRO	GetWord
COND	DEBUG
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
ENDC
COND	1-DEBUG
	call	$ReadWord
ENDC
ENDM

	psect	text
;
COND	DEBUG
_Alloc:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	jp	AllocDyn
_Free:	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl
	jp	FreeDyn
ENDC
;
;	Init dynamic memory
;
_InitDynM:
	push	ix			;save IX
COND	1-DEBUG
	call	StoreShared		;setup shadow routines
ENDC
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
;	GetTotalFree
;
;	returns HL=total amount of free memory in KB
;
_GetTotalFree:
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
COND	1-DEBUG
;
;	Alloc128
;
;void*	Alloc128(short size, char* type)
;
;	returns addr of allocated block &
;		 	type=0:block is in Lower 64KB RAM or
;			type=1:block is in Upper 64KB RAM
;		or NULL
_Alloc128:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE = size
	inc	hl		;HL=pointer of pointer of type
	push	hl		;on stack
	push	de		;size on stack
	push	de		;size on stack	
	call	_malloc		;try to allocate in the Lower 64KB RAM
	pop	de		;drop this one (it seems that malloc, when fails, alters-it!!!)
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
	ld	e,0
settype:			;E=type to be set
	ld	b,h
	ld	c,l		;BC=buffer
	pop	hl		;HL=pointer of pointer of type
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=pointer of type
	ld	a,e
	ld	(hl),a		;set type=0
	ld	h,b
	ld	l,c		;HL=buffer
	ret
inupper:			;AllocDyn ok
	ld	bc,HEADERSIZE	;add the size of header
	add	hl,bc
	ld	e,1
	jr	settype
;
;	Free128
;
;void	Free128(void* buf, char type)
;
_Free128:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=buf
	inc	hl
	ld	a,(hl)		;A=type
	or	a
	jr	nz,upfree
	push	de		;dealloc from lower RAM
	call	_free
	pop	de
	ret
				;dealloc from upper RAM
upfree:	dec	a		;should be 1
	ret	nz
	ex	de,hl		;HL=buf
	ld	bc,HEADERSIZE	;carry=0
	sbc	hl,bc		;HL=addr of block header
	jp	FreeDyn
;
ENDC

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
;------------------------------------------------------SC108

;------------------------------------------------------1-SC108
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

	GLOBAL	_GetString
	GLOBAL	_PutString
	GLOBAL	_GetByte
	GLOBAL	_PutByte
	GLOBAL	_GetWord
	GLOBAL	_PutWord
	GLOBAL	_StringLen
;
;	short	StringLen(char* source)
;
;	source is in Upper RAM
;
_StringLen:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source in Upper RAM
	jp	$StringLen
;
;	GetString(char* dest, char* source)
;
;	source is in Upper RAM
;	destination is in Lower RAM
;
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
	jp	$ReadString
;
;	PutString(char* source, char* dest)
;
;	source is in Lower RAM
;	destination is in Upper RAM
;
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
	jp	$WriteString
;
;	PutByte(char* dest, char b)
;
;	dest is in Upper RAM
;
_PutByte:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	ex	de,hl		;HL=dest, C=byte
	jp	$WriteByte
;
;char	GetByte(char* source)
;
;	source is in Upper RAM
;
_GetByte:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source
	call	$ReadByte
	ld	l,c
	ret
;
;	PutWord(char* dest, short w)
;
;	dest is in Upper RAM
;
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
	jp	$WriteWord
;
;short	GetWord(char* source)
;
;	source is in Upper RAM
;
_GetWord:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source, DE=byte pointer
	call	$ReadWord
	ld	h,b
	ld	l,c
	ret
;
