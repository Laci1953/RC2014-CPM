;(c) by ladislau szilagyi, 2021
;
;	Usage:
;
;	>MYDW filename.ext<CR>
;	(then paste from download.txt)
;
	psect	text

ACIA	equ	1		;1=ACIA, 0=SIO or KIO
KIO	equ	0		;1=KIO, 0=SIO
M512	equ	1		;1=512KB RAM + 512KB ROM

REBOOT	equ	0H
BDOS	equ	5H
CONIO	equ	6
CONINP	equ	1
CONOUT	equ	2
PSTRING	equ	9
MAKEF	equ	22
CLOSEF	equ	16
WRITES	equ	21
DELF	equ	19
SETDMA	equ	1AH

CR	equ	0DH
LF	equ	0AH

FCB	equ	05CH
BUFF	equ	080H

COND	ACIA
CTRL	equ	80H
DATA	equ	81H
ENDC

COND	1-ACIA
COND	KIO
DATA      EQU 88H
CTRL      EQU 89H
ENDC
COND	1-KIO
DATA      EQU 81H
CTRL      EQU 80H
ENDC
ENDC

start:
	ld	sp,stack

	LD	C,DELF
	LD	DE,FCB
	CALL	BDOS

	LD	C,MAKEF
	LD	DE,FCB
	CALL	BDOS

	xor	a
	LD	(checkSum),A
	LD	(byteCount),A
	LD	HL,bigbuf	;HL=pbuf
	ld	de,0		;DE=length (for M512, length of last 16KB segment)
COND	M512
	ld	a,36
	ld	(bank),a
	out	(79H),a		;set 36 as current bank for 4000H
ENDC

WAITLT:	call	GETCHR
	CP	':'
	JR	NZ,WAITLT

GETHEX:
	CALL 	GETCHR
	CP	'>'
	JR	Z,CLOSE
	LD   	B,A
	PUSH 	BC
	CALL 	GETCHR
	POP 	BC
	LD   	C,A

	CALL 	BCTOA

	LD	B,A
	LD	A,(checkSum)
	ADD	A,B
	LD	(checkSum),A
	LD	A,(byteCount)
	INC	A
	LD	(byteCount),A

	LD	A,B
	LD	(HL),A	;store byte
	INC	HL	;inc pbuf
	inc	de	;inc len
	ld	a,h
COND	M512
	cp	80H	;have we reached 8000H?
	jr	nz,GETHEX
			;yes, so pass to the next 16KB RAM bank
	ld	a,(bank)
	inc	a
	cp	64	;have we reached max bank # ?
	jr	z,TOOBIG
	ld	(bank),a
	out	(79H),a
	ld	de,0
	ld	hl,bigbuf
ENDC
COND	1-M512
	cp	0E2H	;have we reached E200H?
	jr	z,TOOBIG
ENDC	
	JR	GETHEX
;
TOOBIG:
	LD	DE,toobigErrMess
	LD	C,PSTRING
	CALL	BDOS
	JP	FINISH
;
CLOSE:
	push	de	;save len

; Byte count (lower 8 bits)
	CALL 	GETCHR
	LD   	B,A
	PUSH 	BC
	CALL 	GETCHR
	POP 	BC
	LD   	C,A

	CALL 	BCTOA
	LD	B,A
	LD	A,(byteCount)
	SUB	B
	JR	Z,byteCountOK

	LD	A,CR
	CALL	PUTCHR
	LD	A,LF
	CALL	PUTCHR

	LD	DE,countErrMess
	LD	C,PSTRING
	CALL	BDOS

	; Sink remaining 2 bytes
	CALL 	GETCHR
	CALL 	GETCHR

	JR	FINISH

byteCountOK:

; Checksum
	CALL 	GETCHR
	LD   	B,A
	PUSH 	BC
	CALL 	GETCHR
	POP 	BC
	LD   	C,A

	call	BCTOA

	LD	B,A
	LD	A,(checkSum)
	SUB	B
	JR	Z,checksumOK

	LD	A,CR
	CALL	PUTCHR
	LD	A,LF
	CALL	PUTCHR

	LD	DE,chkErrMess
	LD	C,PSTRING
	CALL	BDOS
	JP	FINISH

checksumOK:
	LD	A,CR
	CALL	PUTCHR
	LD	A,LF
	CALL	PUTCHR

	LD	DE,OKMess
	LD	C,PSTRING
	CALL	BDOS

;now write-it to disk (len is on stack!)

	ld	de,bigbuf	;DE=pbuf

COND	M512
	ld	a,36
	ld	(crtbank),a
	out	(79H),a		;set 36 as current bank for 4000H
ENDC
write:
	ld	c,SETDMA

	push	de	;pbuf on stack 

	call	BDOS

	LD	C,WRITES
	LD	DE,FCB
	CALL	BDOS

	pop	de	;DE=pbuf
			;increment pbuf
	ld	hl,80h	;DE=DE+80H
	add	hl,de
	ex	de,hl

COND	M512
	ld	a,(crtbank)
	ld	c,a
	ld	a,(bank)
	cp	c	;have we reached the last bank?
	jr	z,checklen;yes, check remaining length
			;no, continue writing current bank
	ld	a,d
	cp	80H	;have we reached 8000H ?
	jr	nz,write;no,continue to write
			;yes
	ld	a,(crtbank)
	inc	a	;increment current bank
	ld	(crtbank),a
	out	(79H),a	;load current RAM bank to 4000H
	ld	de,4000H
	jr	write
ENDC

checklen:
	xor	a	;CARRY=0
	pop	hl	;HL=len
	ld	bc,80h
	sbc	hl,bc
	push	hl	;len on stack

	jr	c,FINISH

	ld	a,h
	or	l
	jr	z,FINISH

	jr	write

FINISH:
	LD	C,CLOSEF
	LD	DE,FCB
	CALL	BDOS
COND	M512
	ld	a,33	;RAM bank 33 set back to 4000H 
	out	(79H),a
ENDC
	JP	REBOOT
	
; Wait for a char into A (no echo)
GETCHR: 
        IN      A,(CTRL)
        RRA
        JR      nc,GETCHR
        IN      A,(DATA)
	RET

; Write A to output
PUTCHR: LD C,CONOUT
	LD E,A
	CALL BDOS
	RET

BCTOA:	LD   	A,B	; Move the hi order byte to A
	SUB  	30H	; Take it down from Ascii
	CP   	0AH	; Are we in the 0-9 range here?
	JR   	C,BCTOA1; If so, get the next nybble
	SUB  	7	; But if A-F, take it down some more
BCTOA1:	RLCA		; Rotate the nybble from low to high
	RLCA		; One bit at a time
	RLCA		; Until we
	RLCA		; Get there with it
	LD   	B,A	; Save the converted high nybble
	LD   	A,C	; Now get the low order byte
	SUB  	30H	; Convert it down from Ascii
	CP   	0AH	; 0-9 at this point?
	JR   	C,BCTOA2; Good enough then, but
	SUB  	7	; Take off 7 more if it's A-F
BCTOA2:	ADD  	A,B	; Add in the high order nybble
	ret

checkSum: 	defb	0
byteCount:	defb	0
OKMess:		defm	'OK$'
chkErrMess: 	defm	'======Checksum Error======$'
countErrMess: 	defm	'======File Length Error======$'
toobigErrMess:	defm	'======File too big======$'
	defs	64
stack:

COND	M512
bigbuf	equ	4000H	
bank:	defs	1	;36,37...61
crtbank:defs	1
ENDC
COND	1-M512
bigbuf:
ENDC

	end	start
