/* Zork I: The Great Underground Empire*/
/* (c) 1980 by INFOCOM, Inc.*/
/* C port and parser (c) 2021 by Donnie Russell II*/

/* This source code is provided for personal, educational use only.*/
/* You are welcome to use this source code to develop your own works,*/
/* but the story-related content belongs to the original authors of Zork.*/

#include <stdio.h>
#include <string.h>

#include <ovr36.h>
#include <ovr37.h>
#include <ovr40.h>

/* defines, enums ------------------------------------------------------------- */
#define AllocBuf	((void*)0xBB00)

#define OVR_PARSE			36
#define OVR_EVENT_ROUTINES 		37
#define OVR_OVERRIDE_OBJECT_DESC	38
#define OVR_OVERRIDE_ROOM_DESC 		39
#define OVR_SAVE_RESTORE		40

#define SAVE_FILENAME "zork.sav"
#define SAVE_FILENAME_NUMCHAR_INDEX  6

#define SCORE_MAX  350
#define MAX_PRINTED_LINES  23

enum
{
  V_BRIEF,
  V_VERBOSE,
  V_SUPERBRIEF
};

enum
{
  VILLAIN_TROLL,
  VILLAIN_THIEF,
  VILLAIN_CYCLOPS,

  NUM_VILLAINS
};

enum
{
  O_NOTHING,		/* 0  */
  O_YOU,		/* 1  */
  O_CYCLOPS,		/* 2  */
  O_GHOSTS,		/* 3  */
  O_BAT,		/* 4  */
  O_THIEF,		/* 5  */
  O_TROLL,		/* 6  */
  O_LOWERED_BASKET,	/* 7  */
  O_RAISED_BASKET,	/* 8  */
  O_TROPHY_CASE,	/* 9  */
  O_MACHINE,		/* 10  */
  O_MAILBOX,		/* 11  */
  O_KITCHEN_TABLE,	/* 12  */
  O_ATTIC_TABLE,	/* 13  */
  O_WATER,		/* 14  */
  O_SKULL,		/* 15  */
  O_TIMBERS,		/* 16  */
  O_LUNCH,		/* 17  */
  O_BELL,		/* 18  */
  O_HOT_BELL,		/* 19  */
  O_BOOK,		/* 20 */
  O_AXE,		/* 21  */
  O_BROKEN_LAMP,	/* 22  */
  O_SCEPTRE,		/* 23  */
  O_SANDWICH_BAG,	/* 24  */
  O_CHALICE,		/* 25  */
  O_GARLIC,		/* 26  */
  O_TRIDENT,		/* 27  */
  O_BOTTLE,		/* 28  */
  O_COFFIN,		/* 29  */
  O_PUMP,		/* 30  */
  O_DIAMOND,		/* 31  */
  O_JADE,		/* 32  */
  O_KNIFE,		/* 33  */
  O_BURNED_OUT_LANTERN,	/* 34  */
  O_BAG_OF_COINS,	/* 35  */
  O_LAMP,		/* 36  */
  O_EMERALD,		/* 37  */
  O_ADVERTISEMENT,	/* 38  */
  O_INFLATED_BOAT,	/* 39  */
  O_MATCH,		/* 40  */
  O_PAINTING,		/* 41  */
  O_CANDLES,		/* 42  */
  O_GUNK,		/* 43  */
  O_LEAVES,		/* 44  */
  O_PUNCTURED_BOAT,	/* 45  */
  O_INFLATABLE_BOAT,	/* 46  */
  O_BAR,		/* 47  */
  O_POT_OF_GOLD,	/* 48  */
  O_BUOY,		/* 49  */
  O_ROPE,		/* 50  */
  O_RUSTY_KNIFE,	/* 51  */
  O_BRACELET,		/* 52  */
  O_TOOL_CHEST,		/* 53  */
  O_SCREWDRIVER,	/* 54  */
  O_KEYS,		/* 55  */
  O_SHOVEL,		/* 56  */
  O_COAL,		/* 57  */
  O_SCARAB,		/* 58  */
  O_LARGE_BAG,		/* 59  */
  O_STILETTO,		/* 60  */
  O_SWORD,		/* 61  */
  O_MAP,		/* 62  */
  O_BOAT_LABEL,		/* 63  */
  O_TORCH,		/* 64  */
  O_GUIDE,		/* 65  */
  O_TRUNK,		/* 66  */
  O_TUBE,		/* 67  */
  O_PUTTY,		/* 68  */
  O_OWNERS_MANUAL,	/* 69  */
  O_WRENCH,		/* 70  */
  O_NEST,		/* 71  */
  O_EGG,		/* 72  */
  O_BROKEN_EGG,		/* 73  */
  O_BAUBLE,		/* 74  */
  O_CANARY,		/* 75  */
  O_BROKEN_CANARY,	/* 76  */
  O_ENGRAVINGS,		/* 77  */
  O_ZORKMID,		/* 78  */
  O_GRUE		/* 79  */
};

