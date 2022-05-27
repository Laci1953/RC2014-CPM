include p1.inc

	psect text

	global  sub_4595h
	global  sub_469bh
	global  sub_4838h
	global  sub_49e1h
	global  sub_4a1eh
	global  sub_4a90h
	global  sub_4baah
	global  sub_4c03h
	global  sub_4c57h
	global  sub_4ce8h
	global  sub_4d15h
	global  sub_4d92h
	global  sub_4e90h
	global  sub_4eedh
	global  sub_516ch
	global  sub_51cfh
	global  sub_51e7h
	global  sub_5356h
	global  sub_549ch
	global  sub_54ach
	global  sub_54c0h
	global  sub_56a4h
	global  sub_56cdh
	global  sub_573bh
	global  sub_5785h
	global  sub_578dh
	global  sub_58bdh
	global  sub_591dh
	global  sub_5a4ah
	global  sub_5a76h
	global  sub_5aa4h
	global  sub_5ad5h
	global  sub_5b08h
	global  sub_5b38h
	global  sub_5b69h
	global  sub_5b99h
	global  sub_5be1h
	global  sub_5c19h
IF SHORTMSG
	global _pr_nr_of_error0
	global _pr_nr_of_error1
	global _pr_nr_of_warning0
	global _pr_nr_of_warning1
ENDIF


l021eh  equ 021eh
l010fh  equ 010fh

sub_4595h:
	call ncsv
	defw 0fffah
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 03eh
	jr z,l45b4h
	ld hl,l9585h
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
l45b4h:
	call sub_0bfch
	push hl	
	pop iy
	call sub_2671h
	ld a,l	
	ld (ix-001h),a
	cp 03fh
	jr z,l45d3h
	ld hl,l9587h
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
l45d3h:
	call sub_5785h
	ld (ix-005h),l
	ld (ix-004h),h
	ld hl,00000h
	push hl	
	push iy
	ld l,(ix-005h)
	ld h,(ix-004h)
	push hl	
	call sub_4d15h
	pop bc	
	pop bc	
	ld l,(ix+00ch)
	ld h,(ix+00dh)
	ex (sp),hl	
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
	ld a,(buf34)
	ld (ix-006h),a
	xor a	
	ld (buf34),a
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 01ch
	jp nz,l4688h
	call sub_5785h
	ld (ix-003h),l
	ld (ix-002h),h
	push hl	
	call sub_4ce8h
	ld l,(ix-005h)
	ld h,(ix-004h)
	ex (sp),hl	
	call sub_0258h
	ld l,(ix+00ch)
	ld h,(ix+00dh)
	ex (sp),hl	
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
	ld a,(buf34)
	or a	
	jr z,l4673h
	ld a,(ix-006h)
	or a	
	jr z,l4673h
	ld hl,00001h
	jr l4676h
l4673h:
	ld hl,00000h
l4676h:
	ld a,l	
	ld (buf34),a
	ld l,(ix-003h)
	ld h,(ix-002h)
	push hl	
	call sub_0258h
IF OPTIM
	jp      popBCcret
ELSE
	pop bc	
	jp cret
ENDIF
l4688h:
	ld a,(ix-001h)
	ld (buf7),a
	ld l,(ix-005h)
	ld h,(ix-004h)
	push hl	
	call sub_0258h
	jp cret

sub_469bh:
	call ncsv
	defw 0fbf5h
	call sub_2671h
	ld a,l	
	ld (ix-001h),a
	cp 03eh
	jr z,l46b9h
	ld hl,l9589h
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
l46b9h:
	ld (ix-007h),000h
	ld (ix-006h),000h
	push ix
	pop de	
	ld hl,0fbf9h
	add hl,de	
	ld de,00000h
	ld (hl),e	
	inc hl	
	ld (hl),d	
	push de	
	push ix
	pop de	
	ld hl,0fbf7h
	add hl,de	
	pop de	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	call sub_0bfch
	push hl	
	ld hl,0003ch
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	push hl	
	push ix
	pop de	
	ld hl,0fbf5h
	add hl,de	
	pop de	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	ld a,e	
	or d	
	jr z,l4733h
	push ix
	pop de	
	ld hl,0fbf5h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld hl,00005h
	add hl,bc	
	push hl	
	pop iy
	ld hl,00012h
	push hl	
	push iy
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr nz,l4733h
	push iy
	call sub_5b08h
	pop bc	
	ld a,l	
	or a	
	jr z,l472bh
	ld b,00ah
	ld a,(iy+006h)
	call brelop
	jr c,l4733h
l472bh:
IF SHORTMSG
	ld	l,43
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l958bh
	push hl	
	call _prerror
ENDIF
	pop bc	
l4733h:
	call sub_2671h
	ld a,l	
	ld (ix-001h),a
	cp 03fh
	jr z,l474ch
	ld hl,l95aeh
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
l474ch:
	call sub_5785h
	ld (ix-003h),l
	ld (ix-002h),h
	call sub_5785h
	ld (ix-005h),l
	ld (ix-004h),h
	push hl	
	call sub_4ce8h
	ld a,001h
	ld (buf34),a
	push ix
	pop de	
	ld hl,0fff9h
	add hl,de	
	ex (sp),hl	
	push ix
	pop de	
	ld hl,0fbf5h
	add hl,de	
	push hl	
	ld l,(ix-003h)
	ld h,(ix-002h)
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_4126h
	pop bc	
	pop bc	
	pop bc	
	pop bc	
	push ix
	pop de	
	ld hl,0fbf9h
	add hl,de	
	ld a,(hl)	
	inc hl	
	or (hl)	
	jr nz,l47b0h
	push ix
	pop de	
	ld hl,0fbf9h
	add hl,de	
	ld e,(ix-003h)
	ld d,(ix-002h)
	ld (hl),e	
	inc hl	
	ld (hl),d	
	ld (ix-007h),001h
	ld (ix-006h),000h
l47b0h:
	ld l,(ix-003h)
	ld h,(ix-002h)
	push hl	
	call sub_4ce8h
	ld l,(ix-005h)
	ld h,(ix-004h)
	ex (sp),hl	
	call sub_0258h
	push ix
	pop de	
	ld hl,0fbf5h
	add hl,de	
	ex (sp),hl	
	call sub_02a6h
	pop bc	
	push ix
	pop de	
	ld hl,0fbf5h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	call sub_2569h
	pop bc	
	push ix
	pop de	
	ld hl,0fbf7h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (ix-009h),c
	ld (ix-008h),b
	jr l4809h
l47f1h:
	push ix
	pop de	
	ld l,(ix-009h)
	ld h,(ix-008h)
	add hl,hl	
	add hl,hl	
	add hl,de	
	ld de,0fbfbh
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	call sub_2569h
	pop bc	
l4809h:
	ld l,(ix-009h)
	ld h,(ix-008h)
	dec hl	
	ld (ix-009h),l
	ld (ix-008h),h
	bit 7,h
	jr z,l47f1h
	ld l,(ix-003h)
	ld h,(ix-002h)
	push hl	
	call sub_0258h
	pop bc	
	ld a,(ix-007h)
	or (ix-006h)
	ld hl,00001h
	jr z,l4831h
	dec hl	
l4831h:
	ld a,l	
	ld (buf34),a
	jp cret

