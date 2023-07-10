
/* Gammon IV, by David C. Oshel, Ames, Iowa */

/*--------------------------------------------------------------------*/
/* GAMEPLAN.C -- separately compiled module, contains the tactics and */
/*               strategy for making the computer's move in Gammon IV */
/*--------------------------------------------------------------------*/

#define TRUE 1
#define FALSE 0
#define ME 1
#define YU 2
#define YRBAR 0
#define MYBAR 25
#define YRHOME 26
#define MYHOME 27
#define ERROR 999
#define MYLEVEL 2

#define bool char

extern int list[2][28];  /* two dice, two lists */

extern struct board { 
	int stones,  /* number of stones on that point */
	    owner,   /* and whose they are */
	    x,y,         /*  x and y coordinates of point base */
	    lastx,lasty, /* last location drawn on this point */
	    cx,cy;       /* coordinates for column numbers */
	}
	point[28], bdsave[28]; /* 24 points, plus 2 bars, 2 homes */

extern struct { int cube, whosecube; } doubles;

extern struct { int fr,to,flag; } pending;

extern int level, dice[2], myscore, yrscore, player, movesleft, cantuse, 
    myturns, swapped, tswap, deciding, expert, tone, show, moremsgline, 
    firstmove, helpdisabled, yrdice, lookforit, startcubevalue;

extern char *chatter[80];

int (*fp4)( int f1, int t1, int f2, int t2 );
int (*fp2)( int a, int b );

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
void putscore(void);
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

/*=============================================*/
/*     M Y   M O V E                           */
/*=============================================*/


bool naked(void) {  /* am I leaving too many blots? */
int i, clink;
	i = 24;  clink = 0;
	while (i) {
		if (point[i].stones == 1 && point[i].owner == ME) clink++;
		i--;
	}
	return (clink > 2);
} 


bool yourfolly(void) { /* look for lotsa blots in your inner table */
int i, clink;
	i = 18; clink = 0;
	while (i < 25) {
		if (point[i].owner == YU && point[i].stones == 1) 
			clink++; 
		i++;
	}
	return (clink >= 3);
} 


bool goodboard(void) { /* look for four made points near my inner table */
int i, clank, clink;
	i = 9; clank = 0;
	while (i > 3) {
		if (point[i].owner == ME && point[i].stones > 1) clank++;
		i--;
	}
	if (clank > 4) return (TRUE);  /* bar is nearly blocked */

	i = 6; clank = clink = 0;
	while (i) {
		if (point[i].owner != ME) 
			;
		else if (point[i].stones == 1) clink++; else clank++;
		i--;
	}
	return (clank > 3 && clink < 2);
}


bool bearoff(void) {
	return (topstone(ME) < 7);
} 


int scanahead( int from ) {
int count;

	count = 0;
	while (--from > 0) {
		++count;
		if ( point[ from ].owner == YU ) return (count);
	}
	return (7);
}


bool endgame(void) { /* Is no strategy required from here on in? */
	return ( (25 - topstone(YU)) > topstone(ME) );
}


/*------------------------------------------------------------*/
/* MATCHUP                                                    */
/*                                                            */
/* 2-stone functions that force the choice of the next move.  */
/* These are the HEART and SOUL of this backgammon algorithm! */
/*------------------------------------------------------------*/

void setpend(int from, int to ) {
	pending.fr   = from;
	pending.to   = to;
	pending.flag = TRUE;
}


void clrpend(void) {
	pending.flag = FALSE;
}


int natural(int f1, int t1, int f2, int t2) {
	clrpend();
	if (point[t2].stones == 1 && t1 == f2) setpend(f2,t2);
	return (pending.flag);
}


bool matchup( int (* test4)() ) {
int i, j, ti, tj;

	if ( pending.flag ) return (FALSE); /* this is probably redundant */

	for (i = 1; i < 26; i++) {
		ti = list[0][i];
		if ( ti == ERROR ) goto zoo;
		for (j = 1; j < 26; j++) {
			tj = list[1][j];
			if ( tj == ERROR ) goto voo;
			if ( (* test4)( i, ti, j, tj ) ) {
				lurch( i, ti, 0);
				return (TRUE);
			}
		   voo: ;
		}
	   zoo: ;
	}
	return (FALSE);
}


