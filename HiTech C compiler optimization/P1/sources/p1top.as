	psect top

	global topcode

topcode:

	call csv
	ld l,(ix+006h)
	ld h,(ix+007h)
	dec hl	
	ld (ix+006h),l
	ld (ix+007h),h
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	inc hl	
	ld (ix+008h),l
	ld (ix+009h),h
	jr l36efh
l369eh:
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	inc hl	
	ld a,(hl)	
	cp 043h
	jp z,l3737h
	cp 045h
	jr z,l36fah
	cp 04ch
	jp z,l3730h
	cp 053h
	jr z,l3722h
	cp 057h
	jr z,l3729h
	cp 063h
	jr z,l3737h
	cp 065h
	jr z,l36fah
	cp 06ch
	jr z,l3730h
	cp 073h
	jr z,l3722h
	cp 077h
	jr z,l3729h
l36d4h:
	ld l,(ix+006h)
	ld h,(ix+007h)
	dec hl	
	ld (ix+006h),l
	ld (ix+007h),h
	ld l,(ix+008h)
	ld h,(ix+009h)
	inc hl	
	inc hl	
	ld (ix+008h),l
	ld (ix+009h),h
l36efh:
	ld a,(ix+006h)
	or (ix+007h)
	jr nz,l3762h
	jp l3772h
l36fah:
	ld hl,l9bc9h
	push hl	
	ld hl,l91f2h
	push hl	
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	inc bc	
	inc bc	
	push bc	
	call _freopen
	pop bc	
	pop bc	
	ld hl,buf49
	ex (sp),hl	
	ld hl,l9bc9h
	push hl	
	call sub_7b7eh
	pop bc	
	pop bc	
	jr l36d4h
l3722h:
	ld a,001h
	ld (buf3),a
	jr l36d4h
l3729h:
	ld a,001h
	ld (buf23),a
	jr l36d4h
l3730h:
	ld a,001h
	ld (buf20),a
	jr l36d4h
l3737h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	inc hl	
	inc hl	
	ld a,(hl)	
	or a	
	jr z,l3759h
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld c,(hl)	
	inc hl	
	ld b,(hl)	
	inc bc	
	inc bc	
	ld (buf24),bc
	jp l36d4h
l3759h:
	ld hl,buf50
	ld (buf24),hl
	jp l36d4h
l3762h:
	ld l,(ix+008h)
	ld h,(ix+009h)
	ld a,(hl)	
	inc hl	
	ld h,(hl)	
	ld l,a	
	ld a,(hl)	
	cp 02dh
	jp z,l369eh
l3772h:
	jp	retcode
