include p1.inc

	psect text

	global  _callocm2
	global  sub_2f75h
	global  sub_308bh
	global  _callocm3
	global  sub_320dh
	global  sub_3429h
	global  sub_347ah
	global  sub_3495h
	global  sub_35f7h
	global  sub_3610h
	global  sub_363fh
	global  sub_3666h
	global  _main
	global  _prerror
	global  _fatalerror
	global  _prwarning
	global  _errexpected
	global  _allocmem
	global  sub_3c7eh
	global  sub_4126h
	global l305ah
	global l3075h
	global l3065h
	global l3070h
IF SHORTMSG
	global _pr_nr_of_error0
	global _pr_nr_of_error1
	global _pr_nr_of_warning0
	global _pr_nr_of_warning1
ENDIF

l0200h equ      200h

_callocm2:
	call ncsv
	defw 0ffc8h
	push ix
	pop de	
	ld hl,0ffc9h
	add hl,de	
	push hl	
	pop iy
	jr l2ce5h
l2cd4h:
	ld a,(ix+006h)
	ld (iy+000h),a
	inc iy
	call sub_320dh
	ld (ix+006h),l
	ld (ix+007h),h
l2ce5h:
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,l8ab6h
	add hl,de	
	bit 2,(hl)
	jr nz,l2cd4h
IF OPTIM
	ld      a,(ix+006h)
	cp      2eh
ELSE
	ld de,0002eh
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr z,l2d1eh
IF OPTIM
	cp      65h
ELSE
	ld de,00065h
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr z,l2d1eh
IF OPTIM
	cp      45h
ELSE
	ld de,00045h
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jp nz,l2e2ch
l2d1eh:
IF OPTIM
	ld      a,(ix+006h)
	cp      2eh
ELSE
	ld de,0002eh
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l2d46h
l2d2ch:
	ld a,(ix+006h)
	ld (iy+000h),a
	inc iy
	call sub_320dh
	ex de,hl	
	ld (ix+006h),e
	ld (ix+007h),d
	ld hl,l8ab6h
	add hl,de	
	bit 2,(hl)
	jr nz,l2d2ch
l2d46h:
IF OPTIM
	ld      a,(ix+006h)
	cp      65h
ELSE
	ld de,00065h
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr z,l2d63h
IF OPTIM
	cp      45h
ELSE
	ld de,00045h
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jp nz,l2dcbh
l2d63h:
	ld (iy+000h),065h
	inc iy
	call sub_320dh
	ld (ix+006h),l
	ld (ix+007h),h
IF OPTIM
	ld      a,l
	cp      2bh
ELSE
	ld de,0002bh
	or a	
	sbc hl,de
ENDIF
	jr z,l2d88h
IF OPTIM
	cp      2dh
ELSE
	ld de,0002dh
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l2d99h
l2d88h:
	ld a,(ix+006h)
	ld (iy+000h),a
	inc iy
	call sub_320dh
	ld (ix+006h),l
	ld (ix+007h),h
l2d99h:
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,l8ab6h
	add hl,de	
	bit 2,(hl)
	jr z,l2dc3h
l2da7h:
	ld a,(ix+006h)
	ld (iy+000h),a
	inc iy
	call sub_320dh
	ex de,hl	
	ld (ix+006h),e
	ld (ix+007h),d
	ld hl,l8ab6h
	add hl,de	
	bit 2,(hl)
	jr nz,l2da7h
	jr l2dcbh
l2dc3h:
IF SHORTMSG
	ld	l,52
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l912ah
	push hl	
	call _prerror
ENDIF
	pop bc	
l2dcbh:
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (buf16),hl
	ld (iy+000h),000h
	inc iy
	ld a,(ix-037h)
	cp 02eh
	jr nz,l2de3h
	inc iy
l2de3h:
	push ix
	pop de	
	ld hl,0ffc9h
	add hl,de	
	ex de,hl	
	push iy
	pop hl	
	or a	
	sbc hl,de
	push hl	
	call _allocmem
	pop bc	
	ld (buf8),hl
	ld a,(ix-037h)
	cp 02eh
	push ix
	pop de	
	ld hl,0ffc9h
	jr nz,l2e1ch
	add hl,de	
	push hl	
	ld hl,l913ch
	push hl	
	ld hl,(buf8)
	push hl	
	call _strcpy
	pop bc	
	ex (sp),hl	
	call _strcat
	pop bc	
	pop bc	
	jr l2e27h
l2e1ch:
	add hl,de	
	push hl	
	ld hl,(buf8)
	push hl	
	call _strcpy
	pop bc	
	pop bc	
l2e27h:
	ld l,07fh
	jp cret
l2e2ch:
	ld (ix-005h),00ah
IF OPTIM
	ld      a,(ix+006h)
	cp      78h
ELSE
	ld de,00078h
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr z,l2e54h
IF OPTIM
	cp      58h
ELSE
	ld de,00058h
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	ld a,(ix-037h)
	jp nz,l2e9ah
	cp 030h
	jr nz,l2e97h
l2e54h:
	ld (ix-005h),010h
	push ix
	pop de	
	ld hl,0ffc9h
	add hl,de	
	push hl	
	pop iy
	jr l2e6ch
l2e64h:
	ld a,(ix+006h)
	ld (iy+000h),a
	inc iy
l2e6ch:
	call sub_320dh
	ex de,hl	
	ld (ix+006h),e
	ld (ix+007h),d
	ld hl,l8ab6h
	add hl,de	
	bit 3,(hl)
	jr nz,l2e64h
	push ix
	pop de	
	ld hl,0ffc9h
	add hl,de	
	push iy
	pop de	
	or a	
	sbc hl,de
	jr nz,l2ea2h
IF SHORTMSG
	ld	l,51
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l913eh
	push hl	
	call _prerror
ENDIF
	pop bc	
	jr l2ea2h
l2e97h:
	ld a,(ix-037h)
l2e9ah:
	cp 030h
	jr nz,l2ea2h
	ld (ix-005h),008h
l2ea2h:
	ld (ix-004h),000h
	ld (ix-003h),000h
	ld (ix-002h),000h
	ld (ix-001h),000h
	ld (iy+000h),000h
	push ix
	pop de	
	ld hl,0ffc9h
	add hl,de	
	push hl	
	pop iy
	jp l2f2dh
l2ec3h:
	ld b,041h
	ld a,(iy+000h)
	call brelop
	ld a,(iy+000h)
	jp m,l2edch
	or 020h
	add a,0a9h
	ld (ix-038h),a
	inc iy
	jr l2ee3h
l2edch:
	add a,0d0h
	ld (ix-038h),a
	inc iy
l2ee3h:
	ld e,(ix-005h)
	ld d,000h
	ld l,(ix-038h)
	ld h,d	
	call wrelop
	jr c,l2efbh
IF SHORTMSG
	ld	l,50
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l9151h
	push hl	
	call _prerror
ENDIF
	pop bc	
	jr l2f33h
l2efbh:
	ld a,(ix-038h)
	ld hl,00000h
	ld d,l	
	ld e,a	
	push hl	
	push de	
	ld e,(ix-004h)
	ld d,(ix-003h)
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	push de	
	ld a,(ix-005h)
	ld hl,00000h
	ld d,l	
	ld e,a	
	call llmul
	call aladd
	ld (ix-004h),e
	ld (ix-003h),d
	ld (ix-002h),l
	ld (ix-001h),h
l2f2dh:
	ld a,(iy+000h)
	or a	
	jr nz,l2ec3h
l2f33h:
	ld e,(ix-004h)
	ld d,(ix-003h)
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf8),de
	ld (buf9),hl
