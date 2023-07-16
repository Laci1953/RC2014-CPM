/* A fairly hacked-up version of sokoban, for CP/M.
 * ported 95/7/28 by Russell Marks
 *
 * It compiles ok (with the usual warnings :-)) under Hitech C.
 * All the source files were basically cat'ted together to make compliation
 * less awkward.
 *
 * You'll need to patch the 'clear' and 'move' routines with your
 * machine/terminal's clear screen and cursor move codes. (They're currently
 * set for ZCN.) For very simple terminals, you may even be able to patch
 * the binary directly and not bother recompiling.
 *
 * This version is for VT100 compatible terminals, patched by
 * Anna Christina Nass <acn@acn.wtf>
 *
 * Some cleanup + arrow keys + VT52 support by Ladislau Szilagyi, 2023
 *
 */

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//#define Z80ALL

/**/
/* OBJECT: this typedef is used for internal and external representation */
/*         of objects                                                    */
/**/
typedef struct {
   char obj_intern;	/* internal representation of the object */
   char obj_display1;	/* first  display char for the object		 */
   char obj_display2;	/* second display char for the object		 */
   short invers;	/* if set to 1 the object will be shown invers */
} OBJECT;

/**/
/* You can now alter the definitions below.
/* Attention: Do not alter `obj_intern'. This would cause an error */
/*            when reading the screenfiles                         */
/**/
 OBJECT 
   player = 	 { '@', '@', '@', 0 },
   playerstore = { '+', '@', '@', 1 },
   store = 	 { '.', 'o', 'o', 0 },
   packet = 	 { '$', '[', ']', 0 },
   save = 	 { '*', '<', '>', 1 },
   ground = 	 { ' ', ' ', ' ', 0 },
   wall = 	 { '#', '#', '#', 1 };

/*************************************************************************
********************** DO NOT CHANGE BELOW THIS LINE *********************
*************************************************************************/
#define MAXROW		20
#define MAXCOL		40

typedef struct {
   short x, y;
} POS;

#define E_FOPENSCREEN	1
#define E_PLAYPOS1	2
#define E_ILLCHAR	3
#define E_PLAYPOS2	4
#define E_TOMUCHROWS	5
#define E_TOMUCHCOLS	6
#define E_USAGE		7
#define E_LEVELTOOHIGH	8
#define E_ENDGAME	9

/* defining the types of move */
#define MOVE 		1
#define PUSH 		2
#define SAVE 		3
#define UNSAVE 		4
#define STOREMOVE 	5
#define STOREPUSH 	6

/* defines for control characters */
#define CNTL_L		'\014'
#define CNTL_K		'\013'
#define CNTL_H		'\010'
#define CNTL_J		'\012'
#define CNTL_R		'\022'
#define CNTL_U		'\025'

 POS   tpos1,		   /* testpos1: 1 pos. over/under/left/right */
       tpos2,		   /* testpos2: 2 pos.  "                    */
       lastppos,	   /* the last player position (for undo)    */
       last_p1, last_p2; /* last test positions (for undo)         */
 char lppc, ltp1c, ltp2c;    /* the char for the above pos. (for undo) */
 char action, lastaction;

/** For the temporary save **/
 char  tmp_map[MAXROW+1][MAXCOL+1];
 short tmp_pushes, tmp_moves, tmp_savepack;
 POS   tmp_ppos;

short level, packets, savepack, moves, pushes, rows, cols;

char  map[MAXROW+1][MAXCOL+1];
POS   ppos;

char *message[] = {
   "???",
   "cannot open screen file",
   "more than one player position in screen file",
   "illegal char in screen file",
   "no player position in screenfile",
   "too much rows in screen file",
   "too much columns in screenfile",
   "illegal command line syntax",
   "level number too big in command line"
};

char	AutoRepeat = 0;

#define EOT	4
#define ESCAPE	0x1B

void domove(short moveaction);
void undomove(void);
void tmpsave(void);
void tmpreset(void);
void showscreen(void) ;
void mapchar(char c, short i, short j);
void displevel(void);
void disppackets(void);
void dispsave(void);
void dispmoves(void);
void disppushes(void);

void clear()
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

