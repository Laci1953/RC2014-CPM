;
;	PDIR - paged & sorted disk directory list with file sizes for CP/M
;
;	Ladislau Szilagyi
;
;	June 2023
;

BDOS   		equ     5               ;entry point for the CP/M BDOS.
TFCB    	equ     5CH             ;default file control block.
TBUFF   	equ     80H             ;i/o buffer and command line storage.

CR		equ	0DH
LF		equ	0AH

MAX_FILES_CNT	equ	512

;
;	CP/M Command line structure
;
;	len (byte)
;	chars (zero terminated)
;
CommLine	equ	80H

	psect	bss

CrtFile:	defs	11
FileNames:	defs	11 * MAX_FILES_CNT	;store matching file names
		defs	11
FilesCnt:	defs	2
FilesCount:	defs	2
FilesPointer:	defs	2
FilesPerLine:	defs	1
LinesPerPage:	defs	1
BUF4:	DEFS	4
LBUF4:	DEFS	1
;
	psect	data

RtnCode:defb      0       	;status returned from bdos call.
ChgDrv: defb      0       	;change in drives flag (0=no change).
CrtDrv: defb      0       	;currently active drive.
;
NamePnt:defw      0       	;input line pointer used for error message. 
				;Points to start of name in error.

NoFile: defm    "No file name match!$"
TooManyFiles:
	defm	"Too many files match!$"
NextPage:
	defm	"Hit any key for the next page!$"
Blanks:	defm	"    "
Files:	defm	" file(s)$"

fcb:				; fcb
	defb	0		; disk+1
name:	defb	0,0,0,0,0,0,0,0,0,0,0 ; file name
	defb	0		; EX=0
	defb	0,0		; S1,S2
	defb	0		; RC=0
	defb	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	; D0,...D15
fcbcr:	defb	0		; CR=0
R0:	defb	0
R1:	defb	0
R2:	defb	0		; R0,R1,R2

	psect	text
;
;	pdir [file.ext] 
;
start:
 	ld	sp,(6)
	ld	c,25
	call	BDOS
	ld	(CrtDrv),a	;current disk
	ld      de,TBUFF
        call    SetDMA          ;set standard dma address.
	ld	hl,CommLine
	ld	a,(hl)		;len
	or	a
	jr	nz,notnull
				;if zero, set command line to '*.*'
	push	af
	ld	a,3
	ld	(hl),a
	inc	hl
	ld	a,'*'
	ld	(hl),a
	inc	hl
	ld	a,'.'
	ld	(hl),a
	inc	hl
	ld	a,'*'
	ld	(hl),a
	dec	hl
	dec	hl
	dec	hl
	pop	af
notnull:
	inc	hl
	call	AddHL		;HL=HL+A
	ld	(hl),0		;place a zero at the end of command line

	call    ConvFirst       ;convert file name.
        call    DriveSel        ;select indicated drive.
        ld      hl,TFCB+1       ;was any file indicated?
        ld      a,(hl)
        cp      ' '
        jp      nz,search
        ld      b,11            ;no. Fill field with '?' - same as *.*.
1:	ld      (hl),'?'
        inc     hl
        dec     b		
        jp      nz,1b
search: 
	call    SearchFCB       ;get first file name.
        jp      z,None          ;none found at all?
	ld	hl,0		;init file counter
	ld	(FilesCnt),hl
loop:
	ld      a,(RtnCode)     ;get file's position in segment (0-3).
        rrca	
        rrca
        rrca
        and     60H             ;(A)=position*32
        ld      c,a
        ld      a,10
        call    Extract         ;extract the tenth entry in fcb.
        rla                     ;check system file status bit.
        jp      c,skip          ;we don't list them.
	ld	de,CrtFile
        ld      b,1             ;'extract' each file name character at a time.