/*room properties bit flags*/
#define R_DESCRIBED     1  /* set when room description already printed */
#define R_BODYOFWATER   2
#define R_LIT           4  /* set when there is natural light or a light fixture */
#define R_WATERHERE     8
#define R_SACRED       16  /* set when thief not allowed in room */
#define R_MAZE         32
#define R_ALWAYSDESC   64

#define INSIDE  2048

#define NUM_ROOMS  111 /* including null room 0 */
#define NUM_OBJECTS  80

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

#define NUM_TREASURESCORES  19
#define NUM_ROOMSCORES      5

/* extern data structures ---------------------------------------------------------- */

/* tables.c */
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
  unsigned short      prop;
   unsigned short init_prop;
};
extern struct ROOM_STRUCT Room[];

struct TREASURESCORE_STRUCT
{
   int obj;
   unsigned char take_value;
   unsigned char case_value;
  unsigned char flags; /* &1 taken  &2 put in case */
};
extern struct TREASURESCORE_STRUCT TreasureScore[];

struct ROOMSCORE_STRUCT
{
   int room;
   unsigned char value;
  unsigned char flag;
};
extern struct ROOMSCORE_STRUCT RoomScore[];

struct OVERRIDEROOMDESC_STRUCT
{
   int room;
  void (*f)(void);
};
extern struct OVERRIDEROOMDESC_STRUCT OverrideRoomDesc[];

struct OVERRIDEOBJECTDESC_STRUCT
{
  int obj;
  void (*f)(int);
};
struct OVERRIDEOBJECTDESC_STRUCT OverrideObjectDesc[];

extern char *RoomDesc[];
extern char *ObjectDesc[];

/* extern FILE *InputStream; */
extern int Verbosity;
extern int NumPrintedLines;
extern int CursorColumn;
extern int CurWord;
extern int NumStrWords;
extern int ItObj, PrevItObj; /*the obj "it" refers to*/
extern char TimePassed; /*flag: time passed during action*/
extern char GameOver; /*flag, but with special value 2: restart was requested*/

extern char *StrWord[];

extern unsigned char RugMoved;
extern unsigned char TrapOpen;
extern unsigned char ExitFound; /* set this when player finds an exit from dungeon other than the trapdoor*/
extern unsigned char KitchenWindowOpen;
extern unsigned char GratingRevealed;
extern unsigned char GratingUnlocked;
extern unsigned char GratingOpen;
extern unsigned char GatesOpen;
extern unsigned char LowTide;
extern unsigned char GatesButton;
extern unsigned char LoudRoomQuiet;
extern unsigned char RainbowSolid;
extern unsigned char WonGame;
extern unsigned char MirrorBroken; /* set NotLucky too*/
extern unsigned char RopeTiedToRail;
extern unsigned char SpiritsBanished;
extern unsigned char TrollAllowsPassage;
extern unsigned char YouAreSanta;
extern unsigned char YouAreInBoat;
extern unsigned char NotLucky;
extern unsigned char YouAreDead;
extern unsigned char SongbirdSang;
extern unsigned char ThiefHere;
extern unsigned char ThiefEngrossed;
extern unsigned char YouAreStaggered;
extern unsigned char BuoyFlag;

extern int NumMoves;
extern int LampTurnsLeft;
extern int MatchTurnsLeft;
extern int CandleTurnsLeft;
extern int MatchesLeft;
extern int ReservoirFillCountdown;
extern int ReservoirDrainCountdown;
extern int MaintenanceWaterLevel;
extern int DownstreamCounter;
extern int BellRungCountdown; /* these three are for ceremony*/
extern int CandlesLitCountdown;
extern int BellHotCountdown;
extern int CaveHoleDepth;
extern int Score;
extern int NumDeaths;
extern int CyclopsCounter;
extern int CyclopsState; /* 0: default  1: hungry  2: thirsty  3: asleep  4: fled*/
extern int LoadAllowed;
extern int PlayerStrength;
extern int TrollDescType;
extern int ThiefDescType; /* 0: default  1: unconscious*/
extern int EnableCureRoutine; /* countdown*/