int GetKey(void)
{
        char c;

back:
       	c = getch();

#ifdef	Z80ALL

	if (!c)			/* if zero, the next one is an arrow key or EOT = 4 for end-of-sequence */
	{
		AutoRepeat = 1;
again:
		c = getch();
repeat:
		switch (c)
		{
			case '8': return 'k'; //UP
			case '2': return 'j'; //DOWN;
			case '6': return 'l'; //RIGHT;
			case '4': return 'h'; //LEFT;
			case EOT: AutoRepeat = 0;
				  return 9999;  /* do nothing */
			case 0:	  goto again;
			default:  AutoRepeat = 0;
				  goto cont;
		}
	}

	if (AutoRepeat)
		goto repeat;
cont:

#else

	if (c == ESCAPE)
	{
		AutoRepeat = 1;

		c = getch();
try2:
		if (c == '[')
		{
			c = getch();
try:
			switch(c)
			{
				case 'A': AutoRepeat = 0; return 'k'; //UP
				case 'B': AutoRepeat = 0; return 'j'; //DOWN;
				case 'C': AutoRepeat = 0; return 'l'; //RIGHT;
				case 'D': AutoRepeat = 0; return 'h'; //LEFT;
				default:  goto back;
			}
		}
		else
			goto try;
	}
	else if (AutoRepeat)
		goto try2;

#endif

	return (int)c;
}

short readscreen(void)
{
   FILE *screen;
   short j, c, f, ret = 0;

   if( (screen = fopen( "soklevls.dat", "r")) == NULL)
      ret = E_FOPENSCREEN;
   else
   {
      if(level>1) 
      {
         for(f=1;f<level;f++) 
         {
            while((c=getc(screen))!=12 && c!=EOF) ;
	    if (c == EOF)
	    {
		printf(message[E_LEVELTOOHIGH]);
		exit(0);
	    }
            getc(screen);	/* get the \n after */
         }
      }

      packets = savepack = rows = j = cols  = 0;
      ppos.x = -1; ppos.y = -1;
      while( (ret == 0) && ((c = getc( screen)) != 12) && c!=EOF) {
         if( c == '\n') {
	    map[rows++][j] = '\0';
	    if( rows > MAXROW) 
	       ret = E_TOMUCHROWS;
	    else {
	       if( j > cols) cols = j;
	       j = 0;
	    }
	 }
	 else if( (c == player.obj_intern) || (c == playerstore.obj_intern)) {
	    if( ppos.x != -1) 
	       ret = E_PLAYPOS1;
	    else { 
	       ppos.x = rows; ppos.y = j;
	       map[rows][j++] = c;
	       if( j > MAXCOL) ret = E_TOMUCHCOLS;
	    }
	 }
	 else if( (c == save.obj_intern) || (c == packet.obj_intern) ||
		  (c == wall.obj_intern) || (c == store.obj_intern) ||
		  (c == ground.obj_intern)) {
	    if( c == save.obj_intern)   { savepack++; packets++; }
	    if( c == packet.obj_intern) packets++;
	    map[rows][j++] = c;
	    if( j > MAXCOL) ret = E_TOMUCHCOLS;
	 }
	 else ret = E_ILLCHAR;
      }
      fclose( screen);
      if( (ret == 0) && (ppos.x == -1)) ret = E_PLAYPOS2;
   }
   return( ret);
}

char testmove(register short action)
{
   short ret;
   char  tc;
   short stop_at_object;

   if( (stop_at_object = iscntrl( action))) action = action + 'A' - 1;
   action = (isupper( action)) ? tolower( action) : action;
   if( (action == 'k') || (action == 'j')) {
      tpos1.x = (action == 'k') ? ppos.x-1 : ppos.x+1;
      tpos2.x = (action == 'k') ? ppos.x-2 : ppos.x+2;
      tpos1.y = tpos2.y = ppos.y;
   }
   else {
      tpos1.y = (action == 'h') ? ppos.y-1 : ppos.y+1;
      tpos2.y = (action == 'h') ? ppos.y-2 : ppos.y+2;
      tpos1.x = tpos2.x = ppos.x;
   }
   tc = map[tpos1.x][tpos1.y];
   if( (tc == packet.obj_intern) || (tc == save.obj_intern)) {
      if( ! stop_at_object) {
         if( map[tpos2.x][tpos2.y] == ground.obj_intern)
            ret = (tc == save.obj_intern) ? UNSAVE : PUSH;
         else if( map[tpos2.x][tpos2.y] == store.obj_intern)
            ret = (tc == save.obj_intern) ? STOREPUSH : SAVE;
         else ret = 0;
      }
      else ret = 0;
   }
   else if( tc == ground.obj_intern)
      ret = MOVE;
   else if( tc == store.obj_intern)
      ret = STOREMOVE;
   else ret = 0;
   return( ret);
}

