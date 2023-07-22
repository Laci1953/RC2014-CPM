// Connect Four game
//
// Using Gaelen McIntee's hint about computer's move strategy
//
// Ladislau Szilagyi, July 2023

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>

//uncomment for VT52/Z80ALL, else it will be VT100 compatible
//#define Z80ALL

#define bool char
#define TRUE 1
#define FALSE 0

#define ROWS 6
#define COLS 7

#define ESCAPE 0x1B

char board[ROWS][COLS];
char toMove;	// O = player, X = computer

void xrndseed(void);
unsigned int xrnd(void);

void clear(void)
{
#ifdef Z80ALL
    putchar( ESCAPE ); putchar( 'H' );	//home
    putchar( ESCAPE ); putchar( 'J' );	//erase to end-of-screen
#else
printf("%c[2J%c[H",ESCAPE,ESCAPE);
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

void eraseToEOS(void)
{
#ifdef Z80ALL
    putchar( ESCAPE ); putchar( 'J' );	//erase to end-of-screen
#else
printf("%c[J",ESCAPE);
#endif
}

// Quit if Ctrl^C is hit
char GETCH(void)
{
	char c;

	c = getch();

	if (c == 3)
		exit(1);

	return c;
}

// Print the board
void print_board(void)
{
	int i,j;

	printf("                    **** CONNECT FOUR ****\r\n");
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLS; j++)
		{
			putch(' ');
			putch(board[i][j]);
		}

		printf("\r\n\r\n");
	}

	printf(" 1 2 3 4 5 6 7");
}

// Place the chip (toMove) in the given column. 
// col = 0...6 
// returns FALSE if column is already full 
bool place_chip(int col)
{
	int r;

	for (r = ROWS - 1; r >= 0; r--)
		if (board[r][col] == ' ')
		{
			board[r][col] = toMove;
			return TRUE;
		}

	return FALSE;
}

// Returns TRUE if table is full
bool isFull(void)
{
	int col;

	for (col = 0; col < COLS; col++)
	{
		if (board[0][col] == ' ')
			return FALSE;
	}

	return TRUE;
}

// Returns 
//	O : you won 
//	X : computer won
//	blank if no winner yet
char winnerIs(void)
{
	char cell;
	int offset;
	int row, col, i;

	for (row = 0; row < ROWS; row++)
		for (col = 0; col < COLS; col++)
		{
			cell = board[row][col];

			if (cell == ' ')
				continue;

			/* Horizontal case */
			for ( i = col; i < col+4; i++)
			{
				if (i == COLS || board[row][i] != cell)
					break;

				if (i == col+3) 
					return cell;
			}

			/* Vertical case */
			for ( i = row; i < row+4; i++)
			{
				if (i == ROWS || board[i][col] != cell)
					break;

				if (i == row+3)
					return cell;
			}

			/* Up and Right case */
			for ( i = col, offset = 0; i < col+4; i++, offset++)
			{
				if (i == COLS || (row-offset) < 0 || board[row-offset][i] != cell)
					break;

				if (i == col+3)
					return cell;
			}

			/* Up and Left case */
			for ( i = col, offset = 0; i > col-4; i--, offset++)
			{
				if (i < 0 || (row-offset) < 0 || board[row-offset][i] != cell)
					break;

				if (i == col-3) 
					return cell;
			}
		}

	return ' ';
}

// Returns TRUE is column is full
bool colFull(int col)
{
	return (board[0][col] != ' ');
}

// Returns a random non-full column (0 to 6)
int randomInt(void)
{
	int i;

	do
		i = xrnd() % 7;
	while (colFull(i));

	return i;
}

