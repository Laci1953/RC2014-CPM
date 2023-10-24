;
;	Save & Load game
;
	global	__Lbss, _save, _restore

	psect	text

BDOS	equ	5

fcb:				; fcb
	defb	0		; disk+1
fname:	defm	"ALMAZAR SAV"   ; file name
	defb	0		; EX=0
	defb	0,0		; S1,S2
	defb	0		; RC=0
	defb	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	; D0,...D15
fcbcr:	defb	0		; CR=0
	defb	0,0,0		; R0,R1,R2

DMA:	defs	2
counter:defs	1

_restore:
	ld	de,fcb		; open file
	ld	c,15
	call	BDOS
	inc	a
	jr	nz,1f
	ld	hl,0
	ret
1:
	xor	a		; prepare fcb
	ld	(fcbcr),a

	ld	hl,__Lbss
	ld	(DMA),hl
	ld	a,15
	ld	(counter),a
loopr:
	ld	de,(DMA)	; set DMA addr
	ld	c,26
	call	BDOS

	ld	hl,(DMA)	; increment DMA addr
	ld	de,80H
	add	hl,de
	ld	(DMA),hl

	ld	de,fcb		; read record		
	ld	c,20
	call	BDOS
	or	a
	jr	z,1f
	ld	hl,0
	ret
1:
	ld	hl,counter
	dec	(hl)
	jr	nz,loopr

	ld	de,fcb		; close file
	ld	c,16
	call	BDOS

	ld	hl,1
	ret
;
_save:
	ld	de,fcb		; delete file (if any...)
	ld	c,19
	call	BDOS

	ld	de,fcb		; make file
	ld	c,22
	call	BDOS

	inc	a
	jr	nz,1f
	ld	hl,0
	ret
1:
	xor	a		; prepare fcb
	ld	(fcbcr),a

	ld	hl,__Lbss
	ld	(DMA),hl
	ld	a,15
	ld	(counter),a
loopw:
	ld	de,(DMA)	; set DMA addr
	ld	c,26
	call	BDOS

	ld	hl,(DMA)	; increment DMA addr
	ld	de,80H
	add	hl,de
	ld	(DMA),hl

	ld	de,fcb		; write record		
	ld	c,21
	call	BDOS
	or	a
	jr	z,1f
	ld	hl,0
	ret
1:
	ld	hl,counter
	dec	(hl)
	jr	nz,loopw

	ld	de,fcb		; close file
	ld	c,16
	call	BDOS

	ld	hl,1
	ret
