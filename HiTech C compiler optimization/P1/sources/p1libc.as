include p1.inc

	psect   text

	global rcsv
	global ncsv
	global cret
	global csv
	global _strncpy
	global _strlen
	global _strcpy
	global _strcmp
	global _strcat
	global shar
	global shal
	global shll
	global _sbrk
;	global _brk
	global _rindex
	global almul
	global llmul
	global lmul
	global amul
	global asar
	global divide
	global llmod
	global lldiv
	global sgndiv
	global negat
	global adiv
	global ldiv
	global lmod
	global wrelop
	global brelop
	global lladd
	global aladd
	global _blkclr
	global _atoi
	global _digit
	global asal
	global asll
	global aslladd
	global asaladd
	global _setuc
	global _getuc
	global _unlink
	global __getargs
	global startup
	global _bdos
	global _fflush
	global _bdoshl
	global _fclose
	global _exit
	global __filbuf
	global _fgetc
	global __flsbuf
	global _fputc
	global _printf
	global _pnum
	global _pputc
	global _sprintf
	global _toupper
	global __buffree
	global _setbuf
	global sub_7b7eh
	global _fputs
	global __putchar
	global _free
	global _fopen
	global _fprintf
	global _freopen
	global _fgets
	global _malloc
IF OPTIM
	global popBCpopBCcret
	global popBCcret
	global popHLcret
	global ldHL0cret
	global ldHL1cret
	global ldHLFFFFcret
	global ldL0cret
	global ldL1cret
	global ldLIXm2ldHIXm1cret
	global ldLIXm2cret
	global pushIYpopHLcret
ENDIF

IF SHORTMSG
	global _pr_nr_of_error0
	global _pr_nr_of_error1
	global msgerr0
	global msgerr1
	global _prerror
	global _pr_nr_of_warning0
	global _pr_nr_of_warning1
	global msgwarning0
	global msgwarning1
	global _prwarning
ENDIF

l0200h  equ     200h

_fopen:
	call csv
	ld iy,__iob
	jr l755ah
l7544h:
	ld de,__fcb
	push iy
	pop hl	
	or a	
	sbc hl,de
	jr nz,l7555h
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l7555h:
	ld de,00008h
	add iy,de
l755ah:
	ld a,(iy+006h)
	and 003h
	or a	
	jr nz,l7544h
	push iy
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call _freopen
	pop bc	
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF

_fprintf:
	call csv
	push ix
	pop de	
	ld hl,0000ah
	add hl,de	
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call __doprnt
	pop bc	
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF

_freopen:
	call csv
	push hl	
	push hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	pop iy
	push hl	
	call _fclose
	pop bc	
	ld de,__iob
	push iy
	pop hl	
	or a	
	sbc hl,de
	ld de,00008h
	call adiv
	ld a,l	
	ld (iy+007h),a
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	ld de,00029h
	call amul
	ld de,__fcb
	add hl,de	
	ld (ix-004h),l
	ld (ix-003h),h
	ld (ix-001h),000h
	ld (ix-002h),00fh
	ld a,(iy+006h)
	and 04fh
	ld (iy+006h),a
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld a,(hl)	
	cp 072h
	jr z,l75fdh
	cp 077h
	jr nz,l760dh
	inc (ix-001h)
	ld (ix-002h),016h
l75fdh:
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	ld a,(hl)	
	cp 062h
	jr nz,l760dh
	ld (iy+006h),080h
l760dh:
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	ld l,(ix-004h)
	ld h,(ix-003h)
	push hl	
	call _def_fcb
	pop bc	
	pop bc	
	ld a,(ix-002h)
	cp 016h
	ld l,(ix-004h)
	ld h,(ix-003h)
	push hl	
	jr nz,l763dh
	ld hl,00013h
	push hl	
	call _bdos
	pop bc	
	ld l,(ix-004h)
	ld h,(ix-003h)
	ex (sp),hl	
l763dh:
	ld l,(ix-002h)
	ld h,000h
	push hl	
	call _bdos
	pop bc	
	pop bc	
	ld a,l	
	cp 0ffh
	jr nz,l7653h
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l7653h:
	ld a,(ix-001h)
	or a	
	jr nz,l765eh
	ld hl,00001h
	jr l7661h
l765eh:
	ld hl,00002h
l7661h:
	ld a,l	
	ld e,(ix-004h)
	ld d,(ix-003h)
	ld hl,00024h
	add hl,de	
	ld (hl),a	
	ld a,(ix-001h)
	inc a	
	ld e,a	
	ld a,(iy+006h)
	or e	
	ld (iy+006h),a
	and 00ch
	or a	
	jr nz,l7694h
	ld a,(iy+004h)
	or (iy+005h)
	jr nz,l7694h
	ld hl,l0200h
	push hl	
	call _sbrk
	pop bc	
	ld (iy+004h),l
	ld (iy+005h),h
l7694h:
	ld l,(iy+004h)
	ld h,(iy+005h)
	ld (iy+000h),l
	ld (iy+001h),h
	ld a,l	
	or (iy+005h)
	jr z,l76b6h
	ld a,(ix-001h)
	or a	
	ld (iy+002h),000h
	jr z,l76bah
	ld (iy+003h),002h
	jr l76beh
l76b6h:
	ld (iy+002h),000h
l76bah:
	ld (iy+003h),000h
l76beh:
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

_fgets:
	call csv
	push hl	
	push hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	pop iy
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (ix-002h),l
	ld (ix-001h),h
l76deh:
	ld l,(ix+008h)
	ld h,(ix+009h)
	dec hl	
	ld (ix+008h),l
	ld (ix+009h),h
	inc hl	
	ld a,l	
	or h	
	jr z,l771ah
	push iy
	call _fgetc
	pop bc	
	ld (ix-004h),l
	ld (ix-003h),h
IF OPTIM
	ld	a,h
	and	l
	inc	a
ELSE
	ld de,0ffffh
	or a	
	sbc hl,de
ENDIF
	jr z,l771ah
	ld a,(ix-004h)
	ld l,(ix+006h)
	ld h,(ix+007h)
	inc hl	
	ld (ix+006h),l
	ld (ix+007h),h
	dec hl	
	ld (hl),a	
	cp 00ah
	jr nz,l76deh
l771ah:
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (hl),000h
	ld e,(ix-002h)
	ld d,(ix-001h)
	or a	
	sbc hl,de
	jr nz,l7733h
	ld hl,00000h
	jp cret
l7733h:
IF OPTIM
	jp      ldLIXm2ldHIXm1cret
ELSE
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp cret
ENDIF
;				unused !!!
;	call csv
;	ld hl,__iob
;	push hl	
;	ld hl,0ffffh
;	push hl	
;	ld l,(ix+006h)
;	ld h,(ix+007h)
;	push hl	
;	call _fgets
;	pop bc	
;	pop bc	
;	pop bc	
;	ld (ix+006h),l
;	ld (ix+007h),h
;	ld a,l	
;	or h	
;	jr nz,l7764h
;	ld hl,00000h
;	jp cret
;l7764h:
;	ld l,(ix+006h)
;	ld h,(ix+007h)
;	push hl	
;	call _strlen
;	pop bc	
;	ld e,(ix+006h)
;	ld d,(ix+007h)
;	add hl,de	
;	dec hl	
;	ld (hl),000h
;	ld l,e	
;	ld h,d	
;	jp cret

_setSize:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld hl,00080h
	call wrelop
	jp p,l77b3h
	ld l,(ix+008h)
	ld h,(ix+009h)
	dec hl	
	dec hl	
	dec hl	
	ld (iy+000h),l
	ld (iy+001h),h
	ld de,00003h
	add iy,de
	ld (iy-001h),080h
	jr l77bdh