/*--------------------------------------------------------*/
/* CLOCKWISE and COUNTERCLOCK                             */
/*                                                        */
/* the rest of these are single-stone decisions based on  */
/* rules of thumb, board-scanning functions               */
/*--------------------------------------------------------*/

bool plainstupid( int from ) { /* don't break a safe point */
	return (from < 13 && (point[from].stones == 2 && scanahead(from) < 7));
} 


bool unwise( int innertablept ) {
	/* if it's a hit, just for god's sake don't put him on the bar!! */
	if ( innertablept < 7 ) {
		if (point[ innertablept ].owner == YU ||
		    point[ YRBAR ].stones > 0) 
			return (TRUE);
	}
	return(FALSE);
} 


bool covermine( int from, int to ) {
	if ( from < 8 ) return(FALSE);
	return ( (point[ to ].stones == 1) && (point[ to ].owner == ME) );
} 


bool idareyou( int from, int to ) {
	if (unwise( to )) return (FALSE);
	if ( (point[ from ].stones != 2)
		&& (point[ to ].stones < 2)
		&& (scanahead( to ) > 6) ) return ( TRUE );
	else return (FALSE);
} 


bool hitandrun( int from, int to ) {
	return ( point[ to ].owner == YU );
} 


bool dbuild( int from, int to ) {
int diceleft;
	diceleft = (myturns? 2 + movesleft: movesleft);
	if (diceleft > 1) {
		/* can't possibly be only one stone on from point */
		/* or kamikaze would have covered it on last move */
		return ( point[to].stones == 0 );
	}
	return (FALSE);
} 


bool kamikaze( int from, int to ) { 
/* cover my distant blot, or razzle-dazzle 'em with the long doubles hit */
int j, k, diceleft;
	
	k = from; 
	j = from - to;
	diceleft = myturns * movesleft;  /* NB: 2*2 == 2+2, "fourtunately" */
	while ( diceleft-- ) {   /* predicting where doubles land is easy! */
		k -= j;
		if (k < 1) return (FALSE); /* out of bounds */
		if ( point[ k ].stones == 0 ) continue;        /* simplify */
		if ( point[ k ].stones == 1 )   /* found my blot or yours? */
			return (TRUE);
		else if ( point[k].owner == YU )   /* found your blockade? */
			return (FALSE); 
		else continue;        /* found my safe point, so ignore it */
	}
	return (FALSE);
}


bool hittite( int from, int to ) {
	return (hitandrun(from,to) && to > 9);
}


bool safehit( int from, int to ) {
	return (hittite(from,to) && idareyou(from,to));
}


bool foolsdemise( int from, int to ) { 
        /* annihilate orphaned blots in enemy's inner, my outer table */
	return (to > 17 && point[to].owner == YU);
}


bool landonme( int from, int to ) {
	if ( plainstupid(from) ) return (FALSE);
	if ( loneranger(from,to) ) {
		if (from < 19 && to > 6) return(TRUE);
		else return (FALSE);
	}
	else return ( point[ to ].owner == ME && point[to].stones < 4);
}



/* these evaluations have meaning only in the endgame */


bool nobackgammon( int from, int to ) { /* endgame */
	return (from > 19);
}


bool crosstable( int from, int to ) { 
	/* always move a table ahead if possible, in the endgame */
	if (from < 7) return (FALSE);
	if (from > 18 && to <= 18) return (TRUE);
	if (from > 12 && to <= 12) return (TRUE);
	if (from >  6 && to <=  6) return (TRUE);
	return (FALSE);
}


bool fiftytworule( int from, int to ) { /* endgame */
int p;
	if (from < 7) return (FALSE);   /* not in inner table! */
	p = from % 6;
	if (p == 0) return (TRUE);  		/* improve the six */
	if (p != 5) return ( (to % 6) < 3 );    /* best improve under five */
}



/* these evaluations are universally applicable, last resort moves */


bool gohome( int from, int to ) { /* always go home if you can */
	return (to == MYHOME);
}


