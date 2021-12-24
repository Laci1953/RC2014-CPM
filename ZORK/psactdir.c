#include <string.h>
#include <ovr53.h>
#include <ovr54.h>
#include <ovr55.h>

/* defines, enums ------------------------------------------------------------- */
#define AllocBuf 0xBB00

#define OVR_DO_MISC			41
#define OVR_DO_MISC_TO			50
#define OVR_DO_PARSE_ACTION_EX1		53
#define OVR_DO_PARSE_ACTION_EX2		54
#define OVR_DO_PARSE_ACTION_EX3		55

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
#define A_OPEN 64     
#define A_CLOSE 22       
#define A_LOOKIN 55    
#define A_EXAMINE 37
#define A_WAVE 101
 
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

short xrnd(void);

short CallOverlay1(void* Routine, short Par1, char CalledRamBank);
short CallOverlay0(void* Routine, char CalledRamBank);
void ReturnOverlay1(short);
void ReturnOverlay0(short);
void ReturnOverlay0_(short);
/* variables ----------------------------------------------------------------------- */

/* overlays ------------------------------------------------------------------------ */
/* ---
void DoActionOnObject(int action);
void ParseActionTake(void);
void ParseActionDropPut(int put_flag);
void ParseActionThrow(void);

#define DoActionOnObject_ext 0
#define ParseActionTake_ext 0
#define ParseActionDropPut_ext 0
#define ParseActionThrow_ext 0
--- */
/* code ---------------------------------------------------------------------------- */

/* handle things like water and boats*/
int ActionDirectionRoutine(int newroom)
{
  if (Room[Obj[O_YOU].loc].prop & R_BODYOFWATER)
  {
    /*move from water to land or water*/

    if ((Room[newroom].prop & R_BODYOFWATER) == 0)
      PrintCompLine("\x85\x6d\x61\x67\x69\x63\xb0\x6f\xaf\xb3\xe1\xbe\x89\xd0\xa9\xc5"
"\xae\xb4\x81\x73\x68\xd3\x65\x2e\x0a");

    Obj[O_INFLATED_BOAT].loc = newroom;

    DownstreamCounter = -1; /* in case of moving to water; start at -1 to account for this turn*/
  }
  else
  {
    /*move from land*/
    if (YouAreInBoat)
    {
      PrintCompLine("\xdc\x75\x27\xdf\xc0\x61\xd7\x89\x67\x65\xa6\xa5\xa6\xdd\x80\xb0\x6f"
"\xaf\xc6\x69\x72\x73\x74\x2e");
      return 1;
    }
  }
  return 0;
}

void ParseActionRestartOrQuit(int action)
{
  for (;;)
  {
    GetWords((action == A_RESTART) ? "Do you want to restart the game? " :
 					"Do you want to quit now? ");

    if (MatchCurWord("y") || MatchCurWord("yes"))
    {
      GameOver = (action == A_RESTART) ? 2 : 1;
      ReturnOverlay1(0);
    }

    if (MatchCurWord("n") || MatchCurWord("no")) ReturnOverlay1(0);

    PrintCompLine("\x50\xcf\xe0\x9e\xad\x73\x77\xac\xc8\xbe\xae\xb6\x6e\x6f\x2e");
  }
}

