global  _fgetchar
global  ncsv, cret, indir
global  _fgetc
global  __iob
psect   text
_fgetchar:
ld      hl,__iob
push    hl
call    _fgetc
pop     bc
ret
global  _fputchar
global  _fputc
_fputchar:
global csv
call csv
ld      hl,__iob+8
push    hl
ld      l,(ix+6)
;ld      h,(ix+7)
push    hl
call    _fputc
jp      cret

