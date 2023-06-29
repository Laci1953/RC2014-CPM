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
#define MAXROW      28
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
int xrnd(void);
void xrndseed(void);
char inoperable(char);
char no_klingon(void);
int input_int(void);
unsigned int get_rand(int);
int input_f00(void);
char yesno(void);
char rand8(void);

/* Global Variables */

extern char starbases;			/* Starbases in Quadrant */
extern char base_y, base_x;		/* Starbase Location in sector */
extern char starbases_left;		/* Total Starbases */

extern char c[3][10];

extern char docked;			/* Docked flag */
extern int energy;			/* Current Energy */
extern int energy0;			/* Starting Energy */
extern unsigned int map[9][9];		/* Galaxy. BCD of k b s plus flag */
#define MAP_VISITED 0x1000		/* Set if this sector was mapped */
extern struct klingon kdata[3];		/* Klingon Data */
extern char klingons;			/* Klingons in Quadrant */
extern char total_klingons;		/* Klingons at start */
extern char klingons_left;		/* Total Klingons left */
extern char torps;			/* Photon Torpedoes left */
extern char torps0;			/* Photon Torpedo capacity */
extern unsigned int quad_y, quad_x;	/* Quadrant Position of Enterprise */
extern int shield;			/* Current shield value */
extern char stars;			/* Stars in quadrant */
extern unsigned int time_start;		/* Starting Stardate */
extern unsigned int time_up;		/* End of time */
extern int damage[9];			/* Damage Array */
extern unsigned int d4;			/* Used for computing damage repair time */
extern unsigned int ship_y, ship_x;	/* Current Sector Position of Enterprise, fixed point */
extern unsigned int stardate;		/* Current Stardate */

extern char quad[8][8];
#define Q_SPACE		0
#define Q_STAR		1
#define Q_BASE		2
#define Q_KLINGON	3
#define Q_SHIP		4

extern char quadname[12];		/* Quadrant name */

/* We probably need double digit for co-ordinate maths, single for time */
#define TO_FIXED(x)	((x) * 10)
#define FROM_FIXED(x)	((x) / 10)

#define TO_FIXED00(x)	((x) * 100)
#define FROM_FIXED00(x)	((x) / 100)

char *inc_1_ = "reports:\n  Incorrect course data, sir!\n";

void phaser_control(void)
{
	register int i;
	long phaser_energy;
	long h1;
	int h;
	struct klingon *k = kdata;

	if (inoperable(4))
		return;

	if (no_klingon())
		return;

	/* There's Klingons on the starboard bow... */
	if (damage[8] < 0)
		puts("Computer failure hampers accuracy.");

	printf("Phasers locked on target;\n"
	       "Energy available = %d units\n\n"
	       "Number of units to fire: ", energy);

	phaser_energy = input_int();

	if (phaser_energy <= 0)
		return;

	if (energy - phaser_energy < 0) {
		puts("Not enough energy available.\n");
		return;
	}

	energy -=  phaser_energy;

	/* We can fire up to nearly 3000 points of energy so we do this
	   bit in 32bit math */

	if (damage[8] < 0)
		phaser_energy *= get_rand(100);
	else
		phaser_energy *= 100;

	h1 = phaser_energy / klingons;

	for (i = 0; i <= 2; i++) {
		if (k->energy > 0) {
			/* We are now 32bit with four digits accuracy */
			h = h1 * (get_rand(100) + 200);
			/* Takes us down to 2 digit accuracy */

			h /= distance_to(k);

			if (h <= 15 * k->energy) {	/* was 0.15 */
				printf("Sensors show no damage to enemy at "
				       "%d, %d\n\n", k->y, k->x);
			} else {
				h = FROM_FIXED00(h);
				k->energy -= h;
				printf("%d unit hit on Klingon at sector "
				       "%d, %d\n",
					h, k->y, k->x);
				if (k->energy <= 0) {
					puts("*** Klingon Destroyed ***\n");
					klingons--;
					klingons_left--;
					wipe_klingon(k);
					k->energy = 0;
					/* Minus a Klingon.. */
					map[quad_y][quad_x] -= 0x100;
					if (klingons_left <= 0)
						won_game();
				} else
					printf("   (Sensors show %d units remaining.)\n\n", k->energy);
			}
		}
		k++;
	}

	klingons_shoot();
}

