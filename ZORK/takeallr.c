#include <string.h>

/* defines, enums ------------------------------------------------------------- */

#define AllocBuf 0xBB00

#define NUM_ROOMS  111 /* including null room 0 */

#define NUM_OBJECTS  80

#define LOAD_MAX  100

#define INSIDE  2048

#define MAX_INVENTORY_ITEMS  7
#define INV_LIMIT_CHANCE     8

/*room properties bit flags*/

#define R_DESCRIBED     1  /* set when room description already printed */
#define R_BODYOFWATER   2
#define R_LIT           4  /* set when there is natural light or a light fixture */
#define R_WATERHERE     8
#define R_SACRED       16  /* set when thief not allowed in room */
#define R_MAZE         32
#define R_ALWAYSDESC   64

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
  VILLAIN_TROLL,
  VILLAIN_THIEF,
  VILLAIN_CYCLOPS,

  NUM_VILLAINS
};

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

/* extern data structures ---------------------------------------------------------- */
struct ROOM_STRUCT
{
   char *scenery;
  unsigned short      prop;
   unsigned short init_prop;
};
extern struct ROOM_STRUCT Room[];

struct OBJ_STRUCT
{
   unsigned short init_loc;
  unsigned short            loc;
   unsigned short size;
   unsigned short capacity;
  unsigned short order;
  unsigned short prop;
   unsigned char init_thiefvalue;
  unsigned char            thiefvalue;
};
extern struct OBJ_STRUCT Obj[];

extern int NumStrWords;
extern int CurWord;

extern int ItObj, PrevItObj;

extern unsigned char TimePassed;
extern unsigned char RopeTiedToRail;
extern unsigned char YouAreInBoat;
extern int DownstreamCounter;
extern int ThiefDescType;
extern int LoadAllowed;
extern unsigned char VillainAttacking[];
/* ----------------------- extern functions -----------------*/
int IsObjVisible(int obj);
int IsObjCarriedByActor(int obj);
int IsPlayerInDarkness(void);
void PrintObjectDesc(int obj, int desc_flag);
void PrintContents(int obj, char *heading, int print_empty);
void PrintPresentObjects(int location, char *heading, int list_flag);
void PrintRoomDesc(int room, int force_description);
void PrintPlayerRoomDesc(int force_description);

int GetAllObjFromInput(int room);
int GetNumObjectsInLocation(int loc);
int GetTotalSizeInLocation(int loc);
void MoveObjOrderToLast(int obj);
int MatchCurWord( char *match);
int PercentChance(int x, int x_not_lucky);

int InterceptTakeFixedObj(int obj);
int InterceptTakeOutOf(int container);
int InterceptDropPutObj(int obj, int container, int test, int multi);
int InterceptTakeObj(int obj);
int GetPlayersVehicle(void);
void PrintCantAction(int action);
int VerifyTakeableObj(int obj, int container, int num_takes);
int TakeRoutine(int obj, char *msg);
int VerifyTakeAllExceptObj(int obj);
int DropPutVerifyContainer(int container);
void DropPutAllRoutine(int put_flag);
void MoveTreasuresToLandOfLivingDead(int loc);

short CallOverlay0(void*, char);
short CallOverlay1(void*, short, char);

void ReturnOverlay0(short);
void ReturnOverlay1(short);

