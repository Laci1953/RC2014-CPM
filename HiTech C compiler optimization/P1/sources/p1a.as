include p1.inc

	psect   text

	global  sub_01ech
	global  sub_022eh
	global  sub_0258h
	global  sub_0273h
	global  sub_02a6h
	global  sub_0353h
	global  sub_042dh
	global  sub_0493h
;	global  sub_05b5h	duplicate
	global  sub_05d3h
	global  sub_07e3h
	global  sub_07f5h
	global  sub_0a83h
	global  sub_0bfch
	global  sub_1340h
IF SHORTMSG
	global	_pr_nr_of_error0
	global	_pr_nr_of_error1
ENDIF

start:
	ld hl,(00006h)
	ld sp,hl	
	ld de,data_end
	or a	
	ld hl,__Hbss
	sbc hl,de
	ld c,l	
	ld b,h	
l010fh:
	dec bc	
	ld l,e	
	ld h,d	
	inc de	
	ld (hl),000h
	ldir
	ld hl,text_end
	push hl	
	ld hl,00080h
	ld c,(hl)	
	inc hl	
	ld b,000h
	add hl,bc	
	ld (hl),000h
	ld hl,00081h
	push hl	
	call startup
	pop bc	
	pop bc	
	push hl	
	ld hl,(__argc_)
	push hl	
	call _main
	push hl	
	call _exit
	jp 00000h

sub_013dh:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld de,(buf1)
	ld hl,(data_end)
	or a	
	sbc hl,de
	jr nz,l0166h
	ld hl,buf2
	push hl	
	ld hl,l9cb1h
	push hl	
	call _strcmp
	pop bc	
	pop bc	
	ld a,l	
	or h	
	jr z,l01a1h
l0166h:
	ld hl,(buf1)
	push hl	
	ld hl,l8b36h
	push hl	
	push iy
	call _fprintf
	pop bc	
	pop bc	
	ld hl,l9cb1h
	ex (sp),hl	
	ld hl,buf2
	push hl	
	call _strcmp
	pop bc	
	pop bc	
	ld a,l	
	or h	
	jr z,l0196h
	ld hl,buf2
	push hl	
	ld hl,l8b3ah
	push hl	
	push iy
	call _fprintf
	pop bc	
	pop bc	
	pop bc	
l0196h:
	push iy
	ld hl,0000ah
	push hl	
	call _fputc
	pop bc	
	pop bc	
l01a1h:
	ld hl,(buf1)
	ld (data_end),hl
	ld hl,buf2
	push hl	
	ld hl,l9cb1h
	push hl	
	call _strcpy
	pop bc	
	pop bc	
	ld a,(buf3)
	or a	
	jp z,cret
	call sub_3429h
	jp cret

sub_01c1h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+007h)
	cp 001h
	jr nz,l01e2h
	ld l,(iy+002h)
	ld h,(iy+003h)
	push hl	
	call sub_0470h
IF OPTIM
	jp      popBCcret
ELSE
	pop bc
	jp cret
ENDIF
l01e2h:
	ld hl,00031h
	push hl	
	call __putchar
	jp cret

sub_01ech:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+006h)
	cp 016h
	jp nz,cret
l0200h:
	ld a,(iy+004h)
	or (iy+005h)
	jp nz,cret
	ld e,(iy+000h)
	ld d,(iy+001h)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	cp 002h
	jp z,cret
	ld hl,00012h
	add hl,de	
	ld c,(hl)	
l021eh:
	inc hl	
	ld b,(hl)	
	bit 0,b
	jp nz,cret
	ld l,e	
	ld h,d	
	push hl	
	call sub_0493h
	jp cret

sub_022eh:
	call csv
	ld a,(ix+006h)
	cp 003h
	jr z,l024eh
	cp 004h
	jr nz,l0244h
	ld hl,0007bh
	push hl	
	call __putchar
	pop bc	
l0244h:
	ld hl,0000ah
	push hl	
	call __putchar
	jp cret
l024eh:
	ld hl,0007dh
	push hl	
	call __putchar
	pop bc	
	jr l0244h

sub_0258h:
	call csv
	ld hl,l9bc1h
	push hl	
	call sub_013dh
	ld l,(ix+006h)
	ld h,(ix+007h)
	ex (sp),hl	
	ld hl,l8b3eh
	push hl	
	call _printf
	jp cret

sub_0273h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jp z,cret
	ld hl,l9bc1h
	push hl	
	call sub_013dh
	ld hl,l8b4ah
	ex (sp),hl	
	call _printf
	ld hl,l9bc1h
	ex (sp),hl	
	push iy
	call sub_573bh
	pop bc	
	ld hl,l8b51h
	ex (sp),hl	
	call _printf
	jp cret

sub_02a6h:
	call csv
	push hl	
	ld a,(ix+006h)
	or (ix+007h)
	jp z,cret
	ld hl,l9bc1h
	push hl	
	call sub_013dh
	ld hl,l8b55h
	ex (sp),hl	
	call _printf
	pop bc	
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	call sub_0470h
	ld hl,0000ah
	ex (sp),hl	
	call __putchar
	pop bc	
	ld l,(ix+006h)
	ld h,(ix+007h)
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (ix-002h),c
	ld (ix-001h),b
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,00006h
	add hl,de	
	push hl	
	pop iy
	jr l0319h
l02f7h:
	call __putchar
	ld l,(iy+000h)
	ld h,(iy+001h)
	ex (sp),hl	
	call sub_0470h
	ld l,(iy+002h)
	ld h,(iy+003h)
	ex (sp),hl	
	ld hl,l8b59h
	push hl	
	call _printf
	pop bc	
	pop bc	
	ld de,00004h
	add iy,de
l0319h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	dec hl	
	ld (ix-002h),l
	ld (ix-001h),h
	inc hl	
	ld a,l	
	or h	
	ld hl,00009h
	push hl	
	jr nz,l02f7h
	call __putchar
	pop bc	
	ld l,(ix+006h)
	ld h,(ix+007h)
	inc hl	
	inc hl	
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	ld hl,l8b5eh
	push hl	
	call _printf
	pop bc	
	ld hl,l8b65h
	ex (sp),hl	
	call _printf
	jp cret

