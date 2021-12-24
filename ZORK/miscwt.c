#include <string.h>
#include <ovr47.h>

/* defines, enums ------------------------------------------------------------- */
#define OVR_OVERRIDE_OBJECT_DESC	38
#define OVR_OVERRIDE_ROOM_DESC 		39
#define OVR_VILLAINS_BLOW	47

#define AllocBuf	((void*)0xBB00)

#define LOAD_MAX  100
#define NUM_ROOMS  111 /* including null room 0 */
#define NUM_OBJECTS  80

#define INSIDE  2048

#define O_YOU 1
#define O_LAMP  36
#define O_COFFIN 29
#define O_SWORD  61
#define O_CYCLOPS 2
#define O_BAT 4
#define O_THIEF 5
#define O_TROLL 6
#define O_TROPHY_CASE 9
#define O_MACHINE 10
#define O_MAILBOX 11
#define O_KITCHEN_TABLE 12
#define O_ATTIC_TABLE 13
#define O_WATER 14
#define O_HOT_BELL 19
#define O_BOTTLE 28
#define O_TOOL_CHEST 53
#define O_ROPE 50
#define O_RUSTY_KNIFE 51
#define O_CHALICE 25
#define O_LARGE_BAG 59
#define O_STILETTO 60
#define O_AXE 21
#define O_SWORD  61
#define O_KNIFE 33
#define O_EGG 72
#define O_GHOSTS 3

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
#define R_TROLL_ROOM 18

enum
{
  BLOW_NULL,
  BLOW_MISSED,         /* attacker misses*/
  BLOW_UNCONSCIOUS,    /* defender unconscious*/
  BLOW_KILLED,         /* defender dead*/
  BLOW_LIGHT_WOUND,    /* defender lightly wounded*/
  BLOW_SERIOUS_WOUND,  /* defender seriously wounded*/
  BLOW_STAGGER,        /* defender staggered (miss turn)*/
  BLOW_LOSE_WEAPON,    /* defender loses weapon*/
  BLOW_HESITATE,       /* hesitates (miss on free swing)*/
  BLOW_SITTING_DUCK    /* sitting duck (crunch!)*/
};

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
W_O_AXE,
W_O_KNIFE,
W_O_RUSTY_KNIFE,
W_O_STILETTO,      
W_O_SWORD 
};

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

/*fixed (unmoving) objects*/
enum
{
  F_SCENERY_VIS = 2048, /*some anonymous scenery object, visible*/
  F_SCENERY_NOTVIS,     /*                               not visible*/
  F_NOTVIS,             /*fixed object not visible*/
  F_AMB                /*amibiguous (ask for clarification)*/
};

#define MAX_INVENTORY_ITEMS  7
#define INV_LIMIT_CHANCE     8

#define BL0 233 /* (256-23) */

#define STRENGTH_MIN  2
#define STRENGTH_MAX  7

#define SCORE_MAX  350

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

struct OVERRIDEOBJECTDESC_STRUCT
{
  int obj;
  void (*f)(int);
};
extern struct OVERRIDEOBJECTDESC_STRUCT OverrideObjectDesc[];

struct OVERRIDEROOMDESC_STRUCT
{
  int room;
  void (*f)(void);
};
extern struct OVERRIDEROOMDESC_STRUCT OverrideRoomDesc[];

struct NOUNPHRASETOOBJ_STRUCT
{
   char *phrase;
   unsigned short obj;
};
extern struct NOUNPHRASETOOBJ_STRUCT NounPhraseToObj[];

struct NOUNPHRASETOFIXEDOBJ_STRUCT
{
   char *phrase;
   unsigned short room;
   unsigned short fobj;
};
extern struct NOUNPHRASETOFIXEDOBJ_STRUCT NounPhraseToFixedObj[];

extern unsigned char VillainAttacking[];

extern int NumStrWords;
extern int CurWord;
extern char *StrWord[]; /*pointers to words in str; writing to data pointed-to here is prohibited*/

