;
;	Parts of LIBC.LIB
;
;	dissasembled by Andrey Nikitin & Ladislau Szilagyi
;
;=========================================================
; 8018 csv:	csv.asm
; 8024 cret:	
; 802B indir:	
; 802C ncsv:	
;=========================================================		
	global	csv, cret, indir, ncsv

	psect	text

csv:	pop	hl		;return address
	push	iy
	push	ix
	ld	ix,0
	add	ix,sp		;new frame pointer
	jp	(hl)

cret:	ld	sp,ix
	pop	ix
	pop	iy
	ret

indir:	jp	(hl)

; New csv: allocates space for stack based on word following
; call ncsv

ncsv:
	pop	hl
	push	iy
	push	ix
	ld	ix,0
	add	ix,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ex	de,hl
	add	hl,sp
	ld	sp,hl
	ex	de,hl
	jp	(hl)

	psect	text
	global	rcsv, cret, _rindex

	global	rcsv

ARG	equ	6		;offset of 1st arg
	psect	text
rcsv:
	ex	(sp),iy		;save iy, get return address
	push	ix
	ld	ix,0
	add	ix,sp		;new frame pointer
	ld	l,(ix+ARG+0)
	ld	h,(ix+ARG+1)
	ld	e,(ix+ARG+2)
	ld	d,(ix+ARG+3)
	ld	c,(ix+ARG+4)
	ld	b,(ix+ARG+5)
	jp	(iy)

_rindex:
	call	rcsv

	ld	bc,0
	jr	5f
6:
	inc	hl
	inc	bc
5:
	ld	a,(hl)
	or	a
	jr	nz,6b
1:
	dec	hl
	ld	a,c
	or	b
	jr	z,2f
	dec	bc
	ld	a,(hl)
	cp	e
	jr	nz,1b
4:	jp	cret

2:	ld	hl,0
	jp	4b

;**************************************************************************
;	16 bit divide and modulus routines

;	called with dividend in hl and divisor in de

;	returns with result in hl.

;	adiv (amod) is signed divide (modulus), ldiv (lmod) is unsigned

	global	adiv,ldiv,amod,lmod
	psect	text

amod:
	call	adiv
	ex	de,hl		;put modulus in hl
	ret

lmod:
	call	ldiv
	ex	de,hl
	ret

ldiv:
	xor	a
	ex	af,af'
	ex	de,hl
	jr	dv1


adiv:
	ld	a,h
	xor	d		;set sign flag for quotient
	ld	a,h		;get sign of dividend
	ex	af,af'
	call	negif16
	ex	de,hl
	call	negif16
dv1:	ld	b,1
	ld	a,h
	or	l
	ret	z
dv8:	push	hl
	add	hl,hl
	jr	c,dv2
	ld	a,d
	cp	h
	jr	c,dv2
	jp	nz,dv6
	ld	a,e
	cp	l
	jr	c,dv2
dv6:	pop	af
	inc	b
	jp	dv8

dv2:	pop	hl
	ex	de,hl
	push	hl
	ld	hl,0
	ex	(sp),hl

dv4:	ld	a,h
	cp	d
	jr	c,dv3
	jp	nz,dv5
	ld	a,l
	cp	e
	jr	c,dv3

dv5:	sbc	hl,de
dv3:	ex	(sp),hl
	ccf
	adc	hl,hl
	srl	d
	rr	e
	ex	(sp),hl
	djnz	dv4
	pop	de
	ex	de,hl
	ex	af,af'
	call	m,negat16
	ex	de,hl
	or	a			;test remainder sign bit
	call	m,negat16
	ex	de,hl
	ret

negif16:bit	7,h
	ret	z
negat16:ld	b,h
	ld	c,l
	ld	hl,0
	or	a
	sbc	hl,bc
	ret

;	16 bit integer multiply

;	on entry, left operand is in hl, right operand in de

	psect	text
	global	amul,lmul
amul:
lmul:
	ld	a,e
	ld	c,d
	ex	de,hl
	ld	hl,0
	ld	b,8
	call	mult8b
	ex	de,hl
	jr	3f
2:	add	hl,hl
3:
	djnz	2b
	ex	de,hl
1:
	ld	a,c
mult8b:
	srl	a
	jp	nc,1f
	add	hl,de
1:	ex	de,hl
	add	hl,hl
	ex	de,hl
	ret	z
	djnz	mult8b
	ret

;********************************************************************

;	Long division routines for Z80

	global	lldiv,aldiv,almod,llmod,aslldiv,asaldiv,asllmod,asalmod

	psect	text

;	Called with dividend in HLDE, divisor on stack under 2 return
;	addresses. Returns with dividend in HL/HL', divisor in DE/DE'
;	on return the HIGH words are selected.

lregset:
	pop	bc			;get top return address
	exx				;select other bank
	pop	bc			;return address of call to this module
	pop	de			;get low word of divisor
	exx				;select hi bank
	ex	de,hl			;dividend.low -> hl
	ex	(sp),hl			;divisor.high -> hl
	ex	de,hl			;dividend.high -> hl
	exx				;back to low bank
	push	bc			;put outer r.a. back on stack
	pop	hl			;return address
	ex	(sp),hl			;dividend.low -> hl
	exx
	push	bc			;top return address
	ret

;	Much the same as lregset, except that on entry the dividend
;	is pointed to by HL.
;	The pointer is saved in iy for subsequent updating of memory

iregset:
	pop	de			;immediate return address
	call	lregset			;returns with hi words selected
	push	hl			;save a copy for 'ron
	ex	(sp),iy			;get it in iy, saving old iy
	ld	h,(iy+3)		;high order byte
	ld	l,(iy+2)		;byte 2
	exx				;back to low bank
	push	hl			;return address
	ld	h,(iy+1)		;byte 1
	ld	l,(iy+0)		;and LSB
	exx				;restore hi words
	ret				;now return

;	Called with hi words selected, performs division on the absolute
;	values of the dividend and divisor. Quotient is positive

sgndiv:
	call	negif			;make dividend positive
	exx
	ex	de,hl			;put divisor in HL/HL'
	exx
	ex	de,hl
	call	negif			;make divisor positive
	ex	de,hl			;restore divisor to DE/DE'
	exx
	ex	de,hl
	exx				;select high words again
	jp	divide			;do division

asaldiv:
	call	iregset
	call	dosdiv
store:
	ld	(iy+0),e
	ld	(iy+1),d
	ld	(iy+2),l
	ld	(iy+3),h
	pop	iy			;restore old iy
	ret

aldiv:	call	lregset			;get args

;	Called with high words selected, performs signed division by
;	the rule that the quotient is negative iff the signs of the dividend
;	and divisor differ
;	returns quotient in HL/DE

dosdiv:
	ld	a,h
	xor	d
	ex	af,af'			;sign bit is now sign of quotient
	call	sgndiv			;do signed division
	ex	af,af'			;get sign flag back
	push	bc			;high word
	exx
	pop	hl
	ld	e,c			;low word of quotient
	ld	d,b
	jp	m,negat			;negate quotient if necessary
	ret

lldiv:	call	lregset

;	Called with high words selected, performs unsigned division
;	returns with quotient in HL/DE

doudiv:
	call	divide			;unsigned division
	push	bc			;high word of quotien
	exx
	pop	hl
	ld	e,c			;low word
	ld	d,b
	ret

aslldiv:
	call	iregset
	call	doudiv
	jp	store

almod:
	call	lregset

;	Called with high words selected, performs signed modulus - the rule
;	is that the sign of the remainder is the sign of the dividend

dosrem:
	ld	a,h			;get sign of dividend
	ex	af,af'			;save it
	call	sgndiv			;do signed division
	push	hl			;high word
	exx
	pop	de
	ex	de,hl				;put high word in hl
	ex	af,af'				;get sign bit back
	or	a
	jp	m,negat			;negate if necessary
	ret

asalmod:
	call	iregset
	call	dosrem
	jp	store

llmod:
	call	lregset

;	Called with high words selected, perform unsigned modulus

dourem:
	call	divide
	push	hl			;high word of remainder
	exx
	pop	de
	ex	de,hl				;high word in hl
	ret

asllmod:
	call	iregset
	call	dourem
	jp	store

;	Negate the long in HL/DE

negat:	push	hl			;save high word
	ld	hl,0
	or	a
	sbc	hl,de
	ex	de,hl
	pop	bc			;get high word back
	ld	hl,0
	sbc	hl,bc
	ret				;finito

negif:	;called with high word in HL, low word in HL'
	;returns with positive value

	bit	7,h			;check sign
	ret	z			;already positive
	exx				;select low word
	ld	c,l
	ld	b,h
	ld	hl,0
	or	a
	sbc	hl,bc
	exx
	ld	c,l
	ld	b,h
	ld	hl,0
	sbc	hl,bc
	ret				;finito

;	Called with dividend in HL/HL', divisor in DE/DE', high words in
;	selected register set
;	returns with quotient in BC/BC', remainder in HL/HL', high words
;	selected

divide:
	ld	bc,0			;initialize quotient
	ld	a,e			;check for zero divisor
	or	d
	exx
	ld	bc,0
	or	e
	or	d
	exx				;restor high words
	ret	z			;return with quotient == 0
	ld	a,1			;loop count
	jp	3f			;enter loop in middle
1:
	push	hl			;save divisor
	exx
	push	hl			;low word
	or	a			;clear carry
	sbc	hl,de			;subtract low word
	exx
	sbc	hl,de			;sbutract hi word
	exx
	pop	hl			;restore dividend
	exx
	pop	hl			;and hi word
	jr	c,2f			;finished - divisor is big enough
	exx
	inc	a			;increment count
	ex	de,hl			;put divisor in hl - still low word
	add	hl,hl			;shift left
	ex	de,hl				;put back in de
	exx				;get hi word
	ex	de,hl
	adc	hl,hl			;shift with carry
	ex	de,hl
3:
	bit	7,d			;test for max divisor
	jp	z,1b			;loop if msb not set

2:	;arrive here with shifted divisor, loop count in a, and low words
	;selected

	
3:
	push	hl			;save dividend
	exx
	push	hl			;low word
	or	a			;clear carry
	sbc	hl,de
	exx
	sbc	hl,de
	exx				;restore low word
	jp	nc,4f
	pop	hl			;restore low word of dividend
	exx
	pop	hl			;hi word
	exx				;restore low word
	jr	5f
4:
	inc	sp			;unjunk stack
	inc	sp
	inc	sp
	inc	sp
5:
	ccf				;complement carry bit
	rl	c			;shift in carry bit
	rl	b			;next byte
	exx				;hi word
	rl	c
	rl	b
	srl	d			;now shift divisor right
	rr	e
	exx				;get low word back
	rr	d
	rr	e
	exx				;select hi word again
	dec	a			;decrement loop count
	jr	nz,3b
	ret				;finished

;	Long increment

	psect	text

	global	lainc, llinc, ladec, lldec

gval:
	exx
	pop	hl		;return address
	exx
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	push	de		;lo word
	push	bc		;hi word
	push	bc
	ex	(sp),hl
	exx
	push	hl		;return address
	exx
	ret

lainc:
llinc:
	call	gval
	ld	bc,1
	ex	de,hl
	add	hl,bc
	ex	de,hl
	ld	c,0
	adc	hl,bc

