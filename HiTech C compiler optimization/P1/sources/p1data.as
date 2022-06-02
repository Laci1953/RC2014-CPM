include p1.inc

	psect	data

 global l8ab6h
 global l8b36h
 global l8b3ah
 global l8b3eh
 global l8b4ah
 global l8b51h
 global l8b55h
 global l8b59h
 global l8b5eh
 global l8b65h
 global l8b68h
 global l8b6dh
 global l8b72h
 global l8b76h
 global l8b7ah
 global l8b7eh
 global l8b82h
 global l8b89h
 global l8b8fh
 global l8b93h
 global l8b97h
 global l8b9bh
 global l8b9fh
 global l8ba3h
 global l8babh
 global l8bafh
 global l8bb5h
 global l8bb8h
 global l8bbch
 global l8bc1h
 global l8bc7h
 global l8bc9h
 global l8bd7h
 global l8cbah
 global l8cbch
 global l8f6eh
 global l8f85h
 global l8f86h
 global l8f87h
 global l8fc5h
 global l90b9h
 global l90beh
 global l90c2h
 global l90d8h
 global l913ch
 global l9164h
 global l9170h
 global l9178h
 global l917fh
 global l918dh
 global l918eh
 global l9192h
 global l9199h
 global l91a1h
 global l91a8h
 global l91afh
 global l91beh
 global l91cdh
 global l91dbh
 global l91ddh
 global l91f2h
 global l91f4h
 global l91f6h
 global l91f8h
 global l9200h
 global l9205h
 global l920ah
 global l9226h
 global l922bh
 global l9247h
 global l925fh
 global l9261h
 global l9271h
 global l9274h
 global l9275h
 global l9276h
 global l9278h
 global l9491h
 global l949bh
 global l94a2h
 global l94a6h
 global l94cbh
 global l94cdh
 global l94cfh
 global l94e9h
 global l94ech
 global l94eeh
 global l94f0h
 global l94f2h
 global l950bh
 global l955fh
 global l9561h
 global l9563h
 global l956ah
 global l9571h
 global l9573h
 global l9575h
 global l9577h
 global l9579h
 global l957fh
 global l9581h
 global l9583h
 global l9585h
 global l9587h
 global l9589h
 global l95aeh
 global l95b0h
 global l95b2h
 global l95b4h
 global l95b6h
 global l95d5h
 global l95d7h
 global l9608h
 global l960ah
 global l9638h
 global l965dh
 global l966eh
 global l968bh
 global l968ch
 global l968eh
 global l9690h
 global l9697h
 global l96a5h
 global l96aah
 global l96b4h
 global l96c5h
 global l96cfh
 global l96fah
 global l9729h
 global l972fh
 global l9738h
 global l9749h
 global l974fh
 global l9759h
 global l975fh
 global l9766h
 global l976bh
 global l9774h
 global l977ch
 global l9790h
 global l97a5h
 global l97b3h
 global l97ddh
 global l97e1h
 global l9803h
 global l985fh
 global l9861h
 global l98f9h
 global l9974h
 global l998dh
 global l999bh
 global l999fh
 global l99c6h
 global l99cch
 global l99efh
 global l9a76h
 global l9a78h
 global l9b4ch
 global l9b8dh
 global l9ba2h
 global l9ba4h
 global l9ba8h
 global __iob
 global l9bc1h
 global l9bc9h
 global __fcb
 global l9c05h
 global nularg
 global text_end
 global data_end

IF SHORTMSG
 global msgerr0
 global msgerr1
 global msgwarning0
 global msgwarning1
ENDIF

IF LONGMSG
 global l9b8fh
 global l97e5h
 global l9b6ch
 global l9b4eh
 global l9b02h
 global l9b1ah
 global l9ae7h
 global l9ac9h
 global l9aaeh
 global l9a97h
 global l9a7ah
 global l9a61h
 global l9a49h
 global l9a37h
 global l9a14h
 global l99f1h
 global l99ceh
 global l99aah
 global l9976h
 global l995ah
 global l9948h
 global l992ch
 global l9911h
 global l98e7h
 global l98d8h
 global l98cah
 global l98b8h
 global l98a6h
 global l9879h
 global l9863h
 global l988bh
 global l9848h
 global l9824h
 global l980fh
 global l9702h
 global l96e1h
 global l9670h
 global l9623h
 global l95f0h
 global l95d9h
 global l95b8h
 global l958bh
 global l954ah
 global l94f4h
 global l94d3h
 global l94b4h
 global l9253h
 global l922dh
 global l9151h
 global l913eh
 global l912ah
 global l90fah
 global l90e6h
 global l9112h
 global l8f5bh
 global l8e87h
 global l8e79h
 global l8e3ch
 global l8e06h
 global l8e1dh
 global l8dbdh
 global l8dach
 global l8d80h
 global l8d68h
 global l8d40h
 global l8d25h
 global l8d08h
 global l8cf2h
 global l8cd0h
 global l8c9ch
 global l8c83h
 global l8cbeh
 global l8c62h
 global l8c45h
 global l8c33h
 global l8c20h
 global l8bffh
 global l9b2eh
 global l963ah
 global l9539h
 global l950dh
 global l920ch
 global l8eb8h
 global l8f32h
 global l8f09h
 global l8ee0h
 global l8e9dh
 global l8e5ah
 global l8ddfh
 global l8bd9h
