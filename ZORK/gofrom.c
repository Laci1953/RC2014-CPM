#include <string.h>

/* defines, enums ------------------------------------------------------------- */
#define OVR_OVERRIDE_OBJECT_DESC	38
#define OVR_OVERRIDE_ROOM_DESC 		39

#define AllocBuf	((void*)0xBB00)

#define NUM_OBJECTS  80
#define NUM_ROOMS  111 /* including null room 0 */

#define BL0 (256-23)

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

enum
{
  V_BRIEF,
  V_VERBOSE,
  V_SUPERBRIEF
};

/*room properties bit flags*/
#define R_DESCRIBED     1  /* set when room description already printed */
#define R_BODYOFWATER   2
#define R_LIT           4  /* set when there is natural light or a light fixture */
#define R_WATERHERE     8
#define R_SACRED       16  /* set when thief not allowed in room */
#define R_MAZE         32
#define R_ALWAYSDESC   64

#define O_YOU 1
#define O_CYCLOPS 2 
#define O_COFFIN 29 
#define O_INFLATED_BOAT 39 
#define O_LAMP 36 

#define R_STONE_BARROW 2 
#define R_KITCHEN 14 
#define R_EAST_OF_HOUSE 5 
#define R_STRANGE_PASSAGE 43
#define R_LIVING_ROOM 16 
#define R_EW_PASSAGE 59 
#define R_MAZE_1 22 
#define R_GRATING_CLEARING 12 
#define R_TREASURE_ROOM 44
#define R_RESERVOIR 46
#define R_LAND_OF_LIVING_DEAD 67
#define R_TORCH_ROOM 71 
#define R_ON_RAINBOW 89
#define R_MAZE_4 25 
#define R_DEAD_END_1 26 
#define R_MAZE_11 35 
#define R_MAZE_5 27 
#define R_GRATING_ROOM 36 
#define R_CELLAR 17 
#define R_TINY_CAVE 53 
#define R_WHITE_CLIFFS_NORTH 81 
#define R_WHITE_CLIFFS_SOUTH 82
#define R_DAMP_CAVE 62 
#define R_LOWER_SHAFT 104 
#define R_TIMBER_ROOM 103 
#define R_STUDIO 21 
#define R_ENTRANCE_TO_HADES 66
#define R_CYCLOPS_ROOM 42
#define R_SQUEEKY_ROOM 95 
#define R_MAINTENANCE_ROOM 76 

#define  F_GRATE 2062
#define  F_TRAP_DOOR 2069
#define  F_KITCHEN_WINDOW 2058

#define INSIDE  2048

/* extern data structures ---------------------------------------------------------- */

extern char TimePassed; /*flag: time passed during action*/
extern char GameOver; /*flag, but with special value 2: restart was requested*/
extern int Verbosity;

extern int ItObj, PrevItObj; /*the obj "it" refers to*/

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

extern char *ObjectDesc[];
extern char *RoomDesc[];
extern char *StrWord[]; 

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
struct OVERRIDEOBJECTDESC_STRUCT OverrideObjectDesc[];

struct OVERRIDEROOMDESC_STRUCT
{
   int room;
  void (*f)(void);
};
extern struct OVERRIDEROOMDESC_STRUCT OverrideRoomDesc[];

extern unsigned char *BlockMsg[];

/* extern functions ---------------------------------------------------------------- */
void CallOverlay1(void* Routine, short Par1, char CalledRamBank);
void CallOverlay0(void* Routine, char CalledRamBank);
void ReturnOverlay0(short);
void ReturnOverlay0_(short);

void PrintText(char *p);
void PrintCompText(char *comp_text);
void PrintCompLine(char *p);
int DecompressText(char *text_in, int size_in, char *text_out);
int IsObjVisible(int obj);

/* variables ----------------------------------------------------------------------- */

/* code ---------------------------------------------------------------------------- */

void PrintBlockMsg(int newroom)
{
  if (newroom >= BL0 && newroom <= 255)
    PrintCompLine((char*)BlockMsg[newroom - BL0]);
}

