#include <string.h>

/* defines, enums ------------------------------------------------------------- */
#define OVR_OVERRIDE_OBJECT_DESC	38
#define OVR_OVERRIDE_ROOM_DESC 		39

#define AllocBuf	0xBB00

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

/* extern functions ---------------------------------------------------------------- */
void PrintText(char *p);
void PrintLine(char *p);
void PrintNewLine(void);
void PrintCompText(char *comp_text);
void PrintCompLine(char *);
int MatchCurWord( char *match);
int DecompressText(char *text_in, int size_in, char *text_out);
int GetObjFromInput(void);
int IsObjVisible(int obj);

short xrnd(void);
int GetRandom(int Range);

short CallOverlay1(void* Routine, short Par1, char CalledRamBank);

/* variables ----------------------------------------------------------------------- */
int above_ground[11] =
        { R_WEST_OF_HOUSE, R_NORTH_OF_HOUSE, R_EAST_OF_HOUSE,
          R_SOUTH_OF_HOUSE, R_FOREST_1, R_FOREST_2, R_FOREST_3,
          R_PATH, R_CLEARING, R_GRATING_CLEARING, R_CANYON_VIEW };

/* code ---------------------------------------------------------------------------- */

/*string contains words separated by commas with no whitespace*/
int IsWordInCommaString( char *p,  char *word)
{
  char *q, *buffer = (char*)AllocBuf;
  int len;

  if (p != 0)
    for (;;)
  {
    q = strchr(p, ',');
/* ----------------------------
    if (q == 0) q = strchr(p, 0);
    len = q-p;
--------------------------- */
      if (q == 0)
      {
        len = strlen(p);
        q = p + len;
      }
      else
        len = q - p;

    if (len > 0 && len < 80)
    {
      memcpy(buffer, p, len);
      buffer[len] = 0;
      if (strcmp(word, buffer) == 0) return 1;
    }

    if (*q == 0) break;
    else p = q+1; /*skip past comma*/
  }

  return 0;
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

int GetFixedObjFromInput(int room)
{
  char* buffer = (char*)AllocBuf; /* char buffer[80*2]; */
   char *p;
   char *q;
  int i, j, len, fobj, fobj_inroom, cw_inroom, fobj_notinroom, cw_notinroom, temp;

  fobj_inroom = 0;    /*matched fixed obj that was found in room*/
  fobj_notinroom = 0; /*                           not found in room*/

  /*keep track of word position because we will be matching multiple noun phrases*/
  temp = CurWord; 
  i = 0;
  for (;;)
  {
    fobj = NounPhraseToFixedObj[i].fobj;
    if (fobj == 0) break; /*reached end of list without finding anything*/

    /*fill buffer with up to two words of noun phrase*/
    memset(buffer, 80*2, 0); /* HiTech library bug (value & length inversed) !!! */
    p = NounPhraseToFixedObj[i].phrase;
    for (j=0; j<2; j++)
    {
      q = strchr(p, ' ');
/* -----------------------
      if (q == 0) q = strchr(p, 0);
      len = q-p; 
------------------------------ */
      if (q == 0)
      {
        len = strlen(p);
        q = p + len;
      }
      else
        len = q - p;

      if (len > 0 && len < 80) memcpy(buffer+80*j, p, len);
      if (*q == 0) break;
      else p = q+1; /*skip past space*/
    }


    CurWord = temp;
    if (NumStrWords >= CurWord+1 && strcmp(StrWord[CurWord], buffer+80*0) == 0)
    {
      CurWord++;
      if (buffer[80*1] != 0)
      {
        if (NumStrWords >= CurWord+1 && strcmp(StrWord[CurWord], buffer+80*1) == 0)
          CurWord++;
        else CurWord = temp;
      }
    }

    if (CurWord > temp) /*found noun phrase*/
    {
      if (NounPhraseToFixedObj[i].room == room) /*found a fixed obj in room*/
      {
        if (fobj_inroom == 0) 
        { /*only use first one found, though there should be only one anyway*/
          fobj_inroom = fobj;
          cw_inroom = CurWord; /*keep track of word position past this match*/
        }
      }
      else /*found a fixed obj not in room*/
      { /*only use first one found, though there should be only one anyway*/
        if (fobj_notinroom == 0) 
        {
          fobj_notinroom = fobj;
          cw_notinroom = CurWord; /*keep track of word position past this match*/
        }
      }
    }

    i++;
  }

  /*return fixed or scenery objects in room first*/

  /*fixed object in room*/
  if (fobj_inroom)
  {
    CurWord = cw_inroom;
    return fobj_inroom;
  }

  /*scenery object in room*/
  if (NumStrWords >= CurWord+1 && room < NUM_ROOMS &&
	IsWordInCommaString(Room[room].scenery, StrWord[CurWord]))
  {
    CurWord++;
    return F_SCENERY_VIS;
  }

  /*fixed object not in room*/
  if (fobj_notinroom)
  {
    CurWord = cw_notinroom;
    return F_NOTVIS;
  }

  /*scenery object not in room*/
  if (NumStrWords >= CurWord+1 && IsWordInCommaString(SceneryNouns, StrWord[CurWord]))
  {
    CurWord++;
    return F_SCENERY_NOTVIS;
  }

  return 0;
}

/*gets obj, fixed obj, or scenery obj, whichever comes first*/
/*returns -1 if player needs to be more specific*/
/*        -2 if player used "it" but it wasn't clear what itobj is*/
/*itobj starts at 0*/
/*if itobj already refers to an object, any additional object invalidates itobj (sets to -1)*/
int GetAllObjFromInput(int room)
{
  int obj;

  if (MatchCurWord("it") || MatchCurWord("them"))
  {
    /*itobj from previous sentence*/
    if (PrevItObj <= 0)
    {
      PrintCompLine("\x49\x27\xf9\xe3\xa6\x73\xd8\x9e\x77\xcd\xa6\xc9\x75\x27\xa9\xda\x65\x66"
"\xac\xf1\x9c\xbd\xb7\xc7\xde\xca\x9e\xd3\xee\xd3\x9e\xdd\x95\x6f\xd6\xe4\xa5\x6e\x73\x2e");
      return -2;
    }
    ItObj = PrevItObj;
    return ItObj;
  }

  /*skip article (if any) immediately before object*/
  if (MatchCurWord("the") || MatchCurWord("a") || MatchCurWord("an")) {}

  /*convert noun phrase to obj*/
  obj = GetObjFromInput(); /*can return -1 if player needs to be more specific*/
  if (obj == 0) obj = GetFixedObjFromInput(room);

  if (obj == 0)
  {
    ItObj = -1;
    PrintCompLine("\x49\xcc\x69\x64\x93\xa9\x63\x6f\x67\x6e\x69\x7a\x9e\xca\x9e\xd3\xee\xd3"
"\x9e\xdd\x95\x6f\xd6\xe4\xa5\x6e\x73\xb5\xd3\x86\xcc\x69\x64\x93\x73\xfc\x63\x69\x66\xc4\xca"
"\x65\x2e");
  }
  else if (obj == -1)
  {
    ItObj = -1;
    PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63\xb7\xc7\xde"
"\xca\x9e\xd3\xee\xd3\x9e\xdd\x95\x6f\xd6\xe4\xa5\x6e\x73\x2e");
  }
  else
  {
    if (ItObj == 0) ItObj = obj; /*first object encountered this sentence; set itobj*/
    else ItObj = -1; /*another obj encountered; invalidate itobj*/
  }

  return obj;
}