l77b3h:
	ld a,(ix+008h)
	add a,0ffh
	ld (iy+000h),a
	inc iy
l77bdh:
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

_pack:
	call ncsv
	defw 0fff7h
	ld (ix-009h),000h
l77cch:
	ld hl,buf53
	ld (ix-008h),l
	ld (ix-007h),h
	jp l7922h
l77d8h:
	push iy
	pop hl	
	ld (ix-004h),l
	ld (ix-003h),h
	ld a,(iy-001h)
	ld l,a	
	rla	
	sbc a,a	
	bit 7,l
	ld l,(ix-004h)
	dec hl	
	jr z,l77f9h
	dec hl	
	dec hl	
	ld (ix-004h),l
	ld (ix-003h),h
	jr l77ffh
l77f9h:
	ld (ix-004h),l
	ld (ix-003h),h
l77ffh:
	ld hl,buf53
	ld (ix-006h),l
	ld (ix-005h),h
	jp l7905h
l780bh:
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld l,e	
	ld h,d	
	dec hl	
	bit 7,(hl)
	jr nz,l781dh
	ld l,(hl)	
	ld h,000h
	jr l7823h
l781dh:
	dec hl	
	dec hl	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
l7823h:
	add hl,de	
	ld e,(ix-004h)
	ld d,(ix-003h)
	or a	
	sbc hl,de
	jp nz,l78f9h
	push iy
	pop de	
	ld l,(ix-006h)
	ld h,(ix-005h)
	or a	
	sbc hl,de
	jr nz,l7852h
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld l,(ix-008h)
	ld h,(ix-007h)
	ld (hl),c	
	inc hl	
	ld (hl),b	
	jr l7895h
l7852h:
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld l,(ix-008h)
	ld h,(ix-007h)
	or a	
	sbc hl,de
	jr nz,l7874h
	ld e,(iy+000h)
	ld d,(iy+001h)
	ld l,(ix-006h)
	ld h,(ix-005h)
	ld (hl),e	
	inc hl	
	ld (hl),d	
	jr l7895h
l7874h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld l,(ix-006h)
	ld h,(ix-005h)
	ld (hl),c	
	inc hl	
	ld (hl),b	
	ld e,(iy+000h)
	ld d,(iy+001h)
	ld l,(ix-008h)
	ld h,(ix-007h)
	ld (hl),e	
	inc hl	
	ld (hl),d	
l7895h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	dec hl	
	ld a,(hl)	
	ld l,a	
	rla	
	sbc a,a	
	bit 7,l
	ld l,(ix-002h)
	ld h,(ix-001h)
	dec hl	
	jr z,l78b5h
	dec hl	
	dec hl	
	ld (ix-002h),l
	ld (ix-001h),h
	jr l78bbh
l78b5h:
	ld (ix-002h),l
	ld (ix-001h),h
l78bbh:
	bit 7,(iy-001h)
	jr nz,l78c8h
	ld l,(iy-001h)
	ld h,000h
	jr l78ceh
l78c8h:
	ld l,(iy-003h)
	ld h,(iy-002h)
l78ceh:
	push hl	
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00000h
	or a	
	sbc hl,de
	pop de	
	add hl,de	
	push iy
	pop de	
	add hl,de	
	push hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	call _setSize
	pop bc	
	ex (sp),hl	
	call _free
	pop bc	
	ld (ix-009h),001h
	jp l77cch
l78f9h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (ix-006h),l
	ld (ix-005h),h