ENDIF

text_end:
	defb 000h
	defb 000h
l8ab6h:
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 010h
	defb 010h
	defb 010h
	defb 000h
	defb 010h
	defb 010h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 010h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 00ch
	defb 00ch
	defb 00ch
	defb 00ch
	defb 00ch
	defb 00ch
	defb 00ch
	defb 00ch
	defb 00ch
	defb 00ch
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 009h
	defb 009h
	defb 009h
	defb 009h
	defb 009h
	defb 009h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 001h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 001h
	defb 000h
	defb 00ah
	defb 00ah
	defb 00ah
	defb 00ah
	defb 00ah
	defb 00ah
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 002h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
l8b36h:
	defb 022h
	defb 025h
	defb 064h
	defb 000h
l8b3ah:
	defb 020h
	defb 025h
	defb 073h
	defb 000h
l8b3eh:
	defb 05bh
	defb 065h
	defb 020h
	defb 03ah
	defb 055h
	defb 020h
	defb 025h
	defb 064h
	defb 020h
	defb 05dh
	defb 00ah
	defb 000h
l8b4ah:
	defb 05bh
	defb 065h
	defb 020h
	defb 03ah
	defb 055h
	defb 020h
	defb 000h
l8b51h:
	defb 020h
	defb 05dh
	defb 00ah
	defb 000h
l8b55h:
	defb 05bh
	defb 05ch
	defb 020h
	defb 000h
l8b59h:
	defb 020h
	defb 025h
	defb 064h
	defb 00ah
	defb 000h
l8b5eh:
	defb 02eh
	defb 02eh
	defb 020h
	defb 025h
	defb 064h
	defb 00ah
	defb 000h
l8b65h:
	defb 05dh
	defb 00ah
	defb 000h
l8b68h:
	defb 020h
	defb 053h
	defb 025h
	defb 064h
	defb 000h
l8b6dh:
	defb 020h
	defb 03ah
	defb 025h
	defb 064h
	defb 000h
l8b72h:
	defb 020h
	defb 05dh
	defb 00ah
	defb 000h
l8b76h:
	defb 05bh
	defb 065h
	defb 020h
	defb 000h
l8b7ah:
	defb 020h
	defb 05dh
	defb 00ah
	defb 000h
l8b7eh:
	defb 05bh
	defb 076h
	defb 020h
	defb 000h
l8b82h:
	defb 020h
	defb 025h
	defb 063h
	defb 020h
	defb 05dh
	defb 00ah
	defb 000h
l8b89h:
	defb 05bh
	defb 061h
	defb 020h
	defb 025h
	defb 064h
	defb 000h
l8b8fh:
	defb 020h
	defb 025h
	defb 064h
	defb 000h
l8b93h:
	defb 020h
	defb 05dh
	defb 00ah
	defb 000h
l8b97h:
	defb 02eh
	defb 020h
	defb 060h
	defb 000h
l8b9bh:
	defb 020h
	defb 025h
	defb 064h
	defb 000h
l8b9fh:
	defb 025h
	defb 06ch
	defb 064h
	defb 000h
l8ba3h:
	defb 02dh
	defb 03eh
	defb 020h
	defb 025h
	defb 06ch
	defb 064h
	defb 020h
	defb 000h
l8babh:
	defb 02eh
	defb 025h
	defb 073h
	defb 000h
l8bafh:
	defb 03ah
	defb 073h
	defb 020h
	defb 025h
	defb 064h
	defb 000h
l8bb5h:
	defb 025h
	defb 064h
	defb 000h
l8bb8h:
	defb 02dh
	defb 03eh
	defb 020h
	defb 000h
l8bbch:
	defb 02ah
	defb 020h
	defb 023h
	defb 020h
	defb 000h
l8bc1h:
	defb 025h
	defb 02eh
	defb 033h
	defb 073h
	defb 020h
	defb 000h
l8bc7h:
	defw	buf26
	;defb 01bh
	;defb 09dh
