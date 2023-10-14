;
;	fills the space till 8000H
;

	psect	top

COUNT	equ	2280H

REPT	COUNT
	defb	0
ENDM