extern char *ObjectDesc[];
extern char *RoomDesc[];
extern char *BlockMsg[];
extern int Verbosity;
extern char *SceneryNouns;
extern int ItObj, PrevItObj; /*the obj "it" refers to*/
extern unsigned char RopeTiedToRail;
extern unsigned char LowTide;
extern unsigned char YouAreInBoat;
extern unsigned char LoudRoomQuiet;
extern unsigned char BuoyFlag;
extern unsigned char GratingRevealed;
extern unsigned char ExitFound; /* set this when player finds an exit from dungeon other than the trapdoor*/
extern unsigned char YouAreDead;
extern unsigned char GatesOpen;
extern unsigned char WonGame;
extern unsigned char NotLucky;

extern unsigned char GameOver; 
extern unsigned char TrollAllowsPassage;
extern unsigned char TimePassed; /*flag: time passed during action*/

extern int CursorColumn;

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
extern unsigned char ThiefEngrossed;
extern unsigned char YouAreStaggered;

extern int VillainObj[];
extern char *VillainName[];
extern int VillainBestWeaponAgainst[];
extern int VillainBestWeaponAgainstAdvantage[];
extern int VillainStrength[];
extern unsigned char VillainStaggered[];

/* extern functions ---------------------------------------------------------------- */
void PrintText(char *p);
void PrintLine(char *p);
void PrintNewLine(void);
void PrintCompText(char *comp_text);
void PrintCompLine(char *);
int MatchCurWord( char *match);
int DecompressText(char *text_in, int size_in, char *text_out);
void PrintPresentObjects(int location, char *heading, int list_flag);
void PrintObjectDesc(int obj, int desc_flag);
void YoureDead(void);
int IsObjVisible(int obj);

short xrnd(void);
int GetRandom(int Range);

short CallOverlay1(void* Routine, short Par1, char CalledRamBank);

/* variables ----------------------------------------------------------------------- */
int above_ground[11] =
        { R_WEST_OF_HOUSE, R_NORTH_OF_HOUSE, R_EAST_OF_HOUSE,
          R_SOUTH_OF_HOUSE, R_FOREST_1, R_FOREST_2, R_FOREST_3,
          R_PATH, R_CLEARING, R_GRATING_CLEARING, R_CANYON_VIEW };

char BlowForDefense1[13] =
{
  BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_STAGGER, BLOW_STAGGER,
  BLOW_UNCONSCIOUS, BLOW_UNCONSCIOUS, BLOW_KILLED, BLOW_KILLED, BLOW_KILLED,
  BLOW_KILLED, BLOW_KILLED
};

char BlowForDefense2[22] =
{
  BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_STAGGER,
  BLOW_STAGGER, BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED,
  BLOW_MISSED, BLOW_STAGGER, BLOW_STAGGER, BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND,
  BLOW_LIGHT_WOUND, BLOW_UNCONSCIOUS, BLOW_KILLED, BLOW_KILLED, BLOW_KILLED
};

char BlowForDefense3[31] =
{
  BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_STAGGER, BLOW_STAGGER,
  BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND, BLOW_SERIOUS_WOUND, BLOW_SERIOUS_WOUND, BLOW_MISSED,
  BLOW_MISSED, BLOW_MISSED, BLOW_STAGGER, BLOW_STAGGER, BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND,
  BLOW_LIGHT_WOUND, BLOW_SERIOUS_WOUND, BLOW_SERIOUS_WOUND, BLOW_SERIOUS_WOUND, BLOW_MISSED,
  BLOW_STAGGER, BLOW_STAGGER, BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND,
  BLOW_SERIOUS_WOUND, BLOW_SERIOUS_WOUND, BLOW_SERIOUS_WOUND
};

short BlowMsgOffset[10 * 4] =
{
  0, 6, 11, 14, 18, 22, 27, 29, 30, 31,
  0, 4,  5,  8, 12, 15, 19, 22, 24, 25,
  0, 4,  6,  9, 13, 17, 20, 23, 26, 28,
  0, 2,  3,  4,  6,  8, 10, 12, 13, 14
};

