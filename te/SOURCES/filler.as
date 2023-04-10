;
;	fills the space till 8000H
;

	psect	top

COUNT	equ	2450H

REPT	COUNT
	defb	0
ENDM
