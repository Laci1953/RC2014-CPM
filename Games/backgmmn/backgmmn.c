/***************************************************************************

   GAMMON IV, Version 2
   VT100 version by Anna Christina Nass <acn@acn.wtf>
   HiTech C porting , Game Save/Load by Ladislau Szilagyi

   Author:  David C. Oshel
            1219 Harding Avenue
            Ames, Iowa 50010

   Date:    March 26, 1986

   Gammon IV is probably the best backgammon game currently available for
   CP/M.  I wrote it because I was disgusted with the price and dullness
   of all other programs which allegedly play backgammon on CP/M systems.

   This program has THREE DIFFERENT PLAYING STYLES, any one of which can
   consistently beat a novice player, and occasionally beat a good player.
   In all three levels, the computer's strategy can even seem brilliant;
   there is nothing routine about it.

*****************************************************************************/

#include <conio.h>
#include <ctype.h>

void strcpy(char*, char*);
void strcat(char*, char*);

#define TRUE 1
#define FALSE 0
#define ME 1
#define YOU 2
#define YRBAR 0
#define MYBAR 25
#define YRHOME 26
#define MYHOME 27
#define ERROR 999  /* anything well out of range */

#define ESC 0x1B

#define bool char

int list[2][28];  /* two dice, two lists */

struct board { 
	int stones,  /* number of stones on that point */
	    owner,   /* and whose they are */
	    x,y,         /*  x and y coordinates of point base */
	    lastx,lasty, /* last location drawn on this point */
	    cx,cy;       /* coordinates for column numbers */
	}
point[28], bdsave[28]; /* 24 points, plus 2 bars, 2 homes */

struct { int cube, whosecube; } doubles;

struct { int fr,to,flag; } pending;

int level, dice[2], myscore, yrscore, player, movesleft, cantuse, myturns,
    swapped, tswap, deciding, show, moremsgline, 
    firstmove, yrdice, lookforit, startcubevalue;

char *token1, *token2, chatter[80], buzzard[8];

extern char* TK1;
extern char* TK2;
extern char* TK3;
extern char* TK4;

//backgmmn.c
void xrndseed(void);
unsigned int xrnd(void);
void play(void);
void whofirst(void);
void getmove(void);
void taketurns(void);
void tellmove(void);
void yrmove(void);
bool nomove(void);
bool getyrmove(void);
void setchat( char* p );
void putdice(int f, int t);
void lurch( int f, int t, int zlist );
void checkpips( int whichlist, int ptimon, int pips, int tops );
void build( int whichlist, int pips );
void getlist(void);
int mytotal(void);
int yrtotal(void);
int topstone( int who );
bool cantbearoff( int mypt, int pips, int tops );
bool whosebar( int who );
bool whosehome( int who );
void checkwin(void);
int other( int color );
void winner( int who, int high );
void reverse(void);
void halfswap( int n );
void wipedice(void);
void update(void);
void wipeout(void);
void setup(void);
void newboard(void);
void draw_board(void);
void peek(void);
int getonedie(void);
void fixup(void);
void getdice(void);
void rolldice( int who );
void highroller( int who );
void finishup(void);
void saveboard(void);
void restoreboard(void);
void swaptokens(void);
void blanks( int n );
void msg(char* p);
void debug(char* p);
int nxtyp(int i);
bool isbar(int p);
bool ishome(int p);
void putstone( int pt, int cnt, int color );
void hitblot( int from, int color );
void movestone( int from, int to);
void notyrcube(void);
void notmycube(void);
void barcube(void);
void notbarcube(void);
void mycube(int value);
void yrcube(int value);
void idouble(void);
bool backgame(void);
bool cubeval(void);
void testcube(void);
void udouble(void);
bool endgame(void);
bool bearoff(void);
int getpt(int b, int h);
void cont(void);

//gameplan.c
bool naked(void);
bool yourfolly(void);
bool goodboard(void);
bool bearoff(void);
int scanahead( int from );
bool endgame(void);
void setpend(int from, int to );
void clrpend(void);
int natural(int f1, int t1, int f2, int t2);
bool plainstupid( int from );
bool unwise( int innertablept );
bool covermine( int from, int to );
bool idareyou( int from, int to );
bool hitandrun( int from, int to );
bool dbuild( int from, int to );
bool kamikaze( int from, int to );
bool hittite( int from, int to );
bool safehit( int from, int to );
bool foolsdemise( int from, int to );
bool landonme( int from, int to );
bool nobackgammon( int from, int to );
bool crosstable( int from, int to );
bool fiftytworule( int from, int to );
bool gohome( int from, int to );
bool scatter( int from, int to );
bool runnerup( int from, int to );
bool loneranger( int from, int to );
bool run( int dummy1, int dummy2 );
int buildprime( int f1, int t1, int f2, int t2 );
bool makeprime(void);
bool coverprime( int from, int to );
bool cleanup(void);
bool swivelhips( int from, int to );
bool slink( int from, int to );
bool weasel(void);
bool ihaveprime( int from );
bool walkingprime(void);
bool zip(int a, int b, int c, int d);
bool zoom( int a, int b, int c, int d, int e, int f, int g, int h );
bool book(void);
bool book0( int a, int b );
bool book1( int a, int b );
bool book2( int a, int b );
void torve(void);
void villiers(void);
void louisa(void);
void mymove(void);

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