sub_4838h:
	call ncsv
	defw 0fff1h
	ld (ix-00ah),000h
	ld (ix-009h),000h
	call sub_2671h
	ld a,l	
	ld (ix-00bh),a
	cp 03eh
	jr z,l4858h
	ld hl,l95b0h
	push hl	
	call _errexpected
	pop bc	
l4858h:
	call sub_2671h
	ld a,l	
	ld (ix-00bh),a
	cp 002h
	jr z,l488ch
	ld (buf7),a
	call sub_0bfch
	push hl	
	ld hl,0003ch
	push hl	
	call sub_1441h
	pop bc	
	push hl	
	pop iy
	ex (sp),hl	
	call sub_042dh
	pop bc	
	push iy
	call sub_2569h
	ld hl,l95b2h
	ex (sp),hl	
	ld hl,00002h
	push hl	
	call sub_363fh
	pop bc	
	pop bc	
l488ch:
	call sub_2671h
	ld a,l	
	ld (ix-00bh),a
	cp 002h
	jr z,l48bah
	ld (ix-00ah),001h
	ld (ix-009h),000h
	ld (buf7),a
	call sub_0bfch
	ld (ix-00dh),l
	ld (ix-00ch),h
	ld hl,l95b4h
	push hl	
	ld hl,00002h
	push hl	
	call sub_363fh
	pop bc	
	pop bc	
	jr l48c2h
l48bah:
	ld (ix-00dh),000h
	ld (ix-00ch),000h
l48c2h:
	call sub_2671h
	ld a,l	
	ld (ix-00bh),a
	cp 03fh
	jr z,l48feh
	ld (buf7),a
	call sub_0bfch
	push hl	
	ld hl,0003ch
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	ld (ix-00fh),l
	ld (ix-00eh),h
	call sub_2671h
	ld a,l	
	ld (ix-00bh),a
	cp 03fh
	jr z,l4906h
	ld hl,l95b6h
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-00bh)
	ld (buf7),a
	jr l4906h
l48feh:
	ld (ix-00fh),000h
	ld (ix-00eh),000h
l4906h:
	call sub_5785h
	ld (ix-006h),l
	ld (ix-005h),h
	call sub_5785h
	ld (ix-004h),l
	ld (ix-003h),h
	call sub_5785h
	ld (ix-002h),l
	ld (ix-001h),h
	ld a,(ix-00dh)
	or (ix-00ch)
	jr z,l4937h
	call sub_5785h
	ld (ix-008h),l
	ld (ix-007h),h
	push hl	
	call sub_4ce8h
	pop bc	
l4937h:
	ld l,(ix-006h)
	ld h,(ix-005h)
	push hl	
	call sub_0258h
	push ix
	pop de	
	ld hl,0fff6h
	add hl,de	
	ex (sp),hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	ld l,(ix-004h)
	ld h,(ix-003h)
	push hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	call sub_4126h
	pop bc	
	pop bc	
	pop bc	
	ld l,(ix-002h)
	ld h,(ix-001h)
	ex (sp),hl	
	call sub_0258h
	pop bc	
	ld a,(ix-00fh)
	or (ix-00eh)
	jr z,l498ch
	ld l,(ix-00fh)
	ld h,(ix-00eh)
	push hl	
	call sub_042dh
	ld l,(ix-00fh)
	ld h,(ix-00eh)
	ex (sp),hl	
	call sub_2569h
	pop bc	
l498ch:
	ld a,(ix-00dh)
	or (ix-00ch)
	jr z,l49b8h
	ld l,(ix-008h)
	ld h,(ix-007h)
	push hl	
	call sub_0258h
	ld hl,00001h
	ex (sp),hl	
	ld l,(ix-00dh)
	ld h,(ix-00ch)
	push hl	
	ld l,(ix-006h)
	ld h,(ix-005h)
	push hl	
	call sub_4d15h
	pop bc	
	pop bc	
	pop bc	
	jr l49c3h
l49b8h:
	ld l,(ix-006h)
	ld h,(ix-005h)
	push hl	
	call sub_4ce8h
	pop bc	
l49c3h:
	ld l,(ix-004h)
	ld h,(ix-003h)
	push hl	
	call sub_0258h
	pop bc	
	ld a,(ix-00ah)
	or (ix-009h)
	ld hl,00001h
	jr z,l49dah
	dec hl	
l49dah:
	ld a,l	
	ld (buf34),a
	jp cret

sub_49e1h:
	call csv
	push hl	
	ld a,(ix+006h)
	or (ix+007h)
	jr z,l49ffh
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_4ce8h
	pop bc	
	ld a,001h
	ld (buf34),a
	jr l4a07h
l49ffh:
IF SHORTMSG
	ld	l,42
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l95b8h
	push hl	
	call _prerror
ENDIF
	pop bc	
l4a07h:
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 002h
	jp z,cret
	ld hl,l95d5h
	push hl	
	call _errexpected
	jp cret

sub_4a1eh:
	call csv
	push hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	pop iy
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 063h
	jr z,l4a3fh
	ld hl,l95d7h
	push hl	
	call _errexpected
	pop bc	
l4a3fh:
	push iy
	pop hl	
	ld a,l	
	or h	
	jr z,l4a67h
	ld a,(iy+004h)
	or (iy+005h)
	jr z,l4a58h
IF SHORTMSG
	ld	l,41
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l95d9h
	push hl	
	call _prerror
ENDIF
l4a55h:
	pop bc	
	jr l4a6fh
l4a58h:
	call sub_5785h
	ld (iy+004h),l
	ld (iy+005h),h
	push hl	
	call sub_0258h
	jr l4a55h
l4a67h:
IF SHORTMSG
	ld	l,40
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l95f0h
	push hl	
	call _prerror
ENDIF
	pop bc	
l4a6fh:
	xor a	
	ld (buf34),a
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
	call sub_4126h
	jp cret

sub_4a90h:
	call ncsv
	defw 0fff7h
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	pop iy
	ld hl,00001h
	push hl	
	call sub_0a83h
	pop bc	
	ld (ix-003h),l
	ld (ix-002h),h
	call sub_2671h
	ld a,l	
	ld (ix-001h),a
	cp 063h
	jr z,l4ac5h
	ld hl,l9608h
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
l4ac5h:
	call sub_5785h
	ld (ix-005h),l
	ld (ix-004h),h
	push hl	
	call sub_0258h
	pop bc	
	push iy
	pop hl	
	ld a,l	
	or h	
	jp z,l4b81h
	ld de,000ffh
	ld l,(iy+002h)
	ld h,(iy+003h)
	inc hl	
	ld (iy+002h),l
	ld (iy+003h),h
	dec hl	
	ld (ix-007h),l
	ld (ix-006h),h
	or a	
	sbc hl,de
	jr nz,l4affh
	ld hl,l960ah
	push hl	
	call _fatalerror
	pop bc	
