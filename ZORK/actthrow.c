#include <string.h>

/* defines, enums ------------------------------------------------------------- */
#define AllocBuf 0xBB00

#define OVR_DO_MISC			41
#define OVR_DO_MISC_WITH_TO		44
#define OVR_DO_MISC_TO			50

#define NUM_ROOMS  111 /* including null room 0 */
#define NUM_OBJECTS  80
#define INSIDE  2048

#define BL0 (256-23)

#define A_NORTH 1
#define A_RESTART 76
#define A_EMPTY 35
#define A_WEAR 102       
#define A_REMOVE 75       
#define A_RING 78        
#define A_WIND 104        
#define A_PLAY 65 
#define A_ENTER 36       
#define A_EXIT 38       
#define A_WHEREIS 103
#define A_EXAMINE 37
#define A_WAVE 101
#define A_OPEN 64     
#define A_CLOSE 22       
#define A_LOOKIN 55    
#define A_LOOKTHROUGH 57
#define A_MOUNT 60       
#define A_DISMOUNT 30     
#define A_EAT 33       
#define A_DRINK 31      
#define A_WEAR 102       
#define A_REMOVE 75       
#define A_PLAY 65      
#define A_SLEEPON 83      
#define A_RAISE 73      
#define A_LOWER 59      
#define A_ENTER 36       
#define A_EXIT 38       
#define A_READ 74      
#define A_RING 78        
#define A_WIND 104        
#define A_CLIMB 18      
#define A_CLIMBUP 21     
#define A_CLIMBDOWN 19   
#define A_SLIDEUP 85    
#define A_SLIDEDOWN 84   
#define A_CLIMBTHROUGH 20
#define A_LISTENTO 51    
#define A_CROSS   24       
#define A_EXORCISE 39    
#define A_SMELL 86     
 
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
#define R_LAND_OF_LIVING_DEAD 67
#define R_GRATING_CLEARING 12
#define R_GRATING_ROOM 36
#define R_SLIDE_ROOM 110
#define R_CELLAR 17
#define R_TREASURE_ROOM 44

#define O_YOU 1
#define O_LARGE_BAG 59
#define O_LOWERED_BASKET 7
#define O_CHALICE 25
#define O_LARGE_BAG 59
#define O_WATER 14
#define O_INFLATED_BOAT 39
#define O_BAT 4
#define O_CYCLOPS 2
#define O_THIEF 5
#define O_TROLL 6
#define O_MACHINE 10
#define O_TROPHY_CASE 9
#define O_MAILBOX 11
#define O_KITCHEN_TABLE 12
#define O_ATTIC_TABLE 13
#define O_HOT_BELL 19
#define O_BOTTLE 28
#define O_TOOL_CHEST 53
#define O_ROPE 50
#define O_RUSTY_KNIFE 51
#define O_SWORD 61
#define O_BROKEN_LAMP 22
#define O_LAMP 36
#define O_EGG 72
#define O_BROKEN_EGG 73

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

#define F_BOARD 2053
#define F_SONGBIRD 2054
#define F_BODIES 2065
#define F_RUG 2079 
#define F_NAILS 2095
#define F_GRANITE_WALL 2096
#define F_CHAIN 2097
#define F_BOLT 2072
#define F_BUBBLE 2073
#define F_MIRROR2 2086
#define F_MIRROR1 2087
#define F_BONES 2084
#define F_GRATE 2062
#define F_SLIDE 2052
#define F_RIVER 2067
#define F_SCENERY_VIS 2048
#define F_SCENERY_NOTVIS 2049
#define F_NOTVIS 2050 
#define F_AMB 2051 

enum
{
  VILLAIN_TROLL,
  VILLAIN_THIEF,
  VILLAIN_CYCLOPS,

  NUM_VILLAINS
};

enum
{
  V_BRIEF,
  V_VERBOSE,
  V_SUPERBRIEF
};