int atoi(char* p)
{
	int v = 0;

	while (*p && isdigit(*p))
		v = v * 10 + *p++  - '0';
	return v;
}

char GETC(void)
{
	char c;

	c = getch();

	if (isalpha(c))
		c = toupper(c);

	return c;
}

void putstr(char* p)
{
	while (*p)
		putchar(*p++);
}

bool getchars(char* p, int max)
{
	int n;

	for (n = 0; n < max; n++)
	{
		*p = GETC();
		
		if (*p == '?')
			return TRUE;

		if (*p == 13)
		{
			*p = 0;
			return FALSE;
		}
		
		putchar(*p++);
	}

	*p = 0;
	return FALSE;
}

void gotoxy(int x, int y) {
    /* this outputstr the VT100 Escape sequence for cursor positioning */
    putchar( ESC ); putchar( '[' );
    putstr( itoa(y+1) );
    putchar( ';' );
    putstr( itoa(x+1) );
    putchar( 'H' );
} 

void clr_screen(void)  { 
    putchar( ESC ); putstr( "[2J" );
    putchar( ESC ); putchar( 'H' );
}  

/*======================================================================

  OPPONENT -- A little scenario, in which to select level of play

========================================================================*/

void	chooseplayer(void) {

	level = xrnd() % 3;

	clr_screen();

 	draw_board();
} 

/*====================================================================
                              MAIN
======================================================================*/

char *p1 = "Play, Reverse, Swap, New, Quit, Load, (CR for more ...) ";
char *p2 = "Use %s dice, Opponent, Count (CR to go back...) ";

void main(void) {
char ch;

	setup(); 

	while (TRUE) {
		moremsgline = FALSE;  /* show first command line on entry */

		debug(""); /* erase messages */

		firstmove = TRUE;
		newboard(); /* note, sets starting cube value to 1 */

		deciding = TRUE;
		while (deciding) {

			/* display command line */

			if (show) { mytotal(); yrtotal(); }
			else { gotoxy(0,3); blanks(3); gotoxy(0,19); blanks(3); }

			msg("Select:  ");
			if (moremsgline) printf(p2,(yrdice? "my": "your")); 
			else printf(p1); 


			/* get response and do it */
			ch = GETC();
			switch (ch) {
				case 'L':
					msg("");
					LoadGame(); 
					startcubevalue = 1;
					clr_screen();
 					draw_board();
					update();
					cont(); 
					break;
				case 'Q': {  /* quit play, exit to CP/M */
					finishup(); 
					break; 
					}
				case 'R': {  /* mirror board image */
					reverse();
					update();
					break;
					}
				case 'C': {  /* show mytotal, yrtotal counts */
					show ^= TRUE;
					break;
					}
				case 'S': {  /* SWAP Command - exchange stones */
					swaptokens();
					update(); 
					break;
					}
				case 'U':
					yrdice ^= TRUE;
					break;
				case 'O': {  /* change opponents and skill level */
					chooseplayer();
					deciding = FALSE;
					break;
					}
				case 'P':  /* play the game as board is arranged */
					chooseplayer();
					update();
					play(); 
					break;
				case 'N': {  /* abandon game without quitting */
					deciding = FALSE; player = 0;
					break;
					}
				default:  { moremsgline ^= TRUE; break; }
			}
		}
	}
} 

/*=======================================================================

   PLAY Command - this is the command that initiates the 2-player game

=========================================================================*/

void cont(void) { 

	while (TRUE) {  /* NO EXIT!  Only a Win or player ESC can exit */

		rolldice (player);

		getmove();
		player = other(player);
		if ( player == ME ) {
			if ( endgame() ) {
				if (topstone(ME) < 6 && cubeval()) idouble();
				else if ( mytotal() < (yrtotal() - 8) ) {
					idouble();
				}
			}
			else if ( bearoff() ) idouble();
		}
	}
} 

void play(void) {

	whofirst(); 
	taketurns();
} 