void ParseActionWhereIs(void)
{
  int obj, i;

  obj = GetAllObjFromInput(Obj[O_YOU].loc);

  if (obj <= 0) ReturnOverlay0(0);
  else if (obj == F_SCENERY_NOTVIS || obj == F_NOTVIS)
    {PrintCompLine("\x8b\x66\xa7\xab\x69\x74\x2e"); ReturnOverlay0(0);}
  else if (obj == F_AMB)
    {PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63\x2e"); 
	ReturnOverlay0(0);}

  for (i=0; DoMisc[i].f != 0; i++)
    if (DoMisc[i].action == A_WHEREIS && DoMisc[i].obj == obj)
      {
	/* DoMisc[i].f(); */ 
        CallOverlay0((void*)DoMisc[i].f, OVR_DO_MISC);
        ReturnOverlay0(0);
      }

  if (obj == O_YOU)
    PrintCompLine("\xdc\x75\x27\xa9\xa3\xc2\xf6\xab\xa0\xa9\xaa\xe1\x65\x77\xa0\xa9\x2e\x2e\x2e");
  else if (obj >= NUM_OBJECTS)
    PrintCompLine("\x49\x74\x27\xa1\xf1\x67\x68\xa6\xa0\x72\x65\x2e");
  else if (IsObjVisible(obj))
  {
    if (Obj[obj].loc == INSIDE + O_YOU)
      PrintCompLine("\x8b\xcd\xd7\xa8\x74\x2e");
    else
      PrintCompLine("\x49\x74\x27\xa1\xf1\x67\x68\xa6\xa0\x72\x65\x2e");
  }
  else
    PrintCompLine("\x42\xbf\x74\xa1\x6d\x65\x2e");

  ReturnOverlay0(0);
}


void ParseActionExamine(void)
{
  int obj, i, flag;

  obj = GetAllObjFromInput(Obj[O_YOU].loc);
  if (obj <= 0) ReturnOverlay0(0);

  if (obj == F_SCENERY_NOTVIS || obj == F_NOTVIS)
    {PrintCompLine("\xbc\xaf\xa8\x73\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); ReturnOverlay0(0);}

  if (obj == F_AMB)
    {PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63\x2e"); 
    ReturnOverlay0(0);}

  /*fixed objects only*/
  for (i=0; DoMisc[i].f != 0; i++)
    if (DoMisc[i].action == A_EXAMINE && DoMisc[i].obj >= NUM_OBJECTS && DoMisc[i].obj == obj)
  {
    /* DoMisc[i].f(); */ 
    CallOverlay0((void*)DoMisc[i].f, OVR_DO_MISC);
    ReturnOverlay0(0);
  }

  if (obj == O_YOU)
    {PrintCompLine("\x8b\xd9\x6f\x6b\xc6\x61\x69\x72\xec\xae\x72\x64\xa7\xbb\x79\x2e"); 
     ReturnOverlay0(0);}

  if (obj >= NUM_OBJECTS)
    {PrintCompLine("\x8b\x64\xca\x27\xa6\xd6\x9e\xad\x79\xa2\x84\xf6\xfe\x75\x61\x6c\x2e"); 
     ReturnOverlay0(0);}

  if (IsObjVisible(obj) == 0)
  {
    TimePassed = 0;
    PrintCompLine("\x8b\xe7\x93\xd6\x9e\xa2\xaf\xc0\xac\x65\x2e");
    ReturnOverlay0(0);
  }

  /*non-fixed objects only*/
  for (i=0; DoMisc[i].f != 0; i++)
    if (DoMisc[i].action == A_EXAMINE && DoMisc[i].obj < NUM_OBJECTS && DoMisc[i].obj == obj)
  {
    /* DoMisc[i].f(); */ 
    CallOverlay0((void*)DoMisc[i].f, OVR_DO_MISC);
    ReturnOverlay0(0);
  }

  flag = 0;

  if (Obj[obj].prop & PROP_LIT)
  {
    flag = 1;
    PrintCompLine("\x49\x74\x27\xa1\xf5\x74\x2e");
  }

  if (Obj[obj].prop & PROP_OPENABLE)
  {
    flag = 1;
    if (Obj[obj].prop & PROP_OPEN)
      PrintCompLine("\x49\x74\x27\xa1\x6f\xfc\x6e\x2e");
    else
      PrintCompLine("\x49\x74\x27\xa1\x63\xd9\xd6\x64\x2e");
  }

  if ((Obj[obj].prop & PROP_OPEN) &&
      (Obj[obj].prop & PROP_ACTOR) == 0)
  {
    flag = 1;
    PrintContents(obj, "It contains:", 1); /* 1: allow printing "It's empty."*/
  }

  if (flag == 0)
    PrintCompLine("\x8b\x64\xca\x27\xa6\xd6\x9e\xad\x79\xa2\x84\xf6\xfe\x75\x61\x6c\x2e");

  ReturnOverlay0(0);
}

/* called function must check if player is holding obj*/
void ParseActionGive(void)
{
  int obj, to, flag, swap, i;

  obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) ReturnOverlay0(0);
  flag = MatchCurWord("to");
  to = GetAllObjFromInput(Obj[O_YOU].loc); if (to <= 0) ReturnOverlay0(0);

  if (flag == 0)
  {
    /*if "to" omitted, swap obj and to, as in "give plant water"*/
    swap = obj;
    obj = to;
    to = swap;
  }

  if (obj == F_SCENERY_NOTVIS || obj == F_NOTVIS)
    {PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); ReturnOverlay0(0);}

  if (obj == F_AMB)
    {PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63"
"\xa3\x62\xa5\xa6\xaf\xcb\xbf\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x2e"); ReturnOverlay0(0);}

  if (obj == O_YOU)
    {PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21"); ReturnOverlay0(0);}

  if (obj >= NUM_OBJECTS)
    {PrintCompLine("\x8b\xbb\xd4\x27\xa6\x68\x6f\x6c\x64\x84\xa2\x61\x74\x21"); ReturnOverlay0(0);}


  if (to == F_SCENERY_NOTVIS || to == F_NOTVIS)
    {PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); ReturnOverlay0(0);}

  if (to == F_AMB)
    {PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63"
"\xa3\x62\xa5\xa6\xaf\xcb\xbf\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x2e"); ReturnOverlay0(0);}

  if (to == O_YOU)
    {PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21"); ReturnOverlay0(0);}

  if (to > 0 && to < NUM_OBJECTS && IsObjVisible(to) == 0)
    {PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); ReturnOverlay0(0);}


  for (i=0; DoMiscGiveTo[i].f != 0; i++)
    if (DoMiscGiveTo[i].to == to)
  {
    /* DoMiscGiveTo[i].f(obj); */
    CallOverlay1((void*)DoMiscGiveTo[i].f, obj, OVR_DO_MISC_TO);
    ReturnOverlay0(0);
  }

  PrintCompLine("\x8b\xe7\x93\x67\x69\xd7\xaa\xe1\x65\xa2\x84\xbd\x95\x61\x74\x21");

  ReturnOverlay0(0);
}