sub_0353h:
	call csv
	ld a,(ix+006h)
	or (ix+007h)
	jp z,cret
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,0000ah
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	pop iy
	jr l0380h
l0371h:
	push iy
	call sub_01ech
	pop bc	
	ld l,(iy+00ah)
	ld h,(iy+00bh)
	push hl	
	pop iy
l0380h:
	push iy
	pop de	
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
	jr nz,l0371h
	ld hl,l9bc1h
	push hl	
	call sub_013dh
	ld hl,0005bh
	ex (sp),hl	
	call __putchar
	pop bc	
	ld a,(ix+008h)
	cp 008h
	jr nz,l03aeh
	ld hl,00073h
	push hl	
	call __putchar
	pop bc	
	jr l03b6h
l03aeh:
	ld hl,00075h
	push hl	
	call __putchar
	pop bc	
l03b6h:
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	ld hl,l8b68h
	push hl	
	call _printf
	pop bc	
	pop bc	
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,0000ah
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	pop iy
	jr l0415h
l03dbh:
	bit 2,(iy+013h)
	jr z,l03f1h
	ld l,(iy+010h)
	ld h,(iy+011h)
	push hl	
	ld hl,l8b6dh
	push hl	
	call _printf
	pop bc	
	pop bc	
l03f1h:
	ld hl,00020h
	push hl	
	call __putchar
	pop bc	
	push iy
	call sub_7454h
	ld hl,00020h
	ex (sp),hl	
	call __putchar
	pop bc	
	push iy
	call sub_01c1h
	pop bc	
	ld l,(iy+00ah)
	ld h,(iy+00bh)
	push hl	
	pop iy
l0415h:
	push iy
	pop de	
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
	jr nz,l03dbh
	ld hl,l8b72h
	push hl	
	call _printf
	jp cret

sub_042dh:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld hl,l9bc1h
	push hl	
	call sub_013dh
	ld hl,l8b76h
	ex (sp),hl	
	call _printf
	pop bc	
	jr l0453h
l044ah:
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	pop iy
l0453h:
	push iy
	pop hl	
	ld a,l	
	or h	
	jr z,l0461h
	ld a,(iy+000h)
	cp 07ch
	jr z,l044ah
l0461h:
	push iy
	call sub_0470h
	ld hl,l8b7ah
	ex (sp),hl	
	call _printf
	jp cret

sub_0470h:
	call csv
	ld a,(ix+006h)
	or (ix+007h)
	jr z,l0489h
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_05f1h
IF OPTIM
	jp      popBCcret
ELSE
	pop bc
	jp cret
ENDIF
l0489h:
	ld hl,00031h
	push hl	
	call __putchar
	jp cret

sub_0493h:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jp z,cret
	ld hl,l9bc1h
	push hl	
	call sub_013dh
	pop bc	
	push iy
	call sub_01ech
	set 0,(iy+013h)
	ld hl,l8b7eh
	ex (sp),hl	
	call _printf
	ld hl,l9bc1h
	ex (sp),hl	
	push iy
	call sub_573bh
	pop bc	
	ld hl,00020h
	ex (sp),hl	
	call __putchar
	pop bc	
	push iy
	call sub_7454h
	ld hl,00020h
	ex (sp),hl	
	call __putchar
	pop bc	
	bit 0,(iy+012h)
	jr z,l04eah
	push iy
	call sub_01c1h
	pop bc	
	jr l04f2h
l04eah:
	ld hl,00030h
	push hl	
	call __putchar
	pop bc	
l04f2h:
	ld a,(iy+014h)
	cp 006h
	jr nz,l050ch
	bit 1,(iy+013h)
	jr nz,l0504h
	ld hl,00070h
	jr l0507h
l0504h:
	ld hl,00072h
l0507h:
	ld (ix-001h),l
	jr l0522h
l050ch:
	ld e,(iy+014h)
	ld d,000h
	ld hl,0ffedh
	add hl,de	
	add hl,hl	
	ld de,l8f87h
	add hl,de	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	ld l,(hl)	
	ld (ix-001h),l
l0522h:
	bit 2,(iy+012h)
	ld a,(ix-001h)
	jr z,l0530h
	add a,0e0h
	ld (ix-001h),a
l0530h:
	ld l,a	
	rla	
	sbc a,a	
	ld h,a	
	push hl	
	ld hl,l8b82h
	push hl	
	call _printf
	jp cret

sub_053fh:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld hl,(buf4)
	push hl	
	call sub_013dh
	ld l,(iy+001h)
	ld h,(iy+002h)
	ex (sp),hl	
	ld hl,l8b89h
	push hl	
	ld hl,(buf4)
	push hl	
	call _fprintf
	pop bc	
	pop bc	
	pop bc	
	ld l,(iy+003h)
	ld h,(iy+004h)
	ld (ix-002h),l
	ld (ix-001h),h
l0574h:
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
	ld hl,l8b8fh
	push hl	
	ld hl,(buf4)
	push hl	
	call _fprintf
	pop bc	
	pop bc	
	pop bc	
	ld l,(ix-002h)
	ld h,(ix-001h)
	dec hl	
	ld (ix-002h),l
	ld (ix-001h),h
	inc hl	
	ld a,l	
	or h	
	jr nz,l0574h
	ld hl,l8b93h
	push hl	
	ld hl,(buf4)
	push hl	
	call _fprintf
	jp cret
;				!!! duplicate code (see below)
;sub_05b5h:
;	call csv
;	ld hl,l9bc1h
;	push hl	
;	call sub_013dh
;	ld l,(ix+006h)
;	ld h,(ix+007h)
;	ex (sp),hl	
;	call sub_0470h
;	ld hl,0000ah
;	ex (sp),hl	
;	call __putchar
;	jp cret
;
sub_05d3h:
	call csv
	ld hl,l9bc1h
	push hl	
	call sub_013dh
	ld l,(ix+006h)
	ld h,(ix+007h)
	ex (sp),hl	
	call sub_0470h
	ld hl,0000ah
	ex (sp),hl	
	call __putchar
	jp cret

