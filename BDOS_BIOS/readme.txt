For RC2014'S with SIO/ACIA/KIO, 64MB/128MB CF

This version of BDOS+BIOS is smaller, compared with the "original"

The advantage of having a smaller BIOS:

  1.Suppose you have a memory module provided with 128KB RAM (SC108, SC114, SC118, SC150, SC152).
The software packages that want to use the extra 64KB RAM are constrained to "implant" some small piece of code, in both 64KB RAM partitions, at the highest address possible. This code, stored in both 64KB RAM partitions, will manage the RAM memory allocation that will enable the software to access more than the "usual" 64KB.
With the "original" RC2014 BIOS, this is not possible, because this version of BIOS "eats" all the available memory, up to 0FFFFH. 
My version of BIOS, being smaller, reserves some small buffer at the highest RAM addresses, for this purpose.

  2.If you have a 64MB CF, the resulting CP/M is smaller, being loaded at 0DA00H. This offers a larger TPA.

The source file must be assembled using Z80AS.
