// Game of ARROW for VT100 compatible terminals
//
// Ladislau Szilagyi, October 2024
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

int mul(int,int);
int div(int,int);
int sin(int);
int cos(int);
int arctan(int);
int fpsqrt(int);
int xdivytofp(int,int);
int neg(int);
int positive(int);

void xrndseed(void);
unsigned int xrnd(void);

void CrtSetup(void);
char CrtIn(void);
void CrtOut(char);
void putstr(char*);

#define REV_BLANK	219

#define DOWN_LEFT	191
#define DOWN_RIGHT	218
#define UP_LEFT		217
#define UP_RIGHT	192	

#define int_part(x) (x >> 8)
#define char_pos(x) (x >> 9)

char intbuf[6];

char TrainingMode;

int XA, YA, XT, YT;

int XBTL, YBTL, XBTR, YBTR;
int xbtl, ybtl, xbtr, ybtr;
int XTM, YTM;
int xtm, ytm;

int X, Y, YTMP, YN;
int DELTAX, DELTAY;

int XAIM, YAIM;

int angle;
int speed;
char buf[100];

char treemap[64][48];

int xstart, ystart, xold, yold, xtmp, ytmp;
int time;
int g;

char *itoa(int i)
{
  char *p = intbuf + 5;	/* points to terminating '\0' */
  *p = 0;
  do {
    *--p = '0' + (i % 10);
    i /= 10;
  } while (i != 0);
  return p;
}

// VT100

#define ESC 0x1B

void CursorOff(void)
{
	CrtOut(ESC);
	CrtOut('[');
	CrtOut('?');
	CrtOut('2');
	CrtOut('5');
	CrtOut('l');
}

void CursorOn(void)
{
	CrtOut(ESC);
	CrtOut('[');
	CrtOut('?');
	CrtOut('2');
	CrtOut('5');
	CrtOut('h');
}

void Set00(void)
{
	CrtOut(ESC);
	CrtOut('[');
	CrtOut('1');
	CrtOut(';');
	CrtOut('1');
	CrtOut('H');
}

void SetYX(int y, int x)
{
	CrtOut(ESC);
	CrtOut('[');
	putstr(itoa(48-y));
	CrtOut(';');
	putstr(itoa(x+1));
	CrtOut('H');
}

void Clear(void)
{
	CrtOut(ESC);
	CrtOut('[');
	CrtOut('2');
	CrtOut('J');
}

void ClearTopLine(void)
{
	Set00();
	CrtOut(ESC);
	CrtOut('[');
	CrtOut('K');
}

// print on the first line
void PrintStr(char* msg)
{
	Set00();
	putstr(msg);
}

// ESC [ line ; col H , where line = 0..., col = 0..., (0,0) = top-left
void PrintChar(int y, int x, char ch)
{
	SetYX(y, x);
	CrtOut(ch);
}

void move_up(void)
{
	if (YAIM < 46)
		YAIM = YAIM + 1;
}

void move_down(void)
{
	if (YAIM > 2)
		YAIM = YAIM - 1;
}

void move_left(void)
{
	if (XAIM > XA + 1)
		XAIM = XAIM - 1; 
}

void move_right(void)
{
	if (XAIM < 62)
		XAIM = XAIM + 1;
}

//returns 1 if CR was hit
char MoveAIM(char dir)
{
	PrintChar(YAIM, XAIM, ' ');

	switch (dir)
	{
	case 'x':	move_down(); break;
	case 'e':	move_up(); break;
	case 's':	move_left(); break;
	case 'd':	move_right(); break;
	default:	break;
	}

	PrintChar(YAIM, XAIM, '+');
	
	return (dir == 0xD) ? 1 : 0;
}

int GetChar(int vRow, int vCol)
{
	
	int y = vRow % 2;
	int x = vCol % 2;

	if (x == 0)
	{
		if (y == 0)
			return DOWN_LEFT;
		else
			return UP_LEFT;
	}
	else
	{
		if (y == 0)
			return DOWN_RIGHT;
		else
			return UP_RIGHT;
	}
}

void show(int x, int y)
{
	PrintChar((y/2), (x/2), GetChar(y, x));
}

void hide(int x, int y)
{
	PrintChar((y/2), (x/2), ' ');
}

// count = integer
int mult(int count, int fpval)
{
        int ret=0;

        do
		ret += fpval;
        while (--count);

        return ret;
}