l8bc9h:
	defw l22ech
	;defb 0ech
	;defb 022h
	defw l2310h
	;defb 010h
	;defb 023h
	defw l235ch
	;defb 05ch
	;defb 023h
	defw l2330h
	;defb 030h
	;defb 023h
	defw l2387h
	;defb 087h
	;defb 023h
	defw l2396h
	;defb 096h
	;defb 023h
	defw l23a5h
	;defb 0a5h
	;defb 023h
l8bd7h:
	defb 000h
	defb 000h
l8cbah:
	defb 029h
	defb 000h
l8cbch:
	defb 05dh
	defb 000h
l8f6eh:
	defm	"expression too complex"
	defb 000h
l8f85h:
	defb 000h
l8f86h:
	defb 000h
l8f87h:
	defw l8fff
	;defb 0ffh
	;defb 08fh
	defw l9003
	;defb 003h
	;defb 090h
	defw l9008
	;defb 008h
	;defb 090h
	defw l900e
	;defb 00eh
	;defb 090h
	defw l9013
	;defb 013h
	;defb 090h
	defw l9018
	;defb 018h
	;defb 090h
	defw l9021
	;defb 021h
	;defb 090h
	defw l9029
	;defb 029h
	;defb 090h
	defw l902c
	;defb 02ch
	;defb 090h
	defw l9033
	;defb 033h
	;defb 090h
	defw l9038
	;defb 038h
	;defb 090h
	defw l903e
	;defb 03eh
	;defb 090h
	defw l9043
	;defb 043h
	;defb 090h
	defw l904a
	;defb 04ah
	;defb 090h
	defw l9050
	;defb 050h
	;defb 090h
	defw l9054
	;defb 054h
	;defb 090h
	defw l9059
	;defb 059h
	;defb 090h
	defw l905c
	;defb 05ch
	;defb 090h
	defw l9060
	;defb 060h
	;defb 090h
	defw l9065
	;defb 065h
	;defb 090h
	defw l906e
	;defb 06eh
	;defb 090h
	defw l9075
	;defb 075h
	;defb 090h
	defw l907b
	;defb 07bh
	;defb 090h
	defw l9082
	;defb 082h
	;defb 090h
	defw l9089
	;defb 089h
	;defb 090h
	defw l9090
	;defb 090h
	;defb 090h
	defw l9097
	;defb 097h
	;defb 090h
	defw l909f
	;defb 09fh
	;defb 090h
	defw l90a5
	;defb 0a5h
	;defb 090h
	defw l90ae
	;defb 0aeh
	;defb 090h
	defw l90b3
	;defb 0b3h
	;defb 090h
l8fc5h:
	defw l305ah
	;defb 05ah
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l3065h
	;defb 065h
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l3065h
	;defb 065h
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l3065h
	;defb 065h
	;defb 030h
	defw l305ah
	;defb 05ah
	;defb 030h
	defw l3065h
	;defb 065h
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l3065h
	;defb 065h
	;defb 030h
	defw l3065h
	;defb 065h
	;defb 030h
	defw l305ah
	;defb 05ah
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l3065h
	;defb 065h
	;defb 030h
	defw l3070h
	;defb 070h
	;defb 030h
	defw l305ah
	;defb 05ah
	;defb 030h
	defw l3065h
	;defb 065h
	;defb 030h
	defw l3075h
	;defb 075h
	;defb 030h
	defw l305ah
	;defb 05ah
	;defb 030h
	defw l3065h
	;defb 065h
	;defb 030h
	defw l3065h
	;defb 065h
	;defb 030h
	defw l3065h
	;defb 065h
	;defb 030h
l8fff:	defm	"asm"
	defb 000h
l9003:	defm	"auto"
	defb 000h
l9008:	defm	"break"
	defb 000h
l900e:	defm	"case"
	defb 000h
l9013:	defm	"char"
	defb 000h
l9018:	defm	"continue"
	defb 000h
l9021:	defm	"default"
	defb 000h
l9029:	defm	"do"
	defb 000h
l902c:	defm	"double"
	defb 000h
l9033:	defm	"else"
	defb 000h
l9038:	defm	"@@@@@"
	defb 000h
l903e:	defm	"enum"
	defb 000h
l9043:	defm	"extern"
	defb 000h
l904a:	defm	"float"
	defb 000h
l9050:	defm	"for"
	defb 000h
l9054:	defm	"goto"
	defb 000h
l9059:	defm	"if"
	defb 000h
l905c:	defm	"int"
	defb 000h
l9060:	defm	"long"
	defb 000h
l9065:	defm	"register"
	defb 000h
l906e:	defm	"return"
	defb 000h
l9075:	defm	"short"
	defb 000h
l907b:	defm	"sizeof"
	defb 000h
l9082:	defm	"static"
	defb 000h
l9089:	defm	"struct"
	defb 000h
