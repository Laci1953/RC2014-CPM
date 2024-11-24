// Game of MAZE for VT100 compatible terminals
//
// Ladislau Szilagyi, November 2024
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void xrndseed(void);
unsigned int xrnd(void);

void CrtSetup(void);
char CrtIn(void);
void CrtOut(char);
void putstr(char*);

#define ENTER		0xD
#define BLANK		0x20
#define REV_BLANK	219

char Screen[64][25];

#define ESC	0x1B
#define CTRL_C	3

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

// (0,0) = (botom, left)
void SetYX(int y, int x)
{
	CrtOut(ESC);
	CrtOut('[');
	putstr(itoa(26-y));
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

void PrintChar(int y, int x, char ch)
{
	SetYX(y, x);
	CrtOut(ch);
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

void InitScreen(void)
{
	int x, y;

	for (x = 0; x < 64; x++)
		for (y = 0; y < 25; y++)
			Screen[x][y] = BLANK;

	for (y = 0; y <= 24; y = y + 2)
		for (x = 0; x < 64; x++)
			Screen[x][y] = REV_BLANK;

	for (y = 1; y <= 23; y = y + 2)
		Screen[0][y] = Screen[63][y] = REV_BLANK;

	Screen[16][1] = '0'; Screen[32][1] = REV_BLANK; Screen[53][1] = '1';
	Screen[16][2] = BLANK; Screen[53][2] = BLANK;
	Screen[7][3] = '2'; Screen[16][3] = '3'; Screen[53][3] = '4';
	Screen[7][4] = BLANK;
	Screen[7][5] = '5'; Screen[16][5] = '6'; Screen[46][5] = '7';
	Screen[16][6] = BLANK; Screen[46][6] = BLANK;
	Screen[16][7] = '8'; Screen[24][7] = '9'; Screen[32][7] = REV_BLANK; Screen[46][7] = 'a'; Screen[53][7] = 'b';
	Screen[24][8] = BLANK; Screen[53][8] = BLANK;
	Screen[24][9] = 'c'; Screen[46][9] = 'd'; Screen[53][9] = 'e';
	Screen[46][10] = BLANK;
	Screen[7][11] = 'f'; Screen[46][11] = 'g'; Screen[58][11] = 'h';
	Screen[7][12] = BLANK; Screen[58][12] = BLANK;
	Screen[7][13] = 'i'; Screen[24][13] = 'j'; Screen[32][13] = REV_BLANK; Screen[53][13] = 'k'; Screen[58][13] = 'l';
	Screen[24][14] = BLANK; Screen[53][14] = BLANK;
	Screen[24][15] = 'm'; Screen[32][15] = 'n'; Screen[53][15] = 'o';
	Screen[32][16] = BLANK;
	Screen[7][17] ='p'; Screen[32][17] = 'q';
	Screen[7][18] = BLANK;
	Screen[7][19] ='r'; Screen[24][19] = 's'; Screen[53][19] = 't';
	Screen[24][20] = BLANK; Screen[53][20] = BLANK;
	Screen[7][21] = 'u'; Screen[24][21] = 'v'; Screen[32][21] = REV_BLANK; Screen[46][21] = 'w'; Screen[53][21] = 'x';
	Screen[7][22] = BLANK; Screen[46][22] = BLANK;
	Screen[7][23] = 'y'; Screen[46][23] = 'z';
}

// returns x + (y << 8)
int Find(char ch)
{
	int x, y;

	for (x = 1; x < 63; x++)
		for (y = 1; y < 24; y++)
			if (Screen[x][y] == ch)
				return x + (y << 8);

	return 0;
}

void short_wait(void)
{
	int i;

	for (i=0; i<5000; i++) 
		;
}

#define LEFT 	3
#define RIGHT 	4
#define UP 	1
#define DOWN 	2

char MoveBall(char ch)
{
	int x, y, pos;
	int dir;

	pos = Find('*');

	x = pos & 0xFF;
	y = pos >> 8;

	if (ch == '<')
		dir = LEFT;
	else
		dir = RIGHT;

loop:

	if (dir == LEFT)
	{
		if (x == 1)
			return 0;

		if (Screen[x-1][y] == BLANK)
		{
			Screen[x-1][y] = '*';
			PrintChar(y, x-1, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			short_wait();
			x = x - 1;
			goto loop;
		}
		else if (Screen[x-1][y] == '\\')
		{
			Screen[x-1][y+1] = '*';
			PrintChar(y+1, x-1, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			short_wait();
			x = x - 1;
			y = y + 1;
			dir = UP;
			goto loop;
		}
		else if (Screen[x-1][y] == '/')
		{
			Screen[x-1][y-1] = '*';
			PrintChar(y-1, x-1, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			short_wait();
			x = x - 1;
			y = y - 1;
			dir = DOWN;
			goto loop;
		}
		else if (Screen[x-1][y] == '+')
			return 1;
		else
			return 0;
	}
	else if (dir == RIGHT)
	{
		if (x == 62)
			return 0;

		if (Screen[x+1][y] == BLANK)
		{
			Screen[x+1][y] = '*';
			PrintChar(y, x+1, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			short_wait();
			x = x + 1;
			goto loop;
		}
		else if (Screen[x+1][y] == '\\')
		{
			Screen[x+1][y-1] = '*';
			PrintChar(y-1, x+1, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			short_wait();
			x = x + 1;
			y = y - 1;
			dir = DOWN;
			goto loop;
		}
		else if (Screen[x+1][y] == '/')
		{
			Screen[x+1][y+1] = '*';
			PrintChar(y+1, x+1, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			short_wait();
			x = x + 1;
			y = y + 1;
			dir = UP;
			goto loop;
		}
		else if (Screen[x+1][y] == '+')
			return 1;
		else
			return 0;
	}
	else if (dir == UP)
	{
		if (Screen[x][y+1] == BLANK)
		{
			Screen[x][y+1] = '*';
			PrintChar(y+1, x, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			return 0;
		}
		else if (Screen[x][y+1] == '\\')
		{
			Screen[x-1][y+1] = '*';
			PrintChar(y+1, x-1, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			x = x - 1;
			y = y + 1;
			dir = LEFT;
			goto loop;
		}
		else if (Screen[x][y+1] == '/')
		{
			Screen[x+1][y+1] = '*';
			PrintChar(y+1, x+1, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			x = x + 1;
			y = y + 1;
			dir = RIGHT;
			goto loop;
		}
		else
			return 0;
	}
	else //DOWN
	{
		if (Screen[x][y-1] == BLANK)
		{
			Screen[x][y-1] = '*';
			PrintChar(y-1, x, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			return 0;
		}
		else if (Screen[x][y-1] == '\\')
		{
			Screen[x+1][y-1] = '*';
			PrintChar(y-1, x+1, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			x = x + 1;
			y = y - 1;
			dir = RIGHT;
			goto loop;
		}
		else if (Screen[x][y-1] == '/')
		{
			Screen[x-1][y-1] = '*';
			PrintChar(y-1, x-1, '*');
			short_wait();
			Screen[x][y] = BLANK;
			PrintChar(y, x, BLANK);
			x = x - 1;
			y = y - 1;
			dir = LEFT;
			goto loop;
		}
		else
			return 0;
	}
}

int main(void)
{
	int x, y, pos;
	char tmp;

	xrndseed();

	CrtSetup();
	CursorOff();

	printf("You have to cross a maze with a '*' ball,\r\n in order to reach the '+' target\r\n");
	printf("Once started, the ball will move,\r\n until it reaches the target or hits an obstacle\r\n");
	printf("The maze is composed of horizontal corridors,\r\n connected by small vertical passages\r\n");
	printf("In total, there are 16 different conformations of the maze\r\n");
	printf("In the horizontal corridors, near the vertical passages,\r\n are placed letters or digits,\r\n");
	printf(" which must be configured to allow vertical movement of the ball\r\n");
	printf("For example, if the ball is moving to the right,\r\n and we want it to go up,\r\n");
	printf(" we will place a '/' in its path.\r\n");
	printf("Similarly, if the ball is moving to the left,\r\n and we want it to go up,\r\n");
	printf(" we will place a '\\' in its path.\r\n");
	printf("Also, to remove a letter or digit from a horizontal corridor,\r\n");
	printf(" you can replace it with a <blank>.\r\n");
	printf("Good luck!\r\nPlease extend the size of the screen\r\n to at last 26 rows x 64 columns!\r\n, then hit any key to start...");

	CrtIn();
	Clear();
	InitScreen();

	switch (xrnd() % 4)
	{
		case 0:
			// do nothing
			break;
		case 1:
			// switch lines
			for (x = 1; x < 63; x++)
				// xchg rows 1-23, 2-22, 3-21, 4-20, 5-19, 6-18, 7-17, 8-16, 9-15, 10-14, 11-13
				for (y = 1; y <= 11; y++)
				{
					tmp = Screen[x][y];
					Screen[x][y] = Screen[x][24-y];
					Screen[x][24-y] = tmp;
				}
			break;
		case 2:
			// switch columns
			for (y = 1; y < 24; y++)
				// xchg cols 1-62, 2-61, ... 31-32
				for (x = 1; x <=31; x++)
				{
					tmp = Screen[x][y];
					Screen[x][y] = Screen[63-x][y];
					Screen[63-x][y] = tmp;
				}
			break;
		case 3:
			// switch lines & columns
			for (x = 1; x < 63; x++)
				// xchg rows 1-23, 2-22, 3-21, 4-20, 5-19, 6-18, 7-17, 8-16, 9-15, 10-14, 11-13
				for (y = 1; y <= 11; y++)
				{
					tmp = Screen[x][y];
					Screen[x][y] = Screen[x][24-y];
					Screen[x][24-y] = tmp;
				}
			for (y = 1; y < 24; y++)
				// xchg cols 1-62, 2-61, ... 31-32
				for (x = 1; x <=31; x++)
				{
					tmp = Screen[x][y];
					Screen[x][y] = Screen[63-x][y];
					Screen[63-x][y] = tmp;
				}
			break;
	}

	switch (xrnd() % 4)
	{
		case 0:
			Screen[1][1] = '*';
			Screen[62][23] = '+';
			break;
		case 1:
			Screen[1][1] = '+';
			Screen[62][23] = '*';
			break;
		case 2:
			Screen[1][23] = '*';
			Screen[62][1] = '+';
			break;
		case 3:
			Screen[1][23] = '+';
			Screen[62][1] = '*';
			break;
	}

	for (x = 0; x < 64; x++)
		for (y = 0; y < 25; y++)
			PrintChar(y, x, Screen[x][y]);

do
{
	do
	{
		ClearTopLine();
		PrintStr("Hit Ctrl^C to quit, or a letter/digit, or ENTER to finish setup");

		tmp = CrtIn();

		if (tmp == CTRL_C)
			goto quit;

		if (tmp == ENTER)
			break;

		if ( (tmp >= '0' && tmp <= '9') ||
		     (tmp >= 'a' && tmp <= 'z') )
		{
			pos = Find(tmp);

			if (pos != 0)
			{
				PrintChar(pos >> 8, pos & 0xFF, BLANK);
				short_wait();
				PrintChar(pos >> 8, pos & 0xFF, tmp);
				short_wait();
				PrintChar(pos >> 8, pos & 0xFF, BLANK);
				short_wait();
				PrintChar(pos >> 8, pos & 0xFF, tmp);

				ClearTopLine();
				PrintStr("Hit / or \\ or <blank> to change-it");

				tmp = CrtIn();

				switch (tmp)
				{
					case '\\': Screen[pos & 0xFF][pos >> 8] = '\\'; PrintChar(pos >> 8, pos & 0xFF, '\\'); break;
					case '/': Screen[pos & 0xFF][pos >> 8] = '/'; PrintChar(pos >> 8, pos & 0xFF, '/'); break;
					case BLANK: Screen[pos & 0xFF][pos >> 8] = BLANK; PrintChar(pos >> 8, pos & 0xFF, BLANK); break;
					default: break;
				}
			}
		}
	} while(1);

	ClearTopLine();
	PrintStr("Hit < or > to start the ball, anything else to quit...");

	tmp = CrtIn();

	if (tmp != '<' && tmp != '>')
		goto quit;

	if (MoveBall(tmp))
	{
		Clear();
		printf("\r\nWell done, congratulations!");
		CursorOn();
		exit(0);
	}
} while(1);

quit:

	Clear();
	CursorOn();
	exit(0);
}