extern unsigned char VillainAttacking[NUM_VILLAINS];
extern unsigned char VillainStaggered[NUM_VILLAINS];
extern int VillainWakingChance[NUM_VILLAINS];
extern int VillainStrength[NUM_VILLAINS];

/* extern functions ---------------------------------------------------------------- */
void GetWords(char *);
void PrintPlayerRoomDesc(int);
void PrintNewLine(void);
void PrintCompLine(char *);
void PrintScore(void);
int MatchCurWord(char *);
int IsObjVisible(int obj);

/* rand.as */
void xrndseed(void);
int xrnd(void);

void InitOverlays(void);
short CallOverlay0(void* Routine, char CalledRamBank);

void HexLoad(void);

/* variables ----------------------------------------------------------------------- */

/* overlays ----------------------------------------------------------------------- */

/* -------------
void Parse(void);
void RunEventRoutines(void);
int DoRestore(void);

#define Parse 0
#define RunEventRoutines 0
#define DoRestore 0
------------ */
/* code ---------------------------------------------------------------------------- */
void InitGameState(void)
{
  int i;

  RugMoved           = 0;
  TrapOpen           = 0;
  ExitFound          = 0;
  KitchenWindowOpen  = 0;
  GratingRevealed    = 0;
  GratingUnlocked    = 0;
  GratingOpen        = 0;
  GatesOpen          = 0;
  LowTide            = 0;
  GatesButton        = 0;
  LoudRoomQuiet      = 0;
  RainbowSolid       = 0;
  WonGame            = 0;
  MirrorBroken       = 0;
  RopeTiedToRail     = 0;
  SpiritsBanished    = 0;
  TrollAllowsPassage = 0;
  YouAreSanta        = 0;
  YouAreInBoat       = 0;
  NotLucky           = 0;
  YouAreDead         = 0;
  SongbirdSang       = 0;
  ThiefHere          = 0;
  ThiefEngrossed     = 0;
  YouAreStaggered    = 0;
  BuoyFlag           = 0;

  NumMoves                = 0;
  LampTurnsLeft           = 200;
  MatchTurnsLeft          = 0;
  CandleTurnsLeft         = 40;
  MatchesLeft             = 6;
  ReservoirFillCountdown  = 0;
  ReservoirDrainCountdown = 0;
  MaintenanceWaterLevel   = 0;
  DownstreamCounter       = 0;
  BellRungCountdown       = 0;
  CandlesLitCountdown     = 0;
  BellHotCountdown        = 0;
  CaveHoleDepth           = 0;
  Score                   = 0;
  NumDeaths               = 0;
  CyclopsCounter          = 0;
  CyclopsState            = 0;
  LoadAllowed             = 100;
  PlayerStrength          = 0;
  TrollDescType           = 0;
  ThiefDescType           = 0;
  EnableCureRoutine       = 0;

  for (i=0; i<NUM_VILLAINS; i++)
  {
    VillainAttacking[i]    = 0;
    VillainStaggered[i]    = 0;
    VillainWakingChance[i] = 0;
  }

  VillainStrength[VILLAIN_TROLL  ] = 2;
  VillainStrength[VILLAIN_THIEF  ] = 5;
  VillainStrength[VILLAIN_CYCLOPS] = 10000;

  for (i=0; i<NUM_TREASURESCORES; i++)
    TreasureScore[i].flags = 0;

  for (i=0; i<NUM_ROOMSCORES; i++)
    RoomScore[i].flag = 0;

  for (i=0; i<NUM_ROOMS; i++)
    Room[i].prop = Room[i].init_prop;

  for (i=0; i<NUM_OBJECTS; i++)
  {
    Obj[i].loc = Obj[i].init_loc;
    Obj[i].order = i;
    Obj[i].prop = 0;
    Obj[i].thiefvalue = Obj[i].init_thiefvalue;
  }

  Obj[O_CYCLOPS        ].prop |= PROP_NOTTAKEABLE;
  Obj[O_GHOSTS         ].prop |= PROP_NOTTAKEABLE;
  Obj[O_BAT            ].prop |= PROP_NOTTAKEABLE;
  Obj[O_THIEF          ].prop |= PROP_NOTTAKEABLE;
  Obj[O_TROLL          ].prop |= PROP_NOTTAKEABLE;
  Obj[O_LOWERED_BASKET ].prop |= PROP_NOTTAKEABLE;
  Obj[O_RAISED_BASKET  ].prop |= PROP_NOTTAKEABLE;
  Obj[O_TROPHY_CASE    ].prop |= PROP_NOTTAKEABLE;
  Obj[O_MACHINE        ].prop |= PROP_NOTTAKEABLE;
  Obj[O_MAILBOX        ].prop |= PROP_NOTTAKEABLE;
  Obj[O_KITCHEN_TABLE  ].prop |= PROP_NOTTAKEABLE;
  Obj[O_ATTIC_TABLE    ].prop |= PROP_NOTTAKEABLE;
  Obj[O_TRUNK          ].prop |= PROP_NOTTAKEABLE;
  Obj[O_HOT_BELL       ].prop |= PROP_NOTTAKEABLE;
  Obj[O_POT_OF_GOLD    ].prop |= PROP_NOTTAKEABLE;
  Obj[O_SCARAB         ].prop |= PROP_NOTTAKEABLE;
  Obj[O_MAP            ].prop |= PROP_NOTTAKEABLE;
  Obj[O_TOOL_CHEST     ].prop |= PROP_NOTTAKEABLE;
  Obj[O_ENGRAVINGS     ].prop |= PROP_NOTTAKEABLE;
  Obj[O_WATER          ].prop |= PROP_NOTTAKEABLE;
  Obj[O_STILETTO       ].prop |= PROP_NOTTAKEABLE;
  Obj[O_LARGE_BAG      ].prop |= PROP_NOTTAKEABLE;
  Obj[O_AXE            ].prop |= PROP_NOTTAKEABLE;
  Obj[O_ZORKMID        ].prop |= PROP_NOTTAKEABLE;
  Obj[O_GRUE           ].prop |= PROP_NOTTAKEABLE;

  Obj[O_THIEF          ].prop |= PROP_NODESC;
  Obj[O_TROPHY_CASE    ].prop |= PROP_NODESC;
  Obj[O_MACHINE        ].prop |= PROP_NODESC;
  Obj[O_KITCHEN_TABLE  ].prop |= PROP_NODESC;
  Obj[O_ATTIC_TABLE    ].prop |= PROP_NODESC;
  Obj[O_TRUNK          ].prop |= PROP_NODESC;
  Obj[O_POT_OF_GOLD    ].prop |= PROP_NODESC;
  Obj[O_SCARAB         ].prop |= PROP_NODESC;
  Obj[O_MAP            ].prop |= PROP_NODESC;
  Obj[O_STILETTO       ].prop |= PROP_NODESC;
  Obj[O_LARGE_BAG      ].prop |= PROP_NODESC;
  Obj[O_AXE            ].prop |= PROP_NODESC;
  Obj[O_ZORKMID        ].prop |= PROP_NODESC;
  Obj[O_GRUE           ].prop |= PROP_NODESC;

  Obj[O_TROPHY_CASE    ].prop |= PROP_OPENABLE;
  Obj[O_MACHINE        ].prop |= PROP_OPENABLE;
  Obj[O_MAILBOX        ].prop |= PROP_OPENABLE;
  Obj[O_SANDWICH_BAG   ].prop |= PROP_OPENABLE;
  Obj[O_BOTTLE         ].prop |= PROP_OPENABLE;
  Obj[O_COFFIN         ].prop |= PROP_OPENABLE;
  Obj[O_BUOY           ].prop |= PROP_OPENABLE;
  Obj[O_LARGE_BAG      ].prop |= PROP_OPENABLE;
  Obj[O_TUBE           ].prop |= PROP_OPENABLE;

  Obj[O_KITCHEN_TABLE  ].prop |= PROP_OPEN;
  Obj[O_ATTIC_TABLE    ].prop |= PROP_OPEN;
  Obj[O_RAISED_BASKET  ].prop |= PROP_OPEN;
  Obj[O_LOWERED_BASKET ].prop |= PROP_OPEN;
  Obj[O_INFLATED_BOAT  ].prop |= PROP_OPEN;
  Obj[O_NEST           ].prop |= PROP_OPEN;
  Obj[O_LARGE_BAG      ].prop |= PROP_OPEN;
  Obj[O_CHALICE        ].prop |= PROP_OPEN;
  Obj[O_THIEF          ].prop |= PROP_OPEN;
  Obj[O_TROLL          ].prop |= PROP_OPEN;
  Obj[O_WATER          ].prop |= PROP_OPEN;

  Obj[O_TORCH          ].prop |= PROP_LIT;
  Obj[O_CANDLES        ].prop |= PROP_LIT;

  Obj[O_SCEPTRE        ].prop |= PROP_INSIDEDESC;
  Obj[O_MAP            ].prop |= PROP_INSIDEDESC;
  Obj[O_EGG            ].prop |= PROP_INSIDEDESC;
  Obj[O_CANARY         ].prop |= PROP_INSIDEDESC;
  Obj[O_BROKEN_CANARY  ].prop |= PROP_INSIDEDESC;
  Obj[O_SANDWICH_BAG   ].prop |= PROP_INSIDEDESC;
  Obj[O_BOTTLE         ].prop |= PROP_INSIDEDESC;
  Obj[O_KNIFE          ].prop |= PROP_INSIDEDESC;

  Obj[O_ROPE           ].prop |= PROP_SACRED;
  Obj[O_COFFIN         ].prop |= PROP_SACRED;
  Obj[O_BAR            ].prop |= PROP_SACRED;

  Obj[O_WATER          ].prop |= PROP_EVERYWHERE;
  Obj[O_ZORKMID        ].prop |= PROP_EVERYWHERE;
  Obj[O_GRUE           ].prop |= PROP_EVERYWHERE;

  Obj[O_AXE            ].prop |= PROP_WEAPON;
  Obj[O_STILETTO       ].prop |= PROP_WEAPON;
  Obj[O_RUSTY_KNIFE    ].prop |= PROP_WEAPON;
  Obj[O_SWORD          ].prop |= PROP_WEAPON;
  Obj[O_KNIFE          ].prop |= PROP_WEAPON;
  Obj[O_SCEPTRE        ].prop |= PROP_WEAPON;

  Obj[O_CYCLOPS        ].prop |= PROP_ACTOR;
  Obj[O_GHOSTS         ].prop |= PROP_ACTOR;
  Obj[O_BAT            ].prop |= PROP_ACTOR;
  Obj[O_THIEF          ].prop |= PROP_ACTOR;
  Obj[O_TROLL          ].prop |= PROP_ACTOR;

  Obj[O_PUMP           ].prop |= PROP_TOOL;
  Obj[O_SCREWDRIVER    ].prop |= PROP_TOOL;
  Obj[O_KEYS           ].prop |= PROP_TOOL;
  Obj[O_SHOVEL         ].prop |= PROP_TOOL;
  Obj[O_PUTTY          ].prop |= PROP_TOOL;
  Obj[O_WRENCH         ].prop |= PROP_TOOL;

  Obj[O_LEAVES         ].prop |= PROP_INFLAMMABLE;
  Obj[O_BOOK           ].prop |= PROP_INFLAMMABLE;
  Obj[O_SANDWICH_BAG   ].prop |= PROP_INFLAMMABLE;
  Obj[O_ADVERTISEMENT  ].prop |= PROP_INFLAMMABLE;
  Obj[O_INFLATED_BOAT  ].prop |= PROP_INFLAMMABLE;
  Obj[O_PAINTING       ].prop |= PROP_INFLAMMABLE;
  Obj[O_PUNCTURED_BOAT ].prop |= PROP_INFLAMMABLE;
  Obj[O_INFLATABLE_BOAT].prop |= PROP_INFLAMMABLE;
  Obj[O_COAL           ].prop |= PROP_INFLAMMABLE;
  Obj[O_BOAT_LABEL     ].prop |= PROP_INFLAMMABLE;
  Obj[O_GUIDE          ].prop |= PROP_INFLAMMABLE;
  Obj[O_NEST           ].prop |= PROP_INFLAMMABLE;

  Obj[O_KITCHEN_TABLE  ].prop |= PROP_SURFACE;
  Obj[O_ATTIC_TABLE    ].prop |= PROP_SURFACE;

  ItObj = O_MAILBOX;
}