l4affh:
	push iy
	pop de	
	ld l,(ix-007h)
	ld h,(ix-006h)
	add hl,hl	
	add hl,hl	
	add hl,de	
	ld de,00006h
	add hl,de	
	ld (ix-009h),l
	ld (ix-008h),h
	ld e,(ix-005h)
	ld d,(ix-004h)
	inc hl	
	inc hl	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	ld a,(ix-003h)
	or (ix-002h)
	jp z,l4b89h
	ld a,(iy+000h)
	or (iy+001h)
	jp z,l4b89h
	ld l,(ix-003h)
	ld h,(ix-002h)
	push hl	
	ld e,(iy+000h)
	ld d,(iy+001h)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_24b6h
	ex (sp),hl	
	ld hl,0007dh
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	pop bc	
	ld (ix-003h),l
	ld (ix-002h),h
	inc hl	
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld l,(ix-009h)
	ld h,(ix-008h)
	ld (hl),c	
	inc hl	
	ld (hl),b	
	ld de,00000h
	ld l,(ix-003h)
	ld h,(ix-002h)
	inc hl	
	inc hl	
	inc hl	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	dec hl	
	dec hl	
	dec hl	
	dec hl	
	push hl	
	call sub_2569h
	pop bc	
	jr l4b89h
l4b81h:
IF SHORTMSG
	ld	l,39
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l9623h
	push hl	
	call _prerror
ENDIF
	pop bc	
l4b89h:
	xor a	
	ld (buf34),a
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
	call sub_4126h
	jp cret

sub_4baah:
	call csv
	push hl	
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 002h
	jr z,l4be6h
	ld (buf7),a
	call sub_0bfch
	push hl	
	ld hl,0003ch
	push hl	
	call sub_1441h
	pop bc	
	ex (sp),hl	
	call sub_4d67h
	pop bc	
	call sub_2671h
	ld a,l	
	cp 002h
	jr z,l4bf4h
	ld hl,l9638h
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-001h)
	ld (buf7),a
	jr l4bf4h
l4be6h:
	ld a,(buf33)
	or a	
	jr nz,l4bf4h
IF SHORTMSG
	ld	l,80
	push	hl
	call	_pr_nr_of_warning0
ELSE
	ld hl,l963ah
	push hl	
	call _prwarning
ENDIF
	pop bc	
l4bf4h:
	ld hl,(buf35)
	push hl	
	call sub_4ce8h
	ld a,001h
	ld (buf34),a
	jp cret

sub_4c03h:
	call csv
	push hl	
	call sub_2671h
	ld e,l	
	ld (ix-001h),e
	ld a,e	
	cp 073h
	jr z,l4c1eh
	ld hl,l965dh
	push hl	
	call _errexpected
IF OPTIM
	jp      popBCcret
ELSE
	pop bc	
	jp cret
ENDIF
l4c1eh:
	ld hl,(buf8)
	push hl	
	call sub_4ca4h
	pop bc	
	push hl	
	pop iy
	ld a,l	
	or h	
	jr z,l4c3ch
	ld l,(iy+000h)
	ld h,(iy+001h)
	push hl	
	call sub_4ce8h
	pop bc	
	set 1,(iy+012h)
l4c3ch:
	ld a,001h
	ld (buf34),a
	call sub_2671h
	ld a,l	
	ld (ix-001h),a
	cp 002h
	jp z,cret
	ld hl,l966eh
	push hl	
	call _errexpected
	jp cret

sub_4c57h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	push hl	
	call sub_4ca4h
	pop bc	
	push hl	
	pop iy
	ld a,l	
	or h	
	jr z,l4c7eh
	ld l,(iy+000h)
	ld h,(iy+001h)
	push hl	
	call sub_0258h
	pop bc	
	set 0,(iy+012h)
l4c7eh:
	xor a	
	ld (buf34),a
	ld l,(ix+00eh)
	ld h,(ix+00fh)
	push hl	
	ld l,(ix+00ch)
	ld h,(ix+00dh)
	push hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	call sub_4126h
	jp cret

sub_4ca4h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+014h)
	or a	
	jr z,l4cd0h
	cp 007h
	jr z,l4ce2h
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
IF SHORTMSG
	ld	l,38
	push	hl
	call	_pr_nr_of_error1
ELSE
	ld hl,l9670h
	push hl	
	call _prerror
ENDIF
	pop bc	
	pop bc	
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l4cd0h:
	ld hl,00007h
	push hl	
	push iy
	call sub_4eedh
	pop bc	
	pop bc	
	push hl	
	pop iy
	ld (iy+015h),001h
l4ce2h:
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

sub_4ce8h:
	call csv
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld a,d	
	rla	
	sbc a,a	
	ld l,a	
	ld h,a	
	push hl	
	push de	
	call sub_248ah
	pop bc	
	ex (sp),hl	
	ld hl,0007ah
	push hl	
	call sub_1441h
	pop bc	
	push hl	
	pop iy
	ex (sp),hl	
	call sub_042dh
	pop bc	
	push iy
	call sub_2569h
	jp cret

sub_4d15h:
	call csv
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jp z,cret
	ld a,(ix+00ah)
	or a	
	jr nz,l4d3ah
	push iy
	ld hl,00048h
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	push hl	
	pop iy
l4d3ah:
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld a,d	
	rla	
	sbc a,a	
	ld l,a	
	ld h,a	
	push hl	
	push de	
	call sub_248ah
	pop bc	
	ex (sp),hl	
	push iy
	ld hl,0007bh
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	push hl	
	pop iy
	ex (sp),hl	
	call sub_042dh
	pop bc	
	push iy
	call sub_2569h
	jp cret

sub_4d67h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jp z,cret
	push iy
	ld hl,00079h
	push hl	
	call sub_1441h
	pop bc	
	push hl	
	pop iy
	ex (sp),hl	
	call sub_042dh
	pop bc	
	push iy
	call sub_2569h
	jp cret

sub_4d92h:
	ld hl,00000h
	ld (buf36),hl
	ld (buf37),hl
	ld hl,l021eh
	push hl	
	call _allocmem
	pop bc	
	ld (buf38),hl
	ret
	
sub_4da7h:
	call ncsv
	defw 0fff9h
	ld (ix-006h),000h
	ld (ix-005h),000h
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (ix-004h),l
	ld (ix-003h),h
	jr l4de7h
l4dc2h:
	ld e,(ix-006h)
	ld d,(ix-005h)
	ld l,(ix-004h)
	ld h,(ix-003h)
	ld l,(hl)	
	ld h,000h
	add hl,de	
	ex de,hl	
	add hl,de	
	ld (ix-006h),l
	ld (ix-005h),h
	ld l,(ix-004h)
	ld h,(ix-003h)
	inc hl	
	ld (ix-004h),l
	ld (ix-003h),h
l4de7h:
	ld l,(ix-004h)
	ld h,(ix-003h)
	ld a,(hl)	
	or a	
	jr nz,l4dc2h
	ld de,l010fh
	ld l,(ix-006h)
	ld h,(ix-005h)
	call lmod
	add hl,hl	
	ld de,(buf38)
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	jr l4e77h
l4e0bh:
	ld a,(ix+006h)
	or (ix+007h)
	jr z,l4e6ah
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
	call _strcmp
	pop bc	
	pop bc	
	ld a,l	
	or h	
	jr nz,l4e6ah
	ld a,(iy+014h)
	ld (ix-007h),a
	cp 008h
	jr z,l4e3bh
	ld a,(ix-007h)
	cp 009h
	jr nz,l4e40h
l4e3bh:
	ld de,00001h
	jr l4e43h
l4e40h:
	ld de,00000h
l4e43h:
	ld a,(l8f85h)
	ld l,a	
	ld h,000h
	or a	
	sbc hl,de
	ld a,(ix-007h)
	jr nz,l4e67h
	cp 00ah
	ld de,00001h
	jr z,l4e59h
	dec de	