sub_05f1h:
	call ncsv
	defw 0fffah
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld e,(iy+000h)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9271h
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	inc hl	
	inc hl	
	inc hl	
	inc hl	
	bit 0,(hl)
	ld a,(iy+000h)
	jp z,l0712h
	cp 073h
	jr z,l0641h
	cp 074h
	jp z,l06a5h
	cp 076h
	jp z,l06fah
	cp 077h
	jp z,l06ddh
	cp 07eh
	jp z,l0706h
	cp 07fh
	jp z,l06eah
	jp cret
l0641h:
	ld l,(iy+001h)
	ld h,(iy+002h)
	ld (ix-004h),l
	ld (ix-003h),h
	ld e,(ix-004h)
	ld d,(ix-003h)
	ld hl,00014h
	add hl,de	
	ld a,(hl)	
	cp 00ch
	jr nz,l0695h
	ld hl,l8b97h
	push hl	
	call _printf
	pop bc	
	ld hl,l9bc1h
	push hl	
	ld e,(ix-004h)
	ld d,(ix-003h)
	ld hl,0000ah
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	call sub_573bh
	pop bc	
	pop bc	
	ld e,(ix-004h)
	ld d,(ix-003h)
	ld hl,0000eh
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	ld hl,l8b9bh
	push hl	
	call _printf
IF OPTIM
	jp      popBCpopBCcret
ELSE
l0690h:
	pop bc
l0691h:
	pop bc
	jp cret
ENDIF
l0695h:
	ld hl,l9bc1h
	push hl	
	ld l,(ix-004h)
	ld h,(ix-003h)
	push hl	
	call sub_573bh
IF OPTIM
	jp      popBCpopBCcret
ELSE
	jr l0690h
ENDIF
l06a5h:
	ld hl,00014h
	push hl	
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	ld e,(iy+001h)
	ld d,(iy+002h)
	ld l,(iy+003h)
	ld h,(iy+004h)
	push hl	
	push de	
	jr z,l06d3h
	ld hl,l8b9fh
	push hl	
	call _printf
	pop bc	
l06d0h:
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc
	jr l0691h
ENDIF
l06d3h:
	ld hl,l8ba3h
	push hl	
	call _printf
	pop bc	
	pop bc	
	pop bc	
l06ddh:
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_7454h
IF OPTIM
	jp      popBCcret
ELSE
	jr l0691h
ENDIF
l06eah:
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	ld hl,l8babh
l06f4h:
	push hl	
	call _printf
IF OPTIM
	jp      popBCpopBCcret
ELSE
	jr l06d0h
ENDIF
l06fah:
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	ld hl,l8bafh
	jr l06f4h
l0706h:
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	ld hl,l8bb5h
	jr l06f4h
l0712h:
	cp 04eh
	jr nz,l071eh
	ld hl,l8bb8h
	push hl	
	call _printf
	pop bc	
l071eh:
	ld a,(iy+000h)
	cp 04eh
	jr nz,l0781h
	ld l,(iy+001h)
	ld h,(iy+002h)
	ld (ix-006h),l
	ld (ix-005h),h
	ld a,(hl)	
	cp 073h
	jr nz,l0781h
	ld l,(ix-006h)
	ld h,(ix-005h)
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld hl,00012h
	add hl,bc	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 0,c
	jr nz,l0781h
	ld hl,l8bbch
	push hl	
	call _printf
	ld e,(ix-006h)
	ld d,(ix-005h)
	ld hl,00005h
	add hl,de	
	ex (sp),hl	
	call sub_7454h
	ld hl,00020h
	ex (sp),hl	
	call __putchar
	pop bc	
	ld l,(ix-006h)
	ld h,(ix-005h)
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	call sub_01c1h
	ld hl,00020h
	ex (sp),hl	
	call __putchar
	pop bc	
	jp l06ddh
l0781h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	ld hl,l8bc1h
	push hl	
	call _printf
	pop bc	
	pop bc	
	ld a,(iy+000h)
	cp 078h
	jp z,cret
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	call sub_05f1h
	pop bc	
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	inc hl	
	inc hl	
	inc hl	
	bit 1,(hl)
	jr z,l07c6h
	ld hl,00020h
	push hl	
	call __putchar
	ld l,(iy+003h)
	ld h,(iy+004h)
	ex (sp),hl	
	call sub_05f1h
IF OPTIM
	jp      popBCcret
ELSE
	jp l0691h
ENDIF
l07c6h:
	ld a,(iy+000h)
	cp 04eh
	jp nz,cret
	ld hl,00020h
	push hl	
	call __putchar
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	ex (sp),hl	
	call sub_7454h
	jp cret

sub_07e3h:
	call csv
	ld hl,buf45
	ld (l9cf1h),hl
	ld hl,00000h
	ld (buf5),hl
	jp cret

sub_07f5h:
	call csv
	push hl	
	ld a,(buf6)
	ld (ix-001h),a
	ld a,(ix+006h)
	ld (buf6),a
	call sub_0bfch
	push hl	
	pop iy
	ld a,(ix-001h)
	ld (buf6),a
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl
	jp cret
ENDIF

sub_0817h:
	call ncsv
	defw 0fef9h
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	ld a,(iy+007h)
	jr z,l085eh
	cp 002h
	jr z,l085bh
IF OPTIM
	ld      a,(iy+004h)
	cp      1
ELSE
	ld de,00001h
	ld l,(iy+004h)
	ld h,(iy+005h)
	or a
	sbc hl,de
ENDIF
	jr nz,l085bh
	ld a,(iy+006h)
	cp 016h
	jr nz,l085bh
	ld e,(iy+000h)
	ld d,(iy+001h)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	cp 002h
	jr nz,l085bh
	ld l,e	
	ld h,d	
	push hl	
	pop iy
	jr l086ah
l085bh:
	ld a,(iy+007h)
l085eh:
	cp 002h
	jr z,l086ah
IF SHORTMSG
	ld	l,91
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8bd9h
	push hl	
	call _prerror
ENDIF
	pop bc	