void DoIntro(void)
{
  PrintCompLine("\x57\x65\x6c\x63\xe1\x9e\xbd\x20\x5a\xd3\x6b\x20\x49\x3a\x82\x20\x47\xa9\xaf"
"\x20\x55\xb9\xac\x67\xc2\xf6\xab\x45\x6d\x70\x69\xa9\x21\x0a\x28\x63\x29\x20\x31\x39\x38\x30"
"\xb0\xc4\x49\x4e\x46\x4f\x43\x4f\x4d\xb5\x49\x6e\x63\x2e\x0a\x20\x20\x43\xeb\xd3\xa6\x8c\x70"
"\xbb\xd6\xb6\x28\x63\x29\x20\x32\x30\x32\x31\xb0\xc4\x44\xca\x6e\x69\x9e\x52\xfe\xd6\xdf\x20\x49\x49\x0a\x0a");
}

void RestartGame(void)
{
  int i;

  InitGameState(); /* sets ItObj*/

  NumStrWords = 0;
  GameOver = 0;

  /* clear screen*/
  i = MAX_PRINTED_LINES;
  while (i-- > 0) {PrintNewLine(); NumPrintedLines = 0;}

  DoIntro();

  PrintPlayerRoomDesc(0);
}

void PrintInteger(int num)
{
  int neg = 0, size = 0, i;
  char* buf = AllocBuf; /* char buf[80]; */

  if (num == 0)
  {
    putchar('0');
    CursorColumn++;
    return;
  }

  if (num < 0)
  {
    num = -num;
    neg = 1;
  }

  while (num)
  {
    buf[size++] = '0' + (num % 10);
    num /= 10;
  }

  if (neg)
  {
    putchar('-');
    CursorColumn++;
  }

  for (i=size-1; i>=0; i--)
  {
    putchar(buf[i]);
    CursorColumn++;
  }
}

