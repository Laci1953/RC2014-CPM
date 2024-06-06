;
;       Dynamic memory management functions 
;
;       Algorithm Copyright  
;
;(c) 2002, 2004, 2010 Joerg Wunsch,  
;(c) 2010  Gerben van den Broeke 
;
;       Z80 assembler implementation 
;
;       by Szilagyi Ladislau, June 2024 
;
	global	__Hbss

	psect	text

_get_flp::
	ld	hl,(flp)
	ret

_init_mem::
	ld	de,__Hbss	;free buffer addr
	ld	(flp),de
	ld	hl,(6)		;top stack
	ld	bc,512
	sbc	hl,bc		;HL=stack base
	sbc	hl,de		;HL=free buffer len
	ex	de,hl		;DE=free buffer len, HL=free buffer addr
	ld	(hl),e		;store free buffer size
	inc	hl
	ld	(hl),d
	inc	hl
	xor	a		;set nx=0
	ld	(hl),a
	inc	hl
	ld	(hl),a
	ret

;	short	total_free(void);
_total_free::
	ld      de,0 	
	ld      hl,(flp)	
loopt: 	
	ld      c,(hl)	
	inc     hl 	
	ld      b,(hl)	
	ex      de,hl 	
	add     hl,bc	
	ex      de,hl 	
	inc     hl 	
	ld      a,(hl)	
	inc     hl 	
	ld      h,(hl) 	
	ld      l,a 	
	or      h	
	jr      nz,loopt 	
	ex      de,hl 	
	ret		;HL=total free bytes

;	void*	mymalloc(short size)
_mymalloc::
	push	ix
	push	iy
	ld	hl,6
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	ld      a,b 	
	or      a 	
	jr      nz,1f 	
	ld      a,c 	
	cp      3 	
	jr      nc,1f 	
	ld      bc,2	
1: 	
	ld      de,0	
	push    de 	
	ld      hl,(flp) 	
loop1: 	
	ld      a,h 	
	or      l 	
	jp      z,endloop1 	
	inc     hl 	
	ld      a,(hl)	
	dec     hl 	
	cp      b 	
	jr      c,doif1 	
	jr      z,cplow 	
	jr      endif1 	
cplow:	ld      a,(hl) 	
	cp      c 	
	jr      nc,endif1 	
doif1: 	
	ld      d,h	
	ld      e,l 	
	inc     hl 	
	inc     hl 	
	ld      a,(hl) 	
	inc     hl 	
	ld      h,(hl) 	
	ld      l,a 	
	jr      loop1 	
endif1: 	
	ld      a,(hl) 	
	cp      c 	
	jr      nz,else2 	
	inc     hl 	
	ld      a,(hl) 	
	dec     hl 	
	cp      b 	
	jr      nz,else2 	
	ld      a,d 	
	or      e 	
	jr      z,else3 	
	inc     hl 	
	inc     hl 	
	inc     de 	
	inc     de 	
	push    bc 	
	ld      c,(hl) 	
	inc     hl 	
	ld      b,(hl) 	
	ex      de,hl 	
	ld      (hl),c 	
	inc     hl 	
	ld      (hl),b 	
	ex      de,hl 	
	pop     bc 	
	dec     hl	
	jr      endif3 	
else3: 	
	inc     hl 	
	inc     hl 	
	ld      a,(hl) 	
	ld      (flp),a 	
	inc     hl 	
	ld      a,(hl) 	
	ld      (flp+1),a 	
	dec     hl 	
endif3:	
	pop     de
	pop	iy
	pop	ix 	
	ret	
else2: 	
	ex      de,hl 	
	ex      (sp),hl 	
	ex      de,hl 	
	ld      a,d 	
	or      e	
	jr      z,doif4 	
	inc     hl 	
	ld      a,(hl) 	
	dec     hl 	
	cp      d 	
	jr      c,doif4 	
	jr      z,cplow2 	
	jr      endif4 	
cplow2:	ld      a,(hl) 	
	cp      e 	
	jr      nc,endif4 	
doif4: 	
	ld      e,(hl)	
	inc     hl 	
	ld      d,(hl)  	
	dec     hl 	
	ex      de,hl 	
	ex      (sp),hl 	
	ex      de,hl 	
	ex      de,hl 	
	ld      ixl,e 	
	ld      ixh,d	
	ex      de,hl 	
	ld      iyl,e 	
	ld      iyh,d 	
	jr      endif4_a 	
endif4:	
	ex      de,hl 	
	ex      (sp),hl 	
	ex      de,hl 	