1:	ld      a,b
        call    Extract
        and     7FH             ;strip bit 7 (status bit).
	ld	(de),a		;store file name
	inc	de
        inc     b               ;bump to next character position.
        ld      a,b
        cp      12              ;end of the name?
        jp      c,1b

	ld	hl,CrtFile
	call	Add		;insert file into list
	xor	a
	ld	hl,(FilesCnt)	
	ld	bc,MAX_FILES_CNT+1
	sbc	hl,bc
	jp	z,Toomany
skip:   call    SearchNext      ;get next file name.
	jr	z,List		;if no more, go list
        jp      loop            ;else continue with our list.
;
;   Required file(s) not located.
;
None:   ld      de,NoFile
        call    PrintLine
	jp	0		;reboot
;
;	Too many files
;
Toomany:ld      de,TooManyFiles
        call    PrintLine
	jp	0		;reboot
;
;	List filenames
;
List:
	ld	hl,(FilesCnt)
	ld	(FilesCount),hl	;save files counter
	ld	hl,FileNames	;init files pointer
	ld	(FilesPointer),hl
	call	GetDisk		;store current disk in fcb
	inc	a
	ld	(fcb),a	
nextpage:			;type a page
	ld	a,46
	ld	(LinesPerPage),a
nextline:			;type a line
	ld	a,3
	ld	(FilesPerLine),a
nextfile:
	ld	hl,(FilesPointer);fetch next file name
	ld	de,name
	ld	bc,11
	ldir
	ld	(FilesPointer),hl;update files pointer

	ld	hl,name		;type the file name
	ld	b,8
	call	Print_B_chars

	ld	e,' '		;type a blank
	ld	c,2
	call	BDOS

	ld	hl,name+8	;type the file ext
	ld	b,3
	call	Print_B_chars

	ld	e,' '		;type a blank
	ld	c,2
	call	BDOS

	xor	a		;prepare fcb
	ld	(fcbcr),a
	ld	de,fcb		;get file size
	ld	c,35
	call	BDOS
				;type file size as nnnnK
	ld	hl,(R0)		;HL = count of 128-byte records
				;convert-it in KB
	ld	a,l
	srl	h
	rr	l
	srl	h
	rr	l
	srl	h
	rr	l
	and	7
	jr	z,1f
	inc	hl
1:	
	call	CNVHL		;returns BUF4, LBUF4
				;type 4 - (LBUF4) blanks
	ld	a,(LBUF4)
	ld	c,a
	ld	a,4
	sub	c
	or	a
	jr	z,noblanks
	ld	b,a
	ld	hl,Blanks
	call	Print_B_chars
noblanks:
	call	typeBUF4

	ld	e,'K'		;type 'K'
	ld	c,2
	call	BDOS

	ld	e,';'		;type ';'
	ld	c,2
	call	BDOS

	ld	e,' '		;type a blank
	ld	c,2
	call	BDOS

	ld	hl,(FilesCnt)	;decrement files counter
	dec	hl
	ld	(FilesCnt),hl
	ld	a,l		;if no files left...
	or	h
	jp	z,typecount	;go type files count
				
	ld	a,(FilesPerLine);decrement files/line
	dec	a
	ld	(FilesPerLine),a
	jp	nz,nextfile
				;next line
	call	CrLf
				;decrement lines/page
	ld	a,(LinesPerPage)
	dec	a
	ld	(LinesPerPage),a
	jp	nz,nextline

	ld	de,NextPage	;type 'hit any key...'
	ld	c,9
	call	BDOS
wait:				;wait for any key...
	ld	c,6
	ld	e,0FFH
	call	BDOS
	or	a
	jr	z,wait
	call	CrLf		;next page
	jp	nextpage
;
typecount:
	call	CrLf
	ld	hl,(FilesCount)
	call	CNVHL		;returns BUF4, LBUF4
	call	typeBUF4
	ld	de,Files
	call	PrintLine
	jp	0		;exit