l4e59h:
	ld a,(l8f86h)
	ld l,a	
	ld h,000h
	or a	
	sbc hl,de
	jr z,l4e87h
	ld a,(ix-007h)
l4e67h:
	or a	
	jr z,l4e87h
l4e6ah:
	push iy
	pop de	
	ld hl,00008h
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
l4e77h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	pop iy
	ld a,c	
	or b	
	jr nz,l4e0bh
l4e87h:
IF OPTIM
	jp      ldLIXm2ldHIXm1cret
ELSE
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp cret
ENDIF

sub_4e90h:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	push hl	
	call sub_4da7h
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	ld a,(hl)	
	inc hl	
	or (hl)	
	jr nz,l4ebdh
	push iy
	call sub_5384h
	pop bc	
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (hl),e	
	inc hl	
	ld (hl),d	
l4ebdh:
	ld hl,(buf15)
	ld a,l	
	or h	
	jr z,l4ee0h
	push iy
	pop hl	
	ld a,l	
	or h	
	jr z,l4ee0h
	ld hl,(buf1)
	push hl	
	push iy
	ld hl,l9690h
	push hl	
	ld hl,(buf15)
	push hl	
	call _fprintf
	pop bc	
	pop bc	
	pop bc	
	pop bc	
l4ee0h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	jp cret

sub_4eedh:
	call ncsv
	defw 0fffah
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+014h)
	or a	
	jp z,l5116h
	ld a,(buf22)
	ld e,a	
	ld a,(iy+015h)
	cp e	
	jp nz,l50d7h
	ld a,(ix+008h)
	cp 00ah
	jr nz,l4f2eh
	ld a,(iy+014h)
	cp 00ah
	jp nz,l50d7h
	ld e,(ix+00ch)
	ld d,(ix+00dh)
	ld l,(iy+00ah)
	ld h,(iy+00bh)
	or a	
	sbc hl,de
	jp nz,l50d7h
l4f2eh:
	ld (ix-004h),000h
	ld (ix-003h),000h
	ld a,(ix+008h)
	cp (iy+014h)
	jr z,l4f4ah
	ld hl,l9697h
l4f41h:
	ld (ix-004h),l
	ld (ix-003h),h
	jp l4feah
l4f4ah:
	bit 4,(iy+012h)
	jp z,l4feah
	push iy
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	call sub_591dh
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr nz,l4f6ch
	ld hl,l96a5h
	ld (ix-004h),l
	ld (ix-003h),h
l4f6ch:
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	cp 002h
	jp nz,l4feah
	ld l,e	
	ld h,d	
	inc hl	
	inc hl	
	ld a,(hl)	
	inc hl	
	or (hl)	
	jr z,l4f92h
	ld a,(iy+002h)
	or (iy+003h)
	jr nz,l4f92h
	ld hl,l96aah
	jr l4f41h
l4f92h:
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	inc hl	
	inc hl	
	ld a,(hl)	
	inc hl	
	or (hl)	
	jr z,l4feah
	ld l,(iy+002h)
	ld h,(iy+003h)
	ld e,(hl)	
	inc hl	
	ld d,(hl)	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	inc hl	
	inc hl	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	or a	
	sbc hl,de
	jr z,l4fc3h
	ld hl,l96b4h
	jp l4f41h
l4fc3h:
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	inc hl	
	inc hl	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (ix-006h),c
	ld (ix-005h),b
l4fd8h:
	ld l,(ix-006h)
	ld h,(ix-005h)
	dec hl	
	ld (ix-006h),l
	ld (ix-005h),h
	inc hl	
	ld a,l	
	or h	
	jr nz,l500dh
l4feah:
	ld a,(ix-004h)
	or (ix-003h)
	jr z,l5047h
	ld l,(ix-004h)
	ld h,(ix-003h)
	push hl	
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
	ld hl,l96cfh
	push hl	
	call _prerror
	pop bc	
	pop bc	
	pop bc	
	jp l50d1h
l500dh:
	ld e,(iy+002h)
	ld d,(iy+003h)
	ld l,(ix-006h)
	ld h,(ix-005h)
	add hl,hl	
	add hl,hl	
	add hl,hl	
	add hl,de	
	inc hl	
	inc hl	
	push hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld l,(ix-006h)
	ld h,(ix-005h)
	add hl,hl	
	add hl,hl	
	add hl,hl	
	add hl,bc	
	inc hl	
	inc hl	
	push hl	
	call sub_591dh
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr nz,l4fd8h
	ld hl,l96c5h
	jp l4f41h
l5047h:
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	cp 001h
	jr nz,l5089h
	ld l,e	
	ld h,d	
	inc hl	
	inc hl	
	ld a,(hl)	
	inc hl	
	or (hl)	
	jr z,l5089h
	ld e,(iy+002h)
	ld d,(iy+003h)
	dec hl	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	or a	
	sbc hl,de
	jr z,l5089h
	ld l,e	
	ld h,d	
	push hl	
	call sub_2569h
	pop bc	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	inc hl	
	inc hl	
l507eh:
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (iy+002h),c
	ld (iy+003h),b
	jr l50d1h
l5089h:
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	cp 002h
	jr nz,l50d1h
	ld a,(iy+002h)
	or (iy+003h)
	ld l,e	
	ld h,d	
	inc hl	
	inc hl	
	jr z,l507eh
	ld a,(hl)	
	inc hl	
	or (hl)	
	jr z,l50d1h
	ld e,(iy+002h)
	ld d,(iy+003h)
	dec hl	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	or a	
	sbc hl,de
	jr z,l50d1h
	ld l,e	
	ld h,d	
	push hl	
	call sub_583ah
	pop bc	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (iy+002h),c
	ld (iy+003h),b
l50d1h:
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF
l50d7h:
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
	call sub_4da7h
	ld (ix-002h),l
	ld (ix-001h),h
	ld l,(iy+017h)
	ld h,(iy+018h)
	ex (sp),hl	
	call sub_5384h
	pop bc	
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (hl),e	
	inc hl	
	ld (hl),d	
	push iy
	pop de	
	dec hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld hl,00008h
	add hl,bc	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	pop iy
l5116h:
	ld a,(ix+008h)
	ld (iy+014h),a
	cp 007h
	jr z,l514eh
	cp 008h
	jr z,l514eh
	cp 009h
	jr z,l514eh
	cp 00ah
	jr z,l515eh
	cp 00bh
	jr z,l50d1h
	cp 00ch
	jr z,l515ah
	cp 02dh
	jr z,l513ch
	set 4,(iy+012h)
l513ch:
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push iy
	pop de	
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	jr l50d1h
l514eh:
	call sub_5785h
	ld (iy+000h),l
	ld (iy+001h),h
	jp l50d1h
l515ah:
	set 4,(iy+012h)
l515eh:
	ld l,(ix+00ch)
	ld h,(ix+00dh)
	ld (iy+00ah),l
	ld (iy+00bh),h
	jr l513ch

sub_516ch:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jp z,cret
	bit 7,(iy+012h)
	jp nz,cret
	bit 0,(iy+012h)
	jr z,l519ah
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
IF SHORTMSG
	ld	l,37
	push	hl
	call	_pr_nr_of_error1
ELSE
	ld hl,l96e1h
	push hl	
	call _prerror
ENDIF
	pop bc	
	pop bc	
