// -------------------------------------------------------------------------------------------
//
// The Search for Almazar (Winston M. Llamas, 1981)
//
// The Search for Almazar was written in BASIC by Winston Llamas, 
// a computer science student at RPI, circa 1981. 
//
// -------------------------------------------------------------------------------------------
// Port of Almazar in C by Arthur O'Dwyer, in May 2021. 
//
// Arthur O'Dwyer's comments:
//
// For the original source code of Almazar (or as close as I've been able to get),
//  see https://github.com/Quuxplusone/Almazar .
// The code is based the code mainly on Bob Liddelow's SIG/M version, 
// but based the message texts on Llamas's 80 Micro version. 
// (Liddelow's texts were obviously abbreviated, 
// and he'd discarded the short room descriptions entirely.)
// This is not a perfectly faithful port:
// 80 Micro's messages are all-caps, except for the intro text. 
// I've lowercased them, not adhering closely to Liddelow's choices.
// I've "tersened" some short room descriptions in Adventure style; 
// for example, 80 Micro says YOU'RE IN A THREE WAY JUNCTION, but I say You're at three-way junction.
// I've copyedited lots of punctuation and prepositions, and typos, and the spelling of PHYRANIAS (piranhas).
// I've fixed an original bug where if you were carrying the bottle of water when you died, 
//  it would not get randomly scattered with the rest of your possessions.
// -------------------------------------------------------------------------------------------
//
// The current version was adapted to HiTech C by Ladislau Szilagyi, in October 2023
//
// It will run on any Z80 machine under CP/M
//
// Upper to lower letters conversion was adapted to the HiTech C run-time library
// Save & Restore game was added (using the file almazar.sav)
// Some large strings containing introduction to the game and locations initialization
//  were exported to separate files (almazar.txt and almazar.ini), in order
//  to obtain a smaller executable file
//
// -------------------------------------------------------------------------------------------
 
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "almazar.h"

int iplace;

struct Place places[R_VAULT+1];

int object_position[OPEN_PANEL + 1];  // OP in the original program
int lamp_counter = 0;                 // SP1 in the original program
bool lamp_is_on = false;              // SP2 in the original program
bool mouse_is_fed = false;            // SP3 in the original program
// SP4 in the original program was never used
bool wizard_is_dead = false;          // SP5 in the original program
int wizard_counter = 0;               // SP6 in the original program
int vault_status = STATUS_LOCKED;  // SP7 in the original program
bool river_is_open = false;           // SP8 in the original program
bool chasm_is_bridged = false;        // SP9 in the original program
// SP10 in the original program was window_is_open, but we don't need it
int drawer_status = STATUS_LOCKED; // SP11 in the original program
int chest_status = STATUS_LOCKED;  // SP12 in the original program
int darkness_counter = 0;             // SP13 in the original program
int river_counter = 0;                // SP14 in the original program
bool chased_by_gnome = false;         // SC in the original program
bool heard_voiceover = false;         // VO in the original program
bool lamp_has_dimmed = false;         // W1 in the original program
bool better_hurry = false;            // W2 in the original program
int hint_penalties = 0;               // PR in the original program
int turn_counter = 0;                 // TU in the original program
int death_counter = 0;                // DI in the original program

FILE *fpini;

// defined in attempt.c
int attempt_move_to_room(int loc, int nextloc);
void attempt_wave(int iobj, int loc);
void attempt_show(int iobj, int loc);
int attempt_read(int iobj, int loc);
void attempt_fill(int iobj, int loc);
void attempt_light(int iobj, int loc);
int attempt_kill(int iobj, int loc);
int attempt_kick(int iobj, int loc);
void attempt_break(int iobj, int loc);
void attempt_toss(int iobj, int loc);
int attempt_eat(int iobj, int loc);
void attempt_drink(int iobj, int loc);
void attempt_feed(int iobj, int loc);
void attempt_turn(int iobj, int loc);
void attempt_unlock(int iobj, int loc);
int attempt_jump(int loc);
int attempt_cross(int iobj, int loc);
int attempt_swim(int iobj, int loc);
void attempt_open(int iobj, int loc);
void attempt_close(int iobj, int loc);
void attempt_off(int iobj);
int attempt_pull(int iobj, int loc);
void attempt_empty(int iobj);
int attempt_enter(int loc);
int attempt_leave(int loc);
void attempt_hint(int loc);
void attempt_inventory(void);
int attempt_score(bool winning);
void attempt_quit(void);
int attempt_get(int iobj, int loc);
void attempt_drop(int iobj, int loc);