l9090:	defm	"switch"
	defb 000h
l9097:	defm	"typedef"
	defb 000h
l909f:	defm	"union"
	defb 000h
l90a5:	defm	"unsigned"
	defb 000h
l90ae:	defm	"void"
	defb 000h
l90b3:	defm	"while"
	defb 000h
l90b9h:
	defm	"~%s"
	defb 0ah
	defb 000h
l90beh:
	defm	"asm"
	defb 000h
l90c2h:
	defm	"illegal '#' directive"
	defb 000h
l90d8h:
	defm	"closing quote"			;!!!duplicate l917fh
	defb 000h
l913ch:
	defb 030h
	defb 000h
l9164h:
	defm	"EOF in #asm"
	defb 000h
l9170h:
	defm	"#endasm"
	defb 000h
l9178h:
	defm	";; %s"
	defb 0ah
	defb 000h
l917fh:
	defm	"closing quote"
	defb 000h
l918dh:
	defb 000h
l918eh:
	defm	"%s:"
	defb 000h
l9192h:
	defm	" %s()"
	defb 0ah
	defb 000h
l9199h:
	defb 025h
	defb 036h
	defb 064h
	defb 03ah
	defb 009h
	defb 025h
	defb 073h
	defb 000h
l91a1h:
	defm	"%*s ^ "
	defb 000h
l91a8h:
	defm	"%*c %s"
	defb 000h
l91afh:
	defm	";; ;%s: %d: %s"
	defb 000h
l91beh:
	defm	"unexpected EOF"
	defb 000h
l91cdh:
	defm	"Out of memory"
	defb 000h
l91dbh:
	defw l91ebh
	;defb 0ebh
	;defb 091h
l91ddh:
	defm	"Can't open %s"
	defb 000h
l91ebh:	
	defm	"p1.tmp"
	defb 000h
l91f2h:
	defb 061h
	defb 000h
l91f4h:
	defb 072h
	defb 000h
l91f6h:
	defb 077h
	defb 000h
l91f8h:
	defm	"(stdin)"
	defb 000h
l9200h:
	defm	".crf"				;!!!duplicate l9205h
	defb 000h
l9205h:
	defm	".crf"
	defb 000h
l920ah:
	defb 061h
	defb 000h
l9226h:
	defm	"~%s"
	defb 0ah
	defb 000h
l922bh:
	defb 077h
	defb 000h
l9247h:
	defm	" (warning)"
	defb 0ah
	defb 000h
l925fh:
	defb 072h
	defb 000h
l9261h:
	defm	"Can't reopen %s"
	defb 000h
l9271h:
	defb 000h
	defb 000h
	defb 000h
l9274h:
	defb 006h
l9275h:
	defb 000h
l9276h:
	defb 000h
	defb 002h
