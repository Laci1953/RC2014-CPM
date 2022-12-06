global  _tmpFile
psect   data
_tmpFile:
defw    19f
global  _runtop
global  ncsv, cret, indir
global  _InitDynM
global  _freopen
global  __iob
global  _setbuf
global  _errBuf
global  _s_opt
global  _w_opt
global  _l_opt
global  _crfFile
global  _crfNameBuf
global  _n_opt
global  _fatalErr
global  _srcFileArg
global  _strcpy
global  _srcFile
global  _rindex
global  _strcat
global  _crfFp
global  _fopen
global  _prWarning
global  _fprintf
global  _tmpFp
global  _s13_9d28
global  _s13_9d1b
psect   top
_runtop:
global csv
call csv
call    _InitDynM
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
jp      l21
l18:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
inc     hl
ld      a,(hl)
cp      67
jp      z,l33
cp      69
jp      z,l25
cp      76
jp      z,l31
cp      78
jp      z,l37
cp      83
jp      z,l27
cp      87
jp      z,l29
cp      99
jp      z,l33
cp      101
jp      z,l25
cp      108
jp      z,l31
cp      110
jp      z,l37
cp      115
jp      z,l27
cp      119
jp      z,l29
l20:
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
dec     hl
dec     hl
l21:
ld      a,(ix+6)
or      (ix+7)
jp      z,l19
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
ld      a,(hl)
cp      45
jp      z,l18
l19:
ld      a,(ix+6)
or      (ix+7)
jp      z,l38
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
ld      l,(ix+8)
ld      h,(ix+9)
ld      c,(hl)
inc     hl
ld      b,(hl)
jp      nz,L3
push    bc
ld      hl,195
push    hl
call    _fatalErr
pop     bc
pop     bc
ld      l,(ix+8)
ld      h,(ix+9)
ld      c,(hl)
inc     hl
ld      b,(hl)
L3:
ld      (_srcFileArg),bc
ld      l,c
ld      h,b
push    hl
ld      hl,_srcFile
push    hl
call    _strcpy
pop     bc
pop     bc
ld      de,1
ld      l,(ix+6)
ld      h,(ix+7)
or      a
sbc     hl,de
jp      z,l40
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
jp      nz,l40
ld      l,(ix+8)
ld      h,(ix+9)
inc     hl
inc     hl
ld      c,(hl)
inc     hl
ld      b,(hl)
push    bc
ld      hl,195
push    hl
call    _fatalErr
pop     bc
pop     bc
l40:
ld      de,3
ld      l,(ix+6)
ld      h,(ix+7)
or      a
sbc     hl,de
jp      nz,l42
ld      l,(ix+8)
ld      h,(ix+9)
inc     hl
inc     hl
inc     hl
inc     hl
ld      c,(hl)
inc     hl
ld      b,(hl)
ld      (_tmpFile),bc
jp      l42
l25:
ld      hl,__iob+16
push    hl
ld      hl,29f
push    hl
ld      l,(ix+8)
ld      h,(ix+9)
ld      c,(hl)
inc     hl
ld      b,(hl)
inc     bc
inc     bc
push    bc
call    _freopen
pop     bc
pop     bc
ld      hl,_errBuf
ex      (sp),hl
ld      hl,__iob+16
push    hl
call    _setbuf
pop     bc
pop     bc
jp      l20
l27:
ld      a,1
ld      (_s_opt),a
jp      l20
l29:
ld      a,1
ld      (_w_opt),a
jp      l20
l31:
ld      a,1
ld      (_l_opt),a
jp      l20
l33:
ld      l,(ix+8)
ld      h,(ix+9)
ld      a,(hl)
inc     hl
ld      h,(hl)
ld      l,a
inc     hl
inc     hl
ld      a,(hl)
or      a
jp      z,l34
ld      l,(ix+8)
ld      h,(ix+9)
ld      c,(hl)
inc     hl
ld      b,(hl)
inc     bc
inc     bc
ld      (_crfFile),bc
jp      l20
l34:
ld      hl,_crfNameBuf
ld      (_crfFile),hl
jp      l20
l37:
ld      a,1
ld      (_n_opt),a
jp      l20
l38:
ld      hl,59f
ld      (_srcFileArg),hl
push    hl
ld      hl,_srcFile
push    hl
call    _strcpy
pop     bc
pop     bc
l42:
ld      hl,(_crfFile)
ld      a,l
or      h
jp      z,l43
ld      a,(hl)
or      a
jp      nz,l44
ld      hl,_crfNameBuf
ld      (_crfFile),hl
ld      hl,_srcFile
push    hl
ld      hl,_crfNameBuf
push    hl
call    _strcpy
pop     bc
ld      l,46
ex      (sp),hl
ld      hl,_crfNameBuf
push    hl
call    _rindex
pop     bc
pop     bc
push    hl
pop     iy
ld      a,l
or      h
jp      z,l45
ld      hl,69f
push    hl
push    iy
call    _strcpy
pop     bc
pop     bc
jp      l44
l45:
ld      hl,79f
push    hl
ld      hl,_crfNameBuf
push    hl
call    _strcat
pop     bc
pop     bc
l44:
ld      hl,89f
push    hl
ld      hl,(_crfFile)
push    hl
call    _fopen
pop     bc
pop     bc
ld      (_crfFp),hl
ld      a,l
or      h
jp      nz,l47
ld      hl,(_crfFile)
push    hl
ld      hl,192
push    hl
call    _prWarning
pop     bc
pop     bc
jp      l43
l47:
ld      hl,_srcFile
push    hl
ld      hl,99f
push    hl
ld      hl,(_crfFp)
push    hl
call    _fprintf
pop     bc
pop     bc
pop     bc
l43:
ld      hl,109f
push    hl
ld      hl,(_tmpFile)
push    hl
call    _fopen
pop     bc
pop     bc
ld      (_tmpFp),hl
ld      a,l
or      h
jp      nz,l49
ld      hl,(_tmpFile)
push    hl
ld      hl,195
push    hl
call    _fatalErr
l49:
ld      a,116
ld      (_s13_9d28),a
ld      (_s13_9d1b),a
ld      a,8
ld      hl,_s13_9d28+11
ld      (hl),a
ld      hl,_s13_9d1b+11
ld      (hl),a
ld      de,0
ld      l,e
ld      h,d
ld      (_s13_9d1b+1),de
ld      (_s13_9d1b+3),hl
ld      de,1
ld      (_s13_9d28+1),de
ld      (_s13_9d28+3),hl
jp      cret
psect text
global  _main
global  _sub_4d92
global  _sub_07e3
global  _sub_3abf
global  _copyTmp
global  _fclose
global  _prError
global  _closeFiles
global  _TotalSize
global  shlr
global  _exit
global  _errCnt
_main:
global csv
call csv
ld      l,(ix+8)
ld      h,(ix+9)
push    hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _runtop
pop     bc
pop     bc
call    _sub_4d92
call    _sub_07e3
call    _sub_3abf
call    _copyTmp
ld      hl,__iob+8
push    hl
call    _fclose
pop     bc
ld      de,-1
or      a
sbc     hl,de
jp      nz,l51
ld      hl,93
push    hl
call    _prError
pop     bc
l51:
call    _closeFiles
ld      a,(_n_opt)
or      a
jp      z,l52
call    _TotalSize
ld      b,10
call    shlr
push    hl
ld      hl,119f
push    hl
ld      hl,__iob+16
push    hl
call    _fprintf
pop     bc
pop     bc
pop     bc
l52:
ld      hl,(_errCnt)
ld      a,l
or      h
ld      hl,1
jp      nz,L4
dec     hl
L4:
push    hl
call    _exit
jp      cret
global  _prMsg
global  _sprintf
global  _prMsgAt
_prMsg:
call ncsv
defw -128
ld      l,(ix+10)
ld      h,(ix+11)
push    hl
ld      l,(ix+8)
ld      h,(ix+9)
push    hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
push    ix
pop     de
ld      hl,-128
add     hl,de
push    hl
call    _sprintf
pop     bc
pop     bc
pop     bc
push    ix
pop     de
ld      hl,-128
add     hl,de
ex      (sp),hl
call    _prMsgAt
jp      cret
global  _format_and_print
global  wrelop
global  wrelop
_format_and_print:
call ncsv
defw -130
ld      hl,129f
ld      (ix+-2),l
ld      (ix+-1),h
ld      de,100
ld      l,(ix+6)
ld      h,(ix+7)
call    wrelop
jp      p,l55
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
ld      l,(ix+14)
ld      h,(ix+15)
push    hl
push    ix
pop     de
ld      hl,-130
add     hl,de
push    hl
call    _sprintf
pop     bc
L8:
pop     bc
pop     bc
jp      l56
l55:
ld      de,100
ld      l,(ix+6)
ld      h,(ix+7)
or      a
sbc     hl,de
jp      nz,l57
ld      l,(ix+-2)
ld      h,(ix+-1)
push    hl
push    ix
pop     de
ld      hl,-130
add     hl,de
push    hl
call    _strcpy
jp      L8
l57:
ld      de,200
ld      l,(ix+6)
ld      h,(ix+7)
call    wrelop
jp      p,l59
ld      e,(ix+6)
ld      d,(ix+7)
ld      hl,-100
add     hl,de
push    hl
ld      l,(ix+14)
ld      h,(ix+15)
push    hl
push    ix
pop     de
ld      hl,-130
add     hl,de
push    hl
call    _sprintf
pop     bc
pop     bc
ld      l,(ix+-2)
ld      h,(ix+-1)
ex      (sp),hl
push    ix
pop     de
ld      hl,-130
add     hl,de
push    hl
call    _strcat
jp      L8
l59:
ld      de,200
ld      l,(ix+6)
ld      h,(ix+7)
or      a
sbc     hl,de
jp      z,L5
ld      de,300
ld      l,(ix+6)
ld      h,(ix+7)
or      a
sbc     hl,de
jp      nz,l61
L5:
ld      l,(ix+12)
ld      h,(ix+13)
push    hl
ld      l,(ix+10)
ld      h,(ix+11)
push    hl
ld      l,(ix+8)
ld      h,(ix+9)
push    hl
call    _prMsg
pop     bc
pop     bc
pop     bc
jp      cret
l61:
ld      e,(ix+6)
ld      d,(ix+7)
ld      hl,-200
add     hl,de
push    hl
ld      l,(ix+14)
ld      h,(ix+15)
push    hl
push    ix
pop     de
ld      hl,-130
add     hl,de
push    hl
call    _sprintf
pop     bc
pop     bc
ld      l,(ix+-2)
ld      h,(ix+-1)
ex      (sp),hl
push    ix
pop     de
ld      hl,-130
add     hl,de
push    hl
call    _strcat
pop     bc
ld      l,(ix+-2)
ld      h,(ix+-1)
ex      (sp),hl
push    ix
pop     de
ld      hl,-130
add     hl,de
push    hl
call    _strcat
pop     bc
pop     bc
l56:
ld      l,(ix+10)
ld      h,(ix+11)
push    hl
ld      l,(ix+8)
ld      h,(ix+9)
push    hl
push    ix
pop     de
ld      hl,-130
add     hl,de
push    hl
call    _prMsg
jp      cret
global  _fputc
_prError:
global csv
call csv
ld      hl,139f
push    hl
ld      l,(ix+12)
ld      h,(ix+13)
push    hl
ld      l,(ix+10)
ld      h,(ix+11)
push    hl
ld      l,(ix+8)
ld      h,(ix+9)
push    hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _format_and_print
ld      hl,10
add     hl,sp
ld      sp,hl
ld      hl,(_errCnt)
inc     hl
ld      (_errCnt),hl
ld      hl,__iob+16
push    hl
ld      hl,10
push    hl
call    _fputc
jp      cret
_fatalErr:
global csv
call csv
ld      l,(ix+10)
ld      h,(ix+11)
push    hl
ld      l,(ix+8)
ld      h,(ix+9)
push    hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _prError
pop     bc
pop     bc
pop     bc
call    _closeFiles
ld      hl,1
push    hl
call    _exit
jp      cret
_prWarning:
global csv
call csv
ld      hl,149f
push    hl
ld      l,(ix+12)
ld      h,(ix+13)
push    hl
ld      l,(ix+10)
ld      h,(ix+11)
push    hl
ld      l,(ix+8)
ld      h,(ix+9)
push    hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _format_and_print
ld      hl,10
add     hl,sp
ld      sp,hl
ld      hl,159f
push    hl
ld      hl,__iob+16
push    hl
call    _fprintf
jp      cret
global  _expectErr
_expectErr:
global csv
call csv
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
ld      hl,169f
push    hl
ld      hl,200
push    hl
call    _prError
jp      cret
global  _fgetc
_copyTmp:
global csv
call csv
push hl
ld      hl,(_tmpFp)
push    hl
call    _fclose
ld      hl,179f
ex      (sp),hl
ld      hl,(_tmpFile)
push    hl
call    _fopen
pop     bc
pop     bc
ld      (_tmpFp),hl
ld      a,l
or      h
jp      nz,l69
ld      hl,(_tmpFile)
push    hl
ld      hl,189f
push    hl
ld      hl,200
push    hl
call    _fatalErr
pop     bc
pop     bc
pop     bc
jp      l69
l70:
ld      hl,__iob+8
push    hl
ld      l,(ix+-2)
ld      h,(ix+-1)
push    hl
call    _fputc
pop     bc
pop     bc
l69:
ld      hl,(_tmpFp)
push    hl
call    _fgetc
pop     bc
ld      (ix+-2),l
ld      (ix+-1),h
ld      de,-1
or      a
sbc     hl,de
jp      nz,l70
jp      cret
global  _unlink
_closeFiles:
ld      hl,__iob
push    hl
call    _fclose
ld      hl,__iob+8
ex      (sp),hl
call    _fclose
pop     bc
ld      hl,(_tmpFp)
ld      a,l
or      h
jp      z,l73
push    hl
call    _fclose
ld      hl,(_tmpFile)
ex      (sp),hl
call    _unlink
pop     bc
l73:
ld      hl,(_crfFp)
ld      a,l
or      h
ret     z
push    hl
call    _fclose
pop     bc
ret
global  _xalloc
global  _malloc
global  _releaseNodeFreeList
global  _s13ReleaseFreeList
global  _blkclr
_xalloc:
global csv
call csv
l78:
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _malloc
pop     bc
push    hl
pop     iy
ld      a,l
or      h
jp      nz,l80
call    _releaseNodeFreeList
ld      a,l
or      a
jp      nz,l78
call    _s13ReleaseFreeList
ld      a,l
or      a
jp      nz,l78
ld      hl,199f
push    hl
ld      hl,100
push    hl
call    _fatalErr
pop     bc
pop     bc
l80:
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
push    iy
call    _blkclr
pop     bc
pop     bc
push    iy
pop     hl
jp      cret
global  _ungetTok
global  _sub_3adf
global  _yylex
global  _checkScopeExit
_sub_3abf:
global csv
call csv
push hl
jp      l82
l83:
ld      a,(ix+-1)
ld      (_ungetTok),a
call    _sub_3adf
l82:
call    _yylex
ld      a,l
ld      (ix+-1),a
cp      1
jp      nz,l83
call    _checkScopeExit
jp      cret
psect   data
19:
defb    112,49,46,116,109,112,0
29:
defb    97,0
39:
defb    114,0
49:
defb    119,0
59:
defb    40,115,116,100,105,110,41,0
69:
defb    46,99,114,102,0
79:
defb    46,99,114,102,0
89:
defb    97,0
99:
defb    126,37,115,10,0
109:
defb    119,0
119:
defb    37,100,32,75,66,32,117,115,101,100,32,102,114,111,109,32
defb    116,104,101,32,117,112,112,101,114,32,54,52,75,66,32,82
defb    65,77,13,10,0
129:
defb    32,37,115,0
139:
defb    69,114,114,111,114,32,35,37,100,0
149:
defb    87,97,114,110,105,110,103,32,35,37,100,0
159:
defb    32,40,119,97,114,110,105,110,103,41,10,0
169:
defb    37,115,32,101,120,112,101,99,116,101,100,0
179:
defb    114,0
189:
defb    67,97,110,39,116,32,114,101,111,112,101,110,32,37,115,0
199:
defb    79,117,116,32,111,102,32,109,101,109,111,114,121,0
psect   bss
_crfNameBuf:
defs    30
_tmpFp:
defs    2
_errBuf:
defs    128
_w_opt:
defs    1
_crfFile:
defs    2
_errCnt:
defs    2
_crfFp:
defs    2
global  _inBuf
_inBuf:
defs    128
global  _lineNo
_lineNo:
defs    2
_l_opt:
defs    1
_srcFileArg:
defs    2
_s_opt:
defs    1
_n_opt:
defs    1
_srcFile:
defs    100
psect   text