typeBUF4:			;type (LBUF4) chars from BUF4, in reverse order
	ld	hl,BUF4-1
	ld	a,(LBUF4)
	ld	b,0
	ld	c,a
	add	hl,bc		;pointer of first char to type
	ld	b,a		;counter of chars
loopdigs:
	ld	e,(hl)
	dec	hl
	push	bc
	push	hl
	ld	c,2
	call	BDOS
	pop	hl
	pop	bc
	djnz	loopdigs
	ret
;
; converts (HL) in decimal ASCII to (BUF4) in reverse order of digits
; returns (LBUF4) = digits counter 

CNVHL:	LD	IY,BUF4
	LD	D,0
CNVERT:	LD	E,0
	LD	B,16
	OR	A
DVLOOP:	RL	L
	RL	H
	RL	E
	LD	A,E
	SUB	10
	CCF
	JR	NC,DECCNT
	LD	E,A
DECCNT:	DJNZ	DVLOOP
	RL	L
	RL	H
	LD	A,E
	ADD	A,'0'
	LD	(IY+0),A
	INC	IY
	INC	D
	LD	A,H
	OR	L
	JR	NZ,CNVERT
	LD	A,D
	LD	(LBUF4),a
	RET
;
;	print A
;
Print:	ld	e,a
	ld	c,2
	jp	BDOS
;
;	print B chars from HL 
;
Print_B_chars:
	ld	e,(hl)
	inc	hl	
	push	hl
	push	bc
	ld	c,2
	call	BDOS
	pop	bc
	pop	hl
	djnz	Print_B_chars
	ret
;
CrLf:   ld      e,CR
	ld	c,2
	call	BDOS
        ld      e,LF
	ld	c,2
	jp	BDOS
;
;	print (DE)=line
;
PrintLine:
	ld	c,9
	jp	BDOS
;
;   Routine to select disk (A).
;
DiskSel:ld      e,a
        ld      c,14
        jp      BDOS
;
;   Routine to call bdos and save the return code. The zero
; flag is set on a return of 0ffh.
;
BDOS1:  call    BDOS
        ld      (RtnCode),a     ;save return code.
        inc     a               ;set zero if 0ffh returned.
        ret
;
;   Routine to search for the first file with ambigueous name
; (de).
;
SearchFirst:
	ld      c,17
        jp      BDOS1
;
;   Search for the next ambigeous file name.
;
SearchNext:
	ld      c,18
        jp      BDOS1
;
;   Search for file at (FCB).
;
SearchFCB:
	ld      de,TFCB
        jp      SearchFirst
;
;   Routine to get the currently active drive number.
;
GetDisk:ld      c,25
        jp      BDOS
;
;   Routine to set the dma address to (de).
;
SetDMA: ld      c,26
        jp      BDOS
;
;   Routine to convert (A) into upper case ascii. Only letters
; are affected.
;
ToUpper:cp      'a'             ;check for letters in the range of 'a' to 'z'.
        ret     c
        cp      '{'
        ret     nc
        and     5FH             ;convert it if found.
        ret
;
;   Print back file name with a '?' to indicate a syntax error.
;
SyntaxErr: 
	call    CrLf            ;end current line.
        ld      hl,(NamePnt)    ;this points to name in error.
1:	ld      a,(hl)          ;print it until a space or null is found.
        cp      ' '
        jp      z,2f
        OR      A
        jp      z,2f
        push    hl
        call    Print
        pop     hl
        inc     hl
        jp      1b
2:	ld      a,'?'           ;add trailing '?'.
        call    Print
        call    CrLf
	jp	0		;reboot cp/M
