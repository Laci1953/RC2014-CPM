How to install CP/M
-------------------

Under SCM, load the appropriate *_PutSys*.hex (according to your RC2014's serial type & CF size)
then
*g8000<CR>

Your CP/M is ready and can be booted:
*cpm<CR>

How these .hex files were built:

>z80as putsys
>objtohex putsys.obj putsys.hex

Then, edit bdosbios.as, configure the serial & CF size, then:

>z80as bdosbios
>link
-pbdos=0d000h,bios=0e600h,top=0fffeh -obb.obj bdosbios.obj (if you have a 128MB CF)
  or
-pbdos=0da00h,bios=0f000h,top=0fffeh -obb.obj bdosbios.obj (if you have a 64MB CF)  
>objtohex bb.obj bb.hex

Then, "move" in bb.hex all records from the (D000H - FFFFH) range of loading addresses to the (9000H - BFFFH) range 
(simply edit the bb.hex file, substitute all ":20D" with ":209", ...and so on...)

Then, on Z80SIM, run makecks.com, then paste bb.hex... makecks will type the correct hex records, copy these lines to bbOK.hex file.

Then, paste putsys.hex + bbOK.hex into a single sys.hex file
Take care to keep only one EOF hex record, at end...

Now, your sys.hex is ready.

If you have an RC2014 configuration using a 64MB CF, then you will have a CP/M that loads at 0DA00H, instead of 0D000H like in the case of the CP/M for 128MB CF.

The advantage is to be able to assemble/compile larger files.

Both CP/M's (for 64MB & 128MB CF), if used on SC108, will run the text editor TE128, capable of editing (in memory) text files up to 70KB.
If used on a RC2014 provided with the 512KBRAM+512KBROM memory module, you may use the text editor TE512, capable of editing (in memory) text files up to 400KB.
