include p1.inc

	psect   text

	global  sub_1441h
	global  sub_1b4bh
	global  sub_1b70h
	global  sub_1bf7h
	global  sub_1ccch
	global  sub_1d02h
	global  sub_1f5dh
	global  sub_2105h
	global  sub_2157h
	global  _callocm1
	global  sub_21c7h
	global  sub_23b4h
	global  sub_240eh
	global  sub_245dh
	global  sub_248ah
	global  sub_24b6h
	global  sub_24e7h
	global  sub_250ah
	global  sub_2529h
	global  sub_255dh
	global  sub_2569h
	global  sub_25f7h
	global  sub_2671h
	global l22ech
	global l2310h
	global l235ch
	global l2330h
	global l2387h
	global l2396h
	global l23a5h
IF SHORTMSG
	global _pr_nr_of_error0
	global _pr_nr_of_error1
	global _pr_nr_of_warning0
	global _pr_nr_of_warning1
ENDIF

sub_1441h:
	call ncsv
	defw 0ffedh
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	pop iy
	ld e,(ix+006h)
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
	jr nz,l1467h
	dec hl	
l1467h:
	ld (ix-00dh),l
	ld e,(ix+006h)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9271h
	add hl,de	
	ld (ix-012h),l
	ld (ix-011h),h
	push iy
	pop hl	
	ld a,l	
	or h	
	jr z,l1496h
	ld a,(ix-00dh)
	or a	
	jp z,l14b3h
	ld a,(ix+00ah)
	or (ix+00bh)
	jr nz,l14b3h
l1496h:
	push iy
	call sub_2569h
	pop bc	
	ld a,(ix-00dh)
	or a	
	jr z,l14adh
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	call sub_2569h
	pop bc	
l14adh:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l14b3h:
	ld (ix-00eh),000h
	ld e,(ix+006h)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9276h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld (ix-010h),c
	ld (ix-00fh),b
	ld a,(ix+006h)
	cp 046h
	jp nz,l14f8h
	ld a,(iy+000h)
	cp 073h
	jr nz,l14f8h
	ld e,(iy+001h)
	ld d,(iy+002h)
	ld hl,00012h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 2,c
	jr z,l14f8h
IF SHORTMSG
	ld	l,66
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8d40h
	push hl	
	call _prerror
ENDIF
	pop bc	
l14f8h:
	bit 0,(ix-00fh)
	jr nz,l1507h
	push iy
	call sub_1e37h
	pop bc	
	push hl	
	pop iy
l1507h:
	ld a,(ix-00dh)
	or a	
	jr z,l1524h
	bit 3,(ix-00fh)
	jr nz,l1524h
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	call sub_1e37h
	pop bc	
	ld (ix+00ah),l
	ld (ix+00bh),h
l1524h:
	ld a,(ix+006h)
	cp 03dh
	jp nz,l1549h
	bit 0,(iy+009h)
	jp z,l1552h
	ld a,(iy+00ch)
	or a	
	jr nz,l1552h
	push iy
	ld hl,00045h
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	push hl	
	pop iy
	jr l1552h
l1549h:
	push iy
	call sub_1df0h
	pop bc	
	push hl	
	pop iy
l1552h:
	ld a,(ix-00dh)
	or a	
	jr z,l1569h
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	call sub_1df0h
	pop bc	
	ld (ix+00ah),l
	ld (ix+00bh),h
l1569h:
	bit 5,(ix-00fh)
	jp z,l15b1h
	push iy
	call sub_1ef1h
	pop bc	
	ld a,l	
	or a	
	jp nz,l15b1h
	ld a,(ix+006h)
	cp 046h
	jp nz,l15a9h
	ld a,(iy+000h)
	cp 046h
	jr nz,l159fh
	ld e,(iy+001h)
	ld d,(iy+002h)
	ld hl,0000ch
	add hl,de	
	ld a,(hl)	
	cp 001h
	jr nz,l159fh
l1599h:
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF
l159fh:
IF SHORTMSG
	ld	l,65
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8d68h
	push hl	
	call _prerror
ENDIF
	pop bc	
	jr l15b1h
l15a9h:
IF SHORTMSG
	ld	l,64
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8d80h
	push hl	
	call _prerror
ENDIF
	pop bc	
l15b1h:
	bit 6,(ix-00fh)
	jp z,l15cch
	bit 0,(iy+009h)
	jr z,l15c4h
	ld a,(iy+00ch)
	or a	
	jr z,l15cch
l15c4h:
IF SHORTMSG
	ld	l,63
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8dach
	push hl	
	call _prerror
ENDIF
	pop bc	
l15cch:
	ld a,(ix-010h)
	and 003h
	ld l,a	
	xor a	
	ld h,a	
	ld a,l	
	or h	
	jp nz,l1632h
	ld hl,00002h
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
	jr z,l15fch
	ld hl,00008h
	push hl	
	push iy
	call sub_1ccch
	pop bc	
	pop bc	
	push hl	
	pop iy
l15fch:
	ld a,(ix-00dh)
	or a	
	jp z,l1632h
	ld hl,00002h
	push hl	
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jp z,l1632h
	ld hl,00008h
	push hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	call sub_1ccch
	pop bc	
	pop bc	
	ld (ix+00ah),l
	ld (ix+00bh),h
l1632h:
	ld a,(ix+006h)
	cp 03ch
	jp z,l1599h
	cp 043h
	jp z,l167bh
	cp 079h
	jp z,l1691h
l1644h:
	ld a,(ix-010h)
	and 003h
	ld l,a	
	xor a	
	ld h,a	
	ld a,l	
	or h	
	jp z,l16e1h
	bit 1,(ix-010h)
	jr z,l1660h
	push iy
	call sub_1b94h
	pop bc	
	push hl	
	pop iy
l1660h:
	bit 0,(ix-010h)
	jp z,l1ad1h
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	call sub_1b94h
l1671h:
	pop bc	
	ld (ix+00ah),l
	ld (ix+00bh),h
	jp l1ad1h
l167bh:
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	push iy
	call sub_1340h
	pop bc	
	pop bc	
	ld (ix+00ah),l
	ld (ix+00bh),h
	jr l1644h
l1691h:
	push ix
	pop de	
	ld hl,0fff8h
	add hl,de	
	ex de,hl	
	ld hl,(buf11)
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	ld (ix-001h),000h
	ld hl,00014h
	push hl	
	push ix
	pop de	
	ld hl,0fff8h
	add hl,de	
	push hl	
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jr z,l16c7h
IF SHORTMSG
	ld	l,62
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8dbdh
	push hl	
	call _prerror