;
;   Check character at (DE) for legal command input. 
;   Note that Z is set if the character is a delimiter (blank or zero or .)
;   < > , ; : = [ ] % | ( ) / \ are not allowed
;
Check:  ld      a,(de)
        or      a
        ret     z
        cp      ' '             ;control characters are not legal here.
        jp      c,SyntaxErr
        ret     z               ;check for valid delimiter.
        cp      '.'
        ret     z
	cp	'<'
	jp	z,SyntaxErr
	cp	'>'
	jp	z,SyntaxErr
	cp	','
	jp	z,SyntaxErr
	cp	';'
	jp	z,SyntaxErr
	cp	':'
	jp	z,SyntaxErr
	cp	'='
	jp	z,SyntaxErr
	cp	'['
	jp	z,SyntaxErr
	cp	']'
	jp	z,SyntaxErr
	cp	'%'
	jp	z,SyntaxErr
	cp	'|'
	jp	z,SyntaxErr
	cp	'('
	jp	z,SyntaxErr
	cp	')'
	jp	z,SyntaxErr
	cp	'/'
	jp	z,SyntaxErr
	cp	'\'
	jp	z,SyntaxErr
	ret
;
;   Get the next non-blank character from (de).
;
NonBlank:
	ld    	a,(de)
        or	a               ;string ends with a null.
        ret     z
        cp      ' '
        ret     nz
        inc     de
        jr      NonBlank
;
;   Add (HL)=(HL)+(A)
;
AddHL:  add     a,l
        ld      l,a
        ret     nc              ;take care of any carry.
        inc     h
        ret
;
;   Compute (HL)=(TBUFF)+(A)+(C) and get the byte that's here.
;
Extract:ld      hl,TBUFF
        add     a,c
        call    AddHL
        ld      a,(hl)
        ret
;
;	StringLen
;
;	hl = string pointer (string is ended with blank or zero)
;
;	returns B = string length
;	
StringLen:
	ld	b,0
chk:	ld	a,(hl)
	inc	hl
	or	a
	ret	z
	cp	' '
	ret	z
	inc	b
	jr	chk
;
;   Convert the first name in (FCB).
;
ConvFirst:
	xor	a
;
;   Format a file name (convert * to '?', etc.). 
;	On return, A=0 if an unambigueous name was specified. 
;
	ld      hl,TFCB
        call    AddHL
        push    hl		;push fcb 
        push    hl		;push fcb
        xor     a
        ld      (ChgDrv),a      ;initialize drive change flag.
        ld      hl,CommLine+1   ;set (hl) as pointer into input line.
        ex      de,hl
        call    NonBlank        ;get next non-blank character.
        ex      de,hl
        ld      (NamePnt),hl    ;save pointer here for any error message.
	call	StringLen	;check filename length
	ld	a,b
	cp	15
	jp	nc,SyntaxErr	;too big
	ld	hl,(NamePnt)
        ex      de,hl
        pop     hl		;pop fcb
        ld      a,(de)          ;get first character.
        or	a
        jp      z,1f
	call	ToUpper		;to upper case
	or	a
        sbc     a,'A'-1         ;might be a drive name, convert to binary.
        ld      b,a             ;and save.
        inc     de              ;check next character for a ':'.
        ld      a,(de)
        cp      ':'
        jp      z,2f
        dec     de              ;nope, move pointer back to the start of the line.
1:	ld      a,(CrtDrv)
        ld      (hl),a
        jp      3f
2:	ld      a,b
        ld      (ChgDrv),a      ;set change in drives flag.
        ld      (hl),b
        inc     de
;
;   Convert the basic file name.
;
3:	ld      b,08H
4:	call    Check
        jp      z,8f
        inc     hl
        cp      '*'             ;note that an '*' will fill the remaining
        jp      nz,5f	        ;field with '?'.
        ld      (hl),'?'
        jp      6f
5:	call	ToUpper		;to upper case
	ld      (hl),a
        inc     de
6:	dec     b
        jp      nz,4b
7:	call    Check           ;get next delimiter.
        jp      z,GetExt
        inc     de
        jp      7b
8:	inc     hl              ;blank fill the file name.
        ld      (hl),' '
        dec     b
        jp      nz,8b
