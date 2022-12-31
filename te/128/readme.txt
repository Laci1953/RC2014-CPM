Upated on 31 December 2022

For Steve Cousins SC108,SC114,SC118, and Phillip Stenens 32KB ROM + 128KB RAM 

Version supporting "arrow keys" and TABs

TAB intervals (4 or 8, default=8) may be set in the command line: >te file.ext [ 4 | 8 ]

In order to work well, the TE editor MUST be executed on :

- an RC2014 provided with the SC108 module
- a SCM monitor provided with the "move 1 byte to/from upper RAM" routines (older versions missed these API : $2A & $2B) ; a quick SCM test will be needed...
- my "custom CP/M" ( https://github.com/Laci1953/RC2014-CPM/tree/main/PutSys ), because the "classic" CP/M's BIOS "eats" all the high RAM up to FFFF, making impossible the storage of the "shadow" RAM routines... (while my BIOS leaves plenty of free RAM at the top...)

Of course, TE must be built from the sources found at: https://github.com/Laci1953/RC2014-CPM/tree/main/te/128 

UPDATE: (31 dec 2022)

I added TEW.COM, a new version for SC108, for very wide screens (it can handle lines up to 175 characters)