IF OPTIM
	ld      a,(ix+006h)
	cp      6ch
ELSE
	ld de,0006ch
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr z,l2f62h
IF OPTIM
	cp      4ch
ELSE
	ld de,0004ch
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l2f67h
l2f62h:
	ld l,075h
	jp cret
l2f67h:
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (buf16),hl
	ld l,074h
	jp cret

sub_2f75h:
	call ncsv
	defw 0fff9h
	ld iy,buf47
	ld (ix-002h),000h
	ld (ix-001h),000h
l2f86h:
IF OPTIM
	ld      a,(ix-002h)
	cp      1fh
ELSE
	ld de,0001fh
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr z,l2fa9h
	ld a,(ix+006h)
	ld (iy+000h),a
	inc iy
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
l2fa9h:
	call sub_320dh
	ld (ix+006h),l
	ld a,l	
	ld e,a	
	rla	
	sbc a,a	
	ld d,a	
	ld hl,l8ab6h
	add hl,de	
	ld a,(hl)	
	and 007h
	or a	
	jr nz,l2f86h
	ld a,e	
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	ld (buf16),hl
	ld (iy+000h),000h
	ld (ix-004h),013h
	ld (ix-005h),031h
l2fd2h:
	ld e,(ix-005h)
	ld d,000h
	ld l,(ix-004h)
	ld h,d	
	add hl,de	
	srl h
	rr l
	ld (ix-003h),l
	ld e,l	
	ld hl,0ffedh
	add hl,de	
	add hl,hl	
	ld de,l8f87h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	ld hl,buf47
	push hl	
	call _strcmp
	pop bc	
	pop bc	
	ex de,hl	
	ld (ix-007h),e
	ld (ix-006h),d
	ld hl,00000h
	call wrelop
	jp m,l3012h
	ld a,(ix-003h)
	add a,0ffh
	ld (ix-005h),a
l3012h:
	bit 7,(ix-006h)
	jr nz,l301fh
	ld a,(ix-003h)
	inc a	
	ld (ix-004h),a
l301fh:
	ld b,(ix-004h)
	ld a,(ix-005h)
	call brelop
	jr nc,l2fd2h
	ld e,(ix-004h)
	ld d,000h
	ld hl,0ffffh
	add hl,de	
	ex de,hl	
	ld l,(ix-005h)
	call wrelop
	jp nc,l307bh
	ld a,(ix-003h)
	add a,0ech
	ld l,a	
	ld h,000h
	ld a,h	
	cp h	
	jr c,l3075h
	jr nz,l3050h
	ld a,01ch
	cp l	
	jr c,l3075h
l3050h:
	add hl,hl	
	ld de,l8fc5h
	add hl,de	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	jp (hl)	
l305ah:	ld a,(ix-003h)
	ld (buf8),a
	ld l,005h
	jp cret
l3065h:	ld a,(ix-003h)
	ld (buf8),a
	ld l,077h
	jp cret
l3070h:	ld l,04eh
	jp cret
l3075h:
	ld l,(ix-003h)
	jp cret
l307bh:
	ld hl,buf47
	push hl	
	call sub_4e90h
	pop bc	
	ld (buf8),hl
	ld l,073h
	jp cret

sub_308bh:
	call ncsv
	defw 0fdfeh
loop:
	push ix
	pop de	
	ld hl,0fdfeh
	add hl,de	
	push hl	
	pop iy
	jr l30a4h
l309ch:
	ld a,(ix-002h)
	ld (iy+000h),a
	inc iy
l30a4h:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      0ah
ELSE
	ld de,0000ah
	or a	
	sbc hl,de
ENDIF
	jr z,l30c3h
IF OPTIM
	ld	a,(ix-002h)
	and	(ix-001h)
	inc	a
ELSE
	ld de,0ffffh
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l309ch
l30c3h:
	ld (iy+000h),000h
IF OPTIM
	ld	a,(ix-002h)
	and	(ix-001h)
	inc	a
ELSE
	ld de,0ffffh
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l30ddh
	ld hl,l9164h
	push hl	
	call _fatalerror
	pop bc	
l30ddh:
	ld hl,00007h
	push hl	
	ld hl,l9170h
	push hl	
	push ix
	pop de	
	ld hl,0fdfeh
	add hl,de	
	push hl	
	call _strncpy
	pop bc	
	pop bc	
	pop bc	
	ld a,l	
	or h	
	jp z,cret
	push ix
	pop de	
	ld hl,0fdfeh
	add hl,de	
	push hl	
	ld hl,l9178h
	push hl	
	call _printf
	pop bc	
	pop bc	
	jr loop

_callocm3:
	call ncsv
	defw 0fbfch
	push ix
	pop de	
	ld hl,0fbfch
	add hl,de	
	push hl	
	pop iy
	jp l3187h
l311dh:
IF OPTIM
	ld      a,(ix+006h)
	cp      5ch
ELSE
	ld de,0005ch
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l314fh
	call sub_320dh
	ld (ix+006h),l
	ld (ix+007h),h
IF OPTIM
	ld      a,l
	cp      0ah
ELSE
	ld de,0000ah
	or a	
	sbc hl,de
ENDIF
	jr z,l3157h
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_3495h
	pop bc	
	ld e,l	
	ld (iy+000h),e
	inc iy
	jr l3157h
l314fh:
	ld a,(ix+006h)
	ld (iy+000h),a
	inc iy
l3157h:
	call sub_320dh
	ld (ix+006h),l
	ld (ix+007h),h
IF OPTIM
	ld      a,l
	cp      22h
ELSE
	ld de,00022h
	or a	
	sbc hl,de
ENDIF
	jr z,l317eh
IF OPTIM
	cp      0ah
ELSE
	ld de,0000ah
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l311dh
	ld hl,l917fh
	push hl	
	call _errexpected
	pop bc	
l317eh:
	call sub_347ah
	ld (ix+006h),l
	ld (ix+007h),h
l3187h:
IF OPTIM
	ld      a,(ix+006h)
	cp      22h
ELSE
	ld de,00022h
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr z,l3157h
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (buf16),hl
	ld (iy+000h),000h
	push ix
	pop de	
	ld hl,0fbfch
	add hl,de	
	ex de,hl	
	push iy
	pop hl	
	or a	
	sbc hl,de
	ld (buf10),hl
	inc hl	
	push hl	
	call _allocmem
	pop bc	
	ld (ix-004h),l
	ld (ix-003h),h
	ld (ix-002h),l
	ld (ix-001h),h
	ld hl,(buf10)
	inc hl	
	ld (ix+006h),l
	ld (ix+007h),h
	push ix
	pop de	
	ld hl,0fbfch
	add hl,de	
	push hl	
	pop iy
	jr l31efh
l31dbh:
	ld a,(iy+000h)
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
	dec hl	
	ld (hl),a	
	inc iy
l31efh:
	ld l,(ix+006h)
	ld h,(ix+007h)
	dec hl	
	ld (ix+006h),l
	ld (ix+007h),h
	inc hl	
	ld a,l	
	or h	
	jr nz,l31dbh
	ld l,(ix-004h)
	ld h,(ix-003h)
	ld (buf8),hl
	jp cret

sub_320dh:
	call csv
	push hl	
	ld hl,(buf16)
	ld a,l	
	or h	
	jr z,l322dh
	ld (ix-002h),l
	ld (ix-001h),h
	ld hl,00000h
	ld (buf16),hl
l3224h:
IF OPTIM
	jp      ldLIXm2ldHIXm1cret
