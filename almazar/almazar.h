typedef unsigned char bool;
enum { false=0, true=1 };

#define SOFT_NL "\n"

#define apport(obj, loc) object_position[obj] = loc
#define toting(obj) (object_position[obj] == -1)
#define there(obj, loc) (object_position[obj] == loc)

#define V(e, w) if (strncmp(word, w, 4) == 0) return e

#define object(obj) apport(obj, iplace);

enum {
    R_LIMBO=0, R_START, R_SHACK, R_STREWN, R_FOREST1, R_FOREST2, R_FOREST3,
    R_ROAD, R_OIL, R_JUNC, R_CAVE, R_FILTER, R_BROKEN, R_REMAINS, R_HALL1,
    R_LIGHTED, R_TOOL, R_CORR, R_MAZE18, R_MAZE19, R_MAZE20, R_MAZE21,
    R_MAZE22, R_MAZE23, R_TALL, R_HALL2, R_MEET, R_CHASM, R_LEDGE, R_NARROW,
    R_TEE, R_MINE, R_TREE, R_MINE33, R_MINE34, R_MINE35, R_MINE36, R_MINE37,
    R_MINE38, R_WINDS, R_ROAD2, R_HILL, R_ROCK, R_RIVER, R_WELCOME,
    R_CROSSROADS, R_HOLEY, R_STABLE, R_END, R_RIVERFRONT, R_GARGOYLE,
    R_OUTOUT, R_IDOL, R_GARDEN, R_ORCHARD, R_HOUSE, R_FOYER, R_DINING,
    R_KITCHEN, R_LIVING, R_HIDDEN, R_BASEMENT, R_OUTSIDE, R_RECEPTION,
    R_MIDDLE, R_LIBRARY, R_BEDROOM, R_CLOSET, R_TOP, R_ATTIC, R_OUTHOUSE,
    R_LIFT, R_VAULT
};

typedef enum {
    NO_VERB,
    NORTH, EAST, SOUTH, WEST, UP, DOWN, LOOK, HINT, INVENTORY,
    JUMP, SCORE, QUIT, SAVE, RESTORE, ENTER, LEAVE,
    GET, DROP, WAVE, SHOW, READ, FILL, LIGHT, KILL, KICK,
    BREAK, TOSS, EAT, DRINK, FEED, TURN, UNLOCK, CROSS, SWIM,
    OPEN, CLOSE, OFF, PULL, EMPTY
};

typedef enum {
    NO_OBJECT,
    // These nouns denote portable objects.
    // - SPICES means the spices in the bottle.
    // - BOTTLE means the empty bottle.
    DIAMOND, EYE, CHAIN, APPLE, PEARLS, SPICES, DAGGER, FIGURINE,
    LAMP, KEY, RING, ROD, STAFF, INCENSE, HAY, CHEESE, MOUSE,
    MATCHES, BOTTLE, WATER_BOTTLE,

    // These nouns denote non-portable objects with location.
    // Their order matters to describe_room().
    EYE_IN_PLACE, GNOME, SPICES_IN_PLACE, WIZARD, SNAKE, BALL,
    FIGURINE_IN_PLACE, OPEN_WINDOW, CLOSED_WINDOW, OPEN_PANEL,

    // These nouns are used by the parser, but have no location.
    // Their order doesn't matter.
    IDOL, CHASM, RIVER, HEAD,
    BOOK, MESSAGE, OIL, WATER, VAULT, CHEST, DRAWER, WINDOW,
    ROPE, SIGN
};

typedef enum {
    STATUS_LOCKED, STATUS_UNLOCKED, STATUS_OPEN
};

struct Place {
    char *long_desc;
    char *short_desc;
    int neighbors[6];
    bool visited;
};

int atoi(char*);
void* malloc(int);

void xrndseed(void);
unsigned int xrnd(void);

bool save(void);
bool restore(void);
