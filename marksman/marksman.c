// Game of MARKSMAN for VT100 compatible terminals
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

#define BLANK		0x20
#define REV_BLANK	219
#define OBSTACLE	219

#define DOWN_LEFT	191
#define DOWN_RIGHT	218
#define UP_LEFT		217
#define UP_RIGHT	192	

#define halfPI 0x192
#define PI 0x324
#define twoPI 0x648
#define threePI 0x96C

#define PI4 0xC9
#define PI8 0x64
#define PI16 0x32
#define PI32 0x19
#define PI64 0xC

#define XMIN 2
#define XMAX 126
#define YMIN 2
#define YMAX 94

#define int_part(x) (x >> 8)
#define f_part(x) (x & 0xFF)

#define OBST_COUNT	256

char intbuf[6];

char Screen[48][64];

int PlayerCoord[256];
int CompCoord[256];
int TargetsCoord[256];
int PlayerCoordCount;
int CompCoordCount;
int TargetsCoordCount;

int CompAngleInit, PlayerAngleInit;
int CompAngle, PlayerAngle;
int sinCompAngle, cosCompAngle;
int sinPlayerAngle, cosPlayerAngle;

int rowCH, colCH;
char chold;
int IndexTarget;

int AngleTab[16] = {
PI8,
PI4,
PI4 + PI8,
PI4 - PI16,
PI4 + PI8 + PI16,
PI8 + PI16,
PI4 + PI32,
PI4 + PI32 + PI64,
PI4 - PI32,
PI8 + PI32,
PI8 + PI16 + PI32,
PI4 + PI64,
PI8 + PI64,
PI4 - PI64,
PI8 + PI16 + PI32,
PI8 + PI32 + PI64
};

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

	for (y = 1; y < 47; y++)
		for (x = 1; x < 63; x++)
			Screen[y][x] = BLANK;
}

void InitBoundaries(void)
{
	int x, y;

	for (x = 0; x < 64; x++)
		Screen[47][x] = REV_BLANK;

	for (x = 0; x < 64; x++)
		Screen[0][x] = REV_BLANK;

	for (y = 1; y < 47; y++)
		Screen[y][0] = REV_BLANK;

	for (y = 1; y < 47; y++)
		Screen[y][63] = REV_BLANK;
}