ELSE
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp cret
ENDIF
l322dh:
	ld de,(buf12)
	ld hl,buf21
	add hl,de	
	ld a,(hl)	
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	ld (ix-002h),l
	ld (ix-001h),h
	ld a,l	
	or h	
	jr z,l324ch
	ld l,e	
	ld h,d	
	inc hl	
	ld (buf12),hl
	jr l3224h
l324ch:
	ld a,(buf3)
	or a	
	call nz,sub_3429h
	xor a	
	ld (buf17),a
	ld (buf18),a
	ld hl,__iob
	push hl
IF OPTIM	
	ld hl,128
ELSE
	ld hl,200h
ENDIF
	push hl	
	ld hl,buf21
	push hl	
	call _fgets
	pop bc	
	pop bc	
	pop bc	
	ld a,l	
	or h	
	jr nz,l3276h
IF OPTIM
	jp      ldHLFFFFcret
ELSE
	ld hl,0ffffh
	jp cret
ENDIF
l3276h:
	ld hl,00001h
	ld (buf12),hl
	ld hl,00000h
	ld (buf13),hl
	ld hl,(buf1)
	inc hl	
	ld (buf1),hl
	ld a,(buf20)
	or a	
	call nz,sub_329ah
	ld a,(buf21)
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	jp cret

sub_329ah:
	call csv
	ld a,(buf18)
	or a	
	jp nz,cret
	ld a,(buf22)
	or a	
	jr nz,l32afh
	ld hl,l918dh
	jr l32bbh
l32afh:
	ld de,(buf11)
	ld hl,00017h
	add hl,de	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
l32bbh:
	push hl	
	pop iy
	ld a,(buf20)
	or a	
	jp nz,l3335h
	ld hl,buf45
	push hl	
	ld hl,buf2
	push hl	
	call _strcmp
	pop bc	
	pop bc	
	ld a,l	
	or h	
	jr nz,l32e6h
	ld hl,buf48
	push hl	
	push iy
	call _strcmp
	pop bc	
	pop bc	
	ld a,l	
	or h	
	jp z,l3335h
l32e6h:
	ld hl,buf2
	push hl	
	ld hl,l918eh
	push hl	
	ld hl,l9bc9h
	push hl	
	call _fprintf
	pop bc	
	pop bc	
	pop bc	
	ld a,(iy+000h)
	or a	
	jr z,l3310h
	push iy
	ld hl,l9192h
	push hl	
	ld hl,l9bc9h
	push hl	
	call _fprintf
	pop bc	
	pop bc	
	pop bc	
	jr l331dh
l3310h:
	ld hl,l9bc9h
	push hl	
	ld hl,0000ah
	push hl	
	call _fputc
	pop bc	
	pop bc	
l331dh:
	ld hl,buf2
	push hl	
	ld hl,buf45
	push hl	
	call _strcpy
	pop bc	
	pop bc	
	push iy
	ld hl,buf48
	push hl	
	call _strcpy
	pop bc	
	pop bc	
l3335h:
	ld hl,buf21
	push hl	
	ld hl,(buf1)
	push hl	
	ld hl,l9199h
	push hl	
	ld hl,l9bc9h
	push hl	
	call _fprintf
	ld a,001h
	ld (buf18),a
	jp cret

sub_3350h:
	call csv
	push hl	
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	call sub_329ah
	ld a,(buf21)
	or a	
	ld hl,l9bc9h
	push hl	
	jr nz,l3375h
	push iy
	call _fputs
l3370h:
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF
l3375h:
	ld hl,00009h
	push hl	
	call _fputc
	pop bc	
	pop bc	
	ld hl,00000h
	ld (ix-002h),l
	ld (ix-001h),h
	ld (ix-004h),l
	ld (ix-003h),h
	jp l33d2h
l3390h:
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,buf21
	add hl,de	
	ld a,(hl)	
	cp 009h
	jr nz,l33b8h
	ld e,(ix-004h)
	ld d,(ix-003h)
	ld hl,00008h
	add hl,de	
	ld a,l	
	and 0f8h
	ld l,a	
	ld a,h	
	and a	
	ld h,a	
	ld (ix-004h),l
	ld (ix-003h),h
	jr l33c5h
l33b8h:
	ld l,(ix-004h)
	ld h,(ix-003h)
	inc hl	
	ld (ix-004h),l
	ld (ix-003h),h
l33c5h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
l33d2h:
	ld de,(buf13)
	dec de	
	ld l,(ix-002h)
	ld h,(ix-001h)
	call wrelop
	jp m,l3390h
	push iy
	call _strlen
	pop bc	
	inc hl	
	ld e,(ix-004h)
	ld d,(ix-003h)
	call wrelop
	push iy
	jr nc,l340fh
	ld l,(ix-004h)
	ld h,(ix-003h)
	dec hl	
	push hl	
	ld hl,l91a1h
	push hl	
	ld hl,l9bc9h
	push hl	
	call _fprintf
	pop bc	
	pop bc	
	jp l3370h
l340fh:
	ld hl,0005eh
	push hl	
	ld l,(ix-004h)
	ld h,(ix-003h)
	inc hl	
	push hl	
	ld hl,l91a8h
	push hl	
	ld hl,l9bc9h
	push hl	
	call _fprintf
	jp cret

sub_3429h:
	call csv
	ld a,(buf17)
	or a	
	jp nz,l3472h
	ld a,(buf21)
	or a	
	jp z,l3472h
	ld iy,buf21
	jr l3442h
l3440h:
	inc iy
l3442h:
	ld a,(iy+000h)
	or a	
	jr z,l3455h
	ld e,a	
	rla	
	sbc a,a	
	ld d,a	
	ld hl,l8ab6h
	add hl,de	
	bit 4,(hl)
	jr nz,l3440h
	ld a,e	
l3455h:
	or a	
	jr z,l3472h
	ld a,(iy+000h)
	cp 023h
	jr z,l3472h
	ld hl,buf21
	push hl	
	ld hl,(buf1)
	push hl	
	ld hl,buf2
	push hl	
	ld hl,l91afh
	push hl	
	call _printf
l3472h:
	ld a,001h
	ld (buf17),a
	jp cret

sub_347ah:
	call csv
	push hl	
l347eh:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
	ex de,hl	
	ld hl,l8ab6h
	add hl,de	
	bit 4,(hl)
	jr nz,l347eh
	ld l,e	
	ld h,d	
	jp cret

sub_3495h:
	call csv
	push hl	
	push hl	
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,l8ab6h
	add hl,de	
	bit 2,(hl)
	jp z,l351ch
	ld hl,0ffd0h
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	call sub_320dh
	ld (ix+006h),l
	ld (ix+007h),h
	ex de,hl	
	ld hl,l8ab6h
	add hl,de	
	bit 2,(hl)
	jp z,l350dh
	ld l,(ix-002h)
	ld h,(ix-001h)
	add hl,hl	
	add hl,hl	
	add hl,hl	
	add hl,de	
	ld de,0ffd0h
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	call sub_320dh
	ld (ix+006h),l
	ld (ix+007h),h
	ex de,hl	
	ld hl,l8ab6h
	add hl,de	
	bit 2,(hl)
	jr z,l3502h
	ld l,(ix-002h)
	ld h,(ix-001h)
	add hl,hl	
	add hl,hl	
	add hl,hl	
	add hl,de	
	ld de,0ffd0h
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	jr l3516h
l3502h:
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (buf16),hl
	jr l3516h
l350dh:
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (buf16),hl
l3516h:
IF OPTIM
	jp      ldLIXm2cret
ELSE
	ld l,(ix-002h)
	jp cret