int ValidateObject(int obj)
{
  if (obj == F_SCENERY_NOTVIS || obj == F_NOTVIS)
  {
    PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65\x63\x74"
"\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21");
    return 1;
  }

  if (obj == F_AMB)
  {
    PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63\xa3\x62"
"\xa5\xa6\xaf\xcb\xbf\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x2e");
    return 1;
  }

  if (obj > 1 && obj < NUM_OBJECTS && IsObjVisible(obj) == 0)
  {
    PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65\x63\x74"
"\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21");
    return 1;
  }

  return 0;
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
    decompressed_text = p = (char*)AllocBuf; /* malloc(GetDecompressTextSize(compressed_text, compressed_size)); */
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
  decompressed_text = p = (char*)AllocBuf; /* malloc(GetDecompressTextSize(compressed_text, compressed_size)); */
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

int GoToRoutine(int newroom)
{
  int prev_darkness;

  if (YouAreInBoat)
  {
    PrintCompLine("\xdc\x75\x27\xdf\xc0\x61\xd7\x89\x67\x65\xa6\xa5\xa6\xdd"
"\x80\xb0\x6f\xaf\xc6\x69\x72\x73\x74\x2e");
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
    else 
	PrintCompLine("\x8b\xcd\xd7\xee\x6f\xd7\xab\xa7\xbd\xa3\xcc\xbb\x6b\xeb\xfd\x63\x65\x2e");
  }

  PrintPlayerRoomDesc(0);
  return 1;
}

/* for use outside parser*/
/* returns -1 if failure message was printed*/
int GetWith(void)
{
  int with = 0;

  if (MatchCurWord("with") || MatchCurWord("using"))
  {
    with = GetAllObjFromInput(Obj[O_YOU].loc); if (with <= 0) return -1;
  }

  if (ValidateObject(with)) return -1;

  if (with >= NUM_OBJECTS)
    {PrintCompLine("\x8b\xbb\xd4\x27\xa6\x68\x6f\x6c\x64\x84\xa2\x61\x74\x21"); return -1;}

  return with;
}

