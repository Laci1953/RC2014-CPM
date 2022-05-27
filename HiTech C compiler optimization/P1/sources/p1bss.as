include p1.inc

	psect	bss

 global buf26
 global buf27
 global buf28
 global buf52
 global buf25
 global buf29
 global buf30
 global buf51
 global buf5	
 global buf6
 global buf17	
 global buf12
 global buf8
 global buf9
 global buf47	
 global buf7
 global buf10
 global buf18
 global buf13
 global buf16
 global buf49
 global buf15
 global buf50
 global buf2
 global buf24
 global buf3
 global buf23
 global buf1
 global buf14
 global buf20
 global buf4
 global buf21
 global buf31
 global buf22
 global buf33
 global buf34
 global buf35
 global buf11
 global buf32
 global buf36
 global buf37
 global buf38
 global buf40
 global buf39
 global buf41
 global buf53
 global buf42
 global buf44
 global buf43
 global buf54	
 global _left
 global _ffile
 global _base
 global __argc_
 global memtop
 global __Hbss
 global __Lbss
 global l9cf1h
 global bigbuf
 global l9cb1h
 global buf45
 global l9cf3h
 global buf46
 global buf48

__Lbss:	defs	2
l9cb1h:	defs	64
l9cf1h:	defs	2
l9cf3h: defs	40
buf26:	defs	1	
buf27:	defs	2
buf28:	defs	8
buf52:	defs	2
buf25:	defs	1
buf29:	defs	2
buf30:	defs	8
buf51:	defs	2
buf5:	defs	2	
buf6:	defs	1
buf46:	defs	40
buf45:	defs	64
buf17:	defs	1	
buf12:	defs	2
buf48:	defs	40
buf8:	defs	2
buf9:	defs	2
buf47:	defs	32
buf7:	defs	1
buf10:	defs	2
buf18:	defs	1
buf13:	defs	2
buf16:	defs	2
buf49:	defs	512
buf15:	defs	2
buf50:	defs	30
buf2:	defs	100
buf24:	defs	2
buf3:	defs	1
buf23:	defs	1
buf1:	defs	2
buf14:	defs	2
buf20:	defs	1
buf4:	defs	2
IF OPTIM
buf21:	defs	128
ELSE
buf21:	defs	512
ENDIF
buf31:	defs	2
buf22:	defs	1
buf33:	defs	1
buf34:	defs	1
buf35:	defs	2
buf11:	defs	2
buf32:	defs	2
buf36:	defs	2
buf37:	defs	2
buf38:	defs	2
buf40:	defs	2
buf39:	defs	1
buf41:	defs	2
buf53:	defs	2
buf42:	defs	2
buf44:	defs	2
buf43:	defs	4
buf54:	defs	2	
_left:	defs	1
_ffile:	defs	2
_base:	defs	1
bigbuf:	defs	512
__argc_:defs	2
memtop:	defs	2
__Hbss:
