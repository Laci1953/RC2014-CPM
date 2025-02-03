// Game of BOUNCE for VT100 compatible terminals
//
// Ladislau Szilagyi, Feb 2025
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define bool char
#define TRUE 1
#define FALSE 0

#define int_part(x) (x >> 8)
#define f_part(x) (x & 0xFF)

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
bool CrtSts(void);
void putstr(char*);

char buf[80];

#define REV_BLANK	219

#define DOWN_LEFT	191
#define DOWN_RIGHT	218
#define UP_LEFT		217
#define UP_RIGHT	192	

#define BALL		'o'
#define TARGET		'@'
#define BLANK		' '

bool Direction, DirectionInit;	// 1 = left to right
int Len, LenInit;		// size of start line
int TargetCol, TargetColInit;	// column of @

bool Dropping;	// 1 = yes, 0 = no

int xstart, ystart;
int x, y, xold, yold;
int col, row, colold, rowold;
int colstart;

int vx, vy;
int tx, ty;

int g, half_g;

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

// ESC [ line ; col H , where line = 0..., col = 0..., (0,0) = top-left
void PrintChar(int y, int x, char ch)
{
	SetYX(y, x);
	CrtOut(ch);
}

void Rand(void)
{
	DirectionInit = (bool)(xrnd() & 1);
	LenInit = (xrnd() % 30) + 1;
	TargetColInit = (xrnd() % 20) + 21;
}

void InitScreen(void)
{
	int i;

	for (i = 0; i < 64; i++)
		PrintChar(46, i, REV_BLANK);

	for (i = 0; i < 64; i++)
		PrintChar(0, i, REV_BLANK);

	for (i = 1; i <= 45; i++)
		PrintChar(i, 0, REV_BLANK);

	for (i = 1; i <= 45; i++)
		PrintChar(i, 63, REV_BLANK);

	for (i = 1; i <= LenInit; i++)
	{
		if (DirectionInit)
			PrintChar(44, i, REV_BLANK);
		else
			PrintChar(44, 63-i, REV_BLANK);
	}

	PrintChar(45, DirectionInit ? 1 : 62, BALL);

	PrintChar(0, TargetColInit, TARGET);
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

void move(void)
{
	int tmp;

	tx += 0x0001;
	ty += 0x0001;

	y = ystart - mul(mul(half_g, ty), ty);

	if (!Dropping)
		y += mult(vy, ty);

	tmp = mult(vx, tx);

	if (Direction)
		x = xstart + tmp;
	else
		x = xstart - tmp;
}

int sqrt(int n) {
    
    int res = 1;
    while(res*res <= n)
        res++;
    
    return res - 1;
}

int main(void)
{
	int tmp;

	g = 0x900 + xdivytofp(8, 10);	//9.8
	half_g = g >> 1;

	xrndseed();
	CrtSetup();
	CursorOff();

	printf("\nYou are in a very high room.\n");
	printf("Throwing a ball ( o ), you must touch a marked point on the floor ( @ ).\n");
	printf("The ball will bounce back hitting the side walls and the floor.\n");
	printf("Please extend the size of your terminal screen to 48 rows...\n");
	printf("Then hit any key to start...\n");
	CrtIn();

	Rand();

again:

	Clear();
	InitScreen();

	Direction = DirectionInit;
	Len = LenInit;
	TargetCol = TargetColInit;

	Dropping = TRUE;

	do
	{
		ClearTopLine();
		PrintStr("Ball speed? (1=smallest, 30=highest):");
		cgets(buf);
		vx = atoi(buf);
	}
	while (vx == 0 || vx > 30);

	xstart = xold = Direction ? 0x300 : (125 << 8);
	col = colold = Direction ? 1 : 62;
	colstart = Direction ? (1 + Len) : (62 - Len);

	row = rowold = 45;
	tx = 0;

	ClearTopLine();
	PrintStr("You can quit anytime hitting 'q'");

	do
	{
		tx += 0x0001;

		tmp = mult(vx, tx);

		if (Direction)
		{
			x = xstart + tmp;
		}
		else
		{
			x = xstart - tmp;
		}

		col = int_part(x) >> 1;

		if (col != colold)
		{
			PrintChar(row, colold, BLANK);
			PrintChar(row, col, BALL);

			if (col == colstart)
				break;
			else
				colold = col;
		}

		if (CrtSts() && CrtIn() == 'q')
		{
			CursorOn();
			Clear();
			exit(0);
		}
	} while (TRUE);

	colold = colstart;
	xstart = xold = ((col << 1) + 1) << 8;
	ystart = yold = ((row << 1) + 1) << 8;

	tx = ty = 0;

	vy = sqrt(10 * (int_part(ystart) + int_part(ystart))) - 5;	//used when touching the floor

	do
	{
		move();

		col = int_part(x) >> 1;
		row = int_part(y) >> 1;

		if (col == 0)
		{
			xstart = xold;
			tx = 0;
			Direction = TRUE;
			vx -= 1;

			if (vx <= 0)
			{
				ClearTopLine();
				PrintStr("Sorry, you missed the target! Do you want to retry? (y=YES):");

				if (CrtIn() == 'y')
					goto again;
				else
				{
					CursorOn();
					Clear();
					exit(0);
				}
			}

			continue;
		}

		if (col == 63)
		{
			xstart = xold;
			tx = 0;
			Direction = FALSE;
			vx -= 1;

			if (vx <= 0)
			{
				ClearTopLine();
				PrintStr("Sorry, you missed the target! Do you want to retry? (y=YES):");

				if (CrtIn() == 'y')
					goto again;
				else
				{
					CursorOn();
					Clear();
					exit(0);
				}
			}

			continue;
		}

		if (row <= 0)
		{
			if (col == TargetCol)
			{
				ClearTopLine();
				PrintStr("Congratulations, you hit the target!");
				CursorOn();
				exit(0);
			}
	
			if (vy <= 0)
			{
				ClearTopLine();
				PrintStr("Sorry, you missed the target! Do you want to retry? (y=YES):");

				if (CrtIn() == 'y')
					goto again;
				else
				{
					CursorOn();
					Clear();
					exit(0);
				}
			}

			// bounce back from floor, vy already computed !
			ystart = yold;
			ty = 0;
			Dropping = FALSE;
			continue; 
		}

		if (!Dropping && y < yold)
		{
			if ((int_part(yold) >> 1) <= 2)
			{
				ClearTopLine();
				PrintStr("Sorry, you missed the target! Do you want to retry? (y=YES):");

				if (CrtIn() == 'y')
					goto again;
				else
				{
					CursorOn();
					Clear();
					exit(0);
				}
			}

			// start dropping ...
			ystart = yold;
			vy = sqrt(10 * (int_part(yold) + int_part(yold))) - 5;	//used when touching the floor

			Dropping = TRUE;
			ty = 0;
			continue;
		}

		if (col != colold || row != rowold)
		{
			PrintChar(rowold, colold, BLANK);
			PrintChar(row, col, BALL);
			colold = col;
			rowold = row;
		}

		xold = x;
		yold = y;

		if (CrtSts() && CrtIn() == 'q')
			break;
	} while (TRUE);

	CursorOn();
	Clear();
}