l519ah:
	set 0,(iy+012h)
	ld hl,(buf15)
	ld a,l	
	or h	
	jp z,cret
	ld a,(iy+017h)
	or (iy+018h)
	jp z,cret
	bit 7,(iy+012h)
	jp nz,cret
	ld hl,(buf1)
	push hl	
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
	ld hl,l96fah
	push hl	
	ld hl,(buf15)
	push hl	
	call _fprintf
	jp cret

sub_51cfh:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jp z,cret
	set 1,(iy+012h)
	jp cret

sub_51e7h:
	call ncsv
	defw 0fffah
	ld hl,(buf11)
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (ix-006h),c
	ld (ix-005h),b
	ld iy,(buf32)
	jr l5252h
l5200h:
	ld l,(iy+012h)
	ld h,(iy+013h)
	ld a,l	
	and 0d7h
	ld l,a	
	ld (iy+012h),l
	ld a,(ix-006h)
	or (ix-005h)
	jr z,l5219h
	set 1,(iy+013h)
l5219h:
	ld a,(iy+007h)
	cp 001h
	jr nz,l523dh
	ld l,(iy+002h)
	ld h,(iy+003h)
	push hl	
	call sub_2569h
	pop bc	
	ld (iy+002h),000h
	ld (iy+003h),000h
	ld (iy+007h),000h
	push iy
	call sub_5be1h
	pop bc	
l523dh:
	push iy
	call sub_516ch
	pop bc	
	push iy
	call sub_0493h
	pop bc	
	ld l,(iy+00ah)
	ld h,(iy+00bh)
	push hl	
	pop iy
l5252h:
	push iy
	pop hl	
	ld a,l	
	or h	
	jr nz,l5200h
	ld a,(ix-006h)
	or (ix-005h)
	jp z,cret
	ld (ix-004h),000h
	ld (ix-003h),000h
	ld l,(ix-006h)
	ld h,(ix-005h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (ix-002h),c
	ld (ix-001h),b
	ld de,00001h
	ld l,c	
	ld h,b	
	or a	
	sbc hl,de
	jr nz,l52a1h
	ld hl,00014h
	push hl	
	ld l,(ix-006h)
	ld h,(ix-005h)
	inc hl	
	inc hl	
	push hl	
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr z,l52a1h
	ld hl,(buf32)
	ld a,l	
	or h	
	jp z,cret
l52a1h:
	ld iy,(buf32)
	jp l5303h
l52a8h:
	ld e,(ix-006h)
	ld d,(ix-005h)
	ld l,(ix-004h)
	ld h,(ix-003h)
	add hl,hl	
	add hl,hl	
	add hl,hl	
	add hl,de	
	ld de,00008h
	add hl,de	
	ld a,(hl)	
	cp 018h
	jr nz,l52cfh
	ld iy,00000h
	ld (ix-002h),000h
	ld (ix-001h),000h
	jr l531ch
l52cfh:
	push iy
	ld e,(ix-006h)
	ld d,(ix-005h)
	ld l,(ix-004h)
	ld h,(ix-003h)
	add hl,hl	
	add hl,hl	
	add hl,hl	
	add hl,de	
	inc hl	
	inc hl	
	push hl	
	call sub_591dh
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr z,l531ch
	ld l,(iy+00ah)
	ld h,(iy+00bh)
	push hl	
	pop iy
	ld l,(ix-004h)
	ld h,(ix-003h)
	inc hl	
	ld (ix-004h),l
	ld (ix-003h),h
l5303h:
	push iy
	pop hl	
	ld a,l	
	or h	
	jr z,l531ch
	ld l,(ix-002h)
	ld h,(ix-001h)
	dec hl	
	ld (ix-002h),l
	ld (ix-001h),h
	inc hl	
	ld a,l	
	or h	
	jr nz,l52a8h
l531ch:
	push iy
	pop hl	
	ld a,l	
	or h	
	jr nz,l5344h
	ld a,(ix-002h)
	or (ix-001h)
	jr z,l534bh
	ld e,(ix-006h)
	ld d,(ix-005h)
	ld l,(ix-004h)
	ld h,(ix-003h)
	add hl,hl	
	add hl,hl	
	add hl,hl	
	add hl,de	
	ld de,00008h
	add hl,de	
	ld a,(hl)	
	cp 018h
	jr z,l534bh
l5344h:
IF SHORTMSG
	ld	l,36
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l9702h
	push hl	
	call _prerror
ENDIF
l534bh:
	ld (ix-002h),001h
	ld (ix-001h),000h
	jp cret

sub_5356h:
	call csv
	ld hl,(buf37)
	ld a,l	
	or h	
	jr nz,l5374h
IF OPTIM
	jp      ldL0cret
ELSE
	ld l,000h
	jp cret
ENDIF
l5365h:
	ld l,(iy+00ah)
	ld h,(iy+00bh)
	ld (buf37),hl
	push iy
	call _free
	pop bc	
l5374h:
	ld iy,(buf37)
	push iy
	pop hl	
	ld a,l	
	or h	
	jr nz,l5365h
IF OPTIM
	jp      ldL1cret
ELSE
	ld l,001h
	jp cret
ENDIF

sub_5384h:
	call csv
	ld hl,(buf37)
	ld a,l	
	or h	
	jr z,l53a8h
	ld iy,(buf37)
	ld l,(iy+00ah)
	ld h,(iy+00bh)
	ld (buf37),hl
	ld hl,00019h
	push hl	
	push iy
	call _blkclr
	pop bc	
	pop bc	
	jr l53b3h
l53a8h:
	ld hl,00019h
	push hl	
	call _allocmem
	pop bc	
	push hl	
	pop iy
l53b3h:
	ld a,(buf22)
	ld (iy+015h),a
	ld (iy+016h),001h
	ld hl,(l968ch)
	inc hl	
	ld (l968ch),hl
	ld (iy+00ch),l
	ld (iy+00dh),h
	ld a,(ix+006h)
	or (ix+007h)
	jr z,l53fdh
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call _strlen
	pop bc	
	inc hl	
	push hl	
	call _allocmem
	ld (iy+017h),l
	ld (iy+018h),h
	ld l,(ix+006h)
	ld h,(ix+007h)
	ex (sp),hl	
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
	call _strcpy
	pop bc	
	pop bc	
	jr l5406h
l53fdh:
	ld hl,l968bh
	ld (iy+017h),l
	ld (iy+018h),h
l5406h:
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

sub_540ch:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+016h)
	add a,0ffh
	ld (iy+016h),a
	or a	
	jp nz,cret
	ld a,(iy+014h)
	or a	
	jp z,l5474h
	cp 007h
	jp z,l5474h
	cp 008h
	jr z,l5474h
	cp 009h
	jr z,l5474h
	cp 00bh
	jr z,l5474h
	ld a,(iy+007h)
	cp 002h
	jr nz,l5450h
	ld l,(iy+002h)
	ld h,(iy+003h)
	push hl	
	call sub_583ah
	pop bc	
	jr l5462h
l5450h:
	ld a,(iy+007h)
	cp 001h
	jr nz,l5462h
	ld l,(iy+002h)
	ld h,(iy+003h)
	push hl	
	call sub_2569h
	pop bc	
l5462h:
	ld a,(iy+006h)
	cp 016h
	jr nz,l5474h
	ld l,(iy+000h)
	ld h,(iy+001h)
	push hl	
	call sub_540ch
	pop bc	
