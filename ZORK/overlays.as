;
;	Overlays manager
;
	GLOBAL _InitOverlays
	GLOBAL _CallOverlay0,_CallOverlay1
	GLOBAL _ReturnOverlay0,_ReturnOverlay1
	GLOBAL _ReturnOverlay0_

OVERLAYS_COUNT 	equ	28
FIRST_OVR	equ	36

;Overlays structure:
;
;Overlays manager
;----------------
;
;RAM MEMORY BANKS
;bank32		= (0000H-3FFFH)	; (base + overlays) buffer
;bank57		= (4000H-7FFFH) ; DATA1
;bank58		= (8000H-BFFFH) ; DATA2
;bank35		= (C000H-FFFFH)	; COMMON code & data, stack, BDOS,BIOS
;bank36...63	= overlays
;
;--------
;
;stack after CallOverlayN: N=0 or 1
;SP -->
;	Return addr
;	Routine
;	(Par1 or none)
;	CALLED RAM bank
;
;stack after entring Routine, before (N)CSV call
;SP -->
;	Return addr
;	(Par1 or none)
;	Routine
;	(Par1 or none)
;	CALLER RAM bank
;
;stack after (N)CSV call
;SP -->
;	local vars
;IX -->
;	old IX
;	old IY
;	Return addr
;	(Par1 or none)
;	Routine
;	(Par1 or none)
;	CALLER RAM bank
;
;stack after executing RET at end of ReturnOverlayN 
;SP -->
;	Routine
;	(Par1 or none)
;	CALLER RAM bank
;
	psect	nearstack
;
;void InitOverlays(void);

_InitOverlays:
;
;	copy ROM banks 4-26 to RAM banks 36-58,
; 	using (4000H-7FFFH = ROM buffer, 8000H-BFFFH = RAM buffer)
;	
	ld	bc,424H		;B=4=first source ROM bank
				;C=36=first destination RAM bank
copyROMtoRAM:
	ld	a,b		;ROM
	out	(79H),a		;4000H
	ld	a,c		;RAM
	out	(7AH),a		;8000H
				;copy from 4000H to 8000H
	push	bc
	ld	de,8000H
	ld	hl,4000H
	ld	b,h		;BC=4000H
	ld	c,l
	ldir
	pop	bc
	inc	b
	inc	c
	ld	a,b
	cp	27
	jr	nz,copyROMtoRAM

;	for RAM bank 57, select 4000H-7FFFH (DATA1)
	ld	a,57		;RAM bank 57
	out	(79H),a		;mapped to 4000H

;	for RAM bank 58, select 8000H-BFFFH (DATA2)
	ld	a,58		;RAM bank 58
	out	(7AH),a		;mapped to 8000H
;
	ld	a,32
	ld	(CrtRamBank),a	;current overlay is 32=BASE
	out	(0),a		;on leds
				;init "overlay called" flags
	xor	a
	ld	b,OVERLAYS_COUNT
	ld	hl,AlreadyCalled
loopz:
	ld	(hl),a
	inc	hl
	djnz	loopz
;				;save BIOS (0000H-0008H)
	ld	de,BIOS_0_to_8
	ld	hl,0
	ld	bc,8
	ldir
;
	ret
;
	psect	text
;
;short CallOverlay0(void* Routine, char CalledRamBank);
;
_CallOverlay0:
	pop	hl		;HL=return addr
	pop	de		;DE=Routine
	push	de		;Routine back on stack
	push	hl		;return address back on stack
	ld	hl,4
see_already_called:		;DE=Routine, HL=RAM bank offset on stack
	add	hl,sp
	ld	c,(hl)		;C=called RAM bank
	ld	a,(CrtRamBank)
	ld	(hl),a		;save caller RAM bank on stack
	ld	hl,AlreadyCalled
	ld	b,0
	ld	a,c
	sub	FIRST_OVR
	ld	c,a
	add	hl,bc
	add	a,FIRST_OVR	
	ld	c,a		;restore C=CalledRamBank
	ld	a,(hl)
	or	a		;ram bank was already called?
	ld	(hl),1		;mark as called
	ld	a,c		;C=CalledRamBank
	out	(78H),a		;mapped to 0000
	out	(0),a		;on leds
	ld	(CrtRamBank),a	;set CrtRamBank=CalledRamBank
	jr	nz,was_called_already
	push	de		;save Routine
	ld	de,0		;if not called already, setup BIOS (0000-0008)
	ld	hl,BIOS_0_to_8
	ld	c,8		;B is already = 0
	ldir
	pop	de		;restore Routine
was_called_already:
	ex	de,hl		;HL=Routine
	jp	(hl)
;
;short CallOverlay1(void* Routine, short Par1, char CalledRamBank);
;
_CallOverlay1:
	pop	hl		;HL=return addr
	pop	de		;DE=Routine
	pop	bc		;BC=Par1
	push	bc		;Par1 on stack
	push	de		;Routine on stack
	push	bc		;Par1 on stack
	push	hl		;return address on stack
	ld	hl,8
	jr	see_already_called
;
;void ReturnOverlay0(short ret_val);
;	returns HL
;
_ReturnOverlay0:
	pop	bc		;drop local return addr
	pop	hl		;HL=ret_val
	ld	a,(ix+8)	;A=CallerRAMBank
	ld	sp,ix		;restore SP
	pop	ix		;restore IX
	pop	iy		;restore IY
set_caller_bank:
	ld	(CrtRamBank),a	;set CrtRamBank=CALLER Ram Bank
	out	(78H),a		;mapped to 0000
	out	(0),a		;on leds
	ret 			;return HL
;
;void ReturnOverlay0_(short ret_val);
;(in routines that do not call (N)CSV)
;
_ReturnOverlay0_:
	pop	bc		;drop local return addr
	pop	de		;DE=ret_val
	ld	hl,4
	add	hl,sp
	ld	a,(hl)		;A=CallerRAMBank
	ex	de,hl		;HL=ret_val
	jr	set_caller_bank
;
;void ReturnOverlay1(short ret_val);
;	returns HL
;
_ReturnOverlay1:
	pop	bc		;drop local return addr
	pop	hl		;HL=ret_val
	ld	a,(ix+12)	;A=CallerRAMBank
	ld	sp,ix		;restore SP
	pop	ix		;restore IX
	pop	iy		;restore IY
	pop	de		;DE=return addr
	pop	bc		;drop Par1
	push	de		;return addr on stack
	jr	set_caller_bank
;
	psect	bss

AlreadyCalled:	defs	OVERLAYS_COUNT
;	0 - bank not yet called
;	1 - bank was called

CrtRamBank:	defs	1	;0 for base
BIOS_0_to_8:	defs	8	;BIOS (0000H - 0008H)
;