void PrintBlockMsg(int newroom)
{
  if (newroom >= BL0 && newroom <= 255)
    PrintCompLine(BlockMsg[newroom - BL0]);
}

void PrintInteger(int num)
{
  int neg = 0, size = 0, i;
  char* buf = (char*)AllocBuf;

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

void PrintDesc_Troll(int desc_flag)
{
  if (desc_flag == 0)
    PrintCompText("\xd0\x74\xc2\x6c\x6c");
  else
    switch (TrollDescType)
  {
    case 0: PrintCompText("\x41\xe4\xe0\x74\x79\x2d\xd9\x6f\x6b\x84\x74\xc2\xdf"
"\xb5\x62\xf4\xb9\xb2\xce\x9c\xd0\x62\xd9\x6f\x64\xc4\x61\x78\x65\xb5\x62\xd9"
"\x63\x6b\xa1\xe2\xea\x70\xe0\x73\x61\x67\xbe\xae\xf7\x8a\x81\xc2\x6f\x6d\x2e"); break;
    case 1: PrintCompText("\x41\xb4\xf6\x63\xca\x73\x63\x69\xa5\xa1\x74\xc2\xdf"
"\x87\x73\x70\xf4\x77\xcf\xab\xca\x80\xc6\xd9\xd3\xa4\x41\xdf\xeb\xe0\x73\x61"
"\x67\xbe\xae\xf7\x8a\x81\xc2\xe1\xa3\xa9\xae\xfc\x6e\x2e"); break;
    case 2: PrintCompText("\x41\xeb\xaf\xa0\xf0\xe7\xdf\xc4\x62\x61\x62\x62\xf5"
"\x9c\x74\xc2\xdf\x87\xa0\x72\x65\x2e"); break;
    case 3: PrintCompText("\x41\x9f\xc2\xdf\x87\xa0\x72\x65\x2e"); break;
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

/*matches a maximum of three words*/
/*if there are multiple match objects that are visible,*/
/*this function will ask player to be more specific by returning -1*/
#define match_size_V (AllocBuf + 80*3)
#define match_obj_V (AllocBuf + 80*4)
int GetObjFromInput(void)
{
  char* buffer = (char*)AllocBuf; /* char buffer[80*3]; */
  int num_matches, i, j;
  char* match_size = (char*)match_size_V; /* char match_size[80]; */
  short* match_obj = (short*)match_obj_V;/* short match_obj[80]; */

  num_matches = 0;

  i = 0;
  for (;;) /*look through noun phrase table*/
  {
    int obj, size;
     char *p;

    obj = NounPhraseToObj[i].obj;
    if (obj == 0) break;

    size = 0;
    p = NounPhraseToObj[i].phrase;
    for (j=0; j<3; j++)
    {
      int len;
       char *q;

      q = strchr(p, ' ');
/*------------------------------
      if (q == 0) q = strchr(p, 0);
      len = q-p;
------------------------------------*/
      if (q == 0)
      {
        len = strlen(p);
        q = p + len;
      }
      else
        len = q - p;

      if (len > 0 && len < 80)
      {
        memcpy(buffer+80*j, p, len);
        buffer[80*j+len] = 0;
        size++;
      }
      if (*q == 0) break;
      else p = q+1; /*skip past space*/
    }

    if (size) /*size of noun phrase to match with input*/
    {
      j = 0;
      if (NumStrWords >= CurWord + size) /*is input long enough to match*/
        for (; j<size; j++)
          if (strcmp(StrWord[CurWord+j], buffer+80*j))
            break; /*break early if words are different*/

      if (j == size) /*all words of phrase matched*/
      {
        match_obj[num_matches] = obj;
        match_size[num_matches] = size;
        num_matches++;
        if (num_matches == 80) break;
      }
    }

    i++; /*try another noun phrase from table*/
  }

  if (num_matches == 0) return 0;

  /*if more than one match, check if objects are both visible at same time*/
  /*if so, return -1 so that player can know to be more specific*/
  for (i=0; i<num_matches; i++)
    if (IsObjVisible(match_obj[i]))
      for (j=i+1; j<num_matches; j++)
        if (match_obj[j] != match_obj[i] && IsObjVisible(match_obj[j]))
          return -1;

  /*watch out: need the longest matched size here*/
  /*first match should always have longest size because*/
  /*phrases must be listed in order of decreasing size in phrase table*/
  for (i=0; i<num_matches; i++)
    if (IsObjVisible(match_obj[i])) break; /*use first visible match, if possible*/
  if (i == num_matches) i = 0; /*if no visible matches, use first one*/

  CurWord += match_size[i];
  return match_obj[i];
}