sval:
	ex	(sp),hl
	pop	bc
	ld	(hl),b
	dec	hl
	ld	(hl),c
	dec	hl
	ld	(hl),d
	dec	hl
	ld	(hl),e
	pop	hl		;restore original value
	pop	de
	ret

lldec:
ladec:
	call	gval
	ld	bc,-1
	ex	de,hl
	add	hl,bc
	ex	de,hl
	adc	hl,bc
	jr	sval

;	logical long right shift
;	value in HLDE, count in B

	global	llrsh
	psect	text

llrsh:
	ld	a,b		;check for zero shift
	or	a
	ret	z
	cp	33
	jr	c,1f		;limit shift to 32 bits
	ld	b,32
1:
	srl	h
	rr	l
	rr	d
	rr	e
	djnz	1b
	ret

;	Long multiplication for Z80

;	Called with 1st arg in HLDE, 2nd arg on stack. Returns with
;	result in HLDE, other argument removed from stack

	global	almul, llmul

	psect	text
almul:
llmul:
	ex	de,hl
	ex	(sp),hl		;return address now in hl
	exx
	pop	de		;low word in de
	pop	bc		;low word of multiplier in bc
	exx
	pop	bc		;hi word of multiplier
	push	hl		;restore return address
	ld	hl,0		;initialize product
	exx			;get lo words back
	ld	hl,0
	ld	a,c
	ld	c,b
	call	mult8b_32
	ld	a,c
	call	mult8b_32
	exx
	ld	a,c
	exx
	call	mult8b_32
	exx
	ld	a,b
	exx
	call	mult8b_32
	push	hl		;low word
	exx
	pop	de
	ret

mult8b_32:ld	b,8
3:
	srl	a
	jp	nc,1f
	add	hl,de
	exx
	adc	hl,de
	exx
1:	ex	de,hl
	add	hl,hl
	ex	de,hl
	exx
	ex	de,hl
	adc	hl,hl
	ex	de,hl
	exx
	djnz	3b
	ret

;***************************************************************

;	routines to support the assignment versions of the long operations

	psect	text
	global	aslladd

liregset:
	ld	e,(hl)	;pick up arg
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	ex	(sp),hl	;save pointer, get return address
	push	bc	;now put the hi word into hl
	ex	(sp),hl	;saving return address on stack again
	pop	bc	;now return address in bc
	exx
	pop	hl	;the pointer
	pop	bc	;the final return address
	pop	de	;now the low word of arg2
	ex	(sp),hl	;get the hi word, save pointer
	push	bc	;this is the return address again
	ex	(sp),hl	;into hl again
	pop	bc	;hi word now in bc
	ex	(sp),hl	;AHA! got return address on stack, ptr in hl
	push	hl	;now its easy, save pointer
	push	bc	;hi word
	push	de	;low word
	exx
	push	bc	;immediate return address
	ret		;finito

liregstore:
	ex	(sp),hl	;get pointer into hl
	pop	bc	;hi word in bc
	ld	(hl),b
	dec	hl
	ld	(hl),c
	dec	hl
	ld	(hl),d
	dec	hl
	ld	(hl),e		;all done
	push	bc	;restore to hl
	pop	hl
	ret		;and return with value in hl

aslladd:
	call	liregset
	call	aladd
	jp	liregstore

;=========================================================
; * 70CB toupper
;=========================================================
;    char toupper(char c) {
;	if(c >= 'a' && c <= 'z') c += 'A' - 'a';
;	return c;
;    }
;
	global	_toupper
	global	brelop
	global	wrelop

	psect	text

_toupper:
	call	csv
	ld	b,'a'
	ld	a,(ix+6)
	call	brelop
	jp	m,loc_70F2
	ld	a,(ix+6)
	ld	e,a
	rla
	sbc	a,a
	ld	d,a
	ld	hl,'z'
	call	wrelop
	jp	m,loc_70F2
	ld	a,(ix+6)
	add	a,224	; 'A' - 'a'
	ld	l, a
	jp	cret
loc_70F2:
	ld	l,(ix+6)
	jp	cret	

;=========================================================
; 7F3D _brk:	sbrk.asm
; 7F45 _sbrk:	
; 7F71 _checksp:	
;=========================================================		
; NB This brk() does not check that the argument is reasonable.

	psect	text

	global	_sbrk,__Hbss, _brk
_brk:
	pop	hl	;return address
	pop	de	;argument
	ld	(memtop),de	;store it
	push	de		;adjust stack
	jp	(hl)	;return
_sbrk:
	pop	bc
	pop	de
	push	de
	push	bc
	ld	hl,(memtop)
	ld	a,l
	or	h
	jr	nz,1f
	ld	hl,__Hbss
	ld	(memtop),hl
1:
	add	hl,de
	jr	c,2f		;if overflow, no room
	ld	bc,1024		;allow 1k bytes stack overhead
	add	hl,bc
	jr	c,2f		;if overflow, no room
	sbc	hl,sp
	jr	c,1f
2:
	ld	hl,-1		;no room at the inn
	ret

1:	ld	hl,(memtop)
	push	hl
	add	hl,de
	ld	(memtop),hl
	pop	hl
	ret

	psect	bss
memtop:	defs	2

;=========================================================
; 7F90 shll:	shll.asm
;=========================================================
; Shift operations - the count is always in B,
; the quantity to be shifted is in HL, except for the
; assignment type operations, when it is in the memory
; location pointed to by HL

	global	shll,shal	;shift left, arithmetic or logical

	psect	text
shll:
shal:
	ld	a,b		;check for zero shift
	or	a
	ret	z
	cp	16		;16 bits is maximum shift
	jr	c,1f		;is ok
	ld	b,16
1:
	add	hl,hl		;shift left
	djnz	1b
	ret

;=========================================================
; 7B06 asll:	asll.asm
;=========================================================
; Shift operations - the count is always in B, the quantity
; to be shifted is in HL, except for the assignment type
; operations, when it is in the memory location pointed to
; by HL

	global	asll,asal	; assign shift left (logical or arithmetic)

	psect	text

	global	shal
asll:
asal:
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	push	hl		; save for the store
	ex	de,hl
	call	shal
	ex	de,hl
	pop	hl
	ld	(hl),d
	dec	hl
	ld	(hl),e
	ex	de,hl		; return value in hl
	ret

	global	aslr	;assign shift logical right
	psect	text
	global	shlr

aslr:
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	push	hl		;save for the store
	ex	de,hl
	call	shlr
	ex	de,hl
	pop	hl
	ld	(hl),d
	dec	hl
	ld	(hl),e
	ex	de,hl		;return value in hl
	ret

	global	shlr	;shift logical right
	psect	text

shlr:
	ld	a,b		;check for zero shift
	or	a
	ret	z
	cp	16		;16 bits is maximum shift
	jr	c,1f		;is ok
	ld	b,16
1:
	srl	h
	rr	l
	djnz	1b
	ret

	psect	text
	global	aladd, lladd

aladd:
lladd:
	exx
	pop	hl
	exx
	pop	bc
	ex	de,hl
	add	hl,bc
	ex	de,hl
	pop	bc
	adc	hl,bc
	exx
	push	hl
	exx
	ret


;=========================================================
; 7AE5 asar:	asar.asm
;=========================================================
; Shift operations - the count is always in B,
; the quantity to be shifted is in HL, except for the
; assignment  type operations, when it is in the memory
; location pointed to by HL

	global	asar	; assign shift arithmetic right

	psect	text

	global	shar

asar:
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	push	hl		; save for the store
	ex	de,hl
	call	shar
	ex	de,hl
	pop	hl
	ld	(hl),d
	dec	hl
	ld	(hl),e
	ex	de,hl		; return value in hl
	ret

;=========================================================
; 7F80 shar:	shar.asm
;=========================================================
; Shift operations - the count is always in B,
; the quantity to be shifted is in HL, except for the
; assignment type operations, when it is in the memory
; location pointed to by HL

	global	shar	;shift arithmetic right

	psect	text
shar:
	ld	a,b		;check for zero shift
	or	a
	ret	z
	cp	16		;16 bits is maximum shift
	jr	c,1f		;is ok
	ld	b,16
1:
	sra	h
	rr	l
	djnz	1b
	ret

;=========================================================
; 781E exit
;=========================================================
;	#include "stdio.h"
;
;	#define	uchar	unsigned char
;	#define	MAXFILE 5
;	#define WPOKE(addr, word) *((unsigned *)addr) = (unsigned)word
;
;	void exit(int code) {
;	    uchar i;
;	    register FILE * ip;
;
;	    i = MAXFILE;
;	    ip = _iob;
;	    do {
;		fclose(ip);
;		ip++;
;	    } while(--i);
;	    WPOKE(0x80, code);
;	#asm
;	    call   0	/* Exit; */
;	#endasm
;	}
;
	global	_exit
	global	__iob
	global	_fclose

	psect	text

_exit:
	call	csv
	push	hl
	ld	(ix+-1),5
	ld	iy,__iob
loc_782A:
	push	iy
	pop	hl
	ld	bc,8
	add	hl,bc
	push	hl
	pop	iy		
	sbc	hl,bc
	push	hl
	call	_fclose	
	pop	bc
	ld	a,(ix+-1)
	add	a,0FFh
	ld	(ix+-1),a
	or	a
	jr	nz,loc_782A
	ld	l,(ix+6)
	ld	h,(ix+7)
	ld	(128),hl
	call	0
	jp	cret
;
;=========================================================
;	Offsets of things in the _iob structure

	ptr	equ	0	; pointer to next byte
	cnt	equ	2	; number of bytes left
	base	equ	4	; beginning of buffer
	flag	equ	6	; flag bits
	file	equ	7	; file number

;	The bit numbers of the flags in flag
	
	_IOREAD_BIT	equ	0
	_IOWRT_BIT	equ	1
	_IONBF_BIT	equ	2
	_IOMYBUF_BIT	equ	3
	_IOEOF_BIT	equ	4
	_IOERR_BIT	equ	5
	_IOSTRG_BIT	equ	6
	_IOBINARY_BIT	equ	7

;	Various characters

	CPMEOF	equ	032q	; EOF byte 
	NEWLINE	equ	012q	; newline character 
	RETURN	equ	015q	; carriage return 

	EOF	equ	-1	; stdio EOF value
;
;	/*
;	 * STDIO.H	Modified version from Tesseract vol 91
;	 */
;	#if	z80
;	#define	BUFSIZ		512
;	#define	_NFILE		8
;	#else	z80
;	#define	BUFSIZ		1024
;	#define	_NFILE		20
;	#endif	z80
;
;	#ifndef FILE
;	#define	uchar	unsigned char
;
;	extern	struct	_iobuf
;	{
;    	char		*_ptr;
;    	int		 _cnt;
;    	char		*_base;
;    	unsigned short	 _flag;
;    	char		 _file;
;	} _iob[_NFILE];
;
	psect	data

FILE:	defs	8
;
	psect	text
;
	global	_sprintf
;
_sprintf:
	call csv
	ld hl,07fffh
	ld (FILE+cnt),hl
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (FILE+ptr),hl
	ld hl,FILE+flag
	ld (hl),082h	;_IOWRT_BIT _IOBINARY_BIT
	push ix
	pop de	
	ld hl,0000ah
	add hl,de	
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld hl,FILE
	push hl	
	call _doprnt
	pop bc	
	pop bc	
	pop bc	
	ld hl,(FILE+ptr)
	ld (hl),000h
	ld e,(ix+006h)
	ld d,(ix+007h)
	or a	
	sbc hl,de
	jp cret