void n_to(int loc) { places[iplace].neighbors[NORTH - NORTH] = loc; }
void e_to(int loc) { places[iplace].neighbors[EAST - NORTH] = loc; }
void s_to(int loc) { places[iplace].neighbors[SOUTH - NORTH] = loc; }
void w_to(int loc) { places[iplace].neighbors[WEST - NORTH] = loc; }
void u_to(int loc) { places[iplace].neighbors[UP - NORTH] = loc; }
void d_to(int loc) { places[iplace].neighbors[DOWN - NORTH] = loc; }

void extra_newline(void) { puts(""); }

void putsn(char* s) { puts(s); extra_newline(); }

int ran(int range) { return xrnd() % range; }

int lookup_verb( char *word)
{
    V(NORTH, "nort"); V(NORTH, "n");
    V(EAST, "east"); V(EAST, "e");
    V(SOUTH, "sout"); V(SOUTH, "s");
    V(WEST, "west"); V(WEST, "w");
    V(UP, "up"); V(UP, "u");
    V(DOWN, "down"); V(DOWN, "d");
    V(LOOK, "look");
    V(HINT, "hint"); V(HINT, "help");
    V(INVENTORY, "inve");
    V(JUMP, "jump");
    V(SCORE, "scor");
    V(QUIT, "quit");
    V(SAVE, "save");
    V(RESTORE, "rest");
    V(ENTER, "ente");
    V(LEAVE, "leav");
    V(GET, "get"); V(GET, "take"); V(GET, "t"); 
    V(DROP, "drop"); V(DROP, "dr");
    V(WAVE, "wave");
    V(SHOW, "show");
    V(READ, "read");
    V(FILL, "fill");
    V(LIGHT, "ligh"); V(LIGHT, "burn");
    V(KILL, "kill"); V(KILL, "atta");
    V(KICK, "kick");
    V(BREAK, "brea"); V(BREAK, "smas");
    V(TOSS, "toss"); V(TOSS, "thro");
    V(EAT, "eat");
    V(DRINK, "drink");
    V(FEED, "feed");
    V(TURN, "turn");
    V(UNLOCK, "unlo");
    V(CROSS, "cros");
    V(SWIM, "swim");
    V(OPEN, "open");
    V(CLOSE, "clos");
    V(OFF, "off");
    V(PULL, "pull");
    V(EMPTY, "empt");

    return NO_VERB;
}

int lookup_noun( char *word)
{
    V(DIAMOND, "diam");
    V(EYE, "emer"); V(EYE, "eye");
    V(CHAIN, "chai"); V(CHAIN, "plat");
    V(APPLE, "appl"); V(APPLE, "gold");
    V(PEARLS, "pear");
    V(SPICES, "spic");
    V(DAGGER, "dagg"); V(DAGGER, "knif");
    V(FIGURINE, "figu");
    V(LAMP, "lamp");
    V(KEY, "key");
    V(RING, "ring");
    V(ROD, "rod");
    V(STAFF, "staff");
    V(INCENSE, "ince");
    V(HAY, "hay"); V(HAY, "stra");
    V(CHEESE, "chee");
    V(MOUSE, "mous");
    V(MATCHES, "matc");
    V(BOTTLE, "bott");
    V(IDOL, "idol");
    V(SNAKE, "snak");
    V(CHASM, "chas"); V(CHASM, "abys");
    V(WIZARD, "wiza");
    V(RIVER, "rive");
    V(HEAD, "head");
    V(BOOK, "book");
    V(MESSAGE, "mess");
    V(BALL, "ball"); V(BALL, "crys");
    V(OIL, "oil");
    V(WATER, "wate");
    V(VAULT, "vaul");
    V(CHEST, "ches");
    V(DRAWER, "draw");
    V(WINDOW, "wind");
    V(ROPE, "rope");
    V(SIGN, "sign");
    V(GNOME, "gnom");

    return NO_OBJECT;
}

bool is_transitive(int v)
{
    return (GET <= v && v <= EMPTY);
}

void listen(int *iverb, int *iobj)
{
    char buffer[72];
    char *first_word;
    char *second_word;
    char *p;

    printf("? "); 
    fgets(buffer, sizeof(buffer), stdin);
    for (p = buffer; *p != '\0'; ++p) 
    {
        if (*p == '\n')
	    *p = 0;
	else if (isalpha(*p))
        {
            if (isupper(*p))
                *p = tolower(*p);
        }
    }
    first_word = buffer;
    second_word = strchr(buffer, ' ');
    if (second_word != NULL) {
        *second_word++ = '\0';
    } else {
        second_word = "";
    }
    *iverb = lookup_verb(first_word);
    if (is_transitive(*iverb)) {
        *iobj = lookup_noun(second_word);
    }
}

