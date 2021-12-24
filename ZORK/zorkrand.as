;
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

	psect	common
	GLOBAL	_xrnd, _xrndseed
_xrnd:
  ld hl,(seed)       ; seed must not be 0
  ld a,h
  rra
  ld a,l
  rra
  xor h
  ld h,a
  ld a,l
  rra
  ld a,h
  rra
  xor l
  ld l,a
  xor h
  ld h,a
  ld (seed),hl
  ret

_xrndseed:	
  ld	hl,(85H);80H=counter, 81H = CP/M command line
  ld	a,l
  or	h	; HL must be not NULL
  jr	nz,1f
  inc	hl
1:
  ld	(seed),hl
  ret

	psect 	bss
seed:	defs	2
