;
;	Disassembled by Andrey Nikitin & Ladislau Szilagyi
;
*Title Disassembling CGEN.COM Hi-Tech C v3.09
*Heading Checking recreated code with original

; File - libc4.asm Created 09.03.2019 Last Modified 31.01.2021

;
; For easier obtaining an exact copy of the binary executable
; file, all library functions are located in files libc1.asm,
; libc2.asm, libc3.asm, libc4.asm, libc5.asm.
;
	global	ncsv, cret, indir
	global	csv
;=========================================================
; 7ECB amul:	imul.asm
; 7EDE mult8b:	
;=========================================================		
;	16 bit integer multiply

;	on entry, left operand is in hl, right operand in de

	psect	text

	global	amul,lmul
amul:
lmul:
	ld	a,e
	ld	c,d
	ex	de,hl
	ld	hl,0
	ld	b,8
	call	mult8b
	ex	de,hl
	jr	3f
2:	add	hl,hl
3:
	djnz	2b
	ex	de,hl
1:
	ld	a,c
mult8b:
	srl	a
	jr	nc,1f
	add	hl,de
1:	ex	de,hl
	add	hl,hl
	ex	de,hl
	ret	z
	djnz	mult8b
	ret

; End file libc4.asm

