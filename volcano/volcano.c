//	Ladislau Szilagyi, Jan 2025

#include "stdio.h"
#include "string.h"

#define bool char
#define TRUE 1
#define FALSE 0

#define int_part(x) (x >> 8)
#define f_part(x) (x & 0xFF)

#define COLS	80
#define ROWS	30

#define PI 0x324
#define hPI 0x192
#define qPI 0xC9
#define oPI 0X64

#define DOWN_LEFT	191
#define DOWN_RIGHT	218
#define UP_LEFT		217
#define UP_RIGHT	192	

#define ESC 0x1B

#define BLANK	' '
#define ROCK	'*'
#define VOLCANO '#'
#define ME	'@'

#define ROW_VTOP	20
#define COL_VTOP	27

#define	ROCK_COUNT	10

void xrndseed(void);
unsigned int xrnd(void);

int mul(int,int);
int div(int,int);
int sin(int);
int cos(int);
int arctan(int);
int fpsqrt(int);
int xdivytofp(int,int);
int neg(int);
int positive(int);

void CrtSetup(void);
void CrtOut(char);
char CrtIn(void);
bool CrtSts(void);
void putstr(char*);

char Screen[ROWS][COLS];

int Angles[8] = 
{
	PI,
	hPI + qPI + oPI,
	hPI + qPI,
	hPI + oPI,
	qPI + oPI,
	qPI,
	oPI,
	0
};

int g;	//9.8
	
// VT100

char intbuf[6];

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

void SetTopLeft(void)
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
	putstr(itoa(ROWS-y));
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
	SetTopLeft();
	CrtOut(ESC);
	CrtOut('[');
	CrtOut('K');
}

// print on the first line
void PrintStr(char* msg)
{
	SetTopLeft();
	putstr(msg);
}

void PrintChar(int y, int x, char ch)
{
	SetYX(y, x);
	CrtOut(ch);
}

void InitScreen(void)
{
	int x, y, n, m;

	Clear();

	for (x = 0; x < COLS; x++)
		for (y = 0; y < ROWS; y++)
			Screen[y][x] = BLANK;

	for (x = COL_VTOP; x < COL_VTOP+ROCK_COUNT; x++)
	{
		PrintChar(ROW_VTOP, x, VOLCANO);
		Screen[ROW_VTOP][x] = VOLCANO;
	}

	for (n = 1; n < 17; n++)
	{
		PrintChar(ROW_VTOP-n, COL_VTOP-n, VOLCANO);
		Screen[ROW_VTOP-n][COL_VTOP-n] = VOLCANO;
		m = COL_VTOP+ROCK_COUNT+2*(n-1);
		PrintChar(ROW_VTOP-n, m, VOLCANO);
		Screen[ROW_VTOP-n][m] = VOLCANO;
	}

	PrintChar(0, 32, ME);
	Screen[0][32] = ME;
}

void show_rock(int x, int y)
{
	if (Screen[y][x] == BLANK)
	{
		PrintChar(y, x, ROCK);
		Screen[y][x] = ROCK;
	}
}

void hide_rock(int x, int y)
{
	if (Screen[y][x] == ROCK)
	{
		PrintChar(y, x, BLANK);
		Screen[y][x] = BLANK;
	}
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

struct _rock {
	int xstart;
	int ystart;
	int xold;
	int yold;
	int x;
	int y;
	int speed;
	int angle;
	int time;
	bool stopped;
};

struct _rock Rocks[ROCK_COUNT];

void InitRocks(void)
{
	int n;

	for (n=0; n<ROCK_COUNT; n++)
	{
		Rocks[n].xstart = Rocks[n].xold = (COL_VTOP+n) << 9;
		Rocks[n].ystart = Rocks[n].yold = (ROW_VTOP+1) << 9;
		Rocks[n].speed = (xrnd() & 0xF) + 1;
		Rocks[n].angle = Angles[xrnd() & 7];
		Rocks[n].time = 0;
		Rocks[n].stopped = FALSE;
	}
}

int main(void)
{
	char ch;
	int n;
	int col, row, oldcol, oldrow;
	int COL = 32;
	int CNT = 5000;
	int rocks = ROCK_COUNT;

	g = 0x900 + xdivytofp(8, 10);	//9.8

	xrndseed();
	CrtSetup();

	printf("\nYou ( @ ) are at the foot of a volcano in full eruption.\n");
	printf("The eruption will last about 5 minutes.\n");
	printf("Try to avoid the stones thrown by the volcano, using 's'=left and 'd'=right.\n");
	printf("You can quit anytime by hitting 'x'.\n");
	printf("This game can be played only using a screen with at least 30 rows...\n");
	printf("Hit any key to start the game ...\n");

	CrtIn();

	CursorOff();
	InitScreen();
	InitRocks();

	do
	{
		for (n = 0; n < ROCK_COUNT; n++)
		{
			if (Rocks[n].stopped)
				continue;

			if (CrtSts())
			{
				ch = CrtIn();

				if (ch == 's' && COL > 8)
				{
					PrintChar(0, COL, BLANK);
					COL--;
					PrintChar(0, COL, ME);
				}
				else if (ch == 'd' && COL < 56)
				{
					PrintChar(0, COL, BLANK);
					COL++;
					PrintChar(0, COL, ME);
				}
				else if (ch == 'x')
				{
					CursorOn();
					Clear();
					exit(0);
				}
			}

			Rocks[n].time += 0x0008;

			Rocks[n].x = Rocks[n].xstart + mult(Rocks[n].speed, mul(Rocks[n].time, cos(Rocks[n].angle)));
			Rocks[n].y = Rocks[n].ystart + ( ( mul(Rocks[n].time, ( mult(Rocks[n].speed, sin(Rocks[n].angle)) << 1) - mul(g, Rocks[n].time) ) ) >> 1 );

			col = int_part(Rocks[n].x) >> 1;
			row = int_part(Rocks[n].y) >> 1;
			oldcol = int_part(Rocks[n].xold) >> 1;
			oldrow = int_part(Rocks[n].yold) >> 1;

			if (col != oldcol || row != oldrow)
			{
				hide_rock(oldcol, oldrow);

				if (col < 0 || col > 63 || row < 0)
				{
					if (CNT <= 0)
					{
						Rocks[n].stopped = TRUE;
						rocks--;
						continue;
					}

					Rocks[n].xstart = Rocks[n].xold = (COL_VTOP+n) << 9;
					Rocks[n].ystart = Rocks[n].yold = (ROW_VTOP+1) << 9;
					Rocks[n].speed = (xrnd() & 0xF) + 1;
					Rocks[n].angle = Angles[xrnd() & 7];
					Rocks[n].time = 0;
					Rocks[n].stopped = FALSE;
					continue;
				}

				// check impact...
				if (COL == col && row < 4)
				{
					ClearTopLine();
					PrintStr("Unfortunately, you were hit by a stone, sorry! Hit any key to exit...");
					CrtIn();
					Clear();
					CursorOn();
					exit(0);
				}

				show_rock(col, row);
			}

			Rocks[n].xold = Rocks[n].x;
			Rocks[n].yold = Rocks[n].y;
		}
	
		CNT--;
	} while (rocks > 0);

	ClearTopLine();
	PrintStr("Congratulations, you managed to avoid being hit! Press any key to exit...");
	CrtIn();
	Clear();
	CursorOn();
	exit(0);
}