/* ----------------------- code ----------------------- */
/* returns 0 if take should go ahead*/
int InterceptTakeObj(int obj)
{
  switch (obj)
  {
    case O_BAT:           PrintCompLine("\x8b\xe7\x93\xa9\x61\xfa\xc0\x69\x6d"
"\x3b\xc0\x65\x27\xa1\xca\x80\xb3\x65\x69\xf5\x6e\x67\x2e"); return 1;
    case O_CYCLOPS:       PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x64\x6f"
"\xbe\x93\x74\x61\x6b\x9e\x6b\xa7\x64\xec\x89\xef\x84\x67\xf4\x62\xef\x64\x2e"); TimePassed = 1; return 1;
    case O_THIEF:         PrintCompLine("\x4f\x6e\x63\x9e\x8f\x67\xff\xc0\x69"
"\x6d\xb5\x77\xcd\xa6\x77\xa5\x6c\xab\x8f\x64\xba\xf8\xa2\xc0\x69\x6d\x3f"); return 1;
    case O_TROLL:         PrintCompLine("\x85\x74\xc2\xdf\xaa\x70\xc7\xa1\xa7"
"\x86\xb6\x66\x61\x63\x65\xb5\x67\x72\xf6\xf0\x9c\x22\x42\x65\x74\xd1\xb6\x6c"
"\x75\x63\x6b\xe4\x65\x78\xa6\xf0\x6d\x65\x22\xa8\xb4\xd0\xf4\x96\xb6\x62\xbb"
"\x62\xbb\xa5\xa1\x61\x63\x63\xd4\x74\x2e"); TimePassed = 1; return 1;
    case O_MACHINE:       PrintCompLine("\x49\xa6\x9a\x66\xbb\x9f\xe9\xcb\xbb"
"\x67\x9e\xbd\xb3\xbb\x72\x79\x2e"); return 1;
    case O_TROPHY_CASE:   PrintCompLine("\x85\x74\xc2\x70\x68\xc4\xe7\xd6\x87"
"\xd6\x63\xd8\x65\xec\xc6\xe0\xd1\xed\xab\xbd\x80\xb7\xe2\x6c\x2e"); return 1;
    case O_MAILBOX:       PrintCompLine("\x49\xa6\x9a\xd6\x63\xd8\x65\xec\xa3"
"\x6e\xfa\xd3\x65\x64\x2e"); return 1;
    case O_KITCHEN_TABLE: PrintCompLine("\x8b\xe7\x93\x74\x61\x6b\x9e\x81\x74"
"\x61\x62\x6c\x65\x2e"); return 1;
    case O_ATTIC_TABLE:   PrintCompLine("\x8b\xe7\x93\x74\x61\x6b\x9e\x81\x74"
"\x61\x62\x6c\x65\x2e"); return 1;
    case O_HOT_BELL:      PrintCompLine("\x85\xef\xdf\x87\xd7\x72\xc4\x68\xff"
"\x8d\x91\xe3\xa6\xef\x9f\x61\x6b\x65\x6e\x2e"); return 1;

    case O_WATER:
      if ((Room[Obj[O_YOU].loc].prop & R_WATERHERE) == 0 &&
           !(IsObjVisible(O_BOTTLE) &&
             (Obj[O_BOTTLE].prop & PROP_OPEN) &&
             Obj[O_WATER].loc == INSIDE + O_BOTTLE))
        PrintCompLine("\x99\xa9\x27\xa1\xe3\xb7\xaf\xac\xc0\xac\x65\x21");
      else
        PrintCompLine("\x85\x77\xaf\xac\xaa\xf5\x70\xa1\xa2\xc2\x75\x67\xde"
"\x92\xc6\x97\xac\x73\x2e");
      return 1;

    case O_TOOL_CHEST:
      PrintCompLine("\x85\xfa\xbe\x74\xa1\xbb\x9e\x73\xba\x72\xfe\x74\xc4\x8c"
"\x63\xd3\xc2\xe8\xab\xa2\xaf\x80\xc4\x63\x72\x75\x6d\x62\xcf\xb7\xa0\xb4\x8f"
"\xbd\x75\xfa\x80\x6d\x2e");
      Obj[O_TOOL_CHEST].loc = 0;
      return 1;

    case O_ROPE:
      if (RopeTiedToRail)
        {PrintCompLine("\x85\xc2\xfc\x87\xf0\xd5\x89\x81\xf4\x69\xf5\x6e\x67\x2e"); return 1;}
    break;

    case O_RUSTY_KNIFE:
      if (Obj[O_SWORD].loc == INSIDE + O_YOU)
        PrintCompLine("\x41\xa1\x8f\xbd\x75\xfa\x80\xda\xfe\x74\xc4\x6b\x6e\x69"
"\x66\x65\xb5\x92\xaa\x77\xd3\xab\x67\x69\xd7\xa1\xd0\x73\x97\xcf\xeb\x75\x6c"
"\xd6\x8a\x62\xf5\xb9\x84\x62\x6c\x75\x9e\xf5\x67\x68\x74\x2e");
    break;

    case O_CHALICE:
      if (Obj[O_CHALICE].loc == R_TREASURE_ROOM &&
          Obj[O_THIEF].loc == R_TREASURE_ROOM &&
          (Obj[O_THIEF].prop & PROP_NODESC) == 0 &&
          VillainAttacking[VILLAIN_THIEF] &&
          ThiefDescType != 1) /* not unconscious*/
        {PrintCompLine("\xdc\x75\x27\xab\xef\xaa\x74\x61\x62\xef\xab\xa7\x80\xb0"
"\x61\x63\x6b\xc6\x69\x72\x73\x74\x2e"); return 1;}
    break;

    case O_LARGE_BAG:
      if (ThiefDescType == 1) /* unconscious*/
        PrintCompLine("\x53\x61\x64\xec\xc6\xd3\x86\xb5\x81\xc2\x62\xef\xb6\x63"
"\x6f\xdf\x61\x70\xd6\xab\xca\x9f\x6f\x70\x8a\x81\x62\x61\x67\x9d\x72\x79\x84"
"\xbd\x9f\x61\x6b\x9e\xc7\xb7\xa5\x6c\xab\x77\x61\x6b\x9e\xce\x6d\x2e");
      else
        PrintCompLine("\x85\x62\x61\xc1\xf8\xdf\xb0\x9e\x74\x61\x6b\xd4\xae\xd7"
"\xb6\xce\xa1\xe8\x61\xab\x62\x6f\x64\x79\x2e");
      return 1;
  }

  return 0;
}

