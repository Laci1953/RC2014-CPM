include cgen.inc
global  _sub_1B2
global  ncsv, cret, indir
psect   data
F614:
defw    19f
defw    29f
defw    39f
defw    49f
defw    59f
defw    69f
defw    79f
defw    89f
defw    99f
defw    109f
defw    119f
defw    129f
defw    139f
defw    149f
defw    159f
defw    169f
defw    179f
defw    189f
defw    199f
defw    209f
defw    219f
defw    229f
defw    239f
defw    249f
defw    259f
defw    269f
defw    279f
defw    289f
defw    299f
defw    309f
defw    319f
defw    329f
defw    339f
defw    349f
defw    359f
defw    369f
defw    379f
defw    389f
defw    399f
defw    409f
defw    419f
defw    429f
defw    439f
defw    449f
defw    459f
defw    469f
defw    479f
defw    489f
defw    499f
defw    509f
defw    519f
defw    529f
defw    539f
defw    549f
defw    559f
defw    569f
defw    579f
defw    589f
defw    599f
defw    609f
defw    619f
defw    629f
defw    639f
defw    649f
defw    659f
defw    669f
defw    679f
defw    689f
defw    699f
defw    709f
defw    719f
defw    729f
F615:
defb    65
global  _strcmp
global  brelop
psect   text
_sub_1B2:
global csv
call csv
push hl
push hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
pop     iy
ld      (ix+-2),0
ld      a,(F615)
ld      (ix+-1),a
l13:
ld      e,(ix+-2)
ld      d,0
ld      l,(ix+-1)
ld      h,d
add     hl,de
srl     h
rr      l
ld      (ix+-3),l
ld      de,F614
ld      h,0
add     hl,hl
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
push    iy
call    _strcmp
pop     bc
pop     bc
ld      a,l
ld      (ix+-4),a
or      a
jp      nz,l14
ld      l,(ix+-3)
jp      cret
l14:
ld      a,(ix+-4)
or      a
ld      a,(ix+-3)
jp      p,L1
add     a,255
ld      (ix+-1),a
jp      l11
L1:
inc     a
ld      (ix+-2),a
l11:
ld      b,(ix+-2)
ld      a,(ix+-1)
call    brelop
jp      nc,l13
ld      l,255
jp      cret
global  _gethashptr
global  _hashtab
global  lmod
_gethashptr:
global csv
call csv
push hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
pop     iy
ld      (ix+-2),0
ld      (ix+-1),0
jp      l21
l18:
ld      e,(iy+0)
ld      d,0
ld      l,(ix+-2)
ld      h,(ix+-1)
add     hl,hl
add     hl,de
ld      (ix+-2),l
ld      (ix+-1),h
inc     iy
l21:
ld      a,(iy+0)
or      a
jp      nz,l18
ld      de,101
ld      l,(ix+-2)
ld      h,(ix+-1)
call    lmod
add     hl,hl
ld      de,_hashtab
add     hl,de
jp      cret
global  _sub_265
IF RAM128
global  _GetString
global  _tmpBuf
global  _MyAlloc
global  _PutString
ENDIF
global  _allocMem
global  _strlen
global  _nstdpth
_sub_265:
global csv
call csv
push hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _gethashptr
pop     bc
ld      (ix+-2),l
ld      (ix+-1),h
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
pop     iy
jp      l26
l23:

IF RAM128

ld      l,(iy+0)
ld      h,(iy+1)
push    hl
ld      hl,_tmpBuf
push    hl
call    _GetString
pop     bc
ld      l,(ix+6)
ld      h,(ix+7)
ex      (sp),hl
ld      hl,_tmpBuf

ELSE

ld      l,(ix+6)
ld      h,(ix+7)
push    hl
ld      l,(iy+0)
ld      h,(iy+1)

ENDIF

push    hl
call    _strcmp
pop     bc
pop     bc
ld      a,l
or      h
jp      nz,l25
L2:
;push    iy
;pop     hl
;jp      cret
jp	pushIYpopHLcret
l25:
ld      l,(iy+7)
ld      h,(iy+8)
push    hl
pop     iy
l26:
push    iy
pop     hl
ld      a,l
or      h
jp      nz,l23
ld      hl,22
push    hl
call    _allocMem
pop     bc
push    hl
pop     iy
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      (iy+7),c
ld      (iy+8),b
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _strlen
pop     bc
inc     hl
push    hl

IF RAM128

call    _MyAlloc
ld      (iy+0),l
ld      (iy+1),h
ex      (sp),hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _PutString

