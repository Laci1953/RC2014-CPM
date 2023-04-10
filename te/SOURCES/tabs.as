;
;	TABs handling routines
;
;	for TE text editor
;
;	Szilagyi Ladislau
;
;	March 2023

	global	_cf_tab_cols
	global	_ExpandTabs
	global	_DropBlanks
;
TAB	equ	9
BLANK	equ	20H
MAXLEN	equ	199
;
	psect	bss
;
OutBuf:	defs	200		;MAXLEN+1
;
	psect	text
;
;	expand tabs
;
;char*	ExpandTabs(char* pbuf);
;
_ExpandTabs:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source
	ld	de,OutBuf	;DE=dest
	ld	a,(_cf_tab_cols)
	dec	a
	ld	b,a		;B=tab width mask (11B or 111B)
	xor	a
	ld	c,a		;C=position in dest
loop:
	ld	a,(hl)		;get next from source
	or	a
	jr	z,done		;if zero, we are done
	inc	hl		;incr source pointer
	cp	TAB
	jr	z,istab		;if TAB, process-it
	ld	(de),a		;else, just store in dest
	inc	de		;incr dest pointer
	inc	c		;incr position in dest
	ld	a,c
	cp	MAXLEN		;if max length reached, done
	jr	z,done
	jr	loop
istab:				;process tab
	ld	a,BLANK+80H	;store a blank with high bit set in dest
tabloop:
	ld	(de),a
	inc	de		;incr dest pointer
	inc	c		;incr position in dest
	ld	a,c
	cp	MAXLEN		;if max length reached, done
	jr	z,done
	and	b		;if at tab boundary
	jr	z,loop		;return to loop
	ld	a,BLANK		;else store a blank in dest
	jr	tabloop		;and continue tab processing
done:	xor	a
	ld	(de),a		;store final zero
	ld	hl,OutBuf	;return dest buf
	ret
;
;	drop expanded blanks
;
;char*	DropBlanks(char* pbuf);
;
_DropBlanks:
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a		;HL=source
	ld	de,OutBuf	;DE=dest
loopdrop:
	ld	a,(hl)		;get next from source
	or	a
	jr	z,done		;if zero, we are done
	inc	hl		;incr source pointer
	cp	BLANK+80H	;if tab mark
	jr	z,istabmark	;process-it
storeit:
	ld	(de),a		;else, just store in dest
	inc	de		;incr dest pointer
	jr	loopdrop
istabmark:			;process tab mark
	ld	a,TAB
	ld	(de),a		;store a TAB in dest
	inc	de		;incr dest pointer
getnext:
	ld	a,(hl)		;get next from source
	or	a
	jr	z,done		;if zero, we are done
	inc	hl		;incr source pointer
	cp	BLANK		;if blank
	jr	z,getnext	;just skip-it
	cp	BLANK+80H	;if tab mark
	jr	z,istabmark	;process-it
	jr	storeit		;else, store char in dest
;