endif4_a: 	
	ld      d,h 	
	ld      e,l 	
	inc     hl 	
	inc     hl 	
	ld      a,(hl) 	
	inc     hl 	
	ld      h,(hl) 	
	ld      l,a 	
	jp      loop1 	
endloop1:	
	ld      d,iyh 	
	ld      e,iyl 	
	pop     hl 	
	ld      a,h 	
	or      l 	
	jr      z,endif5 	
	sbc     hl,bc 	
	ld      a,h 	
	or      a 	
	jr      nz,endif6 	
	ld      a,l 	
	cp      4 	
	jr      nc,endif6 	
	add     hl,bc	
	ld      a,d 	
	or      e 	
	jr      z,else7 	
	ld      c,(ix+2) 	
	ld      b,(ix+3) 	
	ld      (iy+2),c 	
	ld      (iy+3),b 	
	jr      endif7 	
else7:	
	ld      a,(ix+2) 	
	ld      (flp),a 	
	ld      a,(ix+3) 	
	ld      (flp+1),a 	
endif7:	
	push    ix	
	pop     hl	
	inc     hl 	
	inc     hl 	
	pop	iy
	pop	ix 	
	ret 	
endif6: 	
	push    ix 	
	pop     iy 	
	ld      d,h 	
	ld      e,l 	
	add     iy,de 	
	ld      d,iyh 	
	ld      e,iyl	
	ld      a,c 	
	ld      (de),a 	
	inc     de 	
	ld      a,b 	
	ld      (de),a 	
	dec     hl 	
	dec     hl 	
	ld      (ix+0),l 	
	ld      (ix+1),h 	
	inc     de 	
	ex      de,hl 	
	pop	iy
	pop	ix 	
	ret 	
endif5: 	
	ld      hl,0	
	pop	iy
	pop	ix 	
	ret
;
;	myfree(void* pbuf)
_myfree::
	ld	hl,2
	add	hl,sp
	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a
	or      h 	
	ret     z 
	push	ix
	push	iy	
	dec     hl	
	dec     hl	
	push    hl 	
	pop     iy 	
	xor     a 	
	ld      (iy+2),a 	
	ld      (iy+3),a 	
	ld      ix,(flp)	
	ld      bc,0	
loop: 	
	ld      a,ixh 	
	or      ixl 	
	jp      z,endloop 	
	ld      a,ixh 	
	cp      iyh 	
	jr      c,doifa 	
	jr      z,cplow3 	
	jr      endifa 	
cplow3:	ld      a,ixl 	
	cp      iyl 	
	jr      nc,endifa 	
doifa: 	
	ld      b,ixh 	
	ld      c,ixl	
	ld      e,(ix+2) 	
	ld      d,(ix+3) 	
	ld      ixl,e 	
	ld      ixh,d 	
	jp      loop 	
endifa: 	
	ld      d,ixh 	
	ld      e,ixl 	
	ld      a,ixl 	
	ld      (iy+2),a 	
	ld      a,ixh 	
	ld      (iy+3),a 	
	push    hl 	
	push    bc 	
	ld      l,(iy+0) 	
	ld      h,(iy+1) 	
	ld      c,iyl 	
	ld      b,iyh 	
	inc     bc 	
	inc     bc 	
	add     hl,bc 	
	or      a 	
	sbc     hl,de 	
	jr      nz,endifb 	
	ld      c,(ix+0) 	
	ld      b,(ix+1) 	
	ld      l,(iy+0) 	
	ld      h,(iy+1) 	
	add     hl,bc 	
	inc     hl 	
	inc     hl 	
	ld      (iy+0),l 	
	ld      (iy+1),h 	
	ld      c,(ix+2) 	
	ld      b,(ix+3) 	
	ld      (iy+2),c 	
	ld      (iy+3),b 	
endifb: 	
	pop     bc 	
	pop     hl 	
	ld      a,b 	
	or      c 	
	jr      nz,endifc 	
	ld      (flp),iy 
	pop	iy
	pop	ix	
	ret 	
endifc:	
endloop: 	
	ld      ixl,c 	
	ld      ixh,b 	
	ld      a,iyl 	
	ld      (ix+2),a 	
	ld      a,iyh 	
	ld      (ix+3),a 	
	inc     bc 	
	inc     bc 	
	ld      e,(ix+0) 	
	ld      d,(ix+1) 	
	or      a 	
	sbc     hl,de 	
	sbc     hl,bc 	
	jr	z,1f 
	pop	iy
	pop	ix
	ret	
