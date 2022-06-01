include cgen.inc

	psect	top

	global	topcode

topcode:

global csv
call csv
ld      l,(ix+6)
ld      h,(ix+7)
dec     hl
ld      (ix+6),l
ld      (ix+7),h
ld      l,(ix+8)
ld      h,(ix+9)
inc     hl
inc     hl
ld      (ix+8),l
ld      (ix+9),h
jp      l11
l12:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
inc     hl
ld      a,(hl)
cp      66
jp      z,l21
cp      69
jp      z,l22
cp      72
jp      z,l23
cp      80
jp      z,l17
cp      82
jp      z,l20
cp      87
jp      z,l19
cp      112
jp      z,l17
cp      119
jp      z,l19
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
inc     hl
ld      a,(hl)
ld      l,a
rla
sbc     a,a
ld      h,a
push    hl
ld      hl,19f
push    hl
call    _fatalErr
pop     bc
pop     bc
l14:
ld      l,(ix+8)
ld      h,(ix+9)
inc     hl
inc     hl
ld      (ix+8),l
ld      (ix+9),h
ld      l,(ix+6)
ld      h,(ix+7)
dec     hl
ld      (ix+6),l
ld      (ix+7),h
l11:
ld      e,(ix+6)
ld      d,(ix+7)
ld      hl,0
call    wrelop
jp      m,L3
jp      l13
l17:
ld      a,1
ld      (_pflag),a
jp      l14
l19:
ld      a,1
ld      (_wflag),a
jp      l14
l20:
ld      a,1
ld      (_rflag),a
jp      l14
l21:
ld      a,1
ld      (_bflag),a
jp      l14
l22:
ld      a,1
ld      (_eflag),a
jp      l14
l23:
ld      a,1
ld      (_hflag),a
jp      l14
L3:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
ld      a,(hl)
cp      45
jp      z,l12
l13:
ld      e,(ix+6)
ld      d,(ix+7)
dec     de
ld      (ix+6),e
ld      (ix+7),d
inc     de
ld      hl,0
call    wrelop
jp      p,l25
ld      hl,__iob
push    hl
ld      hl,29f
push    hl
ld      l,(ix+8)
ld      h,(ix+9)
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
call    _freopen
pop     bc
pop     bc
pop     bc
ld      a,l
or      h
jp      nz,l26
ld      l,(ix+8)
ld      h,(ix+9)
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
ld      hl,39f
push    hl
call    _fatalErr
pop     bc
pop     bc
jp      l25
l26:
ld      e,(ix+6)
ld      d,(ix+7)
ld      hl,0
call    wrelop
jp      p,l25
ld      hl,__iob+8
push    hl
ld      hl,49f
push    hl
ld      l,(ix+8)
ld      h,(ix+9)
inc     hl
inc     hl
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
call    _freopen
pop     bc
pop     bc
pop     bc
ld      a,l
or      h
jp      nz,l25
ld      l,(ix+8)
ld      h,(ix+9)
inc     hl
inc     hl
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
ld      hl,59f
push    hl
call    _fatalErr
pop     bc
pop     bc
l25:

IF RAM128
global  _InitDynM
call    _InitDynM
ENDIF

	jp	rettop

;=========================================================
; sub_6BF5 freopen OK(no code matching)   Used in: main
;=========================================================
;	FILE * freopen(char * name, char * mode, register FILE * f) {
;	    unsigned char c1, func;
;	    struct fcb * p;
;
;	    fclose(f);
;	    p = _fcb + (f->_file = (f - stdin));
;	    c1 = 0;
;	    func = CPMOPN;
;	    f->_flag &= 0x4F;
;	    switch(*mode) {
;	      case 'w':
;		c1++;
;	        func = CPMMAKE;
;	      case 'r':
;		if(*(mode+1) == 'b') f->_flag = _IOBINARY;
;		break;
;	    }
;	    def_fcb(p, name);
;	    if(func == CPMMAKE) bdos(CPMDEL, p);
;	    if(bdos(func, p) == -1) return 0;
;	    LO_CHAR(p->rwp) = c1 ? 2 : 1;
;	    if(((LO_CHAR(f->_flag) |= (c1+1))&0xC) == 0) {
;		if(f->_base == 0) f->_base = sbrk(512);
;	    }
;	    f->_ptr = f->_base;
;	    if(f->_base == 0) goto m8;
;	    LO_CHAR(f->_cnt) = 0;
;	    if(c1 == 0) goto m9;
;	    HI_CHAR(f->_cnt) = 2;
;	    goto m10;
;	m8: LO_CHAR(f->_cnt) = 0;
;	m9: HI_CHAR(f->_cnt) = 0;
;	m10:
;	    return f;
;	}
;
	global	_freopen
	global	_fclose
	global	__fcb
	global	__iob
	global	adiv
	global	amul
	global	_def_fcb
	global	_bdos
	global	_sbrk

