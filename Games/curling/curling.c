// Game of Curling for VT100 compatible terminals
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

#define REV_BLANK	219

#define DOWN_LEFT	191
#define DOWN_RIGHT	218
#define UP_LEFT		217
#define UP_RIGHT	192	

int halfPI =  0x192;     //fp    PI/2
int PI =      0x324;     //fp    PI
int twoPI =   0x648;     //fp    2*PI
int threePI = 0x96C;     //fp    3*PI

#define IN_RANGE	0x200
#define OUT_OF_RANGE 	0xFF00

#define XMIN 2
#define XMAX 126
#define YMIN 2
#define YMAX 42

#define XHOME_MIN 102
#define XHOME_MAX 126
#define YHOME_MIN 20
#define YHOME_MAX 34

#define int_part(x) (x >> 8)
#define f_part(x) (x & 0xFF)

int round;
int col, row;
int XCH, YCH;	
int colCH, rowCH;
int XCHfp, YCHfp;	// fixed point

#define STOPPED 	0
#define MOVING 		1
#define BASKET 		2

#define WHITE 0
#define BLACK 1

#define ONE_STEP 1

char buf[80];
char intbuf[6];

struct _stone {
	char index;		//1..16
	char color; 		//WHITE or BLACK
	char status; 		//STOPPED , MOVING or BASKET
	int X0;			//vector first ref point
	int Y0;
	int x0;
	int y0;
	int lowPartX1Y1;
	int X1;			//vector second ref point
	int Y1;
	int x1;
	int y1;
	int angle;		//vector angle
	int xstart;
	int ystart;
	int xold;
	int yold;
	int x;
	int y;
	int remainingEnergy;
	int incr;
	struct _stone *ignore;	//just collided
};

struct _stone stone_1;
struct _stone stone_2;
struct _stone stone_3;
struct _stone stone_4;
struct _stone stone_5;
struct _stone stone_6;
struct _stone stone_7;
struct _stone stone_8;
struct _stone stone_9;
struct _stone stone_10;
struct _stone stone_11;
struct _stone stone_12;
struct _stone stone_13;
struct _stone stone_14;
struct _stone stone_15;
struct _stone stone_16;

int AngleTab[32];

char Screen[64][23];

void SetUpAngleTab(void)
{
	AngleTab[31] = 0x100 + xdivytofp(3201 , 10000);
	AngleTab[30] = 0x100 + xdivytofp(2153 , 10000);
	AngleTab[29] = 0x100 + xdivytofp(1343 , 10000);
	AngleTab[28] = 0x100 + xdivytofp(654 , 10000);
	AngleTab[27] = 0x100 + xdivytofp(42 , 10000);
	AngleTab[26] = xdivytofp(9484 , 10000);
	AngleTab[25] = xdivytofp(8966 , 10000);
	AngleTab[24] = xdivytofp(8480 , 10000);
	AngleTab[23] = xdivytofp(8020 , 10000);
	AngleTab[22] = xdivytofp(7580 , 10000);
	AngleTab[21] = xdivytofp(7158 , 10000);
	AngleTab[20] = xdivytofp(6751 , 10000);
	AngleTab[19] = xdivytofp(6357 , 10000);
	AngleTab[18] = xdivytofp(5974 , 10000);
	AngleTab[17] = xdivytofp(5600 , 10000);
	AngleTab[16] = xdivytofp(5235 , 10000);
	AngleTab[15] = xdivytofp(4878 , 10000);
	AngleTab[14] = xdivytofp(4528 , 10000);
	AngleTab[13] = xdivytofp(4183 , 10000);
	AngleTab[12] = xdivytofp(3843 , 10000);
	AngleTab[11] = xdivytofp(3509 , 10000);
	AngleTab[10] = xdivytofp(3178 , 10000);
	AngleTab[9] = xdivytofp(2850 , 10000);
	AngleTab[8] = xdivytofp(2526 , 10000);
	AngleTab[7] = xdivytofp(2205 , 10000);
	AngleTab[6] = xdivytofp(1886 , 10000);
	AngleTab[5] = xdivytofp(1568 , 10000);
	AngleTab[4] = xdivytofp(1253 , 10000);
	AngleTab[3] = xdivytofp(938 , 10000);
	AngleTab[2] = xdivytofp(625 , 10000);
	AngleTab[1] = xdivytofp(312 , 10000);
	AngleTab[0] = 0;
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
	putstr(itoa(24-y));
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

char PlaceChar(char stone, int y, int x, char ch)
{
	if (Screen[x][y] == 0)
	{
		Screen[x][y] = stone;
		PrintChar(y, x, ch);
		return 1;
	}
	else
		return 0;
}

char EraseChar(char index, int y, int x)
{
	if (Screen[x][y] == index)
	{
		Screen[x][y] = 0;
		PrintChar(y, x, ' ');
		return 1;
	}
	else
		return 0;
}

//x,y fp
char GetChar(char stone_color, int y, int x)
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
				return stone_color == WHITE ? 'o' : '*';
			else
				return DOWN_LEFT;
		}
		else
		{
			if (f_part(x) >= 0x80 && f_part(y) <= 0x80)
				return stone_color == WHITE ? 'o' : '*';
			else
				return UP_LEFT;
		}
	}
	else
	{
		if (_row == 0)
		{
			if (f_part(x) <= 0x80 && f_part(y) >= 0x80)
				return stone_color == WHITE ? 'o' : '*';
			else
				return DOWN_RIGHT;
		}
		else
		{
			if (f_part(x) <= 0x80 && f_part(y) <= 0x80)
				return stone_color == WHITE ? 'o' : '*';
			else
				return UP_RIGHT;
		}
	}
}

