;
;	erase with querry for CP/M
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
;	chars (zero terminated)
;
CommLine	equ	80H

	psect	bss

FileNames:	defs	11 * MAX_FILES_CNT	;store matching file names
FilesCnt:	defs	2
FilesPointer:	defs	2

	psect	data

RtnCode:defb      0       	;status returned from bdos call.
ChgDrv: defb      0       	;change in drives flag (0=no change).
CrtDrv: defb      0       	;currently active drive.
;
NamePnt:defw      0       	;input line pointer used for error message. 
				;Points to start of name in error.

NoFile: defm    "No file name match!"
	defb	0
TooManyFiles:
	defm	"Too many files match!"
	defb	0
Querry:	defm	"Erase file "
	defb	0
Qmark:	defm	" ? (Y/y = yes) :"
	defb	0
EraqHelp:
	defm	"Use: eraq filename.ext"
	defb	CR,LF
	defm	"erase the requested files, with a querry for each matching file"
	defb	CR,LF
	defm	"( ambiguous file references may be used, e.g. *.c or test?.asm )"
	defb	CR,LF
	defm	"( up to 512 files can be erased )"
	defb	0
fcb:				; fcb
	defb	0		; disk+1
name:	defb	0,0,0,0,0,0,0,0,0,0,0 ; file name
dfcbz:	defb	0		; EX=0
	defb	0,0		; S1,S2
	defb	0		; RC=0
	defb	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	; D0,...D15
	defb	0		; CR=0
	defb	0,0,0		; R0,R1,R2
fcb_end:

	psect	text
;
;	eraq [file.ext] 
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
	jp	z,Erase		;if no more, go erase
        jp      loop            ;else continue with our list.
;
;   Required file(s) not located.
;
None:   ld      bc,NoFile
        call    PrintLine
	jp	0		;reboot
;
;	Too many files
;
Toomany:ld      bc,TooManyFiles
        call    PrintLine
	jp	0		;reboot
;
;	Display help
;
Help:	ld	bc,EraqHelp
	call	PrintLine
	jp	0
;
;	Erase with querry
;
Erase:
	ld	hl,FileNames	;init files pointer
	ld	(FilesPointer),hl
	call	GetDisk		;store current disk in fcb
	inc	a
	ld	(fcb),a		
sloop:
	ld	hl,(FilesPointer);fetch next file name
	ld	de,name
	ld	bc,11
	ldir
	ld	(FilesPointer),hl;update files pointer
	call	CleanFCB	;prepare fcb
				;print querry msg
	call	CrLf
	ld	bc,Querry
	call	PrintLine
				;type the file name & ext
	ld	hl,name		;first the name
	ld	b,8
	call	PrintToSpace
	ld	a,'.'		;.
	call	Print
	ld	hl,name+8	;then the ext
	ld	b,3
	call	PrintToSpace
				;ask
	ld	bc,Qmark
	call	PrintLine
				;read answer
	call	GetChar
	cp	'Y'
	jr	z,EraseIt
	cp	'y'
	jr	nz,continue
EraseIt:
	ld	de,fcb		;delete file
	ld	c,19
	call	BDOS
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
;   Routine to print (A) on the console. All registers used.
;
Print:  ld      e,a             ;setup bdos call.
        ld      c,2
        jp      BDOS
;
;   Routine to print (A) on the console and to save (bc).
;
PrintB: push    bc
        call    Print
        pop     bc
        ret
;
;   Routine to send a carriage return, line feed combination
; to the console.
;
CrLf:   ld      a,CR
        call    PrintB
        ld      a,LF
        jp      PrintB
;
;   Routine to print character string pointed to be (bc) on the
; console. It must terminate with a null byte.
;
PrintLine:
	push    bc
;       call    CrLf
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
;	Prepare FCB
;
CleanFCB:
	ld	hl,dfcbz
	xor	a
	ld	b,fcb_end - dfcbz
clloop:	ld	(hl),a
	inc	hl
	djnz	clloop
	ret
;
;   Get char from console
;	returns A = char
;
GetChar:ld	c,1
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
        xor     A
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
        OR      A
        jp      z,1f
	call	ToUpper		;to upper case
        Sbc     a,'A'-1         ;might be a drive name, convert to binary.
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
	end	start
