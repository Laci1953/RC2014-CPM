/*
 * startrek.c
 *
 * Super Star Trek Classic (v1.1)
 * Retro Star Trek Game 
 * C Port Copyright (C) 1996  <Chris Nystrom>
 *
 * Reworked for Fuzix by Alan Cox (C) 2018
 *	- Removed all floating point
 *	- Fixed multiple bugs in the BASIC to C conversion
 *	- Fixed a couple of bugs in the BASIC that either got in during it's
 *	  conversion between BASICs or from the original trek
 *	- Put it on a diet to get it to run in 32K. No features were harmed
 *	  in the making of this code smaller.
 *
 * Adapted for CP/M by Ladislau Szilagyi 2023
 *
 * TODO:
 *	- Look hard at all the rounding cases
 *	- Review some of the funnies in the BASIC code that appear to be bugs
 *	  either in the conversion or between the original and 'super' trek
 *	  Notably need to fix the use of shield energy directly for warp
 *	- Find a crazy person to draw ascii art bits we can showfile for things
 *	  like messages from crew/docking/klingons etc
 *	- I think it would make a lot of sense to switch to real angles, but
 *	  trek game traditionalists might consider that heresy.
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * in any way that you wish. _Star Trek_ is a trademark of Paramount
 * I think.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This is a C port of an old BASIC program: the classic Super Star Trek
 * game. It comes from the book _BASIC Computer Games_ edited by David Ahl
 * of Creative Computing fame. It was published in 1978 by Workman Publishing,
 * 1 West 39 Street, New York, New York, and the ISBN is: 0-89489-052-3.
 * 
 * See http://www.cactus.org/~nystrom/startrek.html for more info.
 *
 * Contact Author of C port at:
 *
 * Chris Nystrom
 * 1013 Prairie Dove Circle
 * Austin, Texas  78758
 *
 * E-Mail: cnystrom@gmail.com, nystrom@cactus.org
 *
 * BASIC -> Conversion Issues
 *
 *     - String Names changed from A$ to sA
 *     - Arrays changed from G(8,8) to map[9][9] so indexes can
 *       stay the same.
 *
 * Here is the original BASIC header:
 *
 * SUPER STARTREK - MAY 16, 1978 - REQUIRES 24K MEMORY
 *
 ***        **** STAR TREK ****        ****
 *** SIMULATION OF A MISSION OF THE STARSHIP ENTERPRISE,
 *** AS SEEN ON THE STAR TREK TV SHOW.
 *** ORIGINAL PROGRAM BY MIKE MAYFIELD, MODIFIED VERSION
 *** PUBLISHED IN DEC'S "101 BASIC GAMES", BY DAVE AHL.
 *** MODIFICATIONS TO THE LATTER (PLUS DEBUGGING) BY BOB
 *** LEEDOM - APRIL & DECEMBER 1974,
 *** WITH A LITTLE HELP FROM HIS FRIENDS . . .
 *** COMMENTS, EPITHETS, AND SUGGESTIONS SOLICITED --
 *** SEND TO:  R. C. LEEDOM
 ***           WESTINGHOUSE DEFENSE & ELECTRONICS SYSTEMS CNTR.
 ***           BOX 746, M.S. 338
 ***           BALTIMORE, MD  21203
 ***
 *** CONVERTED TO MICROSOFT 8 K BASIC 3/16/78 BY JOHN BORDERS
 *** LINE NUMBERS FROM VERSION STREK7 OF 1/12/75 PRESERVED AS
 *** MUCH AS POSSIBLE WHILE USING MULTIPLE STATMENTS PER LINE
 *
 */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define Z80ALL

/* Standard Terminal Sizes */

#ifdef Z80ALL
#define MAXROW      48
#define MAXCOL      64
#else
#define MAXROW      24
#define MAXCOL      80
#endif

struct klingon {
	char y;
	char x;
	int energy;
};

/* Function Declarations */

