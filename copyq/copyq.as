;
;	copy with query for CP/M
;
;	Ladislau Szilagyi
;
;	May 2023
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
;	chars (will be zero terminated...)
;
CommLine	equ	80H

	psect	bss

FileNames:	defs	11 * MAX_FILES_CNT	;store matching file names
FilesCnt:	defs	2
FilesPointer:	defs	2

FileBuf:	defs	128

	psect	data

RtnCode:defb      0       	;status returned from bdos call.
ChgDrv: defb      0       	;change in drives flag (0=no change).
CrtDrv: defb      0       	;currently active drive.
;
NamePnt:defw      0       	;input line pointer used for error message. 
				;Points to start of name in error.

NoFile: defb	CR,LF
	defm    "No file name match!"
	defb	0
TooManyFiles:
	defb	CR,LF
	defm	"Too many files match!"
	defb	0
Query:	defb	CR,LF
	defm	"Copy file "
	defb	0
Qmark:	defm	" ? (Y/y = yes) :"
	defb	0
Overwrite:
	defb	CR,LF
	defm	"Overwrite destination file"
	defb	0
WrongSource:
	defb	CR,LF
	defm	"Wrong source disk!"
	defb	0
WrongDest:
	defb	CR,LF
	defm	"Wrong or missing destination disk!"
	defb	0
DiskFull:defb	CR,LF
	defm	"Destination disk (directory) full!"
	defb	0
SameSDdisk:defb	CR,LF
	defm	"Destination disk identical to source disk!"
	defb	0
CopyQHelp:
	defb	CR,LF
	defm	"Use: copyq filename.ext d:"
	defb	CR,LF
	defm	"copy files to destination disk (d:), with a query for each file"
	defb	CR,LF
	defm	"( and an extra query to allow overwriting files )"
	defb	CR,LF
	defm	"( ambiguous file references may be used, e.g. *.c or test?.asm )"
	defb	CR,LF
	defm	"( up to 512 files can be copied )"
	defb	0
CrLf:	defb	CR,LF
	defb	0
;
				;source file
sfcb:				; fcb
	defb	0		; disk+1
sname:	defb	0,0,0,0,0,0,0,0,0,0,0 ; file name
sdfcbz:	defb	0		; EX=0
	defb	0,0		; S1,S2
	defb	0		; RC=0
	defb	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	; D0,...D15
	defb	0		; CR=0
	defb	0,0,0		; R0,R1,R2
sfcb_end:

				;dest file
dfcb:				; fcb
	defb	0		; disk+1
dname:	defb	0,0,0,0,0,0,0,0,0,0,0 ; file name
ddfcbz:	defb	0		; EX=0
	defb	0,0		; S1,S2
	defb	0		; RC=0
	defb	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	; D0,...D15
	defb	0		; CR=0
	defb	0,0,0		; R0,R1,R2
dfcb_end:

	psect	text
;
;	copyq s:file.ext d:  
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
	jp	z,Help
	inc	hl
	call	AddHL		;HL=HL+A
	ld	(hl),0		;place a zero at the end of command line
	call    ConvFirst       ;convert file name.
        call    DriveSel        ;select indicated drive.
				;get destination drive
	ld	a,(de)		;should be a blank
	or	a
	jp	z,WrongDestErr
	cp	' '
	jp	nz,WrongDestErr
	inc	de
	ld	a,(de)		;should be a drive letter
	cp	'A'
	jp	c,WrongDestErr
	cp	'Z'+1
	jp	nc,WrongDestErr
	sub	'A'
	inc	a
	ld	(dfcb),a	;set dest drive
	inc	de
	ld	a,(de)		;should be ':'
	cp	':'
	jp	nz,WrongDestErr
				;...ignore rest of the command line...
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
	ld	hl,FileNames
	ld	(FilesPointer),hl	
loop:
	ld	de,(FilesPointer)
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
				;CARRY=0
	ld	(FilesPointer),de;update files pointer
	ld	hl,(FilesCnt)	;increment files counter
	inc	hl
	ld	(FilesCnt),hl
	ld	bc,MAX_FILES_CNT+1
	sbc	hl,bc
	jp	z,Toomany
skip:   call    SearchNext      ;get next file name.
	jp	z,CopyQ		;if no more, go copy with query
        jp      loop            ;else continue with our list.
;
;	Copy with query
;
CopyQ:
	ld	hl,FileNames	;init files pointer
	ld	(FilesPointer),hl
	call	GetDisk		;store current disk in fcb
	inc	a
	ld	(sfcb),a
	ld	hl,dfcb		;compare source ? dest
	cp	(hl)
	jp	z,SameSDdiskErr	;error if equal		
sloop:
	ld	hl,(FilesPointer);fetch next file name

	ld	de,sname	;and move-it to source fcb
	ld	bc,11
	ldir

	ld	(FilesPointer),hl;update files pointer
				
	ld	bc,Query	;print query msg
	call	PrintLine
				;type the file name & ext
	ld	hl,sname	;first the name
	ld	b,8
	call	PrintToSpace
	ld	a,'.'		;.
	call	Print
	ld	hl,sname+8	;then the ext
	ld	b,3
	call	PrintToSpace

	ld	bc,Qmark
	call	PrintLine
				;read answer
	call	GetChar
	cp	'Y'
	jr	z,DoCopy
	cp	'y'
	jr	nz,continue	;if not YES, skip this file