l7905h:
	ld l,(ix-006h)
	ld h,(ix-005h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (ix-002h),c
	ld (ix-001h),b
	ld a,c	
	or b	
	jp nz,l780bh
	push iy
	pop hl	
	ld (ix-008h),l
	ld (ix-007h),h
l7922h:
	ld l,(ix-008h)
	ld h,(ix-007h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	pop iy
	ld a,c	
	or b	
	jp nz,l77d8h
	jp cret

_malloc:
	call ncsv
	defw 0fffbh
	ld de,00002h
	ld l,(ix+006h)
	ld h,(ix+007h)
	call wrelop
	jr nc,l7951h
	ld (ix+006h),002h
	ld (ix+007h),000h
l7951h:
	ld (ix-005h),000h
l7955h:
	ld iy,buf53
	jp l7a1fh
l795ch:
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld l,(ix-002h)
	ld h,(ix-001h)
	dec hl	
	bit 7,(hl)
	jr nz,l7972h
	ld l,(hl)	
	ld h,000h
	jr l7978h
l7972h:
	dec hl	
	dec hl	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
l7978h:
	call wrelop
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp c,l7a1ch
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (iy+000h),c
	ld (iy+001h),b
	dec hl	
	push hl	
	pop iy
	bit 7,(iy-001h)
	jr nz,l799eh
	ld l,(iy-001h)
	ld h,000h
	jr l79a4h
l799eh:
	ld l,(iy-003h)
	ld h,(iy-002h)
l79a4h:
	ex de,hl	
	push ix
	pop hl	
	dec hl	
	dec hl	
	dec hl	
	dec hl	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	push de	
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,00006h
	add hl,de	
	pop de	
	call wrelop
	push iy
	jr nc,l7a18h
	pop de	
	ld l,(ix+006h)
	ld h,(ix+007h)
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld l,(ix-004h)
	ld h,(ix-003h)
	or a	
	sbc hl,de
	push hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	call _setSize
	pop bc	
	ex (sp),hl	
	call _free
	pop bc	
	ld a,(iy-001h)
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	bit 7,l
	jr z,l7a02h
	ld de,0fffdh
	add iy,de
	jr l7a07h
l7a02h:
	ld de,0ffffh
	add iy,de
l7a07h:
	ld l,(ix+006h)
	ld h,(ix+007h)
l7a0dh:
	push hl	
	push iy
	call _setSize
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF
l7a18h:
	pop hl	
	jp cret
l7a1ch:
	push hl	
	pop iy
l7a1fh:
	ld l,(iy+000h)
	ld h,(iy+001h)
	ld (ix-002h),l
	ld (ix-001h),h
	ld a,l	
	or h	
	jp nz,l795ch
	ld a,(ix-005h)
	or a	
	jr nz,l7a41h
	call _pack
	ld a,l	
	ld (ix-005h),a
	or a	
	jp nz,l7955h
l7a41h:
	ld de,00080h
	ld l,(ix+006h)
	ld h,(ix+007h)
	call wrelop
	jr c,l7a54h
	ld hl,00003h
	jr l7a57h
l7a54h:
	ld hl,00001h
l7a57h:
	ld (ix-004h),l
	ld (ix-003h),h
	ex de,hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	add hl,de	
	push hl	
	call _sbrk
	pop bc	
	push hl	
	pop iy
IF OPTIM
	ld	a,h
	and	l
	inc	a
ELSE
	ld de,0ffffh
	or a	
	sbc hl,de
ENDIF
	jr nz,l7a7bh
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l7a7bh:
	ld e,(ix-004h)
	ld d,(ix-003h)
	ld l,(ix+006h)
	ld h,(ix+007h)
	add hl,de	
	jr l7a0dh

_free:
	call csv
	ld iy,buf53
	jr l7a9ch
l7a93h:
	ld l,(iy+000h)
	ld h,(iy+001h)
	push hl	
	pop iy
l7a9ch:
	ld a,(iy+000h)
	or (iy+001h)
	jr z,l7ad6h
	ld l,(ix+006h)
	ld h,(ix+007h)
	dec hl	
	bit 7,(hl)
	jr nz,l7ab4h
	ld l,(hl)	
	ld h,000h
	jr l7abah
l7ab4h:
	dec hl	
	dec hl	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
l7abah:
	ex de,hl	
	ld l,(iy+000h)
	ld h,(iy+001h)
	dec hl	
	bit 7,(hl)
	jr nz,l7acbh
	ld l,(hl)	
	ld h,000h
	jr l7ad1h
l7acbh:
	dec hl	
	dec hl	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
l7ad1h:
	call wrelop
	jr c,l7a93h
l7ad6h:
	ld e,(iy+000h)
	ld d,(iy+001h)
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (hl),e	
	inc hl	
	ld (hl),d	
	dec hl	
	ld (iy+000h),l
	ld (iy+001h),h
	jp cret

_printf:
	call csv
	push ix
	pop de	
	ld hl,00008h
	add hl,de	
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	ld hl,l9bc1h
	push hl	
	call __doprnt
	pop bc	
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF
;				unused !!!
;	ld hl,__iob
;	push hl	
;	call _fgetc
;	pop bc	
;	ret
	
__putchar:
	call csv
	ld hl,l9bc1h
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call _fputc
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF

_fputs:
	call csv
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	pop iy
	jr l7b55h
l7b3bh:
	push iy
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld a,(hl)	
	inc hl	
	ld (ix+006h),l
	ld (ix+007h),h
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	push hl	
	call _fputc
	pop bc	
	pop bc	
l7b55h:
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld a,(hl)	
	or a	
	jr nz,l7b3bh
	jp cret
;				unused !!!
;	call csv
;	ld hl,l9bc1h
;	push hl	
;	ld l,(ix+006h)
;	ld h,(ix+007h)
;	push hl	
;	call _fputs
;	pop bc	
;	ld hl,0000ah
;	ex (sp),hl	
;	call __putchar
;	jp cret

sub_7b7eh:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld de,__iob
	push iy
	pop hl	
	or a	
	sbc hl,de
	ld de,00008h
	call adiv
	ld de,00029h
	call amul
	ld de,l9c05h
	add hl,de	
	ld a,(hl)	
	ld b,004h
	call brelop
	jr nc,_setbuf
	ld a,(ix+008h)
	or (ix+009h)
	jr nz,_setbuf
IF OPTIM
	jp      ldHLFFFFcret
ELSE
	ld hl,0ffffh
	jp cret
ENDIF

_setbuf:
	bit 3,(iy+006h)
	jr nz,l7bd2h
	ld a,(iy+004h)
	or (iy+005h)
	jr z,l7bd2h
	ld l,(iy+004h)
	ld h,(iy+005h)
	push hl	
	call __buffree
	pop bc	
l7bd2h:
	ld (iy+002h),000h
	ld (iy+003h),000h
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld (iy+000h),l
	ld (iy+001h),h
	ld (iy+004h),l
	ld (iy+005h),h
	ld a,l	
	or h	
	jr nz,l7bfbh
	ld a,(iy+006h)
	and 0f7h
	ld (iy+006h),a
	jp cret
l7bfbh:
	set 3,(iy+006h)
	bit 1,(iy+006h)
	jr z,l7c0dh
	ld (iy+002h),000h
	ld (iy+003h),002h
l7c0dh:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
;				unused !!!
;	call csv
;	ld iy,(buf42)
;	push iy
;	pop hl	
;	ld a,l	
;	or h	
;	jr z,l7c2ch
;	ld l,(iy+000h)
;	ld h,(iy+001h)
;	ld (buf42),hl
;	jr l7c37h
;l7c2ch:
;	ld hl,l0200h
;	push hl	
;	call _sbrk
;	pop bc	
;	push hl	
;	pop iy
;l7c37h:
;	push iy
;	pop hl	
;	jp cret

__buffree:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld hl,(buf42)
	ld (iy+000h),l
	ld (iy+001h),h
	ld (buf42),iy
	jp cret

_toupper:
	call csv
	ld b,061h
	ld a,(ix+006h)
	call brelop
	jp m,l7c80h
	ld a,(ix+006h)
	ld e,a	
	rla	
	sbc a,a	
	ld d,a	
	ld hl,0007ah
	call wrelop
	jp m,l7c80h
	ld a,(ix+006h)
	add a,0e0h
	ld l,a	
	jp cret
l7c80h:
	ld l,(ix+006h)
	jp cret

_def_fcb:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld (iy+000h),000h
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	ld a,(hl)	
	cp 03ah
	jr nz,l7cbbh
	dec hl	
	ld a,(hl)	
	ld l,a	
	rla	
	ld a,l	
	and 00fh
	ld (iy+000h),a
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	inc hl	
	ld (ix+008h),l
	ld (ix+009h),h
l7cbbh:
	push iy
	pop hl	
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
	jr l7ceeh
l7cc7h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld a,(hl)	
	inc hl	
	ld (ix+008h),l
	ld (ix+009h),h
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	push hl	
	call _toupper
	pop bc	
	ld e,l	
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
	dec hl	
	ld (hl),e	
l7ceeh:
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld a,(hl)	
	cp 02eh
	jr z,l7d2ch
	ld a,(hl)	
	ld e,a	
	rla	
	sbc a,a	
	ld d,a	
	ld hl,00009h
	call wrelop
	push iy
	pop de	
	ld hl,00009h
	jp p,l7d32h
	add hl,de	
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	call wrelop
	jr c,l7cc7h
	jr l7d2ch
l7d1ch:
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
	dec hl	
	ld (hl),020h
l7d2ch:
	push iy
	pop de	
	ld hl,00009h
l7d32h:
	add hl,de	
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	call wrelop
	jr c,l7d1ch
l7d3fh:
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld a,(hl)	
	or a	
	ld a,(hl)	
	jr z,l7d85h
	inc hl	
	ld (ix+008h),l
	ld (ix+009h),h
	cp 02eh
	jr nz,l7d3fh
	jr l7d7eh
l7d57h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld a,(hl)	
	inc hl	
	ld (ix+008h),l
	ld (ix+009h),h
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	push hl	
	call _toupper
	pop bc	
	ld e,l	
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
	dec hl	
	ld (hl),e	
l7d7eh:
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld a,(hl)	
l7d85h:
	ld e,a	
	rla	
	sbc a,a	
	ld d,a	
	ld hl,00009h
	call wrelop
	push iy
	pop de	
	ld hl,0000ch
	jp p,l7dbdh
	add hl,de	
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	call wrelop
	jr c,l7d57h
	jr l7db7h
l7da7h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
	dec hl	
	ld (hl),020h
l7db7h:
	push iy
	pop de	
	ld hl,0000ch
l7dbdh:
	add hl,de	
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	call wrelop
	jr c,l7da7h
	xor a	
	ld (iy+020h),a
	ld (iy+00ch),a
	ld l,a	
	jp cret

_sprintf:
	call csv
	ld hl,07fffh
	ld (buf43),hl
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (buf44),hl
	ld hl,buf54
	ld (hl),082h
	push ix
	pop de	
	ld hl,0000ah
	add hl,de	
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld hl,buf44
	push hl	
	call __doprnt
	pop bc	
	pop bc	
	pop bc	
	ld hl,(buf44)
	ld (hl),000h
	ld e,(ix+006h)
	ld d,(ix+007h)
	or a	
	sbc hl,de
	jp cret

_pputc:
	call csv
	ld hl,(_ffile)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call _fputc
	jp cret

_pnum:
	call csv
	push hl	
	push hl	
	ld a,(_left)
	ld e,a	
	dec a	
	ld (_left),a
	ld a,(_base)
	ld hl,00000h
	ld d,l	
	ld e,a	
	push hl	
	push de	
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld l,(ix+008h)
	ld h,(ix+009h)
	call lldiv
	ld (ix-004h),e
	ld (ix-003h),d
	ld (ix-002h),l
	ld (ix-001h),h
	ld a,e	
	or d	
	or l	
	or h	
	jr z,l7e7bh
	push hl	
	push de	
	call _pnum
	pop bc	
	pop bc	
	jr l7e7bh
l7e6bh:
	ld a,(_left)
	ld e,a	
	dec a	
	ld (_left),a
	ld hl,00020h
	push hl	
	call _pputc
	pop bc	
l7e7bh:
	ld a,(_left)
	ld e,a	
	rla	
	sbc a,a	
	ld d,a	
	ld hl,00000h
	call wrelop
	jp m,l7e6bh
	ld a,(_base)
	ld hl,00000h
	ld d,l	
	ld e,a	
	push hl	
	push de	
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld l,(ix+008h)
	ld h,(ix+009h)
	call llmod
	ex de,hl	
	ld de,l9ba8h
	add hl,de	
	ld l,(hl)	
	ld h,000h
	push hl	
	call _pputc
	jp cret

__doprnt:
	call ncsv
	defw 0fff6h
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	pop iy
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (_ffile),hl
	jp l8035h
l7ecdh:
	ld a,(ix-001h)
	cp 025h
	jr z,l7edfh
	ld l,a	
	ld h,000h
	push hl	
l7ed8h:
	call _pputc
	pop bc	
	jp l8035h
l7edfh:
	ld a,00ah
	ld (_base),a
	ld (ix-004h),0ffh
	ld (ix-002h),000h
	ld a,000h
	ld (_left),a
	ld (ix-003h),001h
l7ef5h:
	ld a,(iy+000h)
	inc iy
	ld (ix-001h),a
	or a	
	jp z,cret
	cp 02ah
	jr z,l7f5dh
	cp 02eh
	jp z,l80f3h
	cp 063h
	jp z,l805bh
	cp 064h
	jp z,l8044h
	cp 06ch
	jr z,l7f6eh
	cp 06fh
	jr z,l7f74h
	cp 073h
	jp z,l807fh
	cp 075h
	jr z,l7f79h
	ld b,030h
	call brelop
	jp m,l8127h
	ld a,(ix-001h)
	ld e,a	
	rla	
	sbc a,a	
	ld d,a	
	ld hl,00039h
	call wrelop
	jp m,l8127h
	ld de,0000ah
	ld a,(_left)
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	call amul
	ld a,l	
	ld (_left),a
	ld a,(ix-001h)
	add a,0d0h
	ld e,a	
	ld a,(_left)
	add a,e	
l7f58h:
	ld (_left),a
	jr l7ef5h
l7f5dh:
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	ld a,(hl)	
	inc hl	
	inc hl	
	ld (ix+00ah),l
	ld (ix+00bh),h
	jr l7f58h
l7f6eh:
	ld (ix-003h),002h
	jr l7ef5h
l7f74h:
	ld a,008h
	ld (_base),a
l7f79h:
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	ld e,(hl)	
	inc hl	
	ld d,(hl)	
	ld a,d	
	rla	
	sbc a,a	
	ld l,a	
	ld h,a	
	ld (ix-008h),e
	ld (ix-007h),d
	ld (ix-006h),l
	ld (ix-005h),h
	ld a,(ix-002h)
	or a	
	jr nz,l7fb1h
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	ld e,(hl)	
	inc hl	
	ld d,(hl)	
	ld hl,00000h
	ld (ix-008h),e
	ld (ix-007h),d
	ld (ix-006h),l
	ld (ix-005h),h
l7fb1h:
	ld a,(ix-003h)
	cp 001h
	jr z,l7fd2h
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	ld e,(hl)	
	inc hl	
	ld d,(hl)	
	inc hl	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	ld (ix-008h),e
	ld (ix-007h),d
	ld (ix-006h),l
	ld (ix-005h),h
l7fd2h:
	ld a,(ix-002h)
	or a	
	jr z,l800eh
	bit 7,(ix-005h)
	ld e,(ix-008h)
	ld d,(ix-007h)
	ld l,(ix-006h)
	ld h,(ix-005h)
	push hl	
	push de	
	jr z,l801ch
	ld hl,00000h
	pop bc	
	or a	
	sbc hl,bc
	pop bc	
	ex de,hl	
	ld hl,00000h
	sbc hl,bc
	ld (ix-008h),e
	ld (ix-007h),d
	ld (ix-006h),l
	ld (ix-005h),h
	ld hl,0002dh
	push hl	
	call _pputc
	pop bc	
l800eh:
	ld e,(ix-008h)
	ld d,(ix-007h)
	ld l,(ix-006h)
	ld h,(ix-005h)
	push hl	
	push de	
l801ch:
	call _pnum
	pop bc	
	pop bc	
	ld l,(ix-003h)
	ld h,000h
	add hl,hl	
	ex de,hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	add hl,de	
	ld (ix+00ah),l
	ld (ix+00bh),h
l8035h:
	ld a,(iy+000h)
	inc iy
	ld (ix-001h),a
	or a	
	jp nz,l7ecdh
	jp cret
l8044h:
	ld (ix-002h),001h
	jp l7f79h
l804bh:
	ld hl,00020h
	push hl	
	call _pputc
	pop bc	
	ld a,(_left)
	ld e,a	
	dec a	
	ld (_left),a
l805bh:
	ld a,(_left)
	ld e,a	
	rla	
	sbc a,a	
	ld d,a	
	ld hl,00001h
	call wrelop
	jp m,l804bh
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	inc hl	
	ld (ix+00ah),l
	ld (ix+00bh),h
	push bc	
	jp l7ed8h
l807fh:
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	inc hl	
	ld (ix+00ah),l
	ld (ix+00bh),h
	ld (ix-00ah),c
	ld (ix-009h),b
	ld l,c	
	ld h,b	
	push hl	
	call _strlen
	pop bc	
	ld (ix-002h),l
	ld b,(ix-004h)
	ld a,l	
	call brelop
	jr nc,l80c0h
	ld a,(ix-002h)
	ld (ix-004h),a
	jr l80c0h
l80b0h:
	ld hl,00020h
	push hl	
	call _pputc
	pop bc	
	ld a,(_left)
	ld e,a	
	dec a	
	ld (_left),a
l80c0h:
	ld a,(_left)
	ld e,a	
	ld d,000h
	ld l,(ix-004h)
	ld h,d	
	call wrelop
	jr c,l80b0h
	jr l80e7h
l80d1h:
	ld l,(ix-00ah)
	ld h,(ix-009h)
	inc hl	
	ld (ix-00ah),l
	ld (ix-009h),h
	dec hl	
	ld l,(hl)	
	ld h,000h
	push hl	
	call _pputc
	pop bc	
l80e7h:
	ld a,(ix-004h)
	dec (ix-004h)
	or a	
	jr nz,l80d1h
	jp l8035h
l80f3h:
	ld a,(iy+000h)
	cp 02ah
	jr nz,l8115h
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	inc hl	
	ld (ix+00ah),l
	ld (ix+00bh),h
	push bc	
	call _atoi
	pop bc	
	ld (ix-004h),l
	jp l7ef5h
l8115h:
	ld a,(iy+000h)
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	ld a,l	
	add a,0d0h
	ld (ix-004h),a
	inc iy
	jp l7ef5h
l8127h:
	ld l,(ix-001h)
	ld h,000h
	push hl	
	jp l7ed8h

_fputc:
	pop de	
	pop bc	
	ld b,000h
	ex (sp),iy
	bit 1,(iy+006h)
	jr z,l8183h
	bit 7,(iy+006h)
	jr nz,l8156h
	ld a,c	
	cp 00ah
	jr nz,l8156h
	push bc	
	push de	
	push iy
	ld hl,0000dh
	push hl	
	call _fputc
	pop hl	
	pop bc	
	pop de	
	pop bc	
l8156h:
	ld l,(iy+002h)
	ld h,(iy+003h)
	ld a,l	
	or h	
	jr z,l817ch
	dec hl	
	ld (iy+002h),l
	ld (iy+003h),h
	ld l,(iy+000h)
	ld h,(iy+001h)
	ld (hl),c	
	inc hl	
	ld (iy+000h),l
	ld (iy+001h),h
l8175h:
	ex (sp),iy
	push bc	
	push de	
	ld l,c	
	ld h,b	
	ret	
l817ch:
	ex (sp),iy
	push bc	
	push de	
	jp __flsbuf
l8183h:
	ld bc,0ffffh
	jr l8175h

__flsbuf:
	call csv
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	pop iy
	call _test_brk
	ld de,__iob
	push iy
	pop hl	
	or a	
	sbc hl,de
	ld de,00008h
	call adiv
	ld de,00029h
	call amul
	ld de,__fcb
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	bit 1,(iy+006h)
	jr z,l8217h
	ld a,(iy+004h)
	or (iy+005h)
	jr nz,l81f0h
	ld (iy+002h),000h
	ld (iy+003h),000h
	ex de,hl	
	ld hl,00024h
	add hl,de	
	ld a,(hl)	
	cp 004h
	jr nz,l8217h
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	ld hl,00002h
	push hl	
	call _bdos
	pop bc	
	pop bc	
l81e7h:
	ld l,(ix+006h)
	ld h,(ix+007h)
	jp cret
l81f0h:
	push iy
	call _fflush
	pop bc	
	ld a,(ix+006h)
	ld l,(iy+000h)
	ld h,(iy+001h)
	inc hl	
	ld (iy+000h),l
	ld (iy+001h),h
	dec hl	
	ld (hl),a	
	ld l,(iy+002h)
	ld h,(iy+003h)
	dec hl	
	ld (iy+002h),l
	ld (iy+003h),h
	jr l81e7h
l8217h:
IF OPTIM
	jp      ldHLFFFFcret
ELSE
	ld hl,0ffffh
	jp cret
ENDIF

_fgetc:
	pop de	
	ex (sp),iy
	ld a,(iy+006h)
	bit 0,a
	jr z,l8279h
	bit 4,a
	jr nz,l8279h
l822bh:
	ld l,(iy+002h)
	ld h,(iy+003h)
	ld a,l	
	or h	
	jr z,l8284h
	dec hl	
	ld (iy+002h),l
	ld (iy+003h),h
	ld l,(iy+000h)
	ld h,(iy+001h)
	ld a,(hl)	
	inc hl	
	ld (iy+000h),l
	ld (iy+001h),h
l824ah:
	bit 7,(iy+006h)
	jr z,l8257h
l8250h:
	ld l,a	
	ld h,000h
	ex (sp),iy
	push de	
	ret	
l8257h:
	cp 00dh
	jr z,l822bh
	cp 01ah
	jr nz,l8250h
	ld l,(iy+002h)
	ld h,(iy+003h)
	inc hl	
	ld (iy+002h),l
	ld (iy+003h),h
	ld l,(iy+000h)
	ld h,(iy+001h)
	dec hl	
	ld (iy+000h),l
	ld (iy+001h),h
l8279h:
	set 4,(iy+006h)
	ld hl,0ffffh
	ex (sp),iy
	push de	
	ret	
l8284h:
	bit 6,(iy+006h)
	jr nz,l8279h
	push de	
	push iy
	call __filbuf
	ld a,l	
	pop bc	
	pop de	
	bit 7,h
	jr nz,l8279h
	jr l824ah

__filbuf:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld de,__iob
	push iy
	pop hl	
	or a	
	sbc hl,de
	ld de,00008h
	call adiv
	ld de,00029h
	call amul
	ld de,__fcb
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	ld (iy+002h),000h
	ld (iy+003h),000h
	call _test_brk
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00024h
	add hl,de	
	ld a,(hl)	
	cp 001h
	jp nz,l835ch
	ld l,(iy+004h)
	ld h,(iy+005h)
	ld (iy+000h),l
	ld (iy+001h),h
	jr l8321h
l82eeh:
	ld l,(iy+000h)
	ld h,(iy+001h)
	push hl	
	ld hl,0001ah
	push hl	
	call _bdos
	pop bc	
	ld l,(ix-002h)
	ld h,(ix-001h)
	ex (sp),hl	
	ld hl,00014h
	push hl	
	call _bdos
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr nz,l8337h
	ld de,00080h
	ld l,(iy+000h)
	ld h,(iy+001h)
	add hl,de	
	ld (iy+000h),l
	ld (iy+001h),h
l8321h:
	ld e,(iy+004h)
	ld d,(iy+005h)
	ld hl,l0200h
	add hl,de	
	ex de,hl	
	ld l,(iy+000h)
	ld h,(iy+001h)
	call wrelop
	jr c,l82eeh
l8337h:
	ld e,(iy+004h)
	ld d,(iy+005h)
	ld l,(iy+000h)
	ld h,(iy+001h)
	or a	
	sbc hl,de
	ld (iy+002h),l
	ld (iy+003h),h
	ld l,e	
	ld h,d	
	ld (iy+000h),l
	ld (iy+001h),h
	ld a,(iy+002h)
	or (iy+003h)
	jr nz,l8362h
l835ch:
IF OPTIM
	jp      ldHLFFFFcret
ELSE
	ld hl,0ffffh
	jp cret
ENDIF
l8362h:
	ld l,(iy+002h)
	ld h,(iy+003h)
	dec hl	
	ld (iy+002h),l
	ld (iy+003h),h
	ld l,(iy+000h)
	ld h,(iy+001h)
	inc hl	
	ld (iy+000h),l
	ld (iy+001h),h
	dec hl	
	ld l,(hl)	
	ld h,000h
	jp cret

_test_brk:
	call csv
	push hl	
	ld hl,0000bh
	push hl	
	call _bdos
	pop bc	
	ld a,l	
	or a	
	jp z,cret
	ld hl,00001h
	push hl	
	call _bdos
	pop bc	
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 003h
	jp nz,cret
	call _exit
	jp cret

_exit:
	call csv
	push hl	
	ld (ix-001h),005h
	ld iy,__iob
l83b8h:
	push iy
	pop hl	
	ld bc,00008h
	add hl,bc	
	push hl	
	pop iy
	sbc hl,bc
	push hl	
	call _fclose
	pop bc	
	ld a,(ix-001h)
	add a,0ffh
	ld (ix-001h),a
	or a	
	jr nz,l83b8h
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (00080h),hl
	call 00000h
	jp cret

_fclose:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld de,__iob
	push iy
	pop hl	
	or a	
	sbc hl,de
	ld de,00008h
	call adiv
	ld de,00029h
	call amul
	ld de,__fcb
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	ld a,(iy+006h)
	and 003h
	or a	
	jr nz,l841dh
IF OPTIM
	jp      ldHLFFFFcret
ELSE
	ld hl,0ffffh
	jp cret
ENDIF
l841dh:
	push iy
	call _fflush
	pop bc	
	ld a,(iy+006h)
	and 0f8h
	ld (iy+006h),a
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00024h
	add hl,de	
	ld a,(hl)	
	cp 002h
	jr z,l8446h
	ld hl,0000ch
	push hl	
	call _bdoshl
	pop bc	
	bit 0,h
	jr z,l8456h
l8446h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	ld hl,00010h
	push hl	
	call _bdos
	pop bc	
	pop bc	
l8456h:
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00024h
	add hl,de	
	ld (hl),000h
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF

_bdoshl:
	call csv
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld c,(ix+006h)
	push ix
	call 00005h
	pop ix
	jp cret

_fflush:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld de,__iob
	push iy
	pop hl	
	or a	
	sbc hl,de
	ld de,00008h
	call adiv
	ld de,00029h
	call amul
	ld de,__fcb
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	bit 1,(iy+006h)
	jr nz,l84b6h
IF OPTIM
	jp      ldHLFFFFcret
ELSE
	ld hl,0ffffh
	jp cret
ENDIF
l84b6h:
	ld a,(iy+002h)
	and 07fh
	ld l,a	
	xor a	
	ld h,a	
	ld a,l	
	or h	
	jr z,l84e9h
	ld b,004h
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00024h
	add hl,de	
	ld a,(hl)	
	call brelop
	jr nc,l84e9h
	ld l,(iy+000h)
	ld h,(iy+001h)
	ld (hl),01ah
	ld l,(iy+002h)
	ld h,(iy+003h)
	dec hl	
	ld (iy+002h),l
	ld (iy+003h),h
l84e9h:
	ld l,(iy+004h)
	ld h,(iy+005h)
	ld (iy+000h),l
	ld (iy+001h),h
	ld a,l	
	or h	
	jr nz,l84ffh
l84f9h:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l84ffh:
	ld e,(iy+002h)
	ld d,(iy+003h)
	ld hl,l0200h
	or a	
	sbc hl,de
	ld (iy+002h),l
	ld (iy+003h),h
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00024h
	add hl,de	
	ld a,(hl)	
	cp 002h
	jr z,l8572h
	cp 004h
	jr z,l855eh
l8524h:
	ld (iy+002h),000h
	ld (iy+003h),000h
	ld l,(iy+004h)
	ld h,(iy+005h)
	ld (iy+000h),l
	ld (iy+001h),h
	ld (iy+002h),000h
	ld (iy+003h),002h
	jr l84f9h
l8542h:
	ld l,(iy+000h)
	ld h,(iy+001h)
	ld a,(hl)	
	inc hl	
	ld (iy+000h),l
	ld (iy+001h),h
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	push hl	
	ld hl,00002h
	push hl	
	call _bdos
	pop bc	
	pop bc	
l855eh:
	ld l,(iy+002h)
	ld h,(iy+003h)
	dec hl	
	ld (iy+002h),l
	ld (iy+003h),h
	inc hl	
	ld a,l	
	or h	
	jr nz,l8542h
	jr l8524h
l8572h:
	ld e,(iy+002h)
	ld d,(iy+003h)
	ld hl,0007fh
	add hl,de	
	ld de,00080h
	call adiv
	ld (iy+002h),l
	ld (iy+003h),h
	jr l85beh
l858ah:
	ld l,(iy+000h)
	ld h,(iy+001h)
	push hl	
	ld hl,0001ah
	push hl	
	call _bdos
	pop bc	
	ld l,(ix-002h)
	ld h,(ix-001h)
	ex (sp),hl	
	ld hl,00015h
	push hl	
	call _bdos
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jp nz,l8524h
	ld de,00080h
	ld l,(iy+000h)
	ld h,(iy+001h)
	add hl,de	
	ld (iy+000h),l
	ld (iy+001h),h
l85beh:
	ld l,(iy+002h)
	ld h,(iy+003h)
	dec hl	
	ld (iy+002h),l
	ld (iy+003h),h
	inc hl	
	ld a,l	
	or h	
	jr nz,l858ah
	jp l8524h

_bdos:
	call csv
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld c,(ix+006h)
	push ix
	call 00005h
	pop ix
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	jp cret

startup:
;	jp __getargs

__getargs:
	pop hl	
	exx	
	pop hl	
	pop hl	
	ld a,(00080h)
	inc a	
	neg
	ld l,a	
	ld h,0ffh
	add hl,sp	
	ld sp,hl	
	ld bc,00000h
	push bc	
	ld hl,00080h
	ld c,(hl)	
	ld b,000h
	add hl,bc	
	ld b,c	
	ex de,hl	
	ld hl,(00006h)
	ld c,001h
	dec hl	
	ld (hl),000h
	inc b	
	jr l862ah
l8617h:
	ld a,(de)	
	cp 020h
	dec de	
	jr nz,l8628h
	push hl	
	inc c	
l861fh:
	ld a,(de)	
	cp 020h
	jr nz,l8627h
	dec de	
	jr l861fh
l8627h:
	xor a	
l8628h:
	dec hl	
	ld (hl),a	
l862ah:
	djnz l8617h
	ld (__argc_),bc
	ld hl,nularg
	push hl	
	ld hl,00000h
	add hl,sp	
	exx	
	push de	
	push de	
	push hl	
	exx	
	ret
	
_unlink:
	call ncsv
	defw 0ffd3h
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	push ix
	pop de	
	ld hl,0ffd6h
	add hl,de	
	push hl	
	call _def_fcb
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr z,l8661h
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l8661h:
	call _getuc
	ld (ix-02bh),l
	ld l,(ix-001h)
	ld h,000h
	push hl	
	call _setuc
	push ix
	pop de	
	ld hl,0ffd6h
	add hl,de	
	ex (sp),hl	
	ld hl,00013h
	push hl	
	call _bdos
	pop bc	
	ld a,l	
	rla	
	sbc a,a	
	ld h,a	
	ld (ix-02dh),l
	ld (ix-02ch),h
	ld l,(ix-02bh)
	ld h,000h
	ex (sp),hl	
	call _setuc
	pop bc	
	ld l,(ix-02dh)
	ld h,(ix-02ch)
	jp cret

_getuc:
	call csv
	ld c,020h
	ld e,0ffh
	push ix
	call 00005h
	pop ix
	ld l,a	
	ld h,000h
	jp cret

_setuc:
	call csv
	ld e,(ix+006h)
	ld c,020h
	push ix
	call 00005h
	pop ix
	jp cret

asaladd:
aslladd:
	call iregset
	call aladd
	jp l8745h
asll:
asal:
	ld e,(hl)	
	inc hl	
	ld d,(hl)	
	push hl	
	ex de,hl	
	call shll
	ex de,hl	
	pop hl	
	ld (hl),d	
	dec hl	
	ld (hl),e	
	ex de,hl	
	ret
	
_digit:
	sub 030h
	ret c	
	cp 00ah
	ccf	
	ret
	
_atoi:
	pop bc	
	pop de	
	push de	
	push bc	
	ld hl,00000h
l86e9h:
	ld a,(de)	
	inc de	
	cp 020h
	jr z,l86e9h
	cp 009h
	jr z,l86e9h
	dec de	
	cp 02dh
	jr z,l86fdh
	cp 02bh
	jr nz,l86feh
	or a	
l86fdh:
	inc de	
l86feh:
	ex af,af'	
l86ffh:
	ld a,(de)	
	inc de	
	call _digit
	jr c,l8712h
	add hl,hl	
	ld c,l	
	ld b,h	
	add hl,hl	
	add hl,hl	
	add hl,bc	
	ld c,a	
	ld b,000h
	add hl,bc	
	jr l86ffh
l8712h:
	ex af,af'	
	ret nz	
	ex de,hl	
	ld hl,00000h
	sbc hl,de
	ret

_blkclr:
	pop de	
	pop hl	
	pop bc	
	push bc	
	push hl	
	push de	
	ld e,000h
l8723h:
	ld a,c	
	or b	
	ret z	
	ld (hl),e	
	inc hl	
	dec bc	
	jr l8723h

iregset:
	ld e,(hl)	
	inc hl	
	ld d,(hl)	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ex (sp),hl	
	push bc	
	ex (sp),hl	
	pop bc	
	exx	
	pop hl	
	pop bc	
	pop de	
	ex (sp),hl	
	push bc	
	ex (sp),hl	
	pop bc	
	ex (sp),hl	
	push hl	
	push bc	
	push de	
	exx	
	push bc	
	ret	

l8745h:
	ex (sp),hl	
	pop bc	
	ld (hl),b	
	dec hl	
	ld (hl),c	
	dec hl	
	ld (hl),d	
	dec hl	
	ld (hl),e	
	push bc	
	pop hl	
	ret
	
aladd:
lladd:
	exx	
	pop hl	
	exx	
	pop bc	
	ex de,hl	
	add hl,bc	
	ex de,hl	
	pop bc	
	adc hl,bc
	exx	
	push hl	
	exx	
	ret
	
brelop:
	push de	
	ld e,a	
	xor b	
	jp m,l8769h
	ld a,e	
	sbc a,b	
	pop de	
	ret
l8769h:
	ld a,e	
	and 080h
	ld d,a	
	ld a,e	
	sbc a,b	
	ld a,d	
	inc a	
	pop de	
	ret
	
wrelop:
	ld a,h	
	xor d	
	jp m,l877bh
	sbc hl,de
	ret	
l877bh:
	ld a,h	
	and 080h
	sbc hl,de
	inc a	
	ret	
	call adiv
	ex de,hl	
	ret
	
lmod:
	call ldiv
	ex de,hl	
	ret
	
ldiv:
	xor a	
	ex af,af'	
	ex de,hl	
	jr l879ch
adiv:
	ld a,h	
	xor d	
	ld a,h	
	ex af,af'	
	call negif
	ex de,hl	
	call negif
l879ch:
	ld b,001h
	ld a,h	
	or l	
	ret z	
l87a1h:
	push hl	
	add hl,hl	
	jr c,l87b3h
	ld a,d	
	cp h	
	jr c,l87b3h
	jr nz,l87afh
	ld a,e	
	cp l	
	jr c,l87b3h
l87afh:
	pop af	
	inc b	
	jr l87a1h
l87b3h:
	pop hl	
	ex de,hl	
	push hl	
	ld hl,00000h
	ex (sp),hl	
l87bah:
	ld a,h	
	cp d	
	jr c,l87c6h
	jr nz,l87c4h
	ld a,l	
	cp e	
	jr c,l87c6h
l87c4h:
	sbc hl,de
l87c6h:
	ex (sp),hl	
	ccf	
	adc hl,hl
	srl d
	rr e
	ex (sp),hl	
	djnz l87bah
	pop de	
	ex de,hl	
	ex af,af'	
	call m,negat
	ex de,hl	
	or a	
	call m,negat
	ex de,hl	
	ret
	
negif:
	bit 7,h
	ret z	
negat:
	ld b,h	
	ld c,l	
	ld hl,00000h
	or a	
	sbc hl,bc
	ret
	
lregset:
	pop bc	
	exx	
	pop bc	
	pop de	
	exx	
	ex de,hl	
	ex (sp),hl	
	ex de,hl	
	exx	
	push bc	
	pop hl	
	ex (sp),hl	
	exx	
	push bc	
	ret
	
iregsetd:
	pop de	
	call lregset
	push hl	
	ex (sp),iy
	ld h,(iy+003h)
	ld l,(iy+002h)
	exx	
	push hl	
	ld h,(iy+001h)
	ld l,(iy+000h)
	exx	
	ret	

sgndiv:
	call negif1
	exx	
	ex de,hl	
	exx	
	ex de,hl	
	call negif1
	ex de,hl	
	exx	
	ex de,hl	
	exx	
	jp divide
;				unused !!!
;asaldiv:
;	call iregsetd
;	call dosdiv
l8827h:
	ld (iy+000h),e
	ld (iy+001h),d
	ld (iy+002h),l
	ld (iy+003h),h
	pop iy
	ret	
	call lregset
dosdiv:
	ld a,h	
	xor d	
	ex af,af'	
	call sgndiv
	ex af,af'	
	push bc	
	exx	
	pop hl	
	ld e,c	
	ld d,b	
	jp m,l888ah
	ret	

lldiv:
	call lregset
doudiv:
	call divide
	push bc	
	exx	
	pop hl	
	ld e,c	
	ld d,b	
	ret
;				unused !!!	
;aslldiv:
;	call iregsetd
;	call doudiv
;	jr l8827h
;	call lregset
dosrem:
	ld a,h	
	ex af,af'	
	call sgndiv
	push hl	
	exx	
	pop de	
	ex de,hl	
	ex af,af'	
	or a	
	jp m,l888ah
	ret
;				unused !!!
;asalmod:	
;	call iregsetd
;	call dosrem
;	jr l8827h
llmod:
	call lregset
dourem:
	call divide
	push hl	
	exx	
	pop de	
	ex de,hl	
	ret	
	call iregsetd
	call dourem
	jr l8827h
l888ah:
	push hl	
	ld hl,00000h
	or a	
	sbc hl,de
	ex de,hl	
	pop bc	
	ld hl,00000h
	sbc hl,bc
	ret
	
negif1:
	bit 7,h
	ret z	
	exx	
	ld c,l	
	ld b,h	
	ld hl,00000h
	or a	
	sbc hl,bc
	exx	
	ld c,l	
	ld b,h	
	ld hl,00000h
	sbc hl,bc
	ret	

divide:
	ld bc,00000h
	ld a,e	
	or d	
	exx	
	ld bc,00000h
	or e	
	or d	
	exx	
	ret z	
	ld a,001h
	jr l88d8h
l88bfh:
	push hl	
	exx	
	push hl	
	or a	
	sbc hl,de
	exx	
	sbc hl,de
	exx	
	pop hl	
	exx	
	pop hl	
	jr c,l88dch
	exx	
	inc a	
	ex de,hl	
	add hl,hl	
	ex de,hl	
	exx	
	ex de,hl	
	adc hl,hl
	ex de,hl	
l88d8h:
	bit 7,d
	jr z,l88bfh
l88dch:
	push hl	
	exx	
	push hl	
	or a	
	sbc hl,de
	exx	
	sbc hl,de
	exx	
	jr nc,l88eeh
	pop hl	
	exx	
	pop hl	
	exx	
	jr l88f2h
l88eeh:
	inc sp	
	inc sp	
	inc sp	
	inc sp	
l88f2h:
	ccf	
	rl c
	rl b
	exx	
	rl c
	rl b
	srl d
	rr e
	exx	
	rr d
	rr e
	exx	
	dec a	
	jr nz,l88dch
	ret
	
asar:
	ld e,(hl)	
	inc hl	
	ld d,(hl)	
	push hl	
	ex de,hl	
	call shar
	ex de,hl	
	pop hl	
	ld (hl),d	
	dec hl	
	ld (hl),e	
	ex de,hl	
	ret
	
amul:
lmul:
	ld a,e	
	ld c,d	
	ex de,hl	
	ld hl,00000h
	ld b,008h
	call mult8b1
	ex de,hl	
	jr l8928h
l8927h:
	add hl,hl	
l8928h:
	djnz l8927h
	ex de,hl	
	ld a,c	
mult8b1:
	srl a
	jr nc,l8931h
	add hl,de	
l8931h:
	ex de,hl	
	add hl,hl	
	ex de,hl	
	ret z	
	djnz mult8b1
	ret
	
llmul:
almul:
	ex de,hl	
	ex (sp),hl	
	exx	
	pop de	
	pop bc	
	exx	
	pop bc	
	push hl	
	ld hl,00000h
	exx	
	ld hl,00000h
	ld a,c	
	ld c,b	
	call mult8b
	ld a,c	
	call mult8b
	exx	
	ld a,c	
	exx	
	call mult8b
	exx	
	ld a,b	
	exx	
	call mult8b
	push hl	
	exx	
	pop de	
	ret
	
mult8b:
	ld b,008h
l8962h:
	srl a
	jr nc,l896bh
	add hl,de	
	exx	
	adc hl,de
	exx	
l896bh:
	ex de,hl	
	add hl,hl	
	ex de,hl	
	exx	
	ex de,hl	
	adc hl,hl
	ex de,hl	
	exx	
	djnz l8962h
	ret	

_rindex:
	call rcsv
	ld bc,00000h
	jr l8981h
l897fh:
	inc hl	
	inc bc	
l8981h:
	ld a,(hl)	
	or a	
	jr nz,l897fh
l8985h:
	dec hl	
	ld a,c	
	or b	
	jr z,l8992h
	dec bc	
	ld a,(hl)	
	cp e	
	jr nz,l8985h
l898fh:
	jp cret
l8992h:
	ld hl,00000h
	jr l898fh
;				unused !!!
;_brk:
;	pop hl	
;	pop de	
;	ld (memtop),de
;	push de	
;	jp (hl)
	
_sbrk:
	pop bc	
	pop de	
	push de	
	push bc	
	ld hl,(memtop)
	ld a,l	
	or h	
	jr nz,l89b0h
	ld hl,__Hbss
	ld (memtop),hl
l89b0h:
	add hl,de	
	jr c,l89bdh
	ld bc,00400h
	add hl,bc	
	jr c,l89bdh
	sbc hl,sp
	jr c,l89c1h
l89bdh:
	ld hl,0ffffh
	ret	
l89c1h:
	ld hl,(memtop)
	push hl	
	add hl,de	
	ld (memtop),hl
	pop hl	
	ret	
	ld hl,(memtop)
	ld bc,00080h
	add hl,bc	
	sbc hl,sp
	ld hl,00001h
	ret c	
	dec hl	
	ret	

shll:
shal:
	ld a,b	
	or a	
	ret z	
	cp 010h
	jr c,l89e3h
	ld b,010h
l89e3h:
	add hl,hl	
	djnz l89e3h
	ret	

shar:
	ld a,b	
	or a	
	ret z	
	cp 010h
	jr c,l89f0h
	ld b,010h
l89f0h:
	srl h
	rr l
	djnz l89f0h
	ret	

_strcat:
	pop bc	
	pop de	
	pop hl	
	push hl	
	push de	
	push bc	
	ld c,e	
	ld b,d	
l89ffh:
	ld a,(de)	
	or a	
	jr z,l8a06h
	inc de	
	jr l89ffh
l8a06h:
	ld a,(hl)	
	ld (de),a	
	or a	
	jr z,l8a0fh
	inc de	
	inc hl	
	jr l8a06h
l8a0fh:
	ld l,c	
	ld h,b	
	ret	

_strcmp:
	pop bc	
	pop de	
	pop hl	
	push hl	
	push de	
	push bc	
l8a18h:
	ld a,(de)	
	cp (hl)	
	jr nz,l8a25h
	inc de	
	inc hl	
	or a	
	jr nz,l8a18h
	ld hl,00000h
	ret	
l8a25h:
	ld hl,00001h
	ret nc	
	dec hl	
	dec hl	
	ret	

_strcpy:
	pop bc	
	pop de	
	pop hl	
	push hl	
	push de	
	push bc	
	ld c,e	
	ld b,d	
l8a34h:
	ld a,(hl)	
	ld (de),a	
	inc de	
	inc hl	
	or a	
	jr nz,l8a34h
	ld l,c	
	ld h,b	
	ret	

_strlen:
	pop hl	
	pop de	
	push de	
	push hl	
	ld hl,00000h
l8a45h:
	ld a,(de)	
	or a	
	ret z	
	inc hl	
	inc de	
	jr l8a45h

_strncpy:
	call rcsv
l8a4fh:
	ld a,c	
	or b	
	jr z,l8a5dh
	dec bc	
	ld a,(de)	
	cp (hl)	
	jr nz,l8a63h
	inc de	
	inc hl	
	or a	
	jr nz,l8a4fh
l8a5dh:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l8a63h:
	ld hl,00001h
	jp c,cret
	dec hl	
	dec hl	
	jp cret

csv:
	pop hl	
	push iy
	push ix
	ld ix,00000h
	add ix,sp
indir:	jp (hl)	
;
popBCpopBCcret:
	pop     bc
;
popBCcret:
	pop     bc
	jr      cret
;
popHLcret:
	pop     hl
	jr      cret
;
ldHL0cret:
	ld      hl,0
	jr      cret
;
ldHL1cret:
	ld      hl,1
	jr      cret
;
ldHLFFFFcret:
	ld      hl,0FFFFh
	jr      cret
;
cret:
	ld sp,ix
	pop ix
	pop iy
	ret
;
ldL0cret:
	ld      l,0
	jr      cret
;
ldL1cret:
	ld      l,1
	jr      cret
;
ldLIXm2ldHIXm1cret:
	ld      h,(ix-1)
;
ldLIXm2cret:
	ld      l,(ix-2)
	jr      cret
;
pushIYpopHLcret:
	push    iy
	pop     hl
	jr      cret
;
ncsv:
	pop hl	
	push iy
	push ix
	ld ix,00000h
	add ix,sp
	ld e,(hl)	
	inc hl	
	ld d,(hl)	
	inc hl	
	ex de,hl	
	add hl,sp	
	ld sp,hl	
	ex de,hl	
	jp (hl)	

rcsv:
	ex (sp),iy
	push ix
	ld ix,00000h
	add ix,sp
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld c,(ix+00ah)
	ld b,(ix+00bh)
	jp (iy)

IF SHORTMSG
;
;	void _pr_nr_of_error0(char errno)
;
_pr_nr_of_error0:
	call	csv
	ld	l,(ix+06h)
	ld	h,0
	push	hl
	ld	hl,msgerr0
	push	hl
	call	_prerror
	jp	popBCpopBCcret
;
;	void _pr_nr_of_error1(char errno, char* )
;
_pr_nr_of_error1:
	call	csv
	ld	l,(ix+8)
	ld	h,(ix+9)
	push	hl
	ld	l,(ix+06h)
	ld	h,0
	push	hl
	ld	hl,msgerr1
	push	hl
	call	_prerror
	pop	bc
	jp	popBCpopBCcret
;
;	void _pr_nr_of_warning0(char errno)
;
_pr_nr_of_warning0:
	call	csv
	ld	l,(ix+06h)
	ld	h,0
	push	hl
	ld	hl,msgwarning0
	push	hl
	call	_prwarning
	jp	popBCpopBCcret
;
;	void _pr_nr_of_warning1(char errno, char* )
;
_pr_nr_of_warning1:
	call	csv
	ld	l,(ix+8)
	ld	h,(ix+9)
	push	hl
	ld	l,(ix+06h)
	ld	h,0
	push	hl
	ld	hl,msgwarning1
	push	hl
	call	_prwarning
	pop	bc
	jp	popBCpopBCcret
;
ENDIF