ENDIF
	pop bc	
	jp l1644h
l16c7h:
	ld hl,00001h
	push hl	
	push ix
	pop de	
	ld hl,0fff8h
	add hl,de	
	push hl	
	push iy
	call sub_1f5dh
	pop bc	
	pop bc	
	pop bc	
	push hl	
	pop iy
	jp l1644h
l16e1h:
	bit 2,(ix-010h)
	jp z,l1740h
	bit 0,(iy+009h)
	jp z,l1740h
	ld a,(iy+00ch)
	or a	
	jp nz,l1740h
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5b08h
	pop bc	
	ld a,l	
	or a	
	jp z,l1740h
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,0000bh
	add hl,de	
	bit 0,(hl)
	jr nz,l171dh
	ld hl,0000ch
	jr l1720h
l171dh:
	ld hl,0000dh
l1720h:
	push hl	
	push iy
	call sub_1ebdh
	ex (sp),hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
l172dh:
	push hl	
	ld hl,00050h
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	pop bc	
l1738h:
	push hl	
	call sub_1ccch
	pop bc	
	jp l1671h
l1740h:
	ld a,(ix+006h)
	cp 055h
	jp nz,l17abh
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00009h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 0,c
	jp z,l17abh
	ld hl,0000ch
	add hl,de	
	ld a,(hl)	
	or a	
	jp nz,l17abh
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5b08h
	pop bc	
	ld a,l	
	or a	
	jp z,l17abh
	push iy
	pop hl	
	ld (ix-00ah),l
	ld (ix-009h),h
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	pop iy
	ld e,(ix-00ah)
	ld d,(ix-009h)
	ld hl,0000bh
	add hl,de	
	bit 0,(hl)
	jr nz,l1799h
	ld hl,0000ch
	jr l179ch
l1799h:
	ld hl,0000dh
l179ch:
	push hl	
	push iy
	call sub_1ebdh
	ex (sp),hl	
	ld l,(ix-00ah)
	ld h,(ix-009h)
	jr l172dh
l17abh:
	bit 3,(ix-010h)
	jp z,l187ah
	bit 0,(iy+009h)
	jp z,l187ah
	ld a,(iy+00ch)
	or a	
	jp nz,l187ah
	ld a,(ix-00dh)
	or a	
	jp z,l17e2h
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00009h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 0,c
	jp z,l187ah
	ld hl,0000ch
	add hl,de	
	ld a,(hl)	
	or a	
	jp nz,l187ah
l17e2h:
	xor a	
	ld l,a	
	ld a,(ix-00fh)
	and 080h
	ld h,a	
	ld a,l	
	or h	
	jp z,l1814h
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5a4ah
	pop bc	
	ld a,l	
	or a	
	jp nz,l1ad1h
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5a4ah
	pop bc	
	ld a,l	
	or a	
	jp nz,l1ad1h
l1814h:
	ld a,(ix-00dh)
	or a	
	jp z,l184bh
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_591dh
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jp nz,l184bh
	ld l,(ix-012h)
	ld h,(ix-011h)
	push hl	
IF SHORTMSG
	ld	l,90
	push	hl
	call	_pr_nr_of_warning1
ELSE
	ld hl,l8ddfh
	push hl	
	call _prwarning
ENDIF
	pop bc	
l1847h:
	pop bc	
	jp l1ad1h
l184bh:
	ld a,(ix+006h)
	cp 052h
	jp nz,l1ad1h
	push iy
	pop hl	
	ld (ix-00ch),l
	ld (ix-00bh),h
	ld (ix-00eh),001h
	ld hl,0000ch
	push hl	
	push iy
	call sub_1ccch
	pop bc	
	push hl	
	pop iy
	ld hl,0000ch
	ex (sp),hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	jp l1738h
l187ah:
	ld a,(ix-010h)
	and 030h
	ld l,a	
	xor a	
	ld h,a	
	ld a,l	
	or h	
	jp z,l1937h
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5ad5h
	pop bc	
	ld a,l	
	or a	
	jp z,l1937h
	ld a,(ix-00dh)
	or a	
	jp z,l18b3h
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5ad5h
	pop bc	
	ld a,l	
	or a	
	jp z,l1937h
l18b3h:
	bit 6,(ix-010h)
	jp z,l18fah
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_1d5ah
	pop bc	
	ld e,l	
	ld (ix-013h),e
	ld l,e	
	ld h,000h
	ex (sp),hl	
	push iy
	call sub_1ccch
	pop bc	
	push hl	
	pop iy
	ld l,(ix-013h)
	ld h,000h
	ex (sp),hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	call sub_1ccch
	pop bc	
	pop bc	
	ld (ix+00ah),l
	ld (ix+00bh),h
l18fah:
	bit 4,(ix-010h)
	jp nz,l1ad1h
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5b08h
	pop bc	
	ld a,l	
	or a	
	jp z,l192dh
	ld a,(ix-00dh)
	or a	
	jp z,l1ad1h
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5b08h
	pop bc	
	ld a,l	
	or a	
	jp nz,l1ad1h
l192dh:
IF SHORTMSG
	ld	l,60
ELSE
	ld hl,l8e06h
ENDIF
l1930h:
	push hl	
IF SHORTMSG
	call	_pr_nr_of_error0
ELSE
	call _prerror
ENDIF
	jp l1847h
l1937h:
	bit 7,(ix-010h)
	jp z,l1ac3h
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
	jp nz,l1974h
	ld a,(ix-00dh)
	or a	
	jp z,l19cch
	ld hl,00014h
	push hl	
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jp z,l19cch
l1974h:
	ld a,(ix+006h)
	cp 07ch
	jp nz,l19c6h
	ld a,(iy+000h)
	cp 03dh
	jp nz,l19c6h
	ld hl,00014h
	push hl	
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jp z,l19c6h
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	ex de,hl	
	pop hl	
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	call sub_2569h
	pop bc	
	jp l1599h
l19c6h:
IF SHORTMSG
	ld	l,61
ELSE
	ld hl,l8e1dh
ENDIF
	jp l1930h
l19cch:
	bit 1,(ix-00fh)
	jp nz,l1a11h
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5aa4h
	pop bc	
	ld a,l	
	or a	
	jp z,l19ffh
	ld a,(ix-00dh)
	or a	
	jp z,l1a11h
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5aa4h
	pop bc	
	ld a,l	
	or a	
	jp nz,l1a11h
l19ffh:
	ld l,(ix-012h)
	ld h,(ix-011h)
	push hl	
IF SHORTMSG
	ld	l,59
	push	hl
	call	_pr_nr_of_error1