void whofirst(void) {
char ch;
int myval, yrval;

	if (yrdice || (player < 0)) {        /* board has been re-arranged */
		msg("Is it My "); putstr(token1);
		putstr(" turn or Your "); putstr(token2);
		putstr(" turn? ");
	loo:	ch = GETC();
		if (!(ch == 'M' || ch == 'Y')) goto loo;
		if (ch == 'M') player = YOU;  /* player says Me, of course! */
		else player = ME;
		rolldice(player);
	}

	else if (player == 0) {  /* fresh start, roll the dice */
		barcube();
	zoo:	debug("Tossing for first turn...");
		wipedice();
		rolldice(ME);
		myval = dice[0];
		rolldice(YOU);  
		yrval = dice[0];
		if (myval == yrval) {
                        gotoxy(37,11); putstr("[___]"); 
                        startcubevalue *= 2;
                        if (startcubevalue > 8) startcubevalue = 8;
			doubles.cube = startcubevalue;
			gotoxy(37,11);
			if (doubles.cube < 16) printf("[ %d ]",doubles.cube);
			else printf("[%03d]",doubles.cube); 
                        if (startcubevalue < 9) {
			    msg("Double the cube!");
                        }
			goto zoo;
		}
		else if (myval < yrval) player = YOU;
		else player = ME;
		dice[0] = (myval < yrval) ? yrval : myval; /* max(myval,yrval); */ 
		dice[1] = (myval < yrval) ? myval : yrval; /* min(myval,yrval); */
	}
	/* otherwise, continue with last dice rolled as play is resumed */
}

void getmove(void) {  
int temp, happy;
char ch;

	cantuse = ERROR;  /* important for human player in tellmove */
	movesleft = 2;
	if (dice[0] == dice[1]) movesleft += 2;
	temp = movesleft;		

	getlist(); saveboard(); lookforit = TRUE;

	if ( nomove() ) {
		debug("All "); 
		if (player == ME) putstr("my"); else putstr("your");
		putstr(" moves are blocked!");
		return;
	}


	if (player == ME) {
		/* handle doubles as two consecutive, independent moves */
		setchat("I move"); debug(chatter);
		if (movesleft == 4) myturns = 2; else myturns = 1;
		clrpend();
		while (myturns > 0) {
			cantuse = ERROR;
			movesleft = 2;
			while (movesleft > 0) { getlist(); mymove(); }
			myturns--;
		}
		strcat(chatter,"\008."); debug(chatter);
	}
	else {  /* allow the human to take back a bad board position */
		happy = FALSE;
		while (!happy) {
			while (movesleft > 0) { getlist(); yrmove(); }
			msg("All ok?  Y/N ");
			do {
				ch = GETC();
			} while (ch != 'N' && (ch != 'Y' && ch != '\n'));
			if ( ch == 'N' ) {
				msg("Ok, as it was...");
				restoreboard();
				update();
				movesleft = temp;
				cantuse = ERROR;
			}
			else happy = TRUE;
			debug("");
		}
	}
} 

void taketurns(void) { 

	while (TRUE) {  /* NO EXIT!  Only a Win or player ESC can exit */

		getmove();
		player = other(player);
		if ( player == ME ) {
			if ( endgame() ) {
				if (topstone(ME) < 6 && cubeval()) idouble();
				else if ( mytotal() < (yrtotal() - 8) ) {
					idouble();
				}
			}
			else if ( bearoff() ) idouble();
		}
		rolldice (player); 
	}
} 

/*=============================================*/
/*     Y O U R   M O V E                       */
/*=============================================*/

void tellmove(void) {  /* show what the player's current dice are */
int k,n;

	n = movesleft;
	debug("You "); 
	putstr(token2); putstr(" ");
	putstr("have "); 
	while (n--) {
		if (dice[0] == dice[1]) k = dice[0];  /* doubles? */

		else if (cantuse == 1) k = dice[0];
		else if (cantuse == 0) k = dice[1];
		else if (n == 1) k = dice[1];	/* 2 of 2? */
		else k = dice[0];		/* 1 of 2? */

		printf("[%d] ",k);
	}
	putstr("left");
	putstr(", moving from high to low ('?' for help)");
}

void yrmove(void) {

	if ( nomove() ) {
		debug("You have no more moves in this line of play.");
		movesleft = 0;
		return;
	}
	else {
loo:		tellmove();
		if (!getyrmove()) {
                     goto loo;
                }
		debug("");
	}
}

bool nomove(void) {
int i,j;
	for (i = 0; i < 2; i++)
		for (j = 0; j < 28; j++)
			if (list[i][j] != ERROR) return (FALSE);
	return (TRUE);
}

bool getyrmove(void) {  
int fpoint, tpoint;

	firstmove = FALSE; /* I got it, I got it! */

	msg("Move from? ");
	fpoint = getpt(YRBAR,YRHOME);
	if ( fpoint == ERROR ||
	     (list[0][fpoint] == ERROR && list[1][fpoint] == ERROR)) {
			return (FALSE);
	}
	putstr(" To? ");
	tpoint = getpt(YRBAR,YRHOME);
	if ( tpoint == ERROR || 
	     (list[0][fpoint] != tpoint && list[1][fpoint] != tpoint)) {
			return (FALSE);
	}

	movestone( fpoint, tpoint );
	if (movesleft < 2) {
		if (list[0][fpoint] == tpoint) cantuse = 0;
		else cantuse = 1;
	}
	return (TRUE);
} 

