;
;	Overlays manager
;
	GLOBAL _InitOverlays
	GLOBAL _CallOverlay0,_CallOverlay1,_CallOverlay2
	GLOBAL _ReturnOverlay0,_ReturnOverlay1,_ReturnOverlay2
	GLOBAL _ReturnOverlay0_

;Overlays structure:
;
;Overlays manager
;----------------
;
;ROM MEMORY BANKS
;bank 0		= base
;bank 4...31	= overlays
;
;--------
;
;stack after CallOverlayN: N=0 or 1
;SP -->
;	Return addr
;	Routine
;	(Par1,2 or none)
;	CALLED ROM bank
;
;stack after entring Routine, before (N)CSV call
;SP -->
;	Return addr
;	(Par1,2 or none)
;	Routine
;	(Par1,2 or none)
;	CALLER ROM bank
;
;stack after (N)CSV call
;SP -->
;	local vars
;IX -->
;	old IX
;	old IY
;	Return addr
;	(Par1,2 or none)
;	Routine
;	(Par1,2 or none)
;	CALLER ROM bank
;
;stack after executing RET at end of ReturnOverlayN 
;SP -->
;	Routine
;	(Par1,2 or none)
;	CALLER ROM bank
;
	psect	text
;
;void InitOverlays(void);

_InitOverlays:
	xor	a
	ld	(CrtRomBank),a	;current overlay is 0=BASE
	out	(0),a		;on leds
	ret
;
;short CallOverlay0(void* Routine, char CalledRomBank);
;
_CallOverlay0:
	pop	hl		;HL=return addr
	pop	de		;DE=Routine
	push	de		;Routine back on stack
	push	hl		;return address back on stack
	ld	hl,4
save_caller:			;DE=Routine, HL=ROM bank offset on stack
	add	hl,sp
	ld	c,(hl)		;C=called ROM bank
	ld	a,(CrtRomBank)
	ld	(hl),a		;save caller ROM bank on stack
	ld	a,c		;C=CalledRomBank
	out	(79H),a		;mapped to 4000H
	out	(0),a		;on leds
	ld	(CrtRomBank),a	;set CrtRomBank=CalledRomBank
	ex	de,hl		;HL=Routine
	jp	(hl)
;
;short CallOverlay1(void* Routine, short Par1, char CalledRomBank);
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
	jr	save_caller
;
;short CallOverlay2(void* Routine, short Par1, short Par2, char CalledRomBank);
;
_CallOverlay2:
	pop	de		;DE=return addr
	ld	hl,4
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)		;BC=Par2
	push	bc		;on stack
	dec	hl
	dec	hl
	ld	b,(hl)
	dec	hl
	ld	c,(hl)		;BC=Par1
	push	bc		;on stack
	push	de		;return address on stack
	dec	hl
	ld	d,(hl)
	dec	hl
	ld	e,(hl)		;DE=Routine
	ld	hl,12
	jr	save_caller
;
;void ReturnOverlay0(short ret_val);
;	returns HL
;
_ReturnOverlay0:
	pop	bc		;drop local return addr
	pop	hl		;HL=ret_val
	ld	a,(ix+8)	;A=CallerROMBank
	ld	sp,ix		;restore SP
	pop	ix		;restore IX
	pop	iy		;restore IY
set_caller_bank:
	ld	(CrtRomBank),a	;set CrtRomBank=CALLER Rom Bank
	or	a
	jr	z,1f
	out	(79H),a		;mapped to 4000H
1:	out	(0),a		;on leds
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
	ld	a,(hl)		;A=CallerROMBank
	ex	de,hl		;HL=ret_val
	jr	set_caller_bank
;
;void ReturnOverlay1(short ret_val);
;	returns HL
;
_ReturnOverlay1:
	pop	bc		;drop local return addr
	pop	hl		;HL=ret_val
	ld	a,(ix+12)	;A=CallerROMBank
	ld	sp,ix		;restore SP
	pop	ix		;restore IX
	pop	iy		;restore IY
	pop	de		;DE=return addr
	pop	bc		;drop Par1
	push	de		;return addr on stack
	jr	set_caller_bank
;
;void ReturnOverlay2(short ret_val);
;	returns HL
;
_ReturnOverlay2:
	pop	bc		;drop local return addr
	pop	hl		;HL=ret_val
	ld	a,(ix+16)	;A=CallerROMBank
	ld	sp,ix		;restore SP
	pop	ix		;restore IX
	pop	iy		;restore IY
	pop	de		;DE=return addr
	pop	bc		;drop Par1
	pop	bc		;drop Par2
	push	de		;return addr on stack
	jr	set_caller_bank
;
	psect	bss

CrtRomBank:	defs	1	;0 for base
;
