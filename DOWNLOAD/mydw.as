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
M128	equ	0		;1=SC108 128KB RAM, 0=only 64KB RAM
M512	equ	1		;1=memory module 512KB RAM + 512KB ROM, 0=no 512KB RAM

COND	M128

MEMP_PORT       equ     38H
ROM_OUT_CMD     equ     00000001B
ROM_IN_CMD      equ     00000000B
LOWER_64RAM     equ     00000000B
UPPER_64RAM     equ     10000000B

$StoreToUp	equ	0FFE2H
$LoadFromUp	equ	0FFE8H

MACRO	ROM_IN
	ld	a,LOWER_64RAM .or. ROM_IN_CMD
	out	(MEMP_PORT),a
ENDM

MACRO	ROM_OUT
	ld	a,LOWER_64RAM .or. ROM_OUT_CMD
	out	(MEMP_PORT),a
ENDM

ENDC

TOPUP	equ	0FFH		;FF00=upper storage limit in Upper 64KB RAM

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

start:	DI
	ld	hl,(6)
	ld	sp,hl
	dec	h
	ld	l,0
	ld	(TOPLOW),hl	;set upper storage limit in Lower 64KB RAM 
	ld	hl,endcode
	inc	h
	ld	l,0
	ld	(BIGBUF),hl	;set address of big buffer in Lower 64KB RAM
COND	M128
	xor	a
	ld	(UpRAMUsed),a	;Upper RAM not yet used
				;move StoreToUp,LoadFromUp to 0FFE2H in Lower 64KB RAM
	ld	de,$StoreToUp	;to
	ld	hl,StoreToUp	;from
	ld	bc,12		;count
	ldir
				;move StoreUp to 0FFE2H in Upper 64KB RAM
				;first, move mover to 8000H
	ld	de,8000H	;to
	ld	hl,StoreInUpperRAM	;from
	ld	bc,EndStore - StoreInUpperRAM	;count
	ldir
				;then call mover
	call	8000H
ENDC

COND	M512
	ld	a,36
	ld	(bank),a
	out	(79H),a		;set 36 as current bank for 4000H
ENDC

	LD	C,DELF		;remove file
	LD	DE,FCB
	CALL	BDOS

	LD	C,MAKEF		;make file
	LD	DE,FCB
	CALL	BDOS

	xor	a		;init parameters
	ld	(checkSum),A
	ld	(byteCount),A
COND	1-M512
	ld	hl,(BIGBUF)	;HL=pbuf
ENDC
COND	M512
	ld	hl,4000H
ENDC
	ld	de,0		;DE=len

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

COND	M128
	ld	a,(UpRAMUsed)	;which 64KB RAM are we using?
	or	a
	jr	z,low
				;upper RAM is used
	push	de		;save len
				;prepare StoreUp parameters
	ld	d,UPPER_64RAM .or. ROM_OUT_CMD
	ld	e,LOWER_64RAM .or. ROM_OUT_CMD
	ld	c,MEMP_PORT
	ld	a,b		;A=byte to store
				;HL=addr
	call	$StoreToUp	;call StoreToUp

	pop	de		;restore len

	inc	hl		;inc pbuf
	inc	de		;inc len
	ld	a,h
	cp	TOPUP		;have we reached FF00H?
	jr	z,TOOBIG
	jr	notyet
low:	
	ld	a,b
	ld	(hl),a		;store byte
	inc	hl		;inc pbuf
	inc	de		;inc len
	ld	a,(TOPLOW+1)
	cp	h		;have we reached upper limit?
	jr	nz,notyet
				;yes, switch to Upper RAM
	ld	a,1
	ld	(UpRAMUsed),a	;Upper RAM is used
	ld	hl,0		;pointer back to 0
	ld	de,0		;count back to 0	
notyet:
ENDC

COND	M512
	LD	A,B
	LD	(HL),A	;store byte
	INC	HL	;inc pbuf
	inc	de	;inc len
	ld	a,h
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
	ld	hl,4000H
ENDC

COND	1-M128
	ld	a,b
	ld	(hl),a		;store byte
	inc	hl		;inc pbuf
	inc	de		;inc len
	ld	a,(TOPLOW+1)
	cp	h		;have we reached upper limit?
	jr	z,TOOBIG
ENDC

	jp	GETHEX
;
TOOBIG:
	LD	DE,toobigErrMess
	LD	C,PSTRING
	CALL	BDOS
	JP	FINISH