bool yes( char *q)
{
    char buffer[72];

    while (true) {
        printf("%s ", q); 
        fgets(buffer, sizeof(buffer), stdin);
        extra_newline();

        if (isalpha(*buffer))
        {
            if (isupper(*buffer))
                *buffer = tolower(*buffer);
        }

        if (*buffer == 'y') {
            return true;
        } else if (*buffer == 'n') {
            return false;
        } else {
            putsn(" Please answer Yes or No.");
        }
    }
    return false;
}

bool vault_challenge(void)
{
    char buffer[72];
    int first, second, third;

    printf("Left? "); 
    fgets(buffer, sizeof(buffer), stdin);
    first = atoi(buffer);
    printf("Right? "); 
    fgets(buffer, sizeof(buffer), stdin);
    second = atoi(buffer);
    printf("Left? "); 
    fgets(buffer, sizeof(buffer), stdin);
    third = atoi(buffer);
    return (first == 3 && second == 7 && third == 21);
}

void describe_object(int obj)
{
    switch (obj) {
        case DIAMOND: putsn("A large diamond is lying here!"); break;
        case EYE: putsn("There's an emerald eye here!"); break;
        case CHAIN: putsn("A platinum chain is lying here!"); break;
        case APPLE: putsn("There is a golden apple here!"); break;
        case PEARLS: putsn("A string of pearls is lying here!"); break;
        case SPICES: putsn("There's a bottle of rare spices lying here!"); break;
        case DAGGER: putsn("There is a jewel-encrusted dagger here!"); break;
        case FIGURINE: putsn("A priceless figurine is sitting on some hay!"); break;
        case LAMP: putsn("There is an old oil lamp here."); break;
        case KEY: putsn("An old brass key is sitting here."); break;
        case RING: putsn("There is a worthless-looking ring here."); break;
        case ROD: putsn("An old black rod is sitting here."); break;
        case STAFF: putsn("There's an ancient-looking staff lying here."); break;
        case INCENSE: putsn("There's a package of incense lying here."); break;
        case HAY: putsn("There's a bale of hay here."); break;
        case CHEESE: putsn("A package of Kraft cheese is lying here."); break;
        case MOUSE: putsn("There is a noisy little mouse here."); break;
        case MATCHES: putsn("There's a box of matches lying here."); break;
        case BOTTLE: putsn("There's an empty bottle here."); break;
        case WATER_BOTTLE: putsn("There's a brownish bottle of water here."); break;
        case EYE_IN_PLACE: putsn("An emerald eye sits on top of the idol."); break;
        case GNOME: putsn("There is a knife-wielding gnome in the room with you!"); break;
        case SPICES_IN_PLACE: putsn("There are rare spices sitting on the ground."); break;
        case WIZARD: putsn("An angry wizard wearing a platinum chain is in the room with you!"); break;
        case SNAKE: putsn("A giant snake is jealously guarding a golden apple."); break;
        case BALL: putsn("There is a large crystal ball on the ground."); break;
        case FIGURINE_IN_PLACE: putsn("A priceless figurine stands on the table."); break;
        case OPEN_WINDOW: putsn("The window is open."); break;
        case CLOSED_WINDOW: putsn("The window is closed."); break;
        case OPEN_PANEL: putsn("There is a panel open to the north."); break;
        default: break;
    }
}

bool has_darkness(int loc)
{
    if (loc == R_BROKEN ||
	loc == R_REMAINS ||
	loc == R_HALL1 ||
	loc == R_TOOL ||
	loc == R_CORR ||
	loc == R_MAZE18 ||
	loc == R_MAZE19 ||
	loc == R_MAZE20 ||
	loc == R_MAZE21 ||
	loc == R_MAZE22 ||
	loc == R_MAZE23 ||
	loc == R_TALL)
	return true;

    if (loc == R_HALL2 ||
	loc == R_MEET ||
	loc == R_CHASM ||
	loc == R_LEDGE ||
	loc == R_NARROW ||
	loc == R_MINE33 ||
	loc == R_MINE34 ||
	loc == R_MINE35 ||
	loc == R_MINE36 ||
	loc == R_MINE37 ||
	loc == R_MINE38) 
	return true;

    return false;

//    switch (loc) {
//        case R_BROKEN: case R_REMAINS: case R_HALL1:
//        case R_TOOL: case R_CORR: case R_MAZE18: case R_MAZE19:
//        case R_MAZE20: case R_MAZE21: case R_MAZE22: case R_MAZE23:
//        case R_TALL: case R_HALL2: case R_MEET: case R_CHASM:
//        case R_LEDGE: case R_NARROW: case R_MINE33: case R_MINE34:
//        case R_MINE35: case R_MINE36: case R_MINE37: case R_MINE38:
//            return true;
//        default:
//            return false;
//    }
}

