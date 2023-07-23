	psect	text

CCP_SIZE	equ	0806H

	global	start, _main, _exit, __argc_, startup, nularg
	global __Hbss, __Lbss

start:	ld	(ccp_sp),sp	; save SP
	ld	hl,(6)		; base address of BDOS
	or	a		; CARRY=0
	ld	bc,CCP_SIZE
	sbc	hl,bc		; go below CCP
	ld	sp,hl		; stack grows downwards
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
;	push	hl
;	call	_exit
	ld	sp,(ccp_sp)	; restore CCP's SP
	ret			; return to CCP

	psect	data

nularg:	defb	0
ccp_sp:	defs	2

	end start