/* if player is inside vehicle, return vehicle obj; otherwise return 0*/
/* returns 1 if take intercepted or failed*/
int TakeRoutine(int obj, char *msg)
{
  int num, chance;

  if (InterceptTakeObj(obj)) return 1;

  /* act as if obj is not present when it is not takeable AND is not described*/
  if ((Obj[obj].prop & PROP_NOTTAKEABLE) &&
      (Obj[obj].prop & PROP_NODESC))
    {PrintCompLine("\x8b\xe7\x93\xd6\x9e\xa2\xaf\xc0\xac\x65\x2e"); return 1;}

  if (Obj[obj].prop & PROP_NOTTAKEABLE)
    {PrintCompLine("\x8b\xe7\x93\x74\x61\x6b\x9e\xa2\x61\x74\x2e"); return 1;}


  if (GetTotalSizeInLocation(INSIDE + O_YOU) + Obj[obj].size > LoadAllowed)
  {
    PrintCompText("\xdc\xd8\xcb\x6f\x61\xab\x9a\xbd\xba\xa0\x61\x76\x79");
    if (LoadAllowed < LOAD_MAX)
      PrintCompLine("\xb5\xbe\xfc\x63\x69\xe2\xec\xa8\xb4\xf5\x67\x68\xa6\xdd"
"\x86\xb6\x63\xca\x64\xc7\x69\x6f\x6e\x2e");
    else
      PrintCompLine("\x2e");
    return 1;
  }

  num = GetNumObjectsInLocation(INSIDE + O_YOU);
  chance = INV_LIMIT_CHANCE * num; if (chance > 100) chance = 100;
  if (num > MAX_INVENTORY_ITEMS && PercentChance(chance, -1))
  {
    PrintCompLine("\xdc\x75\x27\xa9\xc0\x6f\x6c\x64\x84\xbd\xba\x6d\xad\xc4"
"\xa2\x97\xa1\xe2\xa9\x61\x64\x79\x21");
    return 1;
  }


  TimePassed = 1;
  PrintLine(msg);

  Obj[obj].loc = INSIDE + O_YOU;
  Obj[obj].prop |= PROP_MOVEDDESC;

  MoveObjOrderToLast(obj);

  return 0;
}

int GetPlayersVehicle(void)
{
  if (YouAreInBoat)
    return O_INFLATED_BOAT;
  else
    return 0;
}

int InterceptTakeOutOf(int container)
{
  switch (container)
  {
    case O_LARGE_BAG:
    {
      PrintCompLine("\x49\xa6\x77\xa5\x6c\xab\xef\xa3\xe6\xe9\xab\x74\xf1\x63\x6b\x2e");
      return 1;
    }
  }

  return 0;
}