ELSE
	ld hl,l8e3ch
	push hl	
	call _prerror
ENDIF
l1a0dh:
	pop bc	
	jp l1847h
l1a11h:
	bit 4,(ix-00fh)
	jp z,l1ad1h
	xor a	
	ld l,a	
	ld a,(ix-00fh)
	and 080h
	ld h,a	
	ld a,l	
	or h	
	jp z,l1a95h
	push iy
	call sub_2105h
	pop bc	
	ld a,l	
	or a	
	jp z,l1a5dh
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00009h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 0,c
	jp z,l1a5dh
	ld hl,0000ch
	add hl,de	
	ld a,(hl)	
	or a	
	jp nz,l1a5dh
	ld hl,00005h
	add hl,de	
	push hl	
	push iy
	call sub_1bf7h
	pop bc	
	pop bc	
	push hl	
	pop iy
	jp l1a95h
l1a5dh:
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	call sub_2105h
	pop bc	
	ld a,l	
	or a	
	jp z,l1a95h
	bit 0,(iy+009h)
	jp z,l1a95h
	ld a,(iy+00ch)
	or a	
	jp nz,l1a95h
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	call sub_1bf7h
	pop bc	
	pop bc	
	ld (ix+00ah),l
	ld (ix+00bh),h
l1a95h:
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_591dh
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jp nz,l1ad1h
	ld l,(ix-012h)
	ld h,(ix-011h)
	push hl	
IF SHORTMSG
	ld	l,89
	push	hl
	call	_pr_nr_of_warning1
ELSE
	ld hl,l8e5ah
	push hl	
	call _prwarning
ENDIF
	jp l1a0dh
l1ac3h:
	bit 1,(ix-00fh)
	jr nz,l1ad1h
IF SHORTMSG
	ld	l,58
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8e79h
	push hl	
	call _prerror
ENDIF
	pop bc	
l1ad1h:
	bit 2,(ix-00fh)
	jp z,l1afeh
	bit 2,(ix-010h)
	ld hl,00001h
	jr z,l1ae2h
	dec hl	
l1ae2h:
	push hl	
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	call sub_1f5dh
	pop bc	
	pop bc	
	pop bc	
	ld (ix+00ah),l
	ld (ix+00bh),h
l1afeh:
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	push iy
	ld l,(ix+006h)
	ld h,000h
	push hl	
	call sub_225ah
	pop bc	
	pop bc	
	pop bc	
	ld (ix-00ah),l
	ld (ix-009h),h
	ld a,(ix-00eh)
	or a	
	jp z,l1b42h
	ld l,(ix-00ch)
	ld h,(ix-00bh)
	push hl	
	call sub_1ebdh
	ex (sp),hl	
	ld l,(ix-00ah)
	ld h,(ix-009h)
	push hl	
	ld hl,00053h
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	pop bc	
	ld (ix-00ah),l
	ld (ix-009h),h
l1b42h:
	ld l,(ix-00ah)
	ld h,(ix-009h)
	jp cret

sub_1b4bh:
	call csv
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	push de	
	call sub_248ah
	pop bc	
	pop bc	
	push hl	
	pop iy
	ld a,(ix+00ah)
	ld (iy+00bh),a
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

sub_1b70h:
	call csv
	ld hl,0007fh
	push hl	
	call _callocm1
	pop bc	
	push hl	
	pop iy
	ld l,(ix+006h)
	ld h,(ix+007h)
	ld (iy+001h),l
	ld (iy+002h),h
	ld (iy+00bh),010h
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

sub_1b94h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	push hl	
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5aa4h
	pop bc	
	ld a,l	
	or a	
	jp nz,l1bbbh
IF SHORTMSG
	ld	l,57
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8e87h
	push hl	
	call _prerror
ENDIF
	pop bc	
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	jp l1bf1h
ENDIF
l1bbbh:
	ld hl,00002h
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
	push iy
IF OPTIM
	jp      nz,popHLcret
ELSE
	jp nz,l1bf3h
ENDIF
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	ld hl,buf26
	push hl	
	call sub_1bf7h
	pop bc	
	ex (sp),hl	
	push iy
	ld hl,0005dh
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	pop bc	
	push hl	
	pop iy
IF OPTIM
	jp      pushIYpopHLcret
ELSE
l1bf1h:
	push iy
l1bf3h:
	pop hl	
	jp cret
ENDIF

sub_1bf7h:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+000h)
	cp 03dh
	jp nz,l1c6ah
	ld l,(iy+001h)
	ld h,(iy+002h)
	ld (ix-002h),l
	ld (ix-001h),h
	ld a,(hl)	
	cp 073h
	jp nz,l1c6ah
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld hl,00012h
	add hl,bc	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 6,c
	jp z,l1c6ah
	ld l,(ix-002h)
	ld h,(ix-001h)
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
IF SHORTMSG
	ld	l,88
	push	hl
	call	_pr_nr_of_warning1
ELSE
	ld hl,l8e9dh
	push hl	
	call _prwarning
ENDIF
	pop bc	
	pop bc	
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld hl,00012h
	add hl,bc	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	ld a,c	
	and 0bfh
	ld c,a	
	ld a,b	
	and a	
	ld b,a	
	ld (hl),b	
	dec hl	
	ld (hl),c	
l1c6ah:
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_591dh
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jp nz,l1cc6h
	ld a,(iy+000h)
	cp 074h
	jp nz,l1c97h
	ld de,__Hbss
	push iy
	pop hl	
	call wrelop
	jp nc,l1cb1h
l1c97h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	call sub_24b6h
	ex (sp),hl	
	push iy
	ld hl,0007ch
	push hl	
	call sub_23b4h
	pop bc	
	pop bc	
	pop bc	
	push hl	
	pop iy
l1cb1h:
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	ex de,hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld bc,00008h
	ldir
	pop hl	
l1cc6h:
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

sub_1ccch:
	call ncsv
	defw 0fff8h
	ld a,(ix+008h)
	ld (ix-002h),a
	ld (ix-004h),000h
	ld (ix-003h),000h
	ld (ix-006h),000h
	ld (ix-005h),000h
	ld (ix-001h),000h
	push ix
	pop de	
	ld hl,0fff8h
	add hl,de	
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	call sub_1bf7h
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF

sub_1d02h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+000h)
	cp 071h
	jr z,l1d19h
	cp 078h
	jr nz,l1d1fh
l1d19h:
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF
l1d1fh:
	ld a,(iy+00ch)
	or a	
	jr nz,l1d19h
	ld a,(iy+009h)
	or (iy+00ah)
	jr nz,l1d19h
	ld b,008h
	ld a,(iy+00bh)
	call brelop
	jp c,l1d40h
	ld a,(iy+00bh)
	cp 012h
	jp nz,l1d4eh