bool has_river(int loc)
{
    return (loc == R_RIVER || loc == R_WELCOME || loc == R_RIVERFRONT);
}

void describe_room(int loc)
{
    bool has_light = (toting(LAMP) || there(LAMP, loc)) && lamp_is_on;
    int i;

    if (has_darkness(loc) && !has_light) {
        putsn("There is insufficient light to see by.");
        if (darkness_counter == 0) {
            darkness_counter = 1;
        }
    } else {
        darkness_counter = 0;
        if (places[loc].visited || places[loc].long_desc == NULL) {
            putsn(places[loc].short_desc);
        } else {
            putsn(places[loc].long_desc);
            places[loc].visited = true;
        }
        for (i = DIAMOND; i <= OPEN_PANEL; ++i) {
            if (there(i, loc) && i != GNOME) {
                describe_object(i);
            }
        }
    }
    if (loc == R_HILL && !heard_voiceover) {
        putsn("A loud voice emanates from beyond, saying: \"Time grows short," SOFT_NL
              "mortal, before we feast again.\"");
        heard_voiceover = true;
    }
}

void offer_hint( char *hint)
{
    if (yes("The hint will cost you 3 points. Do you still want it?")) {
        putsn(hint);
    } else {
        putsn("OK.");
    }
}

int burden(void) 
{
    int count = 0;
    int x;

    for (x = DIAMOND; x <= WATER_BOTTLE; ++x) {
        if (toting(x)) count += 1;
    }
    return count;
}

void describe_inventory_object(int x)
{
    switch (x) {
        case DIAMOND: puts("    Large diamond"); break;
        case EYE: puts("    Emerald eye"); break;
        case CHAIN: puts("    Platinum chain"); break;
        case APPLE: puts("    Golden apple"); break;
        case PEARLS: puts("    String of pearls"); break;
        case SPICES: puts("    Bottle of spices"); break;
        case DAGGER: puts("    Jeweled dagger"); break;
        case FIGURINE: puts("    Priceless figurine"); break;
        case LAMP: puts("    Oil lamp"); break;
        case KEY: puts("    Brass key"); break;
        case RING: puts("    Ring"); break;
        case ROD: puts("    Black rod"); break;
        case STAFF: puts("    Wooden staff"); break;
        case INCENSE: puts("    Incense"); break;
        case HAY: puts("    Hay"); break;
        case CHEESE: puts("    Kraft cheese"); break;
        case MOUSE: puts("    Noisy mouse"); break;
        case MATCHES: puts("    Matches"); break;
        case BOTTLE: puts("    Empty bottle"); break;
        case WATER_BOTTLE: puts("    Bottle of water"); break;
        default: break;
    }
}

void final_score_and_exit(bool winning)
{
    int pt = attempt_score(winning);

    if (pt == 90) putsn("All honor thee, Grandmaster Adventurer!");
    else if (pt >= 80) putsn("You are now an advanced adventurer.");
    else if (pt >= 36) putsn("You may now consider yourself an intermediate in this game.");
    else if (pt >= 8) putsn("You are obviously a rank novice in this game.");
    else putsn("You obviously have no idea what's happening in this game.");
    exit(0);
}

int random_death_drop_room(void)
{
    // In the original program, R_LIMBO was a possible outcome here.
    return R_START + ran(R_LIGHTED - R_START + 1);
}

int death(void)
{
    int x;

    death_counter += 1;
    if (death_counter == 3) {
        putsn("You have died three times. The game is therefore suspended.");
        final_score_and_exit(false);
    }
    if (!yes("Do you want another chance?")) {
        final_score_and_exit(false);
    }

    // Reincarnate the player.
    if (death_counter == 1) {
        putsn("Everything spins around you as a wall of fire encircles you." SOFT_NL
              "When you wake up you find that...");
    } else {
        putsn("A small wall of fire barely surrounds you. You regain" SOFT_NL
              "consciousness and find that...");
    }
    for (x = DIAMOND; x <= WATER_BOTTLE; ++x) {
        // In the original program, this loop didn't include WATER_BOTTLE.
        if (toting(x)) apport(x, random_death_drop_room());
    }
    apport(LAMP, R_SHACK);
    apport(MATCHES, R_FOREST3);
    apport(STAFF, R_ROCK);
    lamp_is_on = false;
    river_is_open = false;
    river_counter = 0;
    chased_by_gnome = false;
    return attempt_move_to_room(R_LIMBO, R_START);
}

