I took the source of the OPTIM.COM, the optim.c file, and sliced-it in 3 parts: optima.c, optimb.c and optimc.c.
You can see below the size of these files... 

D>sdir optim?.c

Directory For Drive D:  User  0

    Name     Bytes   Recs   Attributes      Name     Bytes   Recs   Attributes
------------ ------ ------ ------------ ------------ ------ ------ ------------
OPTIMA   C     128k   1007 Dir RW       OPTIMB   C      68k    529 Dir RW
OPTIMC   C      48k    363 Dir RW

Total Bytes     =    244k  Total Records =    1899  Files Found =    3
Total 1k Blocks =    239   Used/Max Dir Entries For Drive D:  484/ 512

Let's try first to compile these files using the old HiTech toolchain:

E>c -v -c -o optima.c
\HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE
0:CPP -DCPM -DHI_TECH_C -Dz80 -I OPTIMA.C $CTMP1.$$$
0:P1 $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
OPTIMA.C:
   622: int ccSwap[] = {
                     ^ ccSwap: storage class redeclared
                     ^ identifier redefined: ccSwap
                     ^ illegal initialisation
                     ^ initialisation syntax
   744: void           chkIXYUsage();
                                    ^ chkIXYUsage: storage class redeclared
OPTIMA.C: 41
[v _allocInst `(*S8 0 e ]
"742
[v _syntheticLabel `(*S8 0 e ]
"743
[v _optimise `(v 0 e ]
"744
 ] F462 `*********s 1
"745
[15()
   746: char           sub_1071(inst_t *);
                                                                                                                                                                                                                                      : storage class redeclared ^
                                                                                                                                                                                                                                      ) expected ^
 0 ]
"639
[a 159 98 1:
   747: void           sub_122f();
        ^ *: storage class redeclared
        ^ no identifier in declaration
        ^ ; expected
   748: char           sub_1369(operand_t  *);
        RC2014 CP/M BIOS 1.2 by G. Searle 2007-18

CP/M 2.2 Copyright 1979 (c) by Digital Research

A>

(ups ... P1 crashed...)

E>c -v -c -o optimb.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE
0:CPP -DCPM -DHI_TECH_C -Dz80 -I OPTIMB.C $CTMP1.$$$
0:P1 $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
OPTIMB.C:
   465: extern char        key_f;
                                ^ key_f: storage class redeclared
   466: extern char        key_s;
                                ^ key_s: storage class redeclared
   467: extern int         num_warn;
                                   ^ num_warn: storage class redeclared
   496: extern operand_t   regValues[19];
                                       ^ illegal type for array dimension
   497: extern char       *alloct;
                                 ^ alloct: storage class redeclared
   498: extern char       *name_fun;
             Out of memory ^
ERA $CTMP1.$$$
ERA $CTMP2.$$$
ERA $CTMP3.$$$
ERA $$EXEC.$$$

E>c -v -c -o optimc.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE
0:CPP -DCPM -DHI_TECH_C -Dz80 -I OPTIMC.C $CTMP1.$$$
0:P1 $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
OPTIMC.C:
   465: extern char        key_f;
                                ^ key_f: storage class redeclared
   466: extern char        key_s;
                                ^ key_s: storage class redeclared
   467: extern int         num_warn;
                                   ^ num_warn: storage class redeclared
   496: extern operand_t   regValues[19];
                                       ^ illegal type for array dimension
   497: extern char       *alloct;
                                 ^ alloct: storage class redeclared
   498: extern char       *name_fun;
             Out of memory ^
ERA $CTMP1.$$$
ERA $CTMP2.$$$
ERA $CTMP3.$$$
ERA $$EXEC.$$$

E>

Total failure...

Now, let's compile these files using the new HiTech toolchain:

D>c -v -c -o -% optima.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE
0:CPP -DCPM -DHI_TECH_C -Dz80 -I OPTIMA.C $CTMP1.$$$
0:P1 -N $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
20 KB used from the upper 64KB RAM
0:CGEN -N $CTMP2.$$$ $CTMP1.$$$
9 KB used from the upper 64KB RAM
0:OPTIM -N $CTMP1.$$$ $CTMP2.$$$
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
0:ZAS -J -N -oOPTIMA.OBJ $CTMP2.$$$
Z80AS Macro-Assembler V4.7

Errors: 0

Jump optimizations done: 128
Finished.
ERA $CTMP1.$$$
ERA $CTMP2.$$$
ERA $CTMP3.$$$
ERA $$EXEC.$$$

D>c -v -c -o -% optimb.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE
0:CPP -DCPM -DHI_TECH_C -Dz80 -I OPTIMB.C $CTMP1.$$$
0:P1 -N $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
19 KB used from the upper 64KB RAM
0:CGEN -N $CTMP2.$$$ $CTMP1.$$$
8 KB used from the upper 64KB RAM
0:OPTIM -N $CTMP1.$$$ $CTMP2.$$$
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
0:ZAS -J -N -oOPTIMB.OBJ $CTMP2.$$$
Z80AS Macro-Assembler V4.7

Errors: 0

Jump optimizations done: 128
Finished.
ERA $CTMP1.$$$
ERA $CTMP2.$$$
ERA $CTMP3.$$$
ERA $$EXEC.$$$

D>c -v -c -o -% optimc.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE
0:CPP -DCPM -DHI_TECH_C -Dz80 -I OPTIMC.C $CTMP1.$$$
0:P1 -N $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
19 KB used from the upper 64KB RAM
0:CGEN -N $CTMP2.$$$ $CTMP1.$$$
8 KB used from the upper 64KB RAM
0:OPTIM -N $CTMP1.$$$ $CTMP2.$$$
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
0:ZAS -J -N -oOPTIMC.OBJ $CTMP2.$$$
Z80AS Macro-Assembler V4.7

Errors: 0

Jump optimizations done: 128
Finished.
ERA $CTMP1.$$$
ERA $CTMP2.$$$
ERA $CTMP3.$$$
ERA $$EXEC.$$$

D>