// Check for 2 connected chips
int getMove2(void)
{
	int row, col;

	for (row = 0; row < ROWS; row++)
	{
		for (col = 0; col < COLS; col++)
		{
			if (board[row][col] == 'O')
			{
				//   O
				//row O
				//    c
				//    o
				//    l

				if (col >= 2 && row >= 2) 
				{
					if (board[row-1][col-1] == 'O')
					{
						if (board[row-2][col-2] == ' ' && board[row-1][col-2] != ' ') 
						{
							if (!colFull(col-2))
								return col-2;
						}
					}
				}

				//row O
				//   Oc
				//    o
				//    l

				if (col >= 2 && row < ROWS-2) 
				{
					if (board[row+1][col-1] == 'O')
					{
						if (board[row+2][col-2] == ' ') 
						{
							if (row >= ROWS-3) 
							{
								if (!colFull(col-2))
									return col-2;
							}
							else if (board[row+3][col-2] != ' ') 
							{
								if (!colFull(col-2))
									return col-2;
							}
						}
					}
				}

				//     
				//    O
				//row O
				//    c
				//    o
				//    l

				if (row >= 2 && board[row-2][col] == ' ' && board[row-1][col] == 'O') 
				{
					if (!colFull(col))
						return col;
				}

				//  OO row
				//   c
				//   o
				//   l

				if (col >= 2 && board[row][col-2] == ' ' && board[row][col-1] == 'O')
				{
					if (row == ROWS-1) 
					{
						if (!colFull(col-2))
							return col-2;
					}
					else if (board[row+1][col-2] != ' ')
					{
						if (!colFull(col-2))
							return col-2;
					}
				}

				//   OO row
				//   c
				//   o
				//   l

				if (col < COLS-2 && board[row][col+1] == 'O' && board[row][col+2] == ' ')
				{
					if (row == ROWS-1) 
					{
						if (!colFull(col+2))
							return col+2;
					}
					else if (board[row+1][col+2] != ' ')
					{
						if (!colFull(col+2))
							return col+2;
					}
				}
			}
		}
	}

	// no match, pick randomly a non-full column
	return randomInt();
}

// Check for 3 connected chips (part II)
int getMove3b(void)
{
	int row, col;

	for (row = 0; row < ROWS; row++)
	{
		for (col = 0; col < COLS; col++)
		{
			if (board[row][col] == 'O')
			{
				// OO O row
				//    c
				//    o
				//    l

				// O OO row
				//    c
				//    o
				//    l

				if (col >= 3 && board[row][col-3] == 'O') 
				{
					if (board[row][col-2] == 'O') 
					{
						if (row = ROWS-1)
 						{
							if (!colFull(col-1))
								return col-1;
						} 
						else if (board[row+1][col-1] != ' ')
						{
							if (!colFull(col-1))
								return col-1;
						}
					}
					else if (board[row][col-1] == 'O')
					{
						if (row = ROWS-1) 
						{
							if (!colFull(col-2))
								return col-2;
						} 
						else if (board[row+1][col-2] != ' ') 
						{
							if (!colFull(col-2))
								return col-2;
						}
					}
				}

				//O   c
				// O  o
				//    l
				//row O

				//O   c
				//    o
				//   Ol
				//row O

				if (col >= 3 && row >= 3) 
				{
					if (board[row-3][col-3] == 'O')
					{
						if (board[row-2][col-2] == 'O') 
						{
							if (board[row-1][col-1] == ' ' && board[row][col-1] != ' ') 
							{
								if (!colFull(col-1))
									return col-1;
							}
						}
						else if (board[row-1][col-1] == 'O') 
						{
							if (board[row-2][col-2] == ' ' && board[row-1][col-2] != ' ') 
							{
								if (!colFull(col-2))
									return col-2;
							}
						}
					}
				}

				//c  O
				//o O
				//l
				//O row

				//c  O
				//o 
				//lO
				//O row

				if (col < COLS-3 && row >= 3) 
				{
					if (board[row-3][col+3] == 'O')
					{
						if (board[row-2][col+2] == 'O') 
						{
							if (board[row-1][col+1] == ' ' && board[row][col+1] != ' ') 
							{
								if (!colFull(col+1))
									return col+1;
							}
						}
						else if (board[row-1][col+1] == 'O') 
						{
							if (board[row-2][col+2] == ' ' && board[row-1][col+2] != ' ') 
							{
								if (!colFull(col+2))
									return col+2;
							}
						}
					}
				}
			}
		}
	}

	// no match, try to find 2 connected chips...
	return getMove2();
}