l5474h:
	ld de,l968bh
	ld l,(iy+017h)
	ld h,(iy+018h)
	or a	
	sbc hl,de
	jr z,l548ch
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
	call _free
l548ch:
	ld hl,(buf37)
	ld (iy+00ah),l
	ld (iy+00bh),h
	ld (buf37),iy
	jp cret

sub_549ch:
	ld hl,00004h
	push hl	
	call sub_022eh
	pop bc	
	ld a,(buf22)
	inc a	
	ld (buf22),a
	ret
	
sub_54ach:
	call sub_54c0h
	ld a,(buf22)
	sub 001h
	ld (buf22),a
	ld hl,00003h
	push hl	
	call sub_022eh
	pop bc	
	ret	

sub_54c0h:
	call ncsv
	defw 0fff9h
	ld hl,(buf38)
	ld (ix-002h),l
	ld (ix-001h),h
	jp l5643h
l54d1h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (ix-004h),l
	ld (ix-003h),h
	jp l5624h
l54e0h:
	ld a,(buf22)
	ld e,a	
	ld a,(iy+015h)
	cp e	
	jp nz,l5617h
	ld (ix-007h),000h
	ld (ix-006h),000h
	ld a,(iy+014h)
	ld (ix-005h),a
	ld de,00002h
	ld a,(iy+012h)
	and 003h
	ld l,a	
	xor a	
	ld h,a	
	or a	
	sbc hl,de
	jp nz,l5555h
	ld a,(ix-005h)
	cp 007h
	jr z,l554ah
	cp 008h
	jr z,l5526h
	cp 009h
	jr z,l5526h
	cp 01fh
	jr z,l5526h
	ld hl,l972fh
	ld (ix-007h),l
	ld (ix-006h),h
l5526h:
	ld a,(ix-007h)
	or (ix-006h)
	jp z,l55d2h
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
	ld l,(ix-007h)
	ld h,(ix-006h)
	push hl	
	ld hl,l9738h
	push hl	
	call _prerror
	pop bc	
	pop bc	
	pop bc	
	jp l55d2h
l554ah:
	ld hl,l9729h
	ld (ix-007h),l
	ld (ix-006h),h
	jr l5526h
l5555h:
	ld a,(buf22)
	or a	
	jr nz,l5563h
	ld a,(ix-005h)
	cp 02ah
	jp nz,l55d2h
l5563h:
	bit 1,(iy+012h)
	jp nz,l55d2h
	ld a,(ix-005h)
	cp 006h
	jp z,l5608h
	cp 007h
	jr z,l55a9h
	cp 008h
	jp z,l55eah
	cp 009h
	jp z,l55efh
	cp 00ah
	jp z,l55f4h
	cp 00bh
	jp z,l55f9h
	cp 00ch
	jp z,l55feh
	cp 02dh
	jp z,l5603h
	or a	
	jr z,l55b2h
	bit 0,(iy+012h)
	jp z,l5612h
	ld hl,l977ch
l55a1h:
	ld (ix-007h),l
	ld (ix-006h),h
	jr l55b2h
l55a9h:
	ld hl,l9749h
	ld (ix-007h),l
	ld (ix-006h),h
l55b2h:
	ld a,(ix-007h)
	or (ix-006h)
	jr z,l55d2h
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
	ld l,(ix-007h)
	ld h,(ix-006h)
	push hl	
	ld hl,l97a5h
	push hl	
	call _prwarning
	pop bc	
	pop bc	
	pop bc	
l55d2h:
	ld e,(iy+008h)
	ld d,(iy+009h)
	ld l,(ix-004h)
	ld h,(ix-003h)
	ld (hl),e	
	inc hl	
	ld (hl),d	
	push iy
	call sub_540ch
	pop bc	
	jp l5624h
l55eah:
	ld hl,l974fh
	jr l55a1h
l55efh:
	ld hl,l9759h
	jr l55a1h
l55f4h:
	ld hl,l975fh
	jr l55a1h
l55f9h:
	ld hl,l9766h
	jr l55a1h
l55feh:
	ld hl,l976bh
	jr l55a1h
l5603h:
	ld hl,l9774h
	jr l55a1h
l5608h:
	ld (ix-007h),000h
	ld (ix-006h),000h
	jr l55b2h
l5612h:
	ld hl,l9790h
	jr l55a1h
l5617h:
	push iy
	pop de	
	ld hl,00008h
	add hl,de	
	ld (ix-004h),l
	ld (ix-003h),h