short play(void)
{
   short c;
   short ret;
   short undolock = 1;		/* locked for undo */

   showscreen();
   tmpsave();
   ret = 0;

   while( ret == 0) {
      c=GetKey();
      switch(c) { case '8': c = 'k'; break;
                  case '2': c = 'j'; break;
                  case '4': c = 'h'; break;
		  case '6': c = 'l'; break;
                  default: break; };

      switch(c) {
	 case 'q': /* quit the game 					*/
	              ret = E_ENDGAME; 
	              break;
   
         case 's':/* temporary save					*/
		      tmpsave();
		      break;
	  
         case 'r':	/* reset to temporary save 			*/
		      tmpreset();
		      undolock = 1;
		      showscreen();
		      break;
	 case 'U':    /* undo this level 				*/
		      moves = pushes = 0;
		      if( (ret = readscreen()) == 0) {
		         showscreen();
			 undolock = 1;
		      }
		      break;
	 case 'u':    /* undo last move 				*/
		      if( ! undolock) {
		         undomove();
		         undolock = 1;
		      }
		      break;

    	 case 'k':    /* up 						*/
	 case 'K':    /* run up 					*/
	 case CNTL_K: /* run up, stop before object 			*/
	 case 'j':    /* down 						*/
	 case 'J':    /* run down 					*/
	 case CNTL_J: /* run down, stop before object 			*/
	 case 'l':    /* right 						*/
	 case 'L':    /* run right 					*/
	 case CNTL_L: /* run right, stop before object 			*/
	 case 'h':    /* left 						*/
	 case 'H':    /* run left 					*/
	 case CNTL_H: /* run left, stop before object 			*/
		      do {
		         if( (action = testmove( c)) != 0) {
			    lastaction = action;
		            lastppos.x = ppos.x; lastppos.y = ppos.y;
		            lppc = map[ppos.x][ppos.y];
		            last_p1.x = tpos1.x; last_p1.y = tpos1.y; 
		            ltp1c = map[tpos1.x][tpos1.y];
		            last_p2.x = tpos2.x; last_p2.y = tpos2.y; 
		            ltp2c = map[tpos2.x][tpos2.y];
		            domove( lastaction); 
		            undolock = 0;
		         }
		      } while( (action != 0) && (! islower( c))
			      && (packets != savepack));
		      break;
            default:    ; break;
      }

      if( (ret == 0) && (packets == savepack)) 
	 break;
   }

   return( ret);
}

void domove(short moveaction) 
{
   map[ppos.x][ppos.y] = (map[ppos.x][ppos.y] == player.obj_intern) 
			       ? ground.obj_intern 
			       : store.obj_intern;
   switch( moveaction) {
      case MOVE:      map[tpos1.x][tpos1.y] = player.obj_intern; 	break;
      case STOREMOVE: map[tpos1.x][tpos1.y] = playerstore.obj_intern; 	break;
      case PUSH:      map[tpos2.x][tpos2.y] = map[tpos1.x][tpos1.y];
		      map[tpos1.x][tpos1.y] = player.obj_intern;	
		      pushes++;						break;
      case UNSAVE:    map[tpos2.x][tpos2.y] = packet.obj_intern;
		      map[tpos1.x][tpos1.y] = playerstore.obj_intern;		
		      pushes++; savepack--;			 	break;
      case SAVE:      map[tpos2.x][tpos2.y] = save.obj_intern;
		      map[tpos1.x][tpos1.y] = player.obj_intern;			
		      savepack++; pushes++;				break;
      case STOREPUSH: map[tpos2.x][tpos2.y] = save.obj_intern;
		      map[tpos1.x][tpos1.y] = playerstore.obj_intern;		
		      pushes++;						break;
   }
   moves++;
   dispmoves(); disppushes(); dispsave();
   mapchar( map[ppos.x][ppos.y], ppos.x, ppos.y);
   mapchar( map[tpos1.x][tpos1.y], tpos1.x, tpos1.y);
   mapchar( map[tpos2.x][tpos2.y], tpos2.x, tpos2.y);
   gotoxy( MAXROW+1, 0);
   
   ppos.x = tpos1.x; ppos.y = tpos1.y;
}