int TakeOutOfRoutine(int *container)
{
  MatchCurWord("of");

  *container = GetAllObjFromInput(Obj[O_YOU].loc); 

  if ((*container) <= 0) ReturnOverlay1(1); /* return 1; */

  if ((*container) == F_SCENERY_NOTVIS || (*container) == F_NOTVIS)
  {
    PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21");
    ReturnOverlay1(1); /* return 1; */
  }

  if ((*container) == O_YOU || (*container) >= NUM_OBJECTS)
  {
    PrintCompLine("\x8b\xe7\x93\x74\x61\x6b\x9e\xad\x79\xa2\x84\xa5\xa6\xdd\x95\x61\x74\x2e");
    ReturnOverlay1(1); /* return 1; */
  }

  if (IsObjVisible(*container) == 0)
  {
    PrintObjectDesc(*container, 0);
    PrintCompLine("\x3a\x88\x91\x27\xa6\xd6\x9e\xa2\xaf\xc0\xac\x65\x2e");
    ReturnOverlay1(1); /* return 1; */
  }

  if (InterceptTakeOutOf(*container)) ReturnOverlay1(1); /* return 1; */

  if ((Obj[*container].prop & PROP_OPEN) == 0 ||
      (Obj[*container].prop & PROP_ACTOR))
  {
    PrintObjectDesc(*container, 0);
    PrintCompText("\x3a\x20");

    if (Obj[*container].prop & PROP_OPENABLE)
    {
      PrintCompLine("\x8b\xed\xd5\x89\x6f\xfc\xb4\xc7\xc6\x69\x72\x73\x74\x2e");
      ItObj = *container;
    }
    else
      PrintCompLine("\x8b\xe7\x93\x74\x61\x6b\x9e\xad\x79\xa2\x84\xa5\xa6\xdd\x95\x61\x74\x21");

    ReturnOverlay1(1); /* return 1; */
  }

  ReturnOverlay1(0); /* return 1; */
}

int TakeOutOfRoutine_loc(int *container)
{
  MatchCurWord("of");

  *container = GetAllObjFromInput(Obj[O_YOU].loc); 

  if ((*container) <= 0) return 1; 

  if ((*container) == F_SCENERY_NOTVIS || (*container) == F_NOTVIS)
  {
    PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21");
    return 1; 
  }

  if ((*container) == O_YOU || (*container) >= NUM_OBJECTS)
  {
    PrintCompLine("\x8b\xe7\x93\x74\x61\x6b\x9e\xad\x79\xa2\x84\xa5\xa6\xdd\x95\x61\x74\x2e");
    return 1;
  }

  if (IsObjVisible(*container) == 0)
  {
    PrintObjectDesc(*container, 0);
    PrintCompLine("\x3a\x88\x91\x27\xa6\xd6\x9e\xa2\xaf\xc0\xac\x65\x2e");
    return 1; 
  }

  if (InterceptTakeOutOf(*container)) return 1;

  if ((Obj[*container].prop & PROP_OPEN) == 0 ||
      (Obj[*container].prop & PROP_ACTOR))
  {
    PrintObjectDesc(*container, 0);
    PrintCompText("\x3a\x20");

    if (Obj[*container].prop & PROP_OPENABLE)
    {
      PrintCompLine("\x8b\xed\xd5\x89\x6f\xfc\xb4\xc7\xc6\x69\x72\x73\x74\x2e");
      ItObj = *container;
    }
    else
      PrintCompLine("\x8b\xe7\x93\x74\x61\x6b\x9e\xad\x79\xa2\x84\xa5\xa6\xdd\x95\x61\x74\x21");

    return 1; 
  }

  return 0; 
}

int VerifyTakeAllExceptObj(int obj)
{
  if (obj == F_SCENERY_NOTVIS || obj == F_NOTVIS)
  {
    PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21");
    return 1;
  }
  else if (obj == F_AMB)
  {
    PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63"
"\xa3\x62\xa5\xa6\xaf\xcb\xbf\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x2e");
    return 1;
  }
  else if (obj == O_YOU || obj >= NUM_OBJECTS)
  {
    PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xe7\x93\xef\x9f\x61\x6b\x65\x6e\x21");
    return 1;
  }
  else if (Obj[obj].loc == INSIDE + O_YOU)
  {
    PrintCompLine("\xdc\x75\x27\xa9\xa3\x6c\xa9\x61\x64\xc4\x68\x6f\x6c\x64\x84"
"\xaf\xcb\xbf\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x21");
    return 1;
  }
  else if (IsObjVisible(obj) == 0)
  {
    PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21");
    return 1;
  }

  return 0;
}

