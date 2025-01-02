#include <stdlib.h>
#include <string.h>

void CrtOut(char);
void putstr(char*);
char *itoa(int i);
int ltofp(long x, long y);
int ComputeAngle(int xa, int ya, int xb, int yb);
int mul(int,int);
int div(int,int);
int sin(int);
int cos(int);
int arctan(int);
int fpsqrt(int);
int xdivytofp(int,int);
int neg(int);
int positive(int);
void PrintStr(char* msg);
void SetTopLeft(void);

#define BLANK 0x20
#define REV_BLANK	219
#define A_WHITE 178
#define A_BLACK 176

#define ESC 0x1B

extern char* buf;

char AS0[19] = {BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,0};

char AS1[19] = {'[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, '[', ']', A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, ']', 0};
char AS2[19] = {BLANK, '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, '[', A_WHITE, ']', A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, ']', 0};
char AS3[19] = {BLANK, '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, '[', A_WHITE, A_WHITE, ']', A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, ']', BLANK, 0};
char AS4[19] = {BLANK, BLANK, '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, '[', A_WHITE, A_WHITE, A_WHITE, ']', A_BLACK, A_BLACK, A_BLACK, A_BLACK, ']', BLANK, 0};
char AS5[19] = {BLANK, BLANK, '[', A_WHITE, A_WHITE, A_WHITE, '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', A_BLACK, A_BLACK, A_BLACK, ']', BLANK, BLANK, 0};
char AS6[19] = {BLANK, BLANK, BLANK, '[', A_WHITE, A_WHITE, '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', A_BLACK, A_BLACK, ']', BLANK, BLANK, 0};
char AS7[19] = {BLANK, BLANK, BLANK, '[', A_WHITE, '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', A_BLACK, ']', BLANK, BLANK, BLANK, 0};
char AS8[19] = {BLANK, BLANK, BLANK, BLANK, '[', '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', ']', BLANK, BLANK, BLANK, 0};

char AS9[19] = {BLANK, BLANK, BLANK, BLANK, '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', BLANK, BLANK, BLANK, BLANK, 0};

char AS10[19] = {BLANK, BLANK, BLANK, BLANK, '[', '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', ']', BLANK, BLANK, BLANK, 0};
char AS11[19] = {BLANK, BLANK, BLANK, '[', A_BLACK, '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', A_WHITE, ']', BLANK, BLANK, BLANK, 0};
char AS12[19] = {BLANK, BLANK, BLANK, '[', A_BLACK, A_BLACK, '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', A_WHITE, A_WHITE, ']', BLANK, BLANK, 0};
char AS13[19] = {BLANK, BLANK, '[', A_BLACK, A_BLACK, A_BLACK, '[', A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', A_WHITE, A_WHITE, A_WHITE, ']', BLANK, BLANK, 0};
char AS14[19] = {BLANK, BLANK, '[', A_BLACK, A_BLACK, A_BLACK, A_BLACK, '[', A_WHITE, A_WHITE, A_WHITE, ']', A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', BLANK, 0};
char AS15[19] = {BLANK, '[', A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, '[', A_WHITE, A_WHITE, ']', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', BLANK, 0};
char AS16[19] = {BLANK, '[', A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, '[', A_WHITE, ']', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', 0};
char AS17[19] = {'[', A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, A_BLACK, '[', ']', A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, A_WHITE, ']', 0};

int Dist(int X0, int Y0, int X1, int Y1, int xp, int yp)
{
	int a,b,c,f;
	long ff;
	long tmp;
	int dd;

	a = Y0 - Y1;
	b = X1 - X0;
	c = X0 * Y1 - X1 * Y0;

	f = abs(a * xp + b * yp + c);

	if (f == 0)
		return 0;

	ff = (long)f*(long)f;
	tmp = (long)a*(long)a + (long)b*(long)b;

	if (tmp == 0)
		return 0x200;

	if ((ff / tmp) >= 4)
		return 0x200;

	dd = ltofp(ff, tmp);

	if (dd == 0)
		return 0;

	if (ComputeAngle(X0 << 8, Y0 << 8, X1 << 8, Y1 << 8) > ComputeAngle(X0 << 8, Y0 << 8, xp << 8, yp << 8))
		return fpsqrt(dd);
	else
		return neg(fpsqrt(dd));
}

void PrintAimStatus(int d)
{
	int dabs = positive(d);

	SetTopLeft();
	
	if (d == 0x200)
		PrintStr(AS0);
	else if (d > 0)
	{
		if (d > 0x1C0)
			PrintStr(AS1);
		else if (d > 0x180)
			PrintStr(AS2);
		else if (d > 0x140)
			PrintStr(AS3);
		else if (d > 0x120)
			PrintStr(AS4);
		else if (d > 0x100)
			PrintStr(AS5);
		else if (d > 0x0C0)
			PrintStr(AS6);
		else if (d > 0x080)
			PrintStr(AS7);
		else if (d > 0x040)
			PrintStr(AS8);
		else 
			PrintStr(AS9);
	}
	else
	{
		if (dabs <= 0x040)
			PrintStr(AS9);
		else if (dabs <= 0x080)
			PrintStr(AS10);
		else if (dabs <= 0x0C0)
			PrintStr(AS11);
		else if (dabs <= 0x100)
			PrintStr(AS12);
		else if (dabs <= 0x120)
			PrintStr(AS13);
		else if (dabs <= 0x140)
			PrintStr(AS14);
		else if (dabs <= 0x180)
			PrintStr(AS15);
		else if (dabs <= 0x1C0)
			PrintStr(AS16);
		else 
			PrintStr(AS17);
	}
}

