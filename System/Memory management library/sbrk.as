	psect	text

	global	_sbrk, _brk, _checksp, _malloc

_brk:
	pop	hl	;return address
	pop	de	;argument
	ld	(memtop),de	;store it
	push	de		;adjust stack
	jp	(hl)	;return

_sbrk:
	jp	_malloc

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

