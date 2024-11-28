;
;	Ladislau Szilagyi, sept 2024
;
;	Fixed point math
;
	psect text
;
;       Fixed point 8.8 format: 16 bitst

;       HIGH=int part
;       LOW=fract part
;
;	int neg(int fp)
;
_neg::
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)	
__neg:
    	xor 	a
    	sub 	e
    	ld 	e,a
    	sbc 	a,a
    	sub 	d
    	ld 	d,a
	ex	de,hl
	ret
;
;	int positive(int fp)
;
_positive::
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	bit	7,d
	jr	nz,__neg	;if negative, negate-it
	ex	de,hl		;else, keep-it unchanged
	ret
;
;       int fpmul = mul(int fp1, int fp2)
;
_mul::
        ld      hl,2
        add     hl,sp
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
        inc     hl
        ld      a,(hl)
        inc     hl
        ld      h,(hl)
        ld      l,a
                ;HL=DE*HL fixed point 8_8
fpmul::
        ld      a,h
        xor     d
        push    af
        xor     d
        jp      p,1f
        xor     a
        sub     l
        ld      l,a
        sbc     a,a
        sub     h
        ld      h,a
1:
        bit     7,d
        jr      z,2f
        xor     a
        sub     e
        ld      e,a
        sbc     a,a
        sub     d
        ld      d,a
2:
        ld      bc,0
        push    bc
        push    hl
        ld      hl,0
        call    mul16
        ld      b,h
        ld      h,l
        ld      l,d
        ld      a,b
        or      a
        jr      z,3f
        ld      hl,7FFFH
3:
        pop     af
        ret     p
        xor     a
        sub     l
        ld      l,a
        sbc     a,a
        sub     h
        ld      h,a
        ret

mul16:
        ex      de,hl
        ex      (sp),hl
        exx
        pop     de
        pop     bc
        exx
        pop     bc
        push    hl
        ld      hl,0
        exx
        ld      hl,0
        ld      a,c
        ld      c,b
        call    lmult8b
        ld      a,c
        call    lmult8b
        exx
        ld      a,c
        exx
        call    lmult8b
        exx
        ld      a,b
        exx
        call    lmult8b
        push    hl
        exx
        pop     de
        ret

lmult8b:ld      b,8
4:      srl     a
        jp      nc,5f
        add     hl,de
        exx
        adc     hl,de
        exx
5:      ex      de,hl
        add     hl,hl
        ex      de,hl
        exx
        ex      de,hl
        adc     hl,hl
        ex      de,hl
        exx
        djnz    4b
        ret
;
;       fpdiv = div(int pf1, int fp2)
;
_div::
        ld      hl,2
        add     hl,sp
        ld      c,(hl)
        inc     hl
        ld      b,(hl)
        inc     hl
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
                ;HL=BC/DE fixed point 8_8
fpdiv::
        ld      a,b
        xor     d
        push    af
        xor     d
        jp      p,1f
        xor     a
        sub     c
        ld      c,a
        sbc     a,a
        sub     b
        ld      b,a
1:
        ld      a,d
        or      d
        jp      m,2f
        xor     a
        sub     e
        ld      e,a
        sbc     a,a
        sub     d
        ld      d,a
2:
        or      e
        jr      z,div_overflow
        ld      h,0
        ld      a,b
        add     a,e
        ld      a,d
        adc     a,h
        jr      c,div_overflow
        ld      l,b
        ld      a,c
        call    div_sub
        ld      c,a
        ld      a,b
        call    div_sub
        ld      d,c
        ld      e,a
        pop     af
        jp      p,retdiv
        xor     a
        sub     e
        ld      e,a
        sbc     a,a
        sub     d
        ld      d,a
retdiv:
        ex      de,hl
        ret

div_overflow:
        ld      de,7FFFH
        pop     af
        jp      p,retdiv
        inc     de
        inc     e
        jp      retdiv

div_sub:
        ld      b,8
3:
        rla
        adc     hl,hl
        add     hl,de
        jr      c,$+4
        sbc     hl,de
        djnz    3b
        adc     a,a
        ret

