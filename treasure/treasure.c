// Game of TREASURE for VT100 compatible terminals
//
// Ladislau Szilagyi, December 2024
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
char CrtIn(void);
void CrtOut(char);
void putstr(char*);

void xrndseed(void);
unsigned int xrnd(void);

#define BLANK		' '
#define OBSTACLE	'O'

#define DOWN_LEFT	191
#define DOWN_RIGHT	218
#define UP_LEFT		217
#define UP_RIGHT	192	

#define halfPI 0x192
#define PI 0x324
#define twoPI 0x648
#define threePI 0x96C

#define int_part(x) (x >> 8)
#define f_part(x) (x & 0xFF)

#define OBST_COUNT	256

#define MONEY_COUNT	10

char intbuf[6];
char buf[80];

char Screen[48][64];
char ScreenCopy[48][64];

int Money[10];
int MoneyCount = 0;
int Remaining;

int PlayerInit, Player;

int rowCH, colCH;
char chold;

// count = integer
int mult(int count, int fpval)
{
        int ret=0;

        do
		ret += fpval;
        while (--count);

        return ret;
}

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
	putstr(itoa(49-y));
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

void InitScreen(void)
{
	int x, y;

	for (y = 0; y < 48; y++)
		for (x = 0; x < 64; x++)
			Screen[y][x] = BLANK;
}

void InitObjects(void)
{
	unsigned int i, n;
	int row, col;
	char found;

	for (i = 0; i < OBST_COUNT; i++)
	{
		n = xrnd();
		n = n >> 3;
		row = n >> 6;

		if (row < 48)
		{
			col = n & 0x3F;
			Screen[row][col] = OBSTACLE;
		}
	}

	for (i = 0; i < MONEY_COUNT; i++)
	{
		n = xrnd();
		n = n >> 3;
		row = n >> 6;

		if (row < 48)
		{
			col = n & 0x3F;
			Screen[row][col] = '$';
			Money[MoneyCount++] = (row << 8) + col;
		}
	}
	
	Remaining = MoneyCount;

	do
	{
		n = xrnd();
		n = n >> 3;
		row = n >> 6;

		if (row < 48)
		{
			col = n & 0x3F;

			for (i = 0, found = 0; i < MoneyCount; i++)
				if (Money[i] == (row << 8) + col)
					found = 1;			

			if (found == 0)
			{
				Screen[row][col] = '*';
				PlayerInit = (row << 8) + col;
				break;
			}
		}
	} while (1);
}

void SaveScreen(void)
{
	int row, col;

	for (row = 0; row < 48; row++)
		for (col = 0; col < 64; col++)
			ScreenCopy[row][col] = Screen[row][col];
}

void RestoreScreen(void)
{
	int row, col;

	for (row = 0; row < 48; row++)
		for (col = 0; col < 64; col++)
			Screen[row][col] = ScreenCopy[row][col];
}

void PrintScreen(void)
{
	int row, col;

	for (row = 0; row < 48; row++)
	{
		SetYX(row, 0);
		for (col = 0; col < 64; col++)
			CrtOut(Screen[row][col]);
	}
}

//x,y fp
char GetChar(char ball, int y, int x)
{
	int row = int_part(y);
	int col = int_part(x);

	int _row = row % 2;
	int _col = col % 2;

	if (_col == 0)
	{
		if (_row == 0)
		{
			if (f_part(x) >= 0x80 && f_part(y) >= 0x80)
				return ball;
			else
				return DOWN_LEFT;
		}
		else
		{
			if (f_part(x) >= 0x80 && f_part(y) <= 0x80)
				return ball;
			else
				return UP_LEFT;
		}
	}
	else
	{
		if (_row == 0)
		{
			if (f_part(x) <= 0x80 && f_part(y) >= 0x80)
				return ball;
			else
				return DOWN_RIGHT;
		}
		else
		{
			if (f_part(x) <= 0x80 && f_part(y) <= 0x80)
				return ball;
			else
				return UP_RIGHT;
		}
	}
}

void short_wait(void)
{
	int i;

	for (i=0; i<2000; i++) 
		;
}

