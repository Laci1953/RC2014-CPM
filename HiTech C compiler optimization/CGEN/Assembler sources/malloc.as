global  ncsv, cret, indir
global  wrelop
psect   text
_setSize:
global csv
call csv
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
pop     iy
ld      e,(ix+8)
ld      d,(ix+9)
ld      hl,128
call    wrelop
jp      p,l11
ld      l,(ix+8)
ld      h,(ix+9)
dec     hl
dec     hl
dec     hl
ld      (iy+0),l
ld      (iy+1),h
ld      de,3
add     iy,de
ld      (iy+-1),-128
jp      l12
l11:
ld      a,(ix+8)
add     a,255
ld      (iy+0),a
inc     iy
l12:
;push    iy
;pop     hl
;jp      cret
jp	pushIYpopHLcret
global  _free
_pack:
call ncsv
defw -9
ld      (ix+-9),0
l14:
ld      hl,_freeList
ld      (ix+-8),l
ld      (ix+-7),h
jp      l18
l15:
bit     7,(iy+-1)
push    iy
pop     hl
dec     hl
jp      z,L7
dec     hl
dec     hl
L7:
ld      (ix+-4),l
ld      (ix+-3),h
ld      hl,_freeList
ld      (ix+-6),l
ld      (ix+-5),h
jp      l22
l19:
ld      e,(ix+-2)
ld      d,(ix+-1)
ld      l,e
ld      h,d
dec     hl
bit     7,(hl)
jp      nz,L10
ld      l,(hl)
ld      h,0
jp      L5
L10:
dec     hl
dec     hl
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
L5:
add     hl,de
ld      e,(ix+-4)
ld      d,(ix+-3)
or      a
sbc     hl,de
jp      nz,l21
push    iy
pop     de
ld      l,(ix+-6)
ld      h,(ix+-5)
or      a
sbc     hl,de
jp      nz,l24
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      l,(ix+-8)
ld      h,(ix+-7)
ld      (hl),c
inc     hl
ld      (hl),b
jp      l25
l24:
ld      e,(ix+-2)
ld      d,(ix+-1)
ld      l,(ix+-8)
ld      h,(ix+-7)
or      a
sbc     hl,de
jp      nz,l26
ld      e,(iy+0)
ld      d,(iy+1)
ld      l,(ix+-6)
ld      h,(ix+-5)
ld      (hl),e
inc     hl
ld      (hl),d
jp      l25
l26:
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      l,(ix+-6)
ld      h,(ix+-5)
ld      (hl),c
inc     hl
ld      (hl),b
ld      e,(iy+0)
ld      d,(iy+1)
ld      l,(ix+-8)
ld      h,(ix+-7)
ld      (hl),e
inc     hl
ld      (hl),d
l25:
ld      l,(ix+-2)
ld      h,(ix+-1)
dec     hl
bit     7,(hl)
jp      z,L3
dec     hl
dec     hl
L3:
ld      (ix+-2),l
ld      (ix+-1),h
push    iy
pop     de
bit     7,(iy+-1)
jp      nz,L2
ld      l,(iy+-1)
ld      h,0
jp      L1
L2:
ld      l,(iy+-3)
ld      h,(iy+-2)
L1:
add     hl,de
ld      e,(ix+-2)
ld      d,(ix+-1)
or      a
sbc     hl,de
push    hl
ld      l,e
ld      h,d
push    hl
call    _setSize
pop     bc
ex      (sp),hl
call    _free
pop     bc
ld      (ix+-9),1
jp      l14
l21:
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      (ix+-6),l
ld      (ix+-5),h
l22:
ld      l,(ix+-6)
ld      h,(ix+-5)
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      (ix+-2),c
ld      (ix+-1),b
ld      a,c
or      b
jp      nz,l19
push    iy
pop     hl
ld      (ix+-8),l
ld      (ix+-7),h
l18:
ld      l,(ix+-8)
ld      h,(ix+-7)
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
pop     iy
ld      a,c
or      b
jp      nz,l15
ld      l,(ix+-9)
jp      cret
global  _malloc
global  wrelop
global  wrelop
global  wrelop
global  wrelop
global  _sbrk
_malloc:
call ncsv
defw -5
ld      de,2
ld      l,(ix+6)
ld      h,(ix+7)
call    wrelop
jp      nc,l29
ld      (ix+6),2
ld      (ix+7),0
l29:
ld      (ix+-5),0
l32:
ld      iy,_freeList
jp      l36
l33:
ld      e,(ix+6)
ld      d,(ix+7)
ld      l,(ix+-2)
ld      h,(ix+-1)
dec     hl
bit     7,(hl)
jp      nz,L22
ld      l,(hl)
ld      h,0
jp      L20
L22:
dec     hl
dec     hl
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
L20:
call    wrelop
ld      l,(ix+-2)
ld      h,(ix+-1)
jp      c,L25
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      (iy+0),c
ld      (iy+1),b
dec     hl
push    hl
pop     iy
bit     7,(iy+-1)
jp      nz,L19
ld      l,(iy+-1)
ld      h,0
jp      L18
L19:
ld      l,(iy+-3)
ld      h,(iy+-2)
L18:
ex      de,hl
push    ix
pop     hl
dec     hl
dec     hl
dec     hl
dec     hl
ld      (hl),e
inc     hl
ld      (hl),d
ld      l,(ix+6)
ld      h,(ix+7)
inc     hl
inc     hl
inc     hl
inc     hl
call    wrelop
push    iy
jp      nc,L23
pop     de
ld      l,(ix+6)
ld      h,(ix+7)
add     hl,de
ld      (ix+-2),l
ld      (ix+-1),h
ld      e,(ix+6)
ld      d,(ix+7)
ld      l,(ix+-4)
ld      h,(ix+-3)
or      a
sbc     hl,de
push    hl
ld      l,(ix+-2)
ld      h,(ix+-1)
push    hl
call    _setSize
pop     bc
ex      (sp),hl
call    _free
pop     bc
bit     7,(iy+-1)
push    iy
pop     hl
dec     hl
jp      z,L16
dec     hl
dec     hl
L16:
push    hl
pop     iy
ld      l,(ix+6)
ld      h,(ix+7)
L26:
push    hl
push    iy
call    _setSize
;pop     bc
;pop     bc
;jp      cret
jp	popBCpopBCcret
L23:
;pop     hl
;jp      cret
jp	popHLcret
L25:
push    hl
pop     iy
l36:
ld      l,(iy+0)
ld      h,(iy+1)
ld      (ix+-2),l
ld      (ix+-1),h
ld      a,l
or      h
jp      nz,l33
ld      a,(ix+-5)
or      a
jp      nz,l31
call    _pack
ld      a,l
ld      (ix+-5),a
or      a
jp      nz,l32
l31:
ld      de,128
ld      l,(ix+6)
ld      h,(ix+7)
call    wrelop
jp      c,L13
ld      hl,3
jp      L12
L13:
ld      hl,1
L12:
ld      (ix+-4),l
ld      (ix+-3),h
ex      de,hl
ld      l,(ix+6)
ld      h,(ix+7)
add     hl,de
push    hl
call    _sbrk
pop     bc
push    hl
pop     iy
ld      de,-1
or      a
sbc     hl,de
jp      nz,l40
;ld      hl,0
;jp      cret
jp	ldHL0cret
l40:
ld      e,(ix+-4)
ld      d,(ix+-3)
ld      l,(ix+6)
ld      h,(ix+7)
add     hl,de
jp      L26
global  wrelop
_free:
global csv
call csv
ld      iy,_freeList
jp      l45
l44:
ld      l,(iy+0)
ld      h,(iy+1)
push    hl
pop     iy
l45:
ld      a,(iy+0)
or      (iy+1)
jp      z,l43
ld      l,(ix+6)
ld      h,(ix+7)
dec     hl
bit     7,(hl)
jp      nz,L33
ld      l,(hl)
ld      h,0
jp      L30
L33:
dec     hl
dec     hl
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
L30:
ex      de,hl
ld      l,(iy+0)
ld      h,(iy+1)
dec     hl
bit     7,(hl)
jp      nz,L34
ld      l,(hl)
ld      h,0
jp      L28
L34:
dec     hl
dec     hl
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
L28:
call    wrelop
jp      c,l44
l43:
ld      e,(iy+0)
ld      d,(iy+1)
ld      l,(ix+6)
ld      h,(ix+7)
ld      (hl),e
inc     hl
ld      (hl),d
dec     hl
ld      (iy+0),l
ld      (iy+1),h
jp      cret
psect   bss
_freeList:
defs    2
psect   text

