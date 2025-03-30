DEBUG	equ	0

	global	start, _main, __Hbss, __Lbss, __argc_, startup, nularg
	global	_quit

ENTRY	equ	5

	psect	text

start:			

IF	DEBUG
	ld	sp,(ENTRY+1)
ELSE
				;to gain some RAM space
				;save HiTech's EXEC to RAM bank 7
	ld	hl,(ENTRY+1)
	ld	(execjpbdos),hl	;save exec jp to bdos
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl		;HL = BDOS addr
	ld	sp,hl		;set SP
	ld	(ENTRY+1),hl	;store real BDOS addr to ENTRY+1
	dec	hl		;HL = pointer to last byte of EXEC
	ld	(lastbyteEXEC),hl
	ld	de,0BFFFH	;DE = pointer to last byte of RAM bank 8000-C000
	ld	bc,400H		;1KB
	ld	a,32+7		;select RAM bank 7
	out	(7AH),a		;to 8000H
	lddr			;save EXEC to RAM bank 7
	ld	a,32+2		;re-select RAM bank 2
	out	(7AH),a		;to 8000H
ENDIF
				;
	ld	de,__Lbss	; Start of BSS segment
	or	a		; clear carry
	ld	hl,__Hbss
	sbc	hl,de		; Size of uninitialized data area
	ld	c,l
	ld	b,h
	dec	bc	
	ld	l,e
	ld	h,d
	inc	de
	ld	(hl),0
	ldir			; Clear memory
	ld	hl,nularg
	push	hl
	ld	hl,80h		; Argument buffer
	ld	c,(hl)
	inc	hl
	ld	b,0
	add	hl,bc
	ld	(hl),0		; zero terminate it
	ld	hl,81h
	push	hl
	call	startup
	pop	bc		; unjunk stack
	pop	bc
	push	hl
	ld	hl,(__argc_)
	push	hl
	call	_main
store:
	ld	(80H),hl	;store value returned by main

IF	DEBUG
	jp	0
ELSE
				;restore EXEC from RAM bank 7
	ld	de,(lastbyteEXEC);DE = pointer to last byte of EXEC
	ld	hl,0BFFFH	;HL = pointer to last byte of RAM bank 8000-C000
	ld	bc,400H		;1KB
	ld	a,32+7		;select RAM bank 32+7
	out	(7AH),a		;to 8000H
	lddr			;restore EXEC from RAM bank 32+7
	ld	a,32+2		;re-select RAM bank 2
	out	(7AH),a		;to 8000H
	ld	hl,(execjpbdos)	;restore EXEC jp to BDOS 
	ld	(ENTRY+1),hl

	jp	0
ENDIF

_quit:	
	pop	hl		;drop ret addr
	pop	hl		;value to return
	jr	store

	psect	data

nularg:		defb	0

IF	1-DEBUG

execjpbdos:	defs	2
lastbyteEXEC:	defs	2

ENDIF

	end start