;
; stdclean
;
global  _cleanup
global  ncsv, cret, indir
global  __iob
global  _fclose

	psect   text

_cleanup:
global csv
call csv
push hl
ld      (ix+-1),8
ld      iy,__iob
cl5:
push    iy
call    _fclose
pop     bc
ld      de,8
add     iy,de
ld      a,(ix+-1)
add     a,255
ld      (ix+-1),a
or      a
jp      nz,cl5
jp      cret

;**********************************************

psect   data

__iob:
global  __sibuf

defw    __sibuf
defw    0
defw    __sibuf
defb    9
defb    0
defw    0
defw    0
defw    0
defb    6
defb    1
defw    0
defw    0
defw    0
defb    6
defb    2
defb    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
defb    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
defb    0,0,0,0,0,0,0,0

;
;	setbuf
;
	psect text
	global _setbuf
;
_setbuf:
  CALL csv
  LD   L,(IX+06H)
  LD   H,(IX+07H)
  PUSH HL
  POP  IY
  BIT  3,(IY+06H)
  JR   NZ,l_5842
  LD   A,(IY+04H)
  OR   (IY+05H)
  JR   Z,l_5842
  LD   L,(IY+04H)
  LD   H,(IY+05H)
  PUSH HL
  CALL _buffree
  POP  BC
l_5842:
  LD   (IY+02H),00
  LD   (IY+03H),00
  LD   L,(IX+08H)
  LD   H,(IX+09H)
  LD   (IY+00H),L
  LD   (IY+01H),H
  LD   (IY+04H),L
  LD   (IY+05H),H
  LD   A,L
  OR   H
  JR   NZ,l_5872
  LD   A,(IY+06H)
  AND  0F7H
  LD   (IY+06H),A
  JP   cret
l_5872:
  SET  3,(IY+06H)
  BIT  1,(IY+06H)
  JP   Z,cret
  LD   (IY+02H),00
  LD   (IY+03H),02
  JP   cret
;
_buffree:
  CALL csv
  LD   L,(IX+06H)
  LD   H,(IX+07H)
  PUSH HL
  POP  IY
  LD   HL,(buf_81B2)
  LD   (IY+00H),L
  LD   (IY+01H),H
  LD   (buf_81B2),IY
  JP   cret
;
	global	_fopen
_fopen:
	call csv
	ld iy,__iob
	jr l755ah
l7544h:
	ld de,__fcb
	push iy
	pop hl	
	or a	
	sbc hl,de
	jr nz,l7555h
	ld hl,00000h
	jp cret
l7555h:
	ld de,00008h
	add iy,de
l755ah:
	ld a,(iy+006h)
	and 003h
	or a	
	jr nz,l7544h
	push iy
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call _freopen
	pop bc	
	pop bc	
	pop bc	
	jp cret

	psect bss

buf_81B2:defs	2
;

;
;
; fgets
;
global  _fgets
global  ncsv, cret, indir
global  _fgetc

	psect   text

_fgets:
global csv
call csv
push hl
push hl
ld      l,(ix+10)
ld      h,(ix+11)
push    hl
pop     iy
ld      l,(ix+6)
ld      h,(ix+7)
ld      (ix+-2),l
ld      (ix+-1),h
l41:
ld      l,(ix+8)
ld      h,(ix+9)
dec     hl
ld      (ix+8),l
ld      (ix+9),h
ld      a,l
or      h
jp      z,l5
push    iy
call    _fgetc
pop     bc
ld      (ix+-4),l
ld      (ix+-3),h
ld      de,-1
or      a
sbc     hl,de
jp      z,l5
ld      a,(ix+-4)
ld      l,(ix+6)
ld      h,(ix+7)
inc     hl
ld      (ix+6),l
ld      (ix+7),h
dec     hl
ld      (hl),a
cp      10
jp      nz,l41
l5:
ld      l,(ix+6)
ld      h,(ix+7)
ld      (hl),0
ld      e,(ix+-2)
ld      d,(ix+-1)
or      a
sbc     hl,de
jp      nz,cl6
ld      hl,0
jp      cret
cl6:
ld      l,(ix+-2)
ld      h,(ix+-1)
jp      cret
;
;=========================================================
; 768F fgetc(FILE * f) from libraty
;=========================================================
;    fgetc(f) register FILE * f; {
;	int c;
;	
;	if(f->_flag & _IOEOF || !(f->_flag & _IOREAD)) {
;    reteof:
;	    f->_flag |= _IOEOF;
;	    return EOF;
;	}
;    loop:
;	if(f->_cnt > 0) {
;	    c = (unsigned)*f->_ptr++;
;	    f->_cnt--;
;	} else if(f->_flag & _IOSTRG)
;	    goto reteof;
;	else
;	    c = _filbuf(f);
;	if(f->_flag & _IOBINARY)
;	    return c;
;	if(c == '\r')
;	    goto loop;
;	if(c == CPMEOF) {
;	    f->_cnt++;
;	    f->_ptr--;
;	    goto reteof;
;	}
;	return c;
;    }

;	The assembler version of the above routine

;*Include	stdio.i
	global	_fgetc, _filbuf
	psect	text

_fgetc:
	pop	de			;get return address off stack
	ex	(sp),iy			;save iy and get arguement into iy
	ld	a,(iy+flag)		;get flag bits
	bit	_IOREAD_BIT,a
	jr	z,reteof		;return EOF if not open for read
	bit	_IOEOF_BIT,a		;Already seen EOF?
	jr	nz,reteof		;yes, repeat ourselves

loop:
	ld	l,(iy+cnt)
	ld	h,(iy+cnt+1)
	ld	a,l
	or	h			;any bytes left?
	jr	z,1f			;no, go get some more
	dec	hl
	ld	(iy+cnt),l		;update count
	ld	(iy+cnt+1),h
	ld	l,(iy+ptr)		;get the pointer
	ld	h,(iy+ptr+1)
	ld	a,(hl)
	inc	hl
	ld	(iy+ptr),l		;update pointer
	ld	(iy+ptr+1),h
2:
	bit	_IOBINARY_BIT,(iy+flag)	;Binary mode?
	jr	z,3f			;no, check for EOF etc
retch:
	ld	l,a			;return the character in a
	ld	h,0
	ex	(sp),iy			;restore iy
	push	de			;put return address back
	ret				;with char in hl

3:
	cp	RETURN			;carriage return
	jr	z,loop			;yes, get another instead
	cp	CPMEOF			;end of file?
	jr	nz,retch		;no, return it!

;	ld	a,(iy+base)		;buffered?
;	or	(iy+base+1)
;	jr	z,reteof		;yup, leave count alone

	ld	l,(iy+cnt)
	ld	h,(iy+cnt+1)
	inc	hl			;reset count
	ld	(iy+cnt),l
	ld	(iy+cnt+1),h

	ld	l,(iy+ptr)
	ld	h,(iy+ptr+1)
	dec	hl			;reset pointer
	ld	(iy+ptr),l
	ld	(iy+ptr+1),h
reteof:
	set	_IOEOF_BIT,(iy+flag)	;note EOF
	ld	hl,EOF
	ex	(sp),iy			;restore iy
	push	de
	ret				;return with EOF in hl

1:
	bit	_IOSTRG_BIT,(iy+flag)	;end of string?
	jr	nz,reteof		;yes, return EOF
	push	de			;save de
	push	iy			;pass iy as argument
	call	_filbuf		;refill the buffer
	ld	a,l			;the returned value
	pop	bc
	pop	de			;return address in de again
	bit	7,h
	jr	nz,reteof		;returned EOF
	jr	2b

;=========================================================
; sub_770B filbuf			   Used in: fgetc
;=========================================================
;    int filbuf(register FILE * f) {
;	struct fcb * p;
;
;	p = _fcb + (f - stdin);
;	f->_cnt = 0;
;	sub_77F5();
;	if((char)p->rwp == 1) {
;	    for(f->_ptr = f->_base; f->_ptr < f->_base+BUFSIZ; f->_ptr += 0x80) {
;		bdos(CPMSDMA, f->_ptr);
;		if(bdos(CPMREAD, p) != 0) break;
;	    }
;	    f->_cnt = f->_ptr - f->_base;
;	    f->_ptr = f->_base;
;	    if(f->_cnt == 0) return -1;
;	    f->_cnt--;
;	    return (unsigned char)*(f->_ptr++);
;	}
;	return -1;
;    }
;
	global	_filbuf
	global	__fcb
	global	__iob
	global	adiv
	global	amul
	global	_test_brk
	global	_bdos
	global	wrelop

	psect	text

_filbuf:
	call	csv
	push	hl
	ld	l,(ix+6)
	ld	h,(ix+7)
	push	hl
	pop	iy
	ld	de,__iob
	push	iy
	pop	hl
	or	a
	sbc	hl,de
	ld	de,8
	call	adiv
	ld	de,41
	call	amul
	ld	de,__fcb
	add	hl,de
	ld	(ix+-2),l
	ld	(ix+-1),h

	ld	(iy+2),0
	ld	(iy+3),0

	call	_test_brk
	ld	e,(ix+-2)
	ld	d,(ix+-1)
	ld	hl,36
	add	hl,de
	ld	a,(hl)
	cp	1
	jp	nz,loc_77CE

	ld	l,(iy+4)
	ld	h,(iy+5)
	ld	(iy+0),l
	ld	(iy+1),h
	jr	loc_7793

loc_7760:
	ld	l,(iy+0)
	ld	h,(iy+1)
	push	hl
	ld	hl,26 ; Set DMA adr
	push	hl
	call	_bdos
	pop	bc
	ld	l,(ix+-2)
	ld	h,(ix+-1)
	ex	(sp),hl
	ld	hl,20 ; Read next record
	push	hl
	call	_bdos
	pop	bc
	pop	bc
	ld	a,l
	or	a
	jr	nz,loc_77A9

	ld	de,128
	ld	l,(iy+0)
	ld	h,(iy+1)
	add	hl,de
	ld	(iy+0),l
	ld	(iy+1),h

loc_7793:
	ld	e,(iy+4)
	ld	d,(iy+5)
	ld	hl,512
	add	hl,de
	ex	de,hl
	ld	l,(iy+0)
	ld	h,(iy+1)
	call	wrelop
	jr	c,loc_7760

loc_77A9:
	ld	e,(iy+4)
	ld	d,(iy+5)
	ld	l,(iy+0)
	ld	h,(iy+1)
	or	a
	sbc	hl,de
	ld	(iy+2),l
	ld	(iy+3),h
	ld	l,e
	ld	h,d
	ld	(iy+0),l
	ld	(iy+1),h
	ld	a,(iy+2)
	or	(iy+3)
	jr	nz,loc_77D4

loc_77CE:
	ld	hl,-1
	jp	cret

loc_77D4:
	ld	l,(iy+2)
	ld	h,(iy+3)
	dec	hl
	ld	(iy+2),l
	ld	(iy+3),h
	ld	l,(iy+0)
	ld	h,(iy+1)
	inc	hl
	ld	(iy+0),l
	ld	(iy+1),h
	dec	hl
	ld	l,(hl)
	ld	h,0
	jp	cret