ENDIF
l351ch:
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld a,h	
	or a	
	jp nz,l354fh
	ld a,l	
	cp 061h
	jp z,l356eh
	cp 062h
	jp z,l355ah
	cp 066h
	jp z,l3569h
	cp 06eh
	jr z,l3555h
	cp 072h
	jp z,l355fh
	cp 074h
	jp z,l3564h
	cp 076h
	jp z,l3573h
	cp 078h
	jp z,l3578h
l354fh:
	ld l,(ix+006h)
	jp cret
l3555h:
	ld l,00ah
	jp cret
l355ah:
	ld l,008h
	jp cret
l355fh:
	ld l,00dh
	jp cret
l3564h:
	ld l,009h
	jp cret
l3569h:
	ld l,00ch
	jp cret
l356eh:
	ld l,007h
	jp cret
l3573h:
	ld l,00bh
	jp cret
l3578h:
	ld (ix-002h),000h
	ld (ix-001h),000h
	ld (ix-003h),003h
l3584h:
	call sub_320dh
	ld (ix+006h),l
	ld (ix+007h),h
	ex de,hl	
	ld hl,l8ab6h
	add hl,de	
	bit 3,(hl)
	jp z,l3502h
	ld b,004h
	push ix
	pop hl	
	dec hl	
	dec hl	
	call asll
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,l8ab6h
	add hl,de	
	bit 0,(hl)
	jr z,l35b5h
	ld l,e	
	set 5,l
	ld (ix+006h),l
l35b5h:
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,l8ab6h
	add hl,de	
	bit 1,(hl)
	jr z,l35d7h
	ld hl,0ffa9h
	add hl,de	
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	jr l35e9h
l35d7h:
	ld hl,0ffd0h
	add hl,de	
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
l35e9h:
	ld a,(ix-003h)
	add a,0ffh
	ld (ix-003h),a
	or a	
	jr nz,l3584h
	jp l3516h

sub_35f7h:
	call csv
	push hl	
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
	ld (buf16),hl
IF OPTIM
	jp      ldLIXm2ldHIXm1cret
ELSE
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp cret
ENDIF

sub_3610h:
	call csv
	jr l361bh
l3615h:
	call sub_2671h
	ld (ix+006h),l
l361bh:
	ld e,(ix+006h)
	ld d,000h
	ld hl,00003h
	call wrelop
	jr c,l3615h
	ld a,(ix+006h)
	cp 001h
	jr nz,l3636h
	ld hl,l91beh
	push hl	
	call _fatalerror
l3636h:
	ld a,(ix+006h)
	ld (buf7),a
	jp cret

sub_363fh:
	call csv
	push hl	
	call sub_2671h
	ld a,l	
	ld (ix-001h),a
	cp (ix+006h)
	jp z,cret
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	call _errexpected
	ld l,(ix-001h)
	ld h,000h
	ex (sp),hl	
	call sub_3610h
	jp cret

sub_3666h:
	call csv
	push hl	
l366ah:
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 002h
	jr nz,l366ah
	ld a,002h
	ld (buf7),a
	jp cret

_main:
	jp	topcode
	global	retcode

retcode:
	call sub_4d92h
	call sub_07e3h
	ld a,(ix+006h)
	or (ix+007h)
	jp z,l3829h
	ld hl,__iob
	push hl	
	ld hl,l91f4h
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	call _freopen
	pop bc	
	pop bc	
	pop bc	
	ld a,l	
	or h	
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	jr nz,l37b9h
	push bc	
	ld hl,l91ddh
	push hl	
	call _fatalerror
	pop bc	
	pop bc	
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
l37b9h:
	ld (buf14),bc
	push bc	
	ld hl,buf2
	push hl	
	call _strcpy
	pop bc	
	pop bc
IF OPTIM
	ld      a,(ix+006h)
	cp      01h
ELSE	
	ld de,00001h
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr z,l3808h
	ld hl,l9bc1h
	push hl	
	ld hl,l91f6h
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	call _freopen
	pop bc	
	pop bc	
	pop bc	
	ld a,l	
	or h	
	jr nz,l3808h
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	ld hl,l91ddh
	push hl	
	call _fatalerror
	pop bc	
	pop bc	
l3808h:
IF OPTIM
	ld      a,(ix+006h)
	cp      03h
ELSE
	ld de,00003h
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l3839h
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	inc hl	
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (l91dbh),bc
	jr l3839h
l3829h:
	ld hl,l91f8h
	ld (buf14),hl
	push hl	
	ld hl,buf2
	push hl	
	call _strcpy
	pop bc	
	pop bc	
l3839h:
	ld hl,(buf24)
	ld a,l	
	or h	
	jp z,l38bah
	ld a,(hl)	
	or a	
	jr nz,l3885h
	ld hl,buf50
	ld (buf24),hl
	ld hl,buf2
	push hl	
	ld hl,buf50
	push hl	
	call _strcpy
	pop bc	
	ld hl,0002eh
	ex (sp),hl	
	ld hl,buf50
	push hl	
	call _rindex
	pop bc	
	pop bc	
	push hl	
	pop iy
	ld a,l	
	or h	
	jr z,l3878h
	ld hl,l9200h
	push hl	
	push iy
	call _strcpy
	pop bc	
	pop bc	
	jr l3885h
l3878h:
	ld hl,l9205h
	push hl	
	ld hl,buf50
	push hl	
	call _strcat
	pop bc	
	pop bc	
l3885h:
	ld hl,l920ah
	push hl	
	ld hl,(buf24)
	push hl	
	call _fopen
	pop bc	
	pop bc	
	ld (buf15),hl
	ld a,l	
	or h	
	jr nz,l38a8h
	ld hl,(buf24)
	push hl	
IF SHORTMSG
	ld	l,83
	push	hl
	call	_pr_nr_of_warning1
ELSE
	ld hl,l920ch
	push hl	
	call _prwarning
ENDIF
	pop bc	
	pop bc	
	jr l38bah
l38a8h:
	ld hl,buf2
	push hl	
	ld hl,l9226h
	push hl	
	ld hl,(buf15)
	push hl	
	call _fprintf
	pop bc	
	pop bc	
	pop bc	
l38bah:
	ld hl,l922bh
	push hl	
	ld hl,(l91dbh)
	push hl	
	call _fopen
	pop bc	
	pop bc	
	ld (buf4),hl
	ld a,l	
	or h	
	jr nz,l38dbh
	ld hl,(l91dbh)
	push hl	
	ld hl,l91ddh
	push hl	
	call _fatalerror
	pop bc	
	pop bc	
l38dbh:
	ld a,074h
	ld (buf25),a
	ld (buf26),a
	ld a,008h
	ld hl,buf51
	ld (hl),a	
	ld hl,buf52
	ld (hl),a	
	ld de,00000h
	ld l,e	
	ld h,d	
	ld (buf27),de
	ld (buf28),hl
	ld de,00001h
	ld (buf29),de
	ld (buf30),hl
	call sub_3abfh
	call sub_3a07h
	ld hl,l9bc1h
	push hl	
	call _fclose
	pop bc
IF OPTIM
	ld	a,h
	and	l
	inc	a
ELSE	
	ld de,0ffffh
	or a	
	sbc hl,de
ENDIF
	jr nz,l3921h
IF SHORTMSG
	ld	l,49
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l922dh
	push hl	
	call _prerror
ENDIF
	pop bc	
l3921h:
	call sub_3a5eh
	ld hl,(buf31)
	ld a,l	
	or h	
	ld hl,00001h
	jr nz,l392fh
	dec hl	
l392fh:
	push hl	
	call _exit
	jp cret

