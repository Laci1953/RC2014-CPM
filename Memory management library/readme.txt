You may find here an efficient memory allocation package

It contains the following functions:
- init_mem
- alloc
- free
- realloc
- sbrk
- total_free

These routines may be called from C source files (HiTech C)

The routines must be executed ONLY on "real" Z80 computers (they contain undocumented Z80 instructions...)

The source file "new malloc.as" must be assembled using Z80AS.

Results of a test (see test.c) comparing the "classic" HiTech C malloc/free with this new package:

(NEW ALLOC)

D>sdir test.com

Directory For Drive D:  User  0

    Name     Bytes   Recs   Attributes
------------ ------ ------ ------------
TEST     COM     8k     47 Dir RW

D>test
Total 1249 alloc calls
Total 560 free calls
Total 44490 bytes allocated in 689 buffers

D>test
Total 1296 alloc calls
Total 606 free calls
Total 44150 bytes allocated in 690 buffers

D>test
Total 1424 alloc calls
Total 741 free calls
Total 43933 bytes allocated in 683 buffers

D>test
Total 1627 alloc calls
Total 933 free calls
Total 44256 bytes allocated in 694 buffers

D>test
Total 1175 alloc calls
Total 494 free calls
Total 44425 bytes allocated in 681 buffers

D>test
Total 1319 alloc calls
Total 655 free calls
Total 43936 bytes allocated in 664 buffers

D>test
Total 1245 alloc calls
Total 575 free calls
Total 44367 bytes allocated in 670 buffers

D>

(OLD ALLOC)

D>sdir test.com

Directory For Drive D:  User  0

    Name     Bytes   Recs   Attributes
------------ ------ ------ ------------
TEST     COM     8k     45 Dir RW

D>test
Total 1326 alloc calls
Total 679 free calls
Total 40323 bytes allocated in 647 buffers

D>test
Total 1069 alloc calls
Total 410 free calls
Total 41289 bytes allocated in 659 buffers

D>test
Total 1351 alloc calls
Total 781 free calls
Total 38274 bytes allocated in 570 buffers

D>test
Total 1069 alloc calls
Total 410 free calls
Total 41289 bytes allocated in 659 buffers

D>test
Total 1417 alloc calls
Total 817 free calls
Total 37912 bytes allocated in 600 buffers

D>test
Total 1103 alloc calls
Total 473 free calls
Total 40279 bytes allocated in 630 buffers

D>test
Total 1251 alloc calls
Total 625 free calls
Total 39636 bytes allocated in 626 buffers

D>

It can be noticed that this new alloc/free package allows the user to allocate more memory (~4Kb more), compared to the old one.
This is probably because the new alloc/free algorithm works better, compared with the old one, in the case of a highly fragmented free memory.

Testing a C app which uses the new alloc
----------------------------------------

Let's take a simple C app:

D>type t.c
/* Converts a .SYM file to a .AS file */

#include <sys.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

FILE* in_sym;
FILE* out_as;
char inbuf[32];
char outbuf[32];
char value[5];
char sym[28];
char* i, *o;
short n;

int main(int argc, char*argv[])
{
  init_mem();

  if (argc != 3)
  {
    printf("Invalid parameters!\r\nUsage is: symtoas file.as file.sym\r\n");
    exit(0);
  }

  if (!(in_sym=fopen(argv[2], "r")))
  {
    printf("Cannot open .SYM file!\r\n");
    exit(0);
  }

  if (!(out_as=fopen(argv[1], "w")))
  {
    printf("Cannot open .AS file!\r\n");
    exit(0);
  }

  while (i=fgets(inbuf, 31, in_sym))
  {
    for (n=0; n<4; n++)
      value[n] = *i++;

    value[4]=0;
    i++;

    o = outbuf;

    strcpy(o, "GLOBAL ");
    o = o + strlen(o);

    n=0;
    do
      sym[n++] = *i++;
    while (*i != 0x0A);

    sym[n] = 0;

    strcat(o, sym);
    strcat(o, "\n");

    fputs(outbuf, out_as); /* write "GLOBAL sym" */

    o = outbuf;
    strcpy(o, sym);

    strcat(o, " EQU 0");
    strcat(o, value);
    strcat(o, "H\n");

    fputs(outbuf, out_as); /* write "sym EQU 0vvvvH" */
  }

  fclose(in_sym);
  fclose(out_as);

  exit(1);
}

