#	Compile with -dNEW for the new alloc:
#		C -v -o -dNEW test.c newalloc.as xrnd.as
#
#	else use the original alloc
#		C -v -o test.c xrnd.as
#
#	newalloc.as must be assembled using Z80AS,
#		therefore substitute ZAS.COM with Z80AS.COM before compiling
#
#	if your CCP.COM does not support //comments, replace them with /* comments */

#include "stdlib.h"
#include "stdio.h"

#ifdef NEW
void init_mem(void);
void* mymalloc(int size);
void myfree(void* pbuf);
void* myrealloc(void* pbuf, int size);
#endif

unsigned int xrnd(void);
void xrndseed(void);

int handles[1024];		// allocated buffers pointers
int sizes[1024];		// buffers size
int max=0;			// max index
unsigned int size;		// current buffer size
unsigned int alloc_count;	// how many to allocate
unsigned int free_count;	// how many to free
long total_allocs=0;		// total allocs count
long total_frees=0;		// total frees count
int total_size=0;		// total size of allocated buffers

void statistics(void)
{
	int i, n;

	n = 0;
	for (i=0;i<1024;i++)
		if (handles[i])
			n++;

	printf("Total %lu alloc calls\r\n", total_allocs);
	printf("Total %lu free calls\r\n", total_frees);
	printf("Total %u bytes allocated in %u buffers\r\n", total_size, n);

	exit(0);
}

unsigned int random(int limit)
{
	return xrnd() % limit + 1;
}

void store_handle(int p, int size)
{
	int i;

	for (i = 0; i<1024; i++)
		if (handles[i] == 0)
		{
			handles[i] = p;
			sizes[i] = size;

			if (i > max)
				max = i;

			return;
		}

	printf("No more free handles!");
	statistics();
}

void main(void)
{
	int pbuf;
	int n, m;

#ifdef NEW
	init_mem();
#endif

	xrndseed();

	for (n=0; n<1024; n++)
		handles[n] = 0;

	do
	{
		alloc_count = random(100);

		for (n=0; n<alloc_count; n++)
		{
			size = random(128);
#ifdef NEW
			pbuf = (int)mymalloc(size);
#else
			pbuf = (int)malloc(size);
#endif
			if (!pbuf)	// if no more free memory, print statistics & exit
				statistics();

			//uncomment to see allocated buffers
			//printf("allocated @ %04xH %u bytes\r\n", pbuf, size);

			total_allocs++;
			total_size += size;
			store_handle(pbuf, size);
		}

		free_count = random(alloc_count);	//free some of previously allocated

		for (n=0; n<free_count; n++)
		{
			m = random(max);

			if (handles[m])
			{
#ifdef NEW
				myfree((void*)handles[m]);
#else
				free((void*)handles[m]);		
#endif
				//uncomment to see released buffers
				//printf("freed handle %d (%04x)\r\n", m, handles[m]);

				handles[m] = 0;
				total_frees++;
				total_size -= sizes[m];
			}
			else
				free_count++;
		}
	} while (1);
}