bool is_portable(int obj)
{
    return (DIAMOND <= obj && obj <= BOTTLE);
}

void print_default_message(int iobj)
{
    if (toting(iobj)) {
        putsn("Nothing happens.");
    } else if (is_portable(iobj)) {
        putsn("You're not carrying it.");
    } else {
        putsn("I don't understand.");
    }
}

void init_place(int index, char* key)
{
	char* p;
	char buffer[300];

	if (fgets(buffer, sizeof(buffer), fpini) != NULL)
	{
		if (!strncmp(buffer, key, strlen(key)))
		{
			++iplace; 
			places[iplace].visited = false;

			fgets(buffer, sizeof(buffer), fpini);
			p = (char*)malloc(strlen(buffer) + 1);
			strcpy(p, buffer);
			places[iplace].long_desc = p;

			fgets(buffer, sizeof(buffer), fpini);
			if (strlen(buffer))
			{
				p = (char*)malloc(strlen(buffer) + 1);
				strcpy(p, buffer);
				places[iplace].short_desc = p;
			}
			else
				places[iplace].short_desc = NULL;

			return;
		}
	}

	printf("Key %s not found!\n", key);
	exit(0);
}

void initialize_rooms1(void)
{
    iplace = 0;
    fpini = fopen("ALMAZAR.INI", "r");

    if (fpini == NULL) 
    {
	printf("almazar.ini file not found !\n");
	exit(0);
    }

    init_place(R_START, "START");
        n_to(R_ROAD); e_to(R_SHACK); w_to(R_STREWN);
    init_place(R_SHACK, "SHACK");
        w_to(R_START); object(LAMP); object(KEY);
    init_place(R_STREWN, "STREWN");
        e_to(R_START); w_to(R_FOREST1); n_to(R_FOREST3);
    init_place(R_FOREST1, "FOREST1");
        n_to(R_FOREST2); e_to(R_STREWN);
    init_place(R_FOREST2, "FOREST2");
        s_to(R_FOREST1); e_to(R_FOREST3);
    init_place(R_FOREST3, "FOREST3");
        s_to(R_STREWN); w_to(R_FOREST2); object(MATCHES);
    init_place(R_ROAD, "ROAD");
        s_to(R_START); n_to(R_JUNC); e_to(R_OIL);
    init_place(R_OIL, "OIL");
        n_to(R_TREE); w_to(R_ROAD);
    init_place(R_JUNC, "JUNC");
        n_to(R_TEE); s_to(R_ROAD); w_to(R_CAVE);
    init_place(R_CAVE, "CAVE");
        e_to(R_JUNC); w_to(R_FILTER);
    init_place(R_FILTER, "FILTER");
        e_to(R_CAVE); w_to(R_NARROW); s_to(R_BROKEN);
    init_place(R_BROKEN, "BROKEN");
        n_to(R_FILTER); s_to(R_REMAINS);
    init_place(R_REMAINS, "REMAINS");
        n_to(R_BROKEN); w_to(R_HALL1);
    init_place(R_HALL1, "HALL1");
        w_to(R_LIGHTED); e_to(R_REMAINS);
    init_place(R_LIGHTED, "LIGHTED");
        w_to(R_CORR); e_to(R_HALL1); s_to(R_TOOL); object(DAGGER);
    init_place(R_TOOL, "TOOL");
        u_to(R_LEDGE); n_to(R_LIGHTED);
    init_place(R_CORR, "CORR");
        w_to(R_MAZE18); e_to(R_LIGHTED);
    init_place(R_MAZE18, "MAZE18");
        n_to(R_MAZE18); e_to(R_MAZE18); s_to(R_MAZE19); w_to(R_MAZE23);
        u_to(R_MAZE18); d_to(R_MAZE18);
    init_place(R_MAZE19, "MAZE19");
        n_to(R_MAZE18); e_to(R_MAZE19); s_to(R_MAZE19); w_to(R_MAZE20);
        u_to(R_MAZE19); d_to(R_CORR);
    init_place(R_MAZE20, "MAZE20");
        n_to(R_MAZE21); e_to(R_MAZE19); s_to(R_MAZE20); w_to(R_MAZE22);
        u_to(R_MAZE20); d_to(R_MAZE20);
}