void photon_tor(void)
{
	int x3, y3;
	int c1;
	int c2, c3, c4;
	int x, y, x1, x2;

	if (torps <= 0) {
		puts("All photon torpedoes expended");
		return;
	}

	if (inoperable(5))
		return;

	fprintf(stderr, "Course (0-9): ");

	c1 = input_f00();

	if (c1 == 900)
		c1 = 100;

	if (c1 < 100 || c1 >= 900) {
		printf("Ensign Chekov%s", inc_1_);
		return;
	}

	/* FIXME: energy underflow check ? */
	energy = energy - 2;
	torps--;

	c2 = FROM_FIXED00(c1);		/* Integer part */
	c3 = c2 + 1;			/* Next integer part */
	c4 = (c1 - TO_FIXED00(c2));	/* Fractional element in fixed point */

	x1 = 100 * c[1][c2] + (c[1][c3] - c[1][c2]) * c4;
	x2 = 100 * c[2][c2] + (c[2][c3] - c[2][c2]) * c4;

	/* The basic code is very confused about what is X and what is Y */
	x = ship_y + x1;
	y = ship_x + x2;

	x3 = FROM_FIXED00(x);
	y3 = FROM_FIXED00(y);

	puts("Torpedo Track:");

	while (x3 >= 1 && x3 <= 8 && y3 >= 1 && y3 <= 8) {
		char p;

		printf("    %d, %d\n", x3, y3);

		p = quad[x3-1][y3-1];
		/* In certain corner cases the first trace we'll step is
		   ourself. If so treat it as space */
		if (p != Q_SPACE && p != Q_SHIP) {
			torpedo_hit(x3, y3);
			klingons_shoot();
			return;
		}

		x = x + x1;
		y = y + x2;

		x3 = FROM_FIXED00(x);
		y3 = FROM_FIXED00(y);
	}

	puts("Torpedo Missed\n");

	klingons_shoot();
}

void torpedo_hit(char yp, char xp)
{
	int i;
	struct klingon *k;

	switch(quad[yp-1][xp-1]) {
	case Q_STAR:
		printf("Star at %d, %d absorbed torpedo energy.\n\n", yp, xp);
		return;
	case Q_KLINGON:
		puts("*** Klingon Destroyed ***\n");
		klingons--;
		klingons_left--;

		if (klingons_left <= 0)
			won_game();

		k = kdata;
		for (i = 0; i <= 2; i++) {
			if (yp == k->y && xp == k->x)
				k->energy = 0;
			k++;
		}
		map[quad_y][quad_x] -= 0x100;
		break;
	case Q_BASE:					
		puts("*** Starbase Destroyed ***");
		starbases--;
		starbases_left--;

		if (starbases_left <= 0 && klingons_left <= FROM_FIXED(stardate) - time_start - time_up) {
			/* showfile ? FIXME */
			puts("That does it, Captain!!"
			     "You are hereby relieved of command\n"
			     "and sentenced to 99 stardates of hard"
			     "labor on Cygnus 12!!\n");
			resign();
		}

		puts("Starfleet Command reviewing your record to consider\n"
		     "court martial!\n");

		docked = 0;		/* Undock */
		map[quad_y][quad_x] -= 0x10;
		break;
	}
	quad[yp-1][xp-1] = Q_SPACE;
}