l1d40h:
	ld hl,00008h
l1d43h:
	push hl	
	push iy
	call sub_1ccch
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF
l1d4eh:
	ld a,(iy+00bh)
	cp 00eh
	jr nz,l1d19h
	ld hl,00010h
	jr l1d43h

sub_1d5ah:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	bit 0,(iy+006h)
	jp nz,l1d7ch
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld hl,00006h
	add hl,de	
	bit 0,(hl)
	jr z,l1d81h
l1d7ch:
	ld hl,00001h
	jr l1d84h
l1d81h:
	ld hl,00000h
l1d84h:
	ld (ix-001h),l
	ld a,(iy+006h)
	ld (ix-002h),a
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld hl,00006h
	add hl,de	
	ld b,(hl)	
	ld a,(ix-002h)
	call brelop
	jp nc,l1dafh
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld hl,00006h
	add hl,de	
	ld l,(hl)	
	ld (ix-002h),l
l1dafh:
	ld b,008h
	ld a,(ix-002h)
	call brelop
	jr nc,l1dbdh
	ld (ix-002h),008h
l1dbdh:
	ld a,(ix-002h)
	cp 00eh
	jp z,l1dcah
	cp 010h
	jp nz,l1dcfh
l1dcah:
	ld l,010h
	jp cret
l1dcfh:
	ld a,(ix-002h)
	cp 012h
	jr nz,l1ddah
	ld (ix-002h),008h
l1ddah:
	ld a,(ix-001h)
	or a	
	jp z,l1deah
	ld a,(ix-002h)
	or 001h
	ld l,a	
	jp cret
l1deah:
IF OPTIM
	jp      ldLIXm2cret
ELSE
	ld l,(ix-002h)
	jp cret
ENDIF

sub_1df0h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+000h)
	cp 073h
	jp nz,l1e0ch
	ld a,(iy+00ch)
	cp 002h
	jp z,l1e12h
l1e0ch:
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF
l1e12h:
	ld (iy+00bh),016h
	ld (iy+00ch),000h
	ld l,(iy+001h)
	ld h,(iy+002h)
	ld (iy+005h),l
	ld (iy+006h),h
	ld (iy+009h),000h
	ld (iy+00ah),000h
	push iy
	call sub_1e58h
IF OPTIM
	jp      popBCcret
ELSE
	pop bc	
	jp cret
ENDIF

sub_1e37h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+00ch)
	cp 001h
	push iy
	jp nz,l1e54h
	call sub_1e58h
	push hl	
	pop iy
	ex (sp),hl	
l1e54h:
IF OPTIM
	jp      popHLcret
ELSE
	pop hl	
	jp cret
ENDIF

sub_1e58h:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	push hl	
	ld hl,00046h
	push hl	
	call sub_23b4h
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	ex (sp),hl	
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00005h
	add hl,de	
	ex de,hl	
	pop hl	
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00007h
	add hl,de	
	ld de,00000h
	ld (hl),e	
	inc hl	
	ld (hl),d	
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,0000ch
	add hl,de	
	ld (hl),000h
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5be1h
	pop bc	
IF OPTIM
	jp      ldLIXm2ldHIXm1cret
ELSE
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp cret
ENDIF

sub_1ebdh:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	push hl	
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_24b6h
	pop bc	
	push hl	
	pop iy
	ld b,001h
	push iy
	pop de	
	ld hl,00009h
	add hl,de	
	call asar
	push iy
	ld hl,0004eh
	push hl	
	call sub_1441h
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF

sub_1ef1h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+000h)
	cp 043h
	jp z,l1f42h
	cp 045h
	jp z,l1f14h
	cp 073h
	jp z,l1f19h
IF OPTIM
	jp      ldL0cret
ELSE
	ld l,000h
	jp cret
ENDIF
l1f14h:
IF OPTIM
	jp      ldL1cret
ELSE
	ld l,001h
	jp cret
ENDIF
l1f19h:
	ld e,(iy+001h)
	ld d,(iy+002h)
	ld hl,00012h
	add hl,de	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 4,c
	jp z,l1f3ch
	ld hl,00014h
	add hl,de	
	ld a,(hl)	
	cp 00ch
	jp z,l1f3ch
	ld a,(iy+00ch)
	or a	
	jp z,l1f57h
l1f3ch:
IF OPTIM
	jp      ldHL0cret
ELSE
	ld hl,00000h
	jp cret
ENDIF
l1f42h:
	ld a,(iy+00ch)
	or a	
	jr nz,l1f3ch
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	call sub_1ef1h
	pop bc	
	ld a,l	
	or a	
	jr z,l1f3ch
l1f57h:
IF OPTIM
	jp      ldHL1cret
ELSE
	ld hl,00001h
	jp cret
ENDIF

sub_1f5dh:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	push hl	
	pop de	
	ld hl,00005h
	add hl,de	
	ld (ix-002h),l
	ld (ix-001h),h
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	call sub_591dh
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jp nz,l20f4h
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	call sub_5ad5h
	pop bc	
	ld a,l	
	or a	
	ld l,(ix+008h)
	ld h,(ix+009h)
	jp z,l1fd9h
	push hl	
	call sub_5ad5h
	pop bc	
	ld a,l	
	or a	
	ld l,(ix+008h)
	ld h,(ix+009h)
	jp z,l1fd9h
	push hl	
	call sub_5b08h
	pop bc	
	ld a,l	
	or a	
	jp z,l20f4h
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	call sub_5b38h
	pop bc	
	ld a,l	
	or a	
	jp z,l20f4h
IF SHORTMSG
	ld	l,84
ELSE
	ld hl,l8eb8h
ENDIF
l1fd1h:
	push hl	
IF SHORTMSG
	call	_pr_nr_of_warning0
ELSE
	call _prwarning
ENDIF
	pop bc	
	jp l20f4h
l1fd9h:
	inc hl	
	inc hl	
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 0,c
	jp z,l204dh
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	or a	
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp nz,l2053h
	push hl	
	call sub_5b08h
	pop bc	
	ld a,l	
	or a	
	jp z,l204dh
	ld a,(ix+00ah)
	or (ix+00bh)
	jp nz,l203dh
	ld hl,0000ch
	push hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jp nz,l2037h
	ld hl,0000dh
	push hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	call sub_5a76h
	pop bc	
	pop bc	
	ld a,l	
	or a	
	jp z,l203dh