void initialize_rooms2(void)
{
    init_place(R_MAZE21, "MAZE21");
        s_to(R_MAZE20); object(ROD);
    init_place(R_MAZE22, "MAZE22");
        n_to(R_MAZE23); e_to(R_MAZE20); s_to(R_MAZE22); w_to(R_MAZE22);
        u_to(R_MAZE22); d_to(R_MAZE22);
    init_place(R_MAZE23, "MAZE23");
        n_to(R_MAZE23); e_to(R_MAZE18); s_to(R_MAZE22); w_to(R_MAZE23);
        u_to(R_TALL); d_to(R_MAZE23);
    init_place(R_TALL, "TALL");
        s_to(R_MAZE23); e_to(R_HALL2); object(BOTTLE);
    init_place(R_HALL2, "HALL2");
        w_to(R_TALL); e_to(R_MEET);
    init_place(R_MEET, "MEET");
        w_to(R_HALL2); e_to(R_NARROW); n_to(R_CHASM);
    init_place(R_CHASM, "CHASM");
        s_to(R_MEET); n_to(R_LEDGE);
    init_place(R_LEDGE, "LEDGE");
        s_to(R_CHASM); d_to(R_TOOL); object(SPICES_IN_PLACE);
    init_place(R_NARROW, "NARROW");
        w_to(R_MEET); e_to(R_FILTER);
    init_place(R_TEE, "TEE");
        e_to(R_MINE); s_to(R_JUNC); n_to(R_WINDS);
    init_place(R_MINE, "MINE");
        e_to(R_MINE33); s_to(R_TREE); w_to(R_TEE);
    init_place(R_TREE, "TREE");
        n_to(R_MINE); s_to(R_OIL); object(BALL);
    init_place(R_MINE33, "MINE33");
        n_to(R_MINE38); e_to(R_MINE36); s_to(R_MINE34); w_to(R_MINE);
        u_to(R_MINE33); d_to(R_MINE33);
    init_place(R_MINE34, "MINE34");
        n_to(R_MINE33); e_to(R_MINE35); s_to(R_MINE34); w_to(R_MINE34);
        u_to(R_MINE34); d_to(R_MINE34);
    init_place(R_MINE35, "MINE35");
        n_to(R_MINE36); e_to(R_MINE35); s_to(R_MINE35); w_to(R_MINE34);
        u_to(R_MINE35); d_to(R_MINE35);
    init_place(R_MINE36, "MINE36");
        n_to(R_MINE37); e_to(R_MINE36); s_to(R_MINE35); w_to(R_MINE33);
        u_to(R_MINE36); d_to(R_MINE36);
    init_place(R_MINE37, "MINE37");
        n_to(R_MINE37); e_to(R_MINE37); s_to(R_MINE36); w_to(R_MINE38);
        u_to(R_MINE37); d_to(R_MINE37);
    init_place(R_MINE38, "MINE38");
        n_to(R_MINE38); e_to(R_MINE37); s_to(R_MINE33); w_to(R_MINE38);
        u_to(R_MINE38); d_to(R_MINE38);
        object(DIAMOND);
}

void initialize_rooms3(void)
{
    init_place(R_WINDS, "WINDS");
        s_to(R_TEE); n_to(R_ROAD2);
    init_place(R_ROAD2, "ROAD2");
        e_to(R_HILL); n_to(R_RIVER); s_to(R_WINDS);
    init_place(R_HILL, "HILL");
        w_to(R_ROAD2); e_to(R_ROCK);
    init_place(R_ROCK, "ROCK");
        w_to(R_HILL);
        object(STAFF);
    init_place(R_RIVER, "RIVER");
        n_to(R_WELCOME); s_to(R_ROAD2);
    init_place(R_WELCOME, "WELCOME");
        s_to(R_RIVER); n_to(R_CROSSROADS);
    init_place(R_CROSSROADS, "CROSSROADS");
        n_to(R_GARGOYLE); e_to(R_HOLEY); s_to(R_WELCOME); w_to(R_END);
    init_place(R_HOLEY, "HOLEY");
        w_to(R_CROSSROADS); e_to(R_STABLE);
    init_place(R_STABLE, "STABLE");
        w_to(R_HOLEY); n_to(R_ORCHARD); object(HAY);
    init_place(R_END, "END");
        s_to(R_RIVERFRONT); n_to(R_OUTOUT); e_to(R_CROSSROADS);
    init_place(R_RIVERFRONT, "RIVERFRONT");
        n_to(R_END); object(RING);
    init_place(R_GARGOYLE, "GARGOYLE");
        w_to(R_OUTOUT); e_to(R_GARDEN); s_to(R_CROSSROADS); n_to(R_HOUSE);
}

