DEBUG	equ	0
;
;	512 KB RAM Dynamic Memory support
;
OPTIM		equ	0	;1=optimize alloc speed, 0=do not optimize
TRACE		equ	0	;type function calls
RCHECK		equ	0	;check virtual pointers
;
IF	DEBUG
PORT		equ	79H	;maps 4000H to 8000H
Buf16K		equ	4000H	;address of dynamic memory bank
ELSE
PORT		equ	7AH	;maps 8000H to C000H
Buf16K		equ	8000H	;address of dynamic memory bank
ENDIF

BANKS_CNT	equ	8	;8 RAM banks of 16KB each are available (nr.48 to 55)
;
;Structure of the Virtual Pointer to allocated buffer (size = 10H to 4000H)
;
;01bbbbaaaaaaaaaa	:	4000H to 7FFFH
;where
;            bbbb	:	0 to 15 	= virtual RAM bank (Physical RAM bank = 32 + 16 + virtual RAM bank)
;00aaaaaaaaaa0000  	:	0000H to 3FF0H	= offset in RAM bank

; Memory block structure
;
; 	defw	next 	;pointer to next block
;	defw	prev 	;pointer to prev block
;	defb	status 	;0=available, 0FF=allocated
;	defb	size	;block size : from 0(=10H) to 10(=4000H)
;	data
;
OFF_STS		equ	4
OFF_SIZE	equ	5
OFF_DATA	equ	6
;
MAX_SIZE	equ	10	;max block = 4000H
LISTS_NR	equ	MAX_SIZE+1
AVAILABLE	equ	0
ALLOCATED	equ	0FFH
;
	GLOBAL	_Init512Banks
	GLOBAL	_alloc512
	GLOBAL	_free512
	GLOBAL	_IsValid
	GLOBAL	_GetByte
	GLOBAL	_PutByte
	GLOBAL	_GetWord
	GLOBAL	_PutWord
	GLOBAL	_ReadMem
	GLOBAL	_WriteMem
	GLOBAL	_GetString
	GLOBAL	_ZeroFill

	psect	bss

Buddy:	defs	LISTS_NR*2
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
;
Lists:	defs	LISTS_NR*2*BANKS_CNT	;bank0(L0,L1,...L10),bank1(L0+4,L1+4,...L10+4)... 
;
String:	defs	32;		;used by GetString

IF	OPTIM

;	Maximum Available
;	contains (Size+1) if available, or zero if unavailable
;
MaxAv:	defs	BANKS_CNT

ENDIF

IF	RCHECK

	psect	data

msgwrong:
	defm	'Wrong parameter used at %04x'
	defb	0ah,0dh,0

	psect	text

WrongPointer:
	ld	(3FFCH),hl	;save wrong addr
	pop	hl		;ret addr
	push	hl
	ld	hl,msgwrong
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	jr	$

MACRO	CHECK_RANGE
	ld	a,h
	and	0C0H
	cp	40H
	jp	nz,WrongPointer
ENDM

ENDIF

IF	TRACE

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

	psect	data

msgwcall:
	defm	'@%04x '
	defb	0

msgal:	defm	'alloc512 %04x'
	defb	0ah,0dh,0
msgfr:  defm    'free %04x'
        defb	0ah,0dh,0
msgpb:	defm	'PB d=%04x b=%02x'
	defb	0ah,0dh,0
msggb:	defm	'GB s=%04x r %02x'
	defb	0ah,0dh,0
msgpw:	defm	'PW d=%04x w=%04x'
	defb	0ah,0dh,0
msggw:	defm	'GW s=%04x r %04x'
	defb	0ah,0dh,0
msgrm:	defm	'RM d=%04x s=%04x'
	defb	0ah,0dh,0
msgwm:	defm	'WM s=%04x d=%04x'
	defb	0ah,0dh,0

ENDIF

	psect	text