BC_Times_DE:            ; unsigned BC*DE->BHLA
        ld a,b
        ld hl,0
        ld b,h
        add a,a
        jr nc,$+5
        ld h,d
        ld l,e

        REPT 7
        add hl,hl
        rla
        jr nc,$+4
        add hl,de
        adc a,b
        ENDM

        push hl
        ld h,b
        ld l,b
        ld b,a
        ld a,c
        ld c,h
        add a,a
        jr nc,$+5
        ld h,d
        ld l,e

        REPT 7
        add hl,hl
        rla
        jr nc,$+4
        add hl,de
        adc a,c
        ENDM

        pop de
        ld c,a
        ld a,l
        ld l,h
        ld h,c
        add hl,de
        ret nc
        inc b
        ret

;	int	fpsqrt(int)	
_fpsqrt::
	ld	hl,2
	add	hl,sp
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
;
;
;	HL=sqrt(BC)
;
fpsqrt::
	ld	a,b
	or	c
	jr	nz,go
	ld	hl,0
	ret
go:
	ld	e,0
	ld	a,b
	cp	100
	jr	nc,S10
	cp	81
	jr	nc,S9
	cp	64
	jr	nc,S8
	cp	49
	jr	nc,S7
	cp	36
	jr	nc,S6
	cp	25
	jr	nc,S5
	cp	16
	jr	nc,S4
	cp	9
	jr	nc,S3
	cp	4
	jr	nc,S2
	cp	1
	jr	nc,S1
	ld	de,1
	jr	loop
S10:	ld	d,0AH
	jr	loop
S9:	ld	d,09H
	jr	loop
S8:	ld	d,08H
	jr	loop
S7:	ld	d,07H
	jr	loop
S6:	ld	d,06H
	jr	loop
S5:	ld	d,05H
	jr	loop
S4:	ld	d,04H
	jr	loop
S3:	ld	d,03H
	jr	loop
S2:	ld	d,02H
	jr	loop
S1:	ld	d,01H
loop:
	push	de		;S on stack
	push	bc		;V on stack
	call	fpdiv		;HL = V / S
	pop	bc		;restore V
	pop	de		;DE = S
	add	hl,de		;HL = S + V/S
	srl	h
	rr	l		;HL = (S + V/S)/2
	xor	a		;CARRY=0
	push	hl		;HL on stack
	sbc	hl,de		;if S == (S + V/S)/2
	jr	z,retstack	;return-it
	pop	de		;else S = (S + V/S)/2
	jr	loop		;and keep looping
retstack:
	pop	hl		
	ret
;

halfPI          equ     192H
PI              equ     324H
PIplus_halfPI   equ     4B6H
twoPI           equ     648H

;
;       int = sin(int fp)
;
_sin::
        ld      hl,2
        add     hl,sp
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
                        ;CARRY=0
        ld      hl,halfPI-1     ;if angle < PI/2
        sbc     hl,de
        jr      nc,fpsin1       ;...compute sin(angle)

        xor     a       ;CARRY=0
        ld      hl,PI-1         ;if angle < PI
        sbc     hl,de
        inc     hl
        jr      nc,fpsin2       ;...compute sin(PI-angle)

        xor     a       ;CARRY=0
        ld      hl,PIplus_halfPI-1;if angle < PI+PI/2
        sbc     hl,de
        jr      nc,fpsin3       ;...compute -sin(angle-PI)

        xor     a       ;CARRY=0
        ld      hl,twoPI        ;...else compute -sin(2PI-angle)
        sbc     hl,de
        call    fpsin2
        jr      negate

fpsin3:                 ;CARRY=0
        ex      de,hl
        ld      de,PI
        sbc     hl,de
        call    fpsin2
negate:
        ex      de,hl
        xor     a
        ld      hl,0
        sbc     hl,de
        ret

fpsin2: ex      de,hl
                        ;       x < PI/2
                        ;       sin(x) : x-85x^3/512+x^5/128
fpsin1:
                        ;Inputs: DE , output: HL
        push    de
        sra     d
        rr      e       ;DE=x/2
        ld      b,d
        ld      c,e
        call    BC_Times_DE
                        ;HL=x^2/4
        push    hl
        sra     h
        rr      l       ;HL=x^2/8
        ex      de,hl
        ld      b,d
        ld      c,e
        call    BC_Times_DE
                        ;HL=x^4/64
        sra h
        rr l
        inc h
        ex (sp),hl      ;x^4/128+1 is on stack, HL=x^2/4
        xor a
        ld d,a
        ld b,h
        ld c,l
        add hl,hl
        rla
        add hl,hl
        rla
        add hl,bc
        adc a,d
        ld b,h
        ld c,l
        add hl,hl
        rla
        add hl,hl
        rla
        add hl,hl
        rla
        add hl,hl
        rla
        add hl,bc
        adc a,d
        ld e,l
        ld l,h
        ld h,a
        rl e
        adc hl,hl
        rl e
        jr nc,$+3
        inc hl
        pop de
        ex de,hl
        or a
        sbc hl,de
        ex de,hl
        pop bc
        jp BC_Times_DE
