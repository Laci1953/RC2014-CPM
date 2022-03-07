Execute *_PutSys*.hex to set-up a CP/M system to your CF (you must choose the appropriate serial type & CF size)

How these .hex files were builded:

>z80as putsys
>objtohex putsys.obj putsys.hex

Then, edit bdosbios.as, configure the serial & CF size

>z80as bdosbios
>link
-pbdos=0d000h,bios=0e600h,top=0fffeh -obb.obj bdosbios.obj (if you have a 128MB CF)
  or
-pbdos=0da00h,bios=0f000h,top=0fffeh -obb.obj bdosbios.obj (if you have a 64MB CF)  
>objtohex bb.obj bb.hex

Then, paste putsys.hex + bb.hex into a single sys.hex file
Take care on keeping only one EOF hex record, at end...

Then, "move" the "bdosbios" part of the .hex from the D000H - FFFFH range of loading addresses to the 9000H - BFFFH range 
(simply edit the .hex file, substitute all ":20D" with ":209", ...and so on...)

Then, on Z80SIM, run makecks.com, paste all the "displaced" hex fragment... makecks will type the correct hex records.

Take those correct hex records and copy them into the sys.hex, replacing the "moved" part.

Now, your sys.hex is ready.

If you have an RC2014 configuration using a 64MB CF, then you will have a CP/M that loads at 0DA00H, instead of 0D000H like in the case of the CP/M for 128MB CF.

The advantage is to be able to assemble/compile larger files.

Both CP/M's (for 64MB & 128MB CF), if used on SC108, will run the text editor TE128, capable of editing (in memory) text files up to 70KB.
If used on a RC2014 provided with the 512KBRAM+512KBROM memory module, you may use the text editor TE512, capable of editing (in memory) text files up to 400KB.