/*====== Functions That Make The Selected Move ======*/

void setchat( char* p ) {
	strcpy(chatter,p);
} 

void putdice(int f, int t) {
char q[15];
	if (t == MYHOME) sprintf(q," %d to Home,",25 - f);
	else if (f == MYBAR) sprintf(q," Bar to %d,",25 - t);
	else sprintf(q," %d to %d,",25 - f, 25 - t);
	strcat(chatter,q);
	debug(chatter);  /* avoid using save_cursor() */
}

void lurch( int f, int t, int zlist ) {
	movestone( f, t );   /* move the stone */
	putdice(f,t); 	     /* tell user, the action is a bit fast */
	if (movesleft < 2) cantuse = zlist;
} 

/*==========================================================================

  GETLIST -- Find the possible moves for any particular throw of the dice

===========================================================================*/

void checkpips( int whichlist, int ptimon, int pips, int tops ) {
int j,k;

	if ( !ptimon ) {
		j = whosebar(player); 	/* 0 if you, 25 if me  */
		k = (j > pips) ? j - pips : pips - j; /* abs(j - pips); */
	}
	else if (player == ME) {
		j = ptimon;
		k = j - pips;
		if (k < 1) k = MYHOME;
	}
	else {
		j = 25 - ptimon;
		k = j + pips;
		if (k > 24) k = YRHOME;
	}

	if (point[j].stones > 0 && point[j].owner == player) {

		/* no move to a blocked point */
		if (point[k].owner != player && point[k].stones > 1)
			return;

		/* no move home if i can't bear off yet */
		if (k == whosehome(player) && cantbearoff(j,pips,tops)) 
			return;

		/* no other move is allowed if i'm on the bar */
		if (tops == 25 && j != whosebar(player))
			return;

		/* the move is legal (but maybe not optimal) */
		list [whichlist] [j] = k;
	}
} 

void build( int whichlist, int pips ) {
int i, tops;

	if (whichlist == cantuse) return;
	tops = topstone(player);
	for (i = 0; i < 25; i++) checkpips( whichlist, i, pips, tops );
}

void getlist(void) {  /* find all legal moves using these dice */
int i, j;

	for (i = 0; i < 2; i++)  /* initialize the lists */
		for (j = 0; j < 28; j++)
			list [i] [j] = ERROR;

	build( 0, dice[0] ); /* usually the low die  */
	build( 1, dice[1] ); /* usually the high die */
} 

/*==========================================================================

  EVALUATE UTILITIES -- Functions for legal and/or best play, telling who's
			who, who's ahead, who won, etc. etc.

============================================================================*/

int mytotal(void) {
int i, cnt;

	cnt = 0;
	for (i = 0; i < 26; i++) {
		if (point[i].owner == ME) cnt += point[i].stones * i;
	}
	if (show) {
	gotoxy(0,3); printf("%03d",cnt); 
	}
	return (cnt);
}

int yrtotal(void) {
int i, cnt;

	cnt = 0;
	for (i = 0; i < 26; i++) {
		if (point[i].owner == YOU) cnt += point[i].stones * (25 - i);
	}
	if (show) {
	gotoxy(0,19); printf("%03d",cnt); 
	}
	return (cnt);
}

int topstone( int who ) {
int i,j;

	if (point[ whosebar( who ) ].stones > 0) i = 25;
	else {
		i = 24;
		while (i > 0) {
			if (who == ME) j = i; else j = 25 - i;
			if (point[j].stones > 0 &&
			    point[j].owner == who) return (i);
			--i;
		}
	}
	return (i);  /* return normalized value, 1 - 25, 0 is home */
}

bool cantbearoff( int mypt, int pips, int tops ) {

	/* My destination is Home, but can I do it??? */
	if (mypt > 6) mypt = 25 - mypt;  /* normalize inner table */

	/* I can't bear off if there's anybody still not in my inner table */
	if (tops > 6) return (TRUE);

	/* If I'm the highest blot in my own table, I CAN bear off */
	if (tops == mypt) return (FALSE);

	/* If I'm NOT high, I have to have an exact roll to get away with it */
	return ( (mypt != pips) );
}

bool whosebar( int who ) {
	return ( (who == ME? MYBAR: YRBAR) );
}


bool whosehome( int who ) {
	return ( (who == YOU? YRHOME: MYHOME) );
}


void checkwin(void) {  
	if (mytotal() == 0) winner(ME, topstone(YOU));
	if (yrtotal() == 0) winner(YOU, topstone(ME));
}

int other( int color ) {

	if (color == ME) return (YOU); else return (ME);
}