void damage_control(void)
{
	int repair_cost = 0;
	register int i;

	if (damage[6] < 0)
		puts("Damage Control report not available.");

	/* Offer repair if docked */
	if (docked) {
		/* repair_cost is x.xx fixed point */
		repair_cost = 0;
		for (i = 1; i <= 8; i++)
			if (damage[i] < 0)
				repair_cost = repair_cost + 10;

		if (repair_cost) {
			repair_cost = repair_cost + d4;
			if (repair_cost >= 100)
				repair_cost = 90;	/* 0.9 */

			printf("\nTechnicians standing by to effect repairs to your"
			       "ship;\nEstimated time to repair: %s stardates.\n"
			       "Will you authorize the repair order (y/Y=YES)?\n ", print100(repair_cost));

			if (yesno()) {
				for (i = 1; i <= 8; i++)
					if (damage[i] < 0)
						damage[i] = 0;

				/* Work from two digit to one digit. We might actually
				   have to give in and make t a two digt offset from
				   a saved constant base only used in printing to
				   avoid that round below FIXME */
				stardate += (repair_cost + 5)/10 + 1;
			}
			return;
		}
	}

	if (damage[6] < 0)
		return;

	puts("Device            State of Repair");

	for (i = 1; i <= 8; i++)
		printf("%-25s%6s\n", get_dev_name(i), print100(damage[i]));

	printf("\n");
}

void shield_control(void)
{
	int i;

	if (inoperable(7))
		return;

	printf("Energy available = %d\n\n"
	       "Input number of units to shields: ", energy + shield);

	i = input_int();

	if (i < 0 || shield == i) {
unchanged:
		puts("<Shields Unchanged>\n");
		return;
	}

	if (i >= energy + shield) {
		puts("Shield Control Reports:\n"
		     "  'This is not the Federation Treasury.'");
		goto unchanged;
	}

	energy = energy + shield - i;
	shield = i;

	printf("Deflector Control Room report:\n"
	       "  'Shields now at %d units per your command.'\n\n", shield);
}

void lib_computer(void)
{

	if (inoperable(8))
		return;

	fprintf(stderr, "Computer active and awating command: ");

	switch(input_int()) {
		/* -1 means 'typed nothing or junk */
		case -1:
			break;
		case 0:
			galactic_rec();
			break;
		case 1:
			status_report();
			break;
		case 2:
			torpedo_data();
			break;
		case 3:
			nav_data();
			break;
		case 4:
			dirdist_calc();
			break;
		case 5:
			galaxy_map();
			break;
		default:
			/* FIXME: showfile */
			puts("Functions available from Library-Computer:\n\n"
			     "   0 = Cumulative Galactic Record\n"
			     "   1 = Status Report\n"
			     "   2 = Photon Torpedo Data\n"
			     "   3 = Starbase Nav Data\n"
			     "   4 = Direction/Distance Calculator\n"
			     "   5 = Galaxy 'Region Name' Map\n");
	}
}

char *gr_1 = "   ----- ----- ----- ----- ----- ----- ----- -----\n";

void galactic_rec(void)
{
	int i, j;

	printf("\n     Computer Record of Galaxy for Quadrant %d,%d\n\n", quad_y, quad_x);
	puts("     1     2     3     4     5     6     7     8");

	for (i = 1; i <= 8; i++) {
		printf("%s%d", gr_1, i);

		for (j = 1; j <= 8; j++) {
			printf("   ");

			if (map[i][j] & MAP_VISITED)
				putbcd(map[i][j]);
			else
				printf("***");
		}
		putchar('\n');
	}

	printf("%s\n", gr_1);
}

char *str_s = "s";

void status_report(void)
{
	char *plural = str_s + 1;
	int left = TO_FIXED(time_start + time_up) - stardate;

	puts("   Status Report:\n");

	if (klingons_left > 1)
		plural = str_s;

	/* Assumes fixed point is single digit fixed */
	printf("Klingon%s Left: %d\n"
	       "Mission must be completed in %d.%d stardates\n",
		plural, klingons_left,
		FROM_FIXED(left), left%10);

	if (starbases_left < 1) {
		puts("Your stupidity has left you on your own in the galaxy\n"
		     " -- you have no starbases left!\n");
	} else {
		plural = str_s;
		if (starbases_left < 2)
			plural++;

		printf("The Federation is maintaining %d starbase%s in the galaxy\n\n", starbases_left, plural);
	}
}