/* extern data structures ---------------------------------------------------------- */
extern char *ObjectDesc[];
extern char *RoomDesc[];
extern int Verbosity;

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

struct R_PASSAGES_STRUCT
{
  char passage[10];
};
extern struct R_PASSAGES_STRUCT RoomPassages[];

struct DOMISC_STRUCT
{
   int action;
   int obj; /*can be obj or fobj*/
  void (*f)(void);
};
extern struct DOMISC_STRUCT DoMisc[];

struct DOMISCTO_STRUCT
{
   int action;
   int to; /*can be obj or fobj*/
  void (*f)(int);
};
extern struct DOMISCTO_STRUCT DoMiscGiveTo[];
extern struct DOMISCTO_STRUCT DoMiscThrowTo[];

extern int NumStrWords;
extern int CurWord;

extern unsigned char YouAreDead;
extern unsigned char GameOver; /*flag, but with special value 2: restart was requested*/
extern unsigned char NotLucky;
extern int NumDeaths;
extern unsigned char YouAreInBoat;
extern unsigned char ExitFound; /* set this when player finds an exit from dungeon other than the trapdoor*/
extern unsigned char TrollAllowsPassage;
extern int ThiefDescType; /* 0: default  1: unconscious*/
extern int DownstreamCounter;
extern unsigned char TimePassed; /*flag: time passed during action*/

extern char *BlockMsg[];

/* extern functions ---------------------------------------------------------------- */
int GetRandom(int Range);
void ScatterInventory(void);
int IsObjVisible(int obj);
int IsObjCarriedByActor(int obj);
int IsPlayerInDarkness(void);
void PrintObjectDesc(int obj, int desc_flag);
void PrintContents(int obj, char *heading, int print_empty);
void PrintPresentObjects(int location, char *heading, int list_flag);
void PrintRoomDesc(int room, int force_description);
void PrintPlayerRoomDesc(int force_description);
void YoureDead(void);
void PrintBlockMsg(int newroom);
int GetAllObjFromInput(int room);
int GetNumObjectsInLocation(int loc);
int GetTotalSizeInLocation(int loc);
void MoveObjOrderToLast(int obj);
int MatchCurWord( char *match);
int PercentChance(int x, int x_not_lucky);

void MoveTreasuresToLandOfLivingDead(int loc);
int InterceptTakeFixedObj(int obj);
int InterceptTakeOutOf(int container);
int InterceptDropPutObj(int obj, int container, int test, int multi);
int InterceptTakeObj(int obj);
int GetPlayersVehicle(void);
void PrintCantAction(int action);
int TakeOutOfRoutine(int *container);
int VerifyTakeableObj(int obj, int container, int num_takes);
int TakeRoutine(int obj, char *msg);
int VerifyTakeAllExceptObj(int obj);
void TakeAllRoutine(void);
int DropPutVerifyContainer(int container);
void DropPutAllRoutine(int put_flag);
int ActionDirectionRoutine(int newroom);
void ThrowObjRoutine(int obj, int to);
void OpenObj(int obj);
void CloseObj(int obj);
void LookInObj(int obj);
void EmptyObj(int obj);
short xrnd(void);

short CallOverlay1(void* Routine, short Par1, char CalledRamBank);
short CallOverlay0(void* Routine, char CalledRamBank);
void ReturnOverlay1(short);
void ReturnOverlay0(short);

/* variables ----------------------------------------------------------------------- */
  char *prep[13] = { "across", "at", "from", "in", "inside", "into", "off", "out", 
"outside", "over", "to", "toward", "through" };