char show(struct _stone* b)
{
	return PlaceChar(b->index, (b->y >> 9), (b->x >> 9), GetChar(b->color, b->y, b->x));
}

char hide(char index, int x, int y)
{
	return EraseChar(index, (y >> 1), (x >> 1));
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

void show_boundaries(void)
{
	int i;

	Clear();

	for (i = 0; i < 64; i++)
		PrintChar(22, i, REV_BLANK);

	for (i = 0; i < 64; i++)
		PrintChar(0, i, REV_BLANK);
}

void PlotPoint(int x, int y)
{
	if (Screen[x][y] == 0)
		PrintChar(y, x, '.');
}

void PlotHomeCircle(void)
{
	PlotPoint(56,17);
	PlotPoint(57,17);
	PlotPoint(58,17);

	PlotPoint(54,16);
	PlotPoint(60,16);

	PlotPoint(53,15);
	PlotPoint(61,15);

	PlotPoint(52,14);
	PlotPoint(62,14);

	PlotPoint(51,13);
	PlotPoint(63,13);

	PlotPoint(51,12);
	PlotPoint(63,12);

	PlotPoint(51,11);
	PlotPoint(63,11);

	PlotPoint(51,10);
	PlotPoint(63,10);

	PlotPoint(51,9);
	PlotPoint(63,9);

	PlotPoint(52,8);
	PlotPoint(62,8);

	PlotPoint(53,7);
	PlotPoint(61,7);

	PlotPoint(54,6);
	PlotPoint(60,6);

	PlotPoint(56,5);
	PlotPoint(57,5);
	PlotPoint(58,5);
}

void short_wait(void)
{
	int i;

	for (i=0; i<100; i++) 
		;
}

int vxdivytofp(long x, long y)
{
	if ((x & 0xFFFF0000) || (y & 0xFFFF0000))
		return xdivytofp((int)(x >> 16), (int)(y >> 16));
	else
		return xdivytofp((int)(x & 0xFFFF), (int)(y & 0xFFFF));
}

int ltofp(long x, long y)
{
	long lintp;
	long modulo;
	int intp;

	if (x < y)
		return vxdivytofp(x, y);

	lintp = x / y;
	intp = (int)(lintp & 0xFFFF);
	modulo = x - y * lintp;

	if (modulo == 0)
		return (intp << 8);
	else
		return (intp << 8) + vxdivytofp(modulo, y);
}

//
// Compute distance from a point(xp,yp) to a line(x1,y1,x2,y2)
//
int DistanceTo(struct _stone *stone, struct _stone *collided)
{
	int xp = collided->X0;
	int yp = collided->Y0;
	int a,b,c,f;
	long ff;
	long tmp;
	int dd;

	a = stone->Y0 - stone->Y1;
	b = stone->X1 - stone->X0;
	c = stone->X0 * stone->Y1 - stone->X1 * stone->Y0;

	f = abs(a * xp + b * yp + c);

	if (f == 0)
		return 0;

	ff = (long)f*(long)f;
	tmp = (long)a*(long)a + (long)b*(long)b;

	dd = ltofp(ff, tmp);

	return fpsqrt(dd);
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

//
// Try to place a stopped stone in an available place
//
void SetStoneSafeCoord(struct _stone *b)
{
	int X,Y;

	X = int_part(b->xold);
	Y = int_part(b->yold);

	hide(b->index, X, Y);

	if (PlaceChar(b->index, Y >> 1, X >> 1, b->color == WHITE ? 'o' : '*'))
		goto ok;

	if (X > XMIN)
	{
		hide(b->index, X-1, Y);
		if (PlaceChar(b->index, Y >> 1, (X-1) >> 1, b->color == WHITE ? 'o' : '*'))
		{
			X--;
			goto ok;
		}
	}

	if (X < XMAX)
	{
		hide(b->index, X+1, Y);
		if (PlaceChar(b->index, Y >> 1, (X+1) >> 1, b->color == WHITE ? 'o' : '*'))
		{
			X++;
			goto ok;
		}
	}

	if (Y > YMIN)
	{
		hide(b->index, X, Y-1);
		if (PlaceChar(b->index, (Y-1) >> 1, X >> 1, b->color == WHITE ? 'o' : '*'))
		{
			Y--;
			goto ok;
		}
	}
	
	if (X < XMAX)
	{
		hide(b->index, X, Y+1);
		if (PlaceChar(b->index, (Y+1) >> 1, X >> 1, b->color == WHITE ? 'o' : '*'))
		{
			Y++;
			goto ok;
		}
	}

	ClearTopLine();
	PrintStr("Fatal internal error! ... hit any key ...");	
	CrtIn();
	CursorOn();
	Clear();
	exit(0);

ok:

	b->X0 = X;
	b->x0 = X << 8;
	b->Y0 = Y;
	b->y0 = Y << 8;
}

// returns pointer to collided stone, or
// NULL if no collision
struct _stone* CheckCollision(struct _stone* b)
{
	if (b != &stone_1 && stone_1.status == STOPPED && b->ignore != &stone_1 && (abs(b->x - stone_1.x) < IN_RANGE && abs(b->y - stone_1.y) < IN_RANGE))
		return &stone_1;

	if (b != &stone_2 && stone_2.status == STOPPED && b->ignore != &stone_2 && (abs(b->x - stone_2.x) < IN_RANGE && abs(b->y - stone_2.y) < IN_RANGE))
		return &stone_2;

	if (b != &stone_3 && stone_3.status == STOPPED && b->ignore != &stone_3 && (abs(b->x - stone_3.x) < IN_RANGE && abs(b->y - stone_3.y) < IN_RANGE))
		return &stone_3;

	if (b != &stone_4 && stone_4.status == STOPPED && b->ignore != &stone_4 && (abs(b->x - stone_4.x) < IN_RANGE && abs(b->y - stone_4.y) < IN_RANGE))
		return &stone_4;

	if (b != &stone_5 && stone_5.status == STOPPED && b->ignore != &stone_5 && (abs(b->x - stone_5.x) < IN_RANGE && abs(b->y - stone_5.y) < IN_RANGE))
		return &stone_5;

	if (b != &stone_6 && stone_6.status == STOPPED && b->ignore != &stone_6 && (abs(b->x - stone_6.x) < IN_RANGE && abs(b->y - stone_6.y) < IN_RANGE))
		return &stone_6;

	if (b != &stone_7 && stone_7.status == STOPPED && b->ignore != &stone_7 && (abs(b->x - stone_7.x) < IN_RANGE && abs(b->y - stone_7.y) < IN_RANGE))
		return &stone_7;

	if (b != &stone_8 && stone_8.status == STOPPED && b->ignore != &stone_8 && (abs(b->x - stone_8.x) < IN_RANGE && abs(b->y - stone_8.y) < IN_RANGE))
		return &stone_8;

	if (b != &stone_9 && stone_9.status == STOPPED && b->ignore != &stone_9 && (abs(b->x - stone_9.x) < IN_RANGE && abs(b->y - stone_9.y) < IN_RANGE))
		return &stone_9;

	if (b != &stone_10 && stone_10.status == STOPPED && b->ignore != &stone_10 && (abs(b->x - stone_10.x) < IN_RANGE && abs(b->y - stone_10.y) < IN_RANGE))
		return &stone_10;

	if (b != &stone_11 && stone_11.status == STOPPED && b->ignore != &stone_11 && (abs(b->x - stone_11.x) < IN_RANGE && abs(b->y - stone_11.y) < IN_RANGE))
		return &stone_11;

	if (b != &stone_12 && stone_12.status == STOPPED && b->ignore != &stone_12 && (abs(b->x - stone_12.x) < IN_RANGE && abs(b->y - stone_12.y) < IN_RANGE))
		return &stone_12;

	if (b != &stone_13 && stone_13.status == STOPPED && b->ignore != &stone_13 && (abs(b->x - stone_13.x) < IN_RANGE && abs(b->y - stone_13.y) < IN_RANGE))
		return &stone_13;

	if (b != &stone_14 && stone_14.status == STOPPED && b->ignore != &stone_14 && (abs(b->x - stone_14.x) < IN_RANGE && abs(b->y - stone_14.y) < IN_RANGE))
		return &stone_14;

	if (b != &stone_15 && stone_15.status == STOPPED && b->ignore != &stone_15 && (abs(b->x - stone_15.x) < IN_RANGE && abs(b->y - stone_15.y) < IN_RANGE))
		return &stone_15;

	if (b != &stone_16 && stone_16.status == STOPPED && b->ignore != &stone_16 && (abs(b->x - stone_16.x) < IN_RANGE && abs(b->y - stone_16.y) < IN_RANGE))
		return &stone_16;

	return (struct _stone*)0;
}

//
// Start the movement of a stone
//
void StartStone(struct _stone* b, int xstart, int ystart, int angle, int energy, struct _stone* ignore_b)
{
	b->status = MOVING;

	b->xstart = b->xold = b->x = xstart;
	b->ystart = b->yold = b->y = ystart;

	b->remainingEnergy = energy;

	b->angle = angle;
	b->incr = ONE_STEP;

	b->lowPartX1Y1 = 0x7FFF;
	b->ignore = ignore_b;
}

// handle stones collision
//
void HandleCollision(struct _stone *b, struct _stone *collided)
{
	int energy, b_energy, collided_energy;
	int distance;
	int alpha;

	distance = DistanceTo(b, collided);

	if (distance >= IN_RANGE)
		return;

	if (collided->status == STOPPED)
	{
		//static stone was collided
		SetStoneSafeCoord(b);

		//change status of the collided stone
		collided->status = MOVING;

		//determine collision parameters

		if (distance < 0x010)
		{
			//stop b
			b->status = STOPPED;
			b->ignore = collided;

			//transfer angle & energy to collided stone
			StartStone(collided, collided->x0, collided->y0, b->angle, b->remainingEnergy, b); 
		}
		else
		{	//distance = 0x010 ... 0x1FF
			alpha = AngleTab[distance >> 4];	// the angle between the two vectors

			if (b->angle < ComputeAngle(b->x0, b->y0, collided->x0, collided->y0))
			{
				collided->angle = b->angle + alpha;
				if (collided->angle > twoPI)
					collided->angle = collided->angle - twoPI;

				b->angle = b->angle - halfPI + alpha;
				if (b->angle < 0)
					b->angle = twoPI + b->angle;
			}
			else
			{
				collided->angle = b->angle - alpha;
				if (collided->angle < 0)
					collided->angle = twoPI + collided->angle;

				b->angle = b->angle + halfPI - alpha;
				if (b->angle > twoPI)
					b->angle = b->angle - twoPI;
			}

			//distribute energy according to the law of physics
			energy = b->remainingEnergy << 8;

			b_energy = int_part(mul(energy, positive(sin(alpha))));
			collided_energy = int_part(mul(energy, positive(cos(alpha))));

			StartStone(b, b->x0, b->y0, b->angle, b_energy, collided); 
			StartStone(collided, collided->x0, collided->y0, collided->angle, collided_energy, b);
		}
	}
}

void HandleStone(struct _stone *b)
{
	struct _stone* collided;
	int xmove, ymove;

	short_wait();           // wait a little for each step, then...

	if (b->status == MOVING)
	{	
		b->remainingEnergy--;		// decrement remaining energy

		if (b->remainingEnergy == 0)
		{				// stop the stone
			b->status = STOPPED;
			SetStoneSafeCoord(b);
			return;
		}

		xmove = ymove = 0;

                // compute crt coord, using the start coord, the crt distance and the crt angle
                b->x = b->xstart + mult(b->incr, cos(b->angle));
                b->y = b->ystart + mult(b->incr, sin(b->angle));

                if (int_part(b->x) != int_part(b->xold) || int_part(b->y) != int_part(b->yold))
                {
       			xmove = int_part(b->x) - int_part(b->xold);
			ymove = int_part(b->y) - int_part(b->yold);

			//handle X1,Y1
			if (b->lowPartX1Y1 > f_part(b->x) + f_part(b->y))
			{
				b->lowPartX1Y1 = f_part(b->x) + f_part(b->y);
				b->X1 = int_part(b->x);
				b->Y1 = int_part(b->y);
			}
		}

                // handle hitting the boundaries
                if (xmove > 0 && int_part(b->x) > XMAX)
                {
			b->status = BASKET;
			hide(b->index, int_part(b->xold), int_part(b->yold));
			b->x = b->y = OUT_OF_RANGE;
			return;
		}
		else if (xmove < 0 && int_part(b->x) <= XMIN)
		{
			b->status = BASKET;
			hide(b->index, int_part(b->xold), int_part(b->yold));
			b->x = b->y = OUT_OF_RANGE;
			return;
                }
                else if (ymove != 0 && int_part(b->y) <= YMIN)
                {
			b->status = BASKET;
			hide(b->index, int_part(b->xold), int_part(b->yold));
			b->x = b->y = OUT_OF_RANGE;
			return;
                }
                else if (ymove != 0 && int_part(b->y) > YMAX)
                {
			b->status = BASKET;
			hide(b->index, int_part(b->xold), int_part(b->yold));
			b->x = b->y = OUT_OF_RANGE;
			return;
                }
                else
                {
			hide(b->index, int_part(b->xold), int_part(b->yold));
			b->xold = b->x;
			b->yold = b->y;
			show(b);

                        b->incr += ONE_STEP;      // ... continue the movement using the old angle
                }

		if (xmove != 0 || ymove != 0)
		{	// if stone moved, check for possible collision

			if (collided = CheckCollision(b))
				HandleCollision(b, collided);
		}
	}
}

void init_stones(void)
{
	stone_1.index = 1;
	stone_2.index = 2;
	stone_3.index = 3;
	stone_4.index = 4;
	stone_5.index = 5;
	stone_6.index = 6;
	stone_7.index = 7;
	stone_8.index = 8;
	stone_9.index = 9;
	stone_10.index = 10;
	stone_11.index = 11;
	stone_12.index = 12;
	stone_13.index = 13;
	stone_14.index = 14;
	stone_15.index = 15;
	stone_16.index = 16;

	stone_1.color = WHITE;
	stone_2.color = WHITE;
	stone_3.color = WHITE;
	stone_4.color = WHITE;
	stone_5.color = WHITE;
	stone_6.color = WHITE;
	stone_7.color = WHITE;
	stone_8.color = WHITE;

	stone_9.color = BLACK;
	stone_10.color = BLACK;
	stone_11.color = BLACK;
	stone_12.color = BLACK;
	stone_13.color = BLACK;
	stone_14.color = BLACK;
	stone_15.color = BLACK;
	stone_16.color = BLACK;

	stone_1.status = BASKET;
	stone_2.status = BASKET;
	stone_3.status = BASKET;
	stone_4.status = BASKET;
	stone_5.status = BASKET;
	stone_6.status = BASKET;
	stone_7.status = BASKET;
	stone_8.status = BASKET;

	stone_9.status = BASKET;
	stone_10.status = BASKET;
	stone_11.status = BASKET;
	stone_12.status = BASKET;
	stone_13.status = BASKET;
	stone_14.status = BASKET;
	stone_15.status = BASKET;
	stone_16.status = BASKET;

	stone_1.ignore = (struct _stone*)0;
	stone_2.ignore = (struct _stone*)0;
	stone_3.ignore = (struct _stone*)0;
	stone_4.ignore = (struct _stone*)0;
	stone_5.ignore = (struct _stone*)0;
	stone_6.ignore = (struct _stone*)0;
	stone_7.ignore = (struct _stone*)0;
	stone_8.ignore = (struct _stone*)0;
	stone_9.ignore = (struct _stone*)0;
	stone_10.ignore = (struct _stone*)0;
	stone_11.ignore = (struct _stone*)0;
	stone_12.ignore = (struct _stone*)0;
	stone_13.ignore = (struct _stone*)0;
	stone_14.ignore = (struct _stone*)0;
	stone_15.ignore = (struct _stone*)0;
	stone_16.ignore = (struct _stone*)0;
}

void move_up_ch(void)
{
	if (rowCH < 21) 						
		rowCH = rowCH + 1;
}

void move_down_ch(void)
{
	if (rowCH > 1)						
		rowCH = rowCH - 1;
}

void move_left_ch(void)
{
	if (colCH > 1)
		colCH = colCH - 1; 
}

void move_right_ch(void)
{
	if (colCH < 50)
		colCH = colCH + 1;
}

//returns 1 if CR was hit
char MoveCH(char dir)
{
	PrintChar(rowCH, colCH, ' ');

	switch (dir)
	{
	case 'x':	move_down_ch(); break;
	case 'e':	move_up_ch(); break;
	case 's':	move_left_ch(); break;
	case 'd':	move_right_ch(); break;
	default:	break;
	}

	PrintChar(rowCH, colCH, '+');
	
	return (dir == 0xD) ? 1 : 0;
}

void move_up_stone(void)
{
	if (row < 21) 						
		row = row + 1;
}

void move_down_stone(void)
{
	if (row > 1)						
		row = row - 1;
}

//returns 1 if CR was hit
char MoveStone(char dir)
{
	PrintChar(row, col, ' ');

	switch (dir)
	{
		case 'x':	move_down_stone(); break;
		case 'e':	move_up_stone(); break;
		default:	break;
	}

	PrintChar(row, col, 'o');
	
	return (dir == 0xD) ? 1 : 0;
}

void WaitUntilAllStopped(void)
{
	do
	{
		HandleStone(&stone_1);
		HandleStone(&stone_2);
		HandleStone(&stone_3);
		HandleStone(&stone_4);
		HandleStone(&stone_5);
		HandleStone(&stone_6);
		HandleStone(&stone_7);
		HandleStone(&stone_8);
		HandleStone(&stone_9);
		HandleStone(&stone_10);
		HandleStone(&stone_11);
		HandleStone(&stone_12);
		HandleStone(&stone_13);
		HandleStone(&stone_14);
		HandleStone(&stone_15);
		HandleStone(&stone_16);
	}
	while(  stone_1.status == MOVING ||
		stone_2.status == MOVING ||
		stone_3.status == MOVING ||
		stone_4.status == MOVING ||
		stone_5.status == MOVING ||
		stone_6.status == MOVING ||
		stone_7.status == MOVING ||
		stone_8.status == MOVING ||
		stone_9.status == MOVING ||
		stone_10.status == MOVING ||
		stone_11.status == MOVING ||
		stone_12.status == MOVING ||
		stone_13.status == MOVING ||
		stone_14.status == MOVING ||
		stone_15.status == MOVING ||
		stone_16.status == MOVING );
}

void QUIT(void)
{
	CursorOn();
	Clear();
	exit(0);
}

void PushPlayerStone(struct _stone* b)
{
	char ch;
	int force;

	round++;

// position stone

	col = 0;
	row = 11;

	PrintChar(row, col, 'o');

	ClearTopLine();
	sprintf(buf, "Round nr. %d. Position your stone: q=quit, e=up, x=down, <cr>=done", round);
	PrintStr(buf);
	ch = CrtIn();

	if (ch == 'q')
		QUIT();

	do
	{
		if (MoveStone(ch))
			break;
		
		ch = CrtIn();
	}
	while (1);

	b->X0 = 1;
	b->Y0 = (row << 1) + 1; 
	b->x0 = b->X0 << 8;
	b->y0 = b->Y0 << 8;

	PlaceChar(b->index, row, 0, 'o');

// position crosshair

	colCH = 2;
	rowCH = row;

	PrintChar(rowCH, colCH, '+');

	ClearTopLine();
	PrintStr("Aim: e=up, x=down, s=left, d=right, <cr>=done");
	ch = CrtIn();

	if (ch == 'q')
		QUIT();

	do
	{
		if (MoveCH(ch))
			break;
		
		ch = CrtIn();
	}
	while (1);

	PrintChar(rowCH, colCH, '+');

	XCH = (colCH << 1) + 1;
	XCHfp = XCH << 8;
	YCH = (rowCH << 1) + 1;
	YCHfp = YCH << 8;
		
	do
	{
		ClearTopLine();
		PrintStr("Throw force? (1=smallest, 27=highest):");
		cgets(buf);
		ClearTopLine();
		force = atoi(buf);
	}
	while (force == 0 || force > 27);

	PrintChar(rowCH, colCH, ' ');	//erase crosshairs

	StartStone(b, b->x0, b->y0, ComputeAngle(b->x0, b->y0, XCHfp, YCHfp), force + 100, (struct _stone*)0);

	WaitUntilAllStopped();

	PlotHomeCircle();
}

// y = row
int IsFree(int y)
{
	if (stone_9.status == STOPPED && (stone_9.Y0 >> 1) == y)
		return 0;
	else if (stone_10.status == STOPPED && (stone_10.Y0 >> 1) == y)
		return 0;
	else if (stone_11.status == STOPPED && (stone_11.Y0 >> 1) == y)
		return 0;
	else if (stone_12.status == STOPPED && (stone_12.Y0 >> 1) == y)
		return 0;
	else if (stone_13.status == STOPPED && (stone_13.Y0 >> 1) == y)
		return 0;
	else if (stone_14.status == STOPPED && (stone_14.Y0 >> 1) == y)
		return 0;
	else if (stone_15.status == STOPPED && (stone_15.Y0 >> 1) == y)
		return 0;
	else if (stone_16.status == STOPPED && (stone_16.Y0 >> 1) == y)
		return 0;
	else
		return 1;
}

void PushComputerStone(struct _stone* b)
{
	int y, force = 127;

	ClearTopLine();
	PrintStr("Computer throws stone...");

	// try to push out a stone

	if (IsFree(stone_1.Y0 >> 1) && stone_1.status == STOPPED && (stone_1.X0 >= XHOME_MIN && stone_1.Y0 >= YHOME_MIN && stone_1.Y0 <= YHOME_MAX))
		y = stone_1.Y0;
	else if (IsFree(stone_2.Y0 >> 1) && stone_2.status == STOPPED && (stone_2.X0 >= XHOME_MIN && stone_2.Y0 >= YHOME_MIN && stone_2.Y0 <= YHOME_MAX))
		y = stone_2.Y0;
	else if (IsFree(stone_3.Y0 >> 1) && stone_3.status == STOPPED && (stone_3.X0 >= XHOME_MIN && stone_3.Y0 >= YHOME_MIN && stone_3.Y0 <= YHOME_MAX))
		y = stone_3.Y0;
	else if (IsFree(stone_4.Y0 >> 1) && stone_4.status == STOPPED && (stone_4.X0 >= XHOME_MIN && stone_4.Y0 >= YHOME_MIN && stone_4.Y0 <= YHOME_MAX))
		y = stone_4.Y0;
	else if (IsFree(stone_5.Y0 >> 1) && stone_5.status == STOPPED && (stone_5.X0 >= XHOME_MIN && stone_5.Y0 >= YHOME_MIN && stone_5.Y0 <= YHOME_MAX))
		y = stone_5.Y0;
	else if (IsFree(stone_6.Y0 >> 1) && stone_6.status == STOPPED && (stone_6.X0 >= XHOME_MIN && stone_6.Y0 >= YHOME_MIN && stone_6.Y0 <= YHOME_MAX))
		y = stone_6.Y0;
	else if (IsFree(stone_7.Y0 >> 1) && stone_7.status == STOPPED && (stone_7.X0 >= XHOME_MIN && stone_7.Y0 >= YHOME_MIN && stone_7.Y0 <= YHOME_MAX))
		y = stone_7.Y0;
	else if (IsFree(stone_8.Y0 >> 1) && stone_8.status == STOPPED && (stone_8.X0 >= XHOME_MIN && stone_8.Y0 >= YHOME_MIN && stone_8.Y0 <= YHOME_MAX))
		y = stone_8.Y0;
	else
	{
		// try to place a stone in the HOME
		force = 114;

		if (IsFree(11))
			y = 22+1;
		else if (IsFree(12))
			y = 24+1;
		else if (IsFree(10))
			y = 20+1;
		else if (IsFree(13))
			y = 26+1;
		else if (IsFree(9))
			y = 18+1;
		else if (IsFree(14))
			y = 28+1;
		else if (IsFree(8))
			y = 16+1;
		else if (IsFree(15))
			y = 30+1;
		else if (IsFree(7))
			y = 14+1;
		else if (IsFree(16))
			y = 32+1;
		else if (IsFree(6))
			y = 5+1;
		else if (IsFree(17))
			y = 34+1;
		else if (IsFree(5))
			y = 10+1;
		else
			y = 22+1;
	}

	b->X0 = 1;
	b->Y0 = y; 
	b->x0 = b->X0 << 8;
	b->y0 = b->Y0 << 8;

	StartStone(b, b->x0, b->y0, 0, force, (struct _stone*)0);

	WaitUntilAllStopped();

	PlotHomeCircle();
}

int DistFromCenter(struct _stone* b)
{
	int dx, dy;

	dx = positive((int_part(b->x) >> 1) - 57);
	dy = positive((int_part(b->y) >> 1) - 11);

	dx = dx * dx;
	dy = dy * dy;

	if ((dx + dy) > 36)
		return 0;

	return 1;
}

int InHome(struct _stone* b)
{
	if (b->status == STOPPED &&
	(int_part(b->x) >> 1) >= 51 &&
	(int_part(b->y) >> 1) >= 5 &&
	(int_part(b->y) >> 1) <= 17 &&
	DistFromCenter(b))
		return 1;

	return 0;
}

int Player(void)
{
	return InHome(&stone_1) + InHome(&stone_2) + InHome(&stone_3) + InHome(&stone_4) + InHome(&stone_5) + InHome(&stone_6) + InHome(&stone_7) + InHome(&stone_8);
}

int Computer(void)
{
	return InHome(&stone_9) + InHome(&stone_10) + InHome(&stone_11) + InHome(&stone_12) + InHome(&stone_13) + InHome(&stone_14) + InHome(&stone_15) + InHome(&stone_16);
}

int main(int argc, char** argv)
{
	int x, y;

	round = 0;

	for (x = 0; x < 64; x++)
		for (y = 0; y < 23; y++)
			Screen[x][y] = 0;

	SetUpAngleTab();

	CrtSetup();
	CursorOff();
	Clear();

	printf("The little game of curling\r\n\n");
	printf("The game consists of 8 sessions in which, alternatively, you and the computer\r\n");
	printf(" try to push as many stones as possible into the house.\r\n");
	printf("Stones touching the edges will be disqualified.\r\n");
	printf("Stones can collide to reach more advantageous positions.\r\n");
	printf("The computer always pushes the stones horizontally; you can also push stones\r\n");
	printf(" at an angle, for example to 'hide' them behind the opponent's stones.\r\n");
	printf("The one who places more stones in the house wins.\r\n");
	printf("Hit any key to start playing...\r\n");

	CrtIn();

	show_boundaries();		//clear screen, show boundaries

	init_stones();			//init stones 

	PlotHomeCircle();

	PushPlayerStone(&stone_1);
	PushComputerStone(&stone_9);

	PushPlayerStone(&stone_2);
	PushComputerStone(&stone_10);

	PushPlayerStone(&stone_3);
	PushComputerStone(&stone_11);

	PushPlayerStone(&stone_4);
	PushComputerStone(&stone_12);

	PushPlayerStone(&stone_5);
	PushComputerStone(&stone_13);

	PushPlayerStone(&stone_6);
	PushComputerStone(&stone_14);

	PushPlayerStone(&stone_7);
	PushComputerStone(&stone_15);

	PushPlayerStone(&stone_8);
	PushComputerStone(&stone_16);

	ClearTopLine();

	if (Player() > Computer())
		PrintStr("Congratulations, you won! hit any key...");
	else
		PrintStr("Sorry, you lost! hit any key...");
		
	CrtIn();
	CursorOn();
	Clear();
	exit(0);
}