int GetScore(void)
{
  return Score;
}

int GetMaxScore(void)
{
  return SCORE_MAX;
}

char *GetRankName(void)
{
       if (Score == 350) return "Master Adventurer";
  else if (Score >  330) return "Wizard";
  else if (Score >  300) return "Master";
  else if (Score >  200) return "Adventurer";
  else if (Score >  100) return "Junior Adventurer";
  else if (Score >   50) return "Novice Adventurer";
  else if (Score >   25) return "Amateur Adventurer";
  else                   return "Beginner";
}

void PrintScore(void)
{
  PrintText(GameOver ? "\nYou scored " : "Your score is ");
  PrintInteger(GetScore());
  PrintCompText("\xeb\x6f\xa7\x74\xa1\xa5\xa6\xdd\xa3\xee\x61\x78\x69\x6d\x75\xf9\x6f\x66\x20");
  PrintInteger(GetMaxScore());
  PrintCompText("\xb5\xfe\x84");
  PrintInteger(NumMoves);
  PrintText((NumMoves == 1) ? " move.\n" : " moves.\n");

  if (GameOver)
  {
    PrintCompText("\x0a\xbc\xaf\xe6\x69\xd7\xa1\x8f\xd0\xf4\x6e\x6b\x8a");
    PrintText(GetRankName());
    PrintCompText("\x2e\x0a");
  }
}