ELSE

call    _allocMem
ld      (iy+0),l
ld      (iy+1),h
ld      l,(ix+6)
ld      h,(ix+7)
ex      (sp),hl
ld      l,(iy+0)
ld      h,(iy+1)
push    hl
call    _strcpy

ENDIF

pop     bc
pop     bc
ld      a,(_nstdpth)
ld      (iy+4),a
push    iy
pop     de
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      (hl),e
inc     hl
ld      (hl),d
jp      L2
global  _getToken
global  _fgetchar
global  __ctype_
global  _lineno
global  _atoi
global  _strncpy
global  _progname
global  _fputchar
psect   bss
F628:
defs    60
psect   text
_getToken:
global csv
call csv
push hl
push hl
ld      (ix+-4),0
ld      (ix+-3),0
l29:
ld      iy,F628
l33:
call    _fgetchar
ld      (ix+-2),l

;ld      (ix+-1),h

;ld      de,-1
;or      a
;sbc     hl,de
ld	a,l
inc	a

jp      z,l34
ld      e,(ix+-2)

;ld      d,(ix+-1)
ld	d,0

ld      hl,__ctype_+1
add     hl,de
bit     3,(hl)
jp      nz,l33
l34:

;ld      de,-1
;ld      l,(ix+-2)
;ld      h,(ix+-1)
;or      a
;sbc     hl,de
ld	a,(ix+-2)
inc	a

jp      nz,l35

;ld      l,e
;ld      h,d
;jp      cret
jp	ldHLFFFFcret

l35:
ld      a,(ix+-2)
ld      (iy+0),a
inc     iy
l38:
call    _fgetchar
ld      (ix+-2),l
;ld      (ix+-1),h
ld      (iy+0),l
inc     iy

;ld      de,-1
;or      a
;sbc     hl,de
ld	a,l
inc	a

jp      z,l37
ld      e,(ix+-2)

;ld      d,(ix+-1)
ld	d,0

ld      hl,__ctype_+1
add     hl,de
bit     3,(hl)
jp      z,l38
l37:
ld      de,-1
add     iy,de
ld      (iy+0),0
ld      a,(F628)
cp      34
jp      nz,l39
ld      hl,F628+1
push    hl
call    _atoi
pop     bc
ld      (_lineno),hl

;ld      de,10
;ld      l,(ix+-2)
;ld      h,(ix+-1)
;or      a
;sbc     hl,de
ld	a,(ix+-2)
cp	10

ld      hl,1
jp      nz,L3
dec     hl
L3:
ld      (ix+-4),l
ld      (ix+-3),h
jp      l29
l39:
ld      a,(ix+-4)
or      (ix+-3)
jp      z,l41
ld      hl,39
push    hl
ld      hl,F628
push    hl
ld      hl,_progname
push    hl
call    _strncpy
pop     bc
pop     bc
pop     bc
ld      (ix+-4),0
ld      (ix+-3),0
jp      l29
l41:
ld      a,(F628)
cp      59
jp      nz,l43
ld      hl,F628+1
ld      a,(hl)
cp      59
jp      nz,l43
l46:
call    _fgetchar
ld      (ix+-2),l
;ld      (ix+-1),h
push    hl
call    _fputchar
pop     bc

;ld      de,10
;ld      l,(ix+-2)
;ld      h,(ix+-1)
;or      a
;sbc     hl,de
ld	a,(ix+-2)
cp	10

jp      nz,l46
jp      l29
l43:
ld      hl,F628
jp      cret
global  _leaveBlock
global  _word_B017
global  _array_AAE8
global  _sub_5CF5
global  wrelop

IF RAM128

global  _Free128

ENDIF

