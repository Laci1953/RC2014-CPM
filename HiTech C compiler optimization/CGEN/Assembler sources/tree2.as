include cgen.inc
global  _parseExpr
global  ncsv, cret, indir
global  _getToken
global  __ctype_
global  _sub_415E
global  _atol
global  _allocNode
global  _sub_265
global  _prGlobalDef
global  _sub_627
global  _typeDouble
global  _strlen

IF RAM128

global  _MyAlloc
global  _PutString

ELSE

global  _allocMem
global  _strcpy

ENDIF

global  _newLocal
global  _sub_1B2
global  _dopetab
global  _parseVariable
global  _parseMembers
global  _parseEnum
global  _fatalErr
global  _sub_43EF
psect   text
_parseExpr:
call ncsv
defw -8
l11:
call    _getToken
ld      (ix+-4),l
ld      (ix+-3),h

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
ld      l,(ix+-4)
ld      h,(ix+-3)
ld      a,(hl)
jp      nz,L7
cp      45
jp      nz,L8
inc     hl

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
ld      l,(ix+-4)
ld      h,(ix+-3)
ld      a,(hl)
jp      z,L9
L7:
cp      45
jp      nz,l15
ld      (ix+-8),1
ld      (ix+-7),0
ld      l,(ix+-4)
ld      h,(ix+-3)
inc     hl
ld      (ix+-4),l
ld      (ix+-3),h
jp      l16
l15:
ld      (ix+-8),0
ld      (ix+-7),0
l16:
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _atol
ex      (sp),hl
push    de
call    _sub_415E
pop     bc
pop     bc
push    hl
pop     iy
ld      a,(ix+-8)
or      (ix+-7)
jp      z,L14
ld      e,(iy+25)
ld      d,(iy+26)
ld      l,(iy+27)
ld      h,(iy+28)
push    hl
push    de
ld      hl,0
pop     bc
or      a
sbc     hl,bc
pop     bc
ex      de,hl
ld      hl,0
sbc     hl,bc
ld      (iy+25),e
ld      (iy+26),d
ld      (iy+27),l
ld      (iy+28),h
L14:
;push    iy
;pop     hl
;jp      cret
jp	pushIYpopHLcret
L8:
ld      a,(hl)
L9:
cp      95
jp      z,L3
ld      l,(ix+-4)
ld      h,(ix+-3)

;ld      a,(hl)
;ld      e,a
;rla
;sbc     a,a
;ld      d,a
ld	e,(hl)
ld	d,0

ld      hl,__ctype_+1
add     hl,de
ld      a,(hl)
and     3
or      a
jp      z,l18
L3:
call    _allocNode
push    hl
pop     iy
ld      (hl),66
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _sub_265
pop     bc
ld      (iy+25),l
ld      (iy+26),h
ex      de,hl
ld      hl,15
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      (iy+23),c
ld      (iy+24),b
ld      hl,13
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      (iy+21),c
ld      (iy+22),b
ld      hl,21
add     hl,de
ld      a,(hl)
cp      3
jp      nz,L14
ld      l,e
ld      h,d
inc     hl
inc     hl
inc     hl
bit     1,(hl)
jp      nz,L14
dec     hl
dec     hl
dec     hl
push    hl
call    _prGlobalDef
pop     bc
jp      L14
l18:
ld      l,(ix+-4)
ld      h,(ix+-3)
ld      a,(hl)
cp      96
jp      nz,l20
call    _allocNode
push    hl
pop     iy
push    hl
pop     de
ld      hl,21
add     hl,de
push    hl
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _sub_627
pop     bc
pop     bc
ld      (iy+23),l
ld      (iy+24),h
ld      (iy+0),65
jp      L14
l20:
ld      l,(ix+-4)
ld      h,(ix+-3)
ld      a,(hl)
cp      46
jp      nz,L10
inc     hl
ld      a,(hl)
or      a
dec     hl
jp      z,L10
inc     hl
ld      a,(hl)
cp      46
jp      z,l21
call    _allocNode
push    hl
pop     iy
ld      hl,(_typeDouble)
ld      (iy+23),l
ld      (iy+24),h
ld      l,(ix+-4)
ld      h,(ix+-3)
push    hl
call    _strlen
ex      (sp),hl

IF RAM128

call    _MyAlloc

ELSE

call    _allocMem

ENDIF

pop     bc
ld      (iy+25),l
ld      (iy+26),h

IF RAM128

push    hl
ld      l,(ix+-4)
ld      h,(ix+-3)
inc     hl
push    hl
call    _PutString

ELSE

ld      l,(ix+-4)
ld      h,(ix+-3)
inc     hl
push    hl
ld      l,(iy+25)
ld      h,(iy+26)
push    hl
call    _strcpy

ENDIF

pop     bc
pop     bc
call    _newLocal
ld      (iy+27),l
ld      (iy+28),h
ld      (iy+0),68
jp      L14
l21:
ld      l,(ix+-4)
ld      h,(ix+-3)
L10:
push    hl
call    _sub_1B2
pop     bc
ld      a,l
rla
sbc     a,a
ld      h,a
ld      (ix+-8),l
ld      (ix+-7),h
ld      de,_dopetab
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
ld      (ix+-6),l
ld      (ix+-5),h
ld      h,(ix+-5)
ld      a,h
or      a
jp      nz,l11
ld      a,l
or      a
jp      z,l32
cp      4
jp      z,l33
cp      8
jp      z,l34
cp      12
jp      nz,l11
ld      l,(ix+-8)
ld      h,(ix+-7)
ld      a,h
or      a
jp      nz,l31
ld      a,l
cp      52
jp      z,l30
cp      55
jp      z,l29
cp      56
jp      z,l29
cp      57
jp      nz,l31
call    _parseVariable
jp      l11
l29:
ld      l,(ix+-8)
ld      h,(ix+-7)
push    hl
call    _parseMembers
pop     bc
jp      l11
l30:
call    _parseEnum
jp      l11
l31:
ld      hl,19f
push    hl
call    _fatalErr
pop     bc
l32:
ld      hl,0
push    hl
L15:
push    hl
ld      l,(ix+-8)
push    hl
call    _sub_43EF
pop     bc
;pop     bc
;pop     bc
;jp      cret
jp	popBCpopBCcret
l33:
ld      hl,0
push    hl
call    _parseExpr
jp      L15
l34:
call    _parseExpr
ld      (ix+-2),l
ld      (ix+-1),h
call    _parseExpr
push    hl
ld      l,(ix+-2)
ld      h,(ix+-1)
jp      L15
psect   data
19:
defb    69,120,112,114,101,115,115,105,111,110,32,101,114,114,111,114
defb    0
psect   text

