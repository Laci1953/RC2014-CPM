/* defines, enums ------------------------------------------------------------- */

#define NUM_ROOMS  111 /* including null room 0 */
#define NUM_OBJECTS  80
#define INSIDE  2048

#define R_FOREST_1 6
#define R_FOREST_2 7
#define R_FOREST_3 9
#define R_PATH 10 
#define R_UP_A_TREE 11
#define R_ENTRANCE_TO_HADES 66
#define R_SOUTH_TEMPLE 73
#define R_WEST_OF_HOUSE 1
#define R_NORTH_OF_HOUSE 3
#define R_SOUTH_OF_HOUSE 4
#define R_EAST_OF_HOUSE 5
#define R_PATH 10
#define R_UP_A_TREE 11
#define R_GRATING_CLEARING 12
#define R_CLEARING 13
#define R_CANYON_VIEW 93
#define R_EGYPT_ROOM 69
#define R_LIVING_ROOM 16
#define R_TREASURE_ROOM 44

#define O_YOU 1
#define O_LAMP  36
#define O_INFLATED_BOAT  39
#define O_COFFIN 29
#define O_SWORD  61
#define O_THIEF 5
#define O_CHALICE 25

/*object bit flags*/

#define PROP_OPENABLE        1
#define PROP_OPEN            2
#define PROP_LIT             4
#define PROP_NODESC          8
#define PROP_NOTTAKEABLE    16
#define PROP_MOVEDDESC      32  /* set when object is first taken */
#define PROP_INSIDEDESC     64  /* set for objects that are initially 
					described as inside another object */
#define PROP_SACRED        128  /* set for objects that aren't allowed to be taken by thief */
#define PROP_EVERYWHERE    256
#define PROP_WEAPON        512
#define PROP_ACTOR        1024
#define PROP_TOOL         2048
#define PROP_INFLAMMABLE  4096
#define PROP_SURFACE      8192

/*room properties bit flags*/
#define R_DESCRIBED     1  /* set when room description already printed */
#define R_BODYOFWATER   2
#define R_LIT           4  /* set when there is natural light or a light fixture */
#define R_WATERHERE     8
#define R_SACRED       16  /* set when thief not allowed in room */
#define R_MAZE         32
#define R_ALWAYSDESC   64

enum
{
  VILLAIN_TROLL,
  VILLAIN_THIEF,
  VILLAIN_CYCLOPS,

  NUM_VILLAINS
};

/* extern data structures ---------------------------------------------------------- */
struct OBJ_STRUCT
{
  unsigned short init_loc;
  unsigned short loc;
  unsigned short size;
  unsigned short capacity;
  unsigned short order;
  unsigned short prop;
  unsigned char init_thiefvalue;
  unsigned char thiefvalue;
};
extern struct OBJ_STRUCT Obj[];

struct ROOM_STRUCT
{
  char *scenery;
  unsigned short prop;
  unsigned short init_prop;
};
extern struct ROOM_STRUCT Room[];

extern unsigned char YouAreDead;
extern unsigned char GameOver; /*flag, but with special value 2: restart was requested*/
extern unsigned char NotLucky;
extern int NumDeaths;
extern unsigned char YouAreInBoat;
extern unsigned char ExitFound; /* set this when player finds an exit from dungeon other than the trapdoor*/
extern unsigned char TrollAllowsPassage;
extern int ThiefDescType; /* 0: default  1: unconscious*/

extern unsigned char VillainAttacking[];

/* extern functions ---------------------------------------------------------------- */
void PrintCompLine(char *);
void PrintCompText(char*);
void PrintPlayerRoomDesc(int force_description);
short xrnd(void);

/* variables ----------------------------------------------------------------------- */
int above_ground[11] =
        { R_WEST_OF_HOUSE, R_NORTH_OF_HOUSE, R_EAST_OF_HOUSE,
          R_SOUTH_OF_HOUSE, R_FOREST_1, R_FOREST_2, R_FOREST_3,
          R_PATH, R_CLEARING, R_GRATING_CLEARING, R_CANYON_VIEW };

/* code ---------------------------------------------------------------------------- */
/* call just before player enters treasure room*/