;
;	Convert bank & offset to virtual pointer
;
;	B = virtual bank (bbbb = 0...15)
;	HL = offset in RAM bank (00aaaaaa aaaa0000)
;
;	returns HL = 01bbbbaa aaaaaaaa
;
ToVirtual:
				;shift HL -> 4 times
	srl	h
	rr	l
	srl	h
	rr	l
	srl	h
	rr	l
	srl	h
	rr	l
				;now HL = 000000aa aaaaaaaa
				;shift B <- 2 times
	rlc	b
	rlc	b
				;now B = 00bbbb00
				;H = H or B
	ld	a,h
	or	b
	or	01000000B
	ld	h,a
				;now HL = 01bbbbaa aaaaaaaa
	ret
;
;	Convert virtual pointer to bank & offset
;
;	HL = virtual pointer (01bbbbaa aaaaaaaa)
;
;	returns B = virtual bank (bbbb = 0...15)
;		HL = offset (00aaaaaa aaaa0000)
;
FromVirtual:
	ld	b,h		;B = 01bbbbaa
				;shift B -> 2 times
	sra	b
	sra	b		;now B = 0001bbbb
	res	4,b		;now B = 0000bbbb
				;erase 1bbbb from H
	ld	a,h
	and	11B
	ld	h,a		;now HL = 000000aa aaaaaaaa
				;shift HL <- 4 times
	sla	l
	rl	h
	sla	l
	rl	h
	sla	l
	rl	h
	sla	l
	rl	h
				;now HL = 00aaaaaa aaaa0000
	ret
;	
;	allocS
;
;	BC=memory size (must be <= 4000H)
;	Affected regs: A,BC
;	Returns BC=bElement size
;
allocS:
	dec	bc		;bc = memory size-1
	ld	a,b
	and	3FH		;keep it <= 3FH
	or	a
	jr	z,1f
				;high part != 0
	ld	c,MAX_SIZE	;prepare bSize for 4000H
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
;char*	alloc512(short size)
;
;	Size = number of bytes to be allocated
;
;	returns the Virtual Pointer of memory if allocation done or zero if no more available memory
;
_alloc512:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=size in bytes
	inc	bc		;add 6 to size
	inc	bc
	inc	bc
	inc	bc
	inc	bc
	inc	bc
	push	iy		;save IY
	call	allocS	;C=bSize
	ld	b,BANKS_CNT	;B=banks counter
IF	OPTIM
	ld	hl,MaxAv	;HL=MaxAv vector
ENDIF
loop:
IF	OPTIM
				;compare bSize ? MaxAv[bank]
	ld	a,c
	cp	(hl)		;bSize ? MaxAv[bank]		
	jr	nc,toobig	;if >= , it's too big, try to allocate in the next bank
				;if < , try to allocate in this bank
ENDIF
	ld	a,BANKS_CNT
	sub	b		;try to allocate in this bank (A=bank nr)
	add	a,32+16		;select physical RAM bank number A in logical bank 2 (8000-C000)
	out	(PORT),a
IF	OPTIM
	push	hl		;save MaxAv
ENDIF
	push	bc		;B=banks counter, C=bSize on stack
	sub	32+16		;A=current bank nr. (0,1,...15)
	call	alloc		;try to alloc
	jr	nz,alloc_ok
				;local alloc failed
	pop	bc		;B=banks counter, C=bSize
IF	OPTIM
	pop	hl		;HL=MaxAv
	ld	(hl),c		;store failed bSize
toobig:
	inc	hl		;increment MaxAv pointer
ENDIF	
	djnz	loop
				;global alloc failed
	pop	iy		;restore IY
	ld	hl,0		;return NULL
	ret
;
alloc_ok:			;HL=allocated block
IF	TRACE
	push	hl
	ld	hl,msgal
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
ENDIF
	res	7,h		;HL = allocated block-8000H = offset in RAM bank
	pop	bc		;B=banks counter
IF	OPTIM
	pop	de		;drop MaxAv
ENDIF
	pop	iy		;restore IY
	ld	a,BANKS_CNT
	sub	b		;A=current bank
	ld	b,a		;B=bank
	jp	ToVirtual	;return HL=virtual pointer
;
;void	free512(char* Buf)
;
;	Buf = virtual pointer of memory to be deallocated 
;
_free512:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=virtual pointer
	call	FromVirtual	;B = bank (0...15), HL = offset in RAM bank
	set	7,h		;HL = 8000H + offset in RAM bank
