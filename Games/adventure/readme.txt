The Colossal Cave Adventure (version DAIM0350 from https://github.com/Quuxplusone/Advent)

This is Daimler's 350-point "Adventure" (circa June 1990, according
to Russel Dalenberg). Its version information lists
    
        -Conversion to BDS  C by J. R. Jaeger
        -Unix standardization by Jerry D. Pohl.
        -OS/2 Conversion by Martin Heller
        -Conversion to TurboC 2.0 by Daimler
    
It contains Jerry Pohl's original ADVENT.DOC (dated 12 JUNE 1984),
plus comments from Martin Heller (dated 30-Aug-1988). Strangely for
an expansion, Daimler's version actually introduces a number of typos
into the data files, and disables a handful of inessential verbs
(READ, EAT, FILL) with the comment that there is "no room" for them
(presumably in the PC's limited memory).

Adapted for HiTech C Z80 under CP/M by Ladislau Szilagyi, Oct. 2023

I uncommented Daimler's disabled verbs - game is complete again !
I also changed the random number generator (now it is the Xorshift)

Hint: if you get lost, see CaveMap.jpg

To build the executable, use:

c -v -o advent.c database.c english.c itverb.c turn.c verb.c rand.as

But, you MUST use my enhanced HiTech C compiler (https://github.com/Laci1953/HiTech-C-compiler-enhanced) & Z80AS (https://github.com/Laci1953/Z80AS), otherwise the original HiTech C compiler fails to compile/assemble the source files...