void intro(void);
void new_game(void);
void initialize(void);
void new_quadrant(void);
void course_control(void);
void complete_man(int, int);
void man_energy(int);
void s_range_scan(void);
void l_range_scan(void);
void phaser_control(void);
void photon_tor(void);
void torpedo_hit(char y, char x);
void damage_control(void);
void shield_control(void);
void lib_computer(void);
void galactic_rec(void);
void status_report(void);
void torpedo_data(void);
void nav_data(void);
void dirdist_calc(void);
void galaxy_map(void);
void end_of_time(void);
void resign(void);
void won_game(void);
void end_of_game(void);
void klingons_move(void);
void klingons_shoot(void);
void repair_damage(int warp);
void fs_empty_pl(char t, char *z1, char *z2);
char *get_dev_name(int n);
void quadrant_name(char small, char y, char x);
int distance_to(struct klingon *k);
int square00(int t);
int cint100(int d);
void compute_vector(int, int, int, int);
void showfile(char *filename);
unsigned int xrnd(void);
void xrndseed(void);

/* Global Variables */

#define MAP_VISITED 0x1000	/* Set if this sector was mapped */

char c[3][10] =	/* Movement indices 1-9 (9 is wrap of 1) */
{
	{0},
	{0, 0, -1, -1, -1, 0, 1, 1, 1, 0},
	{1, 1, 1, 0, -1, -1, -1, 0, 1, 1}
};

#define Q_SPACE		0
#define Q_STAR		1
#define Q_BASE		2
#define Q_KLINGON	3
#define Q_SHIP		4

struct klingon kdata[3];	/* Klingon Data */
char starbases;			/* Starbases in Quadrant */
char base_y, base_x;		/* Starbase Location in sector */
char starbases_left;		/* Total Starbases */
char docked;			/* Docked flag */
int energy;			/* Current Energy */
int energy0;			/* Starting Energy */
unsigned int map[9][9];		/* Galaxy. BCD of k b s plus flag */
char klingons;			/* Klingons in Quadrant */
char total_klingons;		/* Klingons at start */
char klingons_left;		/* Total Klingons left */
char torps;			/* Photon Torpedoes left */
char torps0;			/* Photon Torpedo capacity */
unsigned int quad_y, quad_x;	/* Quadrant Position of Enterprise */
int shield;			/* Current shield value */
char stars;			/* Stars in quadrant */
unsigned int time_start;	/* Starting Stardate */
unsigned int time_up;		/* End of time */
int damage[9];			/* Damage Array */
unsigned int d4;		/* Used for computing damage repair time */
unsigned int ship_y, ship_x;	/* Current Sector Position of Enterprise, fixed point */
unsigned int stardate;		/* Current Stardate */
char quad[8][8];
char quadname[12];		/* Quadrant name */
char dummy[83];			// total bytes= 3 x 128

/* We probably need double digit for co-ordinate maths, single for time */
#define TO_FIXED(x)	((x) * 10)
#define FROM_FIXED(x)	((x) / 10)

#define TO_FIXED00(x)	((x) * 100)
#define FROM_FIXED00(x)	((x) / 100)

/*
 *	Returns an integer from 1 to spread
 */
unsigned int get_rand(unsigned int spread)
{
	unsigned int r = xrnd();

	return ((r % spread) + 1);
}

/*
 *	Get a random co-ordinate
 */
char rand8(void)
{
	return (get_rand(8));
}

/* This is basically a fancier fgets that always eats the line even if it
   only copies part of it */
void input(char *b, char l)
{
	int c;

	fflush(stdout);
	while((c = getchar()) != '\n') {
		if (c == EOF)
			exit(1);
		if (l > 1) {
			*b++ = c;
			l--;
		}
	}
	*b = 0;
}

char yesno(void)
{
	char c = getch();
	
	if (isupper(c))
		c = tolower(c);

	if (c == (char)'y')
		return 1;

	return 0;
}

/* Input a value between 0.00 and 9.99 */
int input_f00(void)
{
	int v;
	char buf[8];
	char *x;
	input(buf, 8);
	x = buf;
	if (!isdigit(*x))
		return -1;
	v = 100 * (*x++ - '0');
	if (*x == 0)
		return v;
	if (*x++ != '.')
		return -1;
	if (!isdigit(*x))
		return -1;
	v += 10 * (*x++ - '0');
	if (!*x)
		return v;
	if (!isdigit(*x))
		return -1;
	v += *x++ - '0';
	return v;
}

/* Integer: unsigned, or returns -1 for blank/error */
int input_int(void)
{
	char x[8];
	input(x, 8);
	if (!isdigit(*x))
		return -1;
	return atoi(x);
}

char *print100(int v)
{
static	char buf[16];
	char *p = buf;
	if (v < 0) {
		v = -v;
		*p++ = '-';
	}
	p += sprintf(p, "%d.%02d", v / 100, v%100);
	return buf;
}

