// Text Fall
//
// Matrix digital rain style console game
//
// VT100 or VT52 compatible
//
// Szilagyi Ladislau, August 2023

#include <stdio.h>
#include <conio.h>
#include <string.h>

//uncomment for VT52/Z80ALL, else it will be VT100 compatible
#define Z80ALL

#define bool char
#define TRUE 1
#define FALSE 0

#ifdef Z80ALL
#define COLS	64
#define ROWS	48
#define WAIT	750
#else
#define COLS	80
#define ROWS	24
#define WAIT	250
#endif

#define ESCAPE 0x1B
#define CTRL_C 3
#define BLANK 0x20
#define EOT	4

typedef struct _dlink
{
	struct _dlink*	prev;
	struct _dlink*	next;
};

typedef struct _dlist
{
	struct _dlink	link;
	int	x;
	int	y;
};

struct _dlink	Header = { &Header, &Header };
struct _dlist	List[150];
struct _dlist*	Crt = List;
 
bool AutoRepeat = FALSE;
int spoon_x = (COLS / 2) - 1;
int spoon_y = ROWS-1;
int dropped = 0;
int catched = 0;
int missed = 0;
int counter_drop;
int COUNTER = 20;
int counter_add_letter_to_drop;
int counter_falling_chars = 0;
int MAX_FALLING = 10;
bool lettersAvailable = TRUE;
char* Spoon = "\\_/";
char* Text="the quick brown fox jumps over the lazy dog";
char* Help="s=slower f=faster l=less falls m=more falls ^C=quit space=pause";
char intbuf[6];

char screen[COLS][ROWS];

void xrndseed(void);
unsigned int xrnd(void);

void CrtSetup(void);
void CrtOut(char);
char CrtIn(void);
bool CrtSts(void);
void putstr(char*);

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

void clear(void)
{
#ifdef Z80ALL
    CrtOut( ESCAPE ); CrtOut( 'H' );	//home
    CrtOut( ESCAPE ); CrtOut( 'J' );	//erase to end-of-screen
#else
    putstr("\x1B[2J\x1B[H");
#endif
}

void gotoxy(int x, int y)
{
#ifdef Z80ALL
    CrtOut( ESCAPE ); CrtOut( 'Y' );
    CrtOut( 31+y+1 );
    CrtOut( 31+x+1 );
#else
    CrtOut( ESCAPE);
    CrtOut( '[' );
    putstr(itoa(y+1));
    CrtOut( ';' );
    putstr(itoa(x+1));
    CrtOut( 'H' );
	//putstr("%c[%d;%dH",27,y+1,x+1);
#endif
}

void eraseToEOL(void)
{
#ifdef Z80ALL
    CrtOut( ESCAPE ); CrtOut( 'K' );
#else
    putstr("\x1B[K");
#endif
}

char	*cOFF = "\x1B[?25l";
void	CursorOFF(void)
{
	putstr(cOFF);
}

char	*cON = "\x1B[?25h";
void	CursorON(void)
{
	putstr(cON);
}


#define K_LEFT	1
#define K_RIGHT	2

/* Return key from keyboard, according to key bindings
   ---------------------------------------------------
*/

/*
ARROW L ESC [ D
ARROW R ESC [ C
*/

/*
FOR Z80ALL:

Keyboard's Arrow keys:

left	=	'4'
right	=	'6'

sequence: (1...n times)[ 0 key ] then 0 4
	
*/

char GetKey(void)
{
        char c;

back:
       	c = CrtIn();

#ifdef	Z80ALL

	if (!c)			/* if zero, the next one is an arrow key or EOT = 4 for end-of-sequence */
	{
		AutoRepeat = TRUE;
again:
		c = CrtIn();
repeat:
		switch (c)
		{
			case '6': return K_RIGHT;
			case '4': return K_LEFT;
			case EOT: AutoRepeat = FALSE;
				  return 0;  /* do nothing */
			case 0:	  goto again;
			default:  AutoRepeat = FALSE;
				  goto cont;
		}
	}

	if (AutoRepeat)
		goto repeat;
cont:

#else

	if (c == ESCAPE)
	{
		AutoRepeat = TRUE;

		c = CrtIn();
try2:
		if (c == '[')
		{
			c = CrtIn();
try:
			switch(c)
			{
				case 'C': AutoRepeat = FALSE; return K_RIGHT;
				case 'D': AutoRepeat = FALSE; return K_LEFT;
				case 'A': AutoRepeat = FALSE; return 99;
				case 'B': AutoRepeat = FALSE; return 99;
				default:  goto back;
			}
		}
		else
			goto try;
	}
	else if (AutoRepeat)
		goto try2;

#endif

	return c;
}