sub_3936h:
	call ncsv
	defw 0ff80h
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	push ix
	pop de	
	ld hl,0ff80h
	add hl,de	
	push hl	
	call _sprintf
	pop bc	
	pop bc	
	pop bc	
	push ix
	pop de	
	ld hl,0ff80h
	add hl,de	
	ex (sp),hl	
	call sub_3350h
	jp cret

_prerror:
	call csv
	ld hl,(buf31)
	inc hl	
	ld (buf31),hl
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_3936h
	pop bc	
	pop bc	
	ld hl,l9bc9h
	ex (sp),hl	
	ld hl,0000ah
	push hl	
	call _fputc
	jp cret

_fatalerror:
	call csv
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call _prerror
	pop bc	
	pop bc	
	call sub_3a5eh
	ld hl,00001h
	push hl	
	call _exit
	jp cret

_prwarning:
	call csv
	ld a,(buf23)
	or a	
	jp nz,cret
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_3936h
	pop bc	
	pop bc	
	ld hl,l9247h
	ex (sp),hl	
	ld hl,l9bc9h
	push hl	
	call _fprintf
	jp cret

_errexpected:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
IF SHORTMSG
	ld	l,48
	push	hl
	call	_pr_nr_of_error1
ELSE
	ld hl,l9253h
	push hl	
	call _prerror
ENDIF
	jp cret

sub_3a07h:
	call csv
	push hl	
	ld hl,(buf4)
	push hl	
	call _fclose
	ld hl,l925fh
	ex (sp),hl	
	ld hl,(l91dbh)
	push hl	
	call _fopen
	pop bc	
	pop bc	
	ld (buf4),hl
	ld a,l	
	or h	
	jr nz,l3a45h
	ld hl,(l91dbh)
	push hl	
	ld hl,l9261h
	push hl	
	call _fatalerror
	pop bc	
	pop bc	
	jr l3a45h
l3a35h:
	ld hl,l9bc1h
	push hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	call _fputc
	pop bc	
	pop bc	
l3a45h:
	ld hl,(buf4)
	push hl	
	call _fgetc
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld	a,h
	and	l
	inc	a
ELSE
	ld de,0ffffh
	or a	
	sbc hl,de
ENDIF
	jr nz,l3a35h
	jp cret

sub_3a5eh:
	ld hl,__iob
	push hl	
	call _fclose
	ld hl,l9bc1h
	ex (sp),hl	
	call _fclose
	pop bc	
	ld hl,(buf4)
	ld a,l	
	or h	
	ret z	
	push hl	
	call _fclose
	ld hl,(l91dbh)
	ex (sp),hl	
	call _unlink
	pop bc	
	ret	

_allocmem:
	call csv
l3a83h:
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call _malloc
	pop bc	
	push hl	
	pop iy
	ld a,l	
	or h	
	jr nz,l3aabh
	call sub_5356h
	ld a,l	
	or a	
	jr nz,l3a83h
	call sub_2157h
	ld a,l	
	or a	
	jr nz,l3a83h
	ld hl,l91cdh
	push hl	
	call _fatalerror
	pop bc	
l3aabh:
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	push iy
	call _blkclr
	pop bc	
	pop bc	
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

sub_3abfh:
	call csv
	push hl	
	jr l3aceh
l3ac5h:
	ld a,(ix-001h)
	ld (buf7),a
	call sub_3adfh
l3aceh:
	call sub_2671h
	ld a,l	
	ld (ix-001h),a
	cp 001h
	jr nz,l3ac5h
	call sub_54c0h
	jp cret

sub_3adfh:
	call ncsv
	defw 0fff4h
	push ix
	pop de	
	ld hl,0fff6h
	add hl,de	
	push hl	
	push ix
	pop hl	
	dec hl	
	dec hl	
	push hl	
	call sub_5dd1h
	pop bc	
	pop bc	
	ld (ix-00ch),l
	call sub_2671h
	ld a,l	
	ld (ix-001h),a
	cp 002h
	jp z,cret
	ld (buf7),a
	ld (ix-00bh),001h
l3b0dh:
	ld hl,00000h
	ld (buf32),hl
	ld l,(ix-00ch)
	ld h,000h
	ld a,l	
	and 0feh
	ld l,a	
	ld a,h	
	push hl	
	push ix
	pop de	
	ld hl,0fff6h
	add hl,de	
	push hl	
	ld l,(ix-002h)
	ld h,a	
	push hl	
	call sub_69cah
	pop bc	
	pop bc	
	pop bc	
	push hl	
	pop iy
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	push iy
	pop hl	
	ld a,l	
	or h	
	jp z,l3bdfh
	bit 4,(iy+012h)
	jp z,l3bdfh
	ld a,(iy+007h)
	cp 002h
	jp nz,l3bdfh
	ld a,(ix-00bh)
	or a	
	jr z,l3baah
	ld a,e	
	cp 071h
	jr z,l3baah
	cp 002h
	jr z,l3baah
	ld a,(ix-002h)
	cp 02dh
	jr z,l3baah
	ld a,(iy+007h)
	cp 002h
	jr nz,l3b82h
	ld a,(iy+004h)
	or (iy+005h)
	jr nz,l3b82h
	ld a,(iy+006h)
	cp 014h
	jr z,l3ba5h
	cp 008h
	jr z,l3ba5h
l3b82h:
	ld hl,00000h
l3b85h:
	ld a,l	
	ld (buf33),a
	ld a,(ix-001h)
	ld (buf7),a
	push iy
	call sub_516ch
	pop bc	
	push iy
	call sub_0493h
	pop bc	
	ld (buf11),iy
	call sub_409bh
	jp cret
l3ba5h:
	ld hl,00001h
	jr l3b85h
l3baah:
	ld hl,(buf32)
	ld a,l	
	or h	
	jr z,l3bc5h
	ex de,hl	
	ld hl,00012h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 3,c
	jr nz,l3bc5h
	ld hl,l94a6h
	push hl	
	call _errexpected
	pop bc	
l3bc5h:
	ld a,(buf22)
	inc a	
	ld (buf22),a
	call sub_54c0h
	ld a,(buf22)
	sub 001h
	ld (buf22),a
l3bd7h:
	push iy
	call sub_0493h
	pop bc	
	jr l3c32h
l3bdfh:
	ld a,(ix-001h)
	cp 064h
	jr nz,l3c10h
	ld a,(ix-002h)
	cp 02dh
	jr nz,l3bf5h
IF SHORTMSG
	ld	l,47
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l94b4h
	push hl	
	call _prerror
ENDIF
	pop bc	
l3bf5h:
	push iy
	call sub_516ch
	pop bc	
	push iy
	call sub_0493h
	pop bc	
	push iy
	call sub_3c7eh
	pop bc	
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	jr l3c32h
l3c10h:
	push iy
	pop hl	
	ld a,l	
	or h	
	jr z,l3c32h
	bit 0,(ix-00ch)
	jr nz,l3c24h
	ld a,(iy+014h)
	cp 01fh
	jr z,l3c2ch
l3c24h:
	push iy
	call sub_516ch
	pop bc	
	jr l3bd7h
l3c2ch:
	push iy
	call sub_01ech
	pop bc	
l3c32h:
	ld a,(ix-001h)
	cp 073h
	jr z,l3c3dh
	cp 050h
	jr nz,l3c52h
l3c3dh:
	ld hl,l94cbh
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
l3c4bh:
	ld (ix-00bh),000h
	jp l3b0dh
l3c52h:
	ld a,(ix-001h)
	cp 071h
	jr z,l3c4bh
	cp 002h
	jp z,cret
	ld hl,l94cdh
	push hl	
	call _errexpected
	pop bc	
	jr l3c6eh
l3c68h:
	call sub_2671h
	ld (ix-001h),l
