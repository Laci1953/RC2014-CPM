#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MAX 2000

int n;
int count = 0;
char* ptr[MAX];
int size[MAX];
int crt;
int s;
int total = 0;

FILE* in;
FILE* out;
char buf[256];

void xrndseed(void);
unsigned int xrnd(void);

int next(void)
{
	if (count == 0)
	{
		count++;
		return 0;
	}

	for (n = 0; n < count; n++)
		if (ptr[n] == (char*)0)
			return n;

	count++;

	if (count > MAX)
	{
		printf("TOO MANY ITEMS! ...exiting");
		exit(0);
	}

	return (count-1);
}

int main(void)
{
	// read from in, write to out (and terminal)

	if (!(in=fopen("in.txt", "r")))
	{
    		printf("Cannot open in.txt file!\r\n");
		exit(0);
	}

	if (!(out=fopen("out.txt", "w")))
	{
    		printf("Cannot open out.txt file!\r\n");
    		exit(0);
	}

	while (n=fgets(buf, 256, in))
	{
		fputs(buf, out);
		printf(buf);
	}

	fclose(in);
	fclose(out);

	xrndseed();

	for (n = 0; n < MAX; n++)
		ptr[n] = (char*)0;

	do
	{
		//two allocs

		s = (xrnd() % 256) + 1;
		crt = next();
		ptr[crt] = malloc(s);

		if (ptr[crt] == 0)
			break;

		size[crt] = s;
		total += s;

		//printf("%u bytes allocated @ %04x, total=%u\n", s, ptr[crt], total);

		s = (xrnd() % 256) + 1;
		crt = next();
		ptr[crt] = malloc(s);

		if (ptr[crt] == 0)
			break;

		size[crt] = s;
		total += s;

		//printf("%u bytes allocated @ %04x, total=%u\n", s, ptr[crt], total);

		//one free
		
		do
		{
			n = xrnd() % count;

			if (ptr[n] != (char*)0)
			{
				free(ptr[n]);
				total -= size[n];
				//printf("%u bytes freed from %04x, total=%u\n", size[n], ptr[n], total);
				ptr[n] = (char*)0;
				break;
			}
		} while (1);
	} while(1);

	printf("Total bytes allocated = %u", total);
	exit(1);
}