/*returns number of objects in location*/
/*location can be a room, player's inventory, or inside object*/
int GetNumObjectsInLocation(int loc)
{
  int count, i, obj;

  count = 0;
  for (i=2; i<NUM_OBJECTS; i++)
  {
    obj = Obj[i].order;
    if (Obj[obj].loc == loc) count++;
  }
  return count;
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
    PrintCompLine("\x49\xa6\x9a\x70\xc7\xfa\xb0\xfd\x63\x6b\x8e\xc3\xbb\x9e\xf5"
"\x6b\x65\xec\x89\xef\xfb\xaf\xd4\xb0\xc4\xd0\x67\x72\x75\x65\x2e");
  else
  {
    PrintRoomDesc(Obj[O_YOU].loc, force_description);
    if (force_description || Verbosity != V_SUPERBRIEF) 
	PrintPresentObjects(Obj[O_YOU].loc, 0, 0);
  }
}

/*these functions return 1 if action completed; otherwise fall through*/

int GoToRoutine(int newroom)
{
  int prev_darkness;

  if (YouAreInBoat)
  {
    PrintCompLine("\xdc\x75\x27\xdf\xc0\x61\xd7\x89\x67\x65\xa6\xa5\xa6\xdd\x80"
"\xb0\x6f\xaf\xc6\x69\x72\x73\x74\x2e");
    return 1;
  }

  prev_darkness = IsPlayerInDarkness();

  Obj[O_YOU].loc = newroom;
  TimePassed = 1;

  if (IsPlayerInDarkness())
  {
    if (prev_darkness)
    {
      /*kill player that tried to walk from dark to dark*/
      PrintCompLine("\x0a\x0a\x0a\x0a\x0a\x4f\x68\xb5\xe3\x21\x88\xc0\x61\xd7"
"\xb7\xe2\x6b\xd5\xa8\xe5\xba\x81\x73\xfd\xd7\xf1\x9c\x66\xad\x67\xa1\xdd\xa3"
"\xcb\xd8\x6b\x84\x67\x72\x75\x65\x21");
      YoureDead(); /* ##### RIP #####*/
      return 1;
    }
    else PrintCompLine("\x8b\xcd\xd7\xee\x6f\xd7\xab\xa7\xbd\xa3\xcc\xbb\x6b"
"\xeb\xfd\x63\x65\x2e");
  }

  PrintPlayerRoomDesc(0);
  return 1;
}

