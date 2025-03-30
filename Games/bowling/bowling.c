// Game of Bowling for VT100 compatible terminals
//
// Ladislau Szilagyi, November 2024
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

#define IN_RANGE_B	0x200
#define IN_RANGE_P	0x400
#define OUT_OF_RANGE 	0xFF00

#define XMIN 2
#define XMAX 124
#define YMIN 2
#define YMAX 22

#define int_part(x) (x >> 8)
#define f_part(x) (x & 0xFF)

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

struct _ball {
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
	int incr;
	struct _ball *ignore;	//just collided
};

struct _ball ball_1;
struct _ball ball_2;
struct _ball ball_3;
struct _ball ball_4;
struct _ball ball_5;
struct _ball ball_6;
struct _ball ball_7;
struct _ball ball_8;
struct _ball ball_9;
struct _ball ball_10;

int AngleTab[32];

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

//x,y fp
char GetChar(struct _ball* b, int y, int x)
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
				return b == &ball_1 ? 'o' : '*';
			else
				return DOWN_LEFT;
		}
		else
		{
			if (f_part(x) >= 0x80 && f_part(y) <= 0x80)
				return b == &ball_1 ? 'o' : '*';
			else
				return UP_LEFT;
		}
	}
	else
	{
		if (_row == 0)
		{
			if (f_part(x) <= 0x80 && f_part(y) >= 0x80)
				return b == &ball_1 ? 'o' : '*';
			else
				return DOWN_RIGHT;
		}
		else
		{
			if (f_part(x) <= 0x80 && f_part(y) <= 0x80)
				return b == &ball_1 ? 'o' : '*';
			else
				return UP_RIGHT;
		}
	}
}

void show(struct _ball* ball, int x, int y)
{
	PrintChar((y >> 9), (x >> 9), GetChar(ball, y, x));
}

void hide(int x, int y)
{
	PrintChar((y >> 9), (x >> 9), ' ');
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
		PrintChar(12, i, REV_BLANK);

	for (i = 0; i < 64; i++)
		PrintChar(0, i, REV_BLANK);

	for (i = 1; i < 12; i++)
		PrintChar(i, 0, REV_BLANK);
}