l5624h:
	ld l,(ix-004h)
	ld h,(ix-003h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	pop iy
	ld a,c	
	or b	
	jp nz,l54e0h
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
l5643h:
	ld de,(buf38)
	ld hl,l021eh
	add hl,de	
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	call wrelop
	jp c,l54d1h
	ld hl,buf36
	ld (ix-004h),l
	ld (ix-003h),h
	jr l5691h
l5663h:
	ld a,(buf22)
	ld e,a	
	ld a,(iy+015h)
	cp e	
	jr nz,l5684h
	ld e,(iy+008h)
	ld d,(iy+009h)
	ld l,(ix-004h)
	ld h,(ix-003h)
	ld (hl),e	
	inc hl	
	ld (hl),d	
	push iy
	call sub_540ch
	pop bc	
	jr l5691h
l5684h:
	push iy
	pop de	
	ld hl,00008h
	add hl,de	
	ld (ix-004h),l
	ld (ix-003h),h
l5691h:
	ld l,(ix-004h)
	ld h,(ix-003h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	pop iy
	ld a,c	
	or b	
	jr nz,l5663h
	jp cret

sub_56a4h:
	call csv
	ld hl,00000h
	push hl	
	call sub_5384h
	pop bc	
	push hl	
	pop iy
	ld a,(iy+012h)
	or 083h
	ld (iy+012h),a
	ld hl,(buf36)
	ld (iy+008h),l
	ld (iy+009h),h
	ld (buf36),iy
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

sub_56cdh:
	call csv
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,0000ah
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	pop iy
l56e0h:
	push iy
	pop de	
	ld l,(ix+006h)
	ld h,(ix+007h)
	or a	
	sbc hl,de
	jr nz,l5713h
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,00017h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld hl,l97b3h
	push hl	
	call _prerror
	pop bc	
	pop bc	
	pop bc	
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l5713h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
	call _strcmp
	pop bc	
	pop bc	
	ld a,l	
	or h	
	jr nz,l5730h
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF
l5730h:
	ld l,(iy+00ah)
	ld h,(iy+00bh)
	push hl	
	pop iy
	jr l56e0h

sub_573bh:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jp z,cret
	bit 7,(iy+012h)
	jr z,l576dh
	ld l,(iy+00ch)
	ld h,(iy+00dh)
	push hl	
	ld hl,l97ddh
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	call _fprintf
	pop bc	
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF
l576dh:
	ld l,(iy+017h)
	ld h,(iy+018h)
	push hl	
	ld hl,l97e1h
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	call _fprintf
	jp cret

sub_5785h:
	ld hl,(l968eh)
	inc hl	
	ld (l968eh),hl
	ret
	
sub_578dh:
	call ncsv
	defw 0fffah
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jr nz,l57a5h
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF
l57a5h:
	ld e,(iy+000h)
	ld d,(iy+001h)
	ld hl,0ffffh
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,0000ah
	add hl,de	
	push hl	
	call _allocmem
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	ld e,(iy+000h)
	ld d,(iy+001h)
	ld (hl),e	
	inc hl	
	ld (hl),d	
	ld (ix-006h),e
	ld (ix-005h),d
	jr l581fh
l57d2h:
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld l,(ix-006h)
	ld h,(ix-005h)
	add hl,hl	
	add hl,hl	
	add hl,hl	
	add hl,de	
	inc hl	
	inc hl	
	ld (ix-004h),l
	ld (ix-003h),h
	push iy
	pop de	
	ld l,(ix-006h)
	ld h,(ix-005h)
	add hl,hl	
	add hl,hl	
	add hl,hl	
	add hl,de	
	inc hl	
	inc hl	
	ld e,(ix-004h)
	ld d,(ix-003h)
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	ld e,(ix-004h)
	ld d,(ix-003h)
	ld hl,00006h
	add hl,de	
	ld a,(hl)	
	cp 016h
	jr nz,l581fh
	ld l,e	
	ld h,d	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld hl,00016h
	add hl,bc	
	inc (hl)	
l581fh:
	ld l,(ix-006h)
	ld h,(ix-005h)
	dec hl	
	ld (ix-006h),l
	ld (ix-005h),h
	inc hl	
	ld a,l	
	or h	
	jr nz,l57d2h
IF OPTIM
	jp      ldLIXm2ldHIXm1cret
ELSE
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp cret
ENDIF
sub_583ah:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jp z,cret
	push iy
	pop hl	
	inc hl	
	inc hl	
	ld (ix-002h),l
	ld (ix-001h),h
	jr l58a3h
l5859h:
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	cp 002h
	jr nz,l587ah
	ld l,e	
	ld h,d	
	inc hl	
	inc hl	
	ld a,(hl)	
	inc hl	
	or (hl)	
	jr z,l587ah
	dec hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	call sub_583ah
	pop bc	
l587ah:
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00006h
	add hl,de	
	ld a,(hl)	
	cp 016h
	jr nz,l5893h
	ld l,e	
	ld h,d	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	call sub_540ch
	pop bc	
l5893h:
	ld de,00008h
	ld l,(ix-002h)
	ld h,(ix-001h)
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
l58a3h:
	ld l,(iy+000h)
	ld h,(iy+001h)
	dec hl	
	ld (iy+000h),l
	ld (iy+001h),h
	inc hl	
	ld a,l	
	or h	
	jr nz,l5859h
	push iy
	call _free
	jp cret

sub_58bdh:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld e,(ix+008h)
	ld d,(ix+009h)
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	cp 001h
	jr nz,l5900h
	ld l,e	
	ld h,d	
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	call sub_21c7h
	pop bc	
l58f1h:
	ex de,hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	inc hl	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	jp cret
l5900h:
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	cp 002h
	jp nz,cret
	ld l,e	
	ld h,d	
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	call sub_578dh
	jr l58f1h

sub_591dh:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	push hl	
	pop de	
	ld l,(ix+008h)
	ld h,(ix+009h)
	or a	
	sbc hl,de
	jr nz,l593ch
l5937h:
IF OPTIM
	jp      ldL1cret
ELSE
	ld l,001h
	jp cret
ENDIF
l593ch:
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld hl,00007h
	add hl,de	
	ld a,(iy+007h)
	cp (hl)	
	jr z,l5951h
l594ch:
IF OPTIM
	jp      ldL0cret
ELSE
	ld l,000h
	jp cret
ENDIF
l5951h:
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld hl,00006h
	add hl,de	
	ld a,(iy+006h)
	cp (hl)	
	jr nz,l594ch
	ld l,e	
	ld h,d	
	inc hl	
	inc hl	
	inc hl	
	inc hl	
	ld e,(hl)	
	inc hl	
	ld d,(hl)	
	ld l,(iy+004h)
	ld h,(iy+005h)
	or a	
	sbc hl,de
	jr nz,l594ch
	cp 012h
	jr z,l5985h
	cp 016h
	jr z,l59a1h
	cp 01ah
	jr z,l5985h
	cp 01ch
	jr nz,l59bah
l5985h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld e,(hl)	
	inc hl	
	ld d,(hl)	
	ld l,(iy+000h)
	ld h,(iy+001h)
	or a	
	sbc hl,de
	ld hl,00001h
	jp z,cret
	dec hl	
	jp cret
l59a1h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	push bc	
	ld l,(iy+000h)
	ld h,(iy+001h)
	push hl	
	call sub_591dh
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF
l59bah:
	ld a,(iy+007h)
	cp 002h
	jp nz,l5937h
	ld a,(iy+002h)
	or (iy+003h)
	jp z,l5937h
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	inc hl	
	ld a,(hl)	
	inc hl	
	or (hl)	
	jp z,l5937h
	dec hl	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	ld e,(hl)	
	inc hl	
	ld d,(hl)	
	ld l,(iy+002h)
	ld h,(iy+003h)
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	or a	
	sbc hl,de
	jp nz,l594ch
	ld l,(iy+002h)
	ld h,(iy+003h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (ix-002h),c
	ld (ix-001h),b
l5a00h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	dec hl	
	ld (ix-002h),l
	ld (ix-001h),h
	inc hl	
	ld a,l	
	or h	
	jp z,l5937h
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld l,(ix-002h)
	ld h,(ix-001h)
	add hl,hl	
	add hl,hl	
	add hl,hl	
	add hl,bc	
	inc hl	
	inc hl	
	push hl	
	ld e,(iy+002h)
	ld d,(iy+003h)
	ld l,(ix-002h)
	ld h,(ix-001h)
	add hl,hl	
	add hl,hl	
	add hl,hl	
	add hl,de	
	inc hl	
	inc hl	
	push hl	
	call sub_591dh
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr nz,l5a00h
	jp l594ch

sub_5a4ah:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+006h)
	cp 014h
	jr nz,l5a70h
IF OPTIM
	ld      a,(iy+004h)
	cp      01h
ELSE
	ld de,00001h
	ld l,(iy+004h)
	ld h,(iy+005h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l5a70h
	ld l,e	
	ld h,d	
	jp cret
l5a70h:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF

sub_5a76h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+006h)
	cp (ix+008h)
	jr nz,l5a98h
	ld a,(iy+004h)
	or (iy+005h)
	jr nz,l5a98h
	ld a,(iy+007h)
	or a	
	jr z,l5a9eh
l5a98h:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l5a9eh:
IF OPTIM
	jp      ldHL1cret
ELSE
	ld hl,00001h
	jp cret
ENDIF

sub_5aa4h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+007h)
	or a	
	jr nz,l5acfh
	bit 0,(iy+004h)
	jr z,l5abfh
	or a	
	jr z,l5ac9h
l5abfh:
	ld b,014h
	ld a,(iy+006h)
	call brelop
	jr nc,l5acfh
l5ac9h:
IF OPTIM
	jp      ldHL1cret
ELSE
	ld hl,00001h
	jp cret
ENDIF
l5acfh:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF

sub_5ad5h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+007h)
	or a	
	jr nz,l5afch
	ld a,(iy+004h)
	or (iy+005h)
	jr nz,l5afch
	ld e,(iy+006h)
	ld d,000h
	ld hl,00012h
	call wrelop
	jr nc,l5b02h
l5afch:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l5b02h:
IF OPTIM
	jp      ldHL1cret
ELSE
	ld hl,00001h
	jp cret
ENDIF

