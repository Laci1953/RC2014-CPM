;
;	fills the space till 8000H
;

	psect	top

COUNT	equ	2100H

REPT	COUNT
	defb	0
ENDM