l086ah:
	push iy
	pop hl	
	ld a,l	
	or h	
	jr z,l0895h
	ld a,(iy+007h)
	cp 002h
	jr nz,l0895h
	ld a,(iy+002h)
	or (iy+003h)
	jr z,l0895h
	ld l,(iy+002h)
	ld h,(iy+003h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (ix-004h),c
	ld (ix-003h),b
	inc hl	
	push hl	
	pop iy
	jr l08a1h
l0895h:
	ld iy,00000h
	ld (ix-004h),000h
	ld (ix-003h),000h
l08a1h:
	ld (ix-002h),000h
	ld (ix-001h),000h
	call sub_2671h
	ld a,l	
	ld (ix-007h),a
	ld (buf7),a
l08b3h:
	push iy
	pop hl	
	ld a,l	
	or h	
	jr z,l08cdh
	ld a,(iy+006h)
	cp 018h
	jr nz,l08cdh
	ld (ix-004h),000h
	ld (ix-003h),000h
	ld iy,00000h
l08cdh:
	ld a,(ix-007h)
	cp 03fh
	jp z,l09b8h
	push iy
	pop hl	
	ld a,l	
	or h	
	jr z,l08ffh
	ld hl,00014h
	push hl	
	push iy
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr z,l08ffh
IF SHORTMSG
	ld	l,78
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8bffh
	push hl	
	call _prerror
ENDIF
	pop bc	
	ld (ix-004h),000h
	ld (ix-003h),000h
	ld iy,00000h
l08ffh:
	ld hl,00003h
	push hl	
	call sub_07f5h
	ex (sp),hl	
	ld hl,0003ch
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	ld (ix-006h),l
	ld (ix-005h),h
	ld a,l	
	or (ix-005h)
	jp z,l09b8h
	push iy
	pop hl	
	ld a,l	
	or h	
	jr z,l094ah
	ld l,(ix-004h)
	ld h,(ix-003h)
	dec hl	
	ld (ix-004h),l
	ld (ix-003h),h
	inc hl	
	ld a,l	
	or h	
	jr nz,l094ah
IF SHORTMSG
	ld	l,77
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8c20h
	push hl	
	call _prerror
ENDIF
	pop bc	
	ld iy,00000h
	ld (ix-004h),000h
	ld (ix-003h),000h
l094ah:
	push iy
	pop hl	
	ld a,l	
	or h	
	jr z,l0977h
	ld hl,00001h
	push hl	
	push iy
	pop hl	
	ld bc,00008h
	add hl,bc	
	push hl	
	pop iy
	sbc hl,bc
	push hl	
	ld l,(ix-006h)
	ld h,(ix-005h)
	push hl	
	call sub_1f5dh
	pop bc	
	pop bc	
	pop bc	
	ld (ix-006h),l
	ld (ix-005h),h
	jr l0988h
l0977h:
	ld l,(ix-006h)
	ld h,(ix-005h)
	push hl	
	call sub_1d02h
	pop bc	
	ld (ix-006h),l
	ld (ix-005h),h
l0988h:
	push ix
	pop de	
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
	dec hl	
	add hl,hl	
	add hl,de	
	ld de,0fef9h
	add hl,de	
	ld e,(ix-006h)
	ld d,(ix-005h)
	ld (hl),e	
	inc hl	
	ld (hl),d	
	call sub_2671h
	ld e,l	
	ld (ix-007h),e
	ld a,e	
	cp 071h
	jp z,l08b3h
	ld (buf7),a
l09b8h:
IF OPTIM
	ld      a,(ix-004h)
	cp      1
ELSE
	ld de,00001h
	ld l,(ix-004h)
	ld h,(ix-003h)
	or a
	sbc hl,de
ENDIF
	jr nz,l09ddh
	ld a,(ix-002h)
	or (ix-001h)
	jr nz,l09ddh
	ld hl,00014h
	push hl	
	push iy
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr nz,l09f4h
l09ddh:
	ld a,(ix-004h)
	or (ix-003h)
	jr z,l09f4h
	ld a,(iy+006h)
	cp 018h
	jr z,l09f4h
IF SHORTMSG
	ld	l,76
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8c33h
	push hl	
	call _prerror
ENDIF
	pop bc	
l09f4h:
	ld a,(ix-002h)
	or (ix-001h)
	jr nz,l0a07h
	ld hl,00078h
	push hl	
	call sub_23b4h
IF OPTIM
	jp      popBCcret
ELSE
	pop bc
	jp cret
ENDIF
l0a07h:
	ld (ix-004h),000h
	ld (ix-003h),000h
	jr l0a5bh
l0a11h:
	inc hl	
	add hl,hl	
	add hl,de	
	ld de,0fef9h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	push ix
	pop de	
	ld l,(ix-004h)
	ld h,(ix-003h)
	add hl,hl	
	add hl,de	
	ld de,0fef9h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	ld hl,00071h
	push hl	
	call sub_23b4h
	pop bc	
	pop bc	
	pop bc	
	push hl	
	push ix
	pop de	
	ld l,(ix-004h)
	ld h,(ix-003h)
	inc hl	
	add hl,hl	
	add hl,de	
	ld de,0fef9h
	add hl,de	
	pop de	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	ld l,(ix-004h)
	ld h,(ix-003h)
	inc hl	
	ld (ix-004h),l
	ld (ix-003h),h
l0a5bh:
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld l,(ix-004h)
	ld h,(ix-003h)
	inc hl	
	or a	
	sbc hl,de
	push ix
	pop de	
	ld l,(ix-004h)
	ld h,(ix-003h)
	jr nz,l0a11h
	add hl,hl	
	add hl,de	
	ld de,0fef9h
	add hl,de	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	jp cret

sub_0a83h:
	call csv
	ld a,(ix+006h)
	ld (buf6),a
	call sub_0bfch
	push hl	
	ld hl,0003ch
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	push hl	
	pop iy
	ld a,l	
	or h	
	jr z,l0ae3h
	push iy
	call sub_0aedh
	pop bc	
	ld a,l	
	or a	
	jr nz,l0ab4h
IF SHORTMSG
	ld	l,75
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8c45h
	push hl	
	call _prerror
ENDIF
	pop bc	
	jr l0ae3h
l0ab4h:
	ld a,(buf6)
	cp 002h
	jr nz,l0ae3h
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5b69h
	pop bc	
	ld a,l	
	or a	
	jr z,l0adbh
	ld hl,0000ch
	push hl	
	push iy
	call sub_1ccch
	pop bc	
	pop bc	
	push hl	
	pop iy
	jr l0ae3h
l0adbh:
IF SHORTMSG
	ld	l,74
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8c62h
	push hl	
	call _prerror
ENDIF
	pop bc	
l0ae3h:
	xor a	
	ld (buf6),a
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl
	jp cret
ENDIF

sub_0aedh:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jr nz,l0b03h
l0afeh:
IF OPTIM
	jp      ldL1cret
ELSE
	ld l,001h
	jp cret
ENDIF
l0b03h:
	ld a,(iy+000h)
	ld (ix-001h),a
	ld a,(ix-001h)
	cp 073h
	jr nz,l0b24h
	ld hl,00012h
	push hl	
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5a76h
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc
	pop bc
	jp cret
ENDIF
l0b24h:
	ld a,(ix-001h)
	cp 04eh
	jr z,l0afeh
	cp 046h
	jr nz,l0b3eh
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	call sub_0b93h
	pop bc	
	ld a,l	
	or a	
	jr nz,l0afeh
l0b3eh:
	ld e,(ix-001h)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9275h
	add hl,de	
	ld l,(hl)	
	ld (ix-002h),l
	bit 4,(ix-002h)
	jr nz,l0b5dh
IF OPTIM
	jp      ldL0cret
ELSE
	ld l,000h
	jp cret
ENDIF
l0b5dh:
	bit 0,(ix-002h)
	jr nz,l0afeh
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	call sub_0aedh
	pop bc	
	ld a,l	
	or a	
	jr z,l0b8dh
	bit 1,(ix-002h)
	jr z,l0b87h
	ld l,(iy+003h)
	ld h,(iy+004h)
	push hl	
	call sub_0aedh
	pop bc	
	ld a,l	
	or a	
	jr z,l0b8dh
l0b87h:
IF OPTIM
	jp      ldHL1cret
ELSE
	ld hl,00001h
	jp cret
ENDIF
l0b8dh:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF

sub_0b93h:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+000h)
	ld (ix-001h),a
	ld a,(ix-001h)
	cp 073h
	jr nz,l0bcbh
	ld e,(iy+001h)
	ld d,(iy+002h)
	ld hl,00014h
	add hl,de	
	ld a,(hl)	
	cp 01fh
	jr z,l0bc5h
	ld hl,00014h
	add hl,de	
	ld a,(hl)	
	cp 02ah
	jr nz,l0bf6h
