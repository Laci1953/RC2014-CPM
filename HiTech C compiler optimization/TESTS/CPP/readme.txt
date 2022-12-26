CPP.C

D>c -v -c -o cpp.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE
0:CPP -DCPM -DHI_TECH_C -Dz80 -I CPP.C $CTMP1.$$$
0:P1 $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
CPP.C:
  2194: unused variable definition: sccsid (warning)
0:CGEN $CTMP2.$$$ $CTMP1.$$$
0:OPTIM $CTMP1.$$$ $CTMP2.$$$
0:ZAS -J -N -oCPP.OBJ $CTMP2.$$$
Z80AS Macro-Assembler V4.7

Errors: 0

Jump optimizations done: 128
Finished.
ERA $CTMP1.$$$
ERA $CTMP2.$$$
ERA $CTMP3.$$$
ERA $$EXEC.$$$

D> 

(with statistics)

D>CPP -DCPM -DHI_TECH_C -Dz80 -I CPP.C $CTMP1.$$$

D>p1 -n $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
CPP.C:
  2194: unused variable definition: sccsid (warning)
16 KB used from the upper 64KB RAM

D>cgen -n $CTMP2.$$$ $CTMP1.$$$
8 KB used from the upper 64KB RAM

D>optim -n $CTMP1.$$$ $CTMP2.$$$
22K, 3 iterations
123 Redundant labels
173 Jumps to jumps
171 Stack adjustments
107 Temporary labels
388 Unref'ed labels
620 Unreachable code
43 Jumps to .+1
48 Skips over jumps
59 Common code seq's
34 Ex (sp),hl's used
6 Redundant operations
280 Redundant loads/stores
25 Simplified addresses
1 Xor a's used
3 Redundant ex de,hl's
23 Code motions
51 KB used from the upper 64KB RAM

CPY.C

D>c -v -c -o cpy.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE
0:CPP -DCPM -DHI_TECH_C -Dz80 -I CPY.C $CTMP1.$$$
0:P1 $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
CPY.C: yyparse()
   414: }
        ^ unused label: yyerrlab (warning)
CPY.C:
   415: unused variable definition: sccsid (warning)
0:CGEN $CTMP2.$$$ $CTMP1.$$$
0:OPTIM $CTMP1.$$$ $CTMP2.$$$
0:ZAS -J -N -oCPY.OBJ $CTMP2.$$$
Z80AS Macro-Assembler V4.7

Errors: 0

Jump optimizations done: 88
Finished.
ERA $CTMP1.$$$
ERA $CTMP2.$$$
ERA $CTMP3.$$$
ERA $$EXEC.$$$

D>

(with statistics)

D>CPP -DCPM -DHI_TECH_C -Dz80 -I CPY.C $CTMP1.$$$

D>p1 -n $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
CPY.C: yyparse()
   414: }
        ^ unused label: yyerrlab (warning)
CPY.C:
   415: unused variable definition: sccsid (warning)
4 KB used from the upper 64KB RAM

D>cgen -n $CTMP2.$$$ $CTMP1.$$$
2 KB used from the upper 64KB RAM

D>optim -n $CTMP1.$$$ $CTMP2.$$$
15K, 3 iterations
105 Redundant labels
112 Jumps to jumps
19 Stack adjustments
60 Temporary labels
129 Unref'ed labels
102 Unreachable code
14 Jumps to .+1
11 Skips over jumps
58 Common code seq's
2 Redundant operations
42 Redundant loads/stores
2 Simplified addresses
1 Xor a's used
14 Code motions
13 KB used from the upper 64KB RAM

D>
