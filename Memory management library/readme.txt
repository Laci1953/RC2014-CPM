You may find here an efficient memory allocation package

It contains the following functions:
- alloc
- free
- realloc
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

It can be noticed that this new alloc/free package allows the user to allocate more memory (~4KB more), compared to the old one.
This is probably because the new alloc/free algorithm works better, compared with the old one, in the case of a highly fragmented free memory.

The file testall.c contains a stress test for the new memory management routines.

The LIBC.LIB file contains a modified HiTech C library.

Replaced modules:
- malloc.obj
- sbrk.obj

Also, crtcpm.obj must be replaced (new file crtcpm.as is included).

The stress.c contains a test - some file functions (read, write) then a series of malloc & free calls, repeated until no more memory is available.

A log of executing stress.com (built with the new LIBC.LIB and the new crtcpm.obj), compared with the execution of stressoo.com (built with the original LIBC.LIB):

D>stress
00000000000000000000000000000
1111111111111111111111111
22222222222222222
Total bytes allocated = 35302
D>stress
00000000000000000000000000000
1111111111111111111111111
22222222222222222
Total bytes allocated = 35340
D>stress
00000000000000000000000000000
1111111111111111111111111
22222222222222222
Total bytes allocated = 35577
D>stressoo
00000000000000000000000000000
1111111111111111111111111
22222222222222222
Total bytes allocated = 31971
D>stressoo
00000000000000000000000000000
1111111111111111111111111
22222222222222222
Total bytes allocated = 31882
D>stressoo
00000000000000000000000000000
1111111111111111111111111
22222222222222222
Total bytes allocated = 31334
D>

Again, you may see a better performance of the new allocation functions (~4KB more bytes allocated with the new functions)