/* overlays ----------------------------------------------------------------------- */
/* ------
void BlowRemark(short msg);
int FindWeapon(int obj);

#define BlowRemark 0
#define FindWeapon 0
--------- */
/* code ---------------------------------------------------------------------------- */
int GetRandom(int Range)
{
  long tmp;

  tmp = xrnd() * Range;

  return ((int)(tmp >> 16));
}

/* returns 1 if event of x% chance occurred*/
/* second parameter is used instead if it is >=0 and you're not lucky*/
int PercentChance(int x, int x_not_lucky)
{
  if (NotLucky && x_not_lucky >= 0) x = x_not_lucky;

  if (GetRandom(100) < x) return 1;
  else return 0;
}

void PrintBlowRemark(int player_flag, int i, int blow, int weapon)
{
  int j, index, num, msg, nweapon, arg;

  j = player_flag ? 0 : 1+i;
  index = 10*j + (blow-1);
  num = BlowMsgOffset[index+1] - BlowMsgOffset[index];
  msg = 100*j + BlowMsgOffset[index] + GetRandom(num);

  switch (weapon)
  {
    case O_STILETTO:    nweapon = W_O_STILETTO;		break;
    case O_AXE:         nweapon = W_O_AXE;  		break;
    case O_SWORD:       nweapon = W_O_SWORD;       	break;
    case O_KNIFE:       nweapon = W_O_KNIFE; 		break;
    case O_RUSTY_KNIFE: nweapon = W_O_RUSTY_KNIFE; 	break;
  }

  arg = (msg << 5) + (nweapon << 2) + i;

  CallOverlay1((void*)BlowRemark, arg, OVR_VILLAINS_BLOW);
}

int ThiefDepositBooty(int room)
{
  int flag = 0, obj;

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == INSIDE + O_THIEF &&
        Obj[obj].thiefvalue > 0 &&
        obj != O_STILETTO &&
        obj != O_LARGE_BAG)
  {
    flag = 1;
    Obj[obj].loc = room;
    if (obj == O_EGG)
      Obj[O_EGG].prop |= PROP_OPEN;
  }

  return flag;
}

void VillainUnconcious(int i)
{
  if (i == VILLAIN_TROLL)
  {
    VillainAttacking[i] = 0;

    if (Obj[O_AXE].loc == INSIDE + O_TROLL)
    {
      Obj[O_AXE].loc = Obj[O_YOU].loc;
      Obj[O_AXE].prop &= ~PROP_NODESC;
      Obj[O_AXE].prop &= ~PROP_NOTTAKEABLE;
      Obj[O_AXE].prop |= PROP_WEAPON;
    }

    TrollDescType = 1; /* unconcious*/
    TrollAllowsPassage = 1;
  }
  else if (i == VILLAIN_THIEF)
  {
    VillainAttacking[i] = 0;

    Obj[O_STILETTO].loc = Obj[O_YOU].loc;
    Obj[O_STILETTO].prop &= ~PROP_NODESC;
    Obj[O_STILETTO].prop &= ~PROP_NOTTAKEABLE;

    ThiefDescType = 1; /* unconcious*/
  }
}

void ThiefRecoverStiletto(void)
{
  if (Obj[O_STILETTO].loc == Obj[O_THIEF].loc)
  {
    Obj[O_STILETTO].loc = INSIDE + O_THIEF;
    Obj[O_STILETTO].prop |= PROP_NODESC;
    Obj[O_STILETTO].prop |= PROP_NOTTAKEABLE;
  }
}