1:	
	ld      l,(ix+0) 	
	ld      h,(ix+1) 	
	ld      c,(iy+0) 	
	ld      b,(iy+1) 	
	add     hl,bc 	
	inc     hl 	
	inc     hl 	
	ld      (ix+0),l 	
	ld      (ix+1),h 	
	ld      c,(iy+2) 	
	ld      b,(iy+3) 	
	ld      (ix+2),c 	
	ld      (ix+3),b 	
	pop	iy
	pop	ix
	ret

;
;	offsets for stack buffer
;
off_ptr          equ     0 	
off_fp3          equ     2 	
off_ofp3         equ     4 	
off_fp2          equ     6 	
off_incr         equ     8 	

;	void*	myrealloc(void* pbuf, short size)
_myrealloc::
	push	ix
	push	iy
	ld	hl,6
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)	;DE=pbuf
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)	;BC=size
	ex	de,hl	;HL=pbuf
	ld      a,b 	
	or      a 	
	jr      nz,2f 	
	ld      a,c 	
	cp      2 	
	jr      nc,2f 	
	ld      c,2 	
2: 	
	ld      a,h 	
	or      l 	
	jp      nz,1f 	
	push	bc
	call	_mymalloc
	pop	bc
	pop	iy
	pop	ix
	ret 		;ret HL
1: 	
	ld      d,h 	
	ld      e,l 	
	dec     de 	
	dec     de 	
	ld      ixl,e 	
	ld      ixh,d 	
	ld      a,b 	
	cp      (ix+1) 	
	jp      z,cplowf 	
	jp      nc,endiff 	
doiff: 	
	ld      a,(ix+1) 	
	or      a 	
	jr      nz,seelen 	
	ld      a,(ix+0) 	
	cp      5 	
	jr	nc,seelen
	pop	iy
	pop	ix
	ret	
seelen: 	
	inc     bc 	
	inc     bc 	
	inc     bc 	
	inc     bc 	
	ld      a,b 	
	cp      (ix+1) 	
	jr      z,cplowg 	
	jr      c,endifg	
	ret 	
cplowg: 	
	ld      a,c 	
	cp      (ix+0) 	
	jr      c,endifg 	
	jr	z,endifg
	pop	iy
	pop	ix
	ret
endifg: 	
	dec     bc 	
	dec     bc 	
	dec     bc 	
	dec     bc 	
	push    hl 	
	pop     iy 	
	add     iy,bc 	
	push    hl 	
	ld      h,b 	
	ld      l,c 	
	inc     hl 	
	inc     hl 	
	ld      e,(ix+0) 	
	ld      d,(ix+1) 	
	ex      de,hl 	
	or      a 	
	sbc     hl,de 	
	ex      de,hl 	
	ld      (iy+0),e 	
	ld      (iy+1),d 	
	ld      (ix+0),c 	
	ld      (ix+1),b 	
	ld      d,iyh 	
	ld      e,iyl 	
	inc     de 	
	inc     de  	
	push	de	
	call	_myfree
	pop	de
	pop     hl 	
	pop	iy
	pop	ix
	ret 	
cplowf: 	
	ld      a,c 	
	cp      (ix+0) 	
	jp      c,doiff 	
	jp      z,doiff 	
endiff:	
	push    hl 	
	ld      h,b 	
	ld      l,c	
	ld      e,(ix+0) 	
	ld      d,(ix+1) 	
	or      a 	
	sbc     hl,de 	
	ex      (sp),hl 	
	push    hl 	
	add     hl,de	
	pop     de  	
	ex      de,hl 	
	push    de 	
	ld      de,0 	
	push    de 	
	ld      de,(flp)	
	push    de	
	push    hl	
	ld      iy,0 	
	add     iy,sp 	
loop3: 	
	ld      e,(iy+off_fp3+0) 	
	ld      d,(iy+off_fp3+1) 	
	ld      a,d 	
	or      e 	
	jp      z,endloop3 	
	ld      a,d 	
	cp      (iy+off_fp2+1) 	
	jp      nz,endifh 	
	ld      a,e 	
	cp      (iy+off_fp2+0) 	
	jp      nz,endifh 	
	ex      de,hl 	
	ld      a,(hl) 	
	inc     hl 	
	ld      h,(hl) 	
	ld      l,a	
	inc     hl 	
	inc     hl	
	ld      a,h 	
	cp      (iy+off_incr+1) 	
	jp      c,endifh 	
	jp      z,cplowh 	
doifh: 	
	dec     hl 	
	dec     hl 	
	dec     hl 	
	dec     hl 	
	ld      a,h	
	cp      (iy+off_incr+1) 	
	jr      c,elsei 	
	jr      z,cplowi 	
