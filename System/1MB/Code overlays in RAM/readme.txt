Example of implementing code overlays as 16KB RAM 'slices' in the 512KB RAM/512KB ROM memory module

As an example of implementation, I selected a component of HiTech's C compiler, the OPTIM.COM

I had a past version of OPTIM implemented using code overlays (I eventually replaced this implementation with another 'final 'version, dropping the code overlays, preferring to have a single source code for all the hardware variants - 2x64KB RAM, 32x16KB RAM and 4x32KB RAM, but as a comment... the performance of the 'overlays' version is very close to the 'final 'version without overlays...).

The OPTIM's memory map was:

0000 - 4000H	Base code partition
		Code (including LIBC routines)
		Data
		BSS
		File buffer (1 x 200H)
4000 - 8000H	Overlays code partition (code,data) for 2 code overlays
8000 - C000H	Dynamic memory buffer (maps one of the 16KB memory banks from the 512KB RAM)
C000 - 		Available for malloc
     -	BDOS	Stack

The two code overlays (optim.ov1 and optim.ov2) are read from two files at the OPTIM's start, into two 16KB 'slices' in the memory module's RAM.

These two overlays are built using oplkovr.sub

Then, at each call of a function belonging to a specific code overlay, the appropriate 16KB RAM 'slice' is mapped to the 4000H - 8000H RAM area (using a single OUT instruction), and the code executed. That's a very fast overlay loading!

The source file 'opover.as' contains the code overlays support routines.

As an example of using the code overlays:

(old source)
...
    optimise();
...


(source with code overlays)
...
    CallOverlay0(optimise, 32+4);       /* CallOverlay0 means 'no parameters' */
                                        /* 32+4 is the index of the 16KB RAM 'slice' containing */
                                        /* the code overlay including the optimise routine */
...

Parameters can be passed to the routines, and return values received.

As a final comment, I consider this approach of using code overlays to be very easy to implement and very effective to exploit.


