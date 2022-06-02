include cgen.inc
global  _main
global  ncsv, cret, indir
global  _baseHeap
global  _sbrk
global  _pflag
global  _wflag
global  _rflag
global  _bflag
global  _eflag
global  _hflag
global  _fatalErr
global  wrelop
global  wrelop
global  _freopen
global  __iob
global  wrelop
global  _sub_1680
global  _parseStmt
global  _fclose
global  _prError
global  _exit
global  _errcnt
psect   text

_main:
	jp	topcode
	global	rettop

rettop:

ld      hl,0
push    hl
call    _sbrk
pop     bc
ld      (_baseHeap),hl

call    _sub_1680
call    _parseStmt
ld      hl,__iob+8
push    hl
call    _fclose
pop     bc

;ld      de,-1
;or      a
;sbc     hl,de
ld	a,h
and	l
inc	a

jp      nz,l29
ld      hl,69f
push    hl
call    _prError
pop     bc
l29:
ld      hl,(_errcnt)
ld      a,l
or      h
ld      hl,1
jp      nz,L1
dec     hl
L1:
push    hl
call    _exit
jp      cret
global  _prMsg
_fatalErr:
global csv
call csv
push hl
push    ix
pop     de
ld      hl,8
add     hl,de
ld      (ix+-2),l
ld      (ix+-1),h
push    ix
pop     hl
dec     hl
dec     hl
push    hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _prMsg
pop     bc
ld      hl,__iob+8
ex      (sp),hl
call    _fclose
ld      hl,2
ex      (sp),hl
call    _exit
jp      cret
global  _prWarning
global  _fprintf
global  _progname
global  _lineno
global  __doprnt
_prWarning:
global csv
call csv
push hl
ld      a,(_wflag)
or      a
jp      nz,cret
ld      hl,(_lineno)
push    hl
ld      hl,_progname
push    hl
ld      hl,79f
push    hl
ld      hl,__iob+16
push    hl
call    _fprintf
pop     bc
pop     bc
pop     bc
pop     bc
push    ix
pop     de
ld      hl,8
add     hl,de
ld      (ix+-2),l
ld      (ix+-1),h
push    ix
pop     hl
dec     hl
dec     hl
push    hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
ld      hl,__iob+16
push    hl
call    __doprnt
pop     bc
pop     bc
ld      hl,89f
ex      (sp),hl
ld      hl,__iob+16
push    hl
call    _fprintf
jp      cret
global  wrelop
_prError:
global csv
call csv
push hl
push    ix
pop     de
ld      hl,8
add     hl,de
ld      (ix+-2),l
ld      (ix+-1),h
push    ix
pop     hl
dec     hl
dec     hl
push    hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _prMsg
pop     bc
pop     bc
ld      de,30
ld      hl,(_errcnt)
inc     hl
ld      (_errcnt),hl
call    wrelop
jp      m,cret
ld      hl,99f
push    hl
call    _fatalErr
jp      cret
global  _fputc
_prMsg:
global csv
call csv
ld      hl,(_lineno)
push    hl
ld      hl,_progname
push    hl
ld      hl,109f
push    hl
ld      hl,__iob+16
push    hl
call    _fprintf
pop     bc
pop     bc
pop     bc
ld      l,(ix+8)
ld      h,(ix+9)
ex      (sp),hl
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
ld      hl,__iob+16
push    hl
call    __doprnt
pop     bc
pop     bc
ld      hl,__iob+16
ex      (sp),hl

;ld      hl,10
ld	l,10

push    hl
call    _fputc
jp      cret
global  _allocMem
global  _malloc
global  _releaseNodeFreeList
global  _blkclr
_allocMem:
global csv
call csv
l39:
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _malloc
pop     bc
push    hl
pop     iy
ld      a,l
or      h
jp      nz,l41
call    _releaseNodeFreeList
ld      a,l
or      a
jp      nz,l39
ld      hl,119f
push    hl
call    _fatalErr
pop     bc
l41:
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
push    iy
call    _blkclr
pop     bc
pop     bc
;push    iy
;pop     hl
;jp      cret
jp	pushIYpopHLcret

IF RAM128

global  _MyAlloc
global  _Alloc128
_MyAlloc:
global csv
call csv
ld      l,(ix+6)
ld      h,(ix+7)
push    hl
call    _Alloc128
pop     bc
push    hl
pop     iy
ld      a,l
or      h
jp      z,l43
;push    iy
;pop     hl
;jp      cret
jp	pushIYpopHLcret
l43:
ld      hl,129f
push    hl
call    _fatalErr
jp      cret

ENDIF

psect   data
69:
defb    69,114,114,111,114,32,99,108,111,115,105,110,103,32,111,117
defb    116,112,117,116,32,102,105,108,101,0
79:
defb    37,115,58,37,100,58,9,0
89:
defb    32,40,119,97,114,110,105,110,103,41,10,0
99:
defb    84,111,111,32,109,97,110,121,32,101,114,114,111,114,115,0
109:
defb    37,115,58,37,100,58,9,0
119:
defb    78,111,32,114,111,111,109,0
129:
defb    85,112,112,101,114,32,82,65,77,32,102,117,108,108,0
psect   text