/* Main Program */

int main(int argc, char *argv[])
{

	energy0 = 3000;
	torps0 = 10;

	xrndseed();

#ifdef Z80ALL
	Save_1_to_16();
	Load_1_to_16();
#endif
	
	intro();

	new_game();

#ifdef Z80ALL
	Restore_1_to_16();
#endif

	return (0);
}

char inoperable(char u)
{
	if (damage[u] < 0) {
		printf("%s %s inoperable.\n",
			get_dev_name(u),
			u == 5 ? "are":"is");
		return 1;
	}
	return 0;
}

void intro(void)
{
	showfile("st.int");

	if (yesno())
		showfile("st.txt");

	showfile("st.log");

	/* Max of 4000, which works nicely with our 0.1 fixed point giving
	   us a 16bit unsigned range of time */
	stardate = TO_FIXED((get_rand(20) + 20) * 100);
}

void new_game(void)
{
	char cmd[4];

	fprintf(stderr, "Load saved game?(y/Y=YES)\r\n");

	if (yesno())
		LoadGame();
	else
	{
		initialize();
		new_quadrant();
	}

	s_range_scan();

	while (1) {
		if (shield + energy <= 10 && (energy < 10 || damage[7] < 0)) {
			showfile("st.fat");
			end_of_time();
		}

		fprintf(stderr, "Command? ");

		input(cmd, 4);
		putchar('\n');

		if (!strncmp(cmd, "sav", 3))
			SaveGame();
		else if (!strncmp(cmd, "nav", 3))
			course_control();
		else if (!strncmp(cmd, "srs", 3))
			s_range_scan();
		else if (!strncmp(cmd, "lrs", 3))
			l_range_scan();
		else if (!strncmp(cmd, "pha", 3))
			phaser_control();
		else if (!strncmp(cmd, "tor", 3))
			photon_tor();
		else if (!strncmp(cmd, "shi", 3))
			shield_control();
		else if (!strncmp(cmd, "dam", 3))
			damage_control();
		else if (!strncmp(cmd, "com", 3))
			lib_computer();
		else if (!strncmp(cmd, "xxx", 3))
			resign();
		else {
			/* FIXME: showfile ?*/
			puts("Enter one of the following:\n");
			puts("  sav - Save game to disk file & quit");
			puts("  nav - To Set Course");
			puts("  srs - Short Range Sensors");
			puts("  lrs - Long Range Sensors");
			puts("  pha - Phasers");
			puts("  tor - Photon Torpedoes");
			puts("  shi - Shield Control");
			puts("  dam - Damage Control");
			puts("  com - Library Computer");
			puts("  xxx - Resign Command\n");
		}
	}
}

void initialize(void)
{
	int i, j;
	static char plural_2[2];
	static char plural[4];
	char yp, xp;

	strcpy(plural_2, "");
	strcpy(plural, "is");

	/* Initialize time */

	time_start = FROM_FIXED(stardate);
	time_up = 25 + get_rand(10);

	/* Initialize Enterprise */

	docked = 0;
	energy = energy0;
	torps = torps0;
	shield = 0;

	quad_y = rand8();
	quad_x = rand8();
	ship_y = TO_FIXED00(rand8());
	ship_x = TO_FIXED00(rand8());

	for (i = 1; i <= 8; i++)
		damage[i] = 0;

	/* Setup What Exists in Galaxy */

	for (i = 1; i <= 8; i++) {
		for (j = 1; j <= 8; j++) {
			char r = get_rand(100);
			klingons = 0;
			if (r > 98)
				klingons = 3;
			else if (r > 95)
				klingons = 2;
			else if (r > 80)
				klingons = 1;

			klingons_left = klingons_left + klingons;
			starbases = 0;

			if (get_rand(100) > 96)
				starbases = 1;

			starbases_left = starbases_left + starbases;

			map[i][j] = (klingons << 8) + (starbases << 4) + rand8();
		}
	}

	/* Give more time for more Klingons */
	if (klingons_left > time_up)
		time_up = klingons_left + 1;

	/* Add a base if we don't have one */
	if (starbases_left == 0) {
		yp = rand8();
		xp = rand8();
		if (map[yp][xp] < 0x200) {
			map[yp][xp] += (1 << 8);
			klingons_left++;
		}

		map[yp][xp] += (1 << 4);
		starbases_left++;
	}

	total_klingons = klingons_left;

	if (starbases_left != 1) {
		strcpy(plural_2, "s");
		strcpy(plural, "are");
	}

	printf("Your orders are as follows:\n"
	       " Destroy the %d Klingon warships which have invaded\n"
	       " the galaxy before they can attack Federation Headquarters\n"
	       " on stardate %u. This gives you %d days. There %s\n"
	       " %d starbase%s in the galaxy for resupplying your ship.\n\n"
	       "Hit any key to accept command. ",
	       klingons_left, time_start + time_up, time_up, plural, starbases_left, plural_2);
	fflush(stdout);
	getchar();
}