bool scatter( int from, int to ) {  /* scatter, esp. in the endgame */
	if (plainstupid(from) || unwise(to)) return (FALSE);
	return ( point[ from ].stones > 1 && point[ to ].stones == 0 );
}


bool runnerup( int from, int to ) {
	if (from < 10 || from > 18) return (FALSE);
	return (TRUE);
}


bool loneranger( int from, int to ) {
	return( point[ from ].stones == 1 );
}


bool run( int dummy1, int dummy2 ) { /* MUST move something! */
	return (TRUE);
}


/* clockwise and counterclock make a 1-stone choice on rules of thumb */


bool counterclock( int (* test)() ) {
int i,j;

	for (i = 0; i < 2; i++) {
		for (j = 1; j < 25; j++) {
			if ( list[i][j] == ERROR ) continue;
			if ( (* test)( j, list[i][j] ) ) {
				lurch( j, list[i][j], i);
				return ( TRUE );
	}	}	}
	return (FALSE);
}

bool clockwise( int (* test)() ) {
int i,j;

	for (i = 0; i < 2; i++) {
		for (j = 25; j > 0; j--) {
			if ( list[i][j] == ERROR ) continue;
			if ( (* test)( j, list[i][j] ) ) {
				lurch( j, list[i][j], i);
				return ( TRUE );
	}	}	}
	return (FALSE);
}


/*-------------------------------------------*/
/*  Make Prime                               */
/*-------------------------------------------*/

int prmchk;


int buildprime( int f1, int t1, int f2, int t2 ) {
	clrpend();
	/* check for the doubles bug */
	if ((dice[0] == dice[1]) && (point[f1].stones < 2)) return(FALSE);

	/* look for the combination */
	if ( t1 == prmchk && t2 == prmchk) setpend(f2,t2);

	/* stick like glue to a made point, but doubles may move forward */
	if (dice[0] != dice[1]) { 
		if ((f2 < 8) && (point[f2].stones == 2)) clrpend();
		if ((f1 < 8) && (point[f1].stones == 2)) clrpend();
	}

	return(pending.flag);
}

int tab[] = { ERROR,1,2,3,20,22,24,9,4,6,8,5,7 }; 

bool makeprime(void) {
int i; 
	i = 12;
	while (i) {
		prmchk = tab[i]; 
		i--;
		if ( point[ prmchk ].stones > 1 ) continue;
		else if ( matchup( fp4 = buildprime ) ) return(TRUE);
	}
	return(FALSE);
}


bool coverprime( int from, int to ) {
	return (((to == prmchk) && 
		(point[prmchk].owner == ME)) &&
		(point[from].stones != 2));
}


bool cleanup(void) {
int i;
	i = 12;
	while (i) {
		prmchk = tab[i]; 
		i--;
		if ( point[ prmchk ].stones != 1 ) continue;
		else if ( counterclock( fp2 = coverprime ) ) return(TRUE);
	}
	return(FALSE);
}


/*-------------------------------------*/
/* Walking Prime                       */
/*-------------------------------------*/

bool swivelhips( int from, int to ) {
	return ( from > prmchk );
}


bool slink( int from, int to ) {
	return( (from > prmchk) && (point[to].stones == 1) );
}


bool weasel(void) {
	if ( clockwise( fp2 = slink ) )
		return(TRUE);
	if ( counterclock( fp2 = swivelhips ) ) 
		return(TRUE);
	if ( clockwise( fp2 = run ) )
		return(TRUE);
}

bool ihaveprime( int from ) {
int i, to, ez;
	ez = 0;
	for (i = 0; i < 6; i++) {
		to = from - i;
		if ((point[to].owner == ME) && (point[to].stones > 1)) ez++;
	}
	return (ez > 4);
} 


bool walkingprime(void) { 
/* looks for the walking prime anywhere in the front tables */
/* then tries to bring up a runner from behind the prime,   */
/* ensuring that a back stone WILL move before a front one  */
int i;
	i = 12;
	while (i > 5) {
		if ( ihaveprime(i) ) {
			prmchk = i;
			if ( weasel() ) return (TRUE);
		}
		i--;
	}
	return(FALSE);
} 