IF	TRACE
        push    hl
        ld      hl,msgfr
        push    hl
	ld	hl,__iob+16		;stderr
	push	hl
	call	_fprintf
	pop	hl
        pop     hl
        pop     hl
ENDIF
	ex	de,hl		;DE = addr in RAM bank
	ld	a,b		;A = bank (0...15)
IF	OPTIM
	ld	hl,MaxAv	;HL=MaxAv vector
	ld	c,b
	ld	b,0
	add	hl,bc
	ld	(hl),MAX_SIZE+1	;set availability at full range...first alloc will settle-it
ENDIF
	add	a,32+16
	out	(PORT),a	;select physical RAM bank number A in logical bank 2 (8000-C000)
	ex	de,hl		;HL=addr in RAM bank
	sub	32+16		;A=current bank nr. (0...15)
	push	iy		;save IY
	call	dealloc
	pop	iy		;restore IY
	ret
;
;	Allocate a memory block of given size
;
;	A=current bank nr. (0,1,...15)
;	C=bSize (0=10H,...,10=4000H)
;	returns Z=0 and HL=pointer to memory block if available, 
;		else Z=1 if no memory available
;	Local variables: DE = Element, 
;			BC on stack (B=Size, C=CrtSize), 
;			IY=Lists[crt.bank],
;	Affected regs: A,BC,DE,HL,IY
;	IX not affected
;
alloc:
				;compute IY=Lists+(crt.bank)*(LISTS_NR*2)
				;A=current bank nr. (0,1,...15)
	ld	de,LISTS_NR*2	;DE=LISTS_NR*2
	ld	iy,Lists
	ld	b,4		;how many times to shift A
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
	call	GetFromL	;HL=GetFromL(CrtL)
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
	ld	a,ALLOCATED
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
	ld	hl,Buddy	;Buddy must be placed at an xx00H address
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
	call	AddToL	;AddToL(HL=CrtL, DE=ElementBuddy)
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
;	Deallocate a memory block
;
;	HL=memory block addr
;	A=current bank nr. (0,1,...15)
;	Local variables : DE = Element, 
;			C=CrtSize, 
;			IY=Lists[crt.bank],
;	Affected regs: A,BC,DE,HL,IY
;	IX not affected
;
dealloc:
				;compute IY=Lists+(crt.bank)*(LISTS_NR*2)
				;A=current bank nr. (0,1,...15)
	ld	de,LISTS_NR*2	;DE=LISTS_NR*2
	ld	iy,Lists
	ld	b,4		;how many times to shift A
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
	ld	hl,Buddy	;Buddy must be placed at an xx00H address
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
	call	RemoveFromL	;HL is still = ElementBuddy
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
;;;;;;;;jp	AddToL	;Add DE=Element to HL=CrtL 
;(commented out because the called routine is next...!)
;
;	AddToL
;
;	HL=list header, DE=new
;	return HL=new
;	affected regs: A,BC,DE,HL
;	IX,IY not affected
;
AddToL:
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
;	RemoveFromL
;
;	HL=elem to be removed
;	Returns HL=Element
;	affected regs: A,BC,DE,HL
;	IX,IY not affected
;
RemoveFromL:
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
;	GetFromL
;
;	HL=list header
;	returns (HL=elem and Z=0) or (HL=0 and Z=1 if list empty)
;	affected regs: A,BC,DE,HL
;	IX,IY not affected
;
GetFromL:
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
;
;	AdjustAddr
;
;	HL = virtual pointer
;	selects the correct RAM bank
;	returns HL = real address to data
;
AdjustAddr:
	call	FromVirtual
				;B = virtual bank (bbbb = 0...15)
				;HL = offset (00aaaaaa aaaa0000)
	ld	a,b
	add	a,32+16
	out	(PORT),a	;select physical RAM bank number A in logical bank 2 (8000-C000)
	set	7,h		;adjust offset to 8000+offset
	ld	a,l		;add 6=OFF_DATA
	add	a,OFF_DATA
	ld	l,a		;now HL points to the requested byte
	ret	
;	
;char	GetByte(char* source, short offset)
;
;	source is in Upper RAM
;
_GetByte:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=source
	inc	hl
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=offset
	ex	de,hl		;HL=source, DE=offset
