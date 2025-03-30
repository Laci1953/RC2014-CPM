#include <stdio.h>

void main(void)
{
	FILE* f;

	f = fopen("TEST.TXT", "w+");
	fputs("test", f);

	/* the file is NOT closed before exiting */
}