;=========================================================
; sub_7855 fclose
;=========================================================
;    int fclose(register FILE * f) {
;	struct fcb * p;
;
;	p = _fcb + (f - stdin);
;	if(!(f->_flag & _IORW)) return EOF; 
;	fflush(f);
;	f->_flag &= ~(_IOREAD|_IOWRT|_IONBF);
;	if((char)p->rwp == 2) goto l1;
;	if(!(ISMPM())) goto l2;
;    l1:
;	bdos(CPMCLS, p);	/* close file */
;    l2:
;	LO_CHAR(p->rwp) = 0;
;	return 0;
;    }
;
	global	_fclose
	global	__fcb
	global	__iob
	global	adiv
	global	amul
	global	_fflush
	global	_bdoshl
	global	_bdos

	psect	text

_fclose:
	call	csv
	push	hl
	ld	l,(ix+6)
	ld	h,(ix+7)
	push	hl
	pop	iy
	ld	de,__iob
	push	iy
	pop	hl
	or	a
	sbc	hl,de
	ld	de,8
	call	adiv
	ld	de,41
	call	amul
	ld	de,__fcb
	add	hl,de
	ld	(ix+-2),l
	ld	(ix+-1),h
	ld	a,(iy+6)
	and	3
	or	a
	jr	nz,loc_788F
	ld	hl,-1
	jp	cret
loc_788F:
	push	iy
	call	_fflush
	pop	bc
	ld	a,(iy+6)
	and	248
	ld	(iy+6),a
	ld	e,(ix+-2)
	ld	d,(ix+-1)
	ld	hl,36
	add	hl,de
	ld	a,(hl)
	cp	2
	jr	z,l6
	ld	hl,12
	push	hl
	call	_bdoshl
	pop	bc
	bit	0,h
	jr	z,l8
l6:
	ld	l,(ix+-2)
	ld	h,(ix+-1)
	push	hl
	ld	hl,16	; CPMCLS - close file
	push	hl
	call	_bdos
	pop	bc
	pop	bc
l8:
	ld	e,(ix+-2)
	ld	d,(ix+-1)
	ld	hl,36
	add	hl,de
	ld	(hl),0
	ld	hl,0
	jp	cret

;=========================================================
; 78DA _bdoshl - from libraty
;=========================================================
;	Bdos calls which return values in HL
;	short	bdoshl(fun, arg);

psect	text

entry	equ	5		; CP/M entry point

arg	equ	8		; argument to call
func	equ	6		; desired function


	global	_bdoshl
	global	csv,cret

	psect	text

_bdoshl:
	call	csv
	ld	e,(ix+arg)
	ld	d,(ix+arg+1)
	ld	c,(ix+func)
	push	ix
	call	entry
	pop	ix
	jp	cret		; return value already in hl

;=========================================================
; sub_78F0 - fflush
;=========================================================
;	int fflush(register FILE * f) {
;	    struct fcb * p;
;
;	    p = _fcb + (f - stdin);
;
;	    if(!(f->_flag & _IOWRT)) return -1;
;
;	    if(((unsigned int)f->_cnt & 0x7F) != 0) {
;	        if(p->rwp > 4)  {
;	            *(f->_ptr) = CPMETX;
;	            count--;
;	        }
;	    }
;	    if((f->_ptr = f->_base) == 0) return 0;
;	    f->_cnt = BUFSIZ - f->_cnt;
;	    switch((char)p->rwp) {
;	      case 2:
;		f->_cnt = (f->_cnt+127)/128;
;		for(;;) {
;		    if(f->_cnt-- == 0) break;
;		    bdos(CPMSDMA, f->_ptr);
;		    bdos(CPMWRIT, p);
;		    if(bdos(CPMWRIT, p) != 0) break;
;		    f->_ptr += 128;
;	    	}
;	      case 4:
;		for(;;) {
;		    if(f->_cnt-- == 0) break;
;		    bdos(CPMWCON, *(f->_ptr++));
;	    	}
;	    }
;	    f->_cnt = 0;
;	    f->_ptr = f->_base;
;	    LO_CHAR(f->_cnt) = 0;
;	    HI_CHAR(f->_cnt) = 2;
;	    return 0;
;	}
;
	global	_fflush
	global	__fcb
	global	__iob
	global	adiv
	global	amul
	global	wrelop
	global	_bdos

	psect	text

_fflush:
	call	csv
	push	hl
	ld	l,(ix+6)
	ld	h,(ix+7)
	push	hl
	pop	iy
	ld	de,__iob
	push	iy
	pop	hl
	or	a
	sbc	hl,de
	ld	de,8
	call	adiv
	ld	de,41
	call	amul
	ld	de,__fcb
	add	hl,de
	ld	(ix+-2),l
	ld	(ix+-1),h
	bit	1,(iy+6)
	jr	nz,loc_7928
	ld	hl,-1
	jp	cret
loc_7928:			; m0:
	ld	a,(iy+2)
	and	127
	ld	l,a
	xor	a
	ld	h,a
	ld	a,l
	or	h
	jr	z,loc_795B
	ld	b,4	
	ld	e,(ix+-2)
	ld	d,(ix+-1)
	ld	hl,36
	add	hl,de	
	ld	a,(hl)	
	call	brelop	
	jr	nc,loc_795B
	ld	l,(iy+0)
	ld	h,(iy+1)
	ld	(hl),26
	ld	l,(iy+2)
	ld	h,(iy+3)
	dec	hl
	ld	(iy+2),l
	ld	(iy+3),h
loc_795B:			; m1:
	ld	l,(iy+4)
	ld	h,(iy+5)
	ld	(iy+0),l
	ld	(iy+1),h
	ld	a,l
	or	h
	jr	nz,loc_7971
loc_796B:			; m2:
	ld	hl,0
	jp	cret
loc_7971:			; m3:
	ld	e,(iy+2)
	ld	d,(iy+3)
	ld	hl,512
	or	a
	sbc	hl,de
	ld	(iy+2),l
	ld	(iy+3),h

	ld	e,(ix+-2)
	ld	d,(ix+-1)
	ld	hl,36
	add	hl,de
	ld	a,(hl)
	cp	2
	jr	z,loc_79E4
	cp	4
	jr	z,loc_79D0
loc_7996:			; m4:
	ld	(iy+2),0
	ld	(iy+3),0
	ld	l,(iy+4)
	ld	h,(iy+5)
	ld	(iy+0),l
	ld	(iy+1),h
	ld	(iy+2),0
	ld	(iy+3),2
	jr	loc_796B
loc_79B4:			; m5:
	ld	l,(iy+0)
	ld	h,(iy+1)
	ld	a,(hl)
	inc	hl
	ld	(iy+0),l
	ld	(iy+1),h
	ld	l,a
	rla
	sbc	a,a
	ld	h,a
	push	hl
	ld	hl,2
	push	hl
	call	_bdos
	pop	bc
	pop	bc
loc_79D0:	; case 4:	; m6:
	ld	l,(iy+2)
	ld	h,(iy+3)
	dec	hl
	ld	(iy+2),l
	ld	(iy+3),h
	inc	hl
	ld	a,l
	or	h
	jr	nz,loc_79B4
	jr	loc_7996
loc_79E4:	; case 2:	; m7:
	ld	e,(iy+2)
	ld	d,(iy+3)
	ld	hl,7Fh 	
	add	hl,de	
	ld	de,80h  
	call	adiv    
	ld	(iy+2),l
	ld	(iy+3),h
	jr	loc_7A30
loc_79FC:			; m8:
	ld	l,(iy+0)
	ld	h,(iy+1)
	push	hl
	ld	hl,1Ah ;CPMSDMA
	push	hl
	call	_bdos
	pop	bc

	ld	l,(ix+-2)
	ld	h,(ix+-1)
	ex	(sp),hl
	ld	hl,15h ;CPMWRIT
	push	hl
	call	_bdos
	pop	bc
	pop	bc
	ld	a,l
	or	a
	jp	nz,loc_7996
	ld	de,80h
	ld	l,(iy+0)
	ld	h,(iy+1)
	add	hl,de
	ld	(iy+0),l
	ld	(iy+1),h
loc_7A30:			; m9:
	ld	l,(iy+2)
	ld	h,(iy+3)
	dec	hl
	ld	(iy+2),l
	ld	(iy+3),h
	inc	hl
	ld	a,l
	or	h
	jr	nz,loc_79FC
	jp	loc_7996

;=========================================================
; 7A45 bdos char bdos(func, arg)	from bdos.asm
;=========================================================

;entry	equ	5		; CP/M entry point

;arg	equ	8		; argument to call
;func	equ	6		; desired function

	global	_bdos

	psect	text
_bdos:
	call	csv
	ld	e,(ix+arg)
	ld	d,(ix+arg+1)
	ld	c,(ix+func)
	push	ix
;	push	iy
	call	entry
;	pop	iy
	pop	ix
	ld	l,a
	rla
	sbc	a,a
	ld	h,a
	jp	cret

;********************************************************

	global	_fputs
_fputs:
	call csv
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	pop iy
	jr l7b55h
l7b3bh:
	push iy
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld a,(hl)	
	inc hl	
	ld (ix+006h),l
	ld (ix+007h),h
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	push hl	
	call _fputc
	pop bc	
	pop bc	
l7b55h:
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld a,(hl)	
	or a	
	jr nz,l7b3bh
	jp cret

	global	_putchar

_putchar:
	call csv
	ld hl,__iob+8
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call _fputc
	pop bc	
	pop bc	
	jp cret

;=========================================================
; 75A2 fputc(uchar c, FILE * f) from library
;=========================================================
;	#include	<stdio.h>
;	
;	fputc(c, f) register FILE * f; uchar c; {
;	    if(!(f->_flag & _IOWRT)) return EOF;
;	    if((f->_flag & _IOBINARY) == 0 && c == '\n')
;	        fputc('\r', f);
;		if(f->_cnt > 0) {
;		    f->_cnt--;
;		    *f->_ptr++ = c;
;		} else
;		    return _flsbuf(c, f);
;		return c;
;	}
;
;*Include	stdio.i

	global	_fputc, _flsbuf
	psect	text
_fputc:
	pop	de			;return address
	pop	bc			;character argument
	ld	b,0			;so zero the top byte
	ex	(sp),iy			;save iy and get file pointer
	bit	_IOWRT_BIT,(iy+flag)	;are we reading
	jr	z,reteof1
	bit	_IOBINARY_BIT,(iy+flag)	;binary mode?
	jr	nz,2f			;yes, just return
	ld	a,c			;is it a newline?
	cp	NEWLINE
	jr	nz,2f			;no
	push	bc			;save thingos
	push	de
	push	iy			;file argument
	ld	hl,RETURN
	push	hl
	call	_fputc
	pop	hl			;unjunk stack
	pop	bc
	pop	de
	pop	bc
2:
	ld	l,(iy+cnt)
	ld	h,(iy+cnt+1)
	ld	a,l			;check count
	or	h
	jr	z,1f			;no room at the inn
	dec	hl			;update count
	ld	(iy+cnt),l
	ld	(iy+cnt+1),h
	ld	l,(iy+ptr)
	ld	h,(iy+ptr+1)		;get pointer
	ld	(hl),c			;store character
	inc	hl			;bump pointer
	ld	(iy+ptr),l
	ld	(iy+ptr+1),h
3:
	ex	(sp),iy			;restore iy
	push	bc			;fix stack up
	push	de
	ld	l,c
	ld	h,b			;return the character
	ret

