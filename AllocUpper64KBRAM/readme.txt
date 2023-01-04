How to use the run-time routines to allocate memory in the Upper 64 KB RAM bank

In order to use these routines, the host RC2014 hardware system must fulfill some mandatory conditions.

First, it must have 128 KB RAM (SC108, SC114, SC118 or have the Phillip Stevens memory module installed).

Use the SC108 condition to select the appropriate hardware, in the dynm128.as file.

Also, it must have installed on the 32 KB ROM a SCM version with the $2A and $2B API functions implemented, 
or to have as ROM a CP/M booter from my BOOT folder (it contains, at the address 7F00H in the ROM, two JP's pointing to the Upper 64KB RAM read and write routines).

This is needed in order to install the "shadow" routines on both the low and up 64 KB RAM banks, at the same address.

Further, these "shadow" routines must be installed as high as possible in the memory; this is possible only if the BIOS leaves some free space at its top addresses, thus a "special" BIOS is needed. This BIOS is provided as part of my "custom" CP/M, in the PutSys folder.

If all these conditions are fulfilled, it is possible to allocate memory in the Upper 64 KB RAM bank, from any program running on CP/M.

In order to be able to access the Upper 64 KB RAM, the program must first set-up the software interface, by calling:

	InitDynM();

Then, the following functions are available:

void*	Alloc128(short size, char* type); /* returns NULL is no more free memory is available */
					  /* returns type = 0 : buffer is allocated in the low 64KB RAM, */
					  /*		    1 : buffer is allocated in the upper 64KB RAM */
void	Free128(void* buf, char type);
int	GetTotalFree(void);

/*	source is in Upper RAM, destination is in Lower RAM */
void	ReadMem(char* dest, char* source, short count);
void	GetString(char* dest, char* source);

/*	source is in Lower RAM, destination is in Upper RAM */
void	WriteMem(char* source, char* dest, short count);
void	PutString(char* source, char* dest);

/*	dest is in Upper RAM */
void	PutByte(char* dest, char b);

/*	source is in Upper RAM */
char	GetByte(char* source);

/*	dest is in Upper RAM */
void	PutWord(char* dest, short w);

/*	source is in Upper RAM */
short	GetWord(char* source);

/*	source is in Upper RAM */
short	StringLen(char* source);

After obtaining the pointer to an allocated buffer in the upper 64 KB RAM, the program must read / write bytes in this buffer only using the already mentioned functions. 
A direct access to this allocated buffer is impossible, if the buffer is located in the Upper 64KB RAM bank.

An example is provided in test128.c

Use make.sub to build the test128.com