void ThiefProtectsTreasure(void)
{
  int obj, flag = 0;

  /* if thief is dead or unconcious*/
  if (Obj[O_THIEF].loc == 0 ||
      ThiefDescType == 1) /* unconcious*/
    return;

  if (Obj[O_THIEF].loc != R_TREASURE_ROOM)
  {
    PrintCompLine("\x8b\xa0\xbb\xa3\xaa\x63\xa9\x61\xf9\xdd\xa3\xb1\x75\xb2\xde"
"\xe0\x86\x20\x76\x69\x6f\xfd\xd1\x80\xda\x6f\x62\xef\x72\x27\xa1\xce\xe8\x61"
"\x77\x61\x79\xa4\x55\x73\x84\x70\xe0\x73\x61\x67\xbe\x20\xf6\x6b\xe3\x77\xb4"
"\xbd\x86\xb5\x94\x72\xfe\xa0\xa1\xbd\xa8\x74\xa1\xe8\x66\xd4\x73\x65\x2e");
  
    Obj[O_THIEF].loc = R_TREASURE_ROOM;
    Obj[O_THIEF].prop &= ~PROP_NODESC;
  
    VillainAttacking[VILLAIN_THIEF] = 1;
  
    for (obj=2; obj<NUM_OBJECTS; obj++)
      if (Obj[obj].loc == R_TREASURE_ROOM &&
          obj != O_CHALICE &&
          obj != O_THIEF)
    {
      if (flag == 0)
      {
        flag = 1;
        PrintCompLine("\x85\xa2\x69\x65\xd2\x67\xbe\x74\xd8\xbe\xee\x79\xc5\xac"
"\x69\xa5\x73\xec\xb5\x8c\x81\x74\xa9\xe0\xd8\xbe\xa8\xb4\x81\xc2\xe1\xaa\x75"
"\x64\xe8\x6e\xec\x20\x76\xad\xb2\x68\x2e");
      }
  
      Obj[obj].prop |= PROP_NODESC;
      Obj[obj].prop |= PROP_NOTTAKEABLE;
    }

    PrintCompText("\x0a");
  }
}

int GetRandom(int Range)
{
  long tmp;

  tmp = xrnd() * Range;

  return ((int)(tmp >> 16));
}

void ScatterInventory(void)
{
  int obj;

  if (Obj[O_LAMP].loc == INSIDE + O_YOU)
    Obj[O_LAMP].loc = R_LIVING_ROOM;

  if (Obj[O_COFFIN].loc == INSIDE + O_YOU)
    Obj[O_COFFIN].loc = R_EGYPT_ROOM;

  Obj[O_SWORD].thiefvalue = 0;


  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == INSIDE + O_YOU)
  {
    int room = NUM_ROOMS;

    if (Obj[obj].thiefvalue > 0)
      for (room=1; room<NUM_ROOMS; room++)
        if ((Room[room].prop & R_BODYOFWATER) == 0 &&
            (Room[room].prop & R_LIT) == 0 &&
            GetRandom(2) == 0) break;

    if (room == NUM_ROOMS)
    {
      room = above_ground[GetRandom(11)];
    }

    Obj[obj].loc = room;
  }
}

