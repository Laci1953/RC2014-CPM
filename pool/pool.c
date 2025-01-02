// Game of POOL for VT100 compatible terminals
//
// Ladislau Szilagyi, Jan 2025
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

void SetAimStatus(void);
int Dist(int X0, int Y0, int X1, int Y1, int xp, int yp);
void PrintAimStatus(int d);
void AimStatus(void);

#define REV_BLANK	219

#define DOWN_LEFT	191
#define DOWN_RIGHT	218
#define UP_LEFT		217
#define UP_RIGHT	192	

char buf[64];

int halfPI =  0x192;     //fp    PI/2
int PI =      0x324;     //fp    PI
int twoPI =   0x648;     //fp    2*PI
int threePI = 0x96C;     //fp    3*PI

#define IN_RANGE	0x200
#define OUT_OF_RANGE 	0xFF00

// x = 0   & 1   : border
// x = 126 & 127 : border
// y = 0   & 1   : border
// y = 94  & 95  : text line
// y = 92  & 93  : border

// dynamic balls position from
// x = 2 to 124
// y = 2 to 90

#define XMIN 2
#define XMAX 124
#define YMIN 2
#define YMAX 90

#define ROW1_BASKET 22
#define ROW2_BASKET 23
#define ROW3_BASKET 24

#define Y_BASKET_MIN 44
#define Y_BASKET_MAX 49

#define COL11_BASKET 12
#define COL12_BASKET 13
#define COL13_BASKET 14

#define COL21_BASKET 50
#define COL22_BASKET 51
#define COL23_BASKET 52

#define X_BASKET1_MIN 24
#define X_BASKET1_MAX 28

#define X_BASKET2_MIN 100
#define X_BASKET2_MAX 104

#define int_part(x) (x >> 8)
#define f_part(x) (x & 0xFF)

char TrainingMode;	// 1 = ON
int _X0, _Y0;
int XCH, YCH;	
int colCH, rowCH;
int XCHfp, YCHfp;	// fixed point

#define STOPPED 	0
#define MOVING 		1
#define BASKET 		2

int CountMoving;

#define WHITE 0
#define BLACK 1

#define ONE_STEP 1
#define SPEED_BOUNDARY 26
#define MAX_BRAKE 10

char intbuf[6];

#define INT 0
#define FP 1

struct _ball {
	char index;		//1,2,3,4,5
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
	int remainingEnergy;	//initially set to hit force * 5
	int incr;
	struct _ball *ignore;	//just collided
};

struct _ball ball_1;
struct _ball ball_2;
struct _ball ball_3;
struct _ball ball_4;
struct _ball ball_5;

int AngleTab[32];

char Screen[64][48];

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

char PlaceChar(char ball, int y, int x, char ch)
{
	if (Screen[x][y] == 0)
	{
		Screen[x][y] = ball;
		PrintChar(y, x, ch);
		return 1;
	}
	else
		return 0;
}

char EraseChar(char ball, int y, int x)
{
	if (Screen[x][y] == ball)
	{
		Screen[x][y] = 0;
		PrintChar(y, x, ' ');
		return 1;
	}
	else
		return 0;
}

//x,y fp
char GetChar(int ball_index, int y, int x)
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
				return ball_index == 1 ? 'o' : '*';
			else
				return DOWN_LEFT;
		}
		else
		{
			if (f_part(x) >= 0x80 && f_part(y) <= 0x80)
				return ball_index == 1 ? 'o' : '*';
			else
				return UP_LEFT;
		}
	}
	else
	{
		if (_row == 0)
		{
			if (f_part(x) <= 0x80 && f_part(y) >= 0x80)
				return ball_index == 1 ? 'o' : '*';
			else
				return DOWN_RIGHT;
		}
		else
		{
			if (f_part(x) <= 0x80 && f_part(y) <= 0x80)
				return ball_index == 1 ? 'o' : '*';
			else
				return UP_RIGHT;
		}
	}
}

char show(char ball, int x, int y)
{
	return PlaceChar(ball, (y >> 9), (x >> 9), GetChar(ball, y, x));
}

