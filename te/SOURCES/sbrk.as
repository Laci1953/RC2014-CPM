Z80ALL	equ	0

IF	Z80ALL

;
;	Implements two heap mechanism
;
	psect	text

        global  _sbrk, _brk

;	Heap 1 from 5E00H to 8000H
;		lp_arr_banks 	(2000H)
;		clipboard 	(100H)
;	Heap 2 from 8E00H
;		lp_arr 		(4000H)
;		File buffers 	(200H + 200H)
;
HEAP	equ	0CE00H	;for file buffers

_brk:
        pop     hl      ;return address
        pop     de      ;argument
        ld      (memtop),de     ;store it
        push    de              ;adjust stack
        jp      (hl)    ;return
_sbrk:
        pop     bc
        pop     de
        push    de
        push    bc
        ld      hl,(memtop)
	ld	a,h
	or	l
	jr	nz,notzero
	ld	hl,HEAP
	ld	(memtop),hl
notzero:
        add     hl,de
        jr      c,2f            ;if overflow, no room
        ld      bc,512          ;allow 512 bytes stack overhead
        add     hl,bc
        jr      c,2f            ;if overflow, no room
        sbc     hl,sp
        jr      c,1f
2:
        ld      hl,-1           ;no room at the inn
        ret

1:      ld      hl,(memtop)
        push    hl
        add     hl,de
        ld      (memtop),hl
        pop     hl
        ret

	psect	bss

memtop:	defs	2

ENDIF

