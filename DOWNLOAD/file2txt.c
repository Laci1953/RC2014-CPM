/* 	file2txt.c
	(c) by ladislau szilagyi, 2021
	converts several CP/M files to a "download" hex format file (download.txt).

	(on the "source" CP/M system containing the files to be copied):

	>file2txt list.txt

		list.txt contains the list of the files to be converted, e.g.:

		file1.com
		file2.h

		file2txt builds a file named "download.txt", containing, as text, for each file:

		filename.ext
		: chars converted to hex >(byte count)(checksum)

		e.g.:

		file1.com
		:01AA00......>1035	(to be copy/pasted)
		file2.h
		:2031.....>20AF		(to be copy/pasted)
		
	(then, on the target CP/M sys):

        >mydw file1.com
		<copy & paste from download.txt the appropriate part>
	OK
	>mydw file2.h
		<copy & paste from download.txt the appropriate part>
	OK
	>

*/

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<ctype.h>

char buf[128];
char file[14];
short chk;
FILE *fpL, *fpI, *fpO;
short size, i;
short first;

void main(short argc, char **argv)
{
  if (argc != 2)
  {
    printf("\r\nNo file specified");
    return;
  }

  if ((fpL = fopen(argv[1], "r")) == NULL)
  {
    printf("\r\nList of files %s not found", argv[1]);
    return;
  }

  if ((fpO = fopen("download.txt", "w")) == NULL)
  {
    printf("\r\nCould not create destination file download.txt");
    return;
  }

  first = 1;

  while (fgets(file, 14, fpL))
  {
    for (i=0; i < 14; i++)
    {
      if ((file[i] == 0x0D) || (file[i] == 0x0A))
        file[i] = 0;

      if (isalpha(file[i]))
      {
        if (islower(file[i]))
          file[i] = toupper(file[i]);
      }
    }

    printf("\r\nProcessing %s ...", file);

    if ((fpI = fopen(file, "rb")) == NULL)
    {
      printf("\r\nFile %s not found", file);
      return;
    }
 
    chk = 0;
    size = 0;

    if (first)
    {
      first = 0;
      fprintf(fpO, "%s\r\n", file);
    }
    else
      fprintf(fpO, "\r\n%s\r\n", file);

    fprintf(fpO, ":");

    while (fread(buf, 1, 128, fpI))
    {
      size += 128;

      for (i=0; i<128; i++)
      {
        fprintf(fpO, "%02x", buf[i] & 0xff);
        chk += buf[i] & 0xff;
      }
    }

    fprintf(fpO, ">");
    fprintf(fpO, "%02x", size & 0xff);
    fprintf(fpO, "%02x", chk & 0xff);

    fclose(fpI);
  }

  fclose(fpO);
}
