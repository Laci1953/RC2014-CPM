;
;	Overlays manager for 512KB RAM + 512KB ROM systems
;
;	Ladislau Szilagyi, 2023
;
;
	GLOBAL _InitOverlays
	GLOBAL _CallOverlay0
	GLOBAL _ReturnOverlay0
	GLOBAL CrtRamBank

buffer	equ	8000H
port	equ	7AH

;
;Overlays structure:
;
;Overlays manager
;----------------
;
;RAM MEMORY BANKS
;bank 32	= base at 0000
;bank 36...61	= overlays at 4000H
;
;--------
;
;stack after CallOverlayN: N=0 or 1
;SP -->
;	Return addr
;	Routine
;	(Par1,2,3,4 or none)
;	CALLED RAM bank (+ return type if CHECK_PARS)
;
;stack after entring Routine, before (N)CSV call
;SP -->
;	Return addr
;	(Par1,2,3,4 or none)
;	Routine
;	(Par1,2,3,4 or none)
;	CALLER RAM bank (+ return type if CHECK_PARS)
;
;stack after (N)CSV call
;SP -->
;	local vars
;IX -->
;	old IX
;	old IY
;	Return addr
;	(Par1,2,3,4 or none)
;	Routine
;	(Par1,2,3,4 or none)
;	CALLER RAM bank (+ return type if CHECK_PARS)
;
;stack after executing RET at end of ReturnOverlayN 
;SP -->
;	Routine
;	(Par1,2,3,4 or none)
;	CALLER RAM bank (+ return type if CHECK_PARS)
;

openf 	equ 15 	; open file
closef 	equ 16 	; close file
readf 	equ 20	; sequential read
getdsk	equ 25  ; get crt disk
setdma	equ 26	; set DMA addr

__bdos	equ	5

;------------------------- BEGIN PSECT top --------------------------

	psect	top

fcb:				; fcb
	defs	1		; disk+1
	defm	'OPTIM   '	; file name (P1.OVN, N=1,2,3)
ovrname:defm	'OV '		; 
dfcbz:	defb	0		; EX=0
	defs	2		; S1,S2
	defb	0		; RC=0
	defs	16		; D0,...D15
	defb	0		; CR=0
	defb	0,0,0		; R0,R1,R2
fcb_end:
pointer:defs	2

; system interface subroutines
;
open: 	ld 	c,openf
 	jp 	__bdos
;
close: 	ld 	c,closef
 	jp 	__bdos
;
read: 	ld 	c,readf
 	jp 	__bdos
;
setDMA:	ld	c,setdma
	jp	__bdos
;
GetDisk:ld	c,getdsk
	jp	__bdos
;
_cleanfcb:
	ld	hl,dfcbz
	xor	a
	ld	b,fcb_end - dfcbz
clloop:	ld	(hl),a
	inc	hl
	djnz	clloop
	ret
;
;	Read overlay binary files and store them to RAM banks
;
;	C = RAM bank (36,37,38)
;	B = last char of file extenstion ('1', '2' or '3')
;
;	returns CARRY = 0 : OK, 1 : file not found
; 
ReadOverlay:

	ld	a,c		;RAM bank
	out	(port),a	;set to 8000H

	ld	a,b		; open CGEN.OV
	ld	(ovrname+2),a

	call	_cleanfcb

 	ld 	de,fcb 		; source file
 	call 	open 		; error if 255
 	inc 	a 		; 255 becomes 0
	scf			; CARRY=1
 	ret 	z	 	; done if no file

	ld	de,buffer	; init DMA pointer 

readloop:

	ld	(pointer),de
	call	setDMA

	ld	de,fcb
 	call 	read 		; read next record
 	or 	a 		; end of file?
 	jp 	nz,eofile

	ld	de,(pointer)	;increment pointer
	ld	hl,128
	add	hl,de
	ex	de,hl

	jr	readloop

eofile:	ld	de,fcb
	call	close
	xor	a		;CARRY=0
	ret
;
;------------------------- END PSECT top --------------------------
;
	psect	text
;
;bool InitOverlays(void);

_InitOverlays:
	call	GetDisk
	inc	a
	ld	(fcb),a

	ld	bc,3124H	;B='1', C=36
	call	ReadOverlay
	jr	c,nofile
	ld	bc,3225H	;B='2', C=37
	call	ReadOverlay
	jr	c,nofile

	ld	a,32+2		;reselect RAM bank 2 to 8000H
	out	(7AH),a		
	dec	a
	ld	(CrtRamBank),a	;current overlay on 4000H is 33

	ld	l,1
	ret

nofile:	ld	l,0
	ret
;
;short CallOverlay0(void* Routine, char CalledRamBank);
;
_CallOverlay0:
	pop	hl		;HL=return addr
	pop	de		;DE=Routine
	push	de		;Routine back on stack
	push	hl		;return address back on stack
	ld	hl,4
				;DE=Routine, HL=RAM bank offset on stack
	add	hl,sp
	ld	c,(hl)		;C=called RAM bank
	ld	a,(CrtRamBank)
	ld	(hl),a		;save caller RAM bank on stack
	ld	a,c		;C=CalledRamBank
	out	(79H),a		;mapped to 4000H
	ld	(CrtRamBank),a	;set CrtRamBank=CalledRamBank
	ex	de,hl		;HL=Routine
	jp	(hl)
;
;void ReturnOverlay0(short ret_val);
;	returns HL
;
_ReturnOverlay0:
	pop	bc		;drop local return addr
	pop	hl		;HL=ret_val
	ld	a,(ix+8)	;A=CallerRamBank
	ld	sp,ix		;restore SP
	pop	ix		;restore IX
	pop	iy		;restore IY
	ld	(CrtRamBank),a	;set CrtRamBank=CALLER Ram Bank
	out	(79H),a		;mapped to 4000H
	ret 			;return HL
;
	psect	bss

CrtRamBank:	defs	1
;
