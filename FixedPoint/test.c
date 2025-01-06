#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int mul(int,int);
int sin(int);
int cos(int);

int halfPI =  0x192;     //fp    PI/2

void main(void)
{
	int x;

	for (x = 0x001; x < halfPI; x += 0x001)
	{
		printf("x=%04x sin(x)=%04x cos(x)=%04x sin(x)^2 + cos(x)^2 = %04x\r\n", x, sin(x), cos(x), mul(sin(x), sin(x)) + mul(cos(x), cos(x)));
	}
}
