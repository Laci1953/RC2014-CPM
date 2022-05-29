include cgen.inc
global  _sub_2D09
global  ncsv, cret, indir
global  _fputchar
global  __ctype_
global  _atoi
global  wrelop
global  _codeFrag
global  amul
global  amul
global  amul
global  _word_AFF8
global  _printf
global  _regNames
global  _sub_2CE0

IF RAM128

global  _GetString
global  _tmpBuf

ENDIF

global  wrelop
global  _nodesize
global  _sub_387A
global  lladd
global  _otherFrag
global  _sub_153A
global  lladd
global  aslladd
global  _sub_14F3
global  wrelop
global  shal
global  aslland
global  wrelop
global  alrsh
global  _sub_46F7
global  _array_AB24
global  _word_AFFB
global  _dopetab
psect   text
_sub_2D09:
call ncsv
defw -24
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
pop     iy
ld      l,(ix+8)
ld      h,(ix+9)
ld      (ix+-19),l
ld      (ix+-18),h
ld      a,0
ld      (ix+-24),a
ld      (ix+-17),a
l11:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
inc     hl
ld      (ix+8),l
ld      (ix+9),h
ld      (ix+-10),a
or      a
jp      z,cret
cp      10
jp      z,l22
cp      13
jp      z,l22
cp      32
jp      z,l17
cp      59
jp      z,cret
cp      65
jp      z,l43
cp      66
jp      z,l43
cp      67
jp      z,l43
cp      68
jp      z,l61
cp      70
jp      z,l43
cp      71
jp      z,l24
cp      76
jp      z,l60
cp      77
jp      z,l43
cp      79
jp      z,l43
cp      80
jp      z,l43
cp      81
jp      z,l43
cp      82
jp      z,l60
cp      83
jp      z,l43
cp      84
jp      z,l43
cp      85
jp      z,l60
cp      86
jp      z,l43
cp      87
jp      z,l43
cp      88
jp      z,l24
cp      90
jp      z,l43
cp      126
jp      z,l43
L17:
ld      a,(ix+-10)
ld      l,a
rla
sbc     a,a
ld      h,a
push    hl
call    _fputchar
L37:
pop     bc
jp      l11
l17:
ld      a,(ix+-24)
or      a
jp      nz,L17
ld      hl,9
push    hl
call    _fputchar
pop     bc
ld      (ix+-24),1
jp      l11
l22:
ld      hl,10
push    hl
call    _fputchar
pop     bc
ld      (ix+-24),0
jp      l11
l24:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
ld      e,a
rla
sbc     a,a
ld      d,a
ld      hl,__ctype_+1
add     hl,de
bit     2,(hl)
jp      z,l25
ld      l,(ix+8)
ld      h,(ix+9)
ld      (ix+-6),l
ld      (ix+-5),h
jp      l26
l27:
ld      l,(ix+8)
ld      h,(ix+9)
inc     hl
ld      (ix+8),l
ld      (ix+9),h
l26:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
ld      e,a
rla
sbc     a,a
ld      d,a
ld      hl,__ctype_+1
add     hl,de
bit     2,(hl)
jp      nz,l27
jp      l29
l25:
ld      (ix+-6),0
ld      (ix+-5),0
l29:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
inc     hl
ld      (ix+8),l
ld      (ix+9),h
ld      (ix+-9),a
ld      (ix+-11),71
l64:
ld      a,(ix+-9)
cp      76
jp      z,l65
cp      78
jp      z,l69
cp      82
jp      z,l68
l71:
ld      a,(ix+-10)
cp      65
jp      z,l94
cp      66
jp      z,l89
cp      67
jp      z,l105
cp      68
jp      z,l72
cp      70
jp      z,l86
cp      71
jp      z,l114
cp      79
jp      z,l93
cp      83
jp      z,l74
cp      84
jp      z,l109
cp      86
jp      z,l95
cp      88
jp      z,l114
cp      90
jp      z,l87
cp      126
jp      z,l120
jp      l11
l43:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
ld      (ix+-11),a
cp      45
jp      z,L9
cp      43
jp      nz,l44
L9:
ld      l,(ix+8)
ld      h,(ix+9)
inc     hl
ld      (ix+8),l
ld      (ix+9),h
ld      a,(hl)
ld      e,a
rla
sbc     a,a
ld      d,a
ld      hl,__ctype_+1
add     hl,de
bit     2,(hl)
jp      z,l45
ld      l,(ix+8)
ld      h,(ix+9)
push    hl
call    _atoi
pop     bc
ld      (ix+-8),l
ld      a,(ix+-11)
cp      45
jp      nz,l46
ld      a,l
neg
ld      (ix+-8),a
l46:
ld      (ix+-11),0
jp      l48
l45:
ld      (ix+-8),0
jp      l48
l49:
ld      l,(ix+8)
ld      h,(ix+9)
inc     hl
ld      (ix+8),l
ld      (ix+9),h
l48:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
ld      e,a
rla
sbc     a,a
ld      d,a
ld      hl,__ctype_+1
add     hl,de
bit     2,(hl)
jp      nz,l49
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
cp      43
jp      z,l49
ld      a,(hl)
cp      45
jp      z,l49
jp      l51
l44:
ld      a,(ix+-11)
cp      62
jp      nz,l52
ld      (ix+-8),0
jp      l56
l55:
ld      l,(ix+8)
ld      h,(ix+9)
inc     hl
ld      (ix+8),l
ld      (ix+9),h
inc     (ix+-8)
l56:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
cp      62
jp      z,l55
jp      l51
l52:
ld      (ix+-11),0
ld      (ix+-8),0
l51:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
inc     hl
ld      (ix+8),l
ld      (ix+9),h
ld      (ix+-9),a
jp      l64
l60:
ld      a,(ix+-10)
ld      (ix+-9),a
ld      (ix+-10),126
L18:
ld      (ix+-11),0
ld      (ix+-8),0
jp      l64
l61:
ld      a,(ix+-10)
ld      (ix+-9),a
jp      L18
l65:
ld      a,(ix+10)
ld      e,a
rla
sbc     a,a
ld      d,a
ld      hl,0
call    wrelop
jp      p,l66
push    iy
pop     hl
ld      (ix+-4),l
ld      (ix+-3),h
ld      a,(ix+10)
add     a,255
ld      (ix+-16),a
push    iy
pop     de
L23:
ld      a,(ix+-16)
ld      l,a
rla
sbc     a,a
ld      h,a
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
ld      (ix+-2),l
ld      (ix+-1),h
jp      l71
l66:
ld      l,(iy+25)
ld      h,(iy+26)
L19:
ld      (ix+-4),l
ld      (ix+-3),h
inc     hl
ld      a,(hl)
add     a,255
ld      (ix+-16),a
ld      e,(ix+-4)
ld      d,(ix+-3)
jp      L23
l68:
ld      l,(iy+27)
ld      h,(iy+28)
jp      L19
l69:
push    iy
pop     hl
ld      (ix+-4),l
ld      (ix+-3),h
ld      a,(ix+10)
ld      (ix+-16),a
jp      l71
l72:
ld      hl,(_word_AFF8)
ld      a,l
or      h
jp      z,l11
push    hl
ld      hl,19f
push    hl
call    _printf
pop     bc
pop     bc
ld      hl,0
L24:
ld      (_word_AFF8),hl
jp      l11
l74:
ld      l,(ix+-4)
ld      h,(ix+-3)
ld      a,(hl)
cp      69
jp      nz,l75
ex      de,hl
ld      hl,25
add     hl,de
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
add     hl,hl
ld      de,_regNames
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
ld      hl,29f
push    hl
call    _printf
L25:
pop     bc
pop     bc
jp      l81
l75:
ld      l,(ix+-4)
ld      h,(ix+-3)
ld      a,(hl)
cp      67
jp      nz,l77
ex      de,hl
ld      hl,25
add     hl,de
ld      e,(hl)
inc     hl
ld      d,(hl)
inc     hl
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
push    hl
push    de
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _sub_2CE0
pop     bc
jp      L25
l77:
ld      l,(ix+-4)
ld      h,(ix+-3)
ld      a,(hl)
cp      68
ex      de,hl
ld      hl,25
jp      nz,L11
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc

IF RAM128

ld      hl,_tmpBuf
push    hl
call    _GetString
pop     bc
ld      hl,_tmpBuf
ex      (sp),hl

ENDIF

ld      hl,39f
L21:
push    hl
call    _printf
jp      L25
L11:
add     hl,de
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc

IF RAM128

ld      hl,_tmpBuf
push    hl
call    _GetString
pop     bc
ld      hl,_tmpBuf
ex      (sp),hl

ENDIF

ld      hl,49f
push    hl
call    _printf
pop     bc
pop     bc
l81:
ld      a,(ix+-11)
or      a
jp      nz,l11
ld      a,(_word_AFF8)
ld      e,a
ld      a,(ix+-8)
add     a,e
ld      (ix+-8),a
ld      hl,0
ld      (_word_AFF8),hl
ld      e,a
rla
sbc     a,a
ld      d,a
call    wrelop
ld      a,(ix+-8)
jp      p,L12
ld      l,a
rla
sbc     a,a
ld      h,a
push    hl
ld      hl,59f
L38:
push    hl
call    _printf
L43:
pop     bc
jp      L37
L12:
or      a
jp      p,l11
ld      a,(ix+-8)
ld      l,a
rla
sbc     a,a
ld      h,a
push    hl
ld      hl,69f
jp      L38
l86:
ld      e,(ix+-4)
ld      d,(ix+-3)
ld      hl,27
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
ld      hl,79f
jp      L21
l87:
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _nodesize
pop     bc
ex      de,hl
ld      hl,0
ld      (ix+-15),e
ld      (ix+-14),d
ld      (ix+-13),l
ld      (ix+-12),h
jp      l88
l89:
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _sub_387A
pop     bc
ex      de,hl
ld      hl,0
ld      (ix+-15),e
ld      (ix+-14),d
ld      (ix+-13),l
ld      (ix+-12),h
l88:
ld      a,(ix+-11)
cp      45
jp      nz,l90
ld      a,(ix+-17)
or      a
jp      nz,l92
ld      e,(ix+-15)
ld      d,(ix+-14)
ld      l,(ix+-13)
ld      h,(ix+-12)
push    hl
push    de
ld      de,(_word_AFF8)
ld      a,d
rla
sbc     a,a
ld      l,a
ld      h,a
call    lladd
ld      (ix+-23),e
ld      (ix+-22),d
ld      (ix+-21),l
ld      (ix+-20),h
ld      hl,0
ld      (_word_AFF8),hl
ld      (ix+-17),1
jp      l92
l90:
ld      e,(ix+-15)
ld      d,(ix+-14)
ld      l,(ix+-13)
ld      h,(ix+-12)
push    hl
push    de
ld      hl,89f
push    hl
call    _printf
L46:
pop     bc
jp      L43
l93:
ld      de,_otherFrag
ld      l,(ix+-4)
ld      h,(ix+-3)
ld      l,(hl)
ld      h,0
add     hl,hl
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
ld      hl,99f
L47:
push    hl
call    _printf
jp      L43
l94:
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _sub_153A
jp      L37
l95:
ld      a,(ix+-11)
cp      45
jp      z,L7
cp      43
jp      nz,l96
L7:
ld      a,(ix+-17)
or      a
jp      nz,l92
ld      e,(ix+-4)
ld      d,(ix+-3)
ld      hl,25
add     hl,de
ld      e,(hl)
inc     hl
ld      d,(hl)
inc     hl
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
push    hl
push    de
ld      de,(_word_AFF8)
ld      a,d
rla
sbc     a,a
ld      l,a
ld      h,a
call    lladd
ld      (ix+-23),e
ld      (ix+-22),d
ld      (ix+-21),l
ld      (ix+-20),h
ld      hl,0
ld      (_word_AFF8),hl
ld      (ix+-17),1
l92:
ld      a,(ix+-11)
cp      45
jp      z,L6
ld      de,1
ld      hl,0
jp      L5
L6:
ld      de,-1
ld      l,e
ld      h,d
L5:
push    hl
push    de
push    ix
pop     de
ld      hl,-23
add     hl,de
call    aslladd
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _sub_14F3
pop     bc
ld      a,l
cp      2
jp      nz,l98
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _nodesize
pop     bc
ld      de,4
call    wrelop
jp      nc,l98
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _nodesize
pop     bc
add     hl,hl
add     hl,hl
add     hl,hl
ld      b,l
ld      hl,1
call    shal
dec     hl
ex      de,hl
ld      hl,0
push    hl
push    de
push    ix
pop     de
ld      hl,-23
add     hl,de
call    aslland
l98:
ld      a,(ix+-23)
or      (ix+-22)
or      (ix+-21)
or      (ix+-20)
jp      z,l99
jp      l100
l101:
ld      l,(ix+8)
ld      h,(ix+9)
dec     hl
ld      (ix+8),l
ld      (ix+9),h
l100:
ld      e,(ix+8)
ld      d,(ix+9)
ld      l,(ix+-19)
ld      h,(ix+-18)
call    wrelop
jp      nc,l102
ld      l,(ix+8)
ld      h,(ix+9)
dec     hl
ld      a,(hl)
cp      10
jp      nz,l101
l102:
ld      hl,10
push    hl
call    _fputchar
jp      L37
l99:
ld      (ix+-17),0
jp      l11
l96:
ld      a,(_word_AFF8)
ld      e,a
ld      a,(ix+-8)
add     a,e
ld      (ix+-8),a
ld      hl,0
ld      (_word_AFF8),hl
ld      b,a
sla     b
sla     b
sla     b
ld      e,(ix+-4)
ld      d,(ix+-3)
ld      hl,25
add     hl,de
ld      e,(hl)
inc     hl
ld      d,(hl)
inc     hl
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
call    alrsh
push    hl
push    de
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _sub_2CE0
jp      L46
l105:
ld      l,(ix+-4)
ld      h,(ix+-3)
ld      a,(hl)
cp      71
ld      e,l
ld      d,h
jp      nz,L13
ld      hl,27
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      hl,6
add     hl,bc
ld      l,(hl)
ld      h,0
push    hl
ld      hl,109f
jp      L47
L13:
ld      hl,25
add     hl,de
ld      e,(hl)
inc     hl
ld      d,(hl)
inc     hl
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
ld      (ix+-15),e
ld      (ix+-14),d
ld      (ix+-13),l
ld      (ix+-12),h
push    hl
push    de
call    _sub_46F7
pop     bc
pop     bc
ld      a,l
or      a
ld      e,(ix+-15)
ld      d,(ix+-14)
ld      l,(ix+-13)
ld      h,(ix+-12)
push    hl
push    de
jp      nz,L14
ld      hl,-1
pop     bc
or      a
sbc     hl,bc
pop     bc
ex      de,hl
ld      hl,-1
sbc     hl,bc
ld      (ix+-15),e
ld      (ix+-14),d
ld      (ix+-13),l
ld      (ix+-12),h
push    hl
push    de
L14:
call    _sub_46F7
pop     bc
pop     bc
ld      e,l
ld      d,0
ld      hl,-1
add     hl,de
srl     h
rr      l
srl     h
rr      l
srl     h
rr      l
ex      de,hl
ld      hl,(_word_AFF8)
add     hl,de
jp      L24
l109:
ld      l,(ix+-4)
ld      h,(ix+-3)
ld      a,(hl)
cp      71
ld      e,l
ld      d,h
jp      nz,L15
ld      hl,27
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      hl,5
add     hl,bc
ld      l,(hl)
ld      h,0
push    hl
ld      hl,119f
jp      L47
L15:
ld      hl,25
add     hl,de
ld      e,(hl)
inc     hl
ld      d,(hl)
inc     hl
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
ld      (ix+-15),e
ld      (ix+-14),d
ld      (ix+-13),l
ld      (ix+-12),h
push    hl
push    de
call    _sub_46F7
pop     bc
pop     bc
ld      a,l
or      a
ld      e,(ix+-15)
ld      d,(ix+-14)
ld      l,(ix+-13)
ld      h,(ix+-12)
push    hl
push    de
jp      nz,L16
ld      hl,-1
pop     bc
or      a
sbc     hl,bc
pop     bc
ex      de,hl
ld      hl,-1
sbc     hl,bc
ld      (ix+-15),e
ld      (ix+-14),d
ld      (ix+-13),l
ld      (ix+-12),h
push    hl
push    de
L16:
call    _sub_46F7
pop     bc
ld      e,l
ld      d,0
ld      hl,-1
add     hl,de
ex      (sp),hl
ld      hl,129f
jp      L47
l114:
ld      a,(ix+-10)
cp      71
jp      z,L2
ld      e,(ix+-4)
ld      d,(ix+-3)
ld      a,(ix+-16)
ld      l,a
rla
sbc     a,a
ld      h,a
add     hl,de
ld      de,9
add     hl,de
ld      l,(hl)
ld      h,0
jp      L1
L2:
ld      e,(ix+-4)
ld      d,(ix+-3)
ld      a,(ix+-16)
ld      l,a
rla
sbc     a,a
ld      h,a
add     hl,de
ld      de,15
add     hl,de
ld      l,(hl)
ld      h,0
L1:
ld      (ix+-7),l
ld      a,(ix+-6)
or      (ix+-5)
jp      z,l115
jp      l116
l117:
ld      l,(ix+-6)
ld      h,(ix+-5)
ld      a,(hl)
inc     hl
ld      (ix+-6),l
ld      (ix+-5),h
ld      e,a
rla
sbc     a,a
ld      d,a
ld      hl,-48
add     hl,de
ex      de,hl
ld      l,(ix+-7)
ld      h,0
add     hl,hl
add     hl,de
ld      de,_array_AB24
add     hl,de
ld      l,(hl)
ld      (ix+-7),l
l116:
ld      l,(ix+-6)
ld      h,(ix+-5)
ld      a,(hl)
ld      e,a
rla
sbc     a,a
ld      d,a
ld      hl,__ctype_+1
add     hl,de
bit     2,(hl)
jp      nz,l117
l115:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
cp      43
jp      z,l119
ld      a,(_word_AFF8)
ld      e,a
ld      a,(ix+-7)
add     a,e
ld      (ix+-7),a
ld      hl,0
ld      (_word_AFF8),hl
l119:
ld      de,_regNames
ld      l,(ix+-7)
ld      h,0
add     hl,hl
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
ld      hl,139f
jp      L47
l120:
ld      a,(ix+-9)
cp      85
jp      nz,l121
ld      a,(_word_AFFB)
ld      e,a
ld      a,(ix+-8)
add     a,e
ld      (ix+-8),a
ld      de,_dopetab
ld      l,(iy+0)
ld      h,0
add     hl,hl
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
bit     1,b
jp      z,l122
ld      a,(ix+10)
or      a
jp      z,l122
dec     (ix+-8)
l122:
ld      a,(ix+-8)
ld      l,a
rla
sbc     a,a
ld      h,a
push    hl
ld      hl,149f
jp      L47
l121:
ld      a,(ix+-8)
ld      e,a
rla
sbc     a,a
ld      d,a
ld      hl,(_word_AFF8)
add     hl,de
ld      (_word_AFF8),hl
ld      l,(ix+-16)
push    hl
ld      e,(ix+-2)
ld      d,(ix+-1)
ld      hl,8
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _sub_2D09
jp      L46
psect   data
19:
defb    37,100,0
29:
defb    37,115,0
39:
defb    37,115,0
49:
defb    37,115,0
59:
defb    43,37,100,0
69:
defb    37,100,0
79:
defb    37,100,0
89:
defb    37,108,117,0
99:
defb    37,115,0
109:
defb    37,100,0
119:
defb    37,100,0
129:
defb    37,100,0
139:
defb    37,115,0
149:
defb    37,100,0
psect   text

