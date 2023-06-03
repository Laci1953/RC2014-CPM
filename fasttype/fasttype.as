;	Fast TYPE
;
;	this version of TYPE is twice as faster
;	as the original CP/M TYPE
;
;	June 2023
;
;	Ladislau Szilagyi

BDOS	equ     5               ;entry point for the CP/M BDOS.
FCB	equ     5CH             ;default file control block.
FCBR	equ	FCB+32
	
BUFF   	equ     80H            	;i/o buffer and command line storage.

CR	equ	0DH
LF	equ	0AH
EOF	equ	1AH

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
loop:	call	BiosConst	;quits for any key hit 
	inc	a
	jp	z,0
	ld	de,FCB		;read next buff
	ld	c,20
	call	BDOS
	or	a
	jp	nz,0		;quit if EOF
	ld	hl,BUFF		;type buffer
	ld	b,128
nextchar:
	ld	a,(hl)
	cp	EOF		;quit if EOF
	jp	z,0
	inc	l
	ld	c,a
	call	BiosConout
	djnz	nextchar
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
HelpMsg:
	defm	"Use: ftype filename.ext"
	defb	0
FileNotFoundMsg:
	defm	"Could not open file!"
	defb	0

	end	start
