
	psect	data

	global	_TK1, _TK2, _TK3, _TK4

_TK1:	defw	TK1
_TK2:	defw	TK2
_TK3:	defw	TK3
_TK4:	defw	TK4

TK1:	 DB 27,'[','3','1','m'  	 ; computer's token, ()
	 DB '(',')'
	 DB 27,'[','0','m'
	 DB 0

TK2:	 DB 27,'[','3','4','m'  	 ; player's token, []
	 DB '[',']'
	 DB 27,'[','0','m'
	 DB 0
TK3:	 DB 27,'[','3','1','m'  	 ; computer's alternate token
	 DB '{','}'
	 DB 27,'[','0','m'
	 DB 0
TK4:	 DB 27,'[','3','4','m'  	 ; player's alternate token
	 DB '<','>'
	 DB 27,'[','0','m'
	 DB 0
