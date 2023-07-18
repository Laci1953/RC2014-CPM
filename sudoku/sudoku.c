#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

#define bool char
#define TRUE 1
#define FALSE 0

#define N 9

#define ESCAPE 0x1B
#define CR 0x0D
#define CTRL_C 0x03
#define CTRL_S 0x13

//#define Z80ALL

// 0 means unassigned cells
char grid_nr;
int grid[N][N];
int save[N][N];
int GRID[3][N][N] = 
{
{
{ 3, 0, 6, 5, 0, 8, 4, 0, 0 },
{ 5, 2, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 8, 7, 0, 0, 0, 0, 3, 1 },
{ 0, 0, 3, 0, 1, 0, 0, 8, 0 },
{ 9, 0, 0, 8, 6, 3, 0, 0, 5 },
{ 0, 5, 0, 0, 9, 0, 6, 0, 0 },
{ 1, 3, 0, 0, 0, 0, 2, 5, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 7, 4 },
{ 0, 0, 5, 2, 0, 6, 3, 0, 0 } 
},
{
{ 2, 4, 0, 9, 0, 0, 8, 1, 7 },
{ 0, 6, 0, 0, 0, 7, 0, 5, 0 },
{ 9, 0, 0, 0, 0, 0, 0, 3, 0 },
{ 8, 0, 6, 0, 5, 1, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 6, 0, 0, 9 },
{ 0, 0, 0, 4, 2, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 1, 8, 2, 9, 0, 7, 6, 0 }
},
{
{ 0, 1, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 2, 0, 4, 9, 3, 1, 0, 6 },
{ 6, 8, 9, 5, 1, 7, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 5, 0, 0, 0, 0, 0, 7, 0, 8 },
{ 0, 0, 0, 0, 0, 0, 5, 2, 9 },
{ 0, 0, 0, 0, 0, 1, 0, 5, 2 },
{ 8, 0, 0, 2, 0, 0, 0, 0, 0 },
{ 0, 0, 1, 0, 0, 0, 0, 0, 0 }
}
};

char picture[] = //Y,X,char
{
0,0,'+',
0,1,'-',0,2,'-',0,3,'-',
0,4,'+',
0,5,'-',0,6,'-',0,7,'-',
0,8,'+',
0,9,'-',0,10,'-',0,11,'-',
0,12,'+',
1,0,'|',1,4,'|',1,8,'|',1,12,'|',
2,0,'|',2,4,'|',2,8,'|',2,12,'|',
3,0,'|',3,4,'|',3,8,'|',3,12,'|',
4,0,'+',
4,1,'-',4,2,'-',4,3,'-',
4,4,'+',
4,5,'-',4,6,'-',4,7,'-',
4,8,'+',
4,9,'-',4,10,'-',4,11,'-',
4,12,'+',
5,0,'|',5,4,'|',5,8,'|',5,12,'|',
6,0,'|',6,4,'|',6,8,'|',6,12,'|',
7,0,'|',7,4,'|',7,8,'|',7,12,'|',
8,0,'+',
8,1,'-',8,2,'-',8,3,'-',
8,4,'+',
8,5,'-',8,6,'-',8,7,'-',
8,8,'+',
8,9,'-',8,10,'-',8,11,'-',
8,12,'+',
9,0,'|',9,4,'|',9,8,'|',9,12,'|',
10,0,'|',10,4,'|',10,8,'|',10,12,'|',
11,0,'|',11,4,'|',11,8,'|',11,12,'|',
12,0,'+',
12,1,'-',12,2,'-',12,3,'-',
12,4,'+',
12,5,'-',12,6,'-',12,7,'-',
12,8,'+',
12,9,'-',12,10,'-',12,11,'-',
12,12,'+',
1,13,'1',
2,13,'2',
3,13,'3',
5,13,'4',
6,13,'5',
7,13,'6',
9,13,'7',
10,13,'8',
11,13,'9',
13,1,'1',13,2,'2',13,3,'3',13,5,'4',13,6,'5',13,7,'6',13,9,'7',13,10,'8',13,11,'9',
-1};

void LoadGame(void);
void SaveGame(void);

// --------------------------------------------------------------
// Algorithm by Pradeep Mondal

bool isSafe(int row, int col, int num)
{
	int i,j,x,startRow,startCol;
	
	for (x = 0; x <= 8; x++)
		if (grid[row][x] == num)
			return FALSE;

	for (x = 0; x <= 8; x++)
		if (grid[x][col] == num)
			return FALSE;

	startRow = row - row % 3;
	startCol = col - col % 3;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			if (grid[i + startRow][j + startCol] == num)
				return FALSE;

	return TRUE;
}

bool solveSudoku(int row, int col)
{
	int num;
	
	if (row == N - 1 && col == N)
		return 1;

	if (col == N)
	{
		row++;
		col = 0;
	}

	if (grid[row][col] > 0)
		return solveSudoku(row, col + 1);

	for (num = 1; num <= N; num++)
	{
		if (isSafe(row, col, num))
		{
			grid[row][col] = num;

			if (solveSudoku(row, col + 1))
				return TRUE;
		}
	
		grid[row][col] = 0;
	}
	return FALSE;
}