;
;   Get the extension and convert it.
;
GetExt: ld      b,03H
        cp      '.'
        jp      nz,5f
        inc     de
1:	call    Check
        jp      z,5f
        inc     hl
        cp      '*'
        jp      nz,2f
        ld      (hl),'?'
        jp      3f
2:	call	ToUpper		;to upper case
	ld      (hl),a
        inc     de
3:	dec     b
        jp      nz,1b
4:	call    Check
        jp      z,6f
        inc     de
        jp      4b
5:	inc     hl
        ld      (hl),' '
        dec     b
        jp      nz,5b
6:	ld      b,3
7:	inc     hl
        ld      (hl),0
        dec     b
        jp      nz,7b
        pop     hl
;
;   Check to see if this is an ambigeous file name specification.
; Set the (A) register to non zero if it is.
;
        ld      bc,11           ;set name length.
8:	inc     hl
        ld      a,(hl)
        cp      '?'             ;any question marks?
        jp      nz,9f
        inc     b               ;count them.
9:	dec     c
        jp      nz,8b
        ld      a,b
        or      a
        ret
;
;  Check drive specified. If it means a change, then the new
; drive will be selected. In any case, the drive byte of the
; fcb will be set to null (means use current drive).
;
DriveSel:
	xor     a               ;null out first byte of fcb.
        ld      (TFCB),a
        ld      a,(ChgDrv)      ;a drive change indicated?
        or      a
        ret     z
        dec     a               ;yes, is it the same as the current drive?
        ld      hl,CrtDrv
        cp      (hl)
        ret     z
        jp      DiskSel          ;no. Select it then.
;
;	Compare 11 bytes at (DE) ? (HL)
;	BC,DE,HL not affected
;
Compare:
	push	bc
	push	de
	push	hl
	ld	b,11
1:	ld	a,(de)
	cp	(hl)
	jr	nz,2f
	inc	de
	inc	hl
	djnz	1b
2:	pop	hl
	pop	de
	pop	bc
	ret
;
;	Add 11 bytes from (HL) to sorted list at FileNames
;
Add:
	ld	bc,(FilesCnt)		;BC=records counter
	ld	de,FileNames
	ld	a,b			;if list empty...
	or	c
	jr	nz,loopc
	ld	bc,11			;store new element as first one
	ldir
	ld	(FilesPointer),de	;set pointer of list end
	ld	bc,1			;set counter = 1
	ld	(FilesCnt),bc
	ret
loopc:
	call	Compare			;(crt) ? (new)
	jr	nc,insert		;if > , insert-it here
					;if < ...
	dec	bc
	ld	a,b			;if crt record is the last one... 
	or	c
	jr	z,append		;then append-it to list
					;else skip to the next record
	ld	a,11			;DE = DE + 11
	add	a,e
	ld	e,a			;and continue compare loop
	jr	nc,loopc
	inc	d
	jr	loopc	
append:					;store new element to (FilesPointer)
	ld	de,(FilesPointer)
	ld	bc,11
	ldir
update:
	ld	(FilesPointer),de	;update pointer of list end
	ld	bc,(FilesCnt)		;increment counter
	inc	bc
	ld	(FilesCnt),bc
	ret
insert:					;insert new record from (HL) at (DE)
	push	de
	push	hl
					;move records to accomodate the new one
	xor	a			;compute how many bytes to move
	ld	hl,(FilesPointer)
	sbc	hl,de
	ld	b,h
	ld	c,l			;BC=how many bytes to move
	ld	de,(FilesPointer)
	dec	de
	ld	hl,11
	add	hl,de
	ex	de,hl			;DE=to, HL=from
	lddr				;move records, from end to begin
	pop	hl			;HL=from
	pop	de			;DE=to
	ld	bc,11
	ldir				;insert new record
	ld	de,(FilesPointer)	;update pointer & counter
	ld	hl,11
	add	hl,de
	ex	de,hl
	jr	update
;
	end	start