/* returns 1 if obj is not visible or not specific enough*/
/* assumes player is not an actor*/

int IsObjCarriedByActor(int obj)
{
  int container = Obj[obj].loc - INSIDE; /* get obj's container, if any*/

  if (container > 1 && container < NUM_OBJECTS)
    if (Obj[container].prop & PROP_ACTOR)
      return 1;

  return 0;
}

/* a lighted object carried by an actor will light the room if the actor is "open";*/
/* therefore exclude such objects*/
int IsPlayerInDarkness(void)
{
  int i, obj;

  if (Room[Obj[O_YOU].loc].prop & R_LIT) return 0; /* room is lit*/

  if (Obj[O_YOU].prop & PROP_LIT) return 0; /* you are lit*/

  for (i=2; i<NUM_OBJECTS; i++)
  {
    obj = Obj[i].order;

    if (IsObjVisible(obj) &&              
        IsObjCarriedByActor(obj) == 0 &&  
        (Obj[obj].prop & PROP_LIT))       
      return 0; /* obj is visible, not carried by actor, and lit*/
  }

  return 1;
}

void PrintObjectDesc(int obj, int desc_flag)
{
  int i;
  char *compressed_text, *decompressed_text, *p, *q;
  int compressed_size;

  /*look for object in override obj desc list*/
  for (i=0; OverrideObjectDesc[i].f != 0; i++)
    if (OverrideObjectDesc[i].obj == obj) break;
  if (OverrideObjectDesc[i].f != 0)
    CallOverlay1((void*)OverrideObjectDesc[i].f, desc_flag, OVR_OVERRIDE_OBJECT_DESC); 
    /* (*(OverrideObjectDesc[i].f))(desc_flag); */
  else
  {
    compressed_text = ObjectDesc[obj];
    compressed_size = strlen(compressed_text)+1;
    decompressed_text = p = AllocBuf; /* malloc(GetDecompressTextSize(compressed_text, compressed_size)); */
    DecompressText(compressed_text, compressed_size, decompressed_text);
  
    if (desc_flag)
    {
      p = strchr(p, '^');
      q = NULL;
      if (p != NULL)
      {
        p++;
        q = strchr(p, '^');
        if (q != NULL)
          q++;
      }
  
      if (Obj[obj].prop & PROP_MOVEDDESC)
        p = q;
    }
  
    if (p != NULL && *p != '^' && *p != 0)
      PrintText(p);
  
    /* free(decompressed_text); */
  }
}