void blink_cell(char cell, int row, int col)
{
	int i = 5;

	do
	{
		short_wait();short_wait();short_wait();short_wait();short_wait();
		PrintChar(row, col, BLANK);
		short_wait();short_wait();short_wait();short_wait();short_wait();
		PrintChar(row, col, cell);
	}
	while (--i);
}

//
// compute angle for vector [(xa,ya),(xb,yb)]
//
int ComputeAngle(int xa, int ya, int xb, int yb)	//fixed point
{
	int tan;
	int angle;

	if (yb == ya)
	{
		if (xb > xa)
			angle = 0;
		else
			angle = PI;
	}
	else if (xb == xa)
	{
		if (yb > ya)
			angle = halfPI;
		else
			angle = PI + halfPI;
	}
	else
	{
		tan = div(yb - ya, xb - xa);

		if (tan < 0)
			tan = neg(tan);

		angle = arctan(tan);

		if (xb < xa)
		{
			if (yb < ya)
				angle = angle + PI;
			else
				angle = PI - angle;
		}
		else
		{
			if (yb < ya)
				angle = twoPI - angle;
		}
	}

	return angle;
}

//returns 1 if CR was hit
char MoveCH(char dir)
{
	switch (dir)
	{
	case 'x':	
			if (rowCH > 0)
			{
				PrintChar(rowCH, colCH, chold);
				rowCH--;
			}
			break;
	case 'e':	
			if (rowCH < 47)
			{
				PrintChar(rowCH, colCH, chold);
				rowCH++;
			}
			break;
	case 's':	
			if (colCH > 0)
			{
				PrintChar(rowCH, colCH, chold);
				colCH--;
			}
			break;
	case 'd':	
			if (colCH < 63)
			{
				PrintChar(rowCH, colCH, chold);
				colCH++;
			}
			break;
	default:	
			break;
	}

	chold = Screen[rowCH][colCH];
	PrintChar(rowCH, colCH, '+');
	
	return (dir == 0xD) ? 1 : 0;
}

void SetCH(void)
{
	char ch;

	rowCH = (Player >> 8) < 47 ? (Player >> 8) + 1 : (Player >> 8) - 1;
	colCH = (Player & 0xFF) < 63 ? (Player & 0xFF) + 1 : (Player & 0xFF) - 1;

	chold = Screen[rowCH][colCH];
	PrintChar(rowCH, colCH, '+');

	ClearTopLine();
	PrintStr("Aim (s=left,d=right,x=down,e=up) then hit <cr>");
	
	ch = CrtIn();
	
	do
	{
		if (MoveCH(ch))
			break;
		
		ch = CrtIn();
	}
	while (1);

	PrintChar(rowCH, colCH, chold);
}

void Path(int col_start, int row_start, int col_stop, int row_stop)
{
	int incr;
	int x, y;
	int xstart, ystart, xstop, ystop;
	int Sin, Cos;
	int angle;
	int col, row, col_old, row_old;

	xstart = ((col_start << 1) + 1) << 8;
	ystart = ((row_start << 1) + 1) << 8;

	xstop = ((col_stop << 1) + 1) << 8;
	ystop = ((row_stop << 1) + 1) << 8;

	angle = ComputeAngle(xstart, ystart, xstop, ystop);

	Sin = sin(angle);
	Cos = cos(angle);

	incr = 1;

	col_old = col_start;
	row_old = row_start;

	do
	{
                // compute crt coord, using the start coord, the crt distance and the crt angle
                x = xstart + mult(incr, Cos);
                y = ystart + mult(incr, Sin);

		col = int_part(x) >> 1;
		row = int_part(y) >> 1;

              	if (col != col_old || row != row_old)
		{
			if (Screen[row][col] == '$') 
				return; // ball hit $

			Screen[row][col] = BLANK;

			col_old = col;
			row_old = row;
		}

		// continue moving
		incr++;
	} while(1);
}