void initialize_rooms4(void)
{
    init_place(R_OUTOUT, "OUTOUT");
        w_to(R_IDOL); e_to(R_GARGOYLE); s_to(R_END); n_to(R_OUTHOUSE);
    init_place(R_IDOL, "IDOL");
        e_to(R_OUTOUT); object(EYE_IN_PLACE);
    init_place(R_GARDEN, "GARDEN");
        w_to(R_GARGOYLE); e_to(R_ORCHARD);
    init_place(R_ORCHARD, "ORCHARD");
        w_to(R_GARDEN); s_to(R_STABLE); object(SNAKE);
    init_place(R_HOUSE, "HOUSE");
        n_to(R_FOYER); s_to(R_GARGOYLE);
    init_place(R_FOYER, "FOYER");
        w_to(R_DINING); e_to(R_LIVING); n_to(R_RECEPTION); s_to(R_HOUSE);
    init_place(R_DINING, "DINING");
        n_to(R_KITCHEN); e_to(R_FOYER); object(CHEESE);
    init_place(R_KITCHEN, "KITCHEN");
        s_to(R_DINING); object(INCENSE);
    init_place(R_LIVING, "LIVING");
        w_to(R_FOYER); n_to(R_HIDDEN);
    init_place(R_HIDDEN, "HIDDEN");
        s_to(R_LIVING);
    init_place(R_BASEMENT, "BASEMENT");
        n_to(R_OUTSIDE); object(WIZARD);
    init_place(R_OUTSIDE, "OUTSIDE");
        n_to(R_VAULT); s_to(R_BASEMENT); e_to(R_LIFT);
    init_place(R_RECEPTION, "RECEPTION");
        u_to(R_MIDDLE); s_to(R_FOYER); object(FIGURINE_IN_PLACE);
    init_place(R_MIDDLE, "MIDDLE");
        u_to(R_TOP); d_to(R_RECEPTION); w_to(R_BEDROOM); e_to(R_LIBRARY);
    init_place(R_LIBRARY, "LIBRARY");
        w_to(R_MIDDLE); object(OPEN_WINDOW);
    init_place(R_BEDROOM, "BEDROOM");
        e_to(R_MIDDLE); s_to(R_CLOSET);
    init_place(R_CLOSET, "CLOSET");
        n_to(R_BEDROOM); d_to(R_LIFT);
    init_place(R_TOP, "TOP");
        d_to(R_MIDDLE); w_to(R_ATTIC);
    init_place(R_ATTIC, "ATTIC");
        e_to(R_TOP); object(MOUSE);
    init_place(R_OUTHOUSE, "OUTHOUSE");
        s_to(R_OUTOUT);
    init_place(R_LIFT, "LIFT");
        w_to(R_OUTSIDE); u_to(R_CLOSET);
    init_place(R_VAULT, "VAULT");
        s_to(R_OUTSIDE);

    fclose(fpini);
}

int random_gnome_room(void)
{
    // Somewhere between R_START and R_MINE34, or maybe still R_LIMBO.
    return ran(35);
}

int deal_with_gnomes(int loc)
{
    int p;

    if (!chased_by_gnome) {
        return loc;
    }

    if (there(GNOME, R_LIMBO)) {
        apport(GNOME, random_gnome_room());
    }

    // The gnome walks in your direction, but numerically instead of geographically.
    if (object_position[GNOME] > loc) object_position[GNOME] -= 1;
    if (object_position[GNOME] < loc) object_position[GNOME] += 1;

    if (there(GNOME, loc)) {
        putsn("There is a knife-wielding gnome in the same room as you!");
        p = ran(100) + burden();
        if (p > 70) {
            putsn("He throws a small knife at you!");
            if (p > 90) {
                putsn("It gets you!");
                return death();
            } else {
                putsn("It misses you!");
            }
        }
    }
    return loc;
}

