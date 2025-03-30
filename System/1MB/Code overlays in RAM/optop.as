global  _topmain
global  ncsv, cret, indir
global  _InitOverlays
global  _pr_error
global  _Init512Banks
global  _key_l
global  _key_r
global  _key_f
global  _key_s
global  wrelop
global  wrelop
global  _freopen
global  __iob
global  wrelop
global  _setbuf
psect   top
_topmain:
global csv
call csv
call    _InitOverlays
ld      a,l
or      a
jp      nz,l17
ld      hl,19f
push    hl
call    _pr_error
pop     bc
l17:
call    _Init512Banks
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
jp      l18
l19:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
inc     hl
ld      a,(hl)
cp      70
jp      z,l28
cp      76
jp      z,l24
cp      82
jp      z,l26
cp      102
jp      z,l28
cp      108
jp      z,l24
cp      114
jp      z,l26
cp      115
jp      z,l29
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
ld      hl,29f
push    hl
call    _pr_error
pop     bc
pop     bc
l21:
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
l18:
ld      e,(ix+6)
ld      d,(ix+7)
ld      hl,0
call    wrelop
jp      m,L2
jp      l20
l24:
ld      a,1
ld      (_key_l),a
jp      l21
l26:
ld      a,1
ld      (_key_r),a
jp      l21
l28:
ld      a,1
ld      (_key_f),a
jp      l21
l29:
ld      a,1
ld      (_key_s),a
jp      l21
L2:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
ld      a,(hl)
cp      45
jp      z,l19
l20:
ld      e,(ix+6)
ld      d,(ix+7)
ld      hl,0
call    wrelop
jp      p,l31
ld      hl,__iob
push    hl
ld      hl,39f
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
jp      nz,l32
ld      l,(ix+8)
ld      h,(ix+9)
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
ld      hl,49f
push    hl
call    _pr_error
pop     bc
pop     bc
l32:
ld      e,(ix+6)
ld      d,(ix+7)
ld      hl,1
call    wrelop
jp      p,l31
ld      hl,__iob+8
push    hl
ld      hl,59f
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
jp      nz,l31
ld      l,(ix+8)
ld      h,(ix+9)
inc     hl
inc     hl
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
ld      hl,69f
push    hl
call    _pr_error
pop     bc
pop     bc
l31:
ld      a,(_key_s)
or      a
jp      z,cret
ld      hl,0
push    hl
ld      hl,__iob+8
push    hl
call    _setbuf
jp      cret
19:
defb    67,97,110,39,116,32,111,112,101,110,32,111,118,101,114,108
defb    97,121,115,0
29:
defb    73,108,108,101,103,97,108,32,115,119,105,116,99,104,32,37
defb    99,0
39:
defb    114,0
49:
defb    67,97,110,39,116,32,111,112,101,110,32,37,115,0
59:
defb    119,0
69:
defb    67,97,110,39,116,32,99,114,101,97,116,101,32,37,115,0