void InitObstacles(void)
{
	unsigned int i, n, row, col;

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

int GetCompAngle(void)
{
	return halfPI + AngleTab[xrnd() >> 11];
}

int GetPlayerAngle(void)
{
	return AngleTab[xrnd() >> 11];
}

void ComputeTargets(void)
{
    int i, j, k;
    char already;

    TargetsCoordCount = 0;

    for (i = 0; i < PlayerCoordCount; i++)
        for (j = 0; j < CompCoordCount; j++)
        {
            if (PlayerCoord[i] == CompCoord[j])
            {
                already = 0;

                for (k = 0; k < TargetsCoordCount; k++)
                    if (PlayerCoord[i] == TargetsCoord[k])
                    {
                        already = 1;
                        break;
                    }

                if (already == 0)
                    TargetsCoord[TargetsCoordCount++] = PlayerCoord[i];
            }
        }
}

void PathPlayerBall(void)
{
	int incr;
	int x, y;
	int xold, yold;
	int xmove, ymove;
	int xstart, ystart;

	xold = xstart = 3 << 8;
	yold = ystart = 3 << 8;

	PlayerCoord[0] = 0x101;
	PlayerCoordCount = 1;

	sinPlayerAngle = sin(PlayerAngle);
	cosPlayerAngle = cos(PlayerAngle);

	incr = 1;

	do
	{
		xmove = ymove = 0;

                // compute crt coord, using the start coord, the crt distance and the crt angle
                x = xstart + mult(incr, cosPlayerAngle);
                y = ystart + mult(incr, sinPlayerAngle);

              	if (int_part(x) != int_part(xold) || int_part(y) != int_part(yold))
                {
       			xmove = int_part(x) - int_part(xold);
			ymove = int_part(y) - int_part(yold);
		}

               	// handle hitting the boundaries
               	if (xmove > 0 && int_part(x) >= XMAX)
               	{
			// compute new angle
			if (PlayerAngle < halfPI)
                      	    	PlayerAngle = PI - PlayerAngle;
			else
				PlayerAngle = threePI - PlayerAngle;

			xstart = xold;
			ystart = yold;
			sinPlayerAngle = sin(PlayerAngle);
			cosPlayerAngle = cos(PlayerAngle);
			incr = 1;
                }
		else if (xmove < 0 && int_part(x) <= XMIN)
		{
                       	// compute new angle
                       	if (PlayerAngle > PI)
                                PlayerAngle = threePI - PlayerAngle;
                        else
                                PlayerAngle = PI - PlayerAngle;

			xstart = xold;
			ystart = yold;
			sinPlayerAngle = sin(PlayerAngle);
			cosPlayerAngle = cos(PlayerAngle);
			incr = 1;
               	}
            	else if (ymove < 0 && int_part(y) <= YMIN)
                {
                      	// compute new angle
                        PlayerAngle = twoPI - PlayerAngle;

			xstart = xold;
			ystart = yold;
			sinPlayerAngle = sin(PlayerAngle);
			cosPlayerAngle = cos(PlayerAngle);
			incr = 1;
              	}
              	else if (ymove > 0 && int_part(y) >= YMAX)
                {
                       	// compute new angle
                        PlayerAngle = twoPI - PlayerAngle;

			xstart = xold;
			ystart = yold;
			sinPlayerAngle = sin(PlayerAngle);
			cosPlayerAngle = cos(PlayerAngle);
			incr = 1;
               	}
               	else if (xmove != 0 || ymove != 0)
		{	// ball moved
			// ... to another char?
			if (((int_part(y) >> 1) << 8) + (int_part(x) >> 1) != PlayerCoord[PlayerCoordCount - 1])
			{
				// check for cycles
				if (((int_part(y) >> 1) << 8) + (int_part(x) >> 1) == PlayerCoord[0] )
					return;

				// save new coord
				PlayerCoord[PlayerCoordCount++] = ((int_part(y) >> 1) << 8) + (int_part(x) >> 1);

				// check for overflow
				if (PlayerCoordCount == 256)
					return;
			}
		}

		// continue moving
		xold = x;
		yold = y;
		incr++;
	} while(1);
}

void PathCompBall(void)
{
	int incr;
	int x, y;
	int xold, yold;
	int xmove, ymove;
	int xstart, ystart;

	xold = xstart = 125 << 8;
	yold = ystart = 3 << 8;

	CompCoord[0] = (1 << 8) + 62;
	CompCoordCount = 1;

	sinCompAngle = sin(CompAngle);
	cosCompAngle = cos(CompAngle);

	incr = 1;

	do
	{
		xmove = ymove = 0;

                // compute crt coord, using the start coord, the crt distance and the crt angle
                x = xstart + mult(incr, cosCompAngle);
                y = ystart + mult(incr, sinCompAngle);

              	if (int_part(x) != int_part(xold) || int_part(y) != int_part(yold))
                {
       			xmove = int_part(x) - int_part(xold);
			ymove = int_part(y) - int_part(yold);
		}

               	// handle hitting the boundaries
               	if (xmove > 0 && int_part(x) >= XMAX)
               	{
			// compute new angle
			if (CompAngle < halfPI)
                      	    	CompAngle = PI - CompAngle;
			else
				CompAngle = threePI - CompAngle;

			xstart = xold;
			ystart = yold;
			sinCompAngle = sin(CompAngle);
			cosCompAngle = cos(CompAngle);
			incr = 1;
                }
		else if (xmove < 0 && int_part(x) <= XMIN)
		{
                       	// compute new angle
                       	if (CompAngle > PI)
                                CompAngle = threePI - CompAngle;
                        else
                                CompAngle = PI - CompAngle;

			xstart = xold;
			ystart = yold;
			sinCompAngle = sin(CompAngle);
			cosCompAngle = cos(CompAngle);
			incr = 1;
               	}
            	else if (ymove < 0 && int_part(y) <= YMIN)
                {
                      	// compute new angle
                        CompAngle = twoPI - CompAngle;

			xstart = xold;
			ystart = yold;
			sinCompAngle = sin(CompAngle);
			cosCompAngle = cos(CompAngle);
			incr = 1;
              	}
              	else if (ymove > 0 && int_part(y) >= YMAX)
                {
                       	// compute new angle
                        CompAngle = twoPI - CompAngle;

			xstart = xold;
			ystart = yold;
			sinCompAngle = sin(CompAngle);
			cosCompAngle = cos(CompAngle);
			incr = 1;
               	}
               	else if (xmove != 0 || ymove != 0)
		{	// ball moved
			// ... to another char?
			if (((int_part(y) >> 1) << 8) + (int_part(x) >> 1) != CompCoord[CompCoordCount - 1])
			{
				// check for cycles
				if (((int_part(y) >> 1) << 8) + (int_part(x) >> 1) == CompCoord[0] )
					return;

				// save new coord
				CompCoord[CompCoordCount++] = ((int_part(y) >> 1) << 8) + (int_part(x) >> 1);

				// check for overflow
				if (CompCoordCount == 256)
					return;
			}
		}

		// continue moving
		xold = x;
		yold = y;
		incr++;
	} while(1);
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

void show_target(int n, char ch)
{
	short_wait();short_wait();short_wait();short_wait();short_wait();
	PrintChar(TargetsCoord[n] >> 8, TargetsCoord[n] & 0xFF, ch);
}

void blink_target(int n, int i)
{
	do
	{
		show_target(n, BLANK);
		show_target(n, '@');
	}
	while (--i);
}

void MoveCompBall(void)
{
	int incr;
	int x, y;
	int xold, yold;
	int xmove, ymove;
	int xstart, ystart;

	xold = xstart = 125 << 8;
	yold = ystart = 3 << 8;

	sinCompAngle = sin(CompAngle);
	cosCompAngle = cos(CompAngle);

	incr = 1;

	do
	{
		short_wait();

		xmove = ymove = 0;

                // compute crt coord, using the start coord, the crt distance and the crt angle
                x = xstart + mult(incr, cosCompAngle);
                y = ystart + mult(incr, sinCompAngle);

              	if (int_part(x) != int_part(xold) || int_part(y) != int_part(yold))
                {
       			xmove = int_part(x) - int_part(xold);
			ymove = int_part(y) - int_part(yold);
		}

               	// handle hitting the boundaries
               	if (xmove > 0 && int_part(x) >= XMAX)
               	{
			PrintChar(int_part(yold) >> 1, int_part(xold) >> 1, BLANK);

			// compute new angle
			if (CompAngle < halfPI)
                      	    	CompAngle = PI - CompAngle;
			else
				CompAngle = threePI - CompAngle;

			xstart = xold;
			ystart = yold;
			sinCompAngle = sin(CompAngle);
			cosCompAngle = cos(CompAngle);
			incr = 1;
                }
		else if (xmove < 0 && int_part(x) <= XMIN)
		{
 			PrintChar(int_part(yold) >> 1, int_part(xold) >> 1, BLANK);

                      	// compute new angle
                       	if (CompAngle > PI)
                                CompAngle = threePI - CompAngle;
                        else
                                CompAngle = PI - CompAngle;

			xstart = xold;
			ystart = yold;
			sinCompAngle = sin(CompAngle);
			cosCompAngle = cos(CompAngle);
			incr = 1;
               	}
            	else if (ymove < 0 && int_part(y) <= YMIN)
                {
			PrintChar(int_part(yold) >> 1, int_part(xold) >> 1, BLANK);

                      	// compute new angle
                        CompAngle = twoPI - CompAngle;

			xstart = xold;
			ystart = yold;
			sinCompAngle = sin(CompAngle);
			cosCompAngle = cos(CompAngle);
			incr = 1;
              	}
              	else if (ymove > 0 && int_part(y) >= YMAX)
                {
			PrintChar(int_part(yold) >> 1, int_part(xold) >> 1, BLANK);

                       	// compute new angle
                        CompAngle = twoPI - CompAngle;

			xstart = xold;
			ystart = yold;
			sinCompAngle = sin(CompAngle);
			cosCompAngle = cos(CompAngle);
			incr = 1;
               	}
               	else if (xmove != 0 || ymove != 0)
		{	// ball moved
			if ((int_part(yold) >> 1) < 47 && (int_part(xold) >> 1) < 63)
				PrintChar(int_part(yold) >> 1, int_part(xold) >> 1, BLANK);

			if (Screen[int_part(y) >> 1][int_part(x) >> 1] == '@')
			{
				blink_target(IndexTarget, 5);
				return;
			}

			PrintChar(int_part(y) >> 1, int_part(x) >> 1, GetChar('*', y, x)); 
		}

		// continue moving
		xold = x;
		yold = y;
		incr++;
	} while(1);
}

// returns 0 if obstacle hit, 1 if target hit
char MovePlayerBall(void)
{
	int incr;
	int x, y;
	int xold, yold;
	int xmove, ymove;
	int xstart, ystart;

	xold = xstart = 3 << 8;
	yold = ystart = 3 << 8;

	sinPlayerAngle = sin(PlayerAngle);
	cosPlayerAngle = cos(PlayerAngle);

	incr = 1;

	do
	{
		short_wait();

		xmove = ymove = 0;

                // compute crt coord, using the start coord, the crt distance and the crt angle
                x = xstart + mult(incr, cosPlayerAngle);
                y = ystart + mult(incr, sinPlayerAngle);

              	if (int_part(x) != int_part(xold) || int_part(y) != int_part(yold))
                {
       			xmove = int_part(x) - int_part(xold);
			ymove = int_part(y) - int_part(yold);
		}

               	// handle hitting the boundaries
               	if (xmove > 0 && int_part(x) >= XMAX)
               	{
			PrintChar(int_part(yold) >> 1, int_part(xold) >> 1, BLANK);

			// compute new angle
			if (PlayerAngle < halfPI)
                      	    	PlayerAngle = PI - PlayerAngle;
			else
				PlayerAngle = threePI - PlayerAngle;

			xstart = xold;
			ystart = yold;
			sinPlayerAngle = sin(PlayerAngle);
			cosPlayerAngle = cos(PlayerAngle);
			incr = 1;
                }
		else if (xmove < 0 && int_part(x) <= XMIN)
		{
			PrintChar(int_part(yold) >> 1, int_part(xold) >> 1, BLANK);

                       	// compute new angle
                       	if (PlayerAngle > PI)
                                PlayerAngle = threePI - PlayerAngle;
                        else
                                PlayerAngle = PI - PlayerAngle;

			xstart = xold;
			ystart = yold;
			sinPlayerAngle = sin(PlayerAngle);
			cosPlayerAngle = cos(PlayerAngle);
			incr = 1;
               	}
            	else if (ymove < 0 && int_part(y) <= YMIN)
                {
			PrintChar(int_part(yold) >> 1, int_part(xold) >> 1, BLANK);

                      	// compute new angle
                        PlayerAngle = twoPI - PlayerAngle;

			xstart = xold;
			ystart = yold;
			sinPlayerAngle = sin(PlayerAngle);
			cosPlayerAngle = cos(PlayerAngle);
			incr = 1;
              	}
              	else if (ymove > 0 && int_part(y) >= YMAX)
                {
			PrintChar(int_part(yold) >> 1, int_part(xold) >> 1, BLANK);

                       	// compute new angle
                        PlayerAngle = twoPI - PlayerAngle;

			xstart = xold;
			ystart = yold;
			sinPlayerAngle = sin(PlayerAngle);
			cosPlayerAngle = cos(PlayerAngle);
			incr = 1;
               	}
               	else if (xmove != 0 || ymove != 0)
		{	// ball moved
			if ((int_part(yold) >> 1) < 47 && (int_part(xold) >> 1) < 63)
				PrintChar(int_part(yold) >> 1, int_part(xold) >> 1, BLANK);

			if ((unsigned int)Screen[int_part(y) >> 1][int_part(x) >> 1] == (unsigned int)REV_BLANK)
				return 0;

			if (Screen[int_part(y) >> 1][int_part(x) >> 1] == '@')
			{
				blink_target(IndexTarget, 5);
				return 1;
			}

			PrintChar(int_part(y) >> 1, int_part(x) >> 1, GetChar('o', y, x)); 
		}

		// continue moving
		xold = x;
		yold = y;
		incr++;
	} while(1);
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
			if (rowCH > 1)
			{
				PrintChar(rowCH, colCH, chold);
				rowCH--;
			}
			break;
	case 'e':	
			if (rowCH < 46)
			{
				PrintChar(rowCH, colCH, chold);
				rowCH++;
			}
			break;
	case 's':	
			if (colCH > 1)
			{
				PrintChar(rowCH, colCH, chold);
				colCH--;
			}
			break;
	case 'd':	
			if (colCH < 62)
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

	chold = Screen[2][2];
	PrintChar(2, 2, '+');
	rowCH = colCH = 2;

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

	PlayerAngle = ComputeAngle(3 << 8, 3 << 8, (colCH*2+1) << 8, (rowCH*2+1) << 8);
}

void main(void)
{
	char ch;
	int n;

printf("The 'marksman' game\r\n\n");
printf("On a pool table (without pockets), a lot of obstacles\r\n are randomly placed, plus a target ('@').\r\n");
printf("Using any possible trajectory (including ricochet from the edges of the table),\r\n  you ('o') will try to hit the target.\r\n");
printf("The computer ('*') will try the same.\r\n");
printf("Attention, hitting an obstacle will cause the attempt to fail !\r\n");
printf("Expand the window dimensions to at least 48 rows x 80 columns.\r\n");
printf("Hit any key wnen ready... and be patient,\r\n generating the game configuration will take around 10 secs !");

	xrndseed();
	CrtSetup();

	CrtIn();

	CursorOff();
	Clear();

	InitScreen();
	InitObstacles();
	InitBoundaries();

	CompAngleInit = CompAngle = GetCompAngle();
	PlayerAngleInit = PlayerAngle = GetPlayerAngle();

	PathPlayerBall();
	PathCompBall();

	for (n = 0; n < PlayerCoordCount; n++)
		Screen[PlayerCoord[n] >> 8][PlayerCoord[n] & 0xFF] = BLANK;

	for (n = 0; n < CompCoordCount; n++)
		Screen[CompCoord[n] >> 8][CompCoord[n] & 0xFF] = BLANK;

	ComputeTargets();

	n = IndexTarget = xrnd() % TargetsCoordCount;

	Screen[TargetsCoord[n] >> 8][TargetsCoord[n] & 0xFF] = '@';

	Screen[1][1] = 'o';
	Screen[1][62] = '*';

	PrintScreen();

	blink_target(n, 5);

	SetCH();

	if (MovePlayerBall() == 1)
	{
		ClearTopLine();
		PrintStr("Congratulations, you won! Hit any key...");
		CrtIn();
	}
	else
	{
		ClearTopLine();
		PrintStr("Sorry, you missed the target! Watch the computer hitting...");
		CompAngle = CompAngleInit;
		MoveCompBall();
		ClearTopLine();
		PrintStr("Do you want to see how the correct path would have looked like? (y=YES):");
		ch = CrtIn();

		if (ch == 'y')
		{
			PlayerAngle = PlayerAngleInit;
			MovePlayerBall();
			ClearTopLine();
			PrintStr("Hit any key...");
			CrtIn();
		}
	}

	Clear();
	CursorOn();
}