_freopen:
	call	csv
	push	hl
	push	hl
	ld	l,(ix+10)
	ld	h,(ix+11)
	push	hl
	pop	iy
	push	hl
	call	_fclose
	pop	bc
	ld	de,__iob
	push	iy
	pop	hl
	or	a
	sbc	hl,de
	ld	de,8
	call	adiv
	ld	a,l
	ld	(iy+7),a
	ld	l,a
	rla
	sbc	a,a
	ld	h,a
	ld	de,41
	call	amul
	ld	de,__fcb
	add	hl,de
	ld	(ix+-4),l
	ld	(ix+-3),h
	ld	(ix+-1),0
	ld	(ix+-2),15
	ld	a,(iy+6)
	and	79
	ld	(iy+6),a
	ld	l,(ix+8)
	ld	h,(ix+9)
	ld	a,(hl)
	cp	'r'
	jr	z,loc_6C55
	cp	'w'
	jr	nz,loc_6C65
	inc	(ix+-1)
	ld	(ix+-2),22
loc_6C55:
	ld	l,(ix+8)
	ld	h,(ix+9)
	inc	hl
	ld	a,(hl)
	cp	'b'
	jr	nz,loc_6C65
	ld	(iy+6),-128
loc_6C65:
	ld	l,(ix+6)
	ld	h,(ix+7)
	push	hl
	ld	l,(ix+-4)
	ld	h,(ix+-3)
	push	hl
	call	_def_fcb
	pop	bc
	pop	bc
	ld	a,(ix+-2)
	cp	22
	ld	l,(ix+-4)
	ld	h,(ix+-3)
	push	hl
	jr	nz,loc_6C95
	ld	hl,19
	push	hl
	call	_bdos
	pop	bc
	ld	l,(ix+-4)
	ld	h,(ix+-3)
	ex	(sp),hl
loc_6C95:
	ld	l,(ix+-2)
	ld	h,0
	push	hl
	call	_bdos
	pop	bc
	pop	bc
	ld	a,l
	cp	255
	jr	nz,loc_6CAB
	ld	hl,0
	jp	cret
loc_6CAB:
	ld	a,(ix+-1)
	or	a
	jr	nz,loc_6CB6
	ld	hl,1
	jr	loc_6CB9
loc_6CB6:
	ld	hl,2
loc_6CB9:
	ld	a,l
	ld	e,(ix+-4)
	ld	d,(ix+-3)
	ld	hl,36
	add	hl,de
	ld	(hl),a
	ld	a,(ix+-1)
	inc	a
	ld	e,a
	ld	a,(iy+6)
	or	e
	ld	(iy+6),a
	and	12
	or	a
	jr	nz,loc_6CEC
	ld	a,(iy+4)
	or	(iy+5)
	jr	nz,loc_6CEC
	ld	hl,512
	push	hl
	call	_sbrk
	pop	bc
	ld	(iy+4),l
	ld	(iy+5),h
loc_6CEC:
	ld	l,(iy+4)
	ld	h,(iy+5)
	ld	(iy+0),l
	ld	(iy+1),h
	ld	a,l
	or	(iy+5)
	jr	z,loc_6D0E
	ld	a,(ix+-1)
	or	a
	ld	(iy+2),0
	jr	z,loc_6D12
	ld	(iy+3),2
	jr	loc_6D16
loc_6D0E:
	ld	(iy+2),0
loc_6D12:
	ld	(iy+3),0
loc_6D16:
	push	iy
	pop	hl
	jp	cret

	psect	data

19:
defb    73,108,108,101,103,97,108,9,115,119,105,116,99,104,32,37
defb    99,0
29:
defb    114,0
39:
defb    67,97,110,39,116,32,111,112,101,110,32,37,115,0
49:
defb    119,0
59:
defb    67,97,110,39,116,32,99,114,101,97,116,101,32,37,115,0