char hide(char ball, int x, int y)
{
	return EraseChar(ball, (y >> 1), (x >> 1));
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
		PrintChar(46, i, REV_BLANK);

	for (i = 0; i < 64; i++)
		PrintChar(0, i, REV_BLANK);

	for (i = 1; i <= 45; i++)
		PrintChar(i, 0, REV_BLANK);

	for (i = 1; i <= 45; i++)
		PrintChar(i, 63, REV_BLANK);

	for (i = COL11_BASKET; i <= COL13_BASKET; i++)
		PrintChar(0, i, ' ');

	for (i = COL21_BASKET; i <= COL23_BASKET; i++)
		PrintChar(0, i, ' ');

	for (i = COL11_BASKET; i <= COL13_BASKET; i++)
		PrintChar(46, i, ' ');

	for (i = COL21_BASKET; i <= COL23_BASKET; i++)
		PrintChar(46, i, ' ');

	for (i = ROW1_BASKET; i <= ROW3_BASKET; i++)
		PrintChar(i, 0, ' ');

	for (i = ROW1_BASKET; i <= ROW3_BASKET; i++)
		PrintChar(i, 63, ' ');
}

void short_wait(void)
{
	int i;

	for (i=0; i<500; i++) 
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

char CheckYBasket(int v)
{
	if (v >= Y_BASKET_MIN && v <= Y_BASKET_MAX)
		return 1;
	else
		return 0;
}

char CheckXBasket(int v)
{
	if ((v >= X_BASKET1_MIN && v <= X_BASKET1_MAX) || (v >= X_BASKET2_MIN && v <= X_BASKET2_MAX))
		return 1;
	else
		return 0;
}

//
// Try to place a stopped ball in an available place
//
void SetBallSafeCoord(struct _ball *b)
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

// returns pointer to collided ball, or
// NULL if no collision
struct _ball* CheckCollision(struct _ball* b)
{
	if (b != &ball_1 && ball_1.status == STOPPED && b->ignore != &ball_1 && (abs(b->x - ball_1.x) < IN_RANGE && abs(b->y - ball_1.y) < IN_RANGE))
		return &ball_1;

	if (b != &ball_2 && ball_2.status == STOPPED && b->ignore != &ball_2 && (abs(b->x - ball_2.x) < IN_RANGE && abs(b->y - ball_2.y) < IN_RANGE))
		return &ball_2;

	if (b != &ball_3 && ball_3.status == STOPPED && b->ignore != &ball_3 && (abs(b->x - ball_3.x) < IN_RANGE && abs(b->y - ball_3.y) < IN_RANGE))
		return &ball_3;

	if (b != &ball_4 && ball_4.status == STOPPED && b->ignore != &ball_4 && (abs(b->x - ball_4.x) < IN_RANGE && abs(b->y - ball_4.y) < IN_RANGE))
		return &ball_4;

	if (b != &ball_5 && ball_5.status == STOPPED && b->ignore != &ball_5 && (abs(b->x - ball_5.x) < IN_RANGE && abs(b->y - ball_5.y) < IN_RANGE))
		return &ball_5;

	return (struct _ball*)0;
}

//
// Start the movement of a ball
//
void StartBall(struct _ball* b, int xstart, int ystart, int angle, int energy, struct _ball* ignore_b)
{
	CountMoving++;
	b->status = MOVING;

	b->xstart = b->xold = b->x = xstart;
	b->ystart = b->yold = b->y = ystart;

	b->remainingEnergy = energy;

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

// handle balls collision
//
void HandleCollision(struct _ball *b, struct _ball *collided)
{
//	int tmp;
	int energy, b_energy, collided_energy;
	char aprox;
	int distance;
	int alpha;

	distance = DistanceTo(b, collided);

	if (distance >= IN_RANGE)
		return;

	if (collided->status == STOPPED)
	{
		//static ball was collided
		SetBallSafeCoord(b);

		//change status of the collided ball
		collided->status = MOVING;

		//determine collision parameters

		if (distance < 0x010)
		{
			//stop b
			b->status = STOPPED;
			CountMoving--;
			b->ignore = collided;

			//transfer angle & energy to collided ball
			StartBall(collided, collided->x0, collided->y0, b->angle, b->remainingEnergy, b); 
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
			energy = b->remainingEnergy;		//up to 99*5
			aprox = (energy > 127) ? 1 : 0;		//fixed point may handle only numbers < 128

			if (aprox != 0)
				energy = energy >> 2;		//up to 99*5/4 ~ 123
			
			energy = energy << 8;
			b_energy = int_part(mul(energy, positive(sin(alpha))));
			collided_energy = int_part(mul(energy, positive(cos(alpha))));

			if (aprox)
			{
				b_energy = b_energy << 2;
				collided_energy = collided_energy << 2;
			}

			StartBall(b, b->x0, b->y0, b->angle, b_energy, collided); 
			StartBall(collided, collided->x0, collided->y0, collided->angle, collided_energy, b);
		}
	}
//
//		Ignore for now the colliding of two moving parts...
//	else
//	{
//		//moving ball was collided
//		//so keep existing speeds, but exchange angles
//
//		SetBallSafeCoord(collided);
//
//		tmp = b->angle;
//		StartBall(b, b->x0, b->y0, collided->angle, int_part(mult(speed, sin(alpha))), collided); 
//		StartBall(collided, collided->x0, collided->y0, tmp, int_part(mult(speed, cos(alpha))), b); 
//	}
}

void HandleBall(struct _ball *b)
{
	struct _ball* collided;
	int xmove, ymove;
	int angle;

	short_wait();           // wait a little for each step, then...

	if (b->status == MOVING)
	{	
		b->remainingEnergy--;		// decrement remaining energy

		if (b->remainingEnergy == 0)
		{				// stop the ball
			b->status = STOPPED;
			CountMoving--;
			SetBallSafeCoord(b);
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
				if (CheckYBasket(int_part(b->y)))
				{
					b->status = BASKET;
					CountMoving--;
					hide(b->index, int_part(b->xold), int_part(b->yold));
					b->x = b->y = OUT_OF_RANGE;
					return;
				}

				// compute new angle
				if (b->angle < halfPI)
                      	    		angle = PI - b->angle;
				else
					angle = threePI - b->angle;

				BounceBall(b, angle);

				return;
                       	}
			else if (xmove < 0 && int_part(b->x) <= XMIN)
			{
				if (CheckYBasket(int_part(b->y)))
				{
					b->status = BASKET;
					CountMoving--;
					hide(b->index, int_part(b->xold), int_part(b->yold));
					b->x = b->y = OUT_OF_RANGE;
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
				if (CheckXBasket(int_part(b->x)))
				{
					b->status = BASKET;
					CountMoving--;
					hide(b->index, int_part(b->xold), int_part(b->yold));
					b->x = b->y = OUT_OF_RANGE;
					return;
				}

                               	// compute new angle
                               	angle = twoPI - b->angle;

				BounceBall(b, angle);

				return;
                       	}
                       	else if (ymove != 0 && int_part(b->y) > YMAX)
                       	{
				if (CheckXBasket(int_part(b->x)))
				{
					b->status = BASKET;
					CountMoving--;
					hide(b->index, int_part(b->xold), int_part(b->yold));
					b->x = b->y = OUT_OF_RANGE;
					return;
				}

                               	// compute new angle
                               	angle = twoPI - b->angle;

				BounceBall(b, angle);

				return;
                       	}
                       	else
                       	{
				hide(b->index, int_part(b->xold), int_part(b->yold));
				b->xold = b->x;
				b->yold = b->y;
				show(b->index, b->x, b->y);

                               	b->incr += ONE_STEP;      // ... continue the movement using the old angle
                       	}

			if (xmove != 0 || ymove != 0)
			{	// if ball moved, check for possible collision

				if (collided = CheckCollision(b))
					HandleCollision(b, collided);
			}
		
	}
}

void set_white_ball_rand()
{
	_X0 = 2 * (xrnd() % 20) + 3;
	_Y0 = 2 * (xrnd() % 40) + 3;
}

void set_balls_coord(void)
{
	int x, y;

	for (x = 0; x < 64; x++)
		for (y = 0; y < 48; y++)
			Screen[x][y] = 0;

	CountMoving = 0;

	ball_1.X0 = _X0;
	ball_1.x0 = ball_1.x = ball_1.X0 << 8;
	ball_1.Y0 = _Y0;
	ball_1.y0 = ball_1.y = ball_1.Y0 << 8;

	ball_1.color = WHITE;
	ball_1.index = 1;

	PlaceChar(1, ball_1.Y0 >> 1, ball_1.X0 >> 1, 'o');

	ball_2.X0 = 64 - 10 + 1;
	ball_2.Y0 = 48 + 1;
	ball_2.x0 = ball_2.x = ball_2.X0 << 8;
	ball_2.y0 = ball_2.y = ball_2.Y0 << 8;
	ball_2.index = 2;

	ball_3.X0 = 64 + 10 + 1;
	ball_3.Y0 = 48 + 1;
	ball_3.x0 = ball_3.x = ball_3.X0 << 8;
	ball_3.y0 = ball_3.y = ball_3.Y0 << 8;
	ball_3.index = 3;

	ball_4.X0 = 64 + 1;
	ball_4.Y0 = 48 - 10 + 1;
	ball_4.x0 = ball_4.x = ball_4.X0 << 8;
	ball_4.y0 = ball_4.y = ball_4.Y0 << 8;
	ball_4.index = 4;

	ball_5.X0 = 64 + 1;
	ball_5.Y0 = 48 + 10 + 1;
	ball_5.x0 = ball_5.x = ball_5.X0 << 8;
	ball_5.y0 = ball_5.y = ball_5.Y0 << 8;
	ball_5.index = 5;

	ball_2.color = BLACK;
	ball_3.color = BLACK;
	ball_4.color = BLACK;
	ball_5.color = BLACK;

	ball_2.status = STOPPED;
	ball_3.status = STOPPED;
	ball_4.status = STOPPED;
	ball_5.status = STOPPED;
	
	PlaceChar(2, ball_2.Y0 >> 1, ball_2.X0 >> 1, '*');
	PlaceChar(3, ball_3.Y0 >> 1, ball_3.X0 >> 1, '*');
	PlaceChar(4, ball_4.Y0 >> 1, ball_4.X0 >> 1, '*');
	PlaceChar(5, ball_5.Y0 >> 1, ball_5.X0 >> 1, '*');
}

void place_CH(void)
{
	int n = 0;
	int x = 0, y = 0;
	int _XCH, _YCH;

	if (ball_2.status == STOPPED)
	{
		x += ball_2.X0;
		y += ball_2.Y0;
		n++;
	}

	if (ball_3.status == STOPPED)
	{
		x += ball_3.X0;
		y += ball_3.Y0;
		n++;
	}

	if (ball_4.status == STOPPED)
	{
		x += ball_4.X0;
		y += ball_4.Y0;
		n++;
	}

	if (ball_5.status == STOPPED)
	{
		x += ball_5.X0;
		y += ball_5.Y0;
		n++;
	}

	x = x / n;
	y = y / n;

	_XCH = (x + ball_1.X0) / 2;
	_YCH = (y + ball_1.Y0) / 2;

	do
	{
		XCH =  _XCH + (((xrnd() % 2) == 0) ? (xrnd() % 7) : -(xrnd() % 7));
	}
		while(XCH < XMIN || XCH > XMAX ||
		      abs(XCH - ball_1.X0) < 3 ||
		      abs(XCH - ball_2.X0) < 3 ||
		      abs(XCH - ball_3.X0) < 3 ||
		      abs(XCH - ball_4.X0) < 3 ||
		      abs(XCH - ball_5.X0) < 3);

	XCHfp = XCH << 8;
	colCH = XCH >> 1;

	do
	{
		YCH =  _YCH + (((xrnd() % 2) == 0) ? (xrnd() % 7) : -(xrnd() % 7));
	}
		while(YCH < YMIN || YCH > YMAX ||
		      abs(YCH - ball_1.Y0) < 3 ||
		      abs(YCH - ball_2.Y0) < 3 ||
		      abs(YCH - ball_3.Y0) < 3 ||
		      abs(YCH - ball_4.Y0) < 3 ||
		      abs(YCH - ball_5.Y0) < 3);

	YCHfp = YCH << 8;
	rowCH = YCH >> 1;
}

void move_up_ch(void)
{
	if (rowCH < 45 && ((rowCH + 1 != ball_1.Y0 >> 1) || (colCH != ball_1.X0 >> 1)) &&
			  ((rowCH + 1 != ball_2.Y0 >> 1) || (colCH != ball_2.X0 >> 1)) &&
			  ((rowCH + 1 != ball_3.Y0 >> 1) || (colCH != ball_3.X0 >> 1)) &&
			  ((rowCH + 1 != ball_4.Y0 >> 1) || (colCH != ball_4.X0 >> 1)) &&
			  ((rowCH + 1 != ball_5.Y0 >> 1) || (colCH != ball_5.X0 >> 1)))						
		rowCH = rowCH + 1;
}

void move_down_ch(void)
{
	if (rowCH > 1  && ((rowCH - 1 != ball_1.Y0 >> 1) || (colCH != ball_1.X0 >> 1)) &&
			  ((rowCH - 1 != ball_2.Y0 >> 1) || (colCH != ball_2.X0 >> 1)) &&
			  ((rowCH - 1 != ball_3.Y0 >> 1) || (colCH != ball_3.X0 >> 1)) &&
			  ((rowCH - 1 != ball_4.Y0 >> 1) || (colCH != ball_4.X0 >> 1)) &&
			  ((rowCH - 1 != ball_5.Y0 >> 1) || (colCH != ball_5.X0 >> 1)))						

		rowCH = rowCH - 1;
}

void move_left_ch(void)
{
	if (colCH > 1 && ((colCH - 1 != ball_1.X0 >> 1) || (rowCH != ball_1.Y0 >> 1)) &&
		  	 ((colCH - 1 != ball_2.X0 >> 1) || (rowCH != ball_2.Y0 >> 1)) &&
		  	 ((colCH - 1 != ball_3.X0 >> 1) || (rowCH != ball_3.Y0 >> 1)) &&
		  	 ((colCH - 1 != ball_4.X0 >> 1) || (rowCH != ball_4.Y0 >> 1)) &&
		  	 ((colCH - 1 != ball_5.X0 >> 1) || (rowCH != ball_5.Y0 >> 1)))
		colCH = colCH - 1; 
}

void move_right_ch(void)
{
	if (colCH < 62 && ((colCH + 1 != ball_1.X0 >> 1) || (rowCH != ball_1.Y0 >> 1)) &&
		  	 ((colCH + 1 != ball_2.X0 >> 1) || (rowCH != ball_2.Y0 >> 1)) &&
		  	 ((colCH + 1 != ball_3.X0 >> 1) || (rowCH != ball_3.Y0 >> 1)) &&
		  	 ((colCH + 1 != ball_4.X0 >> 1) || (rowCH != ball_4.Y0 >> 1)) &&
		  	 ((colCH + 1 != ball_5.X0 >> 1) || (rowCH != ball_5.Y0 >> 1)))
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
	
	XCH = (colCH << 1) + 1;
	YCH = (rowCH << 1) + 1;

	AimStatus();

	return (dir == 0xD) ? 1 : 0;
}

void AimStatus(void)
{
	int d;

	if (ball_2.status == STOPPED)
	{
		d = Dist(ball_1.X0, ball_1.Y0, XCH, YCH, ball_2.X0, ball_2.Y0);

		if (positive(d) < 0x200)
		{
			PrintAimStatus(d);
			return;
		}
	}

	if (ball_3.status == STOPPED)
	{
		d = Dist(ball_1.X0, ball_1.Y0, XCH, YCH, ball_3.X0, ball_3.Y0);

		if (positive(d) < 0x200)
		{
			PrintAimStatus(d);
			return;
		}
	}

	if (ball_4.status == STOPPED)
	{
		d = Dist(ball_1.X0, ball_1.Y0, XCH, YCH, ball_4.X0, ball_4.Y0);

		if (positive(d) < 0x200)
		{
			PrintAimStatus(d);
			return;
		}
	}

	if (ball_5.status == STOPPED)
	{
		d = Dist(ball_1.X0, ball_1.Y0, XCH, YCH, ball_5.X0, ball_5.Y0);

		if (positive(d) < 0x200)
		{
			PrintAimStatus(d);
			return;
		}
	}

	PrintAimStatus(0x200);
}

int main(int argc, char** argv)
{
	char ch;
	int force;

	TrainingMode = (argc > 1) ? 1 : 0;

	SetUpAngleTab();
	xrndseed();

	CrtSetup();
	CursorOff();
	Clear();

	printf("The little game of pool\r\n\n");
	printf("Your job is to aim (using '+') and hit the four black (*) balls\r\n");
	printf(" with the white (o) ball, with an appropriate force, in order\r\n");
	printf(" to push them into one of the 6 available pockets...\r\n\n");
	printf("Support for aiming is provided (watch the left part of the top line)\r\n");
	printf("A ball hit with a force equal to N will travel, before stopping,\r\n");
	printf(" a distance equal to 5 x N units of measure;\r\n");
	printf(" (the pool table has 64 x 48 units of measure)\r\n");
	printf("The balls will bounce back when hitting the rim,\r\n");
	printf(" and will collide according to the laws of physics\r\n");
	printf("Due to multiple collisions, several balls might move at the same time\r\n");
	printf("Also, friction will stop eventually the balls movement...\r\n\n");
	printf("Please extend the size of the screen to at least 48 rows x 64 columns\r\n");
	
	if (TrainingMode)
		printf("You are in the training mode...\r\n");

	printf("Hit any key to start:");
	CrtIn();

	show_boundaries();		//clear screen, show boundaries

	set_white_ball_rand();

train:

	set_balls_coord();		//set balls positions

	do
	{
		place_CH();
		PrintChar(rowCH, colCH, '+');

		XCH = (colCH << 1) + 1;
		YCH = (rowCH << 1) + 1;

		ClearTopLine();
		PrintStr("                      ;q=quit or aim (s=left,d=right,x=down,e=up) then hit <cr>");
	
		AimStatus();

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

		XCHfp = XCH << 8;
		YCHfp = YCH << 8;
		
		if (TrainingMode)
		{
			ClearTopLine();
			sprintf(buf, "Aim coords X=%d Y=%d; Hit any key...", XCH, YCH);
			PrintStr(buf);	
			CrtIn();
		}

		do
		{
			ClearTopLine();
			PrintStr("Hit force? (1=smallest, 99=highest):");
			cgets(buf);
			ClearTopLine();
			force = atoi(buf);
		}
		while (force == 0 || force > 99);

		PrintChar(rowCH, colCH, ' ');	//erase crosshairs

		ball_2.ignore = (struct _ball*)0;
		ball_3.ignore = (struct _ball*)0;
		ball_4.ignore = (struct _ball*)0;
		ball_5.ignore = (struct _ball*)0;

		StartBall(&ball_1, ball_1.x0, ball_1.y0, ComputeAngle(ball_1.x0, ball_1.y0, XCHfp, YCHfp), force * 5, (struct _ball*)0);

		do
		{
			if (CountMoving == 1)
				short_wait();

			HandleBall(&ball_1);
			HandleBall(&ball_2);
			HandleBall(&ball_3);
			HandleBall(&ball_4);
			HandleBall(&ball_5);

			if (ball_1.status == BASKET)
			{
				ClearTopLine();
				PrintStr("Bad luck, white ball dropped to pocket! hit any key...");
				CrtIn();

				if (TrainingMode)
					goto train;
				else
					goto quit;
			}
		}
		while(  ball_1.status == MOVING ||
			ball_2.status == MOVING ||
			ball_3.status == MOVING ||
			ball_4.status == MOVING ||
			ball_5.status == MOVING );


		if (ball_2.status == BASKET &&
		    ball_3.status == BASKET &&
		    ball_4.status == BASKET &&
		    ball_5.status == BASKET)
		{
			ClearTopLine();
			PrintStr("Well done, all black balls were dropped to pockets! hit any key...");
			CrtIn();
			break;
		}

	} while (1);

quit:

	CursorOn();
	Clear();
	exit(0);
}