void VillainConscious(int i)
{
  if (i == VILLAIN_TROLL)
  {
    if (Obj[O_TROLL].loc == Obj[O_YOU].loc)
    {
      VillainAttacking[i] = 1;
      PrintCompLine("\x85\x74\xc2\xdf\xaa\xf0\x72\x73\xb5\x71\x75\x69\x63\x6b\xec\xda\xbe\x75\x6d\x84\xd0\x66\x69\x67\x68\xf0\x9c\xc5\xad\x63\x65\x2e");
    }

    if (Obj[O_AXE].loc == INSIDE + O_TROLL)
      TrollDescType = 0; /* default*/
    else if (Obj[O_AXE].loc == R_TROLL_ROOM)
    {
      Obj[O_AXE].loc = INSIDE + O_TROLL;
      Obj[O_AXE].prop |= PROP_NODESC;
      Obj[O_AXE].prop |= PROP_NOTTAKEABLE;
      Obj[O_AXE].prop &= ~PROP_WEAPON;
      TrollDescType = 0; /* default*/
    }
    else
      TrollDescType = 3; /* simple description*/

    TrollAllowsPassage = 0;
  }
  else if (i == VILLAIN_THIEF)
  {
    if (Obj[O_THIEF].loc == Obj[O_YOU].loc)
    {
      VillainAttacking[i] = 1;
      PrintCompLine("\x85\xc2\x62\xef\xb6\xa9\x76\x69\xd7\x73\xb5\x62\xf1\x65"
"\x66\xec\xc6\x65\x69\x67\x6e\x84\x63\xca\xf0\x6e\x75\xd5\x20\xf6\x63\xca\x73"
"\x63\x69\xa5\x73\xed\x73\x73\xb5\xad\x64\xb5\x77\xa0\xb4\x94\xd6\xbe\xc0\x9a"
"\x6d\xe1\xd4\x74\xb5\x73\x63\xf4\x6d\x62\xcf\xa1\x61\x77\x61\xc4\x66\xc2\xf9\xc9\x75\x2e");
    }

    ThiefDescType = 0; /* default*/
    ThiefRecoverStiletto();
  }
}


void VillainDead(int i)
{
  if (i == VILLAIN_TROLL)
  {
    if (Obj[O_AXE].loc == INSIDE + O_TROLL)
    {
      Obj[O_AXE].loc = Obj[O_YOU].loc;
      Obj[O_AXE].prop &= ~PROP_NODESC;
      Obj[O_AXE].prop &= ~PROP_NOTTAKEABLE;
      Obj[O_AXE].prop |= PROP_WEAPON;
    }
    TrollAllowsPassage = 1;
  }
  else if (i == VILLAIN_THIEF)
  {
    int flag;

    Obj[O_STILETTO].loc = Obj[O_YOU].loc;
    Obj[O_STILETTO].prop &= ~PROP_NODESC;
    Obj[O_STILETTO].prop &= ~PROP_NOTTAKEABLE;

    flag = ThiefDepositBooty(Obj[O_YOU].loc);

    if (Obj[O_YOU].loc == R_TREASURE_ROOM)
    {
      int obj;

      for (obj=2; obj<NUM_OBJECTS; obj++)
        if (Obj[obj].loc == R_TREASURE_ROOM &&
            obj != O_CHALICE &&
            obj != O_THIEF)
      {
        Obj[obj].prop &= ~PROP_NODESC;
        Obj[obj].prop &= ~PROP_NOTTAKEABLE;
      }

      Obj[O_CHALICE].prop |= PROP_NODESC;
      PrintPresentObjects(R_TREASURE_ROOM, "As the thief dies, the power of his magic decreases, and his treasures reappear:", 1); /* 1: list, no desc*/
      Obj[O_CHALICE].prop &= ~PROP_NODESC;

      PrintCompLine("\x85\xfa\xe2\x69\x63\x9e\x9a\xe3\x77\xaa\x61\x66\x9e\xbd\x9f\x61\x6b\x65\x2e");
    }
    else if (flag)
      PrintCompLine("\x48\x9a\x62\xe9\x74\xc4\xa9\x6d\x61\xa7\x73\x2e");
  }
}

void VillainResult(int i, int defense, int blow)
{
  VillainStrength[i] = defense;

  if (defense == 0)
  {
    PrintCompText("\x41\x6c\x6d\x6f\xc5\xa3\xa1\x73\xe9\xb4\xe0\x80\x20");
    PrintText(VillainName[i]);
    PrintCompLine("\xb0\xa9\xaf\xa0\xa1\xce\xa1\xfd\xc5\xb0\xa9\xaf\x68\xb5\xd0\x63"
"\xd9\x75\xab\xdd\xaa\xa7\xb2\xd1\xb6\x62\xfd\x63\x6b\xc6\x6f\xc1\xd4\xd7\xd9\x70"
"\xa1\xce\x6d\xb5\x8c\x77\xa0\xb4\x81\x66\x6f\xc1\xf5\x66\x74\x73\xb5\x81\xe7\x72"
"\xe7\x73\xa1\xcd\xa1\x64\xb2\x61\x70\xfc\xbb\x65\x64\x2e");

    VillainAttacking[i] = 0;
    Obj[VillainObj[i]].loc = 0;

    VillainDead(i);
  }
  else if (blow == BLOW_UNCONSCIOUS)
    VillainUnconcious(i);
}