global  _free
global  _lvlidx
global  _array_AE57
global  _array_AEDB
global  _sub_BEE
global  _prFrameTail
global  _array_AE13
_leaveBlock:
call ncsv
defw -12
ld      (ix+-4),101
ld      (ix+-3),0
ld      hl,_hashtab
ld      (ix+-2),l
ld      (ix+-1),h
l51:
ld      (ix+-6),0
ld      (ix+-5),0
ld      l,(ix+-2)
ld      h,(ix+-1)
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
L14:
pop     iy
jp      l52
l53:
ld      a,(iy+2)
cp      57
jp      nz,l55
ld      a,(iy+21)
cp      2
jp      nz,l56
ld      de,_array_AAE8
ld      l,(iy+19)
ld      h,(iy+20)
add     hl,hl
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      hl,(_word_B017)
ld      a,l
or      c
ld      l,a
ld      a,h
or      b
ld      h,a
ld      (_word_B017),hl
l56:
ld      a,(iy+21)
cp      4
jp      z,L12
cp      3
jp      nz,l57
L12:
ld      hl,0
push    hl
push    iy
call    _sub_5CF5
pop     bc
pop     bc
l57:
ld      de,(_nstdpth)
ld      hl,0
call    wrelop
jp      nc,l55
ld      a,(iy+21)
cp      3
jp      nz,l55
ld      l,(iy+7)
ld      h,(iy+8)
ld      (ix+-8),l
ld      (ix+-7),h
ld      l,(ix+-6)
ld      h,(ix+-5)
ld      (iy+7),l
ld      (iy+8),h
push    iy
pop     hl
ld      (ix+-6),l
ld      (ix+-5),h
dec     (iy+4)
ld      l,(ix+-8)
ld      h,(ix+-7)
push    hl
jp      L14
l55:
ld      l,(iy+0)
ld      h,(iy+1)
push    hl

IF RAM128

call    _Free128

ELSE

call    _free

ENDIF

pop     bc
ld      a,(iy+2)
cp      56
jp      z,L10
cp      55
jp      nz,l59
L10:
ld      l,(iy+19)
ld      h,(iy+20)
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      (ix+-12),c
ld      (ix+-11),b
inc     hl
ld      (ix+-10),l
ld      (ix+-9),h
jp      l60
l61:
ld      l,(ix+-10)
ld      h,(ix+-9)
ld      c,(hl)
inc     hl
ld      b,(hl)
inc     hl
ld      (ix+-10),l
ld      (ix+-9),h
push    bc
call    _free
pop     bc
l60:
ld      l,(ix+-12)
ld      h,(ix+-11)
dec     hl
ld      (ix+-12),l
ld      (ix+-11),h
inc     hl
ld      a,l
or      h
jp      nz,l61
ld      l,(iy+19)
ld      h,(iy+20)
push    hl
call    _free
pop     bc
l59:
ld      l,(iy+7)
ld      h,(iy+8)
ld      (ix+-8),l
ld      (ix+-7),h
push    iy
call    _free
pop     bc
ld      l,(ix+-8)
ld      h,(ix+-7)
push    hl
pop     iy
l52:
push    iy
pop     hl
ld      a,l
or      h
jp      z,l54

;ld      de,(_nstdpth)
;ld      l,(iy+4)
;ld      h,0
;or      a
;sbc     hl,de
ld	a,(_nstdpth)
cp	(iy+4)

jp      z,l53
l54:
ld      a,(ix+-6)
or      (ix+-5)
jp      z,l63
ld      e,(ix+-6)
ld      d,(ix+-5)
ld      l,(ix+-2)
ld      h,(ix+-1)
inc     hl
inc     hl
ld      (ix+-2),l
ld      (ix+-1),h
dec     hl
dec     hl
ld      (hl),e
inc     hl
ld      (hl),d
jp      l64
l65:
ld      e,(ix+-6)
ld      d,(ix+-5)
ld      hl,7
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      (ix+-6),c
ld      (ix+-5),b
l64:
ld      e,(ix+-6)
ld      d,(ix+-5)
ld      hl,7
add     hl,de
ld      a,(hl)
inc     hl
or      (hl)
jp      nz,l65
ld      hl,7
add     hl,de
push    iy
pop     de
ld      (hl),e
inc     hl
ld      (hl),d
jp      l49
l63:
push    iy
pop     de
ld      l,(ix+-2)
ld      h,(ix+-1)
inc     hl
inc     hl
ld      (ix+-2),l
ld      (ix+-1),h
dec     hl
dec     hl
ld      (hl),e
inc     hl
ld      (hl),d
l49:
ld      l,(ix+-4)
ld      h,(ix+-3)
dec     hl
ld      (ix+-4),l
ld      (ix+-3),h
ld      a,l
or      h
jp      nz,l51

;ld      hl,(_nstdpth)
;dec     hl
;ld      (_nstdpth),hl
ld	hl,_nstdpth
dec	(hl)

ld      hl,(_lvlidx)
bit     7,h
jp      nz,cret
ld      de,_array_AE57
add     hl,hl
add     hl,de

;ld      a,(hl)
;inc     hl
;ld      h,(hl)
;ld      l,a
;ld      de,(_nstdpth)
;or      a
;sbc     hl,de
ld	a,(_nstdpth)
cp	(hl)