l9278h:
	defb 007h
	defb 028h
	defb 000h
	defb 000h
	defb 01eh
	defb 002h
	defb 000h
	defb 002h
	defb 003h
	defb 000h
	defb 000h
	defb 000h
	defb 01fh
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 002h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 01eh
	defb 002h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 01eh
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 002h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 02eh
	defb 000h
	defb 000h
	defb 01eh
	defb 002h
	defb 000h
	defb 00ah
	defb 002h
	defb 000h
	defb 000h
	defb 000h
	defb 01eh
	defb 002h
	defb 000h
	defb 000h
	defb 000h
	defb 02ah
	defb 055h
	defb 000h
	defb 01ch
	defb 01ch
	defb 000h
	defb 042h
	defb 003h
	defb 026h
	defb 055h
	defb 000h
	defb 01ch
	defb 01ch
	defb 000h
	defb 022h
	defb 004h
	defb 02dh
	defb 055h
	defb 000h
	defb 01ch
	defb 01ch
	defb 030h
	defb 000h
	defb 001h
	defb 021h
	defb 000h
	defb 000h
	defb 01ch
	defb 008h
	defb 002h
	defb 000h
	defb 006h
	defb 07eh
	defb 000h
	defb 000h
	defb 01ch
	defb 018h
	defb 020h
	defb 000h
	defb 001h
	defb 000h
	defb 000h
	defb 000h
	defb 01ch
	defb 008h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 01ch
	defb 008h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 01ch
	defb 008h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 01ch
	defb 008h
	defb 000h
	defb 000h
	defb 000h
	defb 023h
	defb 000h
	defb 000h
	defb 01ch
	defb 018h
	defb 000h
	defb 003h
	defb 005h
	defb 028h
	defb 029h
	defb 000h
	defb 01ch
	defb 00ah
	defb 000h
	defb 000h
	defb 000h
	defb 02ah
	defb 000h
	defb 000h
	defb 01ah
	defb 016h
	defb 070h
	defb 000h
	defb 001h
	defb 026h
	defb 000h
	defb 000h
	defb 011h
	defb 016h
	defb 060h
	defb 000h
	defb 001h
	defb 02dh
	defb 000h
	defb 000h
	defb 018h
	defb 016h
	defb 07ch
	defb 000h
	defb 001h
	defb 02fh
	defb 000h
	defb 000h
	defb 01ah
	defb 012h
	defb 070h
	defb 000h
	defb 001h
	defb 025h
	defb 000h
	defb 000h
	defb 01ah
	defb 012h
	defb 060h
	defb 000h
	defb 001h
	defb 02bh
	defb 000h
	defb 000h
	defb 018h
	defb 012h
	defb 074h
	defb 000h
	defb 001h
	defb 03eh
	defb 03eh
	defb 000h
	defb 016h
	defb 012h
	defb 060h
	defb 000h
	defb 001h
	defb 03ch
	defb 03ch
	defb 000h
	defb 016h
	defb 012h
	defb 060h
	defb 000h
	defb 001h
	defb 03ch
	defb 000h
	defb 000h
	defb 014h
	defb 012h
	defb 0f8h
	defb 010h
	defb 006h
	defb 03eh
	defb 000h
	defb 000h
	defb 014h
	defb 012h
	defb 0f8h
	defb 010h
	defb 006h
	defb 03ch
	defb 03dh
	defb 000h
	defb 014h
	defb 012h
	defb 0f8h
	defb 010h
	defb 006h
	defb 03eh
	defb 03dh
	defb 000h
	defb 014h
	defb 012h
	defb 0f8h
	defb 010h
	defb 006h
	defb 03dh
	defb 03dh
	defb 000h
	defb 012h
	defb 012h
	defb 0f8h
	defb 090h
	defb 006h
	defb 021h
	defb 03dh
	defb 000h
	defb 012h
	defb 012h
	defb 0f8h
	defb 090h
	defb 006h
	defb 05eh
	defb 000h
	defb 000h
	defb 010h
	defb 012h
	defb 060h
	defb 000h
	defb 001h
	defb 07ch
	defb 000h
	defb 000h
	defb 00fh
	defb 012h
	defb 060h
	defb 000h
	defb 001h
	defb 026h
	defb 026h
	defb 000h
	defb 00eh
	defb 002h
	defb 003h
	defb 000h
	defb 006h
	defb 07ch
	defb 07ch
	defb 000h
	defb 00dh
	defb 002h
	defb 003h
	defb 000h
	defb 006h
	defb 03fh
	defb 000h
	defb 000h
	defb 00ch
	defb 01ah
	defb 002h
	defb 000h
	defb 002h
	defb 03ah
	defb 000h
	defb 000h
	defb 00ch
	defb 01ah
	defb 0f8h
	defb 012h
	defb 001h
	defb 03dh
	defb 000h
	defb 000h
	defb 00ah
	defb 00ah
	defb 000h
	defb 026h
	defb 001h
	defb 03dh
	defb 02bh
	defb 000h
	defb 00ah
	defb 00ah
	defb 034h
	defb 024h
	defb 001h
	defb 02bh
	defb 02bh
	defb 000h
	defb 000h
	defb 00ah
	defb 034h
	defb 024h
	defb 001h
	defb 03dh
	defb 02dh
	defb 000h
	defb 00ah
	defb 00ah
	defb 034h
	defb 024h
	defb 001h
	defb 02dh
	defb 02dh
	defb 000h
	defb 000h
	defb 00ah
	defb 034h
	defb 024h
	defb 001h
	defb 03dh
	defb 02ah
	defb 000h
	defb 00ah
	defb 00ah
	defb 030h
	defb 024h
	defb 001h
	defb 03dh
	defb 02fh
	defb 000h
	defb 00ah
	defb 00ah
	defb 030h
	defb 024h
	defb 001h
	defb 03dh
	defb 025h
	defb 000h
	defb 00ah
	defb 00ah
	defb 020h
	defb 024h
	defb 001h
	defb 03dh
	defb 03eh
	defb 03eh
	defb 00ah
	defb 00ah
	defb 020h
	defb 020h
	defb 001h
	defb 03dh
	defb 03ch
	defb 03ch
	defb 00ah
	defb 00ah
	defb 020h
	defb 020h
	defb 001h
	defb 03dh
	defb 026h
	defb 000h
	defb 00ah
	defb 00ah
	defb 020h
	defb 024h
	defb 001h
	defb 03dh
	defb 05eh
	defb 000h
	defb 00ah
	defb 00ah
	defb 020h
	defb 024h
	defb 001h
	defb 03dh
	defb 07ch
	defb 000h
	defb 00ah
	defb 00ah
	defb 020h
	defb 024h
	defb 001h
	defb 02ch
	defb 000h
	defb 000h
	defb 007h
	defb 002h
	defb 000h
	defb 002h
	defb 007h
	defb 03bh
	defb 000h
	defb 000h
	defb 007h
	defb 002h
	defb 000h
	defb 002h
	defb 002h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 001h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 011h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 011h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 011h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 011h
	defb 000h
	defb 000h
	defb 000h
	defb 02eh
	defb 02eh
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 029h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 002h
	defb 007h
	defb 024h
	defb 055h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 002h
	defb 007h
	defb 024h
	defb 000h
	defb 000h
	defb 000h
	defb 002h
	defb 002h
	defb 000h
	defb 007h
	defb 02dh
	defb 03eh
	defb 000h
	defb 000h
	defb 012h
	defb 080h
	defb 008h
	defb 002h
	defb 040h
	defb 000h
	defb 000h
	defb 000h
	defb 002h
	defb 080h
	defb 004h
	defb 007h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 001h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 011h
	defb 000h
	defb 000h
	defb 000h