// Check for 3 connected chips (part I)
int getMove3a(void)
{
	int row, col;

	for (row = 0; row < ROWS; row++)
	{
		for (col = 0; col < COLS; col++)
		{
			if (board[row][col] == 'O')
			{
				//
				//  O
				//   O
				//row O
				//    c
				//    o
				//    l

				if (col >= 3 && row >= 3) 
				{
					if (board[row-1][col-1] == 'O' && board[row-2][col-2] == 'O')
					{
						if (board[row-3][col-3] == ' ' && board[row-2][col-3] != ' ') 
						{
							if (!colFull(col-3))
								return col-3;
						}
					}
				}

				if (col >= 2 && col < COLS-1 && row >= 2 && row < ROWS-2) 
				{
					if (board[row-1][col-1] == 'O' && board[row-2][col-2] == 'O')
					{
						if (board[row+1][col+1] == ' ' && board[row+2][col+1] != ' ') 
						{
							if (!colFull(col+1))
								return col+1;
						}
					}
				}

				//row O
				//   Oc
				//  O o
				//    l

				if (col >= 3 && row < ROWS-3) 
				{
					if (board[row+1][col-1] == 'O' && board[row+2][col-2] == 'O')
					{
						if (board[row+3][col-3] == ' ') 
						{
							if (row == ROWS-4) 
							{
								if (!colFull(col-3))
									return col-3;
							}
							else if (board[row+4][col-3] != ' ') 
							{
								if (!colFull(col-3))
									return col-3;
							}
						}
					}
				}

				if (col >= 2 && col < COLS-1 && row >= 1 && row < ROWS-2) 
				{
					if (board[row+1][col-1] == 'O' && board[row+2][col-2] == 'O')
					{
						if (board[row-1][col+1] == ' ') 
						{
							if (board[row][col+1] != ' ') 
							{
								if (!colFull(col+1))
									return col+1;
							}
						}
					}
				}

				//
				//    O
				//    O
				//row O
				//    c
				//    o
				//    l

				if (row >= 3 && board[row-3][col] == ' ' && board[row-2][col] == 'O' && board[row-1][col] == 'O') 
				{
					if (!colFull(col))
						return col;
				}

				// OOO row
				//   c
				//   o
				//   l

				if (col >= 3 && board[row][col-3] == ' ' && board[row][col-2] == 'O' && board[row][col-1] == 'O')
				{
					if (row == ROWS-1) 
					{
						if (!colFull(col-3))
							return col-3;
					}
					else if (board[row+1][col-3] != ' ')
					{
						if (!colFull(col-3))
							return col-3;
					}
				}

				//   OOO row
				//   c
				//   o
				//   l

				if (col < COLS-3 && board[row][col+1] == 'O' && board[row][col+2] == 'O' && board[row][col+3] == ' ')
				{
					if (row == ROWS-1) 
					{
						if (!colFull(col+3))
							return col+3;
					}
					else if (board[row+1][col+3] != ' ')
					{
						if (!colFull(col+3))
							return col+3;
					}
				}
			}
		}
	}

	return getMove3b();
}

// player 	O
// computer	X
void main(void)
{
	int i,j;
	char c;

	xrndseed();

	// init board (fill-it with blanks)
	for (i = 0; i < ROWS; i++)
		for (j = 0; j < COLS; j++)
			board[i][j] = ' ';

	clear();
	print_board();

	printf("\r\n\r\n\r\nConnect Four is a two-player game, in which the players take\r\n"
		" turns dropping chips (you = O, computer = X) into a six-row,\r\n"
		" seven-column vertically suspended grid. The pieces fall\r\n"
		" straight down, occupying the lowest available space.\r\n"
		"To win, a player must form a horizontal, vertical or diagonal\r\n"
		" line of four of his own chips. Use Ctrl^C to quit...\r\n\r\n");

	printf("Do you want to move first? (Y/y = yes) : ");
	c = GETCH();

	if (isascii(c))
		c = toupper(c);
	
	toMove = (c == 'Y') ? 'O' : 'X';

	while (1)
	{
		if (toMove == 'O')
		{
again:
			gotoxy(16, 0);	//keep line 15 (the computer last move)
			eraseToEOS();

			printf("Choose column to drop your chip into (1 to 7) : ");
			c = GETCH();
			putchar(c);

			if (!isdigit(c) || c == '0' || c > '7')
				goto again;

			c = c - '0' - 1;

			if (!place_chip(c))
			{
				printf("\r\nColumn is full! (hit any key)");
				GETCH();
				goto again;
			}

			toMove = 'X';
		}
		else
		{	
			gotoxy(15,0);
			eraseToEOS();

			c = getMove3a();
			
			printf("Computer drops a chip into the column %d...", c+1);

			place_chip(c);	// no need to check 'column full'

			toMove = 'O';
		}

		gotoxy(0, 0);
		print_board();

		c = winnerIs();

		if (c != ' ')
		{
			gotoxy(15, 0);
			eraseToEOS();
			printf("You %s!", (c == 'O') ? "won" : "lost");
			exit(1);
		}

		if (isFull())
		{
			gotoxy(15, 0);
			eraseToEOS();
			printf("It's a draw (all columns are full) !");
			exit(1);
		}
	}
}
