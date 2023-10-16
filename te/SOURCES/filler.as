;
;	fills the space till 8000H
;

	psect	top

COUNT	equ	2108H

REPT	COUNT
	defb	0
ENDM