l9491h:
	defm	"-> %d `c"
	defb 0ah
	defb 0
l949bh:
	defm	":U .."
	defb 0ah
	defb 000h
l94a2h:
	defm	".."
	defb 0ah
	defb 000h
l94a6h:
	defm	"function body"
	defb 000h
l94cbh:
	defb 02ch
	defb 000h
l94cdh:
	defb 03bh
	defb 000h
l94cfh:
	defb 05bh
	defb 069h
	defb 020h
	defb 000h
l94e9h:
	defb 05dh
	defb 00ah
	defb 000h
l94ech:
	defb 07bh
	defb 000h
l94eeh:
	defb 07dh
	defb 000h
l94f0h:
	defb 07dh
	defb 000h
l94f2h:
	defb 07dh
	defb 000h
l950bh:
	defb 07bh
	defb 000h
l955fh:
	defb 03bh
	defb 000h
l9561h:
	defb 028h
	defb 000h
l9563h:
	defm	"string"
	defb 000h
l956ah:
	defm	";; %s"
	defb 0ah
	defb 000h
l9571h:
	defb 029h
	defb 000h
l9573h:
	defb 03bh
	defb 000h
l9575h:
	defb 028h
	defb 000h
l9577h:
	defb 029h
	defb 000h
l9579h:
	defm	"while"
	defb 000h
l957fh:
	defb 028h
	defb 000h
l9581h:
	defb 029h
	defb 000h
l9583h:
	defb 03bh
	defb 000h
l9585h:
	defb 028h
	defb 000h
l9587h:
	defb 029h
	defb 000h
l9589h:
	defb 028h
	defb 000h
l95aeh:
	defb 029h
	defb 000h
l95b0h:
	defb 028h
	defb 000h
l95b2h:
	defb 03bh
	defb 000h
l95b4h:
	defb 03bh
	defb 000h
l95b6h:
	defb 029h
	defb 000h
l95d5h:
	defb 03bh
	defb 000h
l95d7h:
	defb 03ah
	defb 000h
l9608h:
	defb 03ah
	defb 000h
l960ah:
	defm	"Too many cases in switch"
	defb 000h
l9638h:
	defb 03bh
	defb 000h
l965dh:
	defm	"label identifier"
	defb 000h
l966eh:
	defb 03bh
	defb 000h
l968bh:
	defb 000h
l968ch:
	defb 000h
	defb 000h
l968eh:
	defb 000h
	defb 000h
l9690h:
	defm	"%s %d"
	defb 0ah
	defb 000h
l9697h:
	defm	"storage class"
	defb 000h
l96a5h:
	defm	"type"
	defb 000h
l96aah:
	defm	"arguments"
	defb 000h
l96b4h:
	defm	"no. of arguments"
	defb 000h
l96c5h:
	defm	"arguments"
	defb 000h
l96cfh:
	defm	"%s: %s redeclared"
	defb 000h
l96fah:
	defm	"#%s %d"
	defb 0ah
	defb 000h
l9729h:
	defm	"label"	
	defb 000h
l972fh:
	defm	"variable"
	defb 000h
l9738h:
	defm	"undefined %s: %s"
	defb 000h
l9749h:
	defm	"label"
	defb 000h
l974fh:
	defm	"structure"
	defb 000h
l9759h:
	defm	"union"
	defb 000h
l975fh:
	defm	"member"
	defb 000h
l9766h:
	defm	"enum"
	defb 000h
l976bh:
	defm	"constant"
	defb 000h
l9774h:
	defm	"typedef"
	defb 000h
l977ch:
	defm	"variable definition"
	defb 000h