void place_ship(void)
{
	quad[FROM_FIXED00(ship_y) - 1][FROM_FIXED00(ship_x) - 1] = Q_SHIP;
}

void new_quadrant(void)
{
	int i;
	int tmp;
	struct klingon *k = kdata;

	klingons = 0;
	starbases = 0;
	stars = 0;

	/* Random factor for damage repair. We compute it on each new
	   quadrant to stop the user just retrying until they get a number
	   they like. The conversion here was wrong and now copies the BASIC
	   code generate 0.00 to 0.49 */
	d4 = get_rand(50) - 1;

	/* Copy to computer */
	map[quad_y][quad_x] |= MAP_VISITED;

	if (quad_y >= 1 && quad_y <= 8 && quad_x >= 1 && quad_x <= 8) {
		quadrant_name(0, quad_y, quad_x);

		if (TO_FIXED(time_start) != stardate)
			printf("Now entering %s quadrant...\n\n", quadname);
		else {
			puts("\nYour mission begins with your starship located\r\n");
			printf("in the galactic quadrant %s.\n\n", quadname);
		}
	}

	tmp = map[quad_y][quad_x];
	klingons = (tmp >> 8) & 0x0F;
	starbases = (tmp >> 4) & 0x0F;
	stars = tmp & 0x0F;

	if (klingons > 0) {
		printf("Combat Area  Condition Red\n");

		if (shield < 200)
			printf("Shields Dangerously Low\n");
	}

	for (i = 1; i <= 3; i++) {
		k->y = 0;
		k->x = 0;
		k->energy = 0;
		k++;
	}

	memset(quad, 64, Q_SPACE); //memset(quad, Q_SPACE, 64); HiTech bug!

	place_ship();
	
	if (klingons > 0) {
		k = kdata;
		for (i = 0; i < klingons; i++) {
			fs_empty_pl(Q_KLINGON, &k->y, &k->x);
			k->energy = 100 + get_rand(200);
			k++;
		}
	}

	if (starbases > 0)
		fs_empty_pl(Q_BASE, &base_y, &base_x);

	for (i = 1; i <= stars; i++)
		fs_empty_pl(Q_STAR, NULL, NULL);
}

char *inc_1 = "reports:\n  Incorrect course data, sir!\n";