;
CLOSE:
	ld	(len),de	;save len

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

	JP	FINISH

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
				;now write-it to disk
COND	1-M512
COND	1-M128
	ld	hl,(BIGBUF)
	ld	de,(len)
	call	WriteBuf
	jp	FINISH
ENDC
ENDC
 
COND	M128
	ld	a,(UpRAMUsed)
	or	a
	jr	z,onlylow
				;upper RAM was used, write first from low RAM
	ld	hl,(TOPLOW)	;CARRY=0
	ld	de,(BIGBUF)
	sbc	hl,de
	ex	de,hl		;DE=counter, HL=(BIGBUF)	
	call	WriteBuf
				;then, write from the upper RAM
	ld	hl,0
loopwu:
	ld	de,(len)
	ld	a,d
	or	e
	jp	z,FINISH

	call	Load80		;load 80H bytes from upper RAM to Buf80
				;HL=HL+80H
	push	hl
	ld	hl,Buf80	;write Buf80
	ld	de,80H
	call	WriteBuf
	xor	a
	ld	hl,(len)
	ld	bc,80H
	sbc	hl,bc
	ld	(len),hl
	pop	hl
	jp	c,FINISH
	jr	loopwu
onlylow:
	ld	hl,(BIGBUF)
	ld	de,(len)
	call	WriteBuf
	jp	FINISH
ENDC

COND	M512
	ld	de,4000H
	ld	a,36
	ld	(crtbank),a
	out	(79H),a		;set 36 as current bank for 4000H
write512:
	ld	a,(crtbank)
	ld	c,a
	ld	a,(bank)
	cp	c
	jr	nz,notlast
	ld	hl,(len)	;if last bank, check first remaining len
	ld	a,h
	or	l
	jp	z,FINISH
notlast:
	ld	c,SETDMA
	push	de		;pbuf on stack 
	call	BDOS
	LD	C,WRITES
	LD	DE,FCB
	CALL	BDOS
	pop	de		;DE=pbuf
				;increment pbuf
	ld	hl,80h		;DE=DE+80H
	add	hl,de
	ex	de,hl

	ld	a,(crtbank)
	ld	c,a
	ld	a,(bank)
	cp	c		;have we reached the last bank?
	jr	z,checklen	;yes, check remaining length
				;no, continue writing current bank
	ld	a,d
	cp	80H		;have we reached 8000H ?
	jr	nz,write512	;no,continue to write
				;yes
	ld	a,(crtbank)
	inc	a		;increment current bank
	ld	(crtbank),a
	out	(79H),a		;load current RAM bank to 4000H
	ld	de,4000H
	jr	write512
checklen:
	xor	a		;CARRY=0
	ld	hl,(len)
	ld	bc,80h
	sbc	hl,bc
	ld	(len),hl
	jr	c,FINISH
	ld	a,h
	or	l
	jr	z,FINISH
	jr	write512
ENDC

COND	1-M512
;
;	Write buffer to disk
;
;	HL=buffer start
;	DE=len
;
WriteBuf:
	ld	(count),de
	ex	de,hl		;DE=buffer
wrt:
	ld	c,SETDMA
	push	de		;pbuf on stack 
	call	BDOS
	LD	C,WRITES
	LD	DE,FCB
	CALL	BDOS
	pop	de		;DE=pbuf
				;increment pbuf
	ld	hl,80h		;DE=DE+80H
	add	hl,de
	ex	de,hl

	xor	a		;CARRY=0
	ld	hl,(count)	;HL=count
	ld	bc,80h
	sbc	hl,bc
	ld	(count),hl	;save count
	ret	c
	ld	a,h
	or	l
	ret	z
	jr	wrt
;
ENDC

COND	M128
;
;	Loads 80H bytes from Upper RAM to Buf80
;
;	HL=addr in Upper RAM
;
;	returns HL=HL+80H
;
Load80:
	ld	d,UPPER_64RAM .or. ROM_OUT_CMD
	ld	e,LOWER_64RAM .or. ROM_OUT_CMD
	ld	c,MEMP_PORT
	ld	b,80H
	ld	ix,Buf80
loop80:
	call	$LoadFromUp
	ld	(ix+0),a
	inc	ix
	inc	hl
	djnz	loop80
	ret
;
ENDC
;
FINISH:
	LD	C,CLOSEF
	LD	DE,FCB
	CALL	BDOS
COND	M512
	ld	a,33		;RAM bank 33 set back to 4000H 
	out	(79H),a
