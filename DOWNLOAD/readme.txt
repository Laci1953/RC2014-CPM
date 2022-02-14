Here is how I copy files (text & binaries) from my laptop to the RC2014.

The framework is based on the Grant Searle's DOWNLOAD, with some modifications that bring limitations but also advantages.

First of all, the limitations: I can only handle files up to:
-(roughly) 50KB on any RC2014
- 115KB on RC2014 provided with SC108 32KB ROM+128KB RAM board 
- 512KB on RC2014's provided with the 512RAM+512ROM memory board. 

Now, the advantages: for serial speed, I use always 115200, and I do not need to set any delay. 

It works very fast because on RC2014 the characters are received without using BIOS, are stored to memory, and only after receiving all of them, the disk file is written.

The use scenario:

On my laptop, on Windows, I installed Udo Munk's Z80SIM (via CygWin).

I open CP/M on Z80SIM (the "source").

Here, I use FILE2TXT.COM to convert several CP/M files to a "download" hex format file.

FILE2TXT accepts as parameter the name of a text file containing the list of files to be processed.

Example: list.txt contains the list of the files to be converted, e.g.:

	file1.com
	file2.h

FILE2TXT builds a file named "download.txt", containing, as text:

	file1.com
	:01AA00......>1035	(to be copy/pasted on the target)
	file2.h
	:2031.....>20AF		(to be copy/pasted on the target)

(for each file, the contents begin with a ':', then continues with the file's bytes, in hexa, and ends with '>' followed by a byte count an a checksum)

Then, I open TeraTerm, connect the RC2014 and start CP/M (the "destination")

Here, I use MYDW.COM:

        >mydw file1.com
		<copy & paste from download.txt the appropriate part>
	OK
	>mydw file2.h
		<copy & paste from download.txt the appropriate part>
	OK
	>

Byte count & checksum is verified (but I must confess that I never received such error messages...).

MYDW must be configured according to:
- the serial (ACIA, SIO, KIO)
- 512RAM+512ROM memory, if available

FILE2TXT does not have any settings