l0bc5h:
IF OPTIM
	jp      ldHL1cret
ELSE
	ld hl,00001h
	jp cret
ENDIF
l0bcbh:
	ld a,(ix-001h)
	cp 043h
	jr nz,l0be0h
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	call sub_0b93h
IF OPTIM
	jp      popBCcret
ELSE
	pop bc
	jp cret
ENDIF
l0be0h:
	ld a,(ix-001h)
	cp 045h
	jr nz,l0bf6h
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	call sub_0aedh
	pop bc	
	ld a,l	
	or a	
	jr nz,l0bc5h
l0bf6h:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h	
	jp cret
ENDIF

sub_0bfch:
	call ncsv
	defw 0ffe7h
	ld hl,(l9cf1h)
	ld (ix-00eh),l
	ld (ix-00dh),h
	ld hl,(l8bc7h)
	ld (ix-00ch),l
	ld (ix-00bh),h
	ld hl,0003ch
	push hl	
	call sub_2529h
	pop bc	
	ld (ix-011h),000h
	ld (ix-019h),000h
	ld (ix-018h),000h
l0c27h:
	ld hl,(l8bc7h)
	ld a,(hl)	
	cp 043h
	jr z,l0c34h
	ld a,(hl)	
	cp 044h
	jr nz,l0c39h
l0c34h:
	ld hl,00001h
	jr l0c3ch
l0c39h:
	ld hl,00000h
l0c3ch:
	ld a,l	
	ld (l8f86h),a
	ld (ix-013h),a
	call sub_2671h
	ld e,l	
	ld (ix-00fh),e
	xor a	
	ld (l8f86h),a
	ld b,03ch
	ld a,e	
	call brelop
	jr c,l0c60h
	ld b,080h
	ld a,(ix-00fh)
	call brelop
	jr c,l0c6ah
l0c60h:
	ld a,(ix-00fh)
	ld (buf7),a
	ld (ix-00fh),03ch
l0c6ah:
	ld e,(ix-00fh)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9275h
	add hl,de	
	bit 0,(hl)
	jp z,l0dfah
	ld a,(ix-011h)
	or a	
	jp z,l0ddah
l0c86h:
IF SHORTMSG
	ld	l,73
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8cbeh
	push hl	
	call _prerror
ENDIF
	ld l,(ix-00fh)
	ld h,000h
	ex (sp),hl	
	call sub_3610h
	pop bc	
	jp l1076h
l0c9ah:
	ld iy,(buf8)
	ld a,(ix-013h)
	or a	
	jp nz,l0d57h
	ld a,(iy+014h)
	or a	
	jp nz,l0d1dh
	call sub_35f7h
IF OPTIM
	ld      a,l
	cp      28h
ELSE
	ld de,00028h
	or a
	sbc hl,de
ENDIF
	jr nz,l0cfbh
	ld (ix-001h),002h
	ld (ix-002h),008h
	ld (ix-006h),000h
	ld (ix-005h),000h
	ld (ix-004h),000h
	ld (ix-003h),000h
	push ix
	pop de	
	ld hl,0fff8h
	add hl,de	
	push hl	
	ld hl,0001fh
	push hl	
	push iy
	call sub_4eedh
	pop bc	
	pop bc	
	pop bc	
	push hl	
	pop iy
	ld a,(iy+012h)
	or 042h
	ld (iy+012h),a
	ld (iy+015h),000h
	push iy
	call sub_0493h
l0cf7h:
	pop bc	
	jp l0d51h
l0cfbh:
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
IF SHORTMSG
	ld	l,72
	push	hl
	call	_pr_nr_of_error1
ELSE
	ld hl,l8c83h
	push hl	
	call _prerror