void course_control(void)
{
	register int i;
	int c1;
	int warp;
	int n;
	int c2, c3, c4;
	int z1, z2;
	int x1, x2;
	int x, y;
	static char warpmax[4];

	strcpy(warpmax, "8");

	fprintf(stderr, "Course (0-9): ");

	c1 = input_f00();

	if (c1 == 900)
		c1 = 100;

	if (c1 < 0 || c1 > 900) {
		printf("Lt. Sulu %s", inc_1);
		return;
	}

	if (damage[1] < 0)
		strcpy(warpmax, "0.2");

	printf("Warp Factor (0-%s): ", warpmax);

	warp = input_f00();

	if (damage[1] < 0 && warp > 20) {
		printf("Warp Engines are damaged. "
		       "Maximum speed = Warp 0.2.\n\n");
		return;
	}

	if (warp <= 0)
		return;

	if (warp > 800) {
		printf("Chief Engineer Scott reports:\n"
		       "  The engines won't take warp %s!\n\n", print100(warp));
		return;
	}

	n = warp * 8;

	n = cint100(n);	

	/* FIXME: should be  s + e - n > 0 iff shield control undamaged */
	if (energy - n < 0) {
		printf("Engineering reports:\n"
		       "  Insufficient energy available for maneuvering"
		       " at warp %s!\n\n", print100(warp));

		if (shield >= n && damage[7] >= 0) {
			printf("Deflector Control Room acknowledges:\n"
			       "  %d units of energy presently deployed to shields.\n", shield);
		}

		return;
	}

	klingons_move();

	repair_damage(warp);

	z1 = FROM_FIXED00(ship_y);
	z2 = FROM_FIXED00(ship_x);
	quad[z1-1][z2-1] = Q_SPACE;


	c2 = FROM_FIXED00(c1);	/* Integer part */
	c3 = c2 + 1;		/* Next integer part */
	c4 = (c1 - TO_FIXED00(c2));	/* Fractional element in fixed point */

	x1 = 100 * c[1][c2] + (c[1][c3] - c[1][c2]) * c4;
	x2 = 100 * c[2][c2] + (c[2][c3] - c[2][c2]) * c4;

	x = ship_y;
	y = ship_x;

	for (i = 1; i <= n; i++) {
		ship_y = ship_y + x1;
		ship_x = ship_x + x2;

		z1 = FROM_FIXED00(ship_y);
		z2 = FROM_FIXED00(ship_x);	/* ?? cint100 ?? */

		/* Changed quadrant */
		if (z1 < 1 || z1 >= 9 || z2 < 1 || z2 >= 9) {
			char outside = 0;		/* Outside galaxy flag */
			char quad_y_old = quad_y;
			char quad_x_old = quad_x;

			x = (800 * quad_y) + x + (n * x1);
			y = (800 * quad_x) + y + (n * x2);

			//	printf("X %d Y %d\n", x, y);

			quad_y = x / 800;	/* Fixed point to int and divide by 8 */
			quad_x = y / 800;	/* Ditto */

			//	printf("Q %d %d\n", quad_y, quad_x);

			ship_y = x - (quad_y * 800);
			ship_x = y - (quad_x * 800);

			//	printf("S %d %d\n", ship_y, ship_x);

			if (ship_y < 100) {
				quad_y = quad_y - 1;
				ship_y = ship_y + 800;
			}

			if (ship_x < 100) {
				quad_x = quad_x - 1;
				ship_x = ship_x + 800;
			}

			/* check if outside galaxy */

			if (quad_y < 1) {
				outside = 1;
				quad_y = 1;
				ship_y = 100;
			}

			if (quad_y > 8) {
				outside = 1;
				quad_y = 8;
				ship_y = 800;
			}

			if (quad_x < 1) {
				outside = 1;
				quad_x = 1;
				ship_x = 100;
			}

			if (quad_x > 8) {
				outside = 1;
				quad_x = 8;
				ship_x = 800;
			}

			if (outside == 1) {
				/* Mostly showfile ? FIXME */
				printf("LT. Uhura reports:\n"
		       "  Message from Starfleet Command:\n\n"
		       "  Permission to attempt crossing of galactic perimeter\n"
		       "  is hereby *denied*. Shut down your engines.\n\n"
		       "Chief Engineer Scott reports:\n"
		       "  Warp Engines shut down at sector %d, "
		       "%d of quadrant %d, %d.\n\n",
				       FROM_FIXED00(ship_y),
				       FROM_FIXED00(ship_x), quad_y, quad_x);
			}
			man_energy(n);

			/* this section has a different order in the original.
			   t = t + 1;

			   if (t > time_start + time_up)
			   end_of_time();
			 */

			if (FROM_FIXED(stardate) > time_start + time_up)
				end_of_time();

			if (quad_y != quad_y_old || quad_x != quad_x_old) {
				stardate = stardate + TO_FIXED(1);
				new_quadrant();
			}
			complete_man(warp, n);
			return;
		}

		if (quad[z1-1][z2-1] != Q_SPACE) {	/* Sector not empty */
			ship_y = ship_y - x1;
			ship_x = ship_x - x2;
			printf("Warp Engines shut down at sector "
			       "%d, %d due to bad navigation.\n\n", z1, z2);
			i = n + 1;
		}
	}
	complete_man(warp, n);
}

void complete_man(int warp, int n)
{
	int time_used;

	place_ship();
	man_energy(n);

	time_used = TO_FIXED(1);

	/* Ick FIXME - re really want to tidy up time to FIXED00 */
	if (warp < 100)
		time_used = TO_FIXED(FROM_FIXED00(warp));

	stardate += time_used;

	if (FROM_FIXED(stardate) > time_start + time_up)
		end_of_time();

	s_range_scan();
}