jp      nz,cret
ld      de,_array_AEDB
ld      hl,(_lvlidx)
add     hl,hl
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
pop     iy
ld      l,(iy+15)
ld      h,(iy+16)
inc     hl
inc     hl
ld      a,(hl)
cp      55
jp      nz,l70
bit     2,(iy+13)
jp      nz,l70
ld      l,(iy+9)
ld      h,(iy+10)
push    hl
ld      l,(iy+17)
ld      h,(iy+18)
push    hl
call    _sub_BEE
pop     bc
pop     bc
l70:
ld      de,_array_AE13
ld      hl,(_lvlidx)
add     hl,hl
add     hl,de
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
ld      l,(iy+17)
ld      h,(iy+18)
push    hl
call    _prFrameTail
ld      hl,(_lvlidx)
dec     hl
ld      (_lvlidx),hl
ld      hl,383
ld      (_word_B017),hl
jp      cret
global  _sub_627
global  asll
global  asll
global  aslr
_sub_627:
global csv
call csv
push hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
pop     iy
ld      (ix+-2),0
ld      (ix+-1),0
inc     iy
l74:
ld      b,2
push    ix
pop     hl
dec     hl
dec     hl
call    asll
ld      a,(iy+0)
cp      40
jp      nz,l75
set     1,(ix+-2)
inc     iy
jp      l72
l75:
ld      a,(iy+0)
cp      42
jp      nz,l72
set     0,(ix+-2)
inc     iy
l72:

;ld      a,(iy+0)
;ld      e,a
;rla
;sbc     a,a
;ld      d,a
ld	e,(iy+0)
ld	d,0

ld      hl,__ctype_+1
add     hl,de
ld      a,(hl)
and     7
or      a
jp      nz,l73
ld      a,e
cp      95
jp      nz,l74
l73:
ld      de,0
ld      l,(ix+8)
ld      h,(ix+9)
ld      (hl),e
inc     hl
ld      (hl),d
jp      l78
l79:
ld      b,2
ld      l,(ix+8)
ld      h,(ix+9)
call    asll
ld      a,(ix+-2)
and     3
ld      e,a
xor     a
ld      d,a
ld      l,(ix+8)
ld      h,(ix+9)
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      a,c
or      e
ld      c,a
ld      a,b
or      d
ld      b,a
ld      (hl),b
dec     hl
ld      (hl),c
ld      b,2
push    ix
pop     hl
dec     hl
dec     hl
call    aslr
l78:
ld      a,(ix+-2)
or      (ix+-1)
jp      nz,l79
push    iy
call    _sub_265
;pop     bc
;jp      cret
jp	popBCcret
global  _badIntCode
global  _fatalErr
_badIntCode:
ld      hl,739f
push    hl
call    _fatalErr
pop     bc
ret
global  _parseStmt
global  _prPsect
global  _sub_3DC9
global  _parseExpr
global  _expect
global  _parseVariable
global  _parseMembers
global  _parseSwitch
global  _parseEnum
global  _parseInit
global  _parseData
_parseStmt:
global csv
call csv
push hl
ld      hl,383
ld      (_word_B017),hl
l86:
call    _getToken
push    hl
pop     iy

;ld      de,-1
;or      a
;sbc     hl,de
ld	a,h
and	l
inc	a

jp      nz,l84
call    _leaveBlock
jp      cret
l84:
push    iy
call    _sub_1B2
pop     bc
ld      a,l
rla
sbc     a,a
ld      h,a
ld      (ix+-2),l
ld      (ix+-1),h
ex      de,hl
ld      hl,-50
add     hl,de
xor     a
cp      h
jp      c,l101
jp      nz,L17
ld      a,12
cp      l
jp      c,l101
L17:
add     hl,hl
ld      de,S217
add     hl,de
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
jp      (hl)
l88:

;ld      hl,(_nstdpth)
;inc     hl
;ld      (_nstdpth),hl
ld	hl,_nstdpth
inc	(hl)