void winner( int who, int high ) {
int gammon; 

	if (point[ whosehome( other(who) )].stones > 0) gammon = 1;
	else gammon = 2;			/* nothing off is a gammon! */
	if (high == 0) gammon = 1;		/* someone doubled */
	else if (high > 18) gammon = 3; 	/* backgammon! */


	debug("");
	if (who == ME) putstr("I"); else putstr("You"); putstr(" win"); 
	switch (gammon) {
		case 1: { putstr("!"); break; }
		case 2: { putstr(" a Gammon!"); break; }
		case 3: { putstr(" a Backgammon!"); break; }
	}
	gammon *= doubles.cube;
	switch (who) {
		case ME: { myscore += gammon; break; }
		case YOU: { yrscore += gammon; break; }
	}

	msg("Hit any key to quit play"); 
	getch();
	exit();
}

/*========================================================================

  REVERSE Command - allow player to take the opponent's viewpoint of the
                    board layout, mirror the board.  Returns board layout
		    to the arranged position as seen from opposite side.

==========================================================================*/

void reverse(void) {
int cnt1, cnt2, cnt3, cnt4;

	cnt1 = point[MYHOME].stones;  /* save counts for erase */
	cnt2 = point[YRHOME].stones;
	cnt3 = point[MYBAR].stones;
	cnt4 = point[YRBAR].stones;

	putstone( MYHOME, 0, 0);  /* erase old trays before update */
	putstone( YRHOME, 0, 0);

	halfswap(1); halfswap(13);

	gotoxy(0,11);  blanks(4);  /* erase HOME message */
	gotoxy(75,11); blanks(4);
	if (point[1].x < 40) {
		point[MYHOME].x = point[YRHOME].x = 0;
	}
	else {
		point[MYHOME].x = point[YRHOME].x = 75;
	}

	point[MYHOME].owner  = point[MYBAR].owner = ME; /* restore counts */
	point[YRHOME].owner  = point[YRBAR].owner = YOU;
	point[MYHOME].stones = cnt1;
	point[YRHOME].stones = cnt2;
	point[MYBAR].stones  = cnt3;
	point[YRBAR].stones  = cnt4;

}

void halfswap( int n ) {
int i, j, k, o;

	o = n + 6;
	for (i = n; i < o; i++) {
		j = ((o * 2) - 1) - i;
		k = point[i].x;
		point[i].cx = point[i].x = point[j].x;
		point[j].cx = point[j].x = k;
	}
}

void wipedice(void) {

	gotoxy(47,11); blanks(18);	 /* erase dice roll messages */
	gotoxy(12,11); blanks(18);	
}

void update(void) {
int i, x, c;

	for (i = 1; i < 25; i++) {
		gotoxy(point[i].cx,point[i].cy);
		printf("%2d",25 - i);
	}
	if (doubles.whosecube == YOU) yrcube(doubles.cube); 
	else if (doubles.whosecube == ME) mycube(doubles.cube);
	else barcube();
	for (i = 0; i < 28; i++) {
		x = point[i].stones;
		c = point[i].owner;
		putstone(i,x,c);
	}
	if (point[1].x < 40) x = 0; else x = 75;
	gotoxy(x,11); putstr("HOME");

	mytotal(); yrtotal();
}

/*==========================================================================

  INITIALIZATION and NEWBOARD commands -- start of a new game, or cold

===========================================================================*/


void wipeout(void) {
int i;

	player = 0;
	barcube();
	for (i = 0; i < 28; i++) {
		point[i].stones = point[i].owner = 0;
	}
	update();
}

void setup(void) {
int i, j, k;

	xrndseed();

	myscore = yrscore = player = dice[0] = dice[1] = 0;
	swapped = tswap = yrdice = FALSE;
	show = moremsgline = TRUE;

	token1 = TK1;
	token2 = TK2;

	draw_board();

	for (i = 0; i < 28; i++) {
		point[i].stones = point[i].owner = 0;
		point[i].x = point[i].y = point[i].lastx = point[i].lasty = 0;
		point[i].cx = point[i].cy = 0;
	}

	k = 68;
	for (i = 1; i < 13; i++ ) { /* establish xy coords for the points */
		j = 25 - i;
		point[i].cx = point[j].cx = point[i].x = point[j].x = k; 
		k -= 5;
		point[i].y = 4;
		point[j].y = 18;
		point[i].cy = 2;
		point[j].cy = 20;
		if (k == 38) k -= 5; /* skip over bar */
	}

	point[MYBAR].x  = point[YRBAR].x  = 38;
	point[MYHOME].x = point[YRHOME].x = 75;

	point[MYBAR].y  = point[MYHOME].y =  5;
	point[YRBAR].y  = point[YRHOME].y = 17;
}