ENDIF
	pop bc	
	pop bc	
	ld (iy+014h),02ah
	ld (iy+006h),008h
	ld (iy+012h),011h
	ld (iy+013h),000h
	jr l0d51h
l0d1dh:
	ld l,(iy+012h)
	ld h,(iy+013h)
	ld (ix-017h),l
	ld (ix-016h),h
	bit 4,l
	jr nz,l0d3eh
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
IF SHORTMSG
	ld	l,71
	push	hl
	call	_pr_nr_of_error1
ELSE
	ld hl,l8c9ch
	push hl	
	call _prerror
ENDIF
	pop bc	
	jr l0cf7h
l0d3eh:
	ld a,(iy+014h)
	cp 01fh
	jr nz,l0d51h
	bit 0,(ix-016h)
	push iy
	jr nz,l0d53h
	call sub_0493h
	pop bc	
l0d51h:
	push iy
l0d53h:
	call sub_51cfh
	pop bc	
l0d57h:
	push iy
	call sub_240eh
	pop bc	
	ld (ix-00ah),l
	ld (ix-009h),h
l0d63h:
	ld l,(ix-00ah)
	ld h,(ix-009h)
	push hl	
	call sub_24e7h
	pop bc	
l0d6eh:
	ld (ix-011h),001h
	jp l0c27h
l0d75h:
	ld hl,00008h
	push hl	
	ld de,(buf8)
	ld hl,(buf9)
	push hl	
	push de	
	call sub_1b4bh
	pop bc	
	pop bc	
	pop bc	
l0d88h:
	ld (ix-00ah),l
	ld (ix-009h),h
	jr l0d63h
l0d90h:
	ld hl,0000ah
	push hl	
	ld de,(buf8)
	ld hl,(buf9)
	push hl	
	push de	
	call sub_1b4bh
	pop bc	
	pop bc	
l0da2h:
	pop bc	
	jr l0d88h
l0da5h:
	ld hl,(buf8)
	push hl	
	call sub_1b70h
	jr l0da2h
l0daeh:
	call sub_245dh
	ld (ix-00ah),l
	ld (ix-009h),h
	ld de,(buf10)
	inc hl	
	inc hl	
	inc hl	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	ld hl,(buf8)
	push hl	
	ld l,(ix-00ah)
	ld h,(ix-009h)
	push hl	
	call sub_053fh
	pop bc	
	ld hl,(buf8)
	ex (sp),hl	
	call _free
	pop bc	
	jr l0d63h
l0ddah:
	ld a,(ix-00fh)
	cp 073h
	jp z,l0c9ah
	cp 074h
	jr z,l0d75h
	cp 075h
	jr z,l0d90h
	cp 076h
	jr z,l0daeh
	cp 077h
	jp z,l0c86h
	cp 07fh
	jr z,l0da5h
	jp l0d63h
l0dfah:
	ld a,(ix-00fh)
	cp 03ch
	jp z,l0f23h
	cp 03eh
	jp z,l0e65h
	cp 03fh
	jp z,l0f5ch
	cp 041h
	jp z,l0f4bh
	cp 042h
	jp z,l0f5ch
	cp 04ah
	jp z,l0f65h
	cp 04ch
	jp z,l0f65h
	ld e,a	
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9275h
	add hl,de	
	bit 2,(hl)
	jr z,l0e40h
	ld a,(ix-011h)
	or a	
	jr nz,l0e40h
	ld a,(ix-00fh)
	add a,0f5h
	ld (ix-00fh),a
l0e40h:
	ld e,(ix-00fh)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9275h
	add hl,de	
	bit 1,(hl)
	ld hl,00001h
	jr nz,l0e58h
	dec hl	
l0e58h:
	ld (ix-014h),l
	ld a,l	
	cp (ix-011h)
	jp z,l0f56h
	jp l0c86h
l0e65h:
	call sub_2671h
	ld a,l	
	ld (ix-010h),a
	ld (buf7),a
	cp 077h
	jr z,l0e86h
	cp 073h
	jp nz,l0f02h
	ld de,(buf8)
	ld hl,00014h
	add hl,de	
	ld a,(hl)	
	cp 02dh
	jp nz,l0f02h
l0e86h:
	ld a,(ix-011h)
	or a	
	jp nz,l0c86h
	push ix
	pop de	
	ld hl,0fff8h
	add hl,de	
	push hl	
	ld hl,00000h
	push hl	
	call sub_5dd1h
	pop bc	
	ld hl,00000h
	ex (sp),hl	
	push ix
	pop de	
	ld hl,0fff8h
	add hl,de	
	push hl	
	ld hl,0004fh
	push hl	
	call sub_69cah
	pop bc	
	pop bc	
	push hl	
	pop iy
	ex (sp),hl	
	call sub_516ch
	pop bc	
	push iy
	call sub_51cfh
	pop bc	
	call sub_2671h
	ld a,l	
	ld (ix-010h),a
	cp 03fh
	jp nz,l0c86h
	push ix
	pop de	
	ld hl,0fff8h
	add hl,de	
	ex de,hl	
	push iy
	pop hl	
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	push ix
	pop de	
	ld hl,0fff8h
	add hl,de	
	push hl	
	call sub_24b6h
	ex (sp),hl	
	call sub_24e7h
	pop bc	
	ld hl,(l8bc7h)
	ld a,(hl)	
	cp 04eh
	jr nz,l0efch
	inc hl	
	ld (hl),064h
	jp l0d6eh
l0efch:
	ld (ix-00fh),04fh
	jr l0f23h
