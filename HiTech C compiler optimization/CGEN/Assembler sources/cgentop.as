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