void PrintRoomDesc(int room, int force_description)
{
  char *compressed_text, *decompressed_text, *p;
  int compressed_size, i;

  compressed_text = RoomDesc[room];
  compressed_size = strlen(compressed_text)+1;
  decompressed_text = p = AllocBuf; /* malloc(GetDecompressTextSize(compressed_text, compressed_size)); */
  DecompressText(compressed_text, compressed_size, decompressed_text);

  if (*p != '^' && *p != 0)
  {
    PrintLine(p); /* print room name*/
  }

  if (force_description || Verbosity != V_SUPERBRIEF)
  {
    if ((Room[room].prop & R_ALWAYSDESC) || force_description || Verbosity == V_VERBOSE)
      Room[room].prop &= ~R_DESCRIBED;

    /*look for room in override room desc list*/
    for (i=0; OverrideRoomDesc[i].f != 0; i++)
      if (OverrideRoomDesc[i].room == room) break;
    if (OverrideRoomDesc[i].f != 0)
      CallOverlay0((void*)OverrideRoomDesc[i].f, OVR_OVERRIDE_ROOM_DESC); 
      /*  (*(OverrideRoomDesc[i].f))(); */
    else
      if ((Room[room].prop & R_DESCRIBED) == 0)
    {
      p = strchr(p, '^');
      if (p != NULL)
      {
        p++;
        if (*p != '^' && *p != 0) PrintLine(p);
      }
    }
  }

  /*game logic depends on this being set 
	even if player never actually sees full description*/
  Room[room].prop |= R_DESCRIBED;

  /* free(decompressed_text); */
}

/* only display objects inside something else one level deep*/

void PrintContents(int obj, char *heading, int print_empty)
{
  int flag = 0, i;

  for (i=2; i<NUM_OBJECTS; i++)
  {
    int obj_inside = Obj[i].order;

    if ( Obj[obj_inside].loc == INSIDE + obj &&
         (Obj[obj_inside].prop & PROP_NODESC) == 0 &&
         !( (Obj[obj_inside].prop & PROP_INSIDEDESC) &&
            (Obj[obj_inside].prop & PROP_MOVEDDESC) == 0 ))
    {
      if (flag == 0) {PrintLine(heading); flag = 1;}
      PrintCompText("\x20\x20");
      PrintObjectDesc(obj_inside, 0);
      PrintCompText("\x0a");
    }
  }

  for (i=2; i<NUM_OBJECTS; i++)
  {
    int obj_inside = Obj[i].order;

    if ( Obj[obj_inside].loc == INSIDE + obj &&
         (Obj[obj_inside].prop & PROP_NODESC) == 0 &&
         ( (Obj[obj_inside].prop & PROP_INSIDEDESC) &&
           (Obj[obj_inside].prop & PROP_MOVEDDESC) == 0 ))
    {
      flag = 1;
      PrintObjectDesc(obj_inside, 1);
      PrintCompText("\x0a");
    }
  }

  if (print_empty && flag == 0)
    PrintCompLine("\x49\x74\x27\xa1\x65\x6d\x70\x74\x79\x2e");
}