/*---------- Book Moves ----------*/
/* only valid if my move is first */
/*--------------------------------*/

bool zip(int a, int b, int c, int d) {
	lurch(a,b,0);
	lurch(c,d,0);
	movesleft = 0; return( TRUE );
}


bool zoom( int a, int b, int c, int d, int e, int f, int g, int h ) {
	myturns = 0; zip(a,b,c,d); zip(e,f,g,h); return( TRUE );
}


bool book(void) {
int a,b;
	if (!firstmove) return (FALSE);
	firstmove = FALSE;
	a = dice[0] < dice[1] ? dice[0] : dice[1]; /* min(dice[0],dice[1]); */
	b = dice[0] > dice[1] ? dice[0] : dice[1]; /* max(dice[0],dice[1]); */
	switch (level) {
		case 0: { return ( book0(a,b) ); }
		case 1: { return ( book1(a,b) ); }
		case 2: { return ( book2(a,b) ); }
	}
}

bool book0( int a, int b ) { 
	switch (a) {
		case 1: { switch (b) {
				case 1: return ( zoom(8,7,8,7,6,5,6,5) );
				case 2: return ( zip(24,23,13,11) );
				case 3: return ( zip(8,5,6,5) );
				case 4: return ( zip(24,23,13,9) );
				case 5: return ( zip(24,23,13,8) );
				case 6: return ( zip(13,7,8,7) );
			  }
			break; }
		case 2: { switch (b) {
				case 2: return ( zoom(6,4,6,4,13,11,13,11) );
				case 3: return ( zip(13,11,13,10) );
				case 4: return ( zip(8,4,6,4) );
				case 5: return ( zip(13,8,13,11) );
				case 6: return ( zip(24,18,13,11) );
			  }
			break; }
		case 3: { switch (b) {
				case 3: return ( zoom(13,10,13,10,10,7,10,7) );
				case 4: return ( zip(13,10,13,9) );
				case 5: return ( zip(13,10,13,8) );
				case 6: return ( zip(24,18,13,10) );
			  }
			break; }
		case 4: { switch (b) {
				case 4: return ( zoom(13,9,13,9,24,20,24,20) );
				case 5: return ( zip(13,8,13,9) );
				case 6: return ( zip(24,18,18,14) );
			  }
			break; }
		case 5: { switch (b) {
				case 5: return ( zoom(13,8,13,8,8,3,8,3) );
				case 6: return ( zip(24,18,18,13) );
			  }
			break; }
		case 6: { return ( zoom(13,7,13,7,24,18,24,18) );
			break; }
	}
}

bool book1( int a, int b ) { /* mostly follows Becker */
	switch (a) {
		case 1: { switch (b) {
				case 1: return ( zoom(8,7,8,7,6,5,6,5) );
				case 2: return ( zip(13,11,6,5) );
				case 3: return ( zip(8,5,6,5) );
				case 4: return ( zip(13,9,6,5) );
				case 5: return ( zip(13,8,6,5) );
				case 6: return ( zip(13,7,8,7) );
			  }
			break; }
		case 2: { switch (b) {
				case 2: return ( zoom(6,4,6,4,13,11,13,11) );
				case 3: return ( zip(13,11,13,10) );
				case 4: return ( zip(8,4,6,4) );
				case 5: return ( zip(13,8,13,11) );
				case 6: return ( zip(13,7,7,5) );
			  }
			break; }
		case 3: { switch (b) {
				case 3: return ( zoom(13,10,13,10,8,5,8,5) );
				case 4: return ( zip(13,10,13,9) );
				case 5: return ( zip(13,8,8,5) );
				case 6: return ( zip(13,7,13,10) );
			  }
			break; }
		case 4: { switch (b) {
				case 4: return ( zoom(13,9,13,9,9,5,9,5) );
				case 5: return ( zip(13,8,13,9) );
				case 6: return ( zip(13,7,13,9) );
			  }
			break; }
		case 5: { switch (b) {
				case 5: return ( zoom(13,8,13,8,8,3,8,3) );
				case 6: return ( zip(13,7,13,8) );
			  }
			break; }
		case 6: { return ( zoom(13,7,13,7,24,18,24,18) );
			break; }
	}
}