l9790h:
	defm	"variable declaration"
	defb 000h
l97a5h:
	defm	"unused %s: %s"
	defb 000h
l97b3h:
	defm	"%s is not a member of the struct/union %s"
	defb 000h
l97ddh:
	defm	"F%d"
	defb 000h
l97e1h:
	defm	"_%s"
	defb 000h
l9803h:
	defm	"?bcsilxfd?v"
	defb 000h
l985fh:
	defb 02ch
	defb 000h
l9861h:
	defb 03bh
	defb 000h
l98f9h:
	defm	"struct/union tag or '{'"
	defb 000h
l9974h:
	defb 03bh
	defb 000h
l998dh:
	defm	"enum tag or {"
	defb 000h
l999bh:
	defb 05bh
	defb 063h
	defb 020h
	defb 000h
l999fh:
	defm	"identifier"
	defb 000h
l99c6h:
	defb 02eh
	defb 02eh
	defb 020h
	defb 05dh
	defb 00ah
	defb 000h
l99cch:
	defb 07dh
	defb 000h
l99efh:
	defb 07dh
	defb 000h
l9a76h:
	defb 02ch
	defb 000h
l9a78h:
	defb 029h
	defb 000h
l9b4ch:
	defb 029h
	defb 000h
l9b8dh:
	defb 029h
	defb 000h
l9ba2h:
	defb 05dh
	defb 000h
l9ba4h:
	defb 053h
	defb 025h
	defb 064h
	defb 000h
l9ba8h:
	defm	"0123456789ABCDEF"
	defb 000h

IF LONGMSG
l9b8fh:
	defm	"dimension required"
	defb 000h
l97e5h:
	defm	"functions can't return arrays"
	defb 000h
l9b6ch:
	defm	"functions can't return functions"
	defb 000h
l9b4eh:
	defm	"can't have array of functions"
	defb 000h
l9b02h:
	defm	"argument redeclared: %s"
	defb 000h
l9b1ah:
	defm	"not an argument: %s"
	defb 000h
l9ae7h:
	defm	"undefined struct/union: %s"
	defb 000h
l9ac9h:
	defm	"can't have array of functions"		;!!!duplicate l9b4eh
	defb 000h
l9aaeh:
	defm	"only functions may be void"
	defb 000h
l9a97h:
	defm	"declarator too complex"
	defb 000h
l9a7ah:
	defm	"no identifier in declaration"
	defb 000h
l9a61h:
	defm	"can't initialize arg"
	defb 000h
l9a49h:
	defm	"argument redeclared: %s"
	defb 000h
l9a37h:
	defm	"bad storage class"
	defb 000h
l9a14h:
	defm	"can't mix proto and non-proto args"
	defb 000h
l99f1h:
	defm	"type specifier reqd. for proto arg"
	defb 000h
l99ceh:
	defm	"can't initialise auto aggregates"
	defb 000h
l99aah:
	defm	"integer expression required"
	defb 000h
l9976h:
	defm	"undefined enum tag: %s"
	defb 000h
l995ah:
	defm	"integer constant expected"
	defb 000h
l9948h:
	defm	"bad bitfield type"
	defb 000h
l992ch:
	defm	"members cannot be functions"
	defb 000h
l9911h:
	defm	"struct/union redefined: %s"
	defb 000h
l98e7h:
	defm	"can't be unsigned"
	defb 000h
l98d8h:
	defm	"can't be short"
	defb 000h
l98cah:
	defm	"can't be long"
	defb 000h
l98b8h:
	defm	"can't be register"
	defb 000h
l98a6h:
	defm	"inconsistent type"
	defb 000h
l9879h:
	defm	"bad storage class"
	defb 000h
l9863h:
	defm	"storage class illegal"
	defb 000h
l988bh:
	defm	"inconsistent storage class"
	defb 000h
l9848h:
	defm	"illegal initialisation"
	defb 000h
l9824h:
	defm	"only register storage class allowed"
	defb 000h
l980fh:
	defm	"too much indirection"
	defb 000h
l9702h:
	defm	"argument list conflicts with prototype"
	defb 000h
l96e1h:
	defm	"identifier redefined: %s"
	defb 000h
l9670h:
	defm	"not a label identifier: %s"
	defb 000h
l9623h:
	defm	"'case' not in switch"
	defb 000h
l95f0h:
	defm	"'default' not in switch"
	defb 000h
l95d9h:
	defm	"default case redefined"
	defb 000h
l95b8h:
	defm	"inappropriate break/continue"
	defb 000h
l958bh:
	defm	"illegal type for switch expression"
	defb 000h
l954ah:
	defm	"inappropriate 'else'"
	defb 000h
l94f4h:
	defm	"illegal initialisation"
	defb 000h