void newboard(void) {

    startcubevalue = 1;
	wipedice(); wipeout();

	putstone( MYHOME, 15, ME  );
	putstone( YRHOME, 15, YOU );

	putstone( YRHOME, 13, YOU );
	putstone(  1, 2, YOU );

	putstone( YRHOME, 8, YOU );
	putstone( 12, 5, YOU );

	putstone( YRHOME, 5, YOU );
	putstone( 17, 3, YOU );

	putstone( YRHOME, 0, 0 ); 
	putstone( 19, 5, YOU );

	putstone( MYHOME, 10, ME );
	putstone(  6, 5, ME  );

	putstone( MYHOME,  7, ME );
	putstone(  8, 3, ME  );

	putstone( MYHOME,  2, ME );
	putstone( 13, 5, ME  );

	putstone( MYHOME, 0, 0 );
	putstone( 24, 2, ME  );
} 

char *picture[] = {
"Gammon IV, by David C. Oshel, Ames, Iowa\n\n\n",
":=================================o=================================:\n",
":: ..   \\/   ..   \\/   ..   \\/   |||  ..   \\/   ..   \\/   ..   \\/  ::\n",
"::                               |||                               ::\n",
":: /\\   ..   /\\   ..   /\\   ..   |||  /\\   ..   /\\   ..   /\\   ..  ::\n"
};

void draw_board(void) {
int line;

	clr_screen();
	gotoxy(0,0);
	blanks(5); printf(picture[0]);
	blanks(5); putstr(picture[1]);
	for (line = 0; line < 6; line++) { 
		blanks(5); putstr(picture[2]);
	}
	blanks(5); putstr(picture[3]);
	blanks(5); putstr(picture[3]);
	blanks(5); putstr(picture[3]);
	for (line = 0; line < 6; line++) {
		blanks(5); putstr(picture[4]);
	}
	blanks(5); putstr(picture[1]);
}

/*=========================================================================

  DICE Commands:  How to roll the dice

===========================================================================*/

void peek(void) {
	if (yrdice) return;  /* you know your own dice, probably...? */
	debug("The Dice will Rattle until you Roll.  Now on ");
	getdice();
	printf("[%d] [%d] ...",dice[0],dice[1]);
} 

int getonedie(void) {
	return ( (xrnd() % 6) + 1 );
} 

void fixup(void) {	/* ensure that the low die is in dice[0] */
int d,e;
	if (player == 0) return;  /* whofirst?  don't mess with the odds */
	d = (dice[0] < dice[1]) ? dice[0] : dice[1]; /* min(dice[0],dice[1]); */
	e = (dice[0] < dice[1]) ? dice[1] : dice[0]; /* max(dice[0],dice[1]); */
	dice[0] = d;
	dice[1] = e;
} 

void getdice(void) {
char ch;

        /* if it's MY dice we're using, generate random dice... */
	if (!yrdice) {
		dice[0] = getonedie(); 
		dice[1] = getonedie();
		fixup();
		return;
	}

        /* but if it's YOUR dice, then ask about the roll... */
zoo:	msg("<> ");
	if (player == ME) putstr("My"); else putstr("Your");
	putstr(" roll: ");
	putstr("\008\008, using your dice: ");
	putstr(" First? ");
loo:	ch = GETC();  /* don't bother acg */
	ch -= '0';
	if (ch < 1 || ch > 6) goto loo;
	putchar( ch + '0');
	dice[0] = ch;

	putstr("  Second? ");
	ch = GETC();
	ch -= '0';
	if (ch < 1 || ch > 6) goto zoo;
	putchar( ch + '0');
	dice[1] = ch;

	putstr("  All Ok? ");
	ch = GETC();  /* don't bother acg */
	if (ch != 'N' && ch != 'Y') goto zoo;
	fixup();
} 

void rolldice( int who ) {
int waiting;

	setchat("Your Turn: ");
	
	if (!yrdice) strcat(chatter," Peek,"); 
	strcat(chatter," Double, Quit, Save, or <AnyKey> to Roll ");

	if (player == YOU) {  /* not executed if player == 0, i.e., whofirst */
		waiting = TRUE; 
		while (waiting) {
			msg(chatter);
			switch ( GETC() ) {
				case 'S': msg(""); SaveGame(); exit();
				case 'P': { peek(); break; }
				case 'D': { udouble(); break; }
				case 'Q': exit();
				default : waiting = FALSE;
		}	}
		debug(""); msg("");
	}
	highroller(who);
} 

void highroller( int who ) {  /* parameter is not redundant */
int y = 11;
int xme, xyu;

	/* get the values for two dice, either yours or mine */

	getdice();

	/* display the values of the dice in the board area */

	xme = 47; xyu = 12;    /* decide which half to show the values in */ 
	if (point[1].x > 40) { 
		xme = 12; 
		xyu = 47; 
	}
	if (player) {
		gotoxy(xyu,y); blanks(18); /* erase, if not whofirst */
		gotoxy(xme,y); blanks(18);
	}
	if (who == ME) {
		gotoxy(xme,y); putstr("My");
	}
	else {
		gotoxy(xyu,y); putstr("Your");
	}
	printf(" Roll> [%d] ",dice[0]);
	if (player) printf("[%d] ",dice[1]); /* whofirst doesn't show this */
} 