1:
	ex	(sp),iy			;restore the stack to what it was
	push	bc
	push	de			;return address and all
	jp	_flsbuf		;let flsbuf handle it

reteof1:
	ld	bc,-1
	jr	3b

;=========================================================
; 7CDC brelop:	brelop.asm
;=========================================================
; byte relational operation - returns flags correctly for
; comparision of words in a and c

	psect	text

	global	brelop

brelop:
	push	de
	ld	e,a
	xor	b	;compare signs
	jp	m,1f	;if different, return sign of lhs
	ld	a,e
	sbc	a,b
	pop	de
	ret
1:
	ld	a,e	;get sign of lhs
	and	80h	;mask out sign flag
	ld	d,a
	ld	a,e
	sbc	a,b	;set carry flag if appropriate
	ld	a,d
	inc	a	;set sign flag as appropriate and reset Z flag
	pop	de
	ret

;=========================================================
;7CF0 wrelop:	wrelop.asm
;=========================================================
; word relational operation - returns flags correctly for
; comparision of words in hl and de

	psect	text

	global	wrelop

wrelop:
	ld	a,h
	xor	d	;compare signs
	jp	m,1f	;if different, return sign of lhs
	sbc	hl,de	;just set flags as normal
	ret
1:
	ld	a,h	;get sign of lhs
	and	80h	;mask out sign flag
	sbc	hl,de	;set carry flag if appropriate
	inc	a	;set sign flag as appropriate and reset Z flag
	ret


;=========================================================
; 7A5F start1.asm
;=========================================================
	psect	text

	global	_getargs, startup, __argc_, __sibuf

startup:
	jp	_getargs

	psect	bss

__sibuf:	defs	512
__argc_:	defs	2

	psect	text

;=========================================================
; 7A62  start2.asm
;=========================================================
	global	_getargs, __argc_, nularg

	psect	text

_getargs:
	pop	hl		; return address
	exx
	pop	hl
	pop	hl		; unjunk stack
	ld	a,(80h)		; get buffer length
	inc	a		; allow for null byte
	neg
	ld	l,a
	ld	h,-1
	add	hl,sp
	ld	sp,hl		; allow space for args
	ld	bc,0		; flag end of args
	push	bc
	ld	hl,80h		; address of argument buffer
	ld	c,(hl)
	ld	b,0
	add	hl,bc
	ld	b,c
	ex	de,hl
	ld	hl,(6)
	ld	c,1
	dec	hl
	ld	(hl),0
	inc	b
	jr	3f

2:	ld	a,(de)
	cp	' '
	dec	de
	jr	nz,1f
	push	hl
	inc	c
4:	ld	a,(de)		; remove extra spaces
	cp	' '
	jr	nz,5f
	dec	de
	jr	4b
5:
	xor	a
1:	dec	hl
	ld	(hl),a
3:
	djnz	2b
	ld	(__argc_),bc	; store argcount
	ld	hl,nularg
	push	hl
	ld	hl,0
	add	hl,sp
	exx
	push	de		; junk the stack again
	push	de
	push	hl		; return address
	exx
	ret

;=========================================================
; 708D printf
;=========================================================
;    printf(char * f, int a) {
;
;	return(_doprnt(stdout, f, &a));
;    }
;
	global	_printf
	global	_doprnt
	global	__iob

	psect	text

_printf:
	call csv
	push	ix
	pop	de
	ld	hl,8
	add	hl,de
	push	hl
	ld	l,(ix+6)
	ld	h,(ix+7)
	push	hl
	ld	hl,__iob+8
	push	hl
	call	_doprnt
	pop	bc
	pop	bc
	pop	bc
	jp	cret

;=========================================================
; sub_7325 _doprnt(file, f, a)
;=========================================================
;	_doprnt(FILE * file, register char * f, int * a) {
;	    char  c;
;	    uchar sign, len, width;
;	    long  xxxx;
;	    int   yyyy
;	    ...
;	}
;
	global	_doprnt
	global	_strlen
	global	brelop
	global	wrelop
	global	_atoi
	global	__ctype_
	global	_pnum
	global	_pputc
	global	_left
	global	_base
	global	_ffile

	psect	text

_doprnt:				; sub_7325:
	call	ncsv
	defw	-10	; 0FFF6h
	ld	l,(ix+8)
	ld	h,(ix+9)		;
	push	hl			;
	pop	iy			; f

	ld	l,(ix+6)		;
	ld	h,(ix+7)		;
	ld	(_ffile),hl		; ffile = file;
	jp	loc_74A7		; goto m15;

loc_733F:			; m1:
	ld	a,(ix+-1)
	cp	'%'
	jr	z,loc_7351		; if(c == '%') goto m3;
	
	ld	l,a
	ld	h,0
	push	hl

loc_734A:			; m2:
	call	_pputc			; pputc(c);
	pop	bc
	jp	loc_74A7		; goto m15;

loc_7351:			; m3:
	ld	a,0Ah
	ld	(_base),a		; base = 10;
	ld	(ix+-4),0FFh		; -4 width = -1;
	ld	(ix+-2),0		; -2 sign  =  0;
	ld	a,0
	ld	(_left),a		; left = 0;

	ld	(ix+-3),1		; -3 len = sizeof(int)/sizeof *a;