/* code ---------------------------------------------------------------------------- */
void ThrowObjRoutine(int obj, int to)
{
  int prev_darkness = IsPlayerInDarkness();

  switch (obj)
  {
    case O_LAMP:
      PrintCompLine("\x85\xfd\x6d\x70\xc0\xe0\xaa\x6d\xe0\xa0\xab\xa7\xbd\x80"
"\xc6\xd9\xd3\xb5\x8c\x81\xf5\x67\x68\xa6\xcd\xa1\x67\xca\x9e\xa5\x74\x2e");
      TimePassed = 1;
      Obj[O_LAMP].loc = 0;
      Obj[O_BROKEN_LAMP].loc = Obj[O_YOU].loc;
    break;

    case O_EGG:
      PrintCompLine("\xdc\xd8\xda\xaf\xa0\xb6\xa7\xe8\xf5\xe7\xd1\xc0\xad\x64"
"\xf5\x9c\xdd\x80\xfb\x67\xc1\xcd\xa1\xe7\xfe\xd5\xa8\xa6\x73\xe1\x9e\x64\x61"
"\x6d\x61\x67\x65\xb5\xe2\xa2\xa5\x67\xde\x8f\xcd\xd7\xaa\x75\x63\x63\xf3\xe8"
"\xab\xa7\xae\xfc\x6e\x84\x69\x74\x2e");
      TimePassed = 1;
      Obj[O_EGG].loc = 0;
      Obj[O_BROKEN_EGG].loc = Obj[O_YOU].loc;
      Obj[O_BROKEN_EGG].prop |= PROP_OPENABLE;
      Obj[O_BROKEN_EGG].prop |= PROP_OPEN;
    break;

    case O_BOTTLE:
      PrintCompLine("\x85\x62\xff\x74\xcf\xc0\xc7\xa1\x81\x66\xbb\xb7\xe2\xea"
"\x8c\x73\xcd\x74\xd1\x72\x73\x2e");
      TimePassed = 1;
      Obj[O_BOTTLE].loc = 0;
    break;

    default:
      if (to == 0) PrintCompLine("\x49\xa6\x74\x75\x6d\x62\xcf\xa1\xbd\x80\xe6\xc2\xf6\x64\x2e");
      else         PrintCompLine("\x8b\x6d\xb2\x73\x2e");
      TimePassed = 1;
      Obj[obj].loc = Obj[O_YOU].loc;
      MoveObjOrderToLast(obj);
    break;
  }

  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}

void ParseActionThrow_ext(void)
{
  int obj, to, i;

  obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) ReturnOverlay0(0);
  to = 0;
  for (i=0; i<13; i++)
    if (MatchCurWord(prep[i])) break;
  if (i < 13)
  {
    MatchCurWord("of"); /* as in "throw ball out of park"*/
    to = GetAllObjFromInput(Obj[O_YOU].loc); if (to <= 0) ReturnOverlay0(0);
  }

  if (obj == F_SCENERY_NOTVIS || obj == F_NOTVIS)
    {PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); ReturnOverlay0(0);}
  else if (obj == F_AMB)
    {PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63"
"\xa3\x62\xa5\xa6\xaf\xcb\xbf\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x2e");
    ReturnOverlay0(0);}
  else if (obj == O_YOU)
    {PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21"); ReturnOverlay0(0);}
  else if (obj >= NUM_OBJECTS)
    {PrintCompLine("\x8b\xbb\xd4\x27\xa6\x68\x6f\x6c\x64\x84\xa2\x61\x74\x21"); ReturnOverlay0(0);}
  else if (Obj[obj].loc != INSIDE + O_YOU)
    {PrintCompLine("\x8b\xbb\xd4\x27\xa6\x68\x6f\x6c\x64\x84\xa2\x61\x74\x21"); ReturnOverlay0(0);}

  if (to == F_SCENERY_NOTVIS || to == F_NOTVIS)
    {PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); ReturnOverlay0(0);}
  else if (to == F_AMB)
    {PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63"
"\xa3\x62\xa5\xa6\xaf\xcb\xbf\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x2e"); ReturnOverlay0(0);}
  else if (to == O_YOU)
    {PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21"); ReturnOverlay0(0);}
  else if (to > 0 && to < NUM_OBJECTS && IsObjVisible(to) == 0)
    {PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); ReturnOverlay0(0);}

  for (i=0; DoMiscThrowTo[i].f != 0; i++)
    if (DoMiscThrowTo[i].to == to)
      {
        /* DoMiscThrowTo[i].f(obj); */
        CallOverlay1((void*)DoMiscThrowTo[i].f, obj, OVR_DO_MISC_TO);
        ReturnOverlay0(0);
      }

  ThrowObjRoutine(obj, to);

  ReturnOverlay0(0);
}