struct _dlist* AppendNew(void)
{
	struct _dlist* new, * last;

	new = Crt++;
	last = Header.prev;
	last->link.next = new;
	new->link.prev = last;
	new->link.next = &Header;
	Header.prev = new;

	return new;
}

void Remove(struct _dlist* item)
{
	struct _dlist* prev, * next;

	prev = item->link.prev;
	next = item->link.next;

	prev->link.next = next;
	next->link.prev = prev;

	counter_falling_chars--;
}

bool FindNextLetterToDrop(void) 
{
	int x, y, yd;
	bool ob;
	struct _dlist* new;

	for ( y = 1; y < 4; y++ )
	{
		for ( x = 0; x < COLS; x++ )
		{
			if (((screen[x][y] & 0x80) == (char)0) && // not 'falling'
			     (screen[x][y] != ' ')) // and not blank
			{
				// found a letter, now see if it has free path to bottom...
				ob = FALSE;

				for ( yd = y+1; yd < ROWS-1; yd++ )
					if (screen[x][yd] != ' ')
					{
						ob = TRUE;
						break;
					}

				if (!ob)
				{
					screen[x][y] |= 0x80; // mark as 'falling'

					new = AppendNew();
					new->x = x;
					new->y = y;

					dropped++;
					counter_falling_chars++;

					return TRUE;
				}
			}
		}
	}

	return FALSE;	// all letters consumed
}

// returns FALSE if list empty
bool MoveFirstDown(void)
{
	char c;
	struct _dlist* first, * last;
	struct _dlist* prev, * next;

	first = Header.next;

	if (first == &Header)
		return FALSE;

	c = screen[first->x][first->y];

	// erase letter from screen
	screen[first->x][first->y] = ' ';

	gotoxy(first->x, first->y);
	CrtOut(' ');

	// check for bottom
	if (first->y == ROWS-2)
	{
		// spoon below?
		if (first->x == spoon_x || first->x == spoon_x+1 || first->x == spoon_x+2)
			catched++;
		else
			missed++;

		Remove(first);	//remove letter from dropping letters
	}
	else
	{
		// move down letter
		(first->y)++;

		screen[first->x][first->y] = c;

		gotoxy(first->x, first->y);
		CrtOut(c & 0x7F); // erase 'falling' mark

		// remove item from list 
		prev = first->link.prev;
		next = first->link.next;

		prev->link.next = next;
		next->link.prev = prev;

		// and append-it as last one
		last = Header.prev;
	
		last->link.next = first;
		first->link.prev = last;
		first->link.next = &Header;
		Header.prev = first;
	}
	
	return TRUE;
}

void wait10ms(void)
{
	int i;

	for (i = 0; i < WAIT; i++)
		;
}

void inject_text(void)
{
	int x, y, len;
	char* p;

	clear();

	for (y = 1; y < 4; y++)
	{
		p = Text;
		len = strlen(Text);
		x = xrnd() % (COLS - len);

		gotoxy(x, y);
		putstr(Text);

		do
			screen[x++][y] = *p++;
		while (--len);
	}
}

