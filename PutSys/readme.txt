ONLY FOR RC2014'S WITH CONFIGURATIONS USING CF 64 MB

If you have an RC2014 configuration using a 64MB CF, you may try to install on it a CP/M that loads at 0DA00H, instead of 0D000H like in the case of the "classic" CP/M that runs on both CF128MB/CF64MB configurations.

The advantage is to be able to assemble/compile larger files.

Execute *_PutSys .hex to set-up a CP/M that loads at 0DA00H

(makecks MUST be run on Z80SIM)