l2037h:
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF
l203dh:
	push iy
	call sub_2105h
	pop bc	
	ld a,l	
	or a	
	jp nz,l20f4h
IF SHORTMSG
	ld	l,87
ELSE
	ld hl,l8ee0h
ENDIF
	jr l1fd1h
l204dh:
	ld l,(ix-002h)
	ld h,(ix-001h)
l2053h:
	inc hl	
	inc hl	
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 0,c
	jp z,l2084h
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	or a	
	jp nz,l2084h
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	call sub_5b08h
	pop bc	
	ld a,l	
	or a	
	jp z,l2084h
IF SHORTMSG
	ld	l,86
ELSE
	ld hl,l8f09h
ENDIF
	jp l1fd1h
l2084h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	inc hl	
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 0,c
	jp z,l20ech
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	or a	
	jp nz,l20ech
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	inc hl	
	inc hl	
	inc hl	
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	bit 0,c
	jp z,l20ech
	ld e,(ix+008h)
	ld d,(ix+009h)
	ld hl,00007h
	add hl,de	
	ld a,(hl)	
	or a	
	jp nz,l20ech
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	call sub_5a4ah
	pop bc	
	ld a,l	
	or a	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	jp nz,l20fbh
	call sub_5a4ah
	pop bc	
	ld a,l	
	or a	
	jp nz,l20f4h
IF SHORTMSG
	ld	l,85
ELSE
	ld hl,l8f32h
ENDIF
	jp l1fd1h
l20ech:
IF SHORTMSG
	ld	l,56
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l8f5bh
	push hl	
	call _prerror
ENDIF
	pop bc	
l20f4h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
l20fbh:
	push iy
	call sub_1bf7h
IF OPTIM
	jp      popBCpopBCcret
ELSE
	pop bc	
	pop bc	
	jp cret
ENDIF

sub_2105h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,(iy+000h)
	cp 074h
	jp z,l2123h
	cp 07ch
	jp z,l2139h
l211eh:
IF OPTIM
	jp      ldL0cret
ELSE
	ld l,000h
	jp cret
ENDIF
l2123h:
	ld a,(iy+001h)
	or (iy+002h)
	or (iy+003h)
	or (iy+004h)
	ld hl,00001h
	jp z,cret
	dec hl	
	jp cret
l2139h:
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5b08h
	pop bc	
	ld a,l	
	or a	
	jr z,l211eh
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	call sub_2105h
IF OPTIM
	jp      popBCcret
ELSE
	pop bc	
	jp cret
ENDIF

sub_2157h:
	call csv
	ld hl,(buf5)
	ld a,l	
	or h	
	jp nz,l2176h
IF OPTIM
	jp      ldL0cret
ELSE
	ld l,000h
	jp cret
ENDIF
l2167h:
	ld l,(iy+001h)
	ld h,(iy+002h)
	ld (buf5),hl
	push iy
	call _free
	pop bc	
l2176h:
	ld iy,(buf5)
	push iy
	pop hl	
	ld a,l	
	or h	
	jr nz,l2167h
IF OPTIM
	jp      ldL1cret
ELSE
	ld l,001h
	jp cret
ENDIF

_callocm1:
	call csv
	ld hl,(buf5)
	ld a,l	
	or h	
	jp z,l21ach
	ld iy,(buf5)
	ld l,(iy+001h)
	ld h,(iy+002h)
	ld (buf5),hl
	ld hl,0000dh
	push hl	
	push iy
	call _blkclr
	pop bc	
	pop bc	
	jp l21b7h
l21ach:
	ld hl,0000dh
	push hl	
	call _allocmem
	pop bc	
	push hl	
	pop iy
l21b7h:
	ld a,(ix+006h)
	ld (iy+000h),a
	ld (iy+00bh),014h
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

sub_21c7h:
	call csv
	push hl	
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld hl,00000h
	push hl	
	call _callocm1
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	push iy
	pop hl	
	ld e,(ix-002h)
	ld d,(ix-001h)
	push hl	
	ld bc,0000dh
	ldir
	pop hl	
	ld e,(iy+000h)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9275h
	add hl,de	
	ld l,(hl)	
	ld h,000h
	ld (ix-004h),l
	ld (ix-003h),h
	bit 0,(ix-004h)
	jp nz,l221bh
	ld a,(iy+000h)
	cp 078h
	jp nz,l2224h
l221bh:
IF OPTIM
	jp      ldLIXm2ldHIXm1cret
ELSE
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp cret
ENDIF
l2224h:
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	call sub_21c7h
	pop bc	
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	bit 1,(ix-004h)
	jr z,l221bh
	ld l,(iy+003h)
	ld h,(iy+004h)
	push hl	
	call sub_21c7h
	pop bc	
	ex de,hl	
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	inc hl	
	inc hl	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	jr l221bh

sub_225ah:
	call csv
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	pop iy
	ld a,(ix+006h)
	cp 07ch
	jp nz,l22a3h
	ld a,(iy+000h)
	cp 074h
	jp nz,l22a3h
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	ex de,hl	
	pop hl	
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	ld l,(ix+00ah)
	ld h,(ix+00bh)
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
l22a3h:
	ld l,(ix+00ah)
	ld h,(ix+00bh)
	push hl	
	push iy
	ld l,(ix+006h)
	ld h,000h
	push hl	
	call sub_23b4h
	pop bc	
	pop bc	
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	ld e,(ix+006h)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9278h
	add hl,de	
	ld a,(hl)	
	add a,0ffh
	ld l,a	
	ld h,000h
	ld a,h	
	cp h	
	jp c,l2307h
	jp nz,l22e2h
	ld a,006h
	cp l	
	jp c,l2307h
l22e2h:
	add hl,hl	
	ld de,l8bc9h
	add hl,de	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	jp (hl)	
l22ech:	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00005h
	add hl,de	
	ex de,hl	
	pop hl	
	push hl	
	ld bc,00008h
	ldir
	pop hl	
l2307h:
IF OPTIM
	jp      ldLIXm2ldHIXm1cret
ELSE
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp cret
ENDIF
l2310h:	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld hl,00005h
	add hl,de	
	push hl	
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00005h
	add hl,de	
	ex de,hl	
	pop hl	
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	jr l2307h
l2330h:	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00005h
	add hl,de	
	ex de,hl	
	pop hl	
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5be1h
l2359h:
	pop bc	
	jr l2307h
l235ch:	push iy
	pop de	
	ld hl,00005h
	add hl,de	
	push hl	
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00005h
	add hl,de	
	ex de,hl	
	pop hl	
	push hl	
	ld bc,00008h
	ldir
	pop hl	
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00005h
	add hl,de	
	push hl	
	call sub_5b99h
	jr l2359h