void YoureDead(void)
{
  if (YouAreDead)
  {
    PrintCompLine("\x0a\x49\xa6\x74\x61\x6b\xbe\xa3\x9f\xe2\xd4\xd1\xab\xfc\x72\x73\xca\x89\xef\x20\x6b"
"\x69\xdf\xd5\xb7\xce\xcf\xa3\x6c\xa9\x61\x64\xc4\xe8\x61\x64\xa4\x59\x4f\x55\xa3\xa9\xaa\x75\xfa\xa3\x9f"
"\xe2\xd4\x74\xa4\x55\x6e\x66\xd3\x74\xf6\xaf\x65\xec\xb5\xc7\x9f\x61\x6b\xbe\xa3\x9f\xe2\xd4\xd1\xab\xfc"
"\x72\x73\xca\x89\xe8\xe2\xb7\xc7\xde\xc7\xa4\x49\xa3\xf9\xe3\xa6\x73\x75\xfa\xa3\x9f\xe2\xd4\x74\xa4\x53\xd3\x72\x79\x2e");
    GameOver = 1;
    return;
  }

  if (NotLucky)
    PrintCompLine("\x42\x61\xab\x6c\x75\x63\x6b\xb5\x68\x75\x68\x3f");

  PrintCompLine("\x0a\x20\x20\x20\x20\x2a\x2a\x2a\x2a\x20\x88\xc0\x61\xd7\xcc\x69\xd5\x20\x20\x2a\x2a\x2a\x2a\x0a\x0a");

  NumDeaths++;
  if (NumDeaths == 3)
  {
    PrintCompLine("\x8b\x63\xcf\xbb\xec\xa3\xa9\xa3\xaa\x75\x69\x63\x69\x64\xe2\xee\xad\x69\x61\x63\xa4"
"\x20\x57\x9e\x64\xca\x27\xa6\xe2\xd9\x77\xeb\x73\x79\xfa\xff\x69\x63\xa1\xa7\x80\xb3\x61\xd7\xb5\x73"
"\xa7\x63\x9e\x96\xc4\x6d\x61\xc4\xcd\x72\xf9\xff\xa0\xb6\x61\x64\xd7\xe5\xd8\xac\x73\xa4\x88\xb6\xa9"
"\x6d\x61\xa7\xa1\xf8\xdf\xb0\x9e\xa7\xc5\xe2\xcf\xab\xa7\x80\x20\x4c\x8c\xdd\x80\x20\x4c\x69\x76\x84"
"\x44\xbf\x64\xb5\x77\xa0\xa9\x86\xb6\x66\x65\xdf\xf2\xa3\x64\xd7\xe5\xd8\xac\xa1\x6d\x61\xc4\x67\xd9\xaf\xae\xd7\xb6\x96\x6d\x2e");
    GameOver = 1;
    return;
  }

  YouAreInBoat = 0; /* in case you're in it*/
  ExitFound = 1;
  ScatterInventory();

  if (Room[R_SOUTH_TEMPLE].prop & R_DESCRIBED)
  {
    PrintCompLine("\x41\xa1\x8f\x74\x61\x6b\x9e\x92\xcb\xe0\xa6\x62\xa9\xaf\x68\xb5\x8f\x66\xf3\xea\xa9"
"\xf5\x65\xd7\xab\xdd\x86\xb6\x62\xd8\xe8\x6e\x73\x83\x9e\x66\xf3\xf5\x9c\x70\xe0\xd6\xa1\xe0\x86\xc6"
"\xa7\xab\x92\xd6\x6c\xd2\xef\x66\xd3\x9e\x81\x67\xaf\xbe\x8a\x48\x65\xdf\xb5\x77\xa0\xa9\x80\xaa\x70"
"\x69\xf1\x74\xa1\x6a\xf3\xb6\xaf\x86\x8d\xcc\xd4\xc4\x8f\xd4\x74\x72\x79\xa4\x88\xb6\xd6\x6e\xd6\xa1"
"\xbb\x9e\x64\xb2\x74\xd8\xef\x64\xa4\x82\xae\x62\x6a\x65\x63\x74\xa1\xa7\x80\xcc\xf6\x67\x65\xca\xa3"
"\x70\xfc\xbb\xa8\xb9\xb2\xf0\x6e\x63\x74\xb5\x62\xcf\x61\xfa\xd5\x8a\x63\x6f\xd9\x72\xb5\x65\xd7\xb4\xf6\xa9\xe2\x2e\x0a");
    YouAreDead = 1;
    TrollAllowsPassage = 1;
    Obj[O_LAMP].prop |= PROP_NODESC;
    Obj[O_LAMP].prop |= PROP_NOTTAKEABLE;
    Obj[O_YOU].prop |= PROP_LIT;
    Obj[O_YOU].loc = R_ENTRANCE_TO_HADES;
    PrintPlayerRoomDesc(0);
  }
  else
  {
    PrintCompLine("\x4e\xf2\xb5\xcf\x74\x27\xa1\x74\x61\x6b\x9e\xd0\xd9\x6f\x6b\xc0\xac\x65\x2e\x2e\xa4"
"\x57\x65\xdf\xb5\x8f\x70\xc2\x62\x61\x62\xec\xcc\xbe\xac\xd7\xa3\xe3\x96\xb6\xfa\xad\x63\x65\xa4\x20"
"\x49\x91\x27\xa6\x71\x75\xc7\x9e\x66\x69\x78\x86\x20\x75\x70\xb3\xe1\x70\xcf\xd1\xec\xb5\x62\xf7\x86"
"\x91\x27\xa6\xcd\xd7\xfb\xd7\x72\x79\xa2\x97\x2e\x0a");
    Obj[O_YOU].loc = R_FOREST_1;
    PrintPlayerRoomDesc(0);
  }
}
