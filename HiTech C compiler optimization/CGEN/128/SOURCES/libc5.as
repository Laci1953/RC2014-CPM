;
;	Disassembled by Andrey Nikitin & Ladislau Szilagyi
;
*Title Disassembling CGEN.COM Hi-Tech C v3.09
*Heading Checking recreated code with original

; File - libc5.asm Created 09.03.2019 Last Modified 31.01.2021

;
; For easier obtaining an exact copy of the binary executable
; file, all library functions are located in files libc1.asm,
; libc2.asm, libc3.asm, libc4.asm, libc5.asm.
;
;global	__Hbss
;=========================================================
; 7EEA alrsh:	alrsh.asm
;=========================================================		
; arithmetic long right shift
; value in HLDE, count in B

	global	alrsh

	psect	text

alrsh:
	ld	a,b		;check for zero shift
	or	a
	ret	z
	cp	33
	jr	c,1f		;limit shift to 32 bits
	ld	b,32
1:
	sra	h
	rr	l
	rr	d
	rr	e
	djnz	1b
	ret

;=========================================================
; 7EFE almul:	lmul.asm
; 7F26 mult8b:	
;=========================================================		
; Long multiplication for Z80

; Called with 1st arg in HLDE, 2nd arg on stack. Returns with
; result in HLDE, other argument removed from stack

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
	call	mult8b
	ld	a,c
	call	mult8b
	exx
	ld	a,c
	exx
	call	mult8b
	exx
	ld	a,b
	exx
	call	mult8b
	push	hl		;low word
	exx
	pop	de
	ret

mult8b:	ld	b,8
3:
	srl	a
	jr	nc,1f
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

;=========================================================
; 7F3D _brk:	sbrk.asm
; 7F45 _sbrk:	
; 7F71 _checksp:	
;=========================================================		
; NB This brk() does not check that the argument is reasonable.

	psect	text

	global	_sbrk,__Hbss, _brk, _checksp
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

_checksp:
	ld	hl,(memtop)
	ld	bc,128
	add	hl,bc
	sbc	hl,sp
	ld	hl,1		;true if ok
	ret	c		;if carry, sp > memtop+128
	dec	hl		;make into 0
	ret

	psect	bss
memtop:	defs	2

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
; 7F9D shlr:	shlr.asm
;=========================================================
; Shift operations - the count is always in B,
; the quantity to be shifted is in HL, except for the
; assignment type operations, when it is in the memory
; location pointed to by HL

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

;=========================================================
; 7FAD _strcat:	strcat.asm
;=========================================================
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

;=========================================================
; 8002 _strncpy:	strncpy.asm
;=========================================================		
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

;=========================================================
;8040 rcsv:	rcsv.asm
;=========================================================
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

; End file libc5.asm