int GetBlow(int attack, int defense)
{
  int blow = 0;

  if (defense == 1)
  {
    int j = attack - 1, offset[3]; /* = {0, 2, 4}; */
    offset[0]=0; offset[1]=2; offset[2]=4;

    if (j < 0) j = 0; else if (j > 2) j = 2;
    blow = BlowForDefense1[offset[j] + GetRandom(9)];
  }
  else if (defense == 2)
  {
    int j = attack - 1, offset[4]; /* = {0, 9, 11, 13}; */
    offset[0]=0;offset[1]=9;offset[2]=11;offset[3]=13;

    if (j < 0) j = 0; else if (j > 3) j = 3;
    blow = BlowForDefense2[offset[j] + GetRandom(9)];
  }
  else if (defense > 2)
  {
    int j = attack - defense + 2, offset[5]; /* = {0, 2, 11, 13, 22}; */
    offset[0]=0;offset[1]=2;offset[2]=11;offset[3]=13;offset[4]=22;

    if (j < 0) j = 0; else if (j > 4) j = 4;
    blow = BlowForDefense3[offset[j] + GetRandom(9)];
  }

  return blow;
}

int PlayerFightStrength(int adjust)
{
  int s =  STRENGTH_MIN + (STRENGTH_MAX - STRENGTH_MIN) * Score / SCORE_MAX;

  if (adjust)
    s += PlayerStrength;

  return s;
}

int VillainFightStrength(int i, int player_weapon)
{
  int obj, strength;

  obj = VillainObj[i];
  strength = VillainStrength[i];

  if (strength >= 0)
  {
    if (obj == O_THIEF && ThiefEngrossed)
    {
      ThiefEngrossed = 0;
      if (strength > 2) strength = 2;
    }

    if (player_weapon &&
        (Obj[player_weapon].prop & PROP_WEAPON) &&
        player_weapon == VillainBestWeaponAgainst[i])
    {
      strength -= VillainBestWeaponAgainstAdvantage[i];
      if (strength < 1) strength = 1;
    }
  }

  return strength;
}