sub_5b08h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+007h)
	or a	
	jr nz,l5b2ch
	ld a,(iy+004h)
	or (iy+005h)
	jr nz,l5b2ch
	ld b,00eh
	ld a,(iy+006h)
	call brelop
	jr c,l5b32h
l5b2ch:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l5b32h:
IF OPTIM
	jp      ldHL1cret
ELSE
	ld hl,00001h
	jp cret
ENDIF

sub_5b38h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+007h)
	or a	
	jr nz,l5b5dh
	ld a,(iy+004h)
	or (iy+005h)
	jr nz,l5b5dh
	ld a,(iy+006h)
	cp 00eh
	jr z,l5b63h
	cp 010h
	jr z,l5b63h
l5b5dh:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l5b63h:
IF OPTIM
	jp      ldHL1cret
ELSE
	ld hl,00001h
	jp cret
ENDIF

sub_5b69h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	push hl	
	call sub_5b08h
	pop bc	
	ld a,l	
	or a	
	jr nz,l5b8dh
	ld hl,00012h
	push hl	
	push iy
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr z,l5b93h
l5b8dh:
IF OPTIM
	jp      ldHL1cret
ELSE
	ld hl,00001h
	jp cret
ENDIF
l5b93h:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF

sub_5b99h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+007h)
	cp 002h
	jr nz,l5bb2h
	ld (iy+007h),000h
	jr l5bbeh
l5bb2h:
	ld b,001h
	push iy
	pop hl	
	inc hl	
	inc hl	
	inc hl	
	inc hl	
	call asar
l5bbeh:
	ld hl,00016h
	push hl	
	push iy
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jp z,cret
	ld l,(iy+000h)
	ld h,(iy+001h)
	push iy
	pop de	
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	jp cret

sub_5be1h:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld l,(iy+004h)
	ld h,(iy+005h)
	ld (ix-002h),l
	ld (ix-001h),h
	bit 7,(ix-001h)
	jr z,l5c07h
IF SHORTMSG
	ld	l,35
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l980fh
	push hl	
	call _prerror
ENDIF
l5c07h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	add hl,hl	
	set 0,l
	ld (iy+004h),l
	ld (iy+005h),h
	jp cret

sub_5c19h:
	call csv
	push hl	
	ld a,(ix+006h)
	ld (buf39),a
l5c23h:
	call sub_2671h
	ld a,l	
	ld (ix-001h),a
	ld (buf7),a
	cp 005h
	jr z,l5c44h
	cp 077h
	jr z,l5c44h
	cp 073h
	jr nz,l5c49h
	ld iy,(buf8)
	ld a,(iy+014h)
	cp 02dh
	jr nz,l5c49h
l5c44h:
	call sub_5c50h
	jr l5c23h
l5c49h:
	xor a	
	ld (buf39),a
	jp cret

sub_5c50h:
	call ncsv
	defw 0fff3h
	push ix
	pop de	
	ld hl,0fff7h
	add hl,de	
	push hl	
	push ix
	pop hl	
	dec hl	
	push hl	
	call sub_5dd1h
	pop bc	
	pop bc	
	ld e,l	
	ld (ix-00bh),e
	ld a,(ix-001h)
	cp 006h
	jr z,l5c89h
	cp 026h
	jr z,l5c89h
	ld a,(buf39)
	cp 006h
	jr nz,l5c89h
IF SHORTMSG
	ld	l,34
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l9824h
	push hl	
	call _prerror
ENDIF
	pop bc	
	ld (ix-001h),006h
l5c89h:
	call sub_2671h
	ld a,l	
	ld (ix-00ah),a
	cp 002h
	jp z,cret
	ld (buf7),a
l5c98h:
	ld l,(ix-00bh)
	ld h,000h
	ld a,l	
	and 0feh
	ld l,a	
	ld a,h	
	push hl	
	push ix
	pop de	
	ld hl,0fff7h
	add hl,de	
	push hl	
	ld l,(ix-001h)
	ld h,a	
	push hl	
	call sub_69cah
	pop bc	
	pop bc	
	pop bc	
	push hl	
	pop iy
	ld a,l	
	or h	
	jr z,l5ccah
	bit 4,(iy+012h)
	jr z,l5ccah
	ld a,(iy+007h)
	cp 002h
	jr z,l5cd8h
l5ccah:
	ld hl,00000h
l5ccdh:
	ld (ix-00dh),l
	bit 0,(ix-00bh)
	jr nz,l5cddh
	jr l5ceah
l5cd8h:
	ld hl,00001h
	jr l5ccdh
l5cddh:
	ld a,(ix-001h)
	cp 006h
	jr z,l5ceah
	ld a,(ix-00dh)
	or a	
	jr z,l5d04h
l5ceah:
	ld hl,00000h
l5cedh:
	ld (ix-00ch),l
	call sub_2671h
	ld a,l	
	ld (ix-00ah),a
	cp 064h
	jp nz,l5d6bh
	ld a,(ix-00dh)
	or a	
	jr z,l5d09h
	jr l5d18h
l5d04h:
	ld hl,00001h
	jr l5cedh
l5d09h:
	ld a,(ix-001h)
	cp 006h
	jr z,l5d18h
	cp 01fh
	jr z,l5d18h
	cp 02dh
	jr nz,l5d20h
l5d18h:
IF SHORTMSG
	ld	l,33
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l9848h
	push hl	
	call _prerror
ENDIF
	pop bc	
l5d20h:
	ld a,(ix-001h)
	cp 02ah
	jr z,l5d2bh
	cp 01fh
	jr nz,l5d50h
l5d2bh:
	push iy
	call sub_516ch
	pop bc	
	ld a,(buf22)
	or a	
	jr z,l5d42h
	ld a,(ix-001h)
	cp 02ah
	jr nz,l5d42h
	set 7,(iy+012h)
l5d42h:
	push iy
	call sub_0493h
	pop bc	
	push iy
	call sub_3c7eh
	pop bc	
	jr l5d62h
l5d50h:
	push iy
	call sub_516ch
	pop bc	
	push iy
	call sub_0493h
	pop bc	
	push iy
	call sub_6531h
	pop bc	
l5d62h:
	call sub_2671h
	ld e,l	
	ld (ix-00ah),e
	jr l5d95h
l5d6bh:
	ld a,(ix-001h)
	cp 006h
	jr z,l5d95h
	ld a,(ix-00ch)
	or a	
	jr z,l5d7eh
	push iy
	call sub_516ch
	pop bc	
l5d7eh:
	ld a,(buf22)
	or a	
	jr z,l5d8fh
	ld a,(ix-001h)
	cp 02ah
	jr nz,l5d8fh
	set 7,(iy+012h)
l5d8fh:
	push iy
	call sub_0493h
	pop bc	
l5d95h:
	ld a,(ix-00ah)
	cp 073h
	jr z,l5da0h
	cp 050h
	jr nz,l5db1h
l5da0h:
	ld hl,l985fh
	push hl	
	call _errexpected
	pop bc	
	ld a,(ix-00ah)
	ld (buf7),a
	jp l5c98h
l5db1h:
	ld a,(ix-00ah)
	cp 071h
	jp z,l5c98h
	cp 002h
	jp z,cret
	ld hl,l9861h
	push hl	
	call _errexpected
	ld l,(ix-00ah)
	ld h,000h
	ex (sp),hl	
	call sub_3610h
	jp cret