void undomove(void) 
{
   map[lastppos.x][lastppos.y] = lppc;
   map[last_p1.x][last_p1.y] = ltp1c;
   map[last_p2.x][last_p2.y] = ltp2c;
   ppos.x = lastppos.x; ppos.y = lastppos.y;
   switch( lastaction) {
      case MOVE:      moves--;				break;
      case STOREMOVE: moves--;				break;
      case PUSH:      moves--; pushes--;		break;
      case UNSAVE:    moves--; pushes--; savepack++;	break;
      case SAVE:      moves--; pushes--; savepack--;	break;
      case STOREPUSH: moves--; pushes--;		break;
   }
   dispmoves(); disppushes(); dispsave();
   mapchar( map[ppos.x][ppos.y], ppos.x, ppos.y);
   mapchar( map[last_p1.x][last_p1.y], last_p1.x, last_p1.y);
   mapchar( map[last_p2.x][last_p2.y], last_p2.x, last_p2.y);
   gotoxy( MAXROW+1, 0);
   
}

void tmpsave(void) 
{
   short i, j;

   for( i = 0; i < rows; i++) for( j = 0; j < cols; j++)
      tmp_map[i][j] = map[i][j];
   tmp_pushes = pushes;
   tmp_moves = moves;
   tmp_savepack = savepack;
   tmp_ppos.x = ppos.x; tmp_ppos.y = ppos.y;
}

void tmpreset(void)
{
   short i, j;

   for( i = 0; i < rows; i++) for( j = 0; j < cols; j++)
      map[i][j] = tmp_map[i][j];
   pushes = tmp_pushes;
   moves = tmp_moves;
   savepack = tmp_savepack;
   ppos.x = tmp_ppos.x; ppos.y = tmp_ppos.y;
}

void showscreen(void) 
{
   short i, j;

   clear();
   for( i = 0; i < rows; i++)
      for( j = 0; map[i][j] != '\0'; j++)
         mapchar( map[i][j], i, j);
   gotoxy( MAXROW, 0);
   printf( "Level:      Packets:      Saved:      Moves:       Pushes:");
   displevel();
   disppackets();
   dispsave();
   dispmoves();
   disppushes();
   gotoxy(MAXROW+1,0);
   printf("Use the following keys: arrow keys or ...\r\n");
   printf(" 8/k/up, 2/j/down, 4/h/left, 6/l/right to move/push\r\n");
   printf(" q:quit, u:undo last move, U:undo all, s:save, r:restore");
   gotoxy( MAXROW+1,0);
}

void mapchar(char c, short i, short j) 
{
   OBJECT *obj, *get_obj_adr();
   short offset_row = 0;	/*(MAXROW - rows) / 2;*/
   short offset_col = MAXCOL - cols;

   obj = get_obj_adr( c);

   gotoxy( i + offset_row, 2*j + offset_col); 
   printf( "%c%c", obj ->obj_display1, obj ->obj_display2);
}

OBJECT *get_obj_adr(char c)
{
   OBJECT *ret;

   if(      c == player.obj_intern)		ret = &player;
   else if( c == playerstore.obj_intern)	ret = &playerstore;
   else if( c == store.obj_intern)		ret = &store;
   else if( c == save.obj_intern)		ret = &save;
   else if( c == packet.obj_intern)		ret = &packet;
   else if( c == wall.obj_intern)		ret = &wall;
   else if( c == ground.obj_intern)		ret = &ground;
   else                                         ret = &ground;

   return( ret);
}

void displevel(void) 
{ 
   gotoxy( MAXROW, 7); printf( "%3d", level); 
}
   
void disppackets(void) 
{ 
   gotoxy( MAXROW, 21); printf( "%3d", packets); 
}
   
void dispsave(void) 
{ 
   gotoxy( MAXROW, 33); printf( "%3d", savepack); 
}
   
void dispmoves(void) 
{ 
   gotoxy( MAXROW, 43); printf( "%4d", moves); 
}
      
void disppushes(void) 
{ 
   gotoxy( MAXROW, 57); printf( "%4d", pushes); 
}

void gameloop(void) {

   short ret = 0;

   ret = readscreen();

   while(!ret) 
   {
      ret = play();

      if(!ret) 
      {
         level++;
         moves = pushes = packets = savepack = 0;
         ret = readscreen();
      }
   }

   clear();  
}

void main( int argc, char** argv) 
{
   short optlevel=1;

   moves = pushes = packets = savepack = 0;

   if (argc == 2)
   {
      if (!(optlevel = atoi(argv[1])))
      {
         printf(message[E_USAGE]);
	 exit(1);
      }
   }
   else
   {
      printf("The Sokoban game was created in 1981 by Hiroyuki Imabayashi\r\n\n");
      printf("Use: >sokoban [level] (50 levels available...)\r\n\n");
      printf("You: @@ must push all packs: [] to storage area: oo ,one by one\r\n");
      printf("Hit any key...");
      getch();
   }

   level=optlevel;

   gameloop();
}