// return #0 : coord(row,col) of hit $, 0 : hit obstacle or margin
int Move(int col_start, int row_start, int col_stop, int row_stop)
{
	int incr;
	int x, y, xold, yold;
	int xmove, ymove;
	int xstart, ystart, xstop, ystop;
	int Sin, Cos;
	int angle;
	int col, row;

	xold = xstart = ((col_start << 1) + 1) << 8;
	yold = ystart = ((row_start << 1) + 1) << 8;

	xstop = ((col_stop << 1) + 1) << 8;
	ystop = ((row_stop << 1) + 1) << 8;

	angle = ComputeAngle(xstart, ystart, xstop, ystop);

	Sin = sin(angle);
	Cos = cos(angle);

	incr = 1;

	do
	{
		short_wait();

                // compute crt coord, using the start coord, the crt distance and the crt angle
                x = xstart + mult(incr, Cos);
                y = ystart + mult(incr, Sin);

		if (int_part(x) != int_part(xold) || int_part(y) != int_part(yold))
                {
			PrintChar(int_part(yold) >> 1, int_part(xold) >> 1, BLANK);

       			xmove = int_part(x) - int_part(xold);
			ymove = int_part(y) - int_part(yold);

               		// handle hitting the boundaries
               		if (xmove > 0 && int_part(x) >= 128)
				return 0;
			else if (xmove < 0 && int_part(x) <= 0)
				return 0;
            		else if (ymove < 0 && int_part(y) <= 0)
				return 0;
              		else if (ymove > 0 && int_part(y) >= 96)
				return 0;

			col = int_part(x) >> 1;
			row = int_part(y) >> 1;

			if ((unsigned int)Screen[row][col] == (unsigned int)OBSTACLE)
				return 0;
				
			if (Screen[row][col] == '$')
			{
				blink_cell('$', row, col);
				Screen[row][col] = '*';
				PrintChar(row, col, '*');
				return ((row << 8) + col); // ball hit $
			}

			PrintChar(row, col, GetChar('*', y, x));
		}

		// continue moving
		xold = x;
		yold = y;
		incr++;
	} while(1);
}

void main(void)
{
	char ch;
	int n;

printf("The 'treasure' game\r\n\n");
printf("You ('*') are in a mine full of very deep pits ('O').\r\n");
printf("It is told that in this mine, someone has wasted many precious coins ('$').\r\n");
printf("You have to collect all these coins.\r\n");
printf("Move from coin to coin, avoiding falling into one of the pits.\r\n");
printf("Expand the window dimensions to at least 48 rows x 80 columns.\r\n");
printf("Hit any key when ready...");

	xrndseed();
	CrtSetup();

	CrtIn();

	CursorOff();
	Clear();

	InitScreen();
	InitObjects();

	Path(PlayerInit & 0xFF, PlayerInit >> 8, Money[0] & 0xFF, Money[0] >> 8);

	for (n = 1; n < MoneyCount; n++)
		Path(Money[n-1] & 0xFF, Money[n-1] >> 8, Money[n] & 0xFF, Money[n] >> 8); 

	SaveScreen();

	PrintScreen();

	Player = PlayerInit;

	blink_cell('*', Player >> 8, Player & 0xFF);

	n = 0;

	for (n = 0; n < MoneyCount; n++)
	{
		SetCH();

		Player = Move(Player & 0xFF, Player >> 8, colCH, rowCH);

		if (Player == 0)
		{
			ClearTopLine();
			PrintStr("Sorry, you missed! Do you want to see the correct path? (y=YES):");
			ch = CrtIn();

			if (ch == 'y')
			{
				RestoreScreen();
				PrintScreen();

				Move(PlayerInit & 0xFF, PlayerInit >> 8, Money[0] & 0xFF, Money[0] >> 8);

				for (n = 1; n < MoneyCount; n++)
					Move(Money[n-1] & 0xFF, Money[n-1] >> 8, Money[n] & 0xFF, Money[n] >> 8); 
			}

			Clear();
			CursorOn();
			return;
		}
		else
		{
			Remaining--;
			ClearTopLine();
			sprintf(buf, "Well done, %d coins remaining... Hit any key...", Remaining);
			PrintStr(buf);
			CrtIn();
		}
	}

	ClearTopLine();
	PrintStr("Congratulations, you found all the coins! Hit any key...");
	CrtIn();

	Clear();
	CursorOn();
}
