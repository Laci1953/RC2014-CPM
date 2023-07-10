;
;	Save & Load games
;
	global	__Lbss, _LoadGame, _SaveGame
;
BDOS	equ	5

	MACRO	EXIT
	jp	0
	ENDM

	psect	text

fcb:				; fcb
	defb	0		; disk+1
fname:	defm	"BACKGMMNSV "   ; file name
	defb	0		; EX=0
	defb	0,0		; S1,S2
	defb	0		; RC=0
	defb	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	; D0,...D15
fcbcr:	defb	0		; CR=0
	defb	0,0,0		; R0,R1,R2

DMA:	defs	2
counter:defs	1

CannotWrite:defb	13,10
	defm	'Could not write!$'
CannotOpen:defb	13,10
	defm	'Could not found saved game!$'
CannotRead:defb	13,10
	defm	'Could not read saved game!$'
SaveMsg:
	defm	'...saving game...$'
GameNrMsg:
	defm	'Game number (0...9):$'
GameSaved:
	defm	'Game saved!$'
WrongNumber:defb	13,10
	defm	'It must be a single decimal digit!$'
;
show_string_de:
        ld 	c,9
        jp 	BDOS
;
_LoadGame:
loadgame:
	ld	de,GameNrMsg	; Ask 'game nr'
	call	show_string_de

        ld	c,1             ; Accept response
	call	BDOS

	cp	'0'
	jr	nc,2f
1:
	ld	de,WrongNumber
	call	show_string_de
	jr	loadgame
2:
	cp	'9'+1
	jr	nc,1b

	ld	(fname+10),a	; Store game # as final ext char

	ld	c,2
	ld	e,0DH
	call	BDOS
	ld	c,2
	ld	e,0AH
	call	BDOS

	ld	de,fcb		; open file
	ld	c,15
	call	BDOS
	inc	a
	jr	nz,1f

	ld	de,CannotOpen	; file not found
	call	show_string_de
	EXIT
1:
	xor	a		; prepare fcb
	ld	(fcbcr),a

	ld	hl,__Lbss
	ld	(DMA),hl
	ld	a,14
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

	ld	de,CannotRead	; read failed
	call	show_string_de
	EXIT
1:
	ld	hl,counter
	dec	(hl)
	jr	nz,loopr

	ld	de,fcb		; close file
	ld	c,16
	call	BDOS

	ret
;
_SaveGame:
savegame:
	ld	de,GameNrMsg	; Ask 'game nr'
	call	show_string_de

        ld	c,1             ; Accept response
	call	BDOS

	cp	'0'
	jr	nc,2f
1:
	ld	de,WrongNumber
	call	show_string_de
	jr	savegame
2:
	cp	'9'+1
	jr	nc,1b

	ld	(fname+10),a	; Store game # as final ext char
				; store game info
	ld	de,SaveMsg
	call	show_string_de

	ld	de,fcb		; delete file (if any...)
	ld	c,19
	call	BDOS

	ld	de,fcb		; make file
	ld	c,22
	call	BDOS

	inc	a
	jr	nz,1f

	ld	de,CannotWrite	; disk directory full
	call	show_string_de
	EXIT			; quit
1:
	xor	a		; prepare fcb
	ld	(fcbcr),a

	ld	hl,__Lbss
	ld	(DMA),hl
	ld	a,14
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

	ld	de,CannotWrite	; write failed
	call	show_string_de
	EXIT			; quit
1:
	ld	hl,counter
	dec	(hl)
	jr	nz,loopw

	ld	de,fcb		; close file
	ld	c,16
	call	BDOS

	ld	de,GameSaved
	call	show_string_de

	EXIT			; quit
;