DoCopy:
	ld	bc,11		;copy file name also to dest fcb
	ld	hl,sname
	ld	de,dname
	ldir
				;clean fcb's
	ld	hl,sdfcbz
	call	CleanFCB
	ld	hl,ddfcbz
	call	CleanFCB
				;check if dest file already exists
	ld	de,dfcb		;open file
	call	Open
	push	af

	ld	de,dfcb
	call	Close

	pop	af
	cp	0FFH
	jr	z,doit
				;file already exists, ask to overwrite
	ld	bc,Overwrite
	call	PrintLine
	ld	bc,Qmark
	call	PrintLine
				;read answer
	call	GetChar
	cp	'Y'
	jr	z,doit
	cp	'y'
	jr	nz,continue	;if not YES, skip this file

doit:				;copy source to dest
	ld	de,dfcb
	call	Delete		;just in case...

	ld	de,sfcb
	call	Open

	ld	de,dfcb
	call	Make
	cp	0FFH
	jp	z,DiskFullErr

	ld	de,FileBuf
	call	SetDMA
				;copy recs from source to dest file
copyloop:
	ld	de,sfcb
	call	ReadRec
	jr	nz,closefiles

	ld	de,dfcb
	call	WriteRec
	jp	nz,DiskFullErr

	jr	copyloop

closefiles:
	ld	de,sfcb
	call	Close
	ld	de,dfcb
	call	Close

continue:			;decrement files counter
	ld	hl,(FilesCnt)
	dec	hl
	ld	(FilesCnt),hl
	ld	a,l
	or	h
	jp	nz,sloop
	jp	0		;finis, reboot
;
;	print B chars from HL but stops at a blank
;
PrintToSpace:
	ld	a,(hl)
	cp	' '
	ret	z
	inc	hl	
	push	hl
	push	bc
	call	Print
	pop	bc
	pop	hl
	djnz	PrintToSpace
	ret
;
;   Routine to print character string pointed to be (bc) on the
; console. It must terminate with a null byte.
;
PrintLine:
	push    bc
        pop     hl
1: 	ld      a,(hl)
        or      a
        ret     z
        inc     hl
        push    hl
        call    Print
        pop     hl
        jr      1b
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
		                ;yes, is it the same as the current drive?
        ld      hl,CrtDrv
        cp      (hl)
        ret     z
        jp      DiskSel          ;no. Select it then.
;
;	Prepare FCB
;	HL=xdfcbz (x=s or d)
;
CleanFCB:
	xor	a
	ld	b,sfcb_end - sdfcbz
1:	ld	(hl),a
	inc	hl
	djnz	1b
	ret
;
;   Routine to call bdos and save the return code. The zero
; flag is set on a return of 0ffh.
;
BDOS1:  call    BDOS
        ld      (RtnCode),a     ;save return code.
        inc     a               ;set zero if 0ffh returned.
        ret
;
;   Routine to call the bdos and set the zero flag if a zero
; status is returned.
;
BDOS2: 	call    BDOS
        OR      A               ;set zero flag if appropriate.
        ret
;
;   Get char from console
;	returns A = char
;
GetChar:ld	c,1
	jp	BDOS
;
;   Routine to print (A) on the console. All registers used.
;
Print:  ld      e,a             ;setup bdos call.
        ld      c,2
        jp      BDOS
;
;   Routine to select disk (A).
;
DiskSel:push	de		;save DE
	ld      e,a
        ld      c,14
        call    BDOS
	pop	de		;restore DE
	ret
;
;   Routine to open a file. (de) must point to the FCB.
;
Open:   ld      c,15
        jp      BDOS
;
;   Routine to close a file. (de) points to FCB.
;
Close:  ld      c,16
        jp      BDOS
;
;	Delete a file. (de) points to FCB. 
;
Delete:	ld	c,19
	jp	BDOS
;
;	Create a file. (de) points to FCB.
;
Make:	ld	c,22
	jp	BDOS
;
;   Routine to search for the first file with ambigueous name
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
;   Search for file at (TFCB).
;
SearchFCB:
	ld      de,TFCB
        jp      SearchFirst
;
;   Routine to read the next record from a sequential file.
; (de) points to the FCB.
;
ReadRec:ld      c,20
        jp      BDOS2
;
;   Routine to write the next record to a sequential file.
; (de) points to the FCB.
;
WriteRec:ld      c,21
        jp      BDOS2
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
;   Print back file name with a '?' to indicate a syntax error.
;
SyntaxErr: 
	ld	bc,CrLf
	call	PrintLine
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
	jp	0		;reboot CP/M
;
WrongSourceErr:
	ld	bc,WrongSource
	jr	prq
;
WrongDestErr:
	ld	bc,WrongDest
	jr	prq
;
DiskFullErr:
	ld	bc,DiskFull
	jr	prq
;
SameSDdiskErr:
	ld	bc,SameSDdisk
	jr	prq
;
;   Required file(s) not located.
;
None:   ld      bc,NoFile
	jr	prq
;
;	Too many files
;
Toomany:ld      bc,TooManyFiles
	jr	prq
;
;	Display help
;
Help:	ld	bc,CopyQHelp
prq:	call	PrintLine
	jp	0
;
;   Check character at (DE) for legal command input. 
;   Note that Z is set if the character is a delimiter (blank or zero or .)
;   < > , ; : = [ ] % | ( ) / \ are not allowed
;
Check:  ld      a,(de)
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
        or      a
        jp      z,1f
	ld	b,a		;save-it to B
        inc     de              ;check next character for a ':'.
        ld      a,(de)
        cp      ':'
        jp      z,2f
        dec     de              ;nope, move pointer back to the start of the line.
1:	ld      a,(CrtDrv)
        ld      (hl),a
        jp      3f
2:	ld      a,b
	cp	'A'
	jp	c,WrongSourceErr
	cp	'Z'+1
	jp	nc,WrongSourceErr
        sbc     a,'A'-1         ;might be a drive name, convert to binary.
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
5:	
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
2:	
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
	end	start