l0f02h:
	ld a,(ix-011h)
	or a	
	jr z,l0f23h
	ld hl,0003dh
	push hl	
	call sub_2529h
	pop bc	
	ld hl,(l9cf1h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld hl,00005h
	add hl,bc	
	push hl	
	call sub_0817h
	ex (sp),hl	
	call sub_24e7h
	pop bc	
l0f23h:
	ld e,(ix-00fh)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9274h
	add hl,de	
	ld l,(hl)	
	ld (ix-015h),l
	ld b,003h
	ld a,(buf6)
	call brelop
	jr c,l0f74h
	ld a,(ix-00fh)
	cp 071h
	jr nz,l0f74h
	jp l0f8ah
l0f4bh:
	ld a,(ix-011h)
	or a	
	jp z,l0c86h
	ld (ix-00fh),040h
l0f56h:
	ld (ix-011h),000h
	jr l0f23h
l0f5ch:
	ld a,(ix-011h)
	or a	
	jr nz,l0f23h
	jp l0c86h
l0f65h:
	ld a,(ix-011h)
	or a	
	jr z,l0f23h
	ld a,(ix-00fh)
	inc a	
	ld (ix-00fh),a
	jr l0f23h
l0f74h:
	ld a,(buf6)
	cp 001h
	jr nz,l0f8eh
	ld a,(ix-00fh)
	cp 063h
	jr nz,l0f8eh
	ld hl,(l8bc7h)
	ld a,(hl)	
	cp 062h
	jr z,l0f8eh
l0f8ah:
	ld (ix-015h),005h
l0f8eh:
	ld (ix-012h),000h
	ld b,(ix-015h)
	ld hl,(l8bc7h)
	inc hl	
	ld a,(hl)	
	call brelop
	jr c,l0fbfh
	ld hl,(l8bc7h)
	inc hl	
	ld a,(hl)	
	cp (ix-015h)
	jp nz,l1058h
	ld e,(ix-00fh)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9275h
	add hl,de	
	bit 3,(hl)
	jp z,l1058h
l0fbfh:
	ld a,(ix-00fh)
	cp 03eh
	jr z,l0ff1h
	cp 040h
	jr z,l0ff1h
	cp 04bh
	jr z,l0fd2h
	cp 04dh
	jr nz,l0fd6h
l0fd2h:
	ld (ix-015h),01fh
l0fd6h:
	ld l,(ix-00fh)
	ld h,000h
	push hl	
	call sub_2529h
	pop bc	
	ld a,(ix-015h)
	ld hl,(l8bc7h)
	inc hl	
	ld (hl),a	
l0fe8h:
	ld a,(ix-012h)
	or a	
	jr nz,l0f8eh
	jp l0c27h
l0ff1h:
	ld (ix-015h),004h
	jr l0fd6h
l0ff7h:
	ld a,(ix-00fh)
	cp 03ch
	jr z,l1001h
	ld (buf7),a
l1001h:
	call sub_250ah
	ld (ix-00ah),l
	ld (ix-009h),h
	ld a,l	
	or h	
	jp z,l0c86h
	ld de,(l9cf1h)
	ld l,(ix-00eh)
	ld h,(ix-00dh)
	or a	
	sbc hl,de
	jp nz,l0c86h
	jp l108dh
l1022h:
	ld a,(ix-00fh)
	cp 03fh
	jr z,l1037h
	ld hl,l8cbah
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-00fh)
	ld (buf7),a
l1037h:
	call sub_10a8h
	ld a,l	
	or a	
	jr z,l0fe8h
	jp l0c86h
l1041h:
	ld a,(ix-00fh)
	cp 042h
	jr z,l1037h
	ld hl,l8cbch
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-00fh)
	ld (buf7),a
	jr l1037h
l1058h:
	ld hl,(l8bc7h)
	ld a,(hl)	
	cp 03ch
	jr z,l0ff7h
	cp 03eh
	jr z,l1022h
	cp 040h
	jr z,l1041h
	ld (ix-012h),001h
	jr l1037h
l106eh:
	call sub_250ah
	push hl	
	call sub_2569h
	pop bc	
l1076h:
	ld de,(l9cf1h)
	ld l,(ix-00eh)
	ld h,(ix-00dh)
	or a	
	sbc hl,de
	jr nz,l106eh
	ld (ix-00ah),000h
	ld (ix-009h),000h
l108dh:
	ld l,(ix-00eh)
	ld h,(ix-00dh)
	ld (l9cf1h),hl
	ld l,(ix-00ch)
	ld h,(ix-00bh)
	ld (l8bc7h),hl
	ld l,(ix-00ah)
	ld h,(ix-009h)
	jp cret

sub_10a8h:
	call ncsv
	defw 0fffbh
	call sub_255dh
	ld a,l	
	ld (ix-005h),a
	cp 03eh
	jr nz,l10bdh
l10b8h:
IF OPTIM
	jp      ldL0cret
ELSE
	ld l,000h
	jp cret
ENDIF
l10bdh:
	ld (ix-002h),000h
	ld (ix-001h),000h
	ld a,(ix-005h)
	cp 078h
	jr z,l10fah
	ld e,a	
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9275h
	add hl,de	
	bit 1,(hl)
	jr z,l10ebh
	call sub_250ah
	ld (ix-002h),l
	ld (ix-001h),h
	ld a,l	
	or h	
	jr z,l10f5h
l10ebh:
	call sub_250ah
	push hl	
	pop iy
	ld a,l	
	or h	
	jr nz,l10fah
l10f5h:
IF OPTIM
	jp      ldL1cret
ELSE
	ld l,001h
	jp cret
ENDIF
l10fah:
	ld a,(ix-005h)
	cp 040h
	jp z,l1149h
	cp 044h
	jp z,l11c0h
	cp 04ah
	jp z,l1190h
	cp 04bh
	jp z,l1190h
	cp 04ch
	jp z,l1190h
	cp 04dh
	jp z,l1190h
	cp 04eh
	jp z,l11d5h
	cp 04fh
	jp z,l12eah
	cp 071h
	jp z,l12d3h
l112ah:
	ld a,(ix-002h)
	or (ix-001h)
	jp nz,l1313h
l1133h:
	ld de,00000h
l1136h:
	ld a,(ix-005h)
	cp 062h
	ld hl,00001h
	jr z,l1141h
	dec hl	
l1141h:
	or a	
	sbc hl,de
	jp z,l1325h
	jr l10f5h
l1149h:
	ld hl,00045h
	push hl	
	call sub_2529h
	pop bc	
	ld hl,(l8bc7h)
	inc hl	
	ld (hl),064h
	ld (ix-005h),055h
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5b69h
	pop bc	
	ld a,l	
	or a	
	jr z,l1186h
	ld hl,0000ch
	push hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	call sub_1ccch
	pop bc	
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	jr l112ah
l1186h:
IF SHORTMSG
	ld	l,70
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8cd0h
	push hl	
	call _prerror