void setup(void)
{
	XA = 0 + (xrnd() % 10);
	YA = 1;

	XT = 53 + (xrnd() % 10);
	YT = 1;

	for (X = 0; X <= 63; X++)
		for (Y = 0; Y <= 47; Y++)
			treemap[X][Y] = 0;

	XBTL = 20 + (xrnd() % 10);
	xbtl = (XBTL * 2) << 8;
	YBTL = 1;
	ybtl = YBTL << 8;

	XBTR = 40 + (xrnd() % 10);
	xbtr = (XBTR * 2) << 8;
	YBTR = 1;
	ybtr = YBTR << 8;

	XTM = 30;
	xtm = (XTM * 2) << 8;
	YTM = 10 + (xrnd() % 10);
	ytm = (YTM * 2) << 8;

	// build floor
	for (X = 0; X <= 63; X++)
		PrintChar(0, X, '_');

	// build tree

	DELTAX = XBTL - XTM;
	DELTAY = YBTL - YTM;
	YTMP = YBTL;

	for (X = XBTL; X <= XTM; X++)
	{
		Y = (X * DELTAY + XBTL * YTM - XTM * YBTL) / DELTAX;
		for (YN = YTMP; YN <= Y; YN++)
		{
			PrintChar(YN, X, REV_BLANK);
			treemap[X][YN] = 1;
		}
		YTMP = Y+1;
	}

	DELTAX = XBTR - XTM;
	DELTAY = YBTR - YTM;
	YTMP = YBTR;

	for (X = XBTR; X >= XTM; X--)
	{
		Y = (X * DELTAY + XBTR * YTM - XTM * YBTR) / DELTAX;
		for (YN = YTMP; YN <= Y; YN++)
		{
			PrintChar(YN, X, REV_BLANK);
			treemap[X][YN] = 1;
		}
		YTMP = Y+1;
	}
}

int main(int argc, char** argv)
{
	char ch;

	CrtSetup();

	printf("Welcome to the archery competition!\r\n");
	printf("You have to hit an apple sitting on a table...\r\n");
	printf(", but it's not that simple, because the table is hidden behind a bushy tree.\r\n");
	printf("So, the only chance is to shoot with a vault above the tree,\r\n");
	printf(" hoping to hit the apple.\r\n");
	printf("You can train for the competition, if you add any extra parameter\r\n");
	printf(" to the command line... (e.g '>arrow t<CR>'\r\n");
	printf("Please extend the size of the screen to at least 48 rows x 80 columns\r\n");
	printf("Hit any key when ready...\r\n");
	CrtIn();

	g = 0x900 + xdivytofp(8, 10);	//9.8

	TrainingMode = (argc > 1) ? 1 : 0;

	xrndseed();

	CursorOff();
	Clear();

	setup();

again:
	PrintChar(YA, XA, '^');	//arrow
	PrintChar(YT, XT, 'O'); //apple

	XAIM = XA + 10;
	YAIM = YA + 10;
	PrintChar(YAIM, XAIM, '+');

	PrintStr("q=quit or aim (s=left,d=right,x=down,e=up) then hit <cr>");
	
	ch = CrtIn();
	
	if (ch == 'q')
		goto quit;
	
	do
	{
		if (MoveAIM(ch))
			break;
		
		if (TrainingMode)
		{
			SetYX(47, 70);
			sprintf(buf, "X=%02d Y=%02d", XAIM, YAIM);
			putstr(buf);
		}

		ch = CrtIn();
	}
	while (1);

	angle = arctan( div( (YAIM-YA) << 8, (XAIM-XA) << 8) );

	do
	{
		ClearTopLine();
		PrintStr("Arrow speed? (1=smallest, 35=highest):");
		cgets(buf);
		ClearTopLine();
		speed = atoi(buf);
	}
	while (speed == 0 || speed > 35);

	PrintChar(YAIM, XAIM, ' ');

	xstart = (XA * 2) << 8;
	ystart = (YA * 2) << 8;

	xold = xstart;
	yold = ystart;

	time = 0;

	do
	{
		time = time + 0x0001;

		xtmp = xstart + mult(speed, mul(time, cos(angle)));
		ytmp = ystart + ( ( mul(time, ( mult(speed, sin(angle)) << 1) - mul(g, time) ) ) >> 1 );

		//check apple
		if (char_pos(xtmp) == XT && char_pos(ytmp) == YT)
		{
			ClearTopLine();
			PrintStr("Congratulations, you have won! hit any key...");
			CrtIn();
			goto bye;
		}

		//check limits
		if (char_pos(xtmp) >= 63 || 
		    (ytmp > yold && char_pos(ytmp) >= 47) ||
		    (ytmp <= yold && char_pos(ytmp) <= 1))
		{
			ClearTopLine();
			PrintStr("Bad luck, you have missed! hit any key...");
			CrtIn();
			goto quit;
		}

		//check tree impacted
		if (treemap[char_pos(xtmp)][char_pos(ytmp)])
		{
			ClearTopLine();
			PrintStr("Bad luck, your arrow hit the tree! hit any key...");
			CrtIn();
			goto quit;
		}

		hide(int_part(xold), int_part(yold));
		xold = xtmp;
		yold = ytmp;
		show(int_part(xtmp), int_part(ytmp));
	} while (1);
quit:
	if (TrainingMode)
	{
		hide(int_part(xold), int_part(yold));
		ClearTopLine();
		PrintStr("Try again? (y/n):");
		if (CrtIn() == 'y')
			goto again;
	}
bye:
	CursorOn();
	Clear();
	exit(0);
}

	

