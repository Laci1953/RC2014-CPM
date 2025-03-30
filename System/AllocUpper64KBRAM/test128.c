#include <stdio.h>
#include <string.h>
#include "dynm128.h"

char str[32] = "test\r\n";

void WriteReadPrint(char* p, char type)
{
	char	buf[32];

/* store text */

	if (type)
		PutString(str, p);
	else
		strcpy(p, str);

/* retrieve text */

	if (type)
		GetString(buf, p);
	else
		strcpy(buf, p);

/* print text */

	if (type)
		printf("In upper 64KB RAM: ");
	else
		printf("In lower 64KB RAM: ");

	printf(buf);
}

void main(void)
{
	char*	p;
	char	type;	/* 0 : in lower RAM, 1 : in upper RAM */

	InitDynM();

	p = Alloc128(10, &type);	/* will be allocated in the lower 64 KB RAM */

	WriteReadPrint(p, type);

	p = Alloc128(0xE000, &type);	/* will be allocated in the upper 64 KB RAM */

	WriteReadPrint(p, type);
}