bool book2( int a, int b ) { /* mostly follows Becker */
	switch (a) {
		case 1: { switch (b) {
				case 1: return ( zoom(8,7,8,7,6,5,6,5) );
				case 2: return ( zip(13,11,24,23) );
				case 3: return ( zip(8,5,6,5) );
				case 4: return ( zip(13,9,24,23) );
				case 5: return ( zip(13,8,24,23) );
				case 6: return ( zip(13,7,8,7) );
			  }
			break; }
		case 2: { switch (b) {
				case 2: return ( zoom(6,4,6,4,24,23,24,23) );
				case 3: return ( zip(13,11,13,10) );
				case 4: return ( zip(8,4,6,4) );
				case 5: return ( zip(13,8,13,11) );
				case 6: return ( zip(13,7,13,11) );
			  }
			break; }
		case 3: { switch (b) {
				case 3: return ( zoom(13,10,13,10,10,7,10,7) );
				case 4: return ( zip(13,10,13,9) );
				case 5: return ( zip(13,8,8,5) );
				case 6: return ( zip(13,7,13,10) );
			  }
			break; }
		case 4: { switch (b) {
				case 4: return ( zoom(13,9,13,9,9,5,9,5) );
				case 5: return ( zip(13,8,13,9) );
				case 6: return ( zip(13,7,13,9) );
			  }
			break; }
		case 5: { switch (b) {
				case 5: return ( zoom(13,8,13,8,8,3,8,3) );
				case 6: return ( zip(13,7,13,8) );
			  }
			break; }
		case 6: { return ( zoom(13,7,13,7,24,18,24,18) );
			break; }
	}
}


/*====== MyMove ======*/

void torve(void) {
	if ( makeprime() ) {  /* this will use doubles, if it can */
			return;
	}
	else if ( walkingprime() ) {  /* i have six prime points, so run!!! */
			return;
	}
	else if ( dice[0] == dice[1] ) {	      /* this is too easy! */
		if ( counterclock( fp2 = kamikaze ) )
			return;
		if ( counterclock( fp2 = dbuild ) ) /* claim new turf */
			return;
		if ( clockwise( fp2 = run ) )
			return;
	}
	else if ( cleanup() ) {  /* cover my single blot on prime points */
			return;
	}
	else if ( bearoff() ) { /* I'm ready, but you're in the back game! */
		if ( counterclock( fp2 = gohome ) )
			return;
		if ( clockwise( fp2 = run ) )
			return; 
	}
	else {
		if ( clockwise( fp2 = hitandrun ) )
			return;
		if ( matchup( fp4 = natural ) )
			return; 
		if ( clockwise( fp2 = landonme ) )
			return;
		if ( counterclock( fp2 = runnerup ) )
			return;
		if ( clockwise( fp2 = scatter ) )
			return;
		if ( clockwise( fp2 = run ) )
			return;
	}
}


void villiers(void) {
	if ( makeprime() ) {  /* this will use doubles, if it can */
			return;
	}
	else if ( walkingprime() ) {  /* i have six prime points, so run!!! */
			return;
	}
	else if ( dice[0] == dice[1] ) {	      /* this is too easy! */
		if ( counterclock( fp2 = kamikaze ) )
			return;
		if ( counterclock( fp2 = dbuild ) ) /* claim new turf */
			return;
		if ( clockwise( fp2 = run ) )
			return;
	}
	else if ( cleanup() ) {  /* cover my single blot on prime points */
			return;
	}
	else if ( bearoff() ) { /* I'm ready, but you're in the back game! */
		if ( counterclock( fp2 = gohome ) )
			return;
		if ( clockwise( fp2 = run ) )
			return; 
	}
	else { 					   
		if ( clockwise( fp2 = foolsdemise ) )
			return;
		if ( clockwise( fp2 = idareyou ) ) 
			return;
		if ( counterclock( fp2 = covermine ) )
			return;
		if ( matchup( fp4 = natural ) )
			return;
		if ( clockwise( fp2 = landonme ) )
			return;
		if ( clockwise( fp2 = runnerup ) )
			return;
		if ( clockwise( fp2 = scatter ) )
			return;
		if ( clockwise( fp2 = run ) )
			return;
	}
}