Instead using only the functions provided by the standard HiTech C library, we will use also specific functions provided by the new allocation package.
The particular C library function important in our test is 'sbrk'. It allocates a buffer in the heap.
It is used by all file related system functions to allocate a 512 buffer, to be used by disk I/O operations.
So, we will include 'myalloc.as' in the C command line, to "force" the linker to use the new 'sbrk', instead of the 'sbrk' from CLIB.LIB

D>c -v -o -ft.sym -mt.map t.c myalloc.as
HI-TECH C COMPILER (CP/M-80) V3.09
Copyright (C) 1984-87 HI-TECH SOFTWARE
T.C
0:CPP -DCPM -DHI_TECH_C -Dz80 -I T.C $CTMP1.$$$
0:P1 $CTMP1.$$$ $CTMP2.$$$ $CTMP3.$$$
0:CGEN $CTMP2.$$$ $CTMP1.$$$
0:OPTIM $CTMP1.$$$ $CTMP2.$$$
0:ZAS -J -N -oT.OBJ $CTMP2.$$$
Z80AS Macro-Assembler V4.8

Errors: 0

Jump optimizations done: 5
Finished.
MYALLOC.AS
0:ZAS -J -OMYALLOC.OBJ MYALLOC.AS
Z80AS Macro-Assembler V4.8

Errors: 0

Jump optimizations done: 7
Finished.
ERA $CTMP1.$$$
ERA $CTMP2.$$$
ERA $CTMP3.$$$
0:LINK -Z -DT.SYM -MT.MAP -Ptext=0,data,bss -C100H -OT.COM CRTCPM.OBJ T.OBJ MYALLOC.OBJ LIBC.LIB
ERA $$EXEC.$$$

D>

Let's verify that the new 'sbrk' is used, and NOT the 'old sbrk' from CLIB.LIB :

D>type t.map
Machine type is Z80

CRTCPM.OBJ      text            0      13D      data         24DE        1
T.OBJ           text          13D      1C4      data         24DF       9A
                bss          27B3       6B
MYALLOC.OBJ     text          301      418

LIBC.LIB
printf.obj      text          719       1F
doprnt.obj      text          738      2F3      data         2579        7
                bss          281E        5
gets.obj        text          A2B       BE
puts.obj        text          AE9       55
fputc.obj       text          B3E       58
flsbuf.obj      text          B96       A7
fopen.obj       text          C3D       4B
freopen.obj     text          C88      135
fseek.obj       text          DBD      2C3
fread.obj       text         1080       93
fgetc.obj       text         1113       84
filbuf.obj      text         1197       BB
stdclean.obj    text         1252       25      data         2580       40
                bss          2823      200
fclose.obj      text         1277       67
fflush.obj      text         12DE       80
buf.obj         text         135E       46      bss          2A23        2
exit.obj        text         13A4       13
start1.obj      text         13B7        3      bss          2A25        2
start2.obj      text         13BA       4E      data         25C0        1
open.obj        text         1408       BE
read.obj        text         14C6      268
write.obj       text         172E      21C
seek.obj        text         194A      181
creat.obj       text         1ACB       7A
close.obj       text         1B45       79
unlink.obj      text         1BBE       5F
getfcb.obj      text         1C1D      308      data         25C1       10
signal.obj      text         1F25       73      bss          2A27        2
getuid.obj      text         1F98       26
bdos.obj        text         1FBE       1E
bdoshl.obj      text         1FDC       16
cleanup.obj     text         1FF2       63      data         25D1      150
_exit.obj       text         2055        B
asallsh.obj     text         2060       12
allsh.obj       text         2072       11
asladd.obj      text         2083        9
atoi.obj        text         208C       40
ctype_.obj      data         2721       81
iregset.obj     text         20CC       26
ladd.obj        text         20F2        E
brelop.obj      text         2100       14
wrelop.obj      text         2114        F
lrelop.obj      text         2123       23
lsub.obj        text         2146       10
idiv.obj        text         2156       68
pnum.obj        text         21BE      15A      data         27A2       11
ldiv.obj        text         2318      120
bmove.obj       text         2438       10
imul.obj        text         2448       1F
alrsh.obj       text         2467       14
strcat.obj      text         247B       1B
strcpy.obj      text         2496       12
strlen.obj      text         24A8        E
csv.obj         text         24B6       28

