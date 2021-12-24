	GLOBAL _setname
	GLOBAL _setext
	GLOBAL _openfiler
	GLOBAL _openfilew
	GLOBAL _writefile
	GLOBAL _readfile
	GLOBAL _closefile
	GLOBAL _setdma
        GLOBAL _setdisk
	GLOBAL _cleanfcb

	GLOBAL _savebyte, _loadbyte
	GLOBAL _saveword, _loadword

__bdos	equ	5

openf	equ 15	; open file
closef 	equ 16 	; close file
deletef equ 19 	; delete file
readf	equ 20	; sequential read
writef 	equ 21 	; sequential write
makef 	equ 22	; make file
setdmaf	equ 26	; set DMA addr
;
	psect	text

dfcb:				; destination fcb
	defs	1		; disk (A=1,B=2,...)
n_e:	defm	'           '	; file name & ext
dfcbz:	defb	0		; EX=0
	defs	2		; S1,S2
	defb	0		; RC=0
	defs	16		; D0,...D15
	defb	0		; CR=0
	defb	0,0,0		; R0,R1,R2

DFCBZ_LEN	equ	$-dfcbz

;void cleanfcb(void)
_cleanfcb:
	ld	hl,dfcbz
	xor	a
	ld	b,DFCBZ_LEN
clloop:	ld	(hl),a
	inc	hl
	djnz	clloop
	ret

;void setdisk(void)
_setdisk:
	ld	a,(4)
	and	0FH
	inc	a
	ld	(dfcb),a
	ret

;void setdma(char*)
_setdma:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	c,setdmaf
	jp	__bdos

;void setname(char*, short len)
_setname:
	ld	de,n_e
	ld	b,11
	ld	a,20H
erase:	ld	(de),a
	inc	de
	djnz	erase
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	ld	b,0		;BC=count
	ex	de,hl		;HL=source
	ld	de,n_e		;DE=dest
	ldir
	ret

;void setext(char*, short len)
_setext:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	ld	b,0		;BC=count
	ex	de,hl		;HL=source
	ld	de,n_e+8	;DE=dest
	ldir
	ret

;char openfiler(void)
_openfiler:
 	ld 	de,dfcb
	ld 	c,openf
 	call 	__bdos
	ld	l,a
	ret

;void openfilew(void)
_openfilew:
 	ld 	de,dfcb
	ld 	c,deletef
	call	__bdos
 	ld 	de,dfcb
	ld	c,makef
 	jp 	__bdos

;char readfile(void)
_readfile:
 	ld 	de,dfcb
	ld	c,readf
 	call 	__bdos
	ld	l,a
	ret

;void writefile(void)
_writefile:
 	ld 	de,dfcb
	ld	c,writef
 	jp 	__bdos

;void closefile(void)
_closefile:
 	ld 	de,dfcb
	ld	c,closef
 	jp 	__bdos
;
;char* savebyte(char* dest, char* source)
;	incremented dest returned
_savebyte:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	ld	a,(bc)
	ld	(de),a
	inc	de
	ex	de,hl
	ret
;
;char* saveword(char* dest, char* source)
;	incremented dest returned
_saveword:
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	ld	a,(bc)
	ld	(de),a
	inc	de
	inc	bc
	ld	a,(bc)
	ld	(de),a
	inc	de
	ex	de,hl
	ret
;
;char* loadbyte(char* dest, char* source)
;	incremented source returned
_loadbyte:
	ld	hl,2
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	a,(de)
	ld	(bc),a
	inc	de
	ex	de,hl
	ret
;
;char* loadword(char* dest, char* source)
;	incremented source returned
_loadword:
	ld	hl,2
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ld	a,(de)
	ld	(bc),a
	inc	de
	inc	bc
	ld	a,(de)
	ld	(bc),a
	inc	de
	ex	de,hl
	ret
;