void OpenObj(int obj)
{
  int prev_darkness;

  if ((Obj[obj].prop & PROP_OPENABLE) == 0 ||
      (Obj[obj].prop & PROP_ACTOR))
    {PrintCompLine("\x8b\xe7\x93\x6f\xfc\xb4\xa2\x61\x74\x21"); return;}

  if (Obj[obj].prop & PROP_OPEN)
    {PrintCompLine("\x49\x74\x27\xa1\xe2\xa9\x61\x64\xc4\x6f\xfc\x6e\x2e"); return;}

  prev_darkness = IsPlayerInDarkness();

  Obj[obj].prop |= PROP_OPEN; /*open object*/
  PrintCompLine("\x4f\x6b\x61\x79\x2e");
  TimePassed = 1;

  if (IsPlayerInDarkness() == 0)
    PrintContents(obj, "It contains:", 0); /* 0: don't print "It's empty."*/

  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}

void CloseObj(int obj)
{
  int prev_darkness;

  if ((Obj[obj].prop & PROP_OPENABLE) == 0 ||
      (Obj[obj].prop & PROP_ACTOR))
    {PrintCompLine("\x8b\xe7\x93\x63\xd9\xd6\x95\x61\x74\x21"); return;}

  if ((Obj[obj].prop & PROP_OPEN) == 0)
    {PrintCompLine("\x49\x74\x27\xa1\xe2\xa9\x61\x64\xc4\x63\xd9\xd6\x64\x2e"); return;}

  prev_darkness = IsPlayerInDarkness();

  Obj[obj].prop &= ~PROP_OPEN; /*close object*/
  PrintCompLine("\x4f\x6b\x61\x79\x2e");
  TimePassed = 1;

  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}

void LookInObj(int obj)
{
  if ((Obj[obj].prop & PROP_OPEN) &&
      (Obj[obj].prop & PROP_ACTOR) == 0)
    PrintContents(obj, "It contains:", 1); /* 1: allow printing "It's empty."*/
  else
  {
    if ((Obj[obj].prop & PROP_OPENABLE) == 0)
      PrintCompLine("\x8b\xe7\x93\xd9\x6f\x6b\xa8\x6e\x73\x69\xe8\x95\x61\x74\x21");
    else
      PrintCompLine("\x49\x74\x27\xa1\x63\xd9\xd6\x64\x2e");
  }
}

void EmptyObj(int obj)
{
  int flag = 0, i;

  if ((Obj[obj].prop & PROP_OPENABLE) == 0)
    {PrintCompLine("\x8b\xe7\x93\x65\x6d\x70\x74\xc4\xa2\x61\x74\x21"); return;}

  if ((Obj[obj].prop & PROP_OPEN) == 0)
    {PrintCompLine("\x49\x74\x27\xa1\x63\xd9\xd6\x64\x2e"); return;}

  for (i=2; i<NUM_OBJECTS; i++)
  {
    int obj_inside = Obj[i].order;

    if (Obj[obj_inside].loc == INSIDE + obj)
    {
      flag = 1;
      TimePassed = 1;

      Obj[obj_inside].loc = Obj[O_YOU].loc;
      Obj[obj_inside].prop |= PROP_MOVEDDESC;

      PrintObjectDesc(obj_inside, 0);
      PrintCompLine("\x3a\x20\x44\xc2\x70\xfc\x64\x2e");
    }
  }

  if (flag == 0)
    PrintCompLine("\x49\x74\x27\xa1\xe2\xa9\x61\x64\xc4\x65\x6d\x70\x74\x79\x2e");
}