l3c6eh:
	ld e,(ix-001h)
	ld d,000h
	ld hl,00003h
	call wrelop
	jr c,l3c68h
	jp cret

sub_3c7eh:
	call csv
	push hl	
	ld a,(ix+006h)
	or (ix+007h)
	jp z,l3d1eh
	ld hl,l94cfh
	push hl	
	call _printf
	ld hl,l9bc1h
	ex (sp),hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_573bh
	pop bc	
	ld hl,0000ah
	ex (sp),hl	
	call __putchar
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld hl,00001h
	ex (sp),hl	
	push iy
	call sub_3d24h
	pop bc	
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	bit 7,h
	jr z,l3cd3h
IF SHORTMSG
	ld	l,46
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l94d3h
	push hl	
	call _prerror
ENDIF
	pop bc	
	call sub_3666h
	jr l3d13h
l3cd3h:
	ld a,(iy+007h)
	cp 001h
	jr nz,l3d13h
	ld a,(iy+002h)
	or (iy+003h)
	jr z,l3d13h
	ld l,(iy+002h)
	ld h,(iy+003h)
	push hl	
	call sub_2105h
	pop bc	
	ld a,l	
	or a	
	jr z,l3d13h
	ld l,(iy+002h)
	ld h,(iy+003h)
	push hl	
	call sub_2569h
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld a,d	
	rla	
	sbc a,a	
	ld l,a	
	ld h,a	
	ex (sp),hl	
	push de	
	call sub_248ah
	pop bc	
	pop bc	
	ld (iy+002h),l
	ld (iy+003h),h
l3d13h:
	ld hl,l94e9h
	push hl	
	call _printf
IF OPTIM
	jp      popBCcret
ELSE
	pop bc	
	jp cret
ENDIF
l3d1eh:
	call sub_3666h
	jp cret

sub_3d24h:
	call ncsv
	defw 0fff2h
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld (ix-002h),0ffh
	ld (ix-001h),0ffh
	ld a,(ix+008h)
	or a	
	jp z,l3ec6h
	ld a,(iy+007h)
	cp 001h
	jp nz,l3ec6h
	ld a,(iy+002h)
	or (iy+003h)
	jp z,l3ec6h
	ld hl,l949bh
	push hl	
	call _printf
	pop bc	
	call sub_2671h
	ld a,l	
	ld (ix-003h),a
	cp 004h
	ld hl,00001h
	jr z,l3d69h
	dec hl	
l3d69h:
	ld a,l	
	ld (ix-006h),a
	or a	
	jr z,l3d77h
	call sub_2671h
	ld e,l	
	ld (ix-003h),e
l3d77h:
	ld a,(ix-003h)
	cp 076h
	jp nz,l3e4ah
	ld a,(iy+004h)
	or (iy+005h)
	jp nz,l3e4ah
	ld de,00004h
	ld l,(iy+006h)
	ld h,000h
	ld a,l	
	and 0feh
	ld l,a	
	ld a,h	
	or a	
	sbc hl,de
	jp nz,l3e4ah
	ld (ix-002h),a
	ld (ix-001h),a
	ld hl,(buf8)
	ld (ix-005h),l
	ld (ix-004h),h
	jr l3dd5h
l3dach:
	ld l,(ix-005h)
	ld h,(ix-004h)
	ld a,(hl)	
	inc hl	
	ld (ix-005h),l
	ld (ix-004h),h
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	push hl	
	ld hl,l9491h
	push hl	
	call _printf
	pop bc	
	pop bc	
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
l3dd5h:
	ld de,(buf10)
	ld l,(ix-002h)
	ld h,(ix-001h)
	call wrelop
	jp m,l3dach
	ld hl,(buf8)
	push hl	
	call _free
	ld l,(iy+002h)
	ld h,(iy+003h)
	ex (sp),hl	
	call sub_2105h
	pop bc	
	ld a,l	
	or a	
	jr z,l3e15h
	ld hl,00000h
	push hl	
	ld hl,l9491h
	push hl	
	call _printf
	pop bc	
	pop bc	
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
l3e15h:
	ld a,(ix-006h)
	or a	
	jr z,l3e22h
	call sub_2671h
	ld e,l	
	ld (ix-003h),e
l3e22h:
	ld a,(ix-006h)
	or a	
	jr z,l3e3fh
	ld a,(ix-003h)
	cp 003h
	jr z,l3e3fh
	ld hl,l94eeh
	push hl	
	call _errexpected
	pop bc	
	ld (ix-002h),0ffh
	ld (ix-001h),0ffh
l3e3fh:
	ld hl,l94a2h
	push hl	
	call _printf
	pop bc	
	jp l4092h
l3e4ah:
	ld a,(ix-006h)
	or a	
	jr nz,l3e5ah
	ld hl,l94ech
	push hl	
	call _errexpected
	pop bc	
	jr l3e22h
l3e5ah:
	ld a,(ix-003h)
	ld (buf7),a
	ld a,(iy+004h)
	or (iy+005h)
	jr nz,l3e7ah
	ld a,(iy+006h)
	cp 016h
	jr nz,l3e7ah
	ld l,(iy+000h)
	ld h,(iy+001h)
	push hl	
	pop iy
	jr l3e7eh
l3e7ah:
	ld (ix+008h),000h
l3e7eh:
	ld (ix-002h),000h
	ld (ix-001h),000h
l3e86h:
	ld l,(ix+008h)
	ld h,000h
	push hl	
	push iy
	call sub_3d24h
	pop bc	
	pop bc	
	bit 7,h
	jr nz,l3e22h
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
	call sub_2671h
	ld a,l	
	ld (ix-003h),a
	cp 003h
	jp z,l3e22h
	cp 071h
	jp nz,l3e22h
	call sub_2671h
	ld a,l	
	ld (ix-003h),a
	cp 003h
	jp z,l3e22h
	ld (buf7),a
	jr l3e86h
l3ec6h:
	ld a,(ix+008h)
	or a	
	jr z,l3ed4h
	ld a,(iy+007h)
	cp 001h
	jp z,l3fdfh
l3ed4h:
	ld a,(iy+004h)
	or (iy+005h)
	jp nz,l3fdfh
	ld a,(iy+006h)
	cp 01ah
	ld a,(ix+008h)
	jp nz,l3fe2h
	or a	
	ld hl,l949bh
	push hl	
	jr z,l3ef6h
	call _printf
	ld hl,l949bh
	ex (sp),hl	
l3ef6h:
	call _printf
	pop bc	
	ld l,(iy+000h)
	ld h,(iy+001h)
	ld (ix-008h),l
	ld (ix-007h),h
	ld a,l	
	or h	
	jp z,l3fcdh
	call sub_2671h
	ld a,l	
	ld (ix-003h),a
	cp 004h
	ld hl,00001h
	jr z,l3f1ah
	dec hl	
l3f1ah:
	ld a,l	
	ld (ix-00bh),a
	or a	
	jr nz,l3f27h
	ld a,(ix-003h)
	ld (buf7),a
l3f27h:
	ld e,(ix-008h)
	ld d,(ix-007h)
	ld hl,0000ah
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (ix-00ah),c
	ld (ix-009h),b
l3f3ah:
	ld hl,00001h
	push hl	
	ld l,(ix-00ah)
	ld h,(ix-009h)
	push hl	
	call sub_3d24h
	pop bc	
	pop bc	
	bit 7,h
	jr nz,l3f7ch
	ld e,(ix-00ah)
	ld d,(ix-009h)
	ld hl,0000ah
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (ix-00ah),c
	ld (ix-009h),b
	ld e,(ix-008h)
	ld d,(ix-007h)
	ld l,c	
	ld h,b	
	or a	
	sbc hl,de
	jr z,l3f7ch
	call sub_2671h
	ld a,l	
	ld (ix-003h),a
	cp 071h
	jr z,l3fa5h
	ld (buf7),a