int deal_with_room(int loc)
{
    int iverb;
    int iobj;
    int nextloc;

    if (R_FILTER <= loc && loc <= R_NARROW) {
        loc = deal_with_gnomes(loc);
    }
    turn_counter += 1;
    if (turn_counter > 400) {
        putsn("The heavens part at their seams as the gods prepare their next" SOFT_NL
              "meal. Almazar can no longer protect you, for even the mightiest" SOFT_NL
              "of beings cannot alter that which is preordained. Your life is" SOFT_NL
              "now over. May you rest in peace.");
        death_counter = 3;
        final_score_and_exit(false);
    }
    if (river_is_open) {
        river_counter += 1;
    } else {
        river_counter = 0;
    }
    if (lamp_is_on) {
        lamp_counter -= 1;
        if (lamp_counter <= 0) lamp_is_on = false;
    }
    if (lamp_is_on && (lamp_counter < 20) && toting(LAMP) && !lamp_has_dimmed) {
        putsn("Your lamp is getting dim.");
        lamp_has_dimmed = true;
    }
    if (darkness_counter > 0) {
        darkness_counter += 1;
        if (darkness_counter > 5) {
            putsn("A giant ogre grabs you and throws you into a pit.");
            return death();
        }
    }
    if (turn_counter > 360 && !better_hurry) {
        putsn("You had better hurry. Time is running out.");
        better_hurry = true;
    }
    if (loc == R_LIGHTED) {
        chased_by_gnome = 1;
    }
    if (there(WIZARD, loc)) {
        wizard_counter += 1;
        if (wizard_counter > 5) {
            return attempt_kick(WIZARD, loc);
        }
    }
    if (river_counter >= 8) {
        putsn("You incompetent fool! You forgot to close the river and as a" SOFT_NL
              "result a flood has overrun the countryside.");
        river_is_open = false;
        river_counter = 0;
        return death();
    }

    listen(&iverb, &iobj);
    if ((iverb == NO_VERB) || (is_transitive(iverb) && iobj == NO_OBJECT)) {
        putsn("I don't understand.");
        return loc;
    }
    if (NORTH <= iverb && iverb <= DOWN) {
        if (iverb == NORTH && vault_status != STATUS_OPEN && loc == R_OUTSIDE) {
            putsn("You have to open the vault to get inside it.");
            return loc;
        } else if (iverb == NORTH && loc == R_LIVING && !there(OPEN_PANEL, loc)) {
            putsn("There is no way to go in that direction.");
            return loc;
        } else {
            nextloc = places[loc].neighbors[iverb - NORTH];
            if (nextloc == R_LIMBO) {
                putsn("There is no way to go in that direction.");
                return loc;
            } else {
                return attempt_move_to_room(loc, nextloc);
            }
        }
    } else {
        switch (iverb) {
            case LOOK: places[loc].visited = false; loc = attempt_move_to_room(loc, loc); break;
            case HINT: attempt_hint(loc); break;
            case INVENTORY: attempt_inventory(); break;
            case JUMP: loc = attempt_jump(loc); break;
            case SCORE: (void)attempt_score(false); break;
            case QUIT: attempt_quit(); break;
            case SAVE:
                if (save())
		    putsn("Saved.");
		else
                    putsn("Save failed!");
                break;
            case RESTORE:
		if (restore())
		    putsn("Restored.");
		else
		    putsn("Cannot find saved game!");
                break;
            case ENTER: loc = attempt_enter(loc); break;
            case LEAVE: loc = attempt_leave(loc); break;
            case GET: loc = attempt_get(iobj, loc); break;
            case DROP: attempt_drop(iobj, loc); break;
            case WAVE: attempt_wave(iobj, loc); break;
            case SHOW: attempt_show(iobj, loc); break;
            case READ: loc = attempt_read(iobj, loc); break;
            case FILL: attempt_fill(iobj, loc); break;
            case LIGHT: attempt_light(iobj, loc); break;
            case KILL: loc = attempt_kill(iobj, loc); break;
            case KICK: loc = attempt_kick(iobj, loc); break;
            case BREAK: attempt_break(iobj, loc); break;
            case TOSS: attempt_toss(iobj, loc); break;
            case EAT: loc = attempt_eat(iobj, loc); break;
            case DRINK: attempt_drink(iobj, loc); break;
            case FEED: attempt_feed(iobj, loc); break;
            case TURN: attempt_turn(iobj, loc); break;
            case UNLOCK: attempt_unlock(iobj, loc); break;
            case CROSS: loc = attempt_cross(iobj, loc); break;
            case SWIM: loc = attempt_swim(iobj, loc); break;
            case OPEN: attempt_open(iobj, loc); break;
            case CLOSE: attempt_close(iobj, loc); break;
            case OFF: attempt_off(iobj); break;
            case PULL: loc = attempt_pull(iobj, loc); break;
            case EMPTY: attempt_empty(iobj); break;
        }
    }
    return loc;
}

void title_screen(void)
{
	FILE *fp;
	char buffer[200];

	fp = fopen("ALMAZAR.TXT", "r");

	if (fp == NULL)
	{
		printf("almazar.txt file not found !\n");
		exit(0);
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL) 
		printf(buffer);

	fclose(fp);
}

void main(void)
{
    int loc;

    xrndseed();
    title_screen();
    initialize_rooms1();
    initialize_rooms2();
    initialize_rooms3();
    initialize_rooms4();
    loc = attempt_move_to_room(R_LIMBO, R_START);
    while (true) {
        loc = deal_with_room(loc);
    }
}
