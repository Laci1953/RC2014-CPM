; Xorshift is a class of pseudorandom number generators discovered
; by George Marsaglia and detailed in his 2003 paper, Xorshift RNGs.
;
; 16-bit xorshift pseudorandom number generator by John Metcalf
; returns   hl = pseudorandom number
; corrupts   a

; generates 16-bit pseudorandom numbers with a period of 65535
; using the xorshift method:

; hl ^= hl << 7
; hl ^= hl >> 9
; hl ^= hl << 8

; some alternative shift triplets which also perform well are:
; 6, 7, 13; 7, 9, 13; 9, 7, 13.

	psect	text

	GLOBAL	_xrnd, _xrndseed

Z80ALL	equ	1

_xrnd:
	ld	hl,1		; seed must not be 0
	ld	a,h
	rra
	ld	a,l
	rra
	xor	h
	ld	h,a
	ld	a,l
	rra
	ld	a,h
	rra
	xor	l
	ld	l,a
	xor	h
	ld	h,a
	ld	(_xrnd+1),hl
	res	7,h		;make-it positive...
	ret

;	works only on Z80ALL
;	seed = sum (all screen chars)
;
_xrndseed:

IF	Z80ALL

	xor	a		;make the sum of all chars from screen
	ld	h,a
	ld	l,a
	ld	b,a
	ld	d,a
	ld	c,0BH
loopb:
	in	e,(c)
	add	hl,de
	djnz	loopb
	dec	c
	jp	p,loopb

ELSE

	ld	hl,(6)		;make the sum of last 100 bytes from the stack
	xor	a
	ld	de,100h
	sbc	hl,de
	ld	de,0
	ld	b,0
1:
	ld	a,(hl)
	inc	hl
	add	a,e
	ld	e,a
	jr	nc,2f
	inc	d
2:
	djnz	1b
 	ex	de,hl

ENDIF

  	ld	a,l
  	or	h		; HL must be not NULL
  	jr	nz,1f
	inc	hl
1:
	ld	(_xrnd+1),hl
	ret