l3f7ch:
	ld a,(ix-00bh)
	or a	
	jr z,l3fc5h
	call sub_2671h
	ld a,l	
	ld (ix-003h),a
	cp 071h
	jr nz,l3f94h
	call sub_2671h
	ld e,l	
	ld (ix-003h),e
l3f94h:
	ld a,(ix-003h)
	cp 003h
	jr z,l3fbbh
	ld hl,l94f0h
	push hl	
	call _errexpected
	pop bc	
	jr l3fcdh
l3fa5h:
	ld a,(ix-00bh)
	or a	
	jr z,l3f3ah
	call sub_2671h
	ld a,l	
	ld (ix-003h),a
	ld (buf7),a
	cp 003h
	jr nz,l3f3ah
	jr l3f7ch
l3fbbh:
	ld (ix-002h),001h
	ld (ix-001h),000h
	jr l3fcdh
l3fc5h:
	ld (ix-002h),001h
	ld (ix-001h),000h
l3fcdh:
	ld hl,l94a2h
	push hl	
	call _printf
	pop bc	
	ld a,(ix+008h)
	or a	
	jp z,l4092h
	jp l3e3fh
l3fdfh:
	ld a,(ix+008h)
l3fe2h:
	or a	
	ld a,(iy+007h)
	jr z,l3fedh
	cp 001h
	jp z,l408ah
l3fedh:
	cp 002h
	jp z,l408ah
	bit 0,(iy+004h)
	jr nz,l4003h
	ld b,014h
	ld a,(iy+006h)
	call brelop
	jp nc,l408ah
l4003h:
	call sub_2671h
	ld a,l	
	ld (ix-003h),a
	cp 004h
	ld hl,00001h
	jr z,l4012h
	dec hl	
l4012h:
	ld a,l	
	ld (ix-00eh),a
	or a	
	jr nz,l401fh
	ld a,(ix-003h)
	ld (buf7),a
l401fh:
	push iy
	call sub_24b6h
	ld (ix-00dh),l
	ld (ix-00ch),h
	ex de,hl	
	ld hl,0000ch
	add hl,de	
	ld (hl),000h
	ld hl,00003h
	ex (sp),hl	
	call sub_0a83h
	ex (sp),hl	
	ld l,(ix-00dh)
	ld h,(ix-00ch)
	push hl	
	ld hl,0007dh
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	pop bc	
	ld (ix-00dh),l
	ld (ix-00ch),h
	ld a,l	
	or h	
	jr z,l405fh
	inc hl	
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
;	call sub_05b5h		duplicate
	call sub_05d3h
	pop bc	
l405fh:
	ld a,(ix-00eh)
	or a	
	jr z,l4075h
	call sub_2671h
	ld a,l	
	cp 003h
	jr z,l4075h
	ld hl,l94f2h
	push hl	
	call _errexpected
	pop bc	
l4075h:
	ld l,(ix-00dh)
	ld h,(ix-00ch)
	push hl	
	call sub_2569h
	pop bc	
	ld (ix-002h),001h
	ld (ix-001h),000h
	jr l4092h
l408ah:
IF SHORTMSG
	ld	l,45
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l94f4h
	push hl	
	call _prerror
ENDIF
	pop bc	
l4092h:
IF OPTIM
	jp      ldLIXm2ldHIXm1cret
ELSE
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp cret
ENDIF

sub_409bh:
	call csv
	push hl	
	call sub_549ch
	ld hl,00006h
	push hl	
	call sub_5c19h
	pop bc	
	call sub_51e7h
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 004h
	jr z,l40cah
	ld hl,l950bh
	push hl	
	call _errexpected
	ld l,(ix-001h)
	ld h,000h
	ex (sp),hl	
	call sub_3610h
	pop bc	
l40cah:
	ld hl,(buf11)
	push hl	
	call sub_0273h
	xor a	
	ld (buf34),a
	ld hl,00014h
	ex (sp),hl	
	call sub_5c19h
	pop bc	
	call sub_5785h
	ld (buf35),hl
	jr l40f9h
l40e5h:
	ld a,(ix-001h)
	ld (buf7),a
	ld hl,00000h
	push hl	
	push hl	
	push hl	
	push hl	
	call sub_4126h
	pop bc	
	pop bc	
	pop bc	
	pop bc	
l40f9h:
	call sub_2671h
	ld a,l	
	ld (ix-001h),a
	cp 003h
	jr nz,l40e5h
	ld a,(buf34)
	or a	
	jr nz,l4118h
	ld a,(buf33)
	or a	
	jr nz,l4118h
IF SHORTMSG
	ld	l,82
	push	hl
	call	_pr_nr_of_warning0
ELSE
	ld hl,l950dh
	push hl	
	call _prwarning
ENDIF
	pop bc	
l4118h:
	ld hl,(buf35)
	push hl	
	call sub_0258h
	pop bc	
	call sub_54ach
	jp cret

sub_4126h:
	call csv
	push hl	
	push hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	pop iy
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,(buf34)
	or a	
	ld a,e	
	jr z,l4170h
	cp 002h
	jr z,l416dh
	cp 004h
	jr z,l416dh
	cp 016h
	jr z,l4169h
	cp 019h
	jr z,l4169h
	cp 073h
	jr nz,l4161h
	call sub_35f7h
IF OPTIM
	ld      a,l
	cp      3ah
ELSE
	ld de,0003ah
	or a	
	sbc hl,de
ENDIF
	jr z,l4169h
l4161h:
IF SHORTMSG
	ld	l,81
	push	hl
	call	_pr_nr_of_warning0
ELSE
	ld hl,l9539h
	push hl	
	call _prwarning
ENDIF
	pop bc	
l4169h:
	xor a	
	ld (buf34),a
l416dh:
	ld a,(ix-001h)
l4170h:
	cp 002h
	jp z,cret
	cp 004h
	jp z,l41f5h
	cp 013h
	jp z,l4213h
	cp 015h
	jp z,l428eh
	cp 016h
	jp z,l4256h
	cp 018h
	jp z,l42afh
	cp 019h
	jp z,l4272h
	cp 01ah
	jp z,l421dh
	cp 01ch
	jp z,l42c3h
	cp 021h
	jp z,l4224h
	cp 022h
	jp z,l42bdh
	cp 023h
	jp z,l422bh
	cp 027h
	jp z,l42b7h
	cp 02ch
	jp z,l424ah
	cp 031h
	jr z,l4216h
	cp 073h
	jp z,l42cdh
l41bfh:
	ld a,(ix-001h)
	ld (buf7),a
	call sub_0bfch
	push hl	
	ld hl,0003ch
	push hl	
	call sub_1441h
	pop bc	
	ld (ix-003h),l
	ld (ix-002h),h
	ex (sp),hl	
	call sub_042dh
	ld l,(ix-003h)
	ld h,(ix-002h)
	ex (sp),hl	
	call sub_2569h
	ld hl,l955fh
	ex (sp),hl	
	ld hl,00002h
	push hl	
	call sub_363fh
l41f0h:
	pop bc	
l41f1h:
IF OPTIM
	jp      popBCcret
ELSE
	pop bc	
	jp cret
ENDIF
l41f5h:
	ld l,(ix+00ch)
	ld h,(ix+00dh)
	push hl	
	push iy
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_4300h
	pop bc	
	pop bc	
	jr l41f0h