int GoFrom_StoneBarrow_West(void)
{
  PrintCompLine("\x49\x6e\x73\x69\xe8\x80\x20\x42\xbb\xc2\x77\x0a\x41\xa1\x8f"
"\xd4\xd1\xb6\x81\x62\xbb\xc2\x77\xb5\x81\x64\xe9\xb6\x63\xd9\xd6\xa1\xa7\x65"
"\x78\xd3\x61\x62\xec\xb0\x65\xce\xb9\x86\xa4\x41\xc2\xf6\xab\x8f\xc7\x87\x64"
"\xbb\x6b\xb5\x62\xf7\xa3\xa0\x61\xab\x9a\xad\xfb\xe3\x72\x6d\xa5\xa1\xe7\xd7"
"\x72\x6e\xb5\x62\xf1\x67\x68\x74\xec\xcb\xc7\x83\xc2\x75\x67\xde\xc7\xa1\x63"
"\xd4\xd1\xb6\x72\xf6\xa1\xd0\xf8\xe8\xaa\x74\xa9\x61\x6d\xa4\x53\x70\xad\x6e"
"\x84\x81\xc5\xa9\x61\xf9\x9a\xd0\x73\x6d\xe2\xea\x77\xe9\xe8\xb4\x66\xe9\x74"
"\x62\xf1\x64\x67\x65\xb5\x8c\xef\xc9\xb9\xa3\xeb\xaf\xde\xcf\x61\x64\xa1\xa7"
"\xbd\xa3\xcc\xbb\x6b\x9f\xf6\xed\x6c\xa4\x41\x62\x6f\xd7\x80\xb0\xf1\x64\x67"
"\x65\xb5\x66\xd9\xaf\x84\xa7\x80\xa3\x69\x72\xb5\x9a\xd0\xfd\x72\x67\x9e\x73"
"\x69\x67\x6e\xa4\x49\xa6\xa9\x61\x64\x73\x3a\x20\x20\x41\xdf\xc8\x9e\x77\x68"
"\xba\xc5\x8c\xef\x66\xd3\x9e\xa2\x9a\x62\xf1\x64\x67\x9e\xcd\xd7\xb3\xe1\x70"
"\xcf\xd1\xab\xd0\x67\xa9\xaf\x8d\xeb\xac\x69\xd9\xfe\xa3\x64\xd7\xe5\xd8\x9e"
"\x77\xce\xfa\xc0\xe0\x9f\xbe\xd1\xab\x92\xb7\xc7\x8d\xb3\xa5\xf4\x67\x65\x8e"
"\xc3\xcd\xd7\xee\xe0\xd1\xa9\xab\x81\x66\x69\x72\xc5\xeb\xbb\xa6\xdd\x80\x20"
"\x5a\x4f\x52\x4b\x9f\xf1\xd9\x67\x79\x83\x6f\xd6\xb7\x68\xba\x70\xe0\xa1\x6f"
"\xd7\xb6\xa2\x9a\x62\xf1\x64\x67\x9e\x6d\xfe\xa6\xef\xeb\xa9\x70\xbb\xd5\x89"
"\xf6\xe8\x72\x74\x61\x6b\x9e\xad\xfb\xd7\xb4\x67\xa9\xaf\xac\xa3\x64\xd7\xe5"
"\xd8\x9e\xa2\xaf\xb7\x69\xdf\xaa\x65\xd7\xa9\xec\x9f\xbe\xa6\x92\xaa\x6b\x69"
"\xdf\x8d\xb0\xf4\xd7\x72\x79\x21\x0a\x0a\x85\x5a\x4f\x52\x4b\x9f\xf1\xd9\x67"
"\xc4\x63\xca\xf0\x6e\x75\xbe\xb7\xc7\xde\x22\x5a\x4f\x52\x4b\x20\x49\x49\x3a"
"\x82\x20\x57\x69\x7a\xbb\xab\xdd\x20\x46\xc2\x62\x6f\x7a\x7a\x22\x8d\x87\x63"
"\xe1\x70\xcf\xd1\xab\xa7\x20\x22\x5a\x4f\x52\x4b\x20\x49\x49\x49\x3a\x82\x20"
"\x44\xf6\x67\x65\xca\x20\x4d\xe0\xd1\x72\x2e\x22");
  GameOver = 1;
  ReturnOverlay0_(1);
/*  return 1; */
}

int GoFrom_WestOfHouse_Southwest(void)
{
  if (WonGame == 0) ReturnOverlay0_(0); /* return 0; */
  else ReturnOverlay0_(GoToRoutine(R_STONE_BARROW)); /* return GoToRoutine(R_STONE_BARROW); */
}

int GoFrom_EastOfHouse_West(void)
{
  if (KitchenWindowOpen == 0)
  {
    PrintCompLine("\x85\xf8\xb9\xf2\x87\x63\xd9\xd6\x64\x2e");
    ItObj = F_KITCHEN_WINDOW;
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_KITCHEN)); /* return GoToRoutine(R_KITCHEN); */
}

int GoFrom_Kitchen_East(void)
{
  if (KitchenWindowOpen == 0)
  {
    PrintCompLine("\x85\xf8\xb9\xf2\x87\x63\xd9\xd6\x64\x2e");
    ItObj = F_KITCHEN_WINDOW;
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_EAST_OF_HOUSE)); /* return GoToRoutine(R_EAST_OF_HOUSE); */
}

