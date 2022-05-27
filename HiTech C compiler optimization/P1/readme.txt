Improvement for the P1.COM

It works on any Z80-based computer running CP/M.

Compared to the original HiTech P1.COM, it is smaller by 4KB (the RAM space available to be allocated is larger with 4KB).

The immediate advantage is to being capable to compile larger C source files...

Here is the original P1.COM map:

TOTAL           Name         Link     Load   Length
                (abs)           0        0        0
                text          100      100     89B4
                data         8AB4     8AB4     11FB
                bss          9CAF     9CAF      801	end at 0A4B0H

And here is the improved P1.COM map:

TOTAL           Name         Link     Load   Length
                (abs)           0        0        0
                text          100      100     8440
                data         8540     8540      8E8
                bss          8E28     8E28      681	end at 094A9H , that means 1007H (= 4KB) less !!!
                top          94A9     94A9       F5

To obtain this, I followed a less-used path... because I did not had the C sources, I started working directly on the disassembled source file.

So, all the work was done on Z80 assembler sources.

What I did:

- eliminated duplicated (yes, I found 2 routines with exactly the same content...) or unused code (mostly, from the parts belonging to the C library)
- changed the size of buffer used for file input (from 512 to 128)
- optimized frequent used parts of code (compares - instead of word compares using SBB HL,RR I opted for a simple CP R, returns from C functions containg the same sequences of code, replaced with a jump to the "common" sequence,... )
- placed the initialization-related code (the "top" PSECT) after the bss segment (once executed, that code can be dumped...)
- changed a lot of warning/error message calls, using a simpler (and shorter...) message containing only an error index instead of the original error/warning text (of course, a numbered list of error/warning messages is provided) ; I kept all fatal error messages untouched...

The P1 source files must be assembled using Z80AS; submit files to assemble and link are provided.

