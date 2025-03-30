
	psect	text

;void CrtSetup(void)
global _CrtSetup
_CrtSetup:
	ld  	hl,(1)
	inc 	hl
	inc 	hl
	inc 	hl
	ld  	de,BiosConst
	ld  	bc,9
	ldir
	ret

BiosConst:  jp 0
BiosConin:  jp 0
BiosConout: jp 0

;void CrtOut(char)
global _CrtOut
_CrtOut:
        ld      hl,2
        add     hl,sp
        ld      c,(hl)  	;ch
        jp   	BiosConout

;char CrtIn(void)
global _CrtIn
_CrtIn:
	call 	BiosConin
	ld 	h,0
	ld 	l,a
	ret

;char CrtSts(void)
global _CrtSts
_CrtSts:
	call	BiosConst
	ld	h,0
	ld	l,a
	ret

;void putstr(char*)
global _putstr
_putstr:
        ld      hl,2
        add     hl,sp
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        ex      de,hl
1:      ld      a,(hl)
        or      a
        ret     z
        inc     hl
        ld      c,a
        push    hl
        push    bc
        call    _CrtOut
        pop     bc
        pop     hl
        jr      1b