l94d3h:
	defm	"initialisation syntax"
	defb 000h
l94b4h:
	defm	"illegal initialisation"		;!!!duplicate l94f4h
	defb 000h
l9253h:
	defm	"%s expected"
	defb 000h
l922dh:
	defm	"close error (disk space?)"
	defb 000h
l9151h:
	defm	"digit out of range"
	defb 000h
l913eh:
	defm	"hex digit expected"
	defb 000h
l912ah:
	defm	"exponent expected"
	defb 000h
l90fah:
	defm	"'.' expected after '..'"
	defb 000h
l90e6h:
	defm	"char const too long"
	defb 000h
l9112h:
	defm	"illegal character (0%o)"
	defb 000h
l8f5bh:
	defm	"illegal conversion"
	defb 000h
l8e87h:
	defm	"logical type required"
	defb 000h
l8e79h:
	defm	"type conflict"
	defb 000h
l8e3ch:
	defm	"simple type required for %.3s"
	defb 000h
l8e06h:
	defm	"integral type required"
	defb 000h
l8e1dh:
	defm	"illegal use of void expression"
	defb 000h
l8dbdh:
	defm	"void function cannot return value"
	defb 000h
l8dach:
	defm	"pointer required"
	defb 000h
l8d80h:
	defm	"only lvalues may be assigned to or modified"
	defb 000h
l8d68h:
	defm	"can't take this address"
	defb 000h
l8d40h:
	defm	"can't take address of register variable"
	defb 000h
l8d25h:
	defm	"undefined struct/union: %s"
	defb 000h
l8d08h:
	defm	"struct/union member expected"
	defb 000h
l8cf2h:
	defm	"struct/union required"
	defb 000h
l8cd0h:
	defm	"illegal type for index expression"
	defb 000h
l8c9ch:
	defm	"not a variable identifier: %s"
	defb 000h
l8c83h:
	defm	"undefined identifier: %s"
	defb 000h
l8cbeh:
	defm	"expression syntax"
	defb 000h
l8c62h:
	defm	"illegal type for array dimension"
	defb 000h
l8c45h:
	defm	"constant expression required"
	defb 000h
l8c33h:
	defm	"too few arguments"
	defb 000h
l8c20h:
	defm	"too many arguments"
	defb 000h
l8bffh:
	defm	"function does not take arguments"
	defb 000h
l9b2eh:
	defm	"float param coerced to double"
	defb 000h
l963ah:
	defm	"non-void function returns no value"
	defb 000h
l9539h:
	defm	"Unreachable code"
	defb 000h
l950dh:
	defm	"implicit return at end of non-void function"
	defb 000h
l920ch:
	defm	"Can't create xref file %s"
	defb 000h
l8eb8h:
	defm	"implicit conversion of float to integer"
	defb 000h
l8f32h:
	defm	"illegal conversion between pointer types"
	defb 000h
l8f09h:
	defm	"illegal conversion of pointer to integer"
	defb 000h
l8ee0h:
	defm	"illegal conversion of integer to pointer"
	defb 000h
l8e9dh:
	defm	"%s() declared implicit int"
	defb 000h
l8e5ah:
	defm	"operands of %.3s not same type"
	defb 000h
l8ddfh:
	defm	"operands of %.3s not same pointer type"
	defb 000h
l8bd9h:
	defm	"function or function pointer required"
	defb 000h
ENDIF

IF SHORTMSG
msgerr0:defm	"Err #%02d"
	defb	0
msgerr1:defm	"Err #%02d %s"
	defb	0
msgwarning0:
	defm	"Warning #%02d"
	defb	0
msgwarning1:
	defm	"Warning #%02d %s"
	defb	0
ENDIF

__iob:
	defw bigbuf
	;defb 0ach
	;defb 0a2h
	defb 000h
	defb 000h
	defw bigbuf
	;defb 0ach
	;defb 0a2h
	defb 009h
	defb 000h
l9bc1h:
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 006h
	defb 001h
l9bc9h:
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 006h
	defb 002h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
__fcb:
	defb 000h
	defb 009h
	defb 020h
	defb 020h
	defb 020h
	defb 020h
	defb 000h
	defb 000h
	defb 000h
	defb 020h
	defb 020h
	defb 020h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
l9c05h:
	defb 004h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 009h
	defb 020h
	defb 020h
	defb 020h
	defb 020h
	defb 000h
	defb 000h
	defb 000h
	defb 020h
	defb 020h
	defb 020h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 004h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 009h
	defb 020h
	defb 020h
	defb 020h
	defb 020h
	defb 000h
	defb 000h
	defb 000h
	defb 020h
	defb 020h
	defb 020h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 004h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
	defb 000h
nularg:
	defb 000h
data_end:
