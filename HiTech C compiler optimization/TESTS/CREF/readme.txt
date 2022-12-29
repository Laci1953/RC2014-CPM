This is cref.c from https://github.com/nikitinprior/dcref

D>sdir cref.c

Directory For Drive D:  User  0

    Name     Bytes   Recs   Attributes
------------ ------ ------ ------------
CREF     C      16k    100 Dir RW

D>c -v -c -o -% cref.c
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE
0:CPP -DCPM -DHI_TECH_C -Dz80 -I CREF.C $CTMP1.$$$
0:P1 -N $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
CREF.C: packSym()
   219:     qsort(symtab, var4 - symtab, sizeof(sym_t *), (int (*)(void *, void *))cmpSym);
                                                                             Warning #85 ^  (warning)
CREF.C: printAll()
   387:             qsort(psym->xrefs, psym->cntXrefs, sizeof(xref_t), (int (*)(void *, void *))cmpXref);
                                                                                           Warning #85 ^  (warning)
11 KB used from the upper 64KB RAM
0:CGEN -N $CTMP2.$$$ $CTMP1.$$$
5 KB used from the upper 64KB RAM
0:OPTIM -N $CTMP1.$$$ $CTMP2.$$$
6K, 3 iterations
28 Redundant labels
30 Jumps to jumps
73 Stack adjustments
44 Temporary labels
75 Unref'ed labels
16 Unreachable code
5 Jumps to .+1
7 Skips over jumps
7 Common code seq's
5 Ex (sp),hl's used
3 Redundant operations
73 Redundant loads/stores
15 Simplified addresses
1 Redundant ex de,hl's
4 Code motions
16 KB used from the upper 64KB RAM
0:ZAS -J -N -oCREF.OBJ $CTMP2.$$$
Z80AS Macro-Assembler V4.7

Errors: 0

Jump optimizations done: 73
Finished.
ERA $CTMP1.$$$
ERA $CTMP2.$$$
ERA $CTMP3.$$$
ERA $$EXEC.$$$

D>sdir cref.obj

Directory For Drive D:  User  0

    Name     Bytes   Recs   Attributes
------------ ------ ------ ------------
CREF     OBJ    12k     70 Dir RW

D>
