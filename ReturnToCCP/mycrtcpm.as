	psect	text

CCP_SIZE	equ	0806H

	global	start, _main, _exit, __argc_, startup, nularg
	global __Hbss, __Lbss
        global  __iob
        global  _fclose

start:	ld	(ccp_sp),sp	; save SP
	ld	hl,(6)		; base address of fdos
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
	call	_main		; HL = value returned by main
        ld      (80H),hl	; store-it at 80H, as required by the exit(int) definition
 
; close all 8 CP/M files, just in case the program forgot-it 

	ld	b,8		; CP/M files counter
	ld	hl,__iob	; FILE*	
cloop:
	push	bc		; counter on stack
	push	hl		; FILE* on stack
        call    _fclose		; fclose(FILE*)
	pop	hl		
	ld	bc,8		; sizeof(FILE)
	add	hl,bc		; go to next FILE
	pop	bc		; get counter
	djnz	cloop		; loop until counter=0

	ld	sp,(ccp_sp)	; restore CCP's SP
	ret			; return to CCP

	psect	data

nularg:	defb	0
ccp_sp:	defs	2

	end start