TOTAL           Name         Link     Load   Length
                (abs)           0        0        0
                text            0        0     24DE
                data         24DE     24DE      2D5
                bss          27B3     27B3      276

                                  Symbol Table

__Hbss      bss   2A29  __Hdata     data  27B3  __Htext     text  24DE
__Lbss      bss   27B3  __Ldata     data  24DE  __Ltext     text  0000
__argc_     bss   2A25  __bufallo   text  135E  __buffree   text  1388
__cleanup   text  1252  __cpm_clean text  1FF2  __ctype_    data  2721
__doprnt    text  0779  __exit      text  2055  __fcb       data  25D1
__filbuf    text  1197  __flsbuf    text  0B96  __fsize     text  194A
__getargs   text  13BA  __iob       data  2580  __pnum      text  21BE
__putrno    text  2014  __sibuf     bss   2823  __sigchk    text  1F58
_atoi       text  2093  _bdos       text  1FBE  _bdoshl     text  1FDC
_bmove      text  2438  _close      text  1B45  _creat      text  1ACB
_exit       text  13A4  _fc_parse   text  1D1E  _fclose     text  1277
_fflush     text  12DE  _fgetc      text  1113  _fgets      text  0A2B
_fopen      text  0C3D  _fputc      text  0B3E  _fputs      text  0AE9
_fread      text  1080  _freopen    text  0C88  _fseek      text  0EAC
_ftell      text  0FFF  _get_flp    text  0301  _getfcb     text  1C3C
_gets       text  0AA3  _getuid     text  1F98  _i          bss   2818
_in_sym     bss   27D8  _inbuf      bss   27F6  _init_mem   text  0305
_lseek      text  1A19  _main       text  013D  _movmem     text  2438
_myfree     text  043D  _mymalloc   text  0336  _myrealloc  text  051E
_n          bss   281A  _o          bss   281C  _open       text  1408
_out_as     bss   2816  _outbuf     bss   27B8  _printf     text  0719
_putfcb     text  1C7B  _puts       text  0B1E  _read       text  14C6
_sbrk       text  0336  _setfcb     text  1C8E  _setuid     text  1FAC
_signal     text  1F25  _strcat     text  247B  _strcpy     text  2496
_strlen     text  24A8  _sym        bss   27DA  _total_free text  0320
_unlink     text  1BBE  _value      bss   27B3  _write      text  172E
adiv        text  2165  aladd       text  20F2  aldiv       text  2364
allsh       text  2072  almod       text  238B  alrsh       text  2467
alsub       text  2146  amod        text  2156  amul        text  2448
arelop      text  2123  asaladd     text  2083  asaldiv     text  234F
asallsh     text  2060  asalmod     text  239D  aslladd     text  2083
aslldiv     text  2383  aslllsh     text  2060  asllmod     text  23B0
brelop      text  2100  cret        text  24C2  csv         text  24B6
indir       text  24C9  iregset     text  20CC  iregstore   text  20E6
ldiv        text  2160  lladd       text  20F2  lldiv       text  2377
lllsh       text  2072  llmod       text  23A5  llsub       text  2146
lmod        text  215B  lmul        text  2448  lrelop      text  2123
ncsv        text  24CA  start       text  0100  startup     text  13B7
wrelop      text  2114

D>

As you noticed, in the list of symbols, sbrk = 0336H, which is inside myalloc.obj address space.
Therefore, the 'new' sbrk will be used.

Let's now test the new app:

D>t t.as t.sym

D>type t.as
GLOBAL _myrealloc
_myrealloc EQU 0051EH
GLOBAL __cpm_clean
__cpm_clean EQU 01FF2H
GLOBAL _total_free
_total_free EQU 00320H
GLOBAL _i
_i EQU 02818H
GLOBAL _n
_n EQU 0281AH
GLOBAL _o
_o EQU 0281CH
GLOBAL csv
csv EQU 024B6H
GLOBAL amod
amod EQU 02156H
GLOBAL adiv
adiv EQU 02165H
GLOBAL lmod
lmod EQU 0215BH
GLOBAL cret
cret EQU 024C2H
GLOBAL amul
amul EQU 02448H
GLOBAL ldiv
ldiv EQU 02160H
GLOBAL _sym
_sym EQU 027DAH
GLOBAL ncsv
ncsv EQU 024CAH
...

It works as desiged.
