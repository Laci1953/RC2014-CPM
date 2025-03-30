# RC2014-CPM
------------

You may find here CP/M system-related software (folder System) and some games (folder Games).

Folder SYSTEM
-------------

Some CP/M system programs ( copyq, eraq, grep, fasttype, fastdump ) and a customized CP/M ( folders BDOS_BIOS, BOOT, PutSys )

Why a "customized" CP/M ?
---------------------

Because the "official" RC2014's CP/M BIOS is too "fat" and it is not configured according to the size of the current CF (64 / 128 MB).

The "official" CP/M base address is always 0D000H, eating all the available RAM space till 0FFFFH.

Why is this a bad thing?

For two reasons.

First, BIOS allocates buffers for 16 disk drives. For systems provided with a 64MB CF, this results in vasted 2KB RAM.

Second, for the 128KB RAM Z80 systems, it's impossible to access the second 64KB RAM bank. It is impossible to make use of the "upper" 64KB RAM because at the top adresses, some "shadow" routines must be stored, at the same address, in both the "lower" and "upper" 64KB RAM, in order to be able to move bytes between these two 64KB banks, and this "free" space is not available. So, the owners of Z80 systems provided with SC108, SC114, SC118, SC150, SC152 or the Phillip Stevens memory module, or RCBUS-based systems provided with SC707, SC714, are affected by this.

This is why I tried to customize the BIOS, in order to obtain a "thinner" CP/M.

Using this "customized" BIOS, it is now possible to benefit from :

- an enhanced HiTech C Compiler, able to compile larger C source files ( see https://github.com/Laci1953/HiTech-C-compiler-enhanced )
- an enhanced TE text editor, able to edit (in memory) larger text files ( see https://github.com/Laci1953/RC2014-CPM/tree/main/System/te%20text%20editor )