doifi: 	
	pop     hl 	
	push    hl 	
	add     hl,bc 	
	ld      (iy+off_fp2+0),l 	
	ld      (iy+off_fp2+1),h 	
	ld      l,(iy+off_fp3+0) 	
	ld      h,(iy+off_fp3+1) 	
	ld      e,(iy+off_fp2+0) 	
	ld      d,(iy+off_fp2+1) 	
	inc     hl 	
	inc     hl 	
	inc     de 	
	inc     de 	
	push    bc 	
	ld      c,(hl) 	
	inc     hl 	
	ld      b,(hl) 	
	ex      de,hl 	
	ld      (hl),c 	
	inc     hl 	
	ld      (hl),b 	
	pop     bc 	
	ld      l,(iy+off_fp3+0) 	
	ld      h,(iy+off_fp3+1)	
	ld      a,(hl) 	
	inc     hl 	
	ld      h,(hl) 	
	ld      l,a 	
	ld      e,(iy+off_incr+0) 	
	ld      d,(iy+off_incr+1) 	
	or      a 	
	sbc     hl,de 	
	ex      de,hl 	
	ld      l,(iy+off_fp2+0) 	
	ld      h,(iy+off_fp2+1) 	
	ld      (hl),e 	
	inc     hl 	
	ld      (hl),d 	
	ld      (ix+0),c 	
	ld      (ix+1),b 	
	jr      endifi 	
cplowi: 	
	ld      a,l 	
	cp      (iy+off_incr+0) 	
	jr      c,elsei 	
	jr      nz,doifi 	
elsei:	
	ld      e,(ix+0) 	
	ld      d,(ix+1) 	
	ld      l,(iy+off_fp3+0) 	
	ld      h,(iy+off_fp3+1) 	
	ld      a,(hl) 	
	inc     hl 	
	ld      h,(hl) 	
	ld      l,a 	
	add     hl,de 	
	inc     hl 	
	inc     hl	
	ld      (ix+0),l 	
	ld      (ix+1),h 	
	ld      l,(iy+off_fp3+0) 	
	ld      h,(iy+off_fp3+1)	
	inc     hl 	
	inc     hl 	
	ld      a,(hl) 	
	inc     hl 	
	ld      h,(hl) 	
	ld      l,a 	
	ld      (iy+off_fp2+0),l 	
	ld      (iy+off_fp2+1),h 	
endifi: 	
	ld      l,(iy+off_ofp3+0) 	
	ld      h,(iy+off_ofp3+1) 	
	ld      a,h 	
	or      l 	
	jr      z,elsej 	
	ld      c,(iy+off_fp2+0) 	
	ld      b,(iy+off_fp2+1) 	
	inc     hl 	
	inc     hl 	
	ld      (hl),c 	
	inc     hl 	
	ld      (hl),b 	
	jr      endifj 	
elsej: 	
	ld      l,(iy+off_fp2+0) 	
	ld      h,(iy+off_fp2+1) 	
	ld      (flp),hl 	
endifj:		
	pop     de 	
	ld      hl,8 	
	add     hl,sp	
	ld      sp,hl 	
	ex      de,hl 	
	pop	iy
	pop	ix
	ret 	
cplowh: 	
	ld      a,l	
	cp      (iy+off_incr+0) 	
	jp      nc,doifh 	
endifh: 	
	ld      a,(iy+off_fp3+0) 	
	ld      (iy+off_ofp3+0),a 	
	ld      a,(iy+off_fp3+1) 	
	ld      (iy+off_ofp3+1),a 	
	ld      e,(iy+off_fp3+0) 	
	ld      d,(iy+off_fp3+1)	
	inc     de 	
	inc     de 	
	ld      a,(de) 	
	ld      (iy+off_fp3+0),a 	
	inc     de 	
	ld      a,(de) 	
	ld      (iy+off_fp3+1),a 	
	jp      loop3 	
endloop3: 	
	pop     de 	
	ld      hl,8 	
	add     hl,sp 	
	ld      sp,hl 	
	push    ix 	
	push    de 	
	push	bc
	call	_mymalloc
	pop	bc
	ld      a,h 	
	or      l 	
	jr      nz,copy 	
	pop     bc 	
	pop     bc 	
	pop	iy
	pop	ix
	ret 	
copy: 	
	pop     de	
	pop     ix	
	ld      c,(ix+0) 	
	ld      b,(ix+1) 	
	ex      de,hl 	
	push    de 	
	push    hl	;to be freed	
	ldir 	
	call	_myfree
	pop	hl
	pop     hl	;HL=new pbuf	
	pop	iy
	pop	ix
	ret 	
;
;       Free blocks record structure 
;
;       sz (2 bytes) 
;       nx (2 bytes) 
;
flp: 	defs	2	;pointer to free block

	end