int GoFrom_LivingRoom_West(void)
{
  if (CyclopsState == 4) /* fled*/
    ReturnOverlay0_(GoToRoutine(R_STRANGE_PASSAGE)); /* return GoToRoutine(R_STRANGE_PASSAGE); */
  else
    {
      PrintCompLine("\x85\x64\xe9\xb6\x9a\x6e\x61\x69\xcf\xab\x73\x68\x75\x74\x2e");
      ReturnOverlay0_(1);
/*  return 1; */
    }
}

int GoFrom_Cellar_Up(void)
{
  if (TrapOpen == 0)
  {
    PrintCompLine("\x85\x74\xf4\x70\xcc\xe9\xb6\x9a\x63\xd9\xd6\x64\x2e");
    ItObj = F_TRAP_DOOR;
  }
  else
    ReturnOverlay0_(GoToRoutine(R_LIVING_ROOM)); /* return GoToRoutine(R_LIVING_ROOM); */

  ReturnOverlay0_(1);
/*  return 1; */
}

int GoFrom_TrollRoom_East(void)
{
  if (TrollAllowsPassage == 0) 
  {
    PrintCompLine("\x85\x74\xc2\xdf\xc6\xd4\x64\xa1"
"\x8f\xdd\xd2\xf8\xa2\xa3\xee\xd4\x61\x63\x84\x67\xbe\x74\xd8\x65\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_EW_PASSAGE)); /* return GoToRoutine(R_EW_PASSAGE); */
}

int GoFrom_TrollRoom_West(void)
{
  if (TrollAllowsPassage == 0) 
  {
    PrintCompLine("\x85\x74\xc2\xdf\xc6\xd4\x64\xa1\x8f\xdd\xd2\xf8\xa2\xa3"
"\xee\xd4\x61\x63\x84\x67\xbe\x74\xd8\x65\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_MAZE_1)); /* return GoToRoutine(R_MAZE_1); */
}

int GoFrom_GratingRoom_Up(void)
{
  if (GratingOpen == 0)
  {
    PrintCompLine("\x85\x67\xf4\xf0\x9c\x9a\x63\xd9\xd6\x64\x2e");
    ItObj = F_GRATE;
  }
  else
  {
    ExitFound = 1;
    ReturnOverlay0_(GoToRoutine(R_GRATING_CLEARING)); /* return GoToRoutine(R_GRATING_CLEARING); */
  }

  ReturnOverlay0_(1);
/*  return 1; */
}

int GoFrom_CyclopsRoom_East(void)
{
  if (CyclopsState == 4) /* fled*/
    ReturnOverlay0_(GoToRoutine(R_STRANGE_PASSAGE)); /* return GoToRoutine(R_STRANGE_PASSAGE); */
  else
  {
    PrintCompLine("\x85\xbf\xc5\xb7\xe2\xea\x9a\x73\x6f\xf5\xab\xc2\x63\x6b\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
}

int GoFrom_CyclopsRoom_Up(void)
{
  if (CyclopsState == 3 || Obj[O_CYCLOPS].loc == 0) /* sleeping or dead*/
  {
    if (YouAreInBoat == 0) ThiefProtectsTreasure();
    ReturnOverlay0_(GoToRoutine(R_TREASURE_ROOM)); /* return GoToRoutine(R_TREASURE_ROOM); */
  }
  else
    PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x64\x6f\xbe\x93\xd9\x6f\x6b\xcb"
"\x69\x6b\x9e\xa0\x27\xdf\xcb\x65\xa6\x8f\x70\xe0\x74\x2e");

  ReturnOverlay0_(1);
/*  return 1; */
}

int GoFrom_ReservoirSouth_North(void)
{
  if (LowTide == 0) 
  {
    PrintCompLine("\x8b\x77\xa5\x6c\xab\x64\xc2\x77\x6e\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_RESERVOIR)); /* return GoToRoutine(R_RESERVOIR); */
}

int GoFrom_ReservoirNorth_South(void)
{
  if (LowTide == 0) 
  {
    PrintCompLine("\x8b\x77\xa5\x6c\xab\x64\xc2\x77\x6e\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_RESERVOIR)); /* return GoToRoutine(R_RESERVOIR); */
}

int GoFrom_EntranceToHades_South(void)
{
  if (SpiritsBanished == 0) 
  {
    PrintCompLine("\x53\xe1\x9e\xa7\x76\xb2\x69\x62\xcf\xc6\xd3\x63\x9e\x70\xa9"
"\xd7\xe5\xa1\x8f\x66\xc2\xf9\x70\xe0\x73\x84\xa2\xc2\x75\x67\xde\x81\x67\xaf\x65\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_LAND_OF_LIVING_DEAD)); /* return GoToRoutine(R_LAND_OF_LIVING_DEAD); */
}

int GoFrom_DomeRoom_Down(void)
{
  if (RopeTiedToRail == 0) 
  {
    PrintCompLine("\x8b\xe7\x6e\xe3\xa6\x67\xba\x64\xf2\xb4\xf8\xa2\xa5\xa6\x66"
"\xf4\x63\x74\xd8\x84\x6d\xad\xc4\x62\xca\x65\x73\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_TORCH_ROOM)); /* return GoToRoutine(R_TORCH_ROOM); */
}

int GoFrom_OntoRainbowRoutine(void)
{
  if (RainbowSolid == 0)
    ReturnOverlay0_(0);
/*  return 0; */
  else ReturnOverlay0_(GoToRoutine(R_ON_RAINBOW)); /* return GoToRoutine(R_ON_RAINBOW); */
}

int GoFrom_Maze2_Down(void)
{
  PrintCompLine("\x8b\x77\xca\x27\xa6\xef\xa3\x62\xcf\x89\x67\x65\xa6\x62\x61"
"\x63\x6b\x20\x75\x70\x89\x81\x74\xf6\xed\xea\x8f\xbb\x9e\x67\x6f\x84\xa2\xc2"
"\x75\x67\xde\x77\xa0\xb4\xc7\xe6\x65\x74\xa1\xbd\x80\xe4\x65\x78\xa6\xc2\xe1\x2e\x0a");
  ReturnOverlay0_(GoToRoutine(R_MAZE_4)); /* return GoToRoutine(R_MAZE_4); */
}

int GoFrom_Maze7_Down(void)
{
  PrintCompLine("\x8b\x77\xca\x27\xa6\xef\xa3\x62\xcf\x89\x67\x65\xa6\x62\x61"
"\x63\x6b\x20\x75\x70\x89\x81\x74\xf6\xed\xea\x8f\xbb\x9e\x67\x6f\x84\xa2\xc2"
"\x75\x67\xde\x77\xa0\xb4\xc7\xe6\x65\x74\xa1\xbd\x80\xe4\x65\x78\xa6\xc2\xe1\x2e\x0a");
  ReturnOverlay0_(GoToRoutine(R_DEAD_END_1)); /* return GoToRoutine(R_DEAD_END_1); */
}

int GoFrom_Maze9_Down(void)
{
  PrintCompLine("\x8b\x77\xca\x27\xa6\xef\xa3\x62\xcf\x89\x67\x65\xa6\x62\x61"
"\x63\x6b\x20\x75\x70\x89\x81\x74\xf6\xed\xea\x8f\xbb\x9e\x67\x6f\x84\xa2\xc2"
"\x75\x67\xde\x77\xa0\xb4\xc7\xe6\x65\x74\xa1\xbd\x80\xe4\x65\x78\xa6\xc2\xe1\x2e\x0a");
  ReturnOverlay0_(GoToRoutine(R_MAZE_11)); /* return GoToRoutine(R_MAZE_11); */
}

int GoFrom_Maze12_Down(void)
{
  PrintCompLine("\x8b\x77\xca\x27\xa6\xef\xa3\x62\xcf\x89\x67\x65\xa6\x62\x61"
"\x63\x6b\x20\x75\x70\x89\x81\x74\xf6\xed\xea\x8f\xbb\x9e\x67\x6f\x84\xa2\xc2"
"\x75\x67\xde\x77\xa0\xb4\xc7\xe6\x65\x74\xa1\xbd\x80\xe4\x65\x78\xa6\xc2\xe1\x2e\x0a");
  ReturnOverlay0_(GoToRoutine(R_MAZE_5)); /* return GoToRoutine(R_MAZE_5); */
}

int GoFrom_GratingClearing_Down(void)
{
  if (GratingRevealed == 0)
    PrintBlockMsg(BL0);
  else
  {
    if (GratingOpen == 0)
    {
      PrintCompLine("\x85\x67\xf4\xf0\x9c\x9a\x63\xd9\xd6\x64\x2e");
      ItObj = F_GRATE;
    }
    else
      ReturnOverlay0_(GoToRoutine(R_GRATING_ROOM)); /* return GoToRoutine(R_GRATING_ROOM); */
  }

  ReturnOverlay0_(1);
/*  return 1; */
}

int GoFrom_LivingRoom_Down(void)
{
  if (TrapOpen)
  {
    if (YouAreInBoat)
      PrintCompLine("\xdc\x75\x27\xdf\xc0\x61\xd7\x89\x67\x65\xa6\xa5\xa6\xdd"
"\x80\xb0\x6f\xaf\xc6\x69\x72\x73\x74\x2e");
    else
    {
      GoToRoutine(R_CELLAR);
      if (YouAreDead == 0 && ExitFound == 0)
      {
        TrapOpen = 0;
        PrintCompLine("\x85\x74\xf4\x70\xcc\xe9\xb6\x63\xf4\x73\xa0\xa1\x73\x68"
"\xf7\xb5\x8c\x8f\xa0\xbb\xaa\xe1\x65\xca\x9e\x62\xbb\xf1\x9c\x69\x74\x2e");
      }
    }
  }
  else if (RugMoved == 0)
    PrintBlockMsg(BL0);
  else
  {
    PrintCompLine("\x85\x74\xf4\x70\xcc\xe9\xb6\x9a\x63\xd9\xd6\x64\x2e");
    ItObj = F_TRAP_DOOR;
  }

  ReturnOverlay0_(1);
/*  return 1; */
}

int GoFrom_SouthTemple_Down(void)
{
  if (Obj[O_COFFIN].loc == INSIDE + O_YOU)
  {
    PrintCompLine("\x8b\xcd\xd7\x93\xd0\x70\xf4\x79\xac\x8a\x67\x65\x74\xf0\x9c"
"\x81\x63\xdd\x66\xa7\xcc\xf2\xb4\x96\x72\x65\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_TINY_CAVE)); /* return GoToRoutine(R_TINY_CAVE); */
}

int GoFrom_WhiteCliffsNorth_South(void)
{
  if (Obj[O_INFLATED_BOAT].loc == INSIDE + O_YOU)
  {
    PrintCompLine("\x85\x70\xaf\xde\x9a\xbd\xba\x6e\xbb\xc2\x77\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_WHITE_CLIFFS_SOUTH)); /* return GoToRoutine(R_WHITE_CLIFFS_SOUTH); */
}

int GoFrom_WhiteCliffsNorth_West(void)
{
  if (Obj[O_INFLATED_BOAT].loc == INSIDE + O_YOU)
  {
    PrintCompLine("\x85\x70\xaf\xde\x9a\xbd\xba\x6e\xbb\xc2\x77\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_DAMP_CAVE)); /* return GoToRoutine(R_DAMP_CAVE); */
}

int GoFrom_WhiteCliffsSouth_North(void)
{
  if (Obj[O_INFLATED_BOAT].loc == INSIDE + O_YOU)
  {
    PrintCompLine("\x85\x70\xaf\xde\x9a\xbd\xba\x6e\xbb\xc2\x77\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_WHITE_CLIFFS_NORTH)); /* return GoToRoutine(R_WHITE_CLIFFS_NORTH); */
}

int GoFrom_TimberRoom_West(void)
{
  if (YouAreDead)
  {
    PrintCompLine("\x8b\xe7\x6e\xe3\xa6\xd4\xd1\xb6\xa7\x86\xb6\x63\xca\x64\xc7"
"\x69\x6f\x6e\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else if (GetNumObjectsInLocation(INSIDE + O_YOU) > 0)
  {
    PrintCompLine("\x8b\xe7\x6e\xe3\xa6\x66\xc7\x95\xc2\x75\x67\xde\xa2\x9a\x70"
"\xe0\x73\x61\x67\x9e\xf8\xa2\x95\xaf\xcb\x6f\x61\x64\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_LOWER_SHAFT)); /* return GoToRoutine(R_LOWER_SHAFT); */
}

int GoFrom_LowerShaft_East(void)
{
  if (GetNumObjectsInLocation(INSIDE + O_YOU) > 0)
  {
    PrintCompLine("\x8b\xe7\x6e\xe3\xa6\x66\xc7\x95\xc2\x75\x67\xde\xa2\x9a\x70"
"\xe0\x73\x61\x67\x9e\xf8\xa2\x95\xaf\xcb\x6f\x61\x64\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_TIMBER_ROOM)); /* return GoToRoutine(R_TIMBER_ROOM); */
}

int GoFrom_Kitchen_Down(void)
{
  if (YouAreSanta == 0)
    PrintCompLine("\x4f\x6e\xec\x20\x53\xad\x74\xd0\x43\xfd\xfe\xb3\xf5\x6d\x62"
"\xa1\x64\xf2\xb4\xfa\x69\x6d\xed\x79\x73\x2e");
  else
    ReturnOverlay0_(GoToRoutine(R_STUDIO)); /* return GoToRoutine(R_STUDIO); */

  ReturnOverlay0_(1);
/*  return 1; */
}

int GoFrom_Studio_Up(void)
{
  int count = GetNumObjectsInLocation(INSIDE + O_YOU);

  if (count == 0)
    PrintCompLine("\x47\x6f\x84\x75\x70\xfb\x6d\x70\x74\x79\x2d\xcd\xb9\xd5\x87"
"\xd0\x62\x61\xab\x69\xe8\x61\x2e");
  else if (count < 3 && Obj[O_LAMP].loc == INSIDE + O_YOU)
    ReturnOverlay0(GoToRoutine(R_KITCHEN)); /* return GoToRoutine(R_KITCHEN); */
  else
    PrintCompLine("\x8b\xe7\x93\x67\x65\xa6\x75\x70\x80\xa9\xb7\xc7\xde\x77\xcd"
"\xa6\xc9\x75\x27\xa9\xb3\xbb\x72\x79\x97\x2e");

  ReturnOverlay0(1);
/*  return 1; */
}

int GoFrom_LandOfLivingDead_North(void)
{
  ReturnOverlay0_(GoToRoutine(R_ENTRANCE_TO_HADES)); /* return GoToRoutine(R_ENTRANCE_TO_HADES); */
}

int GoFrom_StrangePassage_West(void)
{
  ReturnOverlay0_(GoToRoutine(R_CYCLOPS_ROOM)); /* return GoToRoutine(R_CYCLOPS_ROOM); */
}

int GoFrom_NorthTemple_North(void)
{
  ReturnOverlay0_(GoToRoutine(R_TORCH_ROOM)); /* return GoToRoutine(R_TORCH_ROOM); */
}

int GoFrom_MineEntrance_West(void)
{
  ReturnOverlay0_(GoToRoutine(R_SQUEEKY_ROOM)); /* return GoToRoutine(R_SQUEEKY_ROOM); */
}

int GoFrom_DamLobby_North_Or_East(void)
{
  if (MaintenanceWaterLevel > 14) 
  {
    PrintCompLine("\x85\xc2\xe1\x87\x66\x75\xdf\x8a\x77\xaf\xac\x8d\x91\xe3\xa6"
"\xef\xfb\xe5\xac\x65\x64\x2e"); 
    ReturnOverlay0_(1);
/*  return 1; */
  }
  else ReturnOverlay0_(GoToRoutine(R_MAINTENANCE_ROOM)); /*return GoToRoutine(R_MAINTENANCE_ROOM); */
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