void PrintCantAction(int action)
{
  switch (action)
  {
    case A_OPEN:         PrintCompLine("\x8b\xe7\x93\x6f\xfc\xb4\xa2\x61\x74\x21");break;
    case A_CLOSE:        PrintCompLine("\x8b\xe7\x93\x63\xd9\xd6\x95\x61\x74\x21");break;
    case A_LOOKIN:       PrintCompLine("\x8b\xe7\x93\xd9\x6f\x6b\xa8\x6e\x73\x69\xe8\x95\x61\x74\x2e");          break;
    case A_LOOKTHROUGH:  PrintCompLine("\x8b\xe7\x93\xd9\x6f\x6b\x95\xc2\x75\x67\xde\xa2\x61\x74\x2e");         break;
    case A_MOUNT:        PrintCompLine("\x8b\xe7\x93\x67\x65\xa6\xca\x95\x61\x74\x21");break;
    case A_DISMOUNT:     PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xae\xb4\xa2\x61\x74\x21");break;
    case A_EAT:          PrintCompLine("\xbc\xaf\xcc\x6f\xbe\xe4\xff\xaa\xa5"
"\xb9\x20\xd7\x72\xc4\x61\x70\xfc\xf0\x7a\x97\x21"); break;
    case A_DRINK:        PrintCompLine("\x8b\xe7\x93\x64\xf1\x6e\x6b\x95\x61\x74\x21");break;
    case A_WEAR:         PrintCompLine("\x8b\xe7\x93\x77\xbf\xb6\xa2\x61\x74\x21");break;
    case A_REMOVE:       PrintCompLine("\x8b\xbb\xd4\x27\xa6\x77\xbf\xf1\x9c\xa2\x61\x74\x21");             break;
    case A_PLAY:         PrintCompLine("\x8b\xe7\x93\x70\xfd\xc4\xa2\x61\x74\x21");break;
    case A_SLEEPON:      PrintCompLine("\x8b\xe7\x93\x73\xcf\x65\x70\xae\xb4\xa2\x61\x74\x21");             break;
    case A_RAISE:        PrintCompLine("\x8b\xe7\x93\xf4\xb2\x9e\xa2\x61\x74\x2e");break;
    case A_LOWER:        PrintCompLine("\x8b\xe7\x93\xd9\x77\xac\x95\x61\x74\x2e");break;
    case A_ENTER:        PrintCompLine("\x8b\xe7\x93\x67\xba\xa7\x73\x69\xe8\x95\x61\x74\x21");            break;
    case A_EXIT:         PrintCompLine("\x8b\xbb\xd4\x27\xa6\xa7\x95\x61\x74\x21");break;
    case A_READ:         PrintCompLine("\x99\xa9\x27\xa1\xe3\xa2\x84\xbd\xda\xbf\x64\
\x2e");break;
    case A_RING:         PrintCompLine("\x8b\xe7\x93\xf1\x9c\xa2\x61\x74\x21");break;
    case A_WIND:         PrintCompLine("\x8b\xe7\x93\xf8\xb9\x95\x61\x74\x21");break;
    case A_CLIMB:        PrintCompLine("\x8b\xe7\x93\x63\xf5\x6d\x62\x95\x61\x74\x21");break;
    case A_CLIMBUP:      PrintCompLine("\x8b\xe7\x93\x63\xf5\x6d\x62\x20\x75\x70\x95\x61\x74\x21");             break;
    case A_CLIMBDOWN:    PrintCompLine("\x8b\xe7\x93\x63\xf5\x6d\x62\xcc\xf2\xb4\xa2\x61\x74\x21");           break;
    case A_SLIDEUP:      PrintCompLine("\x8b\xe7\x93\x73\xf5\xe8\x20\x75\x70\x95\x61\x74\x21");             break;
    case A_SLIDEDOWN:    PrintCompLine("\x8b\xe7\x93\x73\xf5\xe8\xcc\xf2\xb4\xa2\x61\x74\x21");           break;
    case A_CLIMBTHROUGH: PrintCompLine("\x8b\xe7\x93\x63\xf5\x6d\x62\x95\xc2\x75\x67\xde\xa2\x61\x74\x21");        break;
    case A_LISTENTO:     PrintCompLine("\x49\xa6\x6d\x61\x6b\xbe\xe4\xba\x73\xa5\x6e\x64\x2e");                   break;
    case A_CROSS:        PrintCompLine("\x8b\xe7\x93\x63\xc2\x73\xa1\xa2\x61\x74\x21");break;
    case A_EXORCISE:     PrintCompLine("\x57\xcd\xa6\xd0\x62\x69\x7a\xbb\xa9\xb3\xca\x63\x65\x70\x74\x21");              break;
    case A_SMELL:        PrintCompLine("\x49\xa6\x73\x6d\x65\xdf\xa1\xe0\x86\xb7\xa5\x6c\xab\x65\x78\xfc\x63\x74\x2e");       break;

    default:             PrintCompLine("\xbc\xaf\xb7\xa5\x6c\xab\xef\xc6\xf7\x69\x6c\x65\x2e");                break;
  }
}