/*set TimePassed to 1 when an action completes successfully*/
void ParseActionDirection(int action)
{
  unsigned int newroom;
  int prev_darkness;

  newroom = RoomPassages[Obj[O_YOU].loc].passage[action - A_NORTH];

  if (newroom >= BL0)
  {
    PrintBlockMsg(newroom);
    ReturnOverlay1(0); /* return; */
  }

  /* handle things like water and boats in game-specific code*/
  if (ActionDirectionRoutine(newroom)) 
    ReturnOverlay1(0); /* return; */

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
"\xcb\xd8\x6b\x84\x67\x72\x75\
\x65\x21");
      YoureDead(); /* ##### RIP #####*/
      ReturnOverlay1(0); /* return; */
    }
    else PrintCompLine("\x8b\xcd\xd7\xee\x6f\xd7\xab\xa7\xbd\xa3\xcc\xbb\x6b\xeb\xfd\x63\x65\x2e");
  }

  PrintPlayerRoomDesc(0);

  ReturnOverlay1(0); 
}

void DoActionOnObject(int action)
{
  CallOverlay1((void*)DoActionOnObject_ext, action, OVR_DO_PARSE_ACTION_EX3);
  ReturnOverlay1(0);
}

void ParseActionTake(void)
{
  CallOverlay0((void*)ParseActionTake_ext, OVR_DO_PARSE_ACTION_EX2);
  ReturnOverlay0_(0);
}

void ParseActionDropPut(int put_flag)
{
  CallOverlay1((void*)ParseActionDropPut_ext, put_flag, OVR_DO_PARSE_ACTION_EX1);
  ReturnOverlay1(0);
}

void ParseActionThrow(void)
{
  CallOverlay0((void*)ParseActionThrow_ext, OVR_DO_PARSE_ACTION_EX3);
  ReturnOverlay0_(0);
}

