global  _sub_283E
global  ncsv, cret, indir
global  _codeFrag
global  amul
global  __ctype_
global  wrelop
global  _array_AB24
global  _array_AAE8
global  _byte_AFFA
global  wrelop
global  _sub_2B2A
global  _word_B017
psect   text
_sub_283E:
call ncsv
defw -13
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
pop     iy
push    hl
pop     de
ld      l,(ix+8)
ld      h,(ix+9)
add     hl,de
inc     hl
inc     hl
inc     hl
ld      l,(hl)
ld      h,0
ld      de,10
call    amul
ld      de,_codeFrag
add     hl,de
ld      (ix+-4),l
ld      (ix+-3),h
ld      (ix+-13),0
ld      (ix+-12),0
ex      de,hl
ld      hl,8
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      (ix+-2),c
ld      (ix+-1),b
l13:
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      a,(hl)
L17:
or      a
jp      nz,l16
ld      de,(_word_B017)
ld      a,(ix+-13)
and     e
ld      l,a
ld      a,(ix+-12)
and     d
ld      h,a
jp      cret
l16:
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      a,(hl)
inc     hl
ld      (ix+-2),l
ld      (ix+-1),h
ld      (ix+-9),a
cp      65
jp      z,l53
cp      66
jp      z,l53
cp      67
jp      z,l53
cp      68
jp      z,l13
cp      70
jp      z,l53
cp      71
jp      z,l18
cp      76
jp      z,l60
cp      77
jp      z,l53
cp      79
jp      z,l53
cp      80
jp      z,l53
cp      81
jp      z,l53
cp      82
jp      z,l63
cp      83
jp      z,l53
cp      84
jp      z,l53
cp      85
jp      z,l13
cp      86
jp      z,l53
cp      87
jp      z,l53
cp      88
jp      z,l18
cp      90
jp      z,l53
cp      126
jp      z,l57
jp      l13
l18:
ld      l,(ix+-2)
ld      h,(ix+-1)

;ld      a,(hl)
;ld      e,a
;rla
;sbc     a,a
;ld      d,a
ld	e,(hl)
ld	d,0

ld      hl,__ctype_+1
add     hl,de
bit     2,(hl)
jp      z,l19
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      (ix+-8),l
ld      (ix+-7),h
jp      l20
l21:
ld      l,(ix+-2)
ld      h,(ix+-1)
inc     hl
ld      (ix+-2),l
ld      (ix+-1),h
l20:
ld      l,(ix+-2)
ld      h,(ix+-1)

;ld      a,(hl)
;ld      e,a
;rla
;sbc     a,a
;ld      d,a
ld	e,(hl)
ld	d,0

ld      hl,__ctype_+1
add     hl,de
bit     2,(hl)
jp      nz,l21
jp      l25
l19:
ld      (ix+-8),0
ld      (ix+-7),0
l25:
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      a,(hl)
inc     hl
ld      (ix+-2),l
ld      (ix+-1),h
cp      76
jp      z,l27
cp      78
jp      z,l26
cp      82
jp      z,l30
l24:
ld      a,(ix+-9)
cp      83
jp      nz,l31
ld      l,(ix+-6)
ld      h,(ix+-5)
ld      a,(hl)
cp      69
jp      nz,l32
ex      de,hl
ld      hl,25
add     hl,de
ld      l,(hl)
ld      (ix+-9),l
jp      l34
l26:
ld      l,(ix+8)
ld      h,(ix+9)
ld      (ix+-11),l
ld      (ix+-10),h
push    iy
pop     hl
ld      (ix+-6),l
ld      (ix+-5),h
jp      l24
l27:
ld      e,(ix+8)
ld      d,(ix+9)
ld      hl,0
call    wrelop
jp      p,l28
push    iy
pop     hl
ld      (ix+-6),l
ld      (ix+-5),h
ld      l,(ix+8)
ld      h,(ix+9)
L15:
dec     hl
ld      (ix+-11),l
ld      (ix+-10),h
jp      l24
l28:
ld      l,(iy+25)
ld      h,(iy+26)
L13:
ld      (ix+-6),l
ld      (ix+-5),h
inc     hl
ld      l,(hl)
ld      h,0
jp      L15
l30:
ld      l,(iy+27)
ld      h,(iy+28)
jp      L13
l32:
ld      (ix+-9),0
jp      l34
l31:
ld      a,(ix+-9)
cp      71
ld      e,(ix+-11)
ld      d,(ix+-10)
ld      l,(ix+-6)
ld      h,(ix+-5)
jp      z,L14
add     hl,de
ld      de,9
add     hl,de
ld      l,(hl)
ld      h,0
jp      L11
L14:
add     hl,de
ld      de,15
add     hl,de
ld      l,(hl)
ld      h,0
L11:
ld      (ix+-9),l
ld      a,(ix+-8)
or      (ix+-7)
jp      z,l34
jp      l36
l37:
ld      l,(ix+-8)
ld      h,(ix+-7)
ld      a,(hl)
inc     hl
ld      (ix+-8),l
ld      (ix+-7),h
ld      e,a
rla
sbc     a,a
ld      d,a
ld      hl,-48
add     hl,de
ex      de,hl
ld      l,(ix+-9)
ld      h,0
add     hl,hl
add     hl,de
ld      de,_array_AB24
add     hl,de
ld      l,(hl)
ld      (ix+-9),l
l36:
ld      l,(ix+-8)
ld      h,(ix+-7)