/* obj is thing being attacked by player*/
void PlayerBlow(int obj, int player_weapon)
{
  int i, attack, defense, defense_weapon, blow;

  for (i=0; i<NUM_VILLAINS; i++)
    if (VillainObj[i] == obj) break;

  if (i < NUM_VILLAINS)
    VillainAttacking[i] = 1;

  if (YouAreStaggered)
  {
    YouAreStaggered = 0;
    PrintCompLine("\x8b\xbb\x9e\xc5\x69\xdf\xda\x65\x63\x6f\xd7\xf1\x9c\x66\xc2\xf9"
"\xa2\xaf\xcb\xe0\xa6\x62\xd9\x77\xb5\x73\xba\x92\xa3\x74\x74\x61\x63\x6b\x87\xa7"
"\x65\x66\x66\x65\x63\xf0\x76\x65\x2e");
    return;
  }

  if (obj == O_YOU)
  {
    PrintCompLine("\x57\x65\xdf\xb5\x8f\xa9\xe2\xec\xcc\x69\xab\xc7\x95\xaf\x9f\x69"
"\x6d\x65\xa4\x49\xa1\x73\x75\x69\x63\x69\xe8\xeb\x61\xa7\xcf\x73\x73\x3f");
    YoureDead(); /* ##### RIP #####*/
    return;
  }

  attack = PlayerFightStrength(1);
  if (attack < 1) attack = 1;

  if (i < NUM_VILLAINS)
    defense = VillainFightStrength(i, player_weapon);
  else
    defense = 0;

  if (defense == 0) /* catches case of i == NUM_VILLAINS*/
  {
    PrintCompLine("\x41\x74\x74\x61\x63\x6b\x84\xa2\xaf\x87\x70\x6f\xa7\x74\xcf\x73\x73\x2e");
    return;
  }

  defense_weapon = CallOverlay1((void*)FindWeapon, obj, OVR_VILLAINS_BLOW);  /* FindWeapon(obj); */

  if ((defense_weapon == 0 && obj != O_CYCLOPS) || defense < 0)
  {
    PrintCompText("\x85");
    if (defense < 0) PrintCompText("\xf6\x63\xca\x73\x63\x69\xa5\x73\x20");
    else             PrintCompText("\xf6\xbb\x6d\x65\x64\x20");
    PrintText(VillainName[i]);
    PrintCompLine("\x91\xe3\xa6\xe8\x66\xd4\xab\xce\x6d\xd6\x6c\x66\x3a\x20\x48\x9e\x64\x69\x65\x73\x2e");
    blow = BLOW_KILLED;
  }
  else
  {
    blow = GetBlow(attack, defense);

    if (blow == BLOW_STAGGER && defense_weapon && PercentChance(25, -1))
      blow = BLOW_LOSE_WEAPON;

    PrintBlowRemark(1, i, blow, player_weapon); /* 1: player blow*/
  }

  if (blow == BLOW_MISSED || blow == BLOW_HESITATE)
  {
  }
  else if (blow == BLOW_UNCONSCIOUS)
    defense = -defense;
  else if (blow == BLOW_KILLED || blow == BLOW_SITTING_DUCK)
    defense = 0;
  else if (blow == BLOW_LIGHT_WOUND)
  {
    defense--;
    if (defense < 0) defense = 0;
  }
  else if (blow == BLOW_SERIOUS_WOUND)
  {
    defense -= 2;
    if (defense < 0) defense = 0;
  }
  else if (blow == BLOW_STAGGER)
    VillainStaggered[i] = 1;
  else
  {
    Obj[defense_weapon].loc = Obj[O_YOU].loc;
    Obj[defense_weapon].prop &= ~PROP_NODESC;
    Obj[defense_weapon].prop &= ~PROP_NOTTAKEABLE;
    Obj[defense_weapon].prop |= PROP_WEAPON;
  }

  VillainResult(i, defense, blow);
}

void PrintUsingMsg(int obj)
{
  PrintCompText("\x28\xfe\x84");
  PrintObjectDesc(obj, 0);
  PrintCompText("\x29\x0a");
}

