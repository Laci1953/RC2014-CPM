I was curious about the performance of some of the original CP/M transient commands...e.g. DUMP.COM

I found the source of DUMP.COM in the DR's CP/M manual and studied-it carefully.

Then, I wrote my own FASTDUMP.COM

Some comments :

- My FASTDUMP behaves like the "original" DUMP, expecting the same filename.ext parameter and typing the same output
- You may stop-it hitting any key... exactly as the "original" DUMP
- I added a little help text, if no parameters are used in the command line...
- I used direct BIOS calls for console out/in/status, instead of BDOS calls
- I used a faster bin-to-hex routine, compared to the one used by the "original" DUMP

Some testing proved that it is almost twice as faster as the original DUMP.COM
I tested my FASTDUMP on a 7.3728 MHz RC2014, provided with my custom BIOS ( https://github.com/Laci1953/RC2014-CPM/tree/main/BDOS_BIOS )

Statistics:
                 DUMP     FASTDUMP
--------------------------------------------------------
144 KB file:     95s      50s
204 KB file:     110s     65s
