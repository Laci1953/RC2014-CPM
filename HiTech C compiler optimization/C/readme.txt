The C compiler with an extra switch -% providing memory use statistics for P1, CGEN and OPTIM

Example: (CNEW.COM is executed...)

D>c -v -c -o -% te.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE
0:CPP -DCPM -DHI_TECH_C -Dz80 -I TE.C $CTMP1.$$$
0:P1 -N $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
13 KB used from the upper 64KB RAM
0:CGEN -N $CTMP2.$$$ $CTMP1.$$$
6 KB used from the upper 64KB RAM
0:OPTIM -N $CTMP1.$$$ $CTMP2.$$$
5K, 3 iterations
36 Redundant labels
33 Jumps to jumps
113 Stack adjustments
25 Temporary labels
84 Unref'ed labels
24 Unreachable code
2 Jumps to .+1
7 Skips over jumps
12 Common code seq's
4 Ex (sp),hl's used
1 Redundant operations
90 Redundant loads/stores
15 Simplified addresses
2 Xor a's used
2 Code motions
14 KB used from the upper 64KB RAM
0:ZAS -J -N -oTE.OBJ $CTMP2.$$$
Z80AS Macro-Assembler V4.7

Errors: 0

Jump optimizations done: 68
Finished.
ERA $CTMP1.$$$
ERA $CTMP2.$$$
ERA $CTMP3.$$$
ERA $$EXEC.$$$