void torpedo_data(void)
{
	int i;
	char *plural = str_s + 1;
	struct klingon *k;

	/* Need to also check sensors here ?? */
	if (no_klingon())
		return;

	if (klingons > 1)
		plural--;

	printf("From Enterprise to Klingon battlecruiser%s:\n\n", plural);

	k = kdata;
	for (i = 0; i <= 2; i++) {
		if (k->energy > 0) {
			compute_vector(TO_FIXED00(k->y),
				       TO_FIXED00(k->x),
				       ship_y, ship_x);
		}
		k++;
	}
}

void nav_data(void)
{
	if (starbases <= 0) {
		puts("Mr. Spock reports,\n"
		     "  'Sensors show no starbases in this quadrant.'\n");
		return;
	}
	compute_vector(TO_FIXED00(base_y), TO_FIXED00(base_x), ship_y, ship_x);
}

/* Q: do we want to support fractional co-ords ? */
void dirdist_calc(void)
{
	int targetX, targetY;
	printf("Direction/Distance Calculator\n"
	       "You are at quadrant (%d,%d) sector (%d,%d)\n\n",
	       quad_y, quad_x,
	       FROM_FIXED00(ship_y), FROM_FIXED00(ship_x));

	fprintf(stderr,"Please input target Y coordinate: ");
	targetY = TO_FIXED00(input_int());
	if (targetY < 0 || targetY > 900)
		return;

	fprintf(stderr, "Please input target X coordinate: ");
	targetX = TO_FIXED00(input_int());
	if (targetX < 0 || targetX > 900)
		return;

	compute_vector(targetX, targetY, ship_x, ship_y);
}

char *gm_1 = "  ----- ----- ----- ----- ----- ----- ----- -----\n";

void galaxy_map(void)
{
	int i, j, j0;

	printf("\n                   The Galaxy\n\n");
	printf("    1     2     3     4     5     6     7     8\n");

	for (i = 1; i <= 8; i++) {
		printf("%s%d ", gm_1, i);

		quadrant_name(1, i, 1);

		j0 = (int) (11 - (strlen(quadname) / 2));

		for (j = 0; j < j0; j++)
			putchar(' ');

		fprintf(stderr, quadname);

		for (j = 0; j < j0; j++)
			putchar(' ');

		if (!(strlen(quadname) % 2))
			putchar(' ');

		quadrant_name(1, i, 5);

		j0 = (int) (12 - (strlen(quadname) / 2));

		for (j = 0; j < j0; j++)
			putchar(' ');

		puts(quadname);
	}

	puts(gm_1);

}

char *dist_1 = "  DISTANCE = %s\n\n";

void compute_vector(int w1, int x, int c1, int a)
{
	long xl, al;

	fprintf(stderr,"  DIRECTION = ");
	/* All this is happening in fixed point */
	x = x - a;
	a = c1 - w1;

	xl = abs(x);
	al = abs(a);

	if (x < 0) {
		if (a > 0) {
			c1 = 300;
estimate2:
		/* Multiply the top half by 100 to keep in fixed0 */
			if (al >= xl)
				printf("%s", print100(c1 + ((xl * 100) / al)));
			else
				printf("%s", print100(c1 + ((((xl * 2) - al) * 100)  / xl)));

			printf(dist_1, print100((x > a) ? x : a));
			return;
		} else if (x != 0){
			c1 = 500;
			goto estimate1;
		} else {
			c1 = 700;
			goto estimate2;
		}
	} else if (a < 0) {
		c1 = 700;
		goto estimate2;
	} else if (x > 0) {
		c1 = 100;
		goto estimate1;
	} else if (a == 0) {
		c1 = 500;
		goto estimate1;
	} else {
		c1 = 100;
estimate1:
		/* Multiply the top half by 100 as well so that we keep it in fixed00
		   format. Our larget value is int 9 (900) so we must do this 32bit */
		if (al <= xl)
			printf("%s", print100(c1 + ((al * 100) / xl)));
		else
			printf("%s", print100(c1 + ((((al * 2) - xl) * 100) / al)));
		printf(dist_1, print100((xl > al) ? xl : al));
	}
}
void ship_destroyed(void)
{
	puts("The Enterprise has been destroyed. "
	     "The Federation will be conquered.\n");

	end_of_time();
}