l2387h:	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,0000bh
	add hl,de	
	ld (hl),008h
	jp l2307h
l2396h:	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,0000bh
	add hl,de	
	ld (hl),002h
	jp l2307h
l23a5h:	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,0000bh
	add hl,de	
	ld (hl),014h
	jp l2307h

sub_23b4h:
	call csv
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	push hl	
	pop iy
	ld l,(ix+006h)
	ld h,000h
	push hl	
	call _callocm1
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	ld a,(ix+006h)
	cp 078h
	jp z,l2405h
	push iy
	pop de	
	inc hl	
	ld (hl),e	
	inc hl	
	ld (hl),d	
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
	jp z,l2405h
	ld e,(ix+00ah)
	ld d,(ix+00bh)
	ld l,(ix-002h)
	ld h,(ix-001h)
	inc hl	
	inc hl	
	inc hl	
	ld (hl),e	
	inc hl	
	ld (hl),d	
l2405h:
IF OPTIM
	jp      ldLIXm2ldHIXm1cret
ELSE
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp cret
ENDIF

sub_240eh:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld hl,00073h
	push hl	
	call _callocm1
	pop bc	
	ld (ix-002h),l
	ld (ix-001h),h
	push iy
	pop de	
	inc hl	
	ld (hl),e	
	inc hl	
	ld (hl),d	
	bit 4,(iy+012h)
	jp nz,l243fh
	ld a,(iy+014h)
	cp 00ah
	jp nz,l2454h
l243fh:
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,00005h
	add hl,de	
	ex de,hl	
	push iy
	pop hl	
	push hl	
	ld bc,00008h
	ldir
	pop hl	
l2454h:
IF OPTIM
	jp      ldLIXm2ldHIXm1cret
ELSE
	ld l,(ix-002h)
	ld h,(ix-001h)
	jp cret
ENDIF

sub_245dh:
	call csv
	ld hl,00076h
	push hl	
	call _callocm1
	pop bc	
	push hl	
	pop iy
	ld (iy+00bh),004h
	ld (iy+009h),001h
	ld (iy+00ah),000h
	ld hl,(l8bd7h)
	inc hl	
	ld (l8bd7h),hl
	ld (iy+001h),l
	ld (iy+002h),h
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

sub_248ah:
	call csv
	ld hl,00074h
	push hl	
	call _callocm1
	pop bc	
	push hl	
	pop iy
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld (iy+001h),e
	ld (iy+002h),d
	ld (iy+003h),l
	ld (iy+004h),h
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

sub_24b6h:
	call csv
	ld hl,00077h
	push hl	
	call _callocm1
	pop bc	
	push hl	
	pop iy
	push hl	
	pop de	
	ld hl,00005h
	add hl,de	
	ex de,hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	ld bc,00008h
	ldir
	pop hl
IF OPTIM
	jp      pushIYpopHLcret
ELSE	
	push iy
	pop hl	
	jp cret
ENDIF

_exprcomplex:
	ld hl,l8f6eh
	push hl	
	call _fatalerror
	pop bc	
	ret
	
sub_24e7h:
	call csv
	ld de,buf46
	ld hl,(l9cf1h)
	or a	
	sbc hl,de
	call z,_exprcomplex
	ld e,(ix+006h)
	ld d,(ix+007h)
	ld hl,(l9cf1h)
	dec hl	
	dec hl	
	ld (l9cf1h),hl
	ld (hl),e	
	inc hl	
	ld (hl),d	
	jp cret

sub_250ah:
	ld de,buf45
	ld hl,(l9cf1h)
	or a	
	sbc hl,de
	jp z,l2525h
	ld hl,(l9cf1h)
	inc hl	
	inc hl	
	ld (l9cf1h),hl
	dec hl	
	dec hl	
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	ret	
l2525h:
	ld hl,00000h
	ret	

sub_2529h:
	call csv
	ld de,l9cf3h
	ld hl,(l8bc7h)
	or a	
	sbc hl,de
	call z,_exprcomplex
	ld a,(ix+006h)
	ld hl,(l8bc7h)
	dec hl	
	dec hl	
	ld (l8bc7h),hl
	ld (hl),a	
	ld e,a	
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9274h
	add hl,de	
	ld l,(hl)	
	ld iy,(l8bc7h)
	ld (iy+001h),l
	jp cret

sub_255dh:
	ld hl,(l8bc7h)
	inc hl	
	inc hl	
	ld (l8bc7h),hl
	dec hl	
	dec hl	
	ld l,(hl)	
	ret	

sub_2569h:
	call csv
	push hl	
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jp z,cret
	ld a,(iy+000h)
	ld (ix-001h),a
	ld e,(ix-001h)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9275h
	add hl,de	
	bit 0,(hl)
	jp nz,l25c9h
	ld a,(ix-001h)
	cp 078h
	jp z,l25c9h
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	call sub_2569h
	pop bc	
	ld e,(ix-001h)
	ld d,000h
	ld hl,0ffc4h
	add hl,de	
	add hl,hl	
	add hl,hl	
	add hl,hl	
	ld de,l9275h
	add hl,de	
	bit 1,(hl)
	jp z,l25c9h
	ld l,(iy+003h)
	ld h,(iy+004h)
	push hl	
	call sub_2569h
	pop bc	
l25c9h:
	ld de,__Hbss
	push iy
	pop hl	
	call wrelop
	jp c,cret
	ld a,(ix-001h)
	cp 07fh
	jp nz,l25e7h
	ld l,(iy+001h)
	ld h,(iy+002h)
	push hl	
	call _free
l25e7h:
	ld hl,(buf5)
	ld (iy+001h),l
	ld (iy+002h),h
	ld (buf5),iy
	jp cret

sub_25f7h:
	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	push hl	
	pop iy
	ld a,l	
	or h	
	jp z,l266bh
	ld de,__Hbss
	push iy
	pop hl	
	call wrelop
	ld a,(iy+000h)
	jp c,l2631h
	cp 074h
	jp nz,l262eh
	ld de,00001h
	ld hl,00000h
	push hl	
	push de	
	push iy
	pop hl	
	inc hl	
	call asaladd
	jp l266bh
l262eh:
	ld a,(iy+000h)
l2631h:
	cp 055h
	jp nz,l264ah
	ld l,(iy+003h)
	ld h,(iy+004h)
	push hl	
	call sub_25f7h
	pop bc	
	ld (iy+003h),l
	ld (iy+004h),h
	jp l266bh