void finishup(void) {
	exit();
}

void saveboard(void) {
int i;
	for (i = 0; i < 28; i++) {
		bdsave[i].stones = point[i].stones;
		bdsave[i].owner  = point[i].owner;
	}
} 

void restoreboard(void) {
int i;
	for (i = 0; i < 28; i++) {
		point[i].stones = bdsave[i].stones;
		point[i].owner  = bdsave[i].owner;
	}
} 

void swaptokens(void) {
char *temp;

	swapped ^= TRUE;
	if (swapped) {
		temp = token1;
		token1 = token2;
		token2 = temp;
	}
	else {
		tswap ^= TRUE;
		if (tswap) {
			token1 = TK3;
			token2 = TK4;
		}
		else {
			token1 = TK1;
			token2 = TK2;
		}
	}
} 

void blanks( int n ) {
	while (n--) putchar(' ');
} 

void msg(char* p) {
	gotoxy(5,23); blanks(74);
	gotoxy(5,23); putstr(p); 
}

void debug(char* p) {
	gotoxy(5,22); blanks(74);
	gotoxy(5,22); putstr(p); 
}

int nxtyp(int i) {
	if (i > 9) return ( i - 1 ); else return ( i + 1 );
}

bool isbar(int p) {
   return (( p == MYBAR ) || ( p == YRBAR ));
}

bool ishome(int p) {
   return (( p == MYHOME ) || ( p == YRHOME ));
}

void putstone( int pt, int cnt, int color ) {
int i, xp, yp, slack;
char *background, *token;

	if (cnt < 1) { /* empty point has neither stones nor owner */
            cnt   = 0; 
            color = 0;  
        }

	point[pt].stones = cnt;    /* number of stones on this point */
	point[pt].owner  = color;  /* and whose they are */

        /* stack stones 5 high in the home tray, 6 high on the points */
	if (isbar(pt) || ishome(pt)) slack = 5; else slack = 6;

        /* locate the base address of the point for animation */
	xp = point[pt].x;  
	yp = point[pt].y; 


        /* decide on the background pattern to be used for empty places */
	if (pt > 12) background = "/\\  ";  
	else background = "\\/  ";

	if (point[1].x < 40) {
		if ((pt % 2) == 1) background = "..  ";
	}
	else if ((pt % 2) == 0) background = "..  ";

	if (ishome(pt))    background = "    ";
	if (isbar(pt))     background = "||| ";

        /* get the token pattern to be used */
	if (color == ME) {
		token = token1;  
	}
	else {
		token = token2;
	}

        /* draw the entire point with token and background patterns */

	/* first erase all blots from this point (draw the background) */
		for (i = 0; i < slack; i++) {
			gotoxy(xp,yp);
			putstr(background);     /* string has point's width */
			point[pt].lastx = 0;  /* future, not implemented */
                        point[pt].lasty = 0;
			yp = nxtyp(yp);
		}

	/* now draw all the blots there are on this point onto the point */
		for (i = 0; i < cnt; i++) {
			xp = point[pt].x + (i / slack);
			if ((i % slack) == 0) yp = point[pt].y;
			gotoxy(xp, yp);
			putstr(token);
			point[pt].lastx = xp;  /* future, not implemented */
			point[pt].lasty = yp;
			yp = nxtyp(yp);
		}
} 

void hitblot( int from, int color ) {
int barpt, addone;

	barpt = whosebar( color );
	putstone(from, 0, 0);
	addone = point[barpt].stones + 1;
	putstone( barpt, addone, color );
} 

void movestone( int from, int to) {
int opponent, subone, addone;


	opponent = other( player );
	if (point[to].owner == opponent) hitblot(to, opponent);

	subone = point[from].stones - 1;
	addone = point[to].stones + 1;

	putstone(from, subone, player);
	putstone(to, addone, player);

	--movesleft;
	checkwin();  /* never but NEVER let a win go unnoticed! */
} 

/*========================================================================

  CUBE Commands -- commands related to the cube, doubling, etc.

=========================================================================*/

void notyrcube(void) {
	gotoxy(75,19); blanks(5);
	gotoxy(75,20); blanks(5);
} 

void notmycube(void) {
	gotoxy(75,2); blanks(5);
	gotoxy(75,3); blanks(5);
} 

void barcube(void) {
        /* startcubevalue is normally 1, but it may have doubled */
        /* if the opening rolloff for first turn came up doubles */
	doubles.cube = startcubevalue;
	doubles.whosecube = 0;
	notmycube(); notyrcube();
        gotoxy(37,11);
        if (startcubevalue == 1) putstr("[BAR]");
	else if (doubles.cube < 16) printf("[ %d ]",doubles.cube);
	else printf("[%03d]",doubles.cube); 
} 

