?Z80ALL	set	0
;
;	fills the space till 8000H
;

IF	?Z80ALL

	psect	top

COUNT	equ	2100H

REPT	COUNT
	defb	0
ENDM

ENDIF