l264ah:
	ld hl,00008h
	push hl	
	ld de,00001h
	ld hl,00000h
	push hl	
	push de	
	call sub_1b4bh
	pop bc	
	pop bc	
	ex (sp),hl	
	push iy
	ld hl,00055h
	push hl	
	call sub_1441h
	pop bc	
	pop bc	
	pop bc	
	push hl	
	pop iy
l266bh:
IF OPTIM
	jp      pushIYpopHLcret
ELSE
	push iy
	pop hl	
	jp cret
ENDIF

sub_2671h:
	call ncsv
	defw 0ffcbh
	ld a,(buf7)
	or a	
	jp z,l2764h
	ld (ix-003h),a
	xor a	
	ld (buf7),a
	ld a,(ix-003h)
	cp 073h
	jr nz,l269ch
	ld a,(l8f86h)
	or a	
	jr z,l269ch
	ld hl,buf47
	push hl	
	call sub_4e90h
	pop bc	
	ld (buf8),hl
l269ch:
	ld l,(ix-003h)
	jp cret
l26a2h:
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,l8ab6h
	add hl,de	
	bit 2,(hl)
	ld l,e	
	ld h,d	
	jr z,l26bah
l26b2h:
	push hl	
	call _callocm2
l26b6h:
IF OPTIM
	jp      popBCcret
ELSE
	pop bc	
	jp cret
ENDIF
l26bah:
	ld a,h	
	cp 0ffh
	jr nz,l26c5h
	ld a,l	
	cp 0ffh
	jp z,l278dh
l26c5h:
	ld a,h	
	or a	
	jp nz,l2752h
	ld a,l	
	cp 021h
	jp z,l2c1bh
	cp 022h
	jp z,l28f7h
	cp 023h
	jp z,l2792h
	cp 025h
	jp z,l2a7dh
	cp 026h
	jp z,l2aa1h
	cp 027h
	jp z,l2904h
	cp 028h
	jp z,l2c44h
	cp 029h
	jp z,l2c49h
	cp 02ah
	jp z,l2a35h
	cp 02bh
	jp z,l299ch
	cp 02ch
	jp z,l2cbeh
	cp 02dh
	jp z,l29dfh
	cp 02eh
	jp z,l2c62h
	cp 02fh
	jp z,l2a59h
	cp 03ah
	jp z,l2997h
	cp 03bh
	jp z,l2992h
	cp 03ch
	jp z,l2b4bh
	cp 03dh
	jp z,l2bf7h
	cp 03eh
	jp z,l2ba1h
	cp 03fh
	jp z,l2cb9h
	cp 05bh
	jp z,l2c4eh
	cp 05dh
	jp z,l2c53h
	cp 05eh
	jp z,l2b27h
	cp 07bh
	jp z,l2c58h
	cp 07ch
	jp z,l2ae4h
	cp 07dh
	jp z,l2c5dh
	cp 07eh
	jp z,l2c3fh
l2752h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	push hl	
	push hl	
IF SHORTMSG
	ld	l,55
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l9112h
	push hl	
	call _prerror
ENDIF
	pop bc	
	pop bc	
	pop bc	
l2764h:
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
	ld hl,(buf12)
	ld (buf13),hl
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,l8ab6h
	add hl,de	
	ld a,(hl)	
	and 003h
	or a	
	jp z,l26a2h
	ld l,e	
	ld h,d	
	push hl	
	call sub_2f75h
	jp l26b6h
l278dh:
IF OPTIM
	jp      ldL1cret
ELSE
	ld l,001h
	jp cret
ENDIF
l2792h:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
	ex de,hl	
	ld hl,l8ab6h
	add hl,de	
	bit 4,(hl)
	jr z,l27b2h
IF OPTIM
	ld      a,(ix-002h)
	cp      0ah
ELSE
	ld de,0000ah
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l2792h
l27b2h:
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,l8ab6h
	add hl,de	
	bit 2,(hl)
	jp z,l2886h
	ld l,e	
	ld h,d	
	push hl	
	call _callocm2
	pop bc	
	ld a,l	
	cp 074h
	jp nz,l2886h
	ld de,(buf8)
	ld hl,(buf9)
	ex de,hl	
	ld bc,0ffffh
	add hl,bc	
	ex de,hl	
	ld (buf1),de
l27dfh:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
	ex de,hl	
	ld hl,l8ab6h
	add hl,de	
	bit 4,(hl)
	jr z,l27ffh
IF OPTIM
	ld      a,(ix-002h)
	cp      0ah
ELSE
	ld de,0000ah
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l27dfh
l27ffh:
IF OPTIM
	ld      a,(ix-002h)
	cp      22h
ELSE
	ld de,00022h
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jp nz,l2764h
	push ix
	pop de	
	ld hl,0ffcbh
	add hl,de	
	push hl	
	pop iy
	jr l2822h
l281ah:
	ld a,(ix-002h)
	ld (iy+000h),a
	inc iy
l2822h:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      22h
ELSE
	ld de,00022h
	or a	
	sbc hl,de
ENDIF
	jr z,l2841h
IF OPTIM
	cp      0ah
ELSE
	ld de,0000ah
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l281ah
l2841h:
	ld (iy+000h),000h
	ld a,(ix-035h)
	or a	
	jr z,l285eh
	push ix
	pop de	
	ld hl,0ffcbh
	add hl,de	
l2852h:
	push hl	
	ld hl,buf2
	push hl	
	call _strcpy
	pop bc	
	pop bc	
	jr l2869h
l285eh:
	ld hl,(buf14)
	ld a,l	
	or h	
	jr nz,l2852h
	xor a	
	ld (buf2),a
l2869h:
	ld hl,(buf15)
	ld a,l	
	or h	
	jp z,l2764h
	ld hl,buf2
	push hl	
	ld hl,l90b9h
	push hl	
	ld hl,(buf15)
	push hl	
	call _fprintf
	pop bc	
	pop bc	
	pop bc	
	jp l2764h
l2886h:
	push ix
	pop de	
	ld hl,0ffcbh
	add hl,de	
	push hl	
	pop iy
l2890h:
	ld a,(ix-002h)
	ld (iy+000h),a
	inc iy
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
	jr z,l28b7h
	ld e,(ix-002h)
	ld d,(ix-001h)
	ld hl,l8ab6h
	add hl,de	
	bit 4,(hl)
	jr z,l2890h
l28b7h:
	ld (iy+000h),000h
	jr l28c6h
l28bdh:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
l28c6h:
IF OPTIM
	ld      a,(ix-002h)
	cp      0ah