;
;       int = cos(int fp)
;
_cos::
        ld      hl,2
        add     hl,sp
        ld      e,(hl)
        inc     hl
        ld      d,(hl)
                        ;CARRY=0
        ld      hl,halfPI-1     ;if angle < PI/2
        sbc     hl,de
        jr      nc,fpcos1       ;...compute cos(angle)

        xor     a       ;CARRY=0
        ld      hl,PI-1         ;if angle < PI
        sbc     hl,de
        inc     hl
        jr      nc,fpcos2       ;...compute -cos(PI-angle)

        xor     a       ;CARRY=0
        ld      hl,PIplus_halfPI-1;if angle < PI+PI/2
        sbc     hl,de
        jr      nc,fpcos3       ;...compute -cos(angle-PI)

        xor     a       ;CARRY=0
        ld      hl,twoPI        ;...else compute cos(2PI-angle)
        sbc     hl,de
        ex      de,hl
        jr      fpcos1

fpcos3:                 ;CARRY=0
        ex      de,hl
        ld      de,PI
        sbc     hl,de

fpcos2: ex      de,hl
        call    fpcos1
        ex      de,hl
        xor     a
        ld      hl,0
        sbc     hl,de
        ret
                        ;       x < PI/2
                        ;       cos(x) : 1-x^2/2+5x^4/128
fpcos1:
                        ;Inputs: DE , output: HL
        ld      b,d
        ld      c,e
        call    BC_Times_DE
                        ;HL=x^2
        sra     h
        rr      l       ;HL=x^2/2
        push    hl      ; x^2/2 on stack

        ex      de,hl
        ld      b,d
        ld      c,e
        call    BC_Times_DE
                        ;HL=x^^4/4

        sra     h
        rr      l       ;HL=x^^4/8
        sra     h
        rr      l       ;HL=x^^4/16
        sra     h
        rr      l       ;HL=x^^4/32
        push    hl      ; x^^4/32 on stack

        sra     h
        rr      l       ;HL=x^^4/64
        sra     h
        rr      l       ;HL=x^^4/128

        pop     de
        add     hl,de   ;HL=x^4/32 + x^4/128 = 5x^^4/128

        pop     de
        sbc     hl,de   ;HL=5x^^4/128 - x^2/2

        ld      bc,100h
        add     hl,bc   ;HL=1 - x^2/2 + 5x^^4/128
        ret
;
;	int	arctan(x)
;
;	x >=0
;
;	if x on [0,1]
;		arctan(x) : x(9+2x^2)/(9+5x^2)
;	else (x > 1)
;		arctan(x) = halfPI - arctan(1/x)
;
_arctan::
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		;DE=x
				;x > 1 ?
	ld	a,d
	or	a
	jr	z,arctan_1
	dec	a
	jr	nz,arctan_2
	ld	a,e
	or	a
	jr	z,arctan_1

arctan_2:			;x > 1
	ld	bc,100H
	call	fpdiv		;HL=1/x
	ex	de,hl		;DE=1/x
	call	arctan_1	;HL=atan(1/x)
	ld	de,halfPI
	ex	de,hl
	or	a
	sbc	hl,de		;HL=PI/2 - atan(1/x)
	ret

arctan_1:			;x <= 1
	push	de		;save x for later
	ld	b,d
	ld	c,e
	call	BC_Times_DE 	;HL=x^2
	ld	d,h
	ld	e,l		;DE=HL=x^2
	add	hl,hl
	add	hl,hl
	add	hl,de   	;HL=5x^2,DE=x^2
	ld 	a,9
	add 	a,h
	ld 	h,a	  	;HL=9+5x^2, DE=x^2
	ex	de,hl		;DE=9+5x^2, HL=x^2
	ld 	a,9
	sla 	l
	rl 	h
	add 	a,h
	ld 	h,a	  	;HL=9+2x^2
	ld 	b,h
	ld 	c,l		;BC=9+2x^2
	call 	fpdiv   	;HL=(9+2x^2)/(9+5x^2)
	ex	de,hl	 	;DE=(9+2x^2)/(9+5x^2)
	pop 	bc		;BC=x
	jp 	BC_Times_DE