void man_energy(int n)
{
	energy -= n + 10;

	if (energy >= 0)
		return;

	/* FIXME:
	   This never occurs with the nav code as is - ancient trek versions
	   included shield power in movement allowance if shield control
	   was undamaged */
	puts("Shield Control supplies energy to complete maneuver.\n");

	shield = shield + energy;
	energy = 0;

	if (shield <= 0)
		shield = 0;
}

char *srs_1 = "  -1--2--3--4--5--6--7--8-";

char *tilestr[] = {
	"   ",
	" * ",
#ifdef Z80ALL
	"\x0E\x0F\x10", // Federation starbase ">!<",
	"\x4\x5\x6", // Klingon battlecruiser "+K+",
	"\x1\x2\x3"  // Your starship's position "<*>"
#else
	">!<", // Federation starbase
	"+K+", // Klingon battlecruiser
	"<*>"  // Your starship's position
#endif
};

void s_range_scan(void)
{
	register int i, j;
	char *sC = "GREEN";

	if (energy < energy0 / 10)
		sC = "YELLOW";

	if (klingons > 0)
		sC = "*RED*";

	docked = 0;

	for (i = (int) (FROM_FIXED00(ship_y) - 1); i <= (int) (FROM_FIXED00(ship_y) + 1); i++)
		for (j = (int) (FROM_FIXED00(ship_x) - 1); j <= (int) (FROM_FIXED00(ship_x) + 1); j++)
			if (i >= 1 && i <= 8 && j >= 1 && j <= 8) {
				if (quad[i-1][j-1] == Q_BASE) {
					docked = 1;
					sC = "DOCKED";
					energy = energy0;
					torps = torps0;
					puts("Shields dropped for docking purposes.");
					shield = 0;
				}
			}

	if (damage[2] < 0) {
		puts("\n*** Short Range Sensors are out ***");
		return;
	}

	puts(srs_1);
	for (i = 0; i < 8; i++) {
#ifdef	Z80ALL
		putchar('\n');	//to get a more accurate XY perspective
#endif
		fprintf(stderr,"%d ", i+1);

		for (j = 0; j < 8; j++)
			if (quad[i][j] >= 0 && quad[i][j] <= 4)
				fprintf(stderr, tilestr[quad[i][j]]);
			else
				fprintf(stderr, "   ");

		fprintf(stderr," %d", i+1);

		if (i == 0)
			printf("    Stardate            %d\n", FROM_FIXED(stardate));
		if (i == 1)
			printf("    Condition           %s\n", sC);
		if (i == 2)
			printf("    Quadrant            %d, %d\n", quad_y, quad_x);
		if (i == 3)
			printf("    Sector              %d, %d\n", FROM_FIXED00(ship_y), FROM_FIXED00(ship_x));
		if (i == 4)
			printf("    Photon Torpedoes    %d\n", torps);
		if (i == 5)
			printf("    Total Energy        %d\n", energy + shield);
		if (i == 6)
			printf("    Shields             %d\n", shield);
		if (i == 7)
			printf("    Klingons Remaining  %d\n", klingons_left);
#ifdef	Z80ALL
		putchar('\n');	//to get a more accurate XY perspective
#endif
	}
	puts(srs_1);
	putchar('\n');

	return;
}

char *lrs_1 = "-------------------\n";

void put1bcd(char v)
{
	v &= 0x0F;
	putchar('0' + v);
}

void putbcd(int x)
{
	put1bcd(x >> 8);
	put1bcd(x >> 4);
	put1bcd(x);
}

void l_range_scan(void)
{
	register int i, j;

	if (inoperable(3))
		return;

	printf("Long Range Scan for Quadrant %d, %d\n\n", quad_y, quad_x);

	for (i = quad_y - 1; i <= quad_y + 1; i++) {
		printf("%s:", lrs_1);
		for (j = quad_x - 1; j <= quad_x + 1; j++) {
			putchar(' ');
			if (i > 0 && i <= 8 && j > 0 && j <= 8) {
				map[i][j] |= MAP_VISITED;
				putbcd(map[i][j]);
			} else
				fprintf(stderr, "***");
			fprintf(stderr, " :");
		}
		putchar('\n');
	}

	printf("%s\n", lrs_1);
}

char no_klingon(void)
{
	if (klingons <= 0) {
		puts("Science Officer Spock reports:\n"
		     "  'Sensors show no enemy ships in this quadrant'\n");
		return 1;
	}
	return 0;
}

void wipe_klingon(struct klingon *k)
{
	quad[k->y-1][k->x-1] = Q_SPACE;
}