ELSE
	ld de,0000ah
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l28bdh
	ld hl,l90beh
	push hl	
	push ix
	pop de	
	ld hl,0ffcbh
	add hl,de	
	push hl	
	call _strcmp
	pop bc	
	pop bc	
	ld a,l	
	or h	
	jr nz,l28efh
	call sub_308bh
	jp l2764h
l28efh:
	ld hl,l90c2h
	push hl	
	call _fatalerror
	pop bc	
l28f7h:
	ld hl,00022h
	push hl	
	call _callocm3
	pop bc	
	ld l,076h
	jp cret
l2904h:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      5ch
ELSE
	ld de,0005ch
	or a	
	sbc hl,de
ENDIF
	jr z,l291dh
	ld l,(ix-002h)
	ld h,(ix-001h)
	jr l2929h
l291dh:
	call sub_320dh
	push hl	
	call sub_3495h
	pop bc	
	ld a,l	
	rla	
	sbc a,a	
	ld h,a	
l2929h:
	ex de,hl	
	ld a,d	
	rla	
	sbc a,a	
	ld l,a	
	ld h,a	
	ld (buf8),de
	ld (buf9),hl
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
	jr nz,l2951h
	ld hl,l90d8h
	push hl	
	call _errexpected
l294eh:
	pop bc	
	jr l2971h
l2951h:
IF OPTIM
	ld      a,(ix-002h)
	cp      27h
ELSE
	ld de,00027h
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr z,l2971h
IF SHORTMSG
	ld	l,54
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l90e6h
	push hl	
	call _prerror
ENDIF
	jr l294eh
l2968h:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
l2971h:
IF OPTIM
	ld      a,(ix-002h)
	cp      0ah
ELSE
	ld de,0000ah
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr z,l298dh
IF OPTIM
	cp      27h
ELSE
	ld de,00027h
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l2968h
l298dh:
	ld l,074h
	jp cret
l2992h:
	ld l,002h
	jp cret
l2997h:
	ld l,063h
	jp cret
l299ch:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      2bh
ELSE
	ld de,0002bh
	or a	
	sbc hl,de
ENDIF
	jr nz,l29b2h
	ld l,04ah
	jp cret
l29b2h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l29d1h
	ld l,065h
	jp cret
l29d1h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,055h
	jp cret
l29dfh:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      2dh
ELSE
	ld de,0002dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l29f5h
	ld l,04ch
	jp cret
l29f5h:
IF OPTIM
	cp      3eh
ELSE
	ld de,0003eh
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l2a08h
	ld l,044h
	jp cret
l2a08h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l2a27h
	ld l,067h
	jp cret
l2a27h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,052h
	jp cret
l2a35h:
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l2a4bh
	ld l,069h
	jp cret
l2a4bh:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,050h
	jp cret
l2a59h:
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l2a6fh
	ld l,06ah
	jp cret
l2a6fh:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,053h
	jp cret
l2a7dh:
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF	
	jr nz,l2a93h
	ld l,06bh
	jp cret
l2a93h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,054h
	jp cret
l2aa1h:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      26h
ELSE
	ld de,00026h
	or a	
	sbc hl,de
ENDIF
	jr nz,l2ab7h
	ld l,060h
	jp cret
l2ab7h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l2ad6h
	ld l,06eh
	jp cret
l2ad6h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,051h
	jp cret
l2ae4h:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      7ch
ELSE
	ld de,0007ch
	or a	
	sbc hl,de
ENDIF
	jr nz,l2afah
	ld l,061h
	jp cret
l2afah:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l2b19h
	ld l,070h
	jp cret
l2b19h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,05fh
	jp cret
l2b27h:
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l2b3dh
	ld l,06fh
	jp cret
l2b3dh:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,05eh
	jp cret
l2b4bh:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3ch
ELSE
	ld de,0003ch
	or a	
	sbc hl,de
ENDIF
	jr nz,l2b80h
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l2b72h
	ld l,06dh
	jp cret
l2b72h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,057h
	jp cret
l2b80h:
IF OPTIM
	ld      a,(ix-002h)
	cp      3dh
ELSE
	ld de,0003dh
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l2b93h
	ld l,05ah
	jp cret
l2b93h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,058h
	jp cret
l2ba1h:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3eh
ELSE
	ld de,0003eh
	or a	
	sbc hl,de
ENDIF
	jr nz,l2bd6h
	call sub_347ah
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l2bc8h
	ld l,06ch
	jp cret
l2bc8h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,056h
	jp cret
l2bd6h:
IF OPTIM
	ld      a,(ix-002h)
	cp      3dh
ELSE
	ld de,0003dh
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l2be9h
	ld l,05bh
	jp cret
l2be9h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,059h
	jp cret
l2bf7h:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l2c0dh
	ld l,05ch
	jp cret
l2c0dh:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,064h
	jp cret
l2c1bh:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      3dh
ELSE
	ld de,0003dh
	or a	
	sbc hl,de
ENDIF
	jr nz,l2c31h
	ld l,05dh
	jp cret
l2c31h:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,048h
	jp cret
l2c3fh:
	ld l,049h
	jp cret
l2c44h:
	ld l,03eh
	jp cret
l2c49h:
	ld l,03fh
	jp cret
l2c4eh:
	ld l,041h
	jp cret
l2c53h:
	ld l,042h
	jp cret
l2c58h:
	ld l,004h
	jp cret
l2c5dh:
	ld l,003h
	jp cret
l2c62h:
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
	ex de,hl	
	ld hl,l8ab6h
	add hl,de	
	bit 2,(hl)
	jr z,l2c7fh
	ld l,e	
	ld h,d	
	ld (buf16),hl
	ld hl,0002eh
	jp l26b2h
l2c7fh:
IF OPTIM
	ld      a,(ix-002h)
	cp      2eh
ELSE
	ld de,0002eh
	ld l,(ix-002h)
	ld h,(ix-001h)
	or a	
	sbc hl,de
ENDIF
	jr nz,l2cabh
	call sub_320dh
	ld (ix-002h),l
	ld (ix-001h),h
IF OPTIM
	ld      a,l
	cp      2eh
ELSE
	ld de,0002eh
	or a	
	sbc hl,de
ENDIF
	jr z,l2ca6h
IF SHORTMSG
	ld	l,53
	push	hl
	call	_pr_nr_of_error0
ELSE
	ld hl,l90fah
	push hl	
	call _prerror
ENDIF
	pop bc	
l2ca6h:
	ld l,00dh
	jp cret
l2cabh:
	ld l,(ix-002h)
	ld h,(ix-001h)
	ld (buf16),hl
	ld l,043h
	jp cret
l2cb9h:
	ld l,062h
	jp cret
l2cbeh:
	ld l,071h
	jp cret