void get_key_and_do_action(void)
{
	char c;

	if (CrtSts())
	{
		c = GetKey();

		switch (c)
		{
			case CTRL_C:	
					CursorON();
					exit();
			case K_LEFT:
					if (spoon_x)
					{
						screen[spoon_x][spoon_y] = 
						screen[spoon_x+1][spoon_y] = 
						screen[spoon_x+2][spoon_y] = ' ';
						spoon_x--;
						gotoxy(0, ROWS - 1);
						eraseToEOL();
						gotoxy(spoon_x, ROWS - 1);
						putstr(Spoon);
						screen[spoon_x][spoon_y] = '\\';
						screen[spoon_x+1][spoon_y] = '_';
						screen[spoon_x+2][spoon_y] = '/';
					}
					break;
			case K_RIGHT:
					if (spoon_x < COLS - 3)
					{
						screen[spoon_x][spoon_y] = 
						screen[spoon_x+1][spoon_y] = 
						screen[spoon_x+2][spoon_y] = ' ';
						spoon_x++;
						gotoxy(0, ROWS - 1);
						eraseToEOL();
						gotoxy(spoon_x, ROWS - 1);
						putstr(Spoon);
						screen[spoon_x][spoon_y] = '\\';
						screen[spoon_x+1][spoon_y] = '_';
						screen[spoon_x+2][spoon_y] = '/';
					}
					break;
			case BLANK:
					gotoxy(0, 0);
					eraseToEOL();
	    				putstr("Game paused, hit any key to resume...");
	    				CrtIn(); 
	    				gotoxy(0,0);
					eraseToEOL();
					putstr(Help);
					break;
			case 's':	
					COUNTER *= 2; 
					break; 	
			case 'f':	
					if (COUNTER <= 5)
						break;
					COUNTER /= 2;
					break;
			case 'l':	if (MAX_FALLING <= 5)
						break;
					MAX_FALLING /= 2;
					break;
			case 'm':	if (MAX_FALLING >= 20)
						break;
					MAX_FALLING *= 2;
					break;
			default:	
					break;
		}
	}
}

void main(void)
{
	int x,y;

	CrtSetup();
	xrndseed();

	for (x = 0; x < COLS; x++)
		for (y = 0; y < ROWS; y++)
			screen[x][y] = ' ';

	clear();
	gotoxy(0, 0);

	putstr("Text Rain is a game inspired from the 'Matrix digital rain'\r\n\n"
		"You must catch, using a 'spoon' ( \\_/ ), letters starting to\r\n"
		" drop from the text fragments randomly displayed on the screen.\r\n");
	putstr("The letters are dropped from the highest available position\r\n"
		" allowing a free descent path.\r\n"
		"Once a letter is catched, the spoon will be 'emptied',\r\n"
		" allowing the next letter to be catched.\r\n");
	putstr("Letters not catched will 'sink' at the bottom, being 'lost'.\r\n"
		"When all the letters falled, game ends and some statistics\r\n"
		" will be displayed (#catched, #missed)\r\n\n"
		"Move the spoon using the 'arrow keys' (left, right).\r\n"
		"Use: s=drop letters slower,  f=drop letters faster,\r\n"
		"     l=less falling letters, m=more falling letters,\r\n"
		"     space bar to pause,     Ctrl C to quit.\r\n\n"
		"Hit any key to start...");

	CrtIn();

	CursorOFF();

	inject_text();

	gotoxy(0, 0);
	putstr(Help);

	gotoxy(spoon_x, ROWS - 1);
	putstr(Spoon);

	screen[spoon_x][spoon_y] = '\\';
	screen[spoon_x+1][spoon_y] = '_';
	screen[spoon_x+1][spoon_y] = '/';

	counter_drop = COUNTER;
	counter_add_letter_to_drop = 50;

	while (TRUE)
	{
		wait10ms();
		get_key_and_do_action();

		counter_drop--;
		counter_add_letter_to_drop--;

		if (!counter_add_letter_to_drop)
		{
			counter_add_letter_to_drop = 50;	// each 500ms, try to add next letter to drop

			if (counter_falling_chars < MAX_FALLING)
				lettersAvailable = FindNextLetterToDrop();
		}

		if (!counter_drop)
		{
			counter_drop = COUNTER;

			//, move 1 line down first letter from list
			if (!MoveFirstDown() && !lettersAvailable)
			{
				//print statistics
				clear();
				CursorON();
				gotoxy(0, 0);
				printf("Letters dropped: %d catched: %d missed: %d", dropped, catched, missed);
				exit();
			}
		}
	}
}