void AttackVillain(int obj, int with_to)
{
  if (with_to >= NUM_OBJECTS)
    {PrintCompLine("\x8b\xbb\xd4\x27\xa6\x68\x6f\x6c\x64\x84\xa2\x61\x74\x21"); return;}

  if (with_to == 0)
  {
    int i;

    for (i=2; i<NUM_OBJECTS; i++)
    {
      with_to = Obj[i].order;
      if (Obj[with_to].loc == INSIDE + O_YOU &&
          (Obj[with_to].prop & PROP_WEAPON)) break;
    }

    if (i == NUM_OBJECTS) with_to = 0;
    else PrintUsingMsg(with_to);
  }

  if (obj == O_BAT)
  {
    PrintCompLine("\x8b\xe7\x93\xa9\x61\xfa\xc0\x69\x6d\x3b\xc0\x65\x27\xa1\xca\x80\xb3\x65\x69\xf5\x6e\x67\x2e");
    return;
  }
  else if (obj == O_GHOSTS)
  {
    if (with_to == 0) PrintCompLine("\x8b\xd6\x65\xf9\xf6\x61\x62\xcf\x89\xa7\xd1\xf4\x63\xa6\xf8\xa2\x80\xd6\xaa\x70\x69\xf1\x74\x73\x2e");
    else              PrintCompLine("\x48\xf2\x91\x86\xa3\x74\x74\x61\x63\x6b\xa3\xaa\x70\x69\xf1\xa6\xf8\xa2\xee\xaf\xac\x69\xe2\xae\x62\x6a\x65\x63\x74\x73\x3f");
    return;
  }
  else if (obj == O_THIEF && (Obj[O_THIEF].prop & PROP_NODESC))
  {
    PrintCompLine("\x8b\xd6\x6e\xd6\xaa\xe1\x65\xca\x9e\xed\xbb\x62\x79\xb5\x62\xf7\x91\x27\xa6\xd6\x9e\x96\x6d\x2e");
    return;
  }

  if (with_to == 0 || with_to == O_YOU)
  {
    PrintCompText("\x54\x72\x79\x84\xbd\xa3\x74\x74\x61\x63\x6b\x20"); if (obj == O_YOU) PrintCompText("\x92\xd6\x6c\x66"); else PrintCompText("\x69\x74");
    PrintCompLine("\xb7\xc7\xde\x92\xb0\xbb\x9e\xcd\xb9\xa1\x9a\x73\x75\x69\x63\x69\x64\x61\x6c\x2e");
    return;
  }

  if ((Obj[with_to].prop & PROP_WEAPON) == 0)
  {
    PrintCompText("\x54\x72\x79\x84\xbd\xa3\x74\x74\x61\x63\x6b\x20"); if (obj == O_YOU) PrintCompText("\x92\xd6\x6c\x66"); else PrintCompText("\x69\x74");
    PrintCompLine("\xb7\xc7\xde\xa2\xaf\x87\x73\x75\x69\x63\x69\x64\x61\x6c\x2e");
    return;
  }

  TimePassed = 1;

  if (with_to == O_RUSTY_KNIFE)
  {
    Obj[O_RUSTY_KNIFE].loc = 0;
    PrintCompLine("\x41\xa1\x81\x6b\x6e\x69\x66\x9e\x61\x70\x70\xc2\x61\xfa\xbe\xa8\x74\xa1\x76\x69\x63\xf0\x6d\xb5\x92\xee\xa7\xab\x9a\x73\x75\x62\x6d\xac\x67\xd5\xb0\xc4\xad\xae\xd7\x72\x6d\xe0\xd1\xf1\x9c\xf8\xdf\xa4\x53\xd9\x77\xec\xb5\x92\xc0\x8c\x74\xd8\x6e\x73\xb5\xf6\xf0\xea\x81\x72\xfe\x74\xc4\x62\xfd\xe8\x87\xad\xa8\x6e\xfa\xc6\xc2\xf9\x92\xe4\x65\x63\x6b\x83\x9e\x6b\x6e\x69\x66\x9e\xd6\x65\x6d\xa1\xbd\xaa\x84\xe0\xa8\xa6\x73\x61\x76\x61\x67\x65\xec\xaa\xf5\x74\xa1\x92\x95\xc2\x61\x74\x2e");
    YoureDead(); /* ##### RIP #####*/
    return;
  }

  if (obj == O_CYCLOPS && CyclopsState == 3) /* asleep*/
  {
    CyclopsState = 0;
    VillainAttacking[VILLAIN_CYCLOPS] = 1;
    PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x79\x61\x77\x6e\xa1\x8c\xc5\xbb\xbe\xa3\xa6\x81\xa2\x84\xa2\xaf\xb7\x6f\x6b\x9e\xce\xf9\x75\x70\x2e");
    return;
  }

  PlayerBlow(obj, with_to);
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

void PrintPlayerRoomDesc(int force_description)
{
  if (IsPlayerInDarkness())
    PrintCompLine("\x49\xa6\x9a\x70\xc7\xfa\xb0\xfd\x63\x6b\x8e\xc3\xbb\x9e\xf5\x6b\x65\xec"
"\x89\xef\xfb\xaf\xd4\xb0\xc4\xd0\x67\x72\x75\x65\x2e");
  else
  {
    PrintRoomDesc(Obj[O_YOU].loc, force_description);
    if (force_description || Verbosity != V_SUPERBRIEF) 
	PrintPresentObjects(Obj[O_YOU].loc, 0, 0);
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

