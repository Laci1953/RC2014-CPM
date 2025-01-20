#include <stdio.h>
#include <string.h>
#include <conio.h>

void xrndseed(void);
unsigned int xrnd(void);

void init_mem(void);
char* mymalloc(int size);
char* myrealloc(char* p, int size);
void myfree(char* p);
int total_free(void);

char *handles[32];
int sizes[32];

void print_total_free(void)
{
        printf("total free bytes: %04x\r\n", total_free());
}

void print_alloc()
{
	int i;

	for (i = 0; i < 32; i++)
		if (handles[i] != 0)
			printf("allocated block @ %04x size %04x\r\n", handles[i], sizes[i]);
}

int i,j,k,l,m,om,p,f,s,n,newsize;
char ch;

void stats(void)
{
	print_total_free();
	print_alloc();
	//ch=getch();
}

void main(void)
{
	init_mem();

	print_total_free();
	printf("\r\n--------------------------------------------------\n");
	printf("--------------------------------------------------\n");
	printf("Stress test (100 iterations):\n");
	printf("--------------------------------------------------\n");

	xrndseed();

	for (k = 0; k < 100; k++)
	{
		printf("Iteration nr. %d\r\n", k+1);

		for (n = 0; n < 32; n++)
		{
			handles[n] = (char*)0;
			sizes[n] = 0;
		}

        	j = xrnd() % 16 + 15;
        	l = xrnd() % 80 + 7;

		for (i = s = 0; i < j && s < 256; i++)
		{
			sizes[i] = xrnd() % l + 1;
			s += sizes[i];
		}

		j = i;

		for (m = om = 1, p = 1, f = 0; m < 1000; m++) 
		{
			for (i = s = 0; i < j; i++)
				if (handles[i])
					s++;

			if (s == (unsigned)j)
				break;

			if (m / om > 10) 
			{
				p <<= 1;
				p |= 1;
			}

			for (i = 0; i < j; i++)
				if (xrnd() & p) 
				{
					if (handles[i] == (char*)0)
					{
						handles[i] = mymalloc(sizes[i]);
						//printf("malloc @ %04x size %04x\n", handles[i], sizes[i]);
						//stats();

						if (handles[i] == (char*)0)
							f++;
					}
				}

			for (i = 0; i < j; i++)
				if ((xrnd() & 1) && (handles[i] != (char*)0))
				{
					newsize = xrnd() % 0x100;
					//printf("realloc %04x of size %04x", handles[i], sizes[i]);
					handles[i] = myrealloc(handles[i], newsize);
					sizes[i] = newsize;
					//printf(" @ %04x new size %04x\n", handles[i], sizes[i]);
					//stats();

					if (handles[i] == (char*)0)
                                               	f = f + 1;
					else
					{
						myfree(handles[i]);
						//printf("free %04x\n", handles[i]);
						handles[i] = (char*)0;
						//stats();
					}
				}
		}

        	if (f != 0)
                	printf("%d malloc failures.\n", f);
		else
			printf("No malloc failures.\n");

		//printf("After allocs:\n");
		//stats();

		//printf("Free all buffers...\n");

		i = 0;
		while (i < 32)
		{
			if (handles[i] != (char*)0)
				free_new(handles[i]);

                	i = i + 1;
		}

		printf("After cleanup:\n");
		print_total_free();
		printf("--------------------------------------------------\n");
	}
}