void end_of_time(void)
{
	printf("It is stardate %d.\n\n",  FROM_FIXED(stardate));

	resign();
}

void resign(void)
{
	printf("There were %d Klingon Battlecruisers left at the\r\n"
	       " end of your mission.\n\n", klingons_left);

	end_of_game();
}

void won_game(void)
{
	puts("Congratulations, Captain!  The last Klingon Battle Cruiser\n"
	     "menacing the Federation has been destoyed.\n");

	if (FROM_FIXED(stardate) - time_start > 0)
		printf("Your efficiency rating is %s\n",
			print100(square00(TO_FIXED00(total_klingons)/(FROM_FIXED(stardate) - time_start))));
		// 1000 * pow(total_klingons / (float)(FROM_FIXED(t) - time_start), 2));
	end_of_game();
}

void end_of_game(void)
{
	char x[4];
	if (starbases_left > 0) {
		/* FIXME: showfile ? */
		fprintf(stderr, "The Federation is in need of a new starship commander\r\n"
		     " for a similar mission.\n"
		     "If there is a volunteer, let him step forward and\n"
		     " enter 'aye': ");

		input(x,4);
		if (!strncmp(x, "aye", 3))
			new_game();
	}
	exit(0);
}

void klingons_move(void)
{
	int i;
	struct klingon *k = kdata;

	for (i = 0; i <= 2; i++) {
		if (k->energy > 0) {
			wipe_klingon(k);

			fs_empty_pl(Q_KLINGON, &k->y, &k->x);
		}
		k++;
	}

	klingons_shoot();
}

char *dcr_1 = "Damage Control report:";

void klingons_shoot(void)
{
	unsigned long h;
	char i;
	struct klingon *k = kdata;

	if (klingons <= 0)
		return;

	if (docked) {
		puts("Starbase shields protect the Enterprise\n");
		return;
	}

	for (i = 0; i <= 2; i++) {
		if (k->energy > 0) {
			h = k->energy * (200 + get_rand(100));
			h *= 100;	/* Ready for division in fixed */
			h /= distance_to(k);
			/* Takes us back into FIXED00 */
			shield = shield - FROM_FIXED00(h);

			k->energy = (k->energy * 100) / (300 + get_rand(100));

			printf("%d unit hit on Enterprise from sector "
			       "%d, %d\n", (int)(FROM_FIXED00(h)), k->y, k->x);

			if (shield <= 0) {
				putchar('\n');
				ship_destroyed();
			}

			printf("    <Shields down to %d units>\n\n", shield);

			if (h >= 20) {
				/* The check in basic is float and is h/s >.02. We
				   have to use 32bit values here to avoid an overflow
				   FIXME: use a better algorithm perhaps ? */
				long ratio = ((long)h)/shield;
				if (get_rand(10) <= 6 || ratio > 2) {
					char r = rand8();
					/* The original basic code computed h/s in
					   float form the C conversion broke this. We correct it in the fixed
					   point change */
					damage[r] = damage[r] - (ratio + get_rand(50));

					/* FIXME: can we use dcr_1 here ?? */
					printf("Damage Control reports\n"
					       "   '%s' damaged by hit\n\n", get_dev_name(r));
				}
			}
		}
		k++;
	}
}

