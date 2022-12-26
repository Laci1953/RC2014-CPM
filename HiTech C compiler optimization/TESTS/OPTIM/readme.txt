D>sdir optim?.c

Directory For Drive D:  User  0

    Name     Bytes   Recs   Attributes      Name     Bytes   Recs   Attributes
------------ ------ ------ ------------ ------------ ------ ------ ------------
OPTIMA   C     128k   1007 Dir RW       OPTIMB   C      68k    529 Dir RW
OPTIMC   C      48k    363 Dir RW

Total Bytes     =    244k  Total Records =    1899  Files Found =    3
Total 1k Blocks =    239   Used/Max Dir Entries For Drive D:  484/ 512

D>c -v -c -o optima.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE

D>CPP -DCPM -DHI_TECH_C -Dz80 -I OPTIMA.C $CTMP1.$$$

D>p1 -n $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
20 KB used from the upper 64KB RAM

D>cgen -n $CTMP2.$$$ $CTMP1.$$$
9 KB used from the upper 64KB RAM

D>optim -n $CTMP1.$$$ optima.as
10K, 3 iterations
105 Redundant labels
132 Jumps to jumps
122 Stack adjustments
312 Temporary labels
240 Unref'ed labels
78 Unreachable code
11 Jumps to .+1
58 Skips over jumps
33 Common code seq's
8 Ex (sp),hl's used
59 Redundant operations
164 Redundant loads/stores
37 Simplified addresses
4 Xor a's used
3 Redundant ex de,hl's
10 Code motions
42 KB used from the upper 64KB RAM

D>z80as -j optima
Z80AS Macro-Assembler V4.7

Errors: 0

Jump optimizations done: 128
Finished.

D>c -v -c -o optimb.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE

D>CPP -DCPM -DHI_TECH_C -Dz80 -I OPTIMB.C $CTMP1.$$$

D>p1 -n $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
19 KB used from the upper 64KB RAM

D>cgen -n $CTMP2.$$$ $CTMP1.$$$
8 KB used from the upper 64KB RAM

D>optim -n $CTMP1.$$$ optimb.as
16K, 3 iterations
109 Redundant labels
170 Jumps to jumps
165 Stack adjustments
293 Temporary labels
232 Unref'ed labels
245 Unreachable code
27 Jumps to .+1
53 Skips over jumps
77 Common code seq's
5 Ex (sp),hl's used
21 Redundant operations
248 Redundant loads/stores
26 Simplified addresses
1 Xor a's used
2 Redundant ex de,hl's
25 Code motions
46 KB used from the upper 64KB RAM

D>z80as -j optimb
Z80AS Macro-Assembler V4.7

Errors: 0

Jump optimizations done: 128
Finished.

D>c -v -c -o optimc.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE

D>CPP -DCPM -DHI_TECH_C -Dz80 -I OPTIMC.C $CTMP1.$$$

D>p1 -n $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
19 KB used from the upper 64KB RAM

D>cgen -n $CTMP2.$$$ $CTMP1.$$$
8 KB used from the upper 64KB RAM

D>optim -n $CTMP1.$$$ optimc.as
15K, 3 iterations
156 Redundant labels
197 Jumps to jumps
80 Stack adjustments
126 Temporary labels
211 Unref'ed labels
164 Unreachable code
11 Jumps to .+1
37 Skips over jumps
73 Common code seq's
2 Ex (sp),hl's used
7 Redundant operations
76 Redundant loads/stores
8 Simplified addresses
11 Code motions
19 KB used from the upper 64KB RAM

D>z80as -j optimc
Z80AS Macro-Assembler V4.7

Errors: 0

Jump optimizations done: 128
Finished.

