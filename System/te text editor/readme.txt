# Updated on 5 November 2023
-----------------------------

The text editor TE was written by Miguel Garcia.

It's simple, user friendly, and edits the text in the RAM (it's fast...).

But, because the CP/M's 64KB RAM is limited as capacity (the operating system, the program and the text file must fit inside these 64KB RAM), the original TE allowed only small files to be edited (up to 20K).

I made this adaptation to harness the advantages of being provided with some extra RAM, in certain Z80 hardware platforms.

This version of the text editor TE works on the following hardware environments:

TE64     - any RC2014 with 64KB RAM
TE128    - Z80 systems provided with SC108, SC114, SC118, SC150, SC152 or the Phillip Stevens memory module (128 KB RAM),
TE512    - Z80 systems provided with the 512KB RAM + 512KB ROM module
TEZ80ALL - Z80ALL (25MHz Z80, 4 x 32KB RAM, VGA 48x64, keyboard) (see https://github.com/Laci1953/Software-for-Z80ALL-retro-computer/tree/main/TE)
TES80    - Simple80 (2 x 64KB RAM)

Use the HiTech tools to compile & link the sources.

Settings:

TE64:  64K RAM systems - up to 2048 lines, VT100 compatible, with WordStar style keyboard (accepts also arrow keys)
TE128: 128K RAM systems - up to 4093 lines, VT100 compatible, with WordStar style keyboard (accepts also arrow keys)
TE512: 512K RAM systems - up to 8192 lines, VT100 compatible, with WordStar style keyboard (accepts also arrow keys)
TEZ80ALL: Z80ALL standalone retro computer - up to 8192 lines, VGA 48x64, with WordStar style keyboard (accepts also arrow keys)
TES80: 128K RAM systems - up to 4093 lines, VT100 compatible, with WordStar style keyboard (accepts also arrow keys)

Design details and some constraints:

1. For the 512KB version: 
-------------------------

To handle the memory allocation requests, a new memory allocator was written. 

How it works:

The 512KB RAM module uses 32 16KB "virtual" RAM pages, mapped to one of 4 "physical" pages 0000-3FFF, 4000-7FFF, 8000-BFFF and C000-FFFF. 
Any virtual page can be mapped to any physical page.

The memory physical area from 8000H to BFFFH (let's name-it "work area") is used to load one of the 32 "virtual" 16KB pages from the 512KB RAM, and the memory allocator returns a pointer to the available buffer and a byte containing the index of the 16KB page that contains this buffer (let's name-it "page index"). 

When that buffer needs to be accessed for read/write operations, the 16KB RAM page must be first loaded into the "work area", 
using a function call with the "page index" as parameter.

Of course, to "free" the allocated buffer, this "page index" must be provided too.

Text files up to 400KB can be edited.

The 521KB version can be run on any CP/M version, regardless of its TPA area size.

2. For the 128KB version: 
-------------------------

To handle the memory allocation requests, a new memory allocator was written. 

How it works:

The 128KB RAM is divided into 2 "virtual" 64KB banks: the lower and the upper bank. A port selects which "virtual" 64KB RAM bank is mapped to the physical 64KB RAM.

In order to execute code that switches from the lower to the upper bank (and back...), 
that piece of code must be present at the same physical address in both of banks (the so called "shadow code)". 
It is impossible to store shadow code in the upper bank using only programs resident in RAM, 
for this only ROM-based code may help; SCM contains an API function ( $2B ) which moves a byte to a given address of the upper RAM bank. 
In early SCM version (e.g. v1.0), this API call is not documented (it cannot be found in the SCM manual), I've found-it by browsing through the SCM source code... 

Therefore, by using this $2B SCM API call, the shadow code can be moved to the upper RAM bank.

As a second possibility, this routine can be stored also, at a fixed address (7F00H) in the EPROM used to boot CP/M (see https://github.com/Laci1953/RC2014-CPM/tree/main/BOOT ).

But, for the memory allocator, we have also another important constraint: the shadow code must be stored in top of the physical 64KB RAM, in order to offer as much possible RAM available to be allocated (storing at the bottom of the physical 64KB RAM is out of question, we have there the 100H BIOS buffers area). There is only one possible solution: storing a small shadow code on top of CP/M's BIOS. My shadow code is less than 40H. 

Studying the "classic" RC2014 CP/M that is currently used with 64/128MB CF's, I discovered that the BIOS "eats" practically all the space to FFFF ! There was no room for my shadow code! 

I solved this issue by building a smaller CP/M, for both 64MB & 128MB CF's. You may find this CP/M in the PutSys folder.

The algorithm used to allocate memory is the following: if the usual call to malloc fails, the new allocator accesses the upper 64KB RAM bank and allocates there the buffer. 

Besides the address of the buffer, a byte containing the RAM bank index is returned too (0=lower 64KB RAM, 1=upper 64KB RAM).

To access a buffer reserved in the upper 64KB RAM bank, special functions must be used to read/write a byte, a word, or a string.
 
Text files up to 80KB can be edited. 

But, this comes with a cost: when opening a file to be edited, the editor reads all the text and store this text in the dynamic memory, and this takes time, because every single byte that has to be moved to the upper RAM bank is not simply moved, as in case of the lower bank, but is passed to the shadow code in order to be stored in the upper RAM bank. Also, a buffer for every new text line that is read must be allocated, and this allocation means also accessing the shadow code, in order to manage (read/write) the data structures that handle the available memory pool.  

Once the file is read, the browse/search operations prove to be quick (compared for example with the performance of WordStar), because no more file read operations are needed, all the text is in the memory.

As a conclusion: the TE editor 128KB version will work only for RC2014 systems provided with my "custom CP/M"

Comparing the two versions of TE (128KB vs. 512KB), the 512KB version is by far the best. 

First, the size of files that can be edited is larger (400KB vs. 80KB).
Then, the initial phase of file reading when starting the editor is faster.

But, I consider both versions of the TE editor as a step forward in the right direction, allowing larger files to be edited in memory, with an elegant an efficient user interface.

February 2022
-------------

TE now supports TABs.

The files, when loaded, will be displayed using TABs indentation.

The TAB interval (4 or 8) may be set from the command line: >TE file.ext [ 4 | 8 ] ; default = 8.

March 2023
----------

I made some improvements mainly related to the usability of the tool.

1. First, I opted to display a small help message, in the case when RE is executed without parameters;  the message is showing all the available options.

-------------
D>te
TE text editor
Copyright (c) 2015-2021 Miguel Garcia / FloppySoftware
Adapted for 128KB RAM by Ladislau Szilagyi

Use: >TE [options] [filename]

where options (lowercase or uppercase) are:
 -Tn (n = 4 or 8) sets the tabs to n spaces, default is 8
 -Hnn sets the screen height (up to 99, default is 24)
 -Wnnn sets the screen width (up to 200, default is 80)

Continue? (N/n = quit) :
------------

You can press N/n to quit to CP/M, or any other key to continue TE,  being presented with an empty screen, ready for text input...

2. Tabs can be used freely in the text; you have two options related to  TAB's alignment ( 4 or 8 ); the default is 8. 
You can change-it to 4 using  the option -T in the command line.
It important to mention that the use of TABs does not increase the size  of the text file, nor the size of buffers allocated in memory to store the text; this is because TABs are inflated/deflated to/from blanks only when needed, (e.g.) when displayed on screen.

3. When reading/writing files, a line containing a counter of the lines being processed is displayed, to offer to the user a clue related to the status of the I/O operation.

4. The number of columns/lines can now be selected directly in the command line, using the -W or -H options; default values are 24 for lines, and 80 for columns.

I opted to unify the two versions of source code (128/512), now there is a single set of source files for both 128KB and 512KB TE versions.

As a reminder, the TE128 can edit files with sizes up to 80KB, and TE512 up to 400KB.  

April 2023
----------

I made a major improvement, allowing text files with long lines to be processed (lines with up to 145 chars for 80 column screens, up to 115 chars for 64 column screens).

I have now also a TE adapted for Bill Shen's Z80ALL (Z80 at 25MHz, 4 x 32KB RAM, VGA 48x64, keyboard). 
Text files up to 64KB can be edited.
This version is fast enough to allow handling large files without the handicap of long waiting interval until the whole file is read - some seconds are enough...

September 2023
--------------

I improved the code related to cursor placement, in the line and up/down between lines; this involves keeping track of TAB's, and assuring that the cursor is always positioned to a position containing a "real" character (e.g. when moving the cursor in a line containing multiple TAB's, the cursor is never placed to a position that does not contain a character or a TAB)

Also, BackSpace behavior was aligned to the above rule.

October 2023
--------------

The 128KB RAM version is now capable to edit text files with size up to 80KB; also, the initial "read file" step is faster.

November 2023
-------------

The basic idea of the TE editor is to work directly with text files stored in the RAM memory; the advantage is the high processing speed ( e.g. when using the "next page" command repeatedly ). 

The problem is the long time taken until the complete loading of the text file from the disk.

Until now, the user had to wait until the file was completely loaded from disk to memory, to be able to start navigating through the text and editing the text.

So I tried to somehow solve the problem.

Now, in the new version of the TE editor, when opening a file, after reading a number of lines that fit on the screen (usually 24 lines... but depending also on the settings used in the command line), the user is free to start editing the file; during this time, in the "background", reading the file will continue, until the end of the file.

The user is warned that he can start editing, as the first text screen has been filled with records read from the text file. 

Then, after the last record has been read, the user will be notified again (the notification messages are typed on the last line of the screen, outside the space provided for editing).

Therefore, while the file is being read, the user may browse trough the already loaded part of the file, or even delete/add text.

When the user wants to save the edited file back to the disk, a counter representing the number of lines already written will be displayed on the last line of the screen, while writing continues.

A 60KB text file takes about a minute to load on a system provided with SC108.
On a system provided with 512KB RAM, it takes less, only about 15 seconds, but still disturbing...
So this improvement may prove useful for these systems...

On Z80ALL, the same file takes only 5 seconds to load, therefore it does not make sense to implement this improvement also on Z80ALL.

This improvement for the TE text editor is implemented for versions running on systems with 2 x 64 KB RAM or with 512 KB RAM.