/* print objects at "location"; location can be player's inventory*/
void PrintPresentObjects(int location, char *heading, int list_flag)
{
  int flag, i, obj;

  flag = 0;
  for (i=2; i<NUM_OBJECTS; i++)
  {
    obj = Obj[i].order;

    if (Obj[obj].loc == location &&
        (Obj[obj].prop & PROP_NODESC) == 0)
    {
      if (flag == 0)
      {
        flag = 1;

        if (location == INSIDE + O_YOU)
          PrintCompLine("\xdc\x75\x27\xa9\xb3\xbb\x72\x79\x97\x3a");
        else if (heading != 0)
          PrintLine(heading);
      }

      if (location == INSIDE + O_YOU || list_flag)
        PrintObjectDesc(obj, 0);
      else
        PrintObjectDesc(obj, 1);

      PrintCompText("\x0a");

      if ((Obj[obj].prop & PROP_OPEN) &&
          (Obj[obj].prop & PROP_ACTOR) == 0)
        PrintContents(obj, "  (which contains)", 0); /* 0: don't print "It's empty."*/
    }
  }

  if (location == INSIDE + O_YOU && flag == 0) 
    PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xb3\xbb\x72\x79\x84\xad\x79\xa2\x97\x2e");
}

void PrintPlayerRoomDesc(int force_description)
{
  if (IsPlayerInDarkness())
    PrintCompLine("\x49\xa6\x9a\x70\xc7\xfa\xb0\xfd\x63\x6b\x8e\xc3\xbb\x9e"
"\xf5\x6b\x65\xec\x89\xef\xfb\xaf\xd4\xb0\xc4\xd0\x67\x72\x75\x65\x2e");
  else
  {
    PrintRoomDesc(Obj[O_YOU].loc, force_description);
    if (force_description || Verbosity != V_SUPERBRIEF) 
	PrintPresentObjects(Obj[O_YOU].loc, 0, 0);
  }
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/* function can call itself*/
int IsObjVisible(int obj)
{
  if (obj < 2 || obj >= NUM_OBJECTS) return 0;

  if (Obj[obj].prop & PROP_EVERYWHERE) 
	return 1; /* presence must be checked by calling function*/

  if (Obj[obj].loc == INSIDE + O_YOU) return 1;   /* obj is in your inventory*/
  if (Obj[obj].loc == Obj[O_YOU].loc) return 1; /* obj is in same room as you*/

  if (Obj[obj].loc >= INSIDE) /* obj is inside container*/
  {
    int container = Obj[obj].loc - INSIDE;

    if (Obj[container].prop & PROP_OPEN) /* container is open*/
      if (IsObjVisible(container)) return 1; /* yikes it's recursive*/
  }

  return 0;
}

void main(void)
{
  HexLoad();	/* load COMMON code at 0C000H */
  InitOverlays();
  
  /*DONT process command line arguments*/

/* ---------------------------------------------
  if (argc < 2) InputStream = stdin;
  else
  {
    InputStream = fopen(argv[1], "r");
    if (InputStream == NULL) InputStream = stdin;
  }
------------------------------------------------ */
  xrndseed();

  RestartGame();

  for (;;)
  {
    if (NumStrWords == 0)
    {
      for (;;)
      {
        if (Verbosity != V_SUPERBRIEF) PrintNewLine();
        GetWords(">");

        if (NumStrWords == 0) 
          PrintCompLine("\x50\xcf\xe0\x9e\x74\x79\xfc\x86\xb6\x63\xe1\x6d\xad\x64\x2e");
        else break;
      }
    }

    PrevItObj = ItObj;
    ItObj = 0;

    CallOverlay0((void*)Parse, OVR_PARSE);

    if (TimePassed) NumMoves++;

    if (TimePassed && GameOver == 0)
    {
      CallOverlay0((void*)RunEventRoutines, OVR_EVENT_ROUTINES);
      /* RunEventRoutines(); */
    }

    if (GameOver)
    {
      if (GameOver == 2) {RestartGame(); continue;}

      PrintScore();
      for (;;)
      {
        GetWords("\nDo you want to restart, restore or exit? ");
        if (MatchCurWord("restart")) {RestartGame(); break;}
        if (MatchCurWord("restore")) 
	{
	  if (CallOverlay0((void*)DoRestore, OVR_SAVE_RESTORE) /* DoRestore() */)
	  break;
	}
        if (MatchCurWord("exit"))
	{
          /* --------------------------------------------
  	  if (InputStream != NULL && InputStream != stdin) 
    	    fclose(InputStream);
	  ----------------------------------------------- */
	 exit(0);
	}
      }
      continue;
    }

    /* if no time passed or*/
    /*   parsed command contained extra words, such as "open box okay"*/
    /*     then abort parsing any remaining commands from input*/
    if (TimePassed == 0 || MatchCurWord("then") == 0) NumStrWords = 0;

    /*"continue"s above take us here*/
  }
}