void short_wait(void)
{
	int i;

	for (i=0; i<200; i++) 
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
int DistanceTo(struct _ball *ball, struct _ball *collided)
{
	int xp = collided->X0;
	int yp = collided->Y0;
	int a,b,c,f;
	long ff;
	long tmp;
	int dd;

	a = ball->Y0 - ball->Y1;
	b = ball->X1 - ball->X0;
	c = ball->X0 * ball->Y1 - ball->X1 * ball->Y0;

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
// Try to place a stopped ball in an available place
//
void SetBallSafeCoord(struct _ball *b)
{
	int X,Y;

	X = int_part(b->xold);
	Y = int_part(b->yold);

	hide(b->xold, b->yold);

	PrintChar(Y >> 1, X >> 1, b->color == WHITE ? 'o' : '*');

	b->X0 = X;
	b->x0 = X << 8;
	b->Y0 = Y;
	b->y0 = Y << 8;
}

// for ball_1
// returns pointer to collided pin, or
// NULL if no collision
struct _ball* CheckCollisionBall(void)
{
	if (ball_2.status == STOPPED && ball_1.ignore != &ball_2 && (abs(ball_1.x - ball_2.x) < IN_RANGE_B && abs(ball_1.y - ball_2.y) < IN_RANGE_B))
		return &ball_2;
	if (ball_3.status == STOPPED && ball_1.ignore != &ball_3 && (abs(ball_1.x - ball_3.x) < IN_RANGE_B && abs(ball_1.y - ball_3.y) < IN_RANGE_B))
		return &ball_3;
	if (ball_4.status == STOPPED && ball_1.ignore != &ball_4 && (abs(ball_1.x - ball_4.x) < IN_RANGE_B && abs(ball_1.y - ball_4.y) < IN_RANGE_B))
		return &ball_4;
	if (ball_5.status == STOPPED && ball_1.ignore != &ball_5 && (abs(ball_1.x - ball_5.x) < IN_RANGE_B && abs(ball_1.y - ball_5.y) < IN_RANGE_B))
		return &ball_5;
	if (ball_6.status == STOPPED && ball_1.ignore != &ball_6 && (abs(ball_1.x - ball_6.x) < IN_RANGE_B && abs(ball_1.y - ball_6.y) < IN_RANGE_B))
		return &ball_6;
	if (ball_7.status == STOPPED && ball_1.ignore != &ball_7 && (abs(ball_1.x - ball_7.x) < IN_RANGE_B && abs(ball_1.y - ball_7.y) < IN_RANGE_B))
		return &ball_7;
	if (ball_8.status == STOPPED && ball_1.ignore != &ball_8 && (abs(ball_1.x - ball_8.x) < IN_RANGE_B && abs(ball_1.y - ball_8.y) < IN_RANGE_B))
		return &ball_8;
	if (ball_9.status == STOPPED && ball_1.ignore != &ball_9 && (abs(ball_1.x - ball_9.x) < IN_RANGE_B && abs(ball_1.y - ball_9.y) < IN_RANGE_B))
		return &ball_9;
	if (ball_10.status == STOPPED && ball_1.ignore != &ball_10 && (abs(ball_1.x - ball_10.x) < IN_RANGE_B && abs(ball_1.y - ball_10.y) < IN_RANGE_B))
		return &ball_10;

	return (struct _ball*)0;
}

// for pin 2...10
// returns pointer to collided pin, or
// NULL if no collision
struct _ball* CheckCollisionPin(struct _ball* b)
{
	if (ball_2.status == STOPPED && b != &ball_2 && (abs(b->x - ball_2.x) < IN_RANGE_P && abs(b->y - ball_2.y) < IN_RANGE_P))
		return &ball_2;
	if (ball_3.status == STOPPED && b != &ball_3 && (abs(b->x - ball_3.x) < IN_RANGE_P && abs(b->y - ball_3.y) < IN_RANGE_P))
		return &ball_3;
	if (ball_4.status == STOPPED && b != &ball_4 && (abs(b->x - ball_4.x) < IN_RANGE_P && abs(b->y - ball_4.y) < IN_RANGE_P))
		return &ball_4;
	if (ball_5.status == STOPPED && b != &ball_5 && (abs(b->x - ball_5.x) < IN_RANGE_P && abs(b->y - ball_5.y) < IN_RANGE_P))
		return &ball_5;
	if (ball_6.status == STOPPED && b != &ball_6 && (abs(b->x - ball_6.x) < IN_RANGE_P && abs(b->y - ball_6.y) < IN_RANGE_P))
		return &ball_6;
	if (ball_7.status == STOPPED && b != &ball_7 && (abs(b->x - ball_7.x) < IN_RANGE_P && abs(b->y - ball_7.y) < IN_RANGE_P))
		return &ball_7;
	if (ball_8.status == STOPPED && b != &ball_8 && (abs(b->x - ball_8.x) < IN_RANGE_P && abs(b->y - ball_8.y) < IN_RANGE_P))
		return &ball_8;
	if (ball_9.status == STOPPED && b != &ball_9 && (abs(b->x - ball_9.x) < IN_RANGE_P && abs(b->y - ball_9.y) < IN_RANGE_P))
		return &ball_9;
	if (ball_10.status == STOPPED && b != &ball_10 && (abs(b->x - ball_10.x) < IN_RANGE_P && abs(b->y - ball_10.y) < IN_RANGE_P))
		return &ball_10;

	return (struct _ball*)0;
}

struct _ball* CheckCollision(struct _ball* b)
{
	if (b == &ball_1)
		return CheckCollisionBall();
	else
		return CheckCollisionPin(b);
}

//
// Start the movement of a ball
//
void StartBall(struct _ball* b, int xstart, int ystart, int angle, struct _ball* ignore_b)
{
	b->status = MOVING;

	b->xstart = b->xold = b->x = xstart;
	b->ystart = b->yold = b->y = ystart;

	b->angle = angle;
	b->incr = ONE_STEP;

	b->lowPartX1Y1 = 0x7FFF;
	b->ignore = ignore_b;
}

//
// Bounce ball
// in: xstart, ystart, angle
// out: x0, X0, y0, Y0, x1, X1, y1, Y1, angle, incr, xstart, ystart, xold, yold
//
void BounceBall(struct _ball *b, int angle)
{
	SetBallSafeCoord(b);

	b->lowPartX1Y1 = 0x7FFF;

	b->xstart = b->x = b->xold;
	b->ystart = b->y = b->yold;

	b->angle = angle;
	b->incr = ONE_STEP;

	b->ignore = (struct _ball*)0;
}

// handle balls & pins collision
//
void HandleCollision(struct _ball *b, struct _ball *collided)
{
	int distance;
	int alpha;

	if (b != &ball_1)
	{	//pin hits pin, throw them to the basket!
		b->status = BASKET;
		collided->status = BASKET;

		hide(b->xold, b->yold);
		b->x = b->y = OUT_OF_RANGE;

		hide(collided->x, collided->y);
		collided->x = collided->y = OUT_OF_RANGE;

		return;
	}

	// white ball hits pin

	distance = DistanceTo(b, collided);

	if (distance >= IN_RANGE_B)
		return;

	//determine collision parameters

	if (distance < 0x010)
	{
		//stop & throw white ball
		b->status = BASKET;

		//hide white ball
		hide(b->xold, b->yold);
		b->x = b->y = OUT_OF_RANGE;

		//transfer angle to collided ball
		StartBall(collided, collided->x0, collided->y0, b->angle, b); 
	}
	else
	{	//distance = 0x010 ... 0x1FF

		SetBallSafeCoord(b);

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

		StartBall(b, b->x0, b->y0, b->angle, collided); 
		StartBall(collided, collided->x0, collided->y0, collided->angle, b);
	}
}

void HandleBall(struct _ball *b)
{
	struct _ball* collided;
	int xmove, ymove;
	int angle;

	short_wait();           // wait a little for each step, then...

	if (b->status == MOVING)
	{	
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
				hide(b->xold, b->yold);
				b->status = BASKET;
				b->x = b->y = OUT_OF_RANGE;
				return;
			}
			else if (xmove < 0 && int_part(b->x) <= XMIN)
			{
				if (b != &ball_1)
				{	//it is a pin...
					hide(b->xold, b->yold);
					b->x = b->y = OUT_OF_RANGE;
					b->status = BASKET;
					return;
				}

                               	// compute new angle
                               	if (b->angle > PI)
                                     	angle = threePI - b->angle;
                               	else
                                       	angle = PI - b->angle;

				BounceBall(b, angle);

				return;
                       	}

                       	else if (ymove != 0 && int_part(b->y) <= YMIN)
                       	{
				if (b != &ball_1)
				{	//it is a pin...
					hide(b->xold, b->yold);
					b->x = b->y = OUT_OF_RANGE;
					b->status = BASKET;
					return;
				}

                               	// compute new angle
                               	angle = twoPI - b->angle;

				BounceBall(b, angle);

				return;
                       	}
                       	else if (ymove != 0 && int_part(b->y) > YMAX)
                       	{
				if (b != &ball_1)
				{	//it is a pin...
					hide(b->xold, b->yold);
					b->x = b->y = OUT_OF_RANGE;
					b->status = BASKET;
					return;
				}

                               	// compute new angle
                               	angle = twoPI - b->angle;

				BounceBall(b, angle);

				return;
                       	}
                       	else
                       	{
				hide(b->xold, b->yold);
				b->xold = b->x;
				b->yold = b->y;
				show(b, b->x, b->y);

                               	b->incr += ONE_STEP;      // ... continue the movement using the old angle
                       	}

			if (xmove != 0 || ymove != 0)
			{	// if ball moved, check for possible collision

				if (collided = CheckCollision(b))
					HandleCollision(b, collided);
			}
		
	}
}

void set_pins_coord(void)
{
	ball_2.X0 = 52*2+1;
	ball_2.Y0 = 6*2+1;
	ball_2.x0 = ball_2.x = ball_2.X0 << 8;
	ball_2.y0 = ball_2.y = ball_2.Y0 << 8;

	ball_3.X0 = 57*2+1;
	ball_3.Y0 = 8*2+1;
	ball_3.x0 = ball_3.x = ball_3.X0 << 8;
	ball_3.y0 = ball_3.y = ball_3.Y0 << 8;

	ball_4.X0 = 57*2+1;
	ball_4.Y0 = 6*2+1;
	ball_4.x0 = ball_4.x = ball_4.X0 << 8;
	ball_4.y0 = ball_4.y = ball_4.Y0 << 8;

	ball_5.X0 = 57*2+1;
	ball_5.Y0 = 4*2+1;
	ball_5.x0 = ball_5.x = ball_5.X0 << 8;
	ball_5.y0 = ball_5.y = ball_5.Y0 << 8;

	ball_6.X0 = 62*2+1;
	ball_6.Y0 = 10*2+1;
	ball_6.x0 = ball_6.x = ball_6.X0 << 8;
	ball_6.y0 = ball_6.y = ball_6.Y0 << 8;

	ball_7.X0 = 62*2+1;
	ball_7.Y0 = 8*2+1;
	ball_7.x0 = ball_7.x = ball_7.X0 << 8;
	ball_7.y0 = ball_7.y = ball_7.Y0 << 8;

	ball_8.X0 = 62*2+1;
	ball_8.Y0 = 6*2+1;
	ball_8.x0 = ball_8.x = ball_8.X0 << 8;
	ball_8.y0 = ball_8.y = ball_8.Y0 << 8;

	ball_9.X0 = 62*2+1;
	ball_9.Y0 = 4*2+1;
	ball_9.x0 = ball_9.x = ball_9.X0 << 8;
	ball_9.y0 = ball_9.y = ball_9.Y0 << 8;

	ball_10.X0 = 62*2+1;
	ball_10.Y0 = 2*2+1;
	ball_10.x0 = ball_10.x = ball_10.X0 << 8;
	ball_10.y0 = ball_10.y = ball_10.Y0 << 8;

	ball_2.color = BLACK;
	ball_3.color = BLACK;
	ball_4.color = BLACK;
	ball_5.color = BLACK;
	ball_6.color = BLACK;
	ball_7.color = BLACK;
	ball_8.color = BLACK;
	ball_9.color = BLACK;
	ball_10.color = BLACK;

	ball_2.status = STOPPED;
	ball_3.status = STOPPED;
	ball_4.status = STOPPED;
	ball_5.status = STOPPED;
	ball_6.status = STOPPED;
	ball_7.status = STOPPED;
	ball_8.status = STOPPED;
	ball_9.status = STOPPED;
	ball_10.status = STOPPED;
	
	PrintChar(ball_2.Y0 >> 1, ball_2.X0 >> 1, '*');
	PrintChar(ball_3.Y0 >> 1, ball_3.X0 >> 1, '*');
	PrintChar(ball_4.Y0 >> 1, ball_4.X0 >> 1, '*');
	PrintChar(ball_5.Y0 >> 1, ball_5.X0 >> 1, '*');
	PrintChar(ball_6.Y0 >> 1, ball_6.X0 >> 1, '*');
	PrintChar(ball_7.Y0 >> 1, ball_7.X0 >> 1, '*');
	PrintChar(ball_8.Y0 >> 1, ball_8.X0 >> 1, '*');
	PrintChar(ball_9.Y0 >> 1, ball_9.X0 >> 1, '*');
	PrintChar(ball_10.Y0 >> 1, ball_10.X0 >> 1, '*');
}

void place_CH(void)
{
	XCH = 3*2;
	YCH = 6*2;

	XCHfp = XCH << 8;
	colCH = XCH >> 1;

	YCHfp = YCH << 8;
	rowCH = YCH >> 1;
}

void move_up_ch(void)
{
	if (rowCH < 11)						
		rowCH = rowCH + 1;
}

void move_down_ch(void)
{
	if (rowCH > 1)						
		rowCH = rowCH - 1;
}

void move_left_ch(void)
{
	if (colCH > 3)
		colCH = colCH - 1; 
}

void move_right_ch(void)
{
	if (colCH < 51)
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

int main(void)
{
	char ch;
	int attempts = 0;

	SetUpAngleTab();

	CrtSetup();
	CursorOff();
	Clear();

	printf("The little game of bowling\r\n\n");
	printf("Your job is to aim (using '+') and hit the nine pins (*)\r\n");
	printf(" with the white (o) ball, in order to overthrow them,\r\n");
	printf(" in the minimum number of attempts\r\n");
	printf("The white ball will bounce back when hitting the rim\r\n");
	printf("Hit any key to start:");
	CrtIn();

	show_boundaries();		//clear screen, show boundaries

	set_pins_coord();		//set ball & pins positions

	do
	{
		ball_1.X0 = 1*2+1;
		ball_1.x0 = ball_1.x = ball_1.X0 << 8;
		ball_1.Y0 = 6*2+1;
		ball_1.y0 = ball_1.y = ball_1.Y0 << 8;

		ball_1.color = WHITE;

		PrintChar(ball_1.Y0 >> 1, ball_1.X0 >> 1, 'o');

		place_CH();
		PrintChar(rowCH, colCH, '+');

		ClearTopLine();
		PrintStr("q=quit or aim (s=left,d=right,x=down,e=up) then hit <cr>");
	
		ch = CrtIn();
	
		if (ch == 'q')
			break;
	
		do
		{
			if (MoveCH(ch))
				break;
		
			ch = CrtIn();
		}
		while (1);

		ClearTopLine();

		XCH = (colCH << 1) + 1;
		XCHfp = XCH << 8;
		YCH = (rowCH << 1) + 1;
		YCHfp = YCH << 8;
		
		PrintChar(rowCH, colCH, ' ');	//erase crosshairs

		ball_2.ignore = (struct _ball*)0;
		ball_3.ignore = (struct _ball*)0;
		ball_4.ignore = (struct _ball*)0;
		ball_5.ignore = (struct _ball*)0;
		ball_6.ignore = (struct _ball*)0;
		ball_7.ignore = (struct _ball*)0;
		ball_8.ignore = (struct _ball*)0;
		ball_9.ignore = (struct _ball*)0;
		ball_10.ignore = (struct _ball*)0;

		attempts++;
		StartBall(&ball_1, ball_1.x0, ball_1.y0, ComputeAngle(ball_1.x0, ball_1.y0, XCHfp, YCHfp),  (struct _ball*)0);

		do
		{
			HandleBall(&ball_1);
			HandleBall(&ball_2);
			HandleBall(&ball_3);
			HandleBall(&ball_4);
			HandleBall(&ball_5);
			HandleBall(&ball_6);
			HandleBall(&ball_7);
			HandleBall(&ball_8);
			HandleBall(&ball_9);
			HandleBall(&ball_10);
		}
		while(  ball_1.status == MOVING ||
			ball_2.status == MOVING ||
			ball_3.status == MOVING ||
			ball_4.status == MOVING ||
			ball_5.status == MOVING ||
			ball_6.status == MOVING ||
			ball_7.status == MOVING ||
			ball_8.status == MOVING ||
			ball_9.status == MOVING ||
			ball_10.status == MOVING);

		if (ball_2.status == BASKET &&
		    ball_3.status == BASKET &&
		    ball_4.status == BASKET &&
		    ball_5.status == BASKET &&
		    ball_6.status == BASKET &&
		    ball_7.status == BASKET &&
		    ball_8.status == BASKET &&
		    ball_9.status == BASKET &&
		    ball_10.status == BASKET)
		{
			ClearTopLine();
			sprintf(buf, "Well done, all the pins were overthrown in %d attempts! hit any key...", attempts);
			PrintStr(buf);
			CrtIn();
			break;
		}

	} while (1);

	CursorOn();
	Clear();
	exit(0);
}