;ld      a,(hl)
;ld      e,a
;rla
;sbc     a,a
;ld      d,a
ld	e,(hl)
ld	d,0

ld      hl,__ctype_+1
add     hl,de
bit     2,(hl)
jp      nz,l37
l34:
ld      de,_array_AAE8
ld      l,(ix+-9)
ld      h,0
add     hl,hl
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      l,(ix+-13)
ld      h,(ix+-12)
ld      a,l
or      c
ld      l,a
ld      a,h
or      b
ld      h,a
ld      (ix+-13),l
ld      (ix+-12),h
ld      a,(ix+-9)
or      a
jp      z,l13
ld      (_byte_AFFA),a
jp      l13
l54:
ld      l,(ix+-2)
ld      h,(ix+-1)
inc     hl
ld      (ix+-2),l
ld      (ix+-1),h
l53:
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      a,(hl)
or      a
ld      a,(hl)
jp      z,L17
cp      78
ld      a,(hl)
jp      z,L17
cp      76
ld      a,(hl)
jp      z,L17
cp      82
jp      nz,l54
jp      l13
l58:
ld      l,(ix+-2)
ld      h,(ix+-1)
inc     hl
ld      (ix+-2),l
ld      (ix+-1),h
l57:
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      a,(hl)
or      a
ld      a,(hl)
jp      z,L17
cp      76
ld      a,(hl)
jp      z,L17
cp      82
jp      nz,l58
jp      l13
l60:
ld      e,(ix+8)
ld      d,(ix+9)
ld      hl,0
call    wrelop
jp      p,l61
ld      l,(ix+8)
ld      h,(ix+9)
dec     hl
push    hl
push    iy
call    _sub_283E
pop     bc
L18:
pop     bc
ex      de,hl
ld      l,(ix+-13)
ld      h,(ix+-12)
ld      a,l
or      e
ld      l,a
ld      a,h
or      d
ld      h,a
ld      (ix+-13),l
ld      (ix+-12),h
jp      l13
l61:
ld      l,(iy+25)
ld      h,(iy+26)
L16:
push    hl
call    _sub_2B2A
jp      L18
l63:
ld      l,(iy+27)
ld      h,(iy+28)
jp      L16
global  _dopetab
_sub_2B2A:
global csv
call csv
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
pop     iy
ld      a,(iy+1)
or      a
jp      nz,l67
ld      de,_dopetab
ld      l,(iy+0)
ld      h,0
add     hl,hl
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      a,c
and     12
ld      l,a
xor     a
ld      h,a
ld      a,l
or      h
jp      nz,l68
;ld      hl,0
;jp      cret
jp	ldHL0cret
l68:
ld      l,(iy+25)
ld      h,(iy+26)
push    hl
call    _sub_2B2A
L19:
;pop     bc
;jp      cret
jp	popBCcret
l67:
ld      l,(iy+1)
ld      h,0
dec     hl
push    hl
push    iy
call    _sub_283E
pop     bc
jp      L19
global  _sub_2B79
global  _sub_6589
_sub_2B79:
global csv
call csv
push hl
push hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
pop     iy
push    hl
pop     de
ld      l,(iy+1)
ld      h,0
add     hl,de
ld      de,8
add     hl,de
ld      l,(hl)
ld      (ix+-1),l
xor     a
ld      (_byte_AFFA),a
push    iy
call    _sub_2B2A
pop     bc
ld      (ix+-3),l
ld      (ix+-2),h
ld      a,(ix+-1)
or      a
jp      z,l70
ld      l,a
ld      h,0
push    hl
ld      l,(ix+-3)
ld      h,(ix+-2)
push    hl
call    _sub_6589
pop     bc
pop     bc
ld      a,l
cp      (ix+-1)
jp      nz,l70
ld      l,(ix+-1)
jp      cret
l70:
ld      a,(_byte_AFFA)
ld      l,a
jp      cret