// --------------------------------------------------------------

void xrndseed(void);
unsigned int xrnd(void);

void clear(void)
{
#ifdef Z80ALL
    putchar( ESCAPE ); putchar( 'H' );	//home
    putchar( ESCAPE ); putchar( 'J' );	//erase to end-of-screen
#else
printf("%c[2J%c[H",27,27);
#endif
}

void gotoxy(int y,int x)
{
#ifdef Z80ALL
    putchar( ESCAPE ); putchar( 'Y' );
    putchar( 31+y+1 );
    putchar( 31+x+1 );
#else
printf("%c[%d;%dH",27,y+1,x+1);
#endif
}

void print_picture(void)
{
	int n=0;

	while (picture[n] != (char)-1)
	{
		gotoxy(picture[n], picture[n+1]);
		putchar(picture[n+2]);
		n += 3;	
	}
}

void print_grid(void)
{
	int x,y;

	for (x = 0; x < N; x++)
		for (y = 0; y < N; y++)
			if (grid[x][y])
			{
				gotoxy(y+1+(y/3), x+1+(x/3));
				putchar(grid[x][y]+'0');
			}
}

void init_grid(int n)
{
	int x, y;

	for (x = 0; x < N; x++)
		for (y = 0; y < N; y++)
			grid[x][y] = GRID[n][x][y];
}

bool full_grid(void)
{
	int x, y;

	for (x = 0; x < N; x++)
		for (y = 0; y < N; y++)
			if (!grid[x][y])
				return FALSE;

	return TRUE;
}

void show(void)
{
	solveSudoku(0, 0);
	print_grid();
	gotoxy(22,0);
	exit(1);
}

void main(void)
{
	char x,y,v,hint;
	int i,j;
	
	xrndseed();
	clear();
	print_picture();

	gotoxy(15,0);
	printf("SUDOKU is a popular game in Japan and worldwide.\r\n"
	       "The objective is to fill a 9x9 grid with digits so that each\r\n"
	       "column, each row, and each of the nine 3x3 subgrids that\r\n"
	       "compose the grid contain all of the digits from 1 to 9.\r\n");

	printf("Load a saved game (Y/y=yes)? :");

	v = getch();
	putchar(v);

	if (v == 'y' || v == 'Y')
	{
		LoadGame();
		clear();
		print_picture();
	}
	else
	{
		printf("\r\nYou will start with a grid already containing some digits,\r\n"
	       		"fit to be successfully completed...\r\n"
	       		"Choose a grid# from 0 to 2 or <CR> to pick a random # :");
choose:
		grid_nr = getch();

		if (grid_nr == CR)
		{
			grid_nr = xrnd() % 3;
			putchar(grid_nr + '0');
		}
		else
		{
			if (!isdigit(grid_nr) || grid_nr > '2')
				goto choose;

			putchar(grid_nr);
			grid_nr -= '0';
		}

		clear();
		print_picture();
		init_grid(grid_nr);
	}

	print_grid();

	gotoxy(15,0);
	printf("Grid #%d: fill in the missing digits...\r\n"
		"(CTRL^C to see the solution & quit, CTRL^S to save game)", grid_nr);

	while (TRUE)
	{
		gotoxy(17,0);
		printf("                                                               \r");

		printf("Row:");

		do
		{
			y = getch();

			if (y == CTRL_C) 
				show();

			if (y == CTRL_S)
				SaveGame();
		}
		while (!isdigit(y) || y == '0');

		putchar(y);

		y = y - 1 - '0';

		printf(" Col:");

		do
		{
			x = getch();

			if (x == CTRL_C)
				show();

			if (y == CTRL_S)
				SaveGame();
		}
		while (!isdigit(x) || x == '0');

		putchar(x);

		x = x - 1 - '0';

		if (grid[x][y])
		{
			printf(" Digit already filled in!(hit any key...)");
			getch();
			continue;
		}

		printf(" Digit:");

		do
		{
			v = getch();

			if (v == CTRL_C) 
				show();

			if (y == CTRL_S)
				SaveGame();
		}
		while (!isdigit(v) || v == '0');

		putchar(v);

		v = v - '0';
 
		if (!isSafe(x, y, v))
		{
			printf(" Wrong digit, see the rules!(hit any key...)");
			getch();
			continue;
		}

		for (i=0; i<N; i++)
			for (j=0; j<N; j++)
				save[i][j] = grid[i][j];

		solveSudoku(0, 0);
		hint = grid[x][y];

		for (i=0; i<N; i++)
			for (j=0; j<N; j++)
				grid[i][j] = save[i][j];

		if (v != hint)
		{
			printf(" Bad choice! Try instead %d!(hit any key...)", hint);
			getch();
			continue;
		}
		else
		{
			printf(" Good choice!(hit any key...)");
			getch();
			grid[x][y] = v;
			print_grid();
			
			if (full_grid())
			{
				printf("\r\nCongratulations, you won!");
				exit(1);
			}
		}
	}
}