ENDC
	EI
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

BCTOA:	LD   	A,B		; Move the hi order byte to A
	SUB  	30H		; Take it down from Ascii
	CP   	0AH		; Are we in the 0-9 range here?
	JR   	C,BCTOA1	; If so, get the next nybble
	SUB  	7		; But if A-F, take it down some more
BCTOA1:	RLCA			; Rotate the nybble from low to high
	RLCA			; One bit at a time
	RLCA			; Until we
	RLCA			; Get there with it
	LD   	B,A		; Save the converted high nybble
	LD   	A,C		; Now get the low order byte
	SUB  	30H		; Convert it down from Ascii
	CP   	0AH		; 0-9 at this point?
	JR   	C,BCTOA2	; Good enough then, but
	SUB  	7		; Take off 7 more if it's A-F
BCTOA2:	ADD  	A,B		; Add in the high order nybble
	ret

COND	M512
bank:	defs	1		;0 if banks not used, else 36,37...61
crtbank:defs	1
ENDC

COND	M128
UpRAMUsed:defs	1		;0=no, 1=yes
Buf80:	defs	80H
ENDC

checkSum: 	defb	0
byteCount:	defb	0
count:		defs	2	;count for the current buffer
len:		defs	2	;count for the last buffer

OKMess:		defm	'OK$'
chkErrMess: 	defm	'======Checksum Error======$'
countErrMess: 	defm	'======File Length Error======$'
toobigErrMess:	defm	'======File too big======$'

TOPLOW:	defs	2		;upper storage limit in Lower 64KB RAM
BIGBUF:	defs	2		;start of big buffer in Lower 64KB RAM

endcode:			;end of protected code area
				;on the next 100H boundary starts BIGBUF

COND	M128
;
;	Code to store StoreToUp & LoadFromUp (12 bytes) in Upper RAM at 0FFE2H
;
;	will be moved to 8000H and called
;
StoreInUpperRAM:
	ROM_IN
	call	8000H + (get_ltoup_scm - StoreInUpperRAM)
	push	hl
	pop	iy		;IY=SCM function to move A --> UpperRAM in (DE)

				;IX=from, DE=to, A=counter
	ld	ix,8000H + (StoreToUp - StoreInUpperRAM)
	ld	de,$StoreToUp
	ld	a,12
looptoup:
	push	af
	ld	a,(ix+0)
				;(DE) in UpperRAM <-- A
	ld	hl,8000H + (retadr - StoreInUpperRAM)
	push	hl
	jp	(iy)
retadr:	
	inc	ix
	inc	de
	pop	af
	dec	a
	jr	nz,looptoup
	ROM_OUT
	ret
;
;	returns	HL=routine LTOUP_SCM
;
get_ltoup_scm:
	ld	hl,69H
searchjp:
	ld	a,(hl)
	cp	0c3H
	jr	z,jpfound
	inc	hl
	jr	searchjp
jpfound:
	ld	b,6
searchj7thjp:
	inc	hl
	inc	hl
	inc	hl
	ld	a,(hl)
	cp	0c3H
	jr	nz,searchjp
	djnz	searchj7thjp
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl
	ld	bc,2bH
	add	hl,bc
	add	hl,bc
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl
	ret
;
;	Store a byte to upper 64 KB RAM
;	(routine size = 6 bytes, to be stored at 0FFE2H)
;
;	D = UPPER_64RAM .or. ROM_OUT_CMD
;	E = LOWER_64RAM .or. ROM_OUT_CMD
;	C = MEMP_PORT
;	HL = Address to store in Upper 64KB RAM
;	A = byte to store
;
StoreToUp:
	out	(c),d		;select upper 64KB RAM
	ld	(hl),a		;store byte
	out	(c),e		;select lower 64KB RAM
	ret			;return
;
;
;	Load a byte from upper 64 KB RAM
;	(routine size = 6 bytes, to be stored at 0FFE8H)
;
;	D = UPPER_64RAM .or. ROM_OUT_CMD
;	E = LOWER_64RAM .or. ROM_OUT_CMD
;	C = MEMP_PORT
;	HL = Address in Upper 64KB RAM
;	return A = byte
;
LoadFromUp:
	out	(c),d		;select upper 64KB RAM
	ld	a,(hl)		;load byte
	out	(c),e		;select lower 64KB RAM
	ret			;return
;
EndStore:
;		
ENDC

	end	start