;	transform X/Y (X < Y) into fixed point
;
;int	xdivytofp(int x, int y);
;
_xdivytofp::
	ld	hl,2
	add	hl,sp
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
;
;	X = DE, Y = BC
;
;	returns HL
;
xdivytofp::
				;if X * 256 < Y, return 0
	ld	a,d
	or	a
	jr	nz,ok
	ld	h,e
	ld	l,a		;HL=DE*256
	ex	de,hl		;HL=X
	ld	d,h
	ld	e,l		;DE=X
	sbc	hl,bc
	jr	nc,ok
	ld	hl,0		;return 0
	ret
ok:				;it will return a non-zero decimal part
				;2*X >= Y ?
	add	hl,hl		;HL=2*X
	xor	a		;A=0, CARRY=0
	sbc 	hl,bc
	jr	c,1f
	or	80H		;A=A or 80H
				;X=2*X-Y
	sla	c
	rl	b		;Y=2*Y
	jr	2f
1:	ex	de,hl		;HL=X
	ld	d,h
	ld	e,l		;DE=X	
2:				;4*X > Y ?
	add	hl,hl
	add	hl,hl		;HL=4*X
	sbc 	hl,bc
	jr	c,1f
	or	40H		;A=A or 40H
				;X=4*X-Y
	sla	c
	rl	b
	sla	c
	rl	b		;Y=4*Y
	jr	2f
1:	ex	de,hl		;HL=X
	ld	d,h
	ld	e,l		;DE=X
2:				;8*X > Y ?
	add	hl,hl
	add	hl,hl
	add	hl,hl		;HL=8*X
	sbc 	hl,bc		
	jr	c,1f
	or	20H		;A=A or 20H 
				;X=8*X-Y
	sla	c
	rl	b
	sla	c
	rl	b
	sla	c
	rl	b		;Y=8*Y
	jr	2f
1:	ex	de,hl		;HL=X
	ld	d,h
	ld	e,l		;DE=X
2:				;16*X > Y ?
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl		;HL=16*X
	sbc 	hl,bc
	jr	c,1f
	or	10H		;A=A or 10H
				;X=16*X-Y
	sla	c
	rl	b		
	sla	c
	rl	b
	sla	c
	rl	b
	sla	c
	rl	b		;Y=16*Y
	jr	2f
1:
	ex	de,hl		;HL=X
	ld	d,h
	ld	e,l		;DE=X
2:				;32*X > Y ?
	add	hl,hl	
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl		;HL=32*X
	sbc 	hl,bc
	jr	c,1f
	or	8H		;A=A or 8H
				;X=32*X-Y
	sla	c
	rl	b		
	sla	c
	rl	b		
	sla	c
	rl	b
	sla	c
	rl	b
	sla	c
	rl	b		;Y=32*Y
	jr	2f
1:
	ex	de,hl		;HL=X
	ld	d,h
	ld	e,l		;DE=X
2:				;64*X > Y
	add	hl,hl	
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl		;HL=64*X
	sbc 	hl,bc
	jr	c,1f
	or	4H		;A=A or 4H
				;X=64*X-Y
	sla	c
	rl	b		
	sla	c
	rl	b		
	sla	c
	rl	b		
	sla	c
	rl	b
	sla	c
	rl	b
	sla	c
	rl	b		;Y=64*Y
	jr	2f
1:
	ex	de,hl		;HL=X
	ld	d,h
	ld	e,l		;DE=X
2:				;128*X > Y
	add	hl,hl	
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl
	add	hl,hl		;HL=128*X
	sbc 	hl,bc
	jr	c,1f
	or	2H		;A=A or 2H
				;X=128*X-Y
	sla	c
	rl	b		
	sla	c
	rl	b		
	sla	c
	rl	b		
	sla	c
	rl	b		
	sla	c
	rl	b
	sla	c
	rl	b
	sla	c
	rl	b		;Y=128*Y
	jr	2f
1:
	ex	de,hl		;HL=X
2:				;256*X > Y
	ld	h,l
	ld	l,0
	or	a
	sbc	hl,bc
	jr	c,1f
	or	1H		;A=A or 1H
1:	ld	h,0		;high part=0
	ld	l,a		;low part
	ret
;

	