void louisa(void) {
	if ( makeprime() ) {  /* this will use doubles, if it can */
			return;
	}
	else if ( walkingprime() ) {  /* i have six prime points, so run!!! */
			return;
	}
	else if ( dice[0] == dice[1] ) {	      /* this is too easy! */
		if ( counterclock( fp2 = kamikaze ) )
			return;
		if ( counterclock( fp2 = dbuild ) ) /* claim new turf */
			return;
		if ( clockwise( fp2 = run ) )
			return;
	}
	else if ( cleanup() ) {  /* cover my single blot on prime points */
			return;
	}
	else if ( bearoff() ) { /* I'm ready, but you're in the back game! */
		if ( counterclock( fp2 = gohome ) )
			return;
		if ( clockwise( fp2 = run ) )
			return; 
	}
	else if ( (!naked() && goodboard()) || yourfolly() ) { 
		if ( clockwise( fp2 = hitandrun ) )
			return;
		if ( matchup( fp4 = natural ) )
			return; 
		if ( clockwise( fp2 = landonme ) )
			return;
		if ( counterclock( fp2 = runnerup ) )
			return;
		if ( clockwise( fp2 = scatter ) )
			return;
		if ( clockwise( fp2 = run ) )
			return;
	}
	else { 					   
		if ( clockwise( fp2 = foolsdemise ) )
			return;
		if ( clockwise( fp2 = idareyou ) ) 
			return;
		if ( counterclock( fp2 = covermine ) )
			return;
		if ( matchup( fp4 = natural ) )
			return;
		if ( clockwise( fp2 = landonme ) )
			return;
		if ( clockwise( fp2 = runnerup ) )
			return;
		if ( clockwise( fp2 = scatter ) )
			return;
		if ( clockwise( fp2 = run ) )
			return;
	}
}


void mymove(void) {
int d;

	if ( nomove() ) {
		if (lookforit && (dice[0] != dice[1])) {
			lookforit = FALSE;
			puts("\008... ");
			switch (level) {
				case 0: { puts("Blocked!");  break; }
				case 1: { puts("Well, no!"); break; }
				case 2: { puts("Thurb!");    break; }
			}

			restoreboard();
			update(); 

			/* put the high die in list zero */
			d = dice[0]; dice[0] = dice[1]; dice[1] = d;

			cantuse = ERROR; movesleft = 2; myturns = 1;
			switch (level) {
				case 0: { setchat("I move");  break; }
				case 1: { setchat("Let's try"); break; }
				case 2: { setchat("Move is"); break; }
			}
			debug(chatter);
			prmchk = 12;
			weasel();
			/* the rules say, use both dice if you can, or  */
			/* the highest if one or the other but not both */
		}
		else {
			lookforit = TRUE;
			strcat(chatter," and now I'm blocked ");
			myturns = movesleft = 0;
	}	}
	else if ( book() ) {
		return;
	}
	else if ( pending.flag ) {
		lurch( pending.fr, pending.to, 1 );
		clrpend();
	}
	else if ( endgame() ) {		      /* very solid tactics here!! */
		if ( clockwise( fp2 = gohome ) )
			return;
		if ( clockwise( fp2 = nobackgammon ) ) /* no excuse! */
			return;
		if ( clockwise( fp2 = crosstable ) )
			return;
		if ( clockwise( fp2 = fiftytworule ) )
			return;
		if ( clockwise( fp2 = scatter ) )
			return;
		if ( clockwise( fp2 = run ) ) 
			return;
	}
	else if ( point[ MYBAR ].stones > 0 ) {         /* I'm on the bar! */
		if ( clockwise( fp2 = hitandrun ) )       /* wreak havoc, please */
			return;
		if ( clockwise( fp2 = run ) )     /* note: uses low die first */
			return;
	}
	else switch (level) {
		case 0: { villiers(); break; }
		case 1: { louisa(); break; }
		case 2: { torve(); break; }
	}
}

