Various enhancments for the HiTech's C compiler - updated on 29 December 2022

This work is based on the decompiled HiTech tools found at https://github.com/nikitinprior

Thanks to the excellent work of Andrey Nikitin and Mark Ogden, I was able to have a very solid foundation to build on.

The main goal of this project was to obtain an enhanced HiTech C compiler toolchain, to be used in two environments:

1 : generic, for all Z80 computers running CP/M 2.2
2 : specific, only for RC2014's provided with 128 KB RAM (SC108, SC114, SC118 or Phillip Stevens memory module) running CP/M 2.2

For the second (specific) environment, the basic idea was to use the upper 64KB RAM to extend the RAM space available for the HiTech tools.
This was done by intercepting (part of) the malloc/free calls used by a tool, by using some extra assembler code, 
to return pointers not from the "usual" lower 64KB RAM, but from the upper 64 KB RAM. 
Practically, new "memory allocators" were written, to handle the upper 64KB RAM space.
Of course, all the references to those pointers, in the C sources, must be handled by special routines, 
to move bytes from/to the upper 64KB RAM to the lower 64KB RAM.

Obviously, this means some extra code is executed, compared to the "original" HiTech tools, resulting in a longer execution time.

However, this loss of speed ( 10 to 15% ) is largely compensated by the big advantage obtained: to be able to compile larger C source files.

The results:

For the generic environment (all Z80 computers running CP/M 2.2) some limited improvements were obtained, mainly for P1 (4KB more RAM available)

For RC2014's provided with 128 KB RAM (SC108, SC114, SC118, SC150, SC152 or Phillip Stevens memory module) and 64MB CF, 
some significant improvements were obtained, listed below:

- P1 (using the upper 64KB RAM for some of the malloc's)
- CGEN (using the upper 64KB RAM for some of the malloc's)
- OPTIM (using the upper 64KB RAM for some of the malloc's)

The most significant improvement for this second environment (128KB RAM computers),
is that P1 , CGEN and OPTIM can handle now files that could not be compiled previously on any available Z80 computer, including here also the emulators/simulators (Z80SIM, ZXCC).

Also, remember that Z80AS eliminates now all the worries related to the size of files to be assembled.

As a conclusion, the RC2014's provided with 128 KB RAM can now take full advantage of having more memory space, while working with the HiTech C tools.

A clone of the C.COM compiler is provided, with an extra switch -% listing memory use statistics for P1, CGEN and OPTIM

See the TEST folder for examples of large C files being processed on a RC2014 provided with the SC108 board (128KB RAM).