IF	RCHECK
	CHECK_RANGE
ENDIF
	call	AdjustAddr
	add	hl,de		;add offset
IF	TRACE
	ld	b,h
	ld	c,l
ENDIF
	ld	l,(hl)		;get L=byte
	ld	h,0		;set H=0 to return HL = byte !
IF	TRACE
	push	hl		;value read
	push	bc		;source
	ld	hl,msggb
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	pop	hl
ENDIF
	ret
;
;	PutByte(char* dest, short offset, char b)
;
;	dest is in Upper RAM
;
_PutByte:
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
	ld	b,(hl)		;BC=offset
	inc	hl
	ld	a,(hl)		;A=byte	
	ex	de,hl		;HL=dest
	ld	e,a		;E=byte
IF	RCHECK
	CHECK_RANGE
ENDIF
	push	bc		;save offset on stack
	call	AdjustAddr
	pop	bc		;BC=offset
	add	hl,bc		;add offset
	ld	(hl),e		;store byte
IF      TRACE
	ld	d,0
        push    de              ;byte
        push    hl              ;destination
        ld      hl,msgpb
        push    hl
        ld      hl,__iob+16     ;stderr
        push    hl
        call    _fprintf
        pop     hl
        pop     hl
        pop     hl
        pop     hl
ENDIF
	ret
;
;short	GetWord(char* source, short offset)
;
;	source is in Upper RAM
;
_GetWord:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=source
	inc	hl
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=offset
	ex	de,hl		;HL=source, DE=offset
IF	RCHECK
	CHECK_RANGE
ENDIF
	call	AdjustAddr
	add	hl,de		;add offset
IF	TRACE
	ld	b,h
	ld	c,l
ENDIF
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=word
	ex	de,hl		;HL=word
IF	TRACE
	push	hl		;value read
	push	bc		;source
	ld	hl,msggw
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl
	pop	hl
ENDIF
	ret
;
;	PutWord(char* dest, short offset, short w)
;
;	dest is in Upper RAM
;
_PutWord:
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
	ld	b,(hl)		;BC=offset
	inc	hl
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=word
	ex	de,hl		;HL=dest, DE=word
IF	RCHECK
	CHECK_RANGE
ENDIF
	push	bc		;save offset on stack
	call	AdjustAddr
	pop	bc		;restore BC=offset
	add	hl,bc		;add offset
IF	TRACE
	ld	b,h
	ld	c,l
ENDIF
	ld	(hl),e		;store word
	inc	hl
	ld	(hl),d
IF      TRACE
        push    de              ;word
        push    bc              ;destination
        ld      hl,msgpw
        push    hl
        ld      hl,__iob+16     ;stderr
        push    hl
        call    _fprintf
        pop     hl
        pop     hl
        pop     hl
        pop     hl
ENDIF
	ret
;
;	ReadMem(char* dest, char* source, short offset, short count)
;
;	source is in Upper RAM
;	destination is in Lower RAM
;	count <= 256
;
_ReadMem:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=dest
	push	de		;on stack
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=source
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=offset
	inc	hl
	ld	a,(hl)		;A=count
	ld	h,b
	ld	l,c		;HL=source
	ld	c,a		;C=count
IF	RCHECK
	pop	de
	CHECK_RANGE
	push	de
ENDIF
	call	AdjustAddr
	add	hl,de		;add offset
	pop	de		;DE=dest
IF	TRACE
	push	bc		;save count
	push	hl		;source
	push	de		;dest
	ld	hl,msgrm
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	de		;dest
	pop	hl		;source
	pop	bc		;restore count
ENDIF
moveit:
	ld	b,0		;BC=count
	ldir
	ret
;
;	WriteMem(char* source, char* dest, short offset, short count)
;
;	source is in Lower RAM
;	destination is in Upper RAM
;	count <= 256
;
_WriteMem:
IF	TRACE
	WCALL
ENDIF
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=source
	push	de		;on stack
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=dest
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=offset
	inc	hl
	ld	a,(hl)		;A=count
	ld	h,b
	ld	l,c		;HL=dest
	ld	c,a		;C=count
IF	RCHECK
	pop	de
	CHECK_RANGE
	push	de