void notbarcube(void) {
	gotoxy(37,11); putstr(" BAR ");
} 

void mycube(int value) {
	notbarcube(); gotoxy(75,2); putstr("CUBE");
	gotoxy(75,3); 
	sprintf(buzzard,"[%d]",value); 
	printf("%-5s",buzzard);
	doubles.whosecube = ME;
} 

void yrcube(int value) {
	notbarcube; gotoxy(75,19); putstr("CUBE");
	gotoxy(75,20); sprintf(buzzard,"[%d]",value); 
	printf("%-5s",buzzard);
	doubles.whosecube = YOU;
} 

void idouble(void) {
char ch;

	if (doubles.whosecube == YOU) return;  /* not mine, can't double! */
	if (doubles.cube > 256) return;    /* maximum, don't consider it */
	notbarcube();
	debug("I double.  Will you accept the cube ");
	printf("at %d points? ",doubles.cube * 2);
	loo: 	ch = GETC();
		if (ch == 'Y') {
			notmycube();
			doubles.cube *= 2;
			yrcube(doubles.cube);
		}
		else if (ch == 'N') winner(ME,0);
		else goto loo;
} 

bool backgame(void) {
int max, barred, count, i;
	if (topstone(YOU) < 12 && topstone(ME) > 18) {
		if (mytotal() < yrtotal() + 4) return (TRUE);
		max = barred = count = 0;
		i = 24;
		while (i > 18) {
			if (point[i].owner == ME) {
				max = i;
				if (point[i].stones > 1) barred++;
				count += point[i].stones;
			}
			i--;
		}
		return ((max < topstone(YOU) + 1) && (barred > 1 && count < 7));
	}
	else return ( mytotal() < yrtotal() + 24 );
} 

bool cubeval(void) {
int ineed, yuneed, yrtop, mytop;

	if (endgame()) {

		/* calculate the number of dice that are required to end */
		/* the game, with appropriate fudge factors for position */

		yrtop = topstone(YOU);
		mytop = topstone(ME);

		ineed = 15 - point[MYHOME].stones;
		yuneed = 15 - point[YRHOME].stones;

		if (yrtop < 4 && yuneed < 3) return (FALSE); /* obvious */

		/* topstones still running?  use a different method */
		if (mytop > 6 || yrtop > 6) {
			ineed  = 2 * (mytotal() / 8) + 1; /* number of dice */
			yuneed = 2 * (yrtotal() / 8) + 1;
		}

		/* count the stones on point 6 twice, they're losers */
		yuneed += point[6].stones;
		ineed  += point[6].stones;

		/* you doubled, so you have the roll */
		yuneed -= 2;

		/* odd number left? */
		if (ineed  % 2) ineed++;
		if (yuneed % 2) yuneed++;

		/* is the one point empty? */
		if (ineed  > 4 && mytop > 3 && point[24].stones == 0) ineed++;
		if (yuneed > 4 && yrtop > 3 && point[ 1].stones == 0) yuneed++;

		if (mytop < yrtop && ineed < yuneed) return (TRUE);
		if (yrtop < 5 && yuneed < ineed) return (FALSE);
		return ( yuneed >= ineed );
	}
	else return ( backgame() );
} 

void testcube(void) {

	if (cubeval()) {
		debug("I accept the cube.");
		notyrcube();
		doubles.cube *= 2;
		mycube(doubles.cube);
	}
	else winner(YOU,0);
} 

void udouble(void) {

	if (doubles.whosecube == ME) {
		debug("It's MY cube, dummy!"); 
	}
	else testcube();
}

int getpt(int b, int h) {
char ans[6], *p; 
int x, look, try;

	p = ans;

	if (getchars(ans, 5)) {
		x = ERROR;
		msg("");
		if (point[ whosebar(player) ].stones > 0) {
			putstr("You're on the Bar, so let's move that one!  BAR ");
			x = b;
		}
		else {
			putstr("Are you ");
			if (cantuse != 0 && cantuse != 1) putstr("REALLY ");
			putstr("blocked?  Try moving From ");
			look = 24;
			while (look > 0) {
				try = list[0][look];
				if (try == ERROR) try = list[1][look];
				if ( try != ERROR ) {
					printf("%d To ",25-look);
					if (try == YRHOME) printf("HOME");
					else printf("%d",25-try);
					look = 0;
				}
				look--;
			}
		}

		getch();
	}
	else if (ans[0] == 'B') x = b;
	else if (ans[0] == 'H') x = h;
	else {
		x = atoi( ans );
		if (x < 1 || x > 24) x = ERROR;
		else x = 25 - x;  /* translate human to computer view */
	}
	return (x);
} 