void DoActionOnObject_ext(int action)
{
  int must_hold = 0, obj, i;

  /* eat and drink must-hold checks must be handled by called function*/
  switch (action)
  {
    case A_EMPTY:
    case A_WEAR:
    case A_REMOVE:
    case A_PLAY:
    case A_RING:
    case A_WIND:
    case A_WAVE:
      must_hold = 1;
    break;
  }

  switch (action)
  {
    case A_ENTER:
    case A_EXIT:
      MatchCurWord("of"); /* "go inside of"  "get out of"*/
    break;
  }

  obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) ReturnOverlay1(0);

  switch (action)
  {
    case A_WEAR:   MatchCurWord("on");  break; /* "put obj on"*/
    case A_REMOVE: MatchCurWord("off"); break; /* "take obj off"*/
  }

  if (obj == F_SCENERY_NOTVIS || obj == F_NOTVIS)
    {PrintCompLine("\xbc\xaf\xa8\x73\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); ReturnOverlay1(0);}

  if (obj == F_AMB)
    {PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63\x2e");
     ReturnOverlay1(0);}

  if (must_hold && obj == O_YOU)
    {PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21"); ReturnOverlay1(0);}

  if (obj > 1 && obj < NUM_OBJECTS)
  {
    if (IsObjVisible(obj) == 0)
      {PrintCompLine("\x8b\xe7\x93\xd6\x9e\xa2\xaf\xc0\xac\x65\x2e"); ReturnOverlay1(0);}

    if (must_hold && Obj[obj].loc != INSIDE + O_YOU)
      {PrintCompLine("\x8b\xbb\xd4\x27\xa6\x68\x6f\x6c\x64\x84\x69\x74\x2e"); ReturnOverlay1(0);}
  }

  for (i=0; DoMisc[i].f != 0; i++)
    if (DoMisc[i].action == action && DoMisc[i].obj == obj)
  {
    /* DoMisc[i].f(); */ 
    CallOverlay0((void*)DoMisc[i].f, OVR_DO_MISC);
    ReturnOverlay1(0);
  }

  if (obj == O_YOU)
    {PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21"); ReturnOverlay1(0);}

  if (obj > 1 && obj < NUM_OBJECTS)
    switch (action)
  {
    case A_OPEN:   OpenObj(obj);   ReturnOverlay1(0);
    case A_CLOSE:  CloseObj(obj);  ReturnOverlay1(0);
    case A_LOOKIN: LookInObj(obj); ReturnOverlay1(0);
    case A_EMPTY:  EmptyObj(obj);  ReturnOverlay1(0);
  }

  PrintCantAction(action);

  ReturnOverlay1(0);
}
