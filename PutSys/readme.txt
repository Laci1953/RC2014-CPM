Execute *_PutSys .hex to set-up a CP/M system to your CF

How these .hex files were builded:

First, assemble PutSys_CF64_CPM_DA00H.asm --> putsys.hex

Then, assemble the bdosbios.z80, using the appropriate settings (I used Z80ASM) --> bdosbios.hex

Then, paste the two .hex files into a single putsys.hex file
Take care on keeping only one EOF hex record...

Then, "move" the "bdosbios" part of the .hex from the D000H - FFFFH range of loading addresses to the 9000H - BFFFH range 
(simply edit the .hex file, substitute all ":20D" with ":209"...and so on...)

Then, on Z80SIM, run makecks.com, paste all the "displaced" hex fragment... makecks will type the correct hex records.

Take those correct hex records and copy them into the putsys.hex, replacing the "moved" part.

Now, your putsys.hex is ready.

If you have an RC2014 configuration using a 64MB CF, you may try to install on it a CP/M that loads at 0DA00H, instead of 0D000H like in the case of the "classic" CP/M that runs on both CF128MB/CF64MB configurations.

The advantage is to be able to assemble/compile larger files.