loc_7367:			; m4:
	ld	a,(iy+0)
	inc	iy
	ld	(ix+-1),a		; -1 c = *f++
	or	a			; if(c == 0) return;
	jp	z,cret			; switch(c){
	cp	'*'			; case: '*':
	jr	z,loc_73CF		;   goto m
	cp	'.'			; case: '.':
	jp	z,loc_7565		;   goto m
	cp	'c'			; case: 'c':
	jp	z,loc_74CD		;   goto m
	cp	'd'			; case: 'd':
	jp	z,loc_74B6		;   goto m
	cp	'l'			; case: 'l':
	jr	z,loc_73E0		;   goto m
	cp	'o'			; case: 'o':
	jr	z,loc_73E6		;   goto m
	cp	'x'			; case: 'x':
	jr	z,loc_x			;   goto m
	cp	's'			; case: 's':
	jp	z,loc_74F1		;   goto m
	cp	'u'			; case: 'u':
	jr	z,loc_73EB		;   goto m9;

	ld	b,30h	; '0'
	call	brelop
	jp	m,loc_7599		; if((c >= '0') || (c > '9') goto m26;

	ld	a,(ix+-1)
	ld	e,a
	rla
	sbc	a,a
	ld	d,a
	ld	hl,39h	; '9'
	call	wrelop
	jp	m,loc_7599		; if() goto m26;

	ld	de,0Ah
	ld	a,(_left)
	ld	l,a
	rla
	sbc	a,a
	ld	h,a
	call	amul ; hl*de
	ld	a,l
	ld	(_left),a		; left *= 10;

	ld	a,(ix+-1)
	add	a,0D0h		
	ld	e,a			; e = c+0xd0
	ld	a,(_left)		; left +c+0xd0
	add	a,e

loc_73CA:			; m5:
	ld	(_left),a		; left =
	jr	loc_7367		; goto m4;

loc_73CF:	; '*'		; m6:
	ld	l,(ix+0Ah)
	ld	h,(ix+0Bh) ; a
	ld	a,(hl)
	inc	hl
	inc	hl
	ld	(ix+0Ah),l
	ld	(ix+0Bh),h		; a += sizeof(char*)/sizeof *a;
	jr	loc_73CA		; goto m5;

loc_73E0:	; 'l'		; m7:
	ld	(ix+-3),2		; len = 2;
	jp	loc_7367		; goto m4;

loc_x:		; 'x'
	ld	a,16
	ld	(_base),a		; base = 16 ;
	jr	loc_73EB

loc_73E6:	; 'o'		; m8:
	ld	a,8
	ld	(_base),a		; base = 8 ;

loc_73EB:	; 'u'		; m9:

	ld	l,(ix+0Ah)
	ld	h,(ix+0Bh) ; a
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	a,d
	rla
	sbc	a,a
	ld	l,a
	ld	h,a
	ld	(ix+-8),e
	ld	(ix+-7),d
	ld	(ix+-6),l
	ld	(ix+-5),h		; a =

	ld	a,(ix+-2) ; sign	;
	or	a			;
	jr	nz,loc_7423		; if(sign != 0) goto m10;

	ld	l,(ix+0Ah)
	ld	h,(ix+0Bh) ; a
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	hl,0
	ld	(ix+-8),e
	ld	(ix+-7),d
	ld	(ix+-6),l
	ld	(ix+-5),h ;

loc_7423:			; m10:
	ld	a,(ix+-3) ; len	;
	cp	1			;
	jr	z,loc_7444		; if(len == 1) goto m11;

	ld	l,(ix+0Ah)
	ld	h,(ix+0Bh) ; a
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a
	ld	(ix+-8),e
	ld	(ix+-7),d
	ld	(ix+-6),l
	ld	(ix+-5),h ; xxxx 

loc_7444:			; m11:
	ld	a,(ix+-2) ; sign
	or	a
	jr	z,loc_7480		; if(sign == 0)  goto m12;

	bit	7,(ix+-5)

	ld	e,(ix+-8)
	ld	d,(ix+-7)
	ld	l,(ix+-6)
	ld	h,(ix+-5) ; xxxx
	push	hl
	push	de
	jr	z,loc_748E		; if( == ) goto m13;

	ld	hl,0
	pop	bc
	or	a
	sbc	hl,bc
	pop	bc
	ex	de,hl
	ld	hl,0
	sbc	hl,bc
	ld	(ix+-8),e
	ld	(ix+-7),d
	ld	(ix+-6),l
	ld	(ix+-5),h ; xxxx

	ld	hl,'-'
	push	hl
	call	_pputc			; pputc('-');
	pop	bc

loc_7480:			; m12:
	ld	e,(ix+-8)
	ld	d,(ix+-7)
	ld	l,(ix+-6)
	ld	h,(ix+-5) ; xxxx
	push	hl
	push	de

loc_748E:			; m13:
	call	_pnum			;
	pop	bc			;
	pop	bc			;
	ld	l,(ix+-3) ; len	;
	ld	h,0			;
	add	hl,hl			;
	ex	de,hl			;
	ld	l,(ix+0Ah)		;
	ld	h,(ix+0Bh) ; a		;
	add	hl,de			;
	ld	(ix+0Ah),l		;

loc_74A4:			; m14:
	ld	(ix+0Bh),h		; a =

loc_74A7:			; m15:
	ld	a,(iy+0)		;
	inc	iy			;			
	ld	(ix+-1),a		; (c = *(++f))
	or	a			;
	jp	nz,loc_733F		; if(c != 0) goto m1;
	jp	cret

loc_74B6:	; 'd'		; m16:
	ld	(ix+-2),1		; sign = 1;
	jp	loc_73EB		; goto m9;

loc_74BD:			; m17:
	ld	hl,' '			;
	push	hl			;
	call	_pputc			; pputc(' ');
	pop	bc			;

	ld	a,(_left)		;
	ld	e,a			;
	dec	a	;-1		;
	ld	(_left),a		; left--;

loc_74CD:	; 'c'		; m18:
	ld	a,(_left); left	;
	ld	e,a			;
	rla				;
	sbc	a,a			;
	ld	d,a			;
	ld	hl,1			;
	call	wrelop			; if(reg_hl-reg_de)
	jp	m,loc_74BD		; if(  ) goto m17;

	ld	l,(ix+0Ah)		;
	ld	h,(ix+0Bh) ; a		;
	ld	c,(hl)			;
	inc	hl			;
	ld	b,(hl)			;
	inc	hl			;
	ld	(ix+0Ah),l		;
	ld	(ix+0Bh),h		; a =
	push	bc
	jp	loc_734A		; goto m2;

loc_74F1:	; 's'		; m19:
	ld	l,(ix+0Ah)		;
	ld	h,(ix+0Bh)		;
	ld	c,(hl)			;
	inc	hl			;
	ld	b,(hl)			;
	inc	hl			;
	ld	(ix+0Ah),l		;
	ld	(ix+0Bh),h		; a += sizeof(char*)/sizeof *a;

	ld	(ix+-10),c
	ld	(ix+-9),b		; file =
	ld	l,c
	ld	h,b
	push	hl
	call	_strlen			; int strlen(char *s);
	pop	bc
	ld	(ix+-2),l		; sign	=

	ld	b,(ix+-4)		; width
	ld	a,l			;
	call	brelop			;
	jr	nc,loc_7532		; if(reg_a==reg_b) goto m21;

	ld	a,(ix+-2)		;
	ld	(ix+-4),a		; width = sign;
	jr	loc_7532		; goto m21;

loc_7522:			; m20:
	ld	hl,' '
	push	hl
	call	_pputc			; pputc(' ');
	pop	bc

	ld	a,(_left)		;
	ld	e,a			;
	dec	a			;
	ld	(_left),a		; left--;

loc_7532:			; m21:

	ld	a,(_left) ; left
	ld	e,a
	ld	d,0
	ld	l,(ix+-4) ; width
	ld	h,d
	call	wrelop
	jr	c,loc_7522		; if(reg_hl-reg_de) goto m20;
	jr	loc_7559		; goto m23;

loc_7543:			; m22:
	ld	l,(ix+-10)		;
	ld	h,(ix+-9)		;
	inc	hl			;
	ld	(ix+-10),l		;
	ld	(ix+-9),h ; file++;	;
	dec	hl			;
	ld	l,(hl)			;
	ld	h,0			;
	push	hl			;
	call	_pputc			; pputc(*(file++));
	pop	bc

loc_7559:			; m23:
	ld	a,(ix+-4)		;
	dec	(ix+-4)		;
	or	a			;
	jr	nz,loc_7543		; if(width != 0) goto m22;
	jp	loc_74A7		; goto m15;

loc_7565:	; '.'		; m24:
	ld	a,(iy+0)		
	cp	'*'
	jr	nz,loc_7587		; if(*f != '*') goto m25;

	ld	l,(ix+0Ah)
	ld	h,(ix+0Bh)
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	(ix+0Ah),l
	ld	(ix+0Bh),h		; a += sizeof(char*)/sizeof *a;
	push	bc
	call	_atoi
	pop	bc
	ld	(ix+-4),l		; width = atoi(*a);
	jp	loc_7367		; goto m4;

loc_7587:			; m25:
	ld	a,(iy+0)
	ld	l,a
	rla
	sbc	a,a
	ld	h,a
	ld	a,l
	add	a,0D0h
	ld	(ix+-4),a		; width = *f+0xD0;
	inc	iy			; f++;
	jp	loc_7367		; goto m4;

loc_7599:			; m26:

	ld	l,(ix+-1) ; c
	ld	h,0
	push	hl
	jp	loc_734A		; goto m2;

;=========================================================
; 7B4B digit:	
; 7B52 _atoi:	atoi.asm
;=========================================================		
	psect	text

digit:	sub	'0'
	ret	c
	cp	10
	ccf
	ret

	global	_atoi
_atoi:	pop	bc	; return address
	pop	de
	push	de
	push	bc
	ld	hl,0
1:
	ld	a,(de)
	inc	de
	cp	' '
	jr	z,1b
	cp	9	; tab
	jr	z,1b
	dec	de	; point to 1st non blank char
	cp	'-'
	jr	z,3f
	cp	'+'
	jr	nz,2f
	or	a	; reset zero flag
3:
	inc	de
2:	ex	af,af'
1:
	ld	a,(de)
	inc	de
	call	digit
	jr	c,3f
	add	hl,hl
	ld	c,l
	ld	b,h
	add	hl,hl
	add	hl,hl
	add	hl,bc
	ld	c,a
	ld	b,0
	add	hl,bc
	jr	1b
3:
	ex	af,af'
	ret	nz
	ex	de,hl
	ld	hl,0
	sbc	hl,de
	ret

;=========================================================
; sub_7288 pputc
;=========================================================
;    FILE * word_B02F;
;
;    int sub_7288(int c) fputc(c, word_B02F);
;
	global	_fputc
	global	_ffile

	psect	text

_pputc:
	call 	csv
	ld	hl,(_ffile)
	push	hl
	ld	l,(ix+6)
	ld	h,(ix+7)
	push	hl
	call	_fputc
	jp	cret

	psect	bss

_left:		defs	1	; byte_B02E
_ffile:		defs	2	; word_B02F
_base:		defs	1	; byte_B031

;=========================================================
; * sub_729C pnum
;=========================================================
;    char  left;
;    uchar base;
;
;    int pputc(int);
;
;    pnum(long i) {
;	long j;
;	
;	left--;
;	if((j = i/base) != 0) pnum(j);
;	while ((int)left > 0) {
;	    left--;
;	    pputc(' ');
;	}
;	pputc((uchar)"0123456789ABCDEF"[i%base]);
;	return;
;    }
;
	global	_pnum
	global	_left
	global	_base
	global	_ffile
	global	lldiv
	global	_pputc
	global	wrelop
	global	llmod

	psect	text

_pnum:
	call 	csv
	push 	hl
	push 	hl
	ld	a,(_left)
	ld	e, a
	dec	a
	ld	(_left),a
	ld	a,(_base)
	ld	hl,0
	ld	d,l
	ld	e,a
	push	hl
	push	de
	ld	e,(ix+6)
	ld	d,(ix+7)
	ld	l,(ix+8)
	ld	h,(ix+9)
	call	lldiv
	ld	(ix+-4),e
	ld	(ix+-3),d
	ld	(ix+-2),l
	ld	(ix+-1),h
	ld	a,e
	or	d
	or	l
	or	h
	jr	z,loc_72ED
	push	hl
	push	de
	call	_pnum
	pop	bc
	pop	bc
	jr	loc_72ED
loc_72DD:
	ld	a,(_left)
;	sub	1
	ld	e,a
	dec	a
	ld	(_left),a
	ld	hl,32
	push	hl
	call	_pputc
	pop	bc
loc_72ED:
	ld	a,(_left)
	ld	e, a
	rla
	sbc	a, a
	ld	d, a
	ld	hl,0
	call	wrelop
	jp	m,loc_72DD
	ld	a,(_base)
	ld	hl,0
	ld	d,l
	ld	e,a
	push	hl
	push	de
	ld	e,(ix+6)
	ld	d,(ix+7)
	ld	l,(ix+8)
	ld	h,(ix+9)
	call	llmod
	ex	de,hl
	ld	de,19f
	add	hl,de
	ld	l,(hl)
	ld	h,0
	push	hl
	call	_pputc
;	pop	bc
	jp	cret

	psect	data

19:	defb	48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70
	defb	0


;=========================================================
; 7FC8 _strcmp:	strcmp.asm
;=========================================================
	psect	text

	global	_strcmp

_strcmp:
	pop	bc
	pop	de
	pop	hl
	push	hl
	push	de
	push	bc

1:	ld	a,(de)
	cp	(hl)
	jr	nz,2f
	inc	de
	inc	hl
	or	a
	jr	nz,1b
	ld	hl,0
	ret

2:	ld	hl,1
	ret	nc
	dec	hl
	dec	hl
	ret

	psect	text
	global	_strncmp, rcsv, cret

_strncmp:
	call	rcsv

1:	ld	a,c
	or	b
	jp	z,3f
	dec	bc
	ld	a,(de)
	cp	(hl)
	jr	nz,2f
	inc	de
	inc	hl
	or	a
	jr	nz,1b
3:
	ld	hl,0
	jp	cret

2:	ld	hl,1
	jp	c,cret
	dec	hl
	dec	hl
	jp	cret

	psect	text
	global	_strcat

_strcat:
	pop	bc
	pop	de
	pop	hl
	push	hl
	push	de
	push	bc
	ld	c,e		;save destination pointer
	ld	b,d

1:	ld	a,(de)
	or	a
	jr	z,2f
	inc	de
	jr	1b

2:	ld	a,(hl)
	ld	(de),a
	or	a
	jr	z,3f
	inc	de
	inc	hl
	jr	2b

3:
	ld	l,c	;restore destination
	ld	h,b
	ret

;=========================================================
; 7FE2 _strcpy:	strcpy.asm
;=========================================================
	psect	text

	global	_strcpy

_strcpy:
	pop	bc
	pop	de
	pop	hl
	push	hl
	push	de
	push	bc
	ld	c,e
	ld	b,d		;save destination pointer

1:	ld	a,(hl)
	ld	(de),a
	inc	de
	inc	hl
	or	a
	jr	nz,1b
	ld	l,c
	ld	h,b
	ret

;=========================================================
; 7FF4 _strlen:	strlen.asm
;=========================================================
	psect	text

	global	_strlen
_strlen:	pop	hl
	pop	de
	push	de
	push	hl
	ld	hl,0

1:	ld	a,(de)
	or	a
	ret	z
	inc	hl
	inc	de
	jr	1b

	psect	text
	global	_strncpy, rcsv, cret

_strncpy:
	call	rcsv
	push	hl

1:
	ld	a,c
	or	b
	jr	z,2f
	dec	bc
	ld	a,(de)
	ld	(hl),a
	inc	hl
	or	a
	jr	z,1b
	inc	de
	jr	1b

2:	pop	hl
	jp	cret

;=========================================================
; sub_6BD3 fprintf
;=========================================================
;    #include <stdio.h>
;
;    fprintf(file, f, a) FILE * file; char * f; int a; {
;	return(_doprnt(file, f, &a));
;    }
;
	psect	text

	global	_fprintf
	global	_doprnt

_fprintf:
	call	csv
	push	ix
	pop	de
	ld	hl,10
	add	hl,de
	push	hl
	ld	l,(ix+8)
	ld	h,(ix+9)
	push	hl
	ld	l,(ix+6)
	ld	h,(ix+7)
	push	hl
	call	_doprnt
	pop	bc
	pop	bc
	pop	bc
	jp	cret

;=========================================================
; sub_6BF5 freopen OK(no code matching)   Used in: main
;=========================================================
;	FILE * freopen(char * name, char * mode, register FILE * f) {
;	    unsigned char c1, func;
;	    struct fcb * p;
;
;	    fclose(f);
;	    p = _fcb + (f->_file = (f - stdin));
;	    c1 = 0;
;	    func = CPMOPN;
;	    f->_flag &= 0x4F;
;	    switch(*mode) {
;	      case 'w':
;		c1++;
;	        func = CPMMAKE;
;	      case 'r':
;		if(*(mode+1) == 'b') f->_flag = _IOBINARY;
;		break;
;	    }
;	    def_fcb(p, name);
;	    if(func == CPMMAKE) bdos(CPMDEL, p);
;	    if(bdos(func, p) == -1) return 0;
;	    LO_CHAR(p->rwp) = c1 ? 2 : 1;
;	    if(((LO_CHAR(f->_flag) |= (c1+1))&0xC) == 0) {
;		if(f->_base == 0) f->_base = sbrk(512);
;	    }
;	    f->_ptr = f->_base;
;	    if(f->_base == 0) goto m8;
;	    LO_CHAR(f->_cnt) = 0;
;	    if(c1 == 0) goto m9;
;	    HI_CHAR(f->_cnt) = 2;
;	    goto m10;
;	m8: LO_CHAR(f->_cnt) = 0;
;	m9: HI_CHAR(f->_cnt) = 0;
;	m10:
;	    return f;
;	}
;
	global	_freopen
	global	_fclose
	global	__fcb
	global	__iob
	global	adiv
	global	amul
	global	_def_fcb
	global	_bdos
	global	_sbrk

	psect	text

_freopen:
	call	csv
	push	hl
	push	hl
	ld	l,(ix+10)
	ld	h,(ix+11)
	push	hl
	pop	iy
	push	hl
	call	_fclose
	pop	bc
	ld	de,__iob
	push	iy
	pop	hl
	or	a
	sbc	hl,de
	ld	de,8
	call	adiv
	ld	a,l
	ld	(iy+7),a
	ld	l,a
	rla
	sbc	a,a
	ld	h,a
	ld	de,41
	call	amul
	ld	de,__fcb
	add	hl,de
	ld	(ix+-4),l
	ld	(ix+-3),h
	ld	(ix+-1),0
	ld	(ix+-2),15
	ld	a,(iy+6)
	and	79
	ld	(iy+6),a
	ld	l,(ix+8)
	ld	h,(ix+9)
	ld	a,(hl)
	cp	'r'
	jr	z,loc_6C55
	cp	'w'
	jr	nz,loc_6C65
	inc	(ix+-1)
	ld	(ix+-2),22
loc_6C55:
	ld	l,(ix+8)
	ld	h,(ix+9)
	inc	hl
	ld	a,(hl)
	cp	'b'
	jr	nz,loc_6C65
	ld	(iy+6),-128
loc_6C65:
	ld	l,(ix+6)
	ld	h,(ix+7)
	push	hl
	ld	l,(ix+-4)
	ld	h,(ix+-3)
	push	hl
	call	_def_fcb
	pop	bc
	pop	bc
	ld	a,(ix+-2)
	cp	22
	ld	l,(ix+-4)
	ld	h,(ix+-3)
	push	hl
	jr	nz,loc_6C95
	ld	hl,19
	push	hl
	call	_bdos
	pop	bc
	ld	l,(ix+-4)
	ld	h,(ix+-3)
	ex	(sp),hl
loc_6C95:
	ld	l,(ix+-2)
	ld	h,0
	push	hl
	call	_bdos
	pop	bc
	pop	bc
	ld	a,l
	cp	255
	jr	nz,loc_6CAB
	ld	hl,0
	jp	cret
loc_6CAB:
	ld	a,(ix+-1)
	or	a
	jr	nz,loc_6CB6
	ld	hl,1
	jr	loc_6CB9
loc_6CB6:
	ld	hl,2
loc_6CB9:
	ld	a,l
	ld	e,(ix+-4)
	ld	d,(ix+-3)
	ld	hl,36
	add	hl,de
	ld	(hl),a
	ld	a,(ix+-1)
	inc	a
	ld	e,a
	ld	a,(iy+6)
	or	e
	ld	(iy+6),a
	and	12
	or	a
	jr	nz,loc_6CEC
	ld	a,(iy+4)
	or	(iy+5)
	jr	nz,loc_6CEC
	ld	hl,512
	push	hl
	call	_sbrk
	pop	bc
	ld	(iy+4),l
	ld	(iy+5),h
loc_6CEC:
	ld	l,(iy+4)
	ld	h,(iy+5)
	ld	(iy+0),l
	ld	(iy+1),h
	ld	a,l
	or	(iy+5)
	jr	z,loc_6D0E
	ld	a,(ix+-1)
	or	a
	ld	(iy+2),0
	jr	z,loc_6D12
	ld	(iy+3),2
	jr	loc_6D16
loc_6D0E:
	ld	(iy+2),0
loc_6D12:
	ld	(iy+3),0
loc_6D16:
	push	iy
	pop	hl
	jp	cret

;=========================================================
; 75FA flsbuf 		Push next buffer to file
;=========================================================
;	int _flsbuf(int c, register FILE * f) {
;	    struct fcb * p;
;
;	    sub_77F5();	/* Test ^c */
;	    p = _fcb + (f - stdin);
;
;	    if(f->_flag & _IOWRT) {
;		if(f->_base == (char *)NULL) {
;	    	    f->_cnt = 0;
;	    	    if((char)(p->rwp) == 4) {
;			bdos(CPMWCON, c);
;			return c;
;		    }
;	    	    return -1;
;		} else {
;		    fflush(f);
;	    	    *(f->_ptr++) = c;
;	    	    f->_cnt--;
;		}
;		return c;
;	    }
;	    return -1;
;	}
;
	global	_flsbuf
	global	_test_brk
	global	__fcb
	global	__iob
	global	adiv
	global	amul
	global	_bdos
	global	_fflush

	psect	text

_flsbuf:
	call	csv
	push	hl
	ld	l,(ix+8)
	ld	h,(ix+9)
	push	hl
	pop	iy
	call	_test_brk
	ld	de,__iob
	push	iy
	pop	hl
	or	a
	sbc	hl,de
	ld	de,8
	call	adiv
	ld	de,41
	call	amul
	ld	de,__fcb
	add	hl,de
	ld	(ix+-2),l
	ld	(ix+-1),h
	bit	1,(iy+6)
	jr	z,loc_7689
	ld	a,(iy+4)
	or	(iy+5)
	jr	nz,loc_7662
	ld	(iy+2),0
	ld	(iy+3),0
	ex	de,hl
	ld	hl,36
	add	hl,de
	ld	a,(hl)
	cp	4
	jr	nz,loc_7689
	ld	l,(ix+6)
	ld	h,(ix+7)
	push	hl
	ld	hl,2
	push	hl
	call	_bdos
	pop	bc
	pop	bc
loc_7659:
	ld	l,(ix+6)
	ld	h,(ix+7)
	jp	cret
loc_7662:
	push	iy
	call	_fflush
	pop	bc
	ld	a,(ix+6)
	ld	l,(iy+0)
	ld	h,(iy+1)
	inc	hl
	ld	(iy+0),l
	ld	(iy+1),h
	dec	hl
	ld	(hl),a
	ld	l,(iy+2)
	ld	h,(iy+3)
	dec	hl
	ld	(iy+2),l
	ld	(iy+3),h
	jr	loc_7659
loc_7689:
	ld	hl,-1
	jp	cret

;=========================================================
; sub_77F5	Test ^c
;=========================================================
;    #define CPMRCON	1	/* read console */
;    #define	CPMICON	11	/* interrogate console ready */
;
;    char sub_77F5() {	/* Test ^c */
;	char * st;
;	
;	if(bdos(CPMICON) == 0) return;	   /* status console */
;	if(( *st=bdos(CPMRCON)) != 3) return;  /* read console   */
;	exit();
;    }
;
	global	_test_brk
	global	_exit

	psect	text

_test_brk:
	call	csv
	push 	hl
	ld	hl,11	; CPMICON - interrogate console ready
	push	hl
	call	_bdos
	pop	bc
	ld	a,l
	or	a
	jp	z,cret
	ld	hl,1	; CPMRCON - read console
	push	hl
	call	_bdos
	pop	bc
	ld	e,l
	ld	(ix+-1),e
	ld	a,e
	cp	3
	jp	nz,cret
	call	_exit
	jp	cret

;=========================================================
; sub_70F8 def_fcb Define FCB fields
;=========================================================
;    char def_fcb(register struct fcb * fc, char * fname) {
;	char * cp;
;
;	fc->dr = 0;
;	if(*(fname+1) == ':') {		
;	    fc->dr = *(fname) & 0xf;	
;	    fname += 2;			
;	}					
;	cp = &fc->name;
;	while((*fname != '.')
;	&& ('\t' < *fname)		
;	&& (cp < &fc->ft))  *(cp++) = toupper(*(fname++));
;	while(cp < &fc->ft) *(cp++) = ' ';
;	do {if(*fname == 0) break;} while(*(fname++) != '.');	
;	while(('\t' < *(fname))
;	&& (cp < (char*)&fc->ex)) *(cp++) = toupper(*(++fname));
;	while(cp <= (char*)&fc->ex) *(cp++) = ' ';
;	fc->ex = fc->nr =  0;		
;	return 0;
;    }
;
	global	_def_fcb
	global	_toupper
	global	wrelop

	psect	text

_def_fcb:
	call 	csv
	push 	hl
	ld	l,(ix+6)
	ld	h,(ix+7)
	push	hl
	pop	iy
	ld	(iy+0),0
	ld	l,(ix+8)
	ld	h,(ix+9)
	inc	hl
	ld	a,(hl)
	cp	58
	jr	nz,loc_712D
	dec	hl
	ld	a,(hl)
	ld	l,a
	rla
	ld	a,l
	and	15
	ld	(iy+0),a
	ld	l,(ix+8)
	ld	h,(ix+9)
	inc	hl
	inc	hl
	ld	(ix+8),l
	ld	(ix+9),h
loc_712D:
	push	iy
	pop	hl
	inc	hl
	ld	(ix+-2),l
	ld	(ix+-1),h
	jr	loc_7160
loc_7139:
	ld	l,(ix+8)
	ld	h,(ix+9)
	ld	a,(hl)
	inc	hl
	ld	(ix+8),l
	ld	(ix+9),h
	ld	l,a
	rla
	sbc	a,a
	ld	h,a
	push	hl
	call	_toupper
	pop	bc
	ld	e,l
	ld	l,(ix+-2)
	ld	h,(ix+-1)
	inc	hl
	ld	(ix+-2),l
	ld	(ix+-1),h
	dec	hl
	ld	(hl),e
loc_7160:
	ld	l,(ix+8)
	ld	h,(ix+9)
	ld	a,(hl)
	cp	46
	jr	z,loc_719E
	ld	a,(hl)
	ld	e,a
	rla
	sbc	a,a
	ld	d,a
	ld	hl,9
	call	wrelop
	push	iy
	pop	de
	ld	hl,9
	jp	p,loc_71A4
	add	hl,de
	ex	de,hl
	ld	l,(ix+-2)
	ld	h,(ix+-1)
	call	wrelop
	jr	c,loc_7139
	jr	loc_719E
loc_718E:
	ld	l,(ix+-2)
	ld	h,(ix+-1)
	inc	hl
	ld	(ix+-2),l
	ld	(ix+-1),h
	dec	hl
	ld	(hl),32
loc_719E:
	push	iy
	pop	de
	ld	hl,9
loc_71A4:
	add	hl,de
	ex	de,hl
	ld	l,(ix+-2)
	ld	h,(ix+-1)
	call	wrelop
	jr	c,loc_718E
loc_71B1:
	ld	l,(ix+8)
	ld	h,(ix+9)
	ld	a,(hl)
	or	a
	ld	a,(hl)
	jr	z,loc_71F7
	inc	hl
	ld	(ix+8),l
	ld	(ix+9),h
	cp	46
	jr	nz,loc_71B1
	jr	loc_71F0
loc_71C9:
	ld	l,(ix+8)
	ld	h,(ix+9)
	ld	a,(hl)
	inc	hl
	ld	(ix+8),l
	ld	(ix+9),h
	ld	l,a
	rla
	sbc	a,a
	ld	h,a
	push	hl
	call	_toupper
	pop	bc
	ld	e,l
	ld	l,(ix+-2)
	ld	h,(ix+-1)
	inc	hl
	ld	(ix+-2),l
	ld	(ix+-1),h
	dec	hl
	ld	(hl),e
loc_71F0:
	ld	l,(ix+8)
	ld	h,(ix+9)
	ld	a,(hl)
loc_71F7:
	ld	e,a
	rla
	sbc	a,a
	ld	d,a
	ld	hl,9
	call	wrelop
	push	iy
	pop	de
	ld	hl,12
	jp	p,loc_722F
	add	hl,de
	ex	de,hl
	ld	l,(ix+-2)
	ld	h,(ix+-1)
	call	wrelop
	jr	c,loc_71C9
	jr	loc_7229
loc_7219:
	ld	l,(ix+-2)
	ld	h,(ix+-1)
	inc	hl
	ld	(ix+-2),l
	ld	(ix+-1),h
	dec	hl
	ld	(hl),32
loc_7229:
	push	iy
	pop	de
	ld	hl,12
loc_722F:
	add	hl,de
	ex	de,hl
	ld	l,(ix+-2)
	ld	h,(ix+-1)
	call	wrelop
	jr	c,loc_7219
	xor	a
	ld	(iy+32),a
	ld	(iy+12),a
	ld	l,a
	jp	cret

	global _unlink
_unlink:
	call ncsv
	defw 0ffd3h
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	push ix
	pop de	
	ld hl,0ffd6h
	add hl,de	
	push hl	
	call _def_fcb
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr z,l8661h
	ld hl,00000h
	jp cret
l8661h:
	call _getuc
	ld (ix-02bh),l
	ld l,(ix-001h)
	ld h,000h
	push hl	
	call _setuc
	push ix
	pop de	
	ld hl,0ffd6h
	add hl,de	
	ex (sp),hl	
	ld hl,00013h
	push hl	
	call _bdos
	pop bc	
	ld a,l	
	rla	
	sbc a,a	
	ld h,a	
	ld (ix-02dh),l
	ld (ix-02ch),h
	ld l,(ix-02bh)
	ld h,000h
	ex (sp),hl	
	call _setuc
	pop bc	
	ld l,(ix-02dh)
	ld h,(ix-02ch)
	jp cret

_getuc:
	call csv
	ld c,020h
	ld e,0ffh
	push ix
	call 00005h
	pop ix
	ld l,a	
	ld h,000h
	jp cret

_setuc:
	call csv
	ld e,(ix+006h)
	ld c,020h
	push ix
	call 00005h
	pop ix
	jp cret

;*********************************************************

	psect	data

_allocp:defb	0,0
_allocs:defb	0,0,0,0,0,0
_alloct:defb	0,0
_allocx:defb	0,0,0
F170:	defb	0,0

	psect	text

	global	_malloc

_malloc:

call csv
push hl
ld	hl,(_allocs)
ld	a,l
or	h
jp	nz,l3
ld	hl,_allocs+3
ld	(_allocs),hl
ld	(_alloct),hl
ld	hl,_allocs
ld	(_allocs+3),hl
ld	(_allocp),hl
ld	hl,_allocs+2
set	0,(hl)
ld	hl,_allocs+5
set	0,(hl)
l3:
ld	e,(ix+6)
ld	d,(ix+7)
ld	hl,5
add	hl,de
ld	de,3
call	ldiv
ld	de,3
call	lmul
ld	(ix+6),l
ld	(ix+7),h
ld	iy,(_allocp)
l4:
ld	hl,0
ld	(F170),hl
l7:
bit	0,(iy+2)
jp	nz,l10
jp	l11
l12:
ld	l,(ix+-2)
ld	h,(ix+-1)
ld	c,(hl)
inc	hl
ld	b,(hl)
ld	(iy+0),c
ld	(iy+1),b
l11:
ld	l,(iy+0)
ld	h,(iy+1)
ld	(ix+-2),l
ld	(ix+-1),h
inc	hl
inc	hl
bit	0,(hl)
jp	z,l12
push	iy
pop	de
ld	l,(ix+6)
ld	h,(ix+7)
add	hl,de
ex	de,hl
ld	l,(ix+-2)
ld	h,(ix+-1)
call	wrelop
push	iy
jp	c,L5
pop	de
ld	l,(ix+6)
ld	h,(ix+7)
add	hl,de
push	iy
pop	de
call	wrelop
push	iy
jp	c,L5
pop	de
ld	l,(ix+6)
ld	h,(ix+7)
add	hl,de
ld	(_allocp),hl
ld	e,(ix+-2)
ld	d,(ix+-1)
call	wrelop
ld	hl,(_allocp)
jp	nc,L3
ld	de,_allocx
push	hl
ld	bc,3
ldir
pop	hl
ld	e,(iy+0)
ld	d,(iy+1)
ld	hl,(_allocp)
ld	(hl),e
inc	hl
ld	(hl),d
inc	hl
ld	(hl),0
dec	hl
dec	hl
L3:
ld	(iy+0),l
ld	(iy+1),h
set	0,(iy+2)
push	iy
pop	hl
inc	hl
inc	hl
inc	hl
jp	cret
l10:
push	iy
L5:
pop	hl
ld	(ix+-2),l
ld	(ix+-1),h
ld	l,(iy+0)
ld	h,(iy+1)
push	hl
pop	iy
push	hl
pop	de
ld	l,(ix+-2)
ld	h,(ix+-1)
call	wrelop
jp	c,l7
ld	de,(_alloct)
ld	l,(ix+-2)
ld	h,(ix+-1)
or	a
sbc	hl,de
jp	nz,L6
ld	de,_allocs
push	iy
pop	hl
or	a
sbc	hl,de
jp	z,l18
L6:
ld	hl,0
jp	cret
l18:
ld	hl,(F170)
inc	hl
ld	(F170),hl
ex	de,hl
ld	hl,1
call	wrelop
jp	nc,l7
ld	de,-259
ld	l,(ix+6)
ld	h,(ix+7)
call	wrelop
jp	c,l21
ld	l,(ix+6)
ld	h,(ix+7)
ld	(F170),hl
jp	l22
l21:
ld	e,(ix+6)
ld	d,(ix+7)
ld	hl,257
add	hl,de
ld	de,255
call	ldiv
ld	de,255
call	lmul
ld	(F170),hl
l22:
ld	hl,(F170)
push	hl
call	_sbrk
pop	bc
ld	(ix+-2),l
ld	(ix+-1),h
ld	de,-1
or	a
sbc	hl,de
jp	z,L6
ld	e,(ix+-2)
ld	d,(ix+-1)
ld	hl,(_alloct)
ld	(hl),e
inc	hl
ld	(hl),d
inc	hl
inc	hl
or	a
sbc	hl,de
ld	hl,(_alloct)
inc	hl
inc	hl
jp	z,L4
set	0,(hl)
jp	l25
L4:
res	0,(hl)
l25:
ld	de,(F170)
ld	l,(ix+-2)
ld	h,(ix+-1)
add	hl,de
dec	hl
dec	hl
dec	hl
ex	de,hl
ld	l,(ix+-2)
ld	h,(ix+-1)
ld	(hl),e
inc	hl
ld	(hl),d
ld	(_alloct),de
ld	de,_allocs
ld	hl,(_alloct)
ld	(hl),e
inc	hl
ld	(hl),d
inc	hl
set	0,(hl)
ld	l,(ix+-2)
ld	h,(ix+-1)
inc	hl
inc	hl
res	0,(hl)
jp	l4

	global	_free
_free:

call csv
ld	l,(ix+6)
ld	h,(ix+7)
dec	hl
dec	hl
dec	hl
push	hl
pop	iy
ld	(_allocp),iy
res	0,(iy+2)
jp	cret

;	blkclr(ptr, size)
;	char *	ptr; unsigned short size;

;	Fills memory with size null bytes

	psect	text
	global	_blkclr

_blkclr:
	pop	de	;return address
	pop	hl	;pointer
	pop	bc	;count
	push	bc	;adjust stack
	push	hl
	push	de
	ld	e,0

1:
	ld	a,c	;check for finished
	or	b
	ret	z
	ld	(hl),e
	inc	hl
	dec	bc
	jr	1b

;*************************************************************

	psect	data

;	extern  unsigned char   _ctype_[];      /* in libc.lib */

__ctype_:	;81h
 defb 00H,20H,20H,20H,20H,20H,20H
 defb 20H,20H,20H,08H,08H,08H,08H,08H,20H,20H,20H,20H,20H,20H,20H,20H
 defb 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,08H,10H,10H,10H,10H,10H
 defb 10H,10H,10H,10H,10H,10H,10H,10H,10H,10H,04H,04H,04H,04H,04H,04H
 defb 04H,04H,04H,04H,10H,10H,10H,10H,10H,10H,10H,41H,41H,41H,41H,41H
 defb 41H,01H,01H,01H,01H,01H,01H,01H,01H,01H,01H,01H,01H,01H,01H,01H
 defb 01H,01H,01H,01H,01H,10H,10H,10H,10H,10H,10H,42H,42H,42H,42H,42H
 defb 42H,02H,02H,02H,02H,02H,02H,02H,02H,02H,02H,02H,02H,02H,02H,02H
 defb 02H,02H,02H,02H,02H,10H,10H,10H,10H,20H

;	#if     z80
;	#define MAXFILE         8       /* max number of files open */
;	#else   z80
;	#define MAXFILE         15      /* max number of files open */
;	#endif  z80
;	#define SECSIZE         128     /* no. of bytes per sector */
;
;	extern struct   fcb {
;        uchar   dr;             /* drive code */
;        char    name[8];        /* file name */
;        char    ft[3];          /* file type */
;        uchar   ex;             /* file extent */
;        char    fil[2];         /* not used */
;        char    rc;             /* number of records in present extent */
;        char    dm[16];         /* CP/M disk map */
;        char    nr;             /* next record to read or write */
;        uchar   ranrec[3];      /* random record number (24 bit no. ) */
;        long    rwp;            /* + 34 read/write pointer in bytes */
;        uchar   use;            /* use flag */
;        uchar   uid;            /* user id belonging to this file */
;	}       _fcb[MAXFILE];
;
;
__fcb:		;150h
;stdin
 defb 00H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,00H,00H,00H,00H ;16
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H ;32
 defb 00H,00H
 defb 00H,00H ;34
 defb 00H,00H,00H,00H,04H,00H
;stdout
 defb 00H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,00H,00H,00H,00H ;16
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H ;32
 defb 00H,00H
 defb 00H,00H ;34
 defb 00H,00H,00H,00H,04H,00H
;stderr
 defb 00H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,00H,00H,00H,00H ;16
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H ;32
 defb 00H,00H
 defb 04H,00H ;34
 defb 00H,00H,00H,00H,04H,00H

 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H 
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H 
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H 
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H 
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H 
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H 
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H 
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H 
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H 
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H 
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H 
 defb 00H,00H,00H,00H,00H,00H,00H,00H,00H