jp      l86
l89:
call    _leaveBlock
jp      l86
l90:
ld      hl,2
push    hl
call    _prPsect
pop     bc
call    _parseExpr
push    hl
call    _sub_3DC9
ld      l,93
ex      (sp),hl
call    _expect
L18:
pop     bc
jp      l86
l91:
call    _parseVariable
jp      l86
l92:
l93:
ld      l,(ix+-2)
ld      h,(ix+-1)
push    hl
call    _parseMembers
jp      L18
l94:
call    _parseSwitch
jp      l86
l95:
call    _parseEnum
jp      l86
l96:
call    _parseInit
jp      l86
l97:
call    _parseData
jp      l86
l98:
l99:
l100:
l101:
call    _badIntCode
jp      l86
psect   data
S217:
defw    l94
defw    l97
defw    l95
defw    l90
defw    l96
defw    l92
defw    l93
defw    l91
defw    l98
defw    l88
defw    l99
defw    l100
defw    l89
psect   text
_expect:
global csv
call csv

;push hl	not used

l105:
call    _fgetchar

;ld      (ix+-1),l	;why?

;ld      a,l
;ld      e,a
;rla
;sbc     a,a
;ld      d,a
ld	e,l
ld	d,0

ld      hl,__ctype_+1
add     hl,de
bit     3,(hl)
jp      nz,l105
ld      a,e
cp      (ix+6)
jp      z,cret
call    _badIntCode
jp      cret
global  _sub_2BD0
global  _sub_600E
global  _sub_43EF
global  _prDefb
_parseData:
call ncsv
defw -1026
ld      hl,3
push    hl
call    _prPsect
ld      hl,0
ex      (sp),hl
ld      hl,0
push    hl
call    _parseExpr
push    hl
ld      l,27
push    hl
call    _sub_43EF
pop     bc
pop     bc
pop     bc
push    hl
ld      l,26
push    hl
call    _sub_43EF
pop     bc
pop     bc
ex      (sp),hl
call    _sub_600E
ex      (sp),hl
call    _sub_2BD0
pop     bc
push    ix
pop     de
ld      hl,-1026
add     hl,de
push    hl
pop     iy
l108:
call    _getToken
ld      (ix+-2),l
ld      (ix+-1),h
ld      a,(hl)
cp      93
jp      z,l109
push    hl
call    _atoi
pop     bc
ld      (iy+0),l
inc     iy
jp      l108
l109:
push    ix
pop     de
ld      hl,-1026
add     hl,de
ex      de,hl
push    iy
pop     hl
or      a
sbc     hl,de
push    hl
push    ix
pop     de
ld      hl,-1026
add     hl,de
push    hl
call    _prDefb
jp      cret
psect   data
19:
defb    0
29:
defb    33,0
39:
defb    33,61,0
49:
defb    35,0
59:
defb    36,0
69:
defb    36,85,0
79:
defb    37,0
89:
defb    38,0
99:
defb    38,38,0
109:
defb    38,85,0
119:
defb    40,0
129:
defb    41,0
139:
defb    42,0
149:
defb    42,85,0
159:
defb    43,0
169:
defb    43,43,0
179:
defb    43,85,0
189:
defb    44,0
199:
defb    45,0
209:
defb    45,45,0
219:
defb    45,62,0
229:
defb    45,85,0
239:
defb    46,0
249:
defb    46,46,0
259:
defb    47,0
269:
defb    58,0
279:
defb    58,85,0
289:
defb    58,115,0
299:
defb    59,0
309:
defb    59,59,0
319:
defb    60,0
329:
defb    60,60,0
339:
defb    60,61,0
349:
defb    61,0
359:
defb    61,37,0
369:
defb    61,38,0
379:
defb    61,42,0
389:
defb    61,43,0
399:
defb    61,45,0
409:
defb    61,47,0
419:
defb    61,60,60,0
429:
defb    61,61,0
439:
defb    61,62,62,0
449:
defb    61,94,0
459:
defb    61,124,0
469:
defb    62,0
479:
defb    62,61,0
489:
defb    62,62,0
499:
defb    63,0
509:
defb    64,0
519:
defb    91,92,0
529:
defb    91,97,0
539:
defb    91,99,0
549:
defb    91,101,0
559:
defb    91,105,0
569:
defb    91,115,0
579:
defb    91,117,0
589:
defb    91,118,0
599:
defb    94,0
609:
defb    123,0
619:
defb    124,0
629:
defb    124,124,0
639:
defb    125,0
649:
defb    126,0
659:
defb    82,69,67,73,80,0
669:
defb    84,89,80,69,0
679:
defb    73,68,0
689:
defb    67,79,78,83,84,0
699:
defb    70,67,79,78,83,84,0
709:
defb    82,69,71,0
719:
defb    73,78,65,82,69,71,0
729:
defb    66,73,84,70,73,69,76,68,0
739:
defb    66,97,100,32,105,110,116,46,32,99,111,100,101,0
psect   text