ENDIF
	pop bc	
	jr l112ah
l1190h:
	ld hl,buf25
	ld (ix-002h),l
	ld (ix-001h),h
	ld a,(ix-005h)
	cp 04ah
	jr z,l11b7h
	cp 04ch
	jr z,l11b2h
	cp 04bh
	jr z,l11adh
	ld hl,00068h
	jr l11bah
l11adh:
	ld hl,00066h
	jr l11bah
l11b2h:
	ld hl,00067h
	jr l11bah
l11b7h:
	ld hl,00065h
l11bah:
	ld (ix-005h),l
	jp l112ah
l11c0h:
	ld (ix-005h),043h
	push iy
	ld hl,00045h
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
l11cfh:
	push hl	
	pop iy
	jp l112ah
l11d5h:
	ld a,(iy+000h)
	cp 076h
	jp nz,l1218h
	ld hl,00009h
	push hl	
	ld e,(iy+003h)
	ld d,(iy+004h)
	ld a,d	
	rla	
	sbc a,a	
	ld l,a	
	ld h,a	
	ex de,hl	
	ld bc,00001h
	add hl,bc	
	ex de,hl	
	ld bc,00000h
	adc hl,bc
	push hl	
	push de	
	call sub_1b4bh
	pop bc	
	pop bc	
	pop bc	
	ld (ix-004h),l
	ld (ix-003h),h
	push iy
	call sub_2569h
	ld l,(ix-004h)
	ld h,(ix-003h)
	ex (sp),hl	
	call sub_24e7h
l1214h:
	pop bc	
	jp l10b8h
l1218h:
	ld a,(iy+000h)
	cp 077h
	jp z,l1297h
	cp 073h
	jp z,l1294h
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_24b6h
	pop bc	
	ld (ix-004h),l
	ld (ix-003h),h
	ex de,hl	
	ld hl,00007h
	add hl,de	
	ld de,00000h
	ld (hl),e	
	inc hl	
	ld (hl),d	
	ld e,(ix-004h)
	ld d,(ix-003h)
	ld hl,0000ch
	add hl,de	
	ld (hl),000h
	ld a,(iy+00ch)
	cp 001h
	jp nz,l1285h
	ld l,e	
	ld h,d	
	push hl	
	ld hl,0004eh
	push hl	
	call sub_1441h
	pop bc	
	ld (ix-004h),l
	ld (ix-003h),h
	ld hl,00009h
	ex (sp),hl	
	ld l,(iy+007h)
	ld h,(iy+008h)
	push hl	
	call sub_21c7h
	ex (sp),hl	
	call sub_1ccch
	pop bc	
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	ld (ix-005h),050h
l1285h:
	push iy
	call sub_2569h
	pop bc	
	ld l,(ix-004h)
	ld h,(ix-003h)
	jp l11cfh
l1294h:
	ld a,(iy+000h)
l1297h:
	cp 077h
	jp nz,l112ah
	ld a,(iy+00ch)
	cp 001h
	jp nz,l112ah
	ld hl,00009h
	push hl	
	ld l,(iy+007h)
	ld h,(iy+008h)
	push hl	
	call sub_21c7h
	ex (sp),hl	
	call sub_1ccch
	pop bc	
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	push iy
	ld hl,0004eh
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	push hl	
	pop iy
	ld (ix-005h),050h
	jp l112ah
l12d3h:
	ld hl,(l8bc7h)
	ld a,(hl)	
	cp 03eh
	jr nz,l12e3h
	inc hl	
	inc hl	
	ld a,(hl)	
	cp 03dh
	jp z,l112ah
l12e3h:
	ld (ix-005h),072h
	jp l112ah
l12eah:
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	ld hl,0003ch
	push hl	
	call sub_1441h
	pop bc	
	ex (sp),hl	
	call sub_1bf7h
	pop bc	
	ex (sp),hl	
	call sub_24e7h
	pop bc	
	push iy
	call sub_2569h
	jp l1214h
l1313h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld a,(hl)	
	cp 063h
	jp nz,l1133h
	ld de,00001h
	jp l1136h
l1325h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	push iy
	ld l,(ix-005h)
	ld h,000h
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	ex (sp),hl	
	call sub_24e7h
	jp l1214h

sub_1340h:
	call csv
	push hl	
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld hl,0001ah
	push hl	
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr nz,l1389h
	ld hl,0001ch
	push hl	
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr nz,l1389h
IF SHORTMSG
	ld	l,69
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8cf2h
	push hl	
	call _prerror
ENDIF
	pop bc	
l1380h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	jp cret
l1389h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld a,(hl)	
	cp 073h
	jr z,l139eh
IF SHORTMSG
	ld	l,68
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8d08h
	push hl	
	call _prerror
ENDIF
l139bh:
	pop bc	
	jr l1380h
l139eh:
	ld l,(iy+005h)
	ld h,(iy+006h)
	ld (ix-004h),l
	ld (ix-003h),h
	ld a,l	
	or h	
	jr z,l1380h
	ld e,(ix-004h)
	ld d,(ix-003h)
	ld hl,00012h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 0,c
	jr nz,l13d1h
	ld hl,00017h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
IF SHORTMSG
	ld	l,67
	push	hl
	call	_pr_nr_of_error1
ELSE
	ld hl,l8d25h
	push hl	
	call _prerror
ENDIF
	pop bc	
	jr l139bh
l13d1h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld hl,00017h
	add hl,bc	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	ld l,(ix-004h)
	ld h,(ix-003h)
	push hl	
	call sub_56cdh
	pop bc	
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	ld a,l	
	or h	
	jr z,l1380h
	push hl	
	call sub_51cfh
	ld hl,0007eh
	ex (sp),hl	
	call _callocm1
	pop bc	
	push hl	
	pop iy
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,0000eh
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (iy+001h),c
	ld (iy+002h),b
	push iy
	pop de	
	ld hl,00005h
	add hl,de
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	call sub_2569h
	pop bc	
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl
	jp cret
ENDIF