l4213h:
	call sub_4390h
l4216h:
	push iy
	call sub_4406h
	jr l41f1h
l421dh:
	push iy
	call sub_44afh
	jr l41f1h
l4224h:
	push iy
	call sub_4838h
	jr l41f1h
l422bh:
	ld l,(ix+00ch)
	ld h,(ix+00dh)
	push hl	
	push iy
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_4595h
l4245h:
	pop bc	
	pop bc	
	pop bc	
	jr l41f1h
l424ah:
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_469bh
	jr l41f1h
l4256h:
	ld l,(ix+00ch)
	ld h,(ix+00dh)
	push hl	
	push iy
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_4a90h
	jr l4245h
l4272h:
	ld l,(ix+00ch)
	ld h,(ix+00dh)
	push hl	
	push iy
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_4a1eh
	jr l4245h
l428eh:
	ld a,(ix+00ch)
	or (ix+00dh)
	jr z,l42a2h
	ld de,00001h
	ld l,(ix+00ch)
	ld h,(ix+00dh)
	ld (hl),e	
	inc hl	
	ld (hl),d	
l42a2h:
	ld l,(ix+008h)
	ld h,(ix+009h)
l42a8h:
	push hl	
	call sub_49e1h
	jp l41f1h
l42afh:
	ld l,(ix+006h)
	ld h,(ix+007h)
	jr l42a8h
l42b7h:
	call sub_4baah
	jp cret
l42bdh:
	call sub_4c03h
	jp cret
l42c3h:
IF SHORTMSG
	ld	l,44
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l954ah
	push hl	
	call _prerror
ENDIF
	jp l41f1h
l42cdh:
	call sub_35f7h
IF OPTIM
	ld      a,l
	cp      3ah
ELSE
	ld de,0003ah
	or a	
	sbc hl,de
ENDIF
	jp nz,l41bfh
	call sub_2671h
	ld (ix-001h),l
	ld l,(ix+00ch)
	ld h,(ix+00dh)
	push hl	
	push iy
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	ld hl,(buf8)
	push hl	
	call sub_4c57h
	jp cret

sub_4300h:
	call csv
	push hl	
	call sub_2671h
	ld e,l	
	ld (ix-002h),e
	ld a,e	
	cp 005h
	jr z,l4323h
	cp 077h
	jr z,l4323h
	cp 073h
	jr nz,l4328h
	ld iy,(buf8)
	ld a,(iy+014h)
	cp 02dh
	jr nz,l4328h
l4323h:
	ld hl,00001h
	jr l432bh
l4328h:
	ld hl,00000h
l432bh:
	ld a,l	
	ld (ix-001h),a
	or a	
	ld a,(ix-002h)
	jr z,l437fh
	ld (buf7),a
	call sub_549ch
	ld hl,00014h
	push hl	
	call sub_5c19h
	pop bc	
	call sub_2671h
	ld e,l	
	ld (ix-002h),e
	jr l437ch
l434ch:
	ld a,(ix-002h)
	ld (buf7),a
	ld l,(ix+00ch)
	ld h,(ix+00dh)
	push hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_4126h
	pop bc	
	pop bc	
	pop bc	
	pop bc	
	call sub_2671h
	ld e,l	
	ld (ix-002h),e
l437ch:
	ld a,(ix-002h)
l437fh:
	cp 003h
	jr nz,l434ch
	ld a,(ix-001h)
	or a	
	jp z,cret
	call sub_54ach
	jp cret

sub_4390h:
	call csv
	push hl	
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 03eh
	jr z,l43aeh
	ld hl,l9561h
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
l43aeh:
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 076h
	jr z,l43cah
	ld hl,l9563h
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
	jr l43deh
l43cah:
	ld hl,(buf8)
	push hl	
	ld hl,l956ah
	push hl	
	call _printf
	pop bc	
	ld hl,(buf8)
	ex (sp),hl	
	call _free
	pop bc	
l43deh:
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 029h
	jr z,l43f8h
	ld hl,l9571h
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
l43f8h:
	ld hl,l9573h
	push hl	
	ld hl,00002h
	push hl	
	call sub_363fh
	jp cret

sub_4406h:
	call ncsv
	defw 0fff9h
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 03eh
	jr z,l4425h
	ld hl,l9575h
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
l4425h:
	call sub_5785h
	ld (ix-003h),l
	ld (ix-002h),h
	push hl	
	call sub_4ce8h
	pop bc	
	call sub_5785h
	ld (ix-007h),l
	ld (ix-006h),h
	push hl	
	call sub_0258h
	pop bc	
	call sub_0bfch
	push hl	
	pop iy
	call sub_2671h
	ld a,l	
	ld (ix-001h),a
	cp 03fh
	jr z,l4460h
	ld hl,l9577h
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
l4460h:
	ld hl,00000h
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_5785h
	ld (ix-005h),l
	ld (ix-004h),h
	push hl	
	ld l,(ix-003h)
	ld h,(ix-002h)
	push hl	
	call sub_4126h
	pop bc	
	pop bc	
	pop bc	
	ld l,(ix-003h)
	ld h,(ix-002h)
	ex (sp),hl	
	call sub_0258h
	ld hl,00001h
	ex (sp),hl	
	push iy
	ld l,(ix-007h)
	ld h,(ix-006h)
	push hl	
	call sub_4d15h
	pop bc	
	pop bc	
	ld l,(ix-005h)
	ld h,(ix-004h)
	ex (sp),hl	
	call sub_0258h
	xor a	
	ld (buf34),a
	jp cret

sub_44afh:
	call ncsv
	defw 0fff9h
	call sub_5785h
	ld (ix-003h),l
	ld (ix-002h),h
	call sub_5785h
	ld (ix-005h),l
	ld (ix-004h),h
	call sub_5785h
	ld (ix-007h),l
	ld (ix-006h),h
	push hl	
	call sub_0258h
	ld hl,00000h
	ex (sp),hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	ld l,(ix-005h)
	ld h,(ix-004h)
	push hl	
	ld l,(ix-003h)
	ld h,(ix-002h)
	push hl	
	call sub_4126h
	pop bc	
	pop bc	
	pop bc	
	ld l,(ix-003h)
	ld h,(ix-002h)
	ex (sp),hl	
	call sub_0258h
	pop bc	
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 031h
	jr z,l4511h
	ld hl,l9579h
	push hl	
	call _errexpected
	pop bc	
l4511h:
	ld a,(ix-001h)
	cp 031h
	jr z,l451dh
	cp 021h
	jp nz,l4583h
l451dh:
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
l4524h:
	ld a,(ix-001h)
	cp 03eh
	jr z,l4539h
	ld hl,l957fh
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
l4539h:
	call sub_0bfch
	push hl	
	pop iy
	ld hl,l9581h
	push hl	
	ld hl,0003fh
	push hl	
	call sub_363fh
	pop bc	
	ld hl,00001h
	ex (sp),hl	
	push iy
	ld l,(ix-007h)
	ld h,(ix-006h)
	push hl	
	call sub_4d15h
	pop bc	
	pop bc	
	ld l,(ix-005h)
	ld h,(ix-004h)
	ex (sp),hl	
	call sub_0258h
	pop bc	
	xor a	
	ld (buf34),a
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 002h
	jp z,cret
	ld hl,l9583h
	push hl	
	call _errexpected
	jp cret
l4583h:
	ld a,(ix-001h)
	cp 03eh
	jr z,l4524h
	ld l,a	
	ld h,000h
	push hl	
	call sub_3610h
IF OPTIM
	jp      popBCcret
ELSE
	pop bc	
	jp cret
ENDIF
