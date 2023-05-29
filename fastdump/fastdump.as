;	Fast DUMP
;
;	this version of DUMP is twice as faster
;	as the original CP/M DUMP
;
;	May 2023
;
;	Ladislau Szilagyi

BDOS	equ     5               ;entry point for the CP/M BDOS.
FCB	equ     5CH             ;default file control block.
FCBR	equ	FCB+32
	
BUFF   	equ     80H            	;i/o buffer and command line storage.

CR	equ	0DH
LF	equ	0AH

	macro	OutA
	call	Bin2Hex		;A=B,C = chars to type
	ld	b,c
	ld	c,a		;high
	call	BiosConout
	ld	c,b		;low
	call	BiosConout
	endm

	psect	text

start:
	ld	sp,(6)		;set SP below BDOS
	ld  	hl,(1)		;fetch BIOS console routines
	inc 	hl
	inc 	hl
	inc 	hl
	ld  	de,BiosConst
	ld  	bc,9
	ldir
	ld	a,(BUFF)	;any parameter?
	or	a
	jp	z,TypeHelp	;no, type help
	xor	a		;setup FCB
	ld	(FCBR),a
	ld	de,FCB
	ld	c,15
	call	BDOS		;open file
	inc	a
	jp	z,TypeNotFound
	ld	hl,0		;HL=offset in file
	ld	de,BUFF		;DE=pointer in rec
loop:	ld	a,e
	cp	BUFF		;if buff empty...
	jr	nz,1f
	push	de		;save pointer
	push	hl		;save offset
	ld	de,FCB		;read next buff
	ld	c,20
	call	BDOS
	pop	hl		;restore offset
	pop	de		;restore pointer
	or	a
	jp	nz,0		;quit if EOF
1:	ld	a,l		;check offset
	and	0FH
	jr	nz,nooff
	ld	c,CR
	call	BiosConout	;CR
	ld	c,LF
	call	BiosConout	;LF		
	call	BiosConst	;check console status
	or	a
	jp	nz,0		;quit if key hit			
	ld	a,h		;print offset in hexa
	OutA
	ld	a,l
	OutA
nooff:	ld	c,' '		;blank
	call	BiosConout
	ld	a,(de)		;A=char
	inc	e		;increment pointer
	jr	nz,2f		;if end-of-record,
	ld	e,80H		;reset pointer
2:	OutA			;print char in hexa
	inc	hl		;increment addr
	jr	loop
;
TypeNotFound:
	ld	hl,FileNotFoundMsg
	jr	type
;
TypeHelp:
	ld	hl,HelpMsg
type:	ld	a,(hl)
	or	a
	jp	z,0
	inc	hl
	ld	c,a		;char to type
	call	BiosConout
	jr	type
;
;	BIOS shortcuts
;
BiosConst:  jp	0
BiosConin:  jp	0
BiosConout: jp	0
;
;------------------------------------------------------------
;	Bin2Hex
;
;	A = byte
;
;	returns BC = hexa representation of A
;		A = high nibble in hexa (ready to be stored/printed)
;------------------------------------------------------------
Bin2Hex:
	ld	c,a		;C = byte
	and	0FH		;A = low nibble
	call	nibble2hex	;B = hexa
	ld	a,c		;A = byte
	ld	c,b		;C = low nibble in hexa
	and	0F0H		;A = (high nibble, 0000)
	rrca
	rrca
	rrca
	rrca			;A = high nibble
				;falls through, will return A = B = high nibble in hexa
;
;	A = bin
;	returns A = B = hexa
;
nibble2hex:			;A = bin
	add     a,090h
        daa
        adc     a,040h
        daa			;A = hexa
	ld	b,a
	ret
;
HelpMsg:
	defm	"Use: dump filename.ext"
	defb	0
FileNotFoundMsg:
	defm	"Could not open file!"
	defb	0

	end	start