ENDIF
	call	AdjustAddr
	add	hl,de		;add offset
	pop	de		;DE=source
	ex	de,hl		;DE=dest, HL=source
IF	TRACE
	push	bc		;save count
	push	de		;dest
	push	hl		;source
	ld	hl,msgwm
	push	hl
	ld	hl,__iob+16	;stderr
	push	hl
	call	_fprintf
	pop	hl
	pop	hl
	pop	hl		;source
	pop	de		;dest
	pop	bc		;restore count
ENDIF
	jr	moveit
;
;char*	GetString(char* source, short offset);
;
;	string is stored at the virtual pointer @ (source + offset)
;	source is in Upper RAM
;	returns a pointer to a copy of the string, stored in Lower RAM
;
_GetString:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=source in Upper RAM
	inc	hl
	ld	a,(hl)
	inc	hl
	ld	h,(hl)		
	ld	l,a		;HL = offset
	ex	de,hl		;HL = source in Upper RAM, DE = offset
IF	RCHECK
	CHECK_RANGE
ENDIF
	call	AdjustAddr
	add	hl,de		;add offset
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE = virtual pointer of the string
	ex	de,hl		;HL = virtual pointer of the string
	call	AdjustAddr	;HL = pointer to string
	ld	de,String	;DE=local storage for the string
				;move string from HL to DE
move:	ld	a,(hl)
	ld	(de),a
	inc	hl
	inc	de
	or	a
	jr	nz,move
	ld	hl,String	;return String
	ret
;
;	char	IsValid(char* p)
;
;	should be 01bbbbaaaaaaaaaa
;
_IsValid:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=pointer
	ld	a,h
	and	0C0H
	cp	40H
	jr	nz,notvalid
	call	FromVirtual
				;B = virtual bank (bbbb = 0...15)
				;HL = offset (00aaaaaa aaaa0000)
	ld	a,b
	add	a,32+16
	out	(PORT),a	;select physical RAM bank number A in logical bank 2 (8000-C000)
	set	7,h		;adjust offset to 8000+offset
	ld	a,l		;add 4=OFF_STS
	add	a,OFF_STS
	ld	l,a		;now HL points to the requested byte
	ld	a,(hl)		;FFH if valid
	inc	a
	jr	nz,notvalid
	ld	l,0FFH
	ret
notvalid:
	ld	l,0
	ret
;
;	void	ZeroFill(char* p, short size)
;	size < 256
;
_ZeroFill:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=pointer
	inc	hl
	ld	c,(hl)		;C=size
	ex	de,hl		;HL=pointer, C=size
	call	AdjustAddr
	ld	b,c		;B=size
	xor	a
lfill:	ld	(hl),a
	inc	hl
	djnz	lfill
	ret

IF	DEBUG
	psect	text
ELSE
	psect	top
ENDIF
;
;	Initialize buddy-system memory for 512 KB RAM banks
;	
;	Affected regs: A,BC,DE,HL,IY
;
_Init512Banks:
	push	iy		;save IY
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
	ld	d,a
	ld	e,a		;DE=0=offset in lists headers
	ld	iy,Lists
	ld	a,BANKS_CNT
	ld	bc,4 * BANKS_CNT;delta for list headers
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

IF	OPTIM
				;init MaxAv vector
	ld	b,BANKS_CNT
	ld	hl,MaxAv
	ld	a,MAX_SIZE+1
loopav:	ld	(hl),a
	inc	hl
	djnz	loopav

ENDIF
	jp	TEXT

	psect	text
TEXT:
				;init L10 list headers for each bank
	ld	b,BANKS_CNT
	xor	a
	ld	hl,L10		;HL=L10 lists header
initL10:
	push	af
	push	bc
	push	hl
	add	a,32+16		;first physical RAM bank
	out	(PORT),a	;select physical RAM bank number A in logical bank 2 (8000-C000)
	ld	de,Buf16K
	call	AddToL	;add DE to HL header
	ld	hl,Buf16K+OFF_STS;HL=pointer of block status
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
	djnz	initL10

	ld	a,32+2		;restore initial physical RAM bank to 8000
	out	(PORT),a

	pop	iy		;restore IY
	ret