void repair_damage(int warp)
{
	int i;
	int d1;
	int repair_factor;		/* Repair Factor */

	repair_factor = warp;
	if (warp >= 100)
		repair_factor = TO_FIXED00(1);

	for (i = 1; i <= 8; i++) {
		if (damage[i] < 0) {
			damage[i] = damage[i] + repair_factor;
			if (damage[i] > -10 && damage[i] < 0)	/* -0.1 */
				damage[i] = -10;
			else if (damage[i] >= 0) {
				if (d1 != 1) {
					d1 = 1;
					puts(dcr_1);
				}
				printf("    %s repair completed\n\n",
					get_dev_name(i));
				damage[i] = 0;
			}
		}
	}

	if (get_rand(10) <= 2) {
		char r = rand8();

		if (get_rand(10) < 6) {
			/* Working in 1/100ths */
			damage[r] = damage[r] - ((get_rand(500) + 100));
			puts(dcr_1);
			printf("    %s damaged\n\n", get_dev_name(r));
		} else {
			/* Working in 1/100ths */
			damage[r] = damage[r] + get_rand(300) + 100;
			puts(dcr_1);
			printf("    %s state of repair improved\n\n",
					get_dev_name(r));
		}
	}
}

/* Misc Functions and Subroutines
   Returns co-ordinates r1/r2 and for now z1/z2 */

void fs_empty_pl(char t, char *z1, char *z2)
{
	char r1, r2;
	do {
		r1 = rand8();
		r2 = rand8();
	} while (quad[r1-1][r2-1] != Q_SPACE );
	quad[r1-1][r2-1] = t;
	if (z1)
		*z1 = r1;
	if (z2)
		*z2 = r2;
}

char *device_name[] = {
	"", "Warp engines", "Short range sensors", "Long range sensors",
	"Phaser control", "Photon tubes", "Damage control", "Shield control",
	"Library computer"
};

char *get_dev_name(int n)
{
	if (n < 0 || n > 8)
		n = 0;
	return device_name[n];
}

char *quad_name[] = { "",
	"Antares", "Rigel", "Procyon", "Vega", "Canopus", "Altair",
	"Sagittarius", "Pollux", "Sirius", "Deneb", "Capella",
	"Betelgeuse", "Aldebaran", "Regulus", "Arcturus", "Spica"
};

char *sect_name[] = { "", " I", " II", " III", " IV" };

void quadrant_name(char small, char y, char x)
{
	if (y < 1 || y > 8 || x < 1 || x > 8)
		strcpy(quadname, "Unknown");

	if (x <= 4)
		strcpy(quadname, quad_name[y]);
	else
		strcpy(quadname, quad_name[y + 8]);

	if (small != 1) {
		if (x > 4)
			x = x - 4;
		strcat(quadname, sect_name[x]);
	}

	return;
}

/* An unsigned sqrt is all we need.

   What we are actually doing here is a smart version of calculating n^2
   repeatedly until we find the right one */
int isqrt(int i)
{
	int b = 0x4000, q = 0, r = i, t;
	while (b) {
		t = q + b;
		q >>= 1;
		if (r >= t) {
			r -= t;
			q += b;
		}
		b >>= 2;
	}
	return q;
}

int square00(int t)
{
	if (abs(t) > 181) {
		t /= 10;
		t *= t;
	} else {
		t *= t;
		t /= 100;
	}
	return t;
}

/* Return the distance to an object in x.xx fixed point */
int distance_to(struct klingon *k)
{
	int j;

	/* We do the squares in fixed point maths */
	j = square00(TO_FIXED00(k->y) - ship_y);
	j += square00(TO_FIXED00(k->x) - ship_x);

	/* Find the integer square root */
	j = isqrt(j);
	/* Correct back into 0.00 fixed point */
	j *= 10;

	return j;
}


/* Round off floating point numbers instead of truncating */

int cint100(int d)
{
	return (d + 50) / 100;
}

void showfile(char *filename)
{
	FILE *fp;
	int row = 0;
	char buffer[200];

	fp = fopen(filename, "r");
	if (fp == NULL) {
		perror(filename);
		return;
	}
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		fprintf(stderr, buffer);
		if (row++ > MAXROW - 3) {
			getchar();
			row = 0;
		}
	}
	fclose(fp);
}