#define AllocBuf1 AllocBuf+80*2
void TakeAllRoutine(void)
{
  int num_exceptions, num_takes, obj, container, i, j;
  unsigned short* exception = (unsigned short*)AllocBuf; /* unsigned short exception[80]; */ /*stores objects not to be taken by "take all"*/
  unsigned short* take = (unsigned short*)AllocBuf1; /* unsigned short take[80]; */      /*stores objects to be taken by "take all"*/

  num_exceptions = 0;
  container = 0;

  if (MatchCurWord("but") || MatchCurWord("except")) /* take all except * (and ...) (from *)*/
  {
    MatchCurWord("for"); /* skip "for" if it exists*/
    for (;;)
    {
      obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) ReturnOverlay0(0); /* return; */
      if (VerifyTakeAllExceptObj(obj)) ReturnOverlay0(0); /* return; */
      exception[num_exceptions++] = obj; 
      if (num_exceptions == 80 || CurWord == NumStrWords) 
        break;
  
      if (MatchCurWord("from") || MatchCurWord("out") || MatchCurWord("off"))
      {
        if (TakeOutOfRoutine_loc(&container)) ReturnOverlay0(0); /* return; */
        break;
      }
  
      if (MatchCurWord("then")) 
      {CurWord--; break;}/*end of this turn's command; back up so "then" can be matched later*/
      if (MatchCurWord("and") == 0)
        {PrintCompLine("\x50\xcf\xe0\x9e\xfe\x9e\xd0\x63\xe1\x6d\xd0\xd3\x80\xb7"
"\xd3\xab\x22\xad\x64\x22\xb0\x65\x74\x77\xf3\xb4\xe3\xf6\x73\x2e"); ReturnOverlay0(0); /* return; */}
      for (;;) if (MatchCurWord("and") == 0)
		 break; /* handle repeated "and"s like "take one, two, and three"*/
    }
  }
  else if (MatchCurWord("from") || MatchCurWord("out") || MatchCurWord("off")) 
  { /* take all from * except * (and ...)*/
    if (TakeOutOfRoutine_loc(&container)) ReturnOverlay0(0); /* return; */

    if (MatchCurWord("but") || MatchCurWord("except"))
    {
      MatchCurWord("for"); /* skip "for" if it exists*/
      for (;;)
      {
        obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) ReturnOverlay0(0); /* return; */
        if (VerifyTakeAllExceptObj(obj)) ReturnOverlay0(0); /* return; */
        exception[num_exceptions++] = obj; if (num_exceptions == 80 || CurWord == NumStrWords)
						 break;
    
        if (MatchCurWord("then")) 
        {CurWord--; break;}/*end of this turn's command;back up so "then" can be matched later*/
        if (MatchCurWord("and") == 0)
          {PrintCompLine("\x50\xcf\xe0\x9e\xfe\x9e\xd0\x63\xe1\x6d\xd0\xd3\x80"
"\xb7\xd3\xab\x22\xad\x64\x22\xb0\x65\x74\x77\xf3\xb4\xe3\xf6\x73\x2e"); ReturnOverlay0(0); /* return; */}
        for (;;) if (MatchCurWord("and") == 0) 
                   break; /* handle repeated "and"s like "take one, two, and three"*/
      }
    }
  }

  if (container == 0 && GetPlayersVehicle() != 0)
    container = GetPlayersVehicle();

  num_takes = 0;

  for (i=2; i<NUM_OBJECTS; i++)
  {
    obj = Obj[i].order;

    if (Obj[obj].loc == (container ? INSIDE + container : Obj[O_YOU].loc) &&
        (Obj[obj].prop & PROP_NOTTAKEABLE) == 0)
    {
      for (j=0; j<num_exceptions; j++)
        if (obj == exception[j]) break;
      if (j == num_exceptions)
      {
        take[num_takes++] = obj;
        if (num_takes == 80) break;
      }
    }
  }

  for (i=0; i<num_takes; i++)
  {
    obj = take[i];

    PrintObjectDesc(obj, 0);
    PrintCompText("\x3a\x20");

    TakeRoutine(obj, "Okay.");
  }

  if (num_takes == 0)
    PrintCompLine("\x99\xa9\xb7\xe0\xe4\xff\xce\x9c\xbd\x9f\x61\x6b\x65\x2e");

  ReturnOverlay0(0);
}
