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

extern unsigned char VillainAttacking[];

extern int NumStrWords;
extern int CurWord;

extern char *StrWord[];

extern int ItObj, PrevItObj;

extern unsigned char TimePassed;
extern unsigned char RopeTiedToRail;
extern unsigned char YouAreInBoat;
extern int DownstreamCounter;
extern int ThiefDescType;
extern int LoadAllowed;

/* ----------------------- extern functions -----------------*/
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

/* ----------------------- code ----------------------- */


void MoveTreasuresToLandOfLivingDead(int loc)
{
  int obj;

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == loc &&
        (Obj[obj].prop & PROP_NODESC) == 0 &&
        (Obj[obj].prop & PROP_SACRED) == 0 &&
        Obj[obj].thiefvalue > 0)
  {
    Obj[obj].loc = R_LAND_OF_LIVING_DEAD;
    Obj[obj].prop |= PROP_MOVEDDESC;
  }
}



/* returns 1 if intercepted*/

int InterceptTakeFixedObj(int obj)
{
  switch (obj)
  {
    case F_BOARD:        PrintCompLine("\x85\x62\x6f\xbb\x64\xa1\xbb\x9e\xd6\x63"
"\xd8\x65\xec\xc6\xe0\xd1\xed\x64\x2e");                          return 1;
    case F_SONGBIRD:     PrintCompLine("\x85\x73\xca\x67\x62\x69\x72\xab\x9a\xe3"
"\xa6\xa0\xa9\xb0\xf7\x87\x70\xc2\x62\x61\x62\xec\xe4\xbf\x72\x62\x79\x2e");           return 1;
    case F_BODIES:       PrintCompLine("\x41\xc6\xd3\x63\x9e\x6b\xf3\x70\xa1\x8f"
"\x66\xc2\xf9\x74\x61\x6b\x84\x81\x62\x6f\x64\x69\x65\x73\x2e");                  return 1;
    case F_RUG:          PrintCompLine("\x85\x72\x75\xc1\x9a\x65\x78\x74\xa9\x6d"
"\x65\xec\xc0\xbf\x76\xc4\x8c\xe7\x6e\xe3\xa6\xef\xb3\xbb\xf1\x65\x64\x2e");          return 1;
    case F_NAILS:        PrintCompLine("\x85\x6e\x61\x69\x6c\x73\xb5\xe8\x65\x70"
"\xec\xa8\x6d\xef\x64\xe8\xab\xa7\x80\xcc\xe9\x72\xb5\xe7\x6e\xe3\xa6\xef\xda\x65"
"\x6d\x6f\xd7\x64\x2e"); return 1;
    case F_GRANITE_WALL: PrintCompLine("\x49\x74\x27\xa1\x73\x6f\xf5\xab\x67\xf4"
"\x6e\xc7\x65\x2e");                                        return 1;
    case F_CHAIN:        PrintCompLine("\x85\xfa\x61\xa7\x87\xd6\x63\xd8\x65\x2e");                                       return 1;

    case F_BOLT:
    case F_BUBBLE:
      PrintCompLine("\x49\xa6\x9a\xad\xa8\xe5\x65\x67\xf4\xea\x70\xbb\xa6\xdd\x80"
"\xb3\xca\x74\xc2\xea\x70\xad\x65\x6c\x2e");
      return 1;

    case F_MIRROR2:
    case F_MIRROR1:
      PrintCompLine("\x85\x6d\x69\x72\xc2\xb6\x9a\x6d\xad\xc4\xf0\x6d\xbe\x86\xb6"
"\x73\x69\x7a\x65\xa4\x47\x69\xd7\x20\x75\x70\x2e");
      return 1;

    case F_BONES:
      PrintCompLine("\x41\xe6\x68\x6f\xc5\xa3\x70\xfc\xbb\xa1\xa7\x80\xda\xe9\xf9"
"\x8c\x9a\x61\x70\x70\xe2\xcf\xab\xaf\x86\xb6\xe8\xd6\x63\xf4\xf0\xca\x8a\x81\xa9"
"\x6d\x61\xa7\xa1\xdd\xa3\xc6\x65\xdf\xf2\xa3\x64\xd7\xe5\xd8\xac\xa4\x48\x9e\xe7"
"\xc5\xa1\xd0\x63\xd8\xd6\xae\xb4\x92\x20\x76\xe2\x75\x61\x62\xcf\xa1\x8c\x62\xad"
"\xb2\xa0\xa1\x96\xf9\xbd\x80\x20\x4c\x8c\xdd\x80\x20\x4c\x69\x76\x84\x44\xbf\x64"
"\x83\x9e\x67\x68\x6f\xc5\xcb\xbf\xd7\x73\xb5\x6d\xf7\xd1\xf1\x9c\x6f\x62\x73\x63"
"\xd4\xc7\x69\x65\x73\x2e");
      MoveTreasuresToLandOfLivingDead(Obj[O_YOU].loc);
      MoveTreasuresToLandOfLivingDead(INSIDE + O_YOU);
      return 1;
  }

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



/* test flag:  1 if no changes should be made (yet)*/
/* multi flag: 1 if obj name should be printed*/

/* returns:*/
/*   1:  intercepted, and obj MUST leave inventory, unless container is full*/
/*  -1:  intercepted and calling function should immediately return*/

int InterceptDropPutObj(int obj, int container, int test, int multi)
{
  switch (container)
  {
    case O_LOWERED_BASKET:
    {
      if (multi) {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
      PrintCompLine("\x85\x62\xe0\x6b\x65\xa6\x9a\xaf\x80\xae\x96\xb6\xd4\xab"
"\xdd\x80\xaa\xcd\x66\x74\x2e");
      return -1;
    }

    case O_CHALICE:
    {
      if (multi) {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
      PrintCompLine("\x8b\xe7\x6e\x27\x74\xa4\x49\x74\x27\xa1\xe3\xa6\xd0\xd7"
"\x72\xc4\x67\xe9\xab\xfa\xe2\x69\x63\x65\xb5\x9a\x69\x74\x3f");
      return -1;
    }

    case O_LARGE_BAG:
    {
      if (multi) {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
      PrintCompLine("\x49\xa6\x77\xa5\x6c\xab\xef\xa3\xe6\xe9\xab\x74\xf1\x63\x6b\x2e");
      return -1;
    }

    case F_GRATE:
    {
      if (Obj[obj].size > 20)
      {
        if (multi) {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
        PrintCompLine("\x49\xa6\x77\xca\x27\xa6\x66\xc7\x95\xc2\x75\x67\xde\x81"
"\x67\xf4\xf0\x6e\x67\x2e");
        return -1;
      }
      else if (Obj[O_YOU].loc != R_GRATING_CLEARING)
      {
        if (multi) {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
        PrintCompLine("\x49\xa6\x77\xca\x27\xa6\x67\xba\xa2\xc2\x75\x67\x68\x2e");
        return -1;
      }    

      if (test == 0)
      {
        if (multi) {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
        PrintCompLine("\x49\xa6\x67\x6f\xbe\x95\xc2\x75\x67\xde\x81\x67\xf4\xf0"
"\x9c\xa7\xbd\x80\xcc\xbb\x6b\xed\x73\xa1\xef\xd9\x77\x2e");

        Obj[obj].loc = R_GRATING_ROOM;
        MoveObjOrderToLast(obj);
        TimePassed = 1;
      }

      return 1;
    }

    case F_SLIDE:
    {
      if (test == 0)
      {
        if (multi) {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
        if (Obj[O_YOU].loc == R_SLIDE_ROOM)
          PrintCompLine("\x49\xa6\x66\xe2\x6c\xa1\xa7\xbd\x80\xaa\xf5\xe8\x8d\x87\x67\xca\x65\x2e");
        else
          PrintCompLine("\x49\xa6\x66\xe2\x6c\xa1\xa7\xbd\x80\xaa\xf5\x64\x65\x2e");
  
        Obj[obj].loc = R_CELLAR;
        MoveObjOrderToLast(obj);
        TimePassed = 1;
      }

      return 1;
    }

    case F_RIVER:
    case O_WATER:
    {
      if ((Room[Obj[O_YOU].loc].prop & R_WATERHERE) == 0)
      {
        if (multi) {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
        PrintCompLine("\x99\xa9\xa8\x73\x93\xad\xc4\x77\xaf\xac\xc0\xac\x65\x21");
        return -1;
      }

      if (obj == O_INFLATED_BOAT)
      {
        if (multi) {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
        PrintCompLine("\x8b\x73\x68\xa5\x6c\xab\x67\x65\xa6\xa7\x80\xb0\x6f\xaf"
"\x80\xb4\xfd\xf6\xfa\xa8\x74\x2e");
        return -1;
      }

      if (test == 0)
      {
        if (multi) {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
        if (Obj[obj].prop & PROP_INFLAMMABLE)
          PrintCompLine("\x49\xa6\x66\xd9\xaf\xa1\x66\xd3\xa3\xee\xe1\xd4\x74"
"\xb5\x96\xb4\x73\xa7\x6b\x73\x2e");
        else
          PrintCompLine("\x49\xa6\x73\x70\xfd\x73\xa0\xa1\xa7\xbd\x80\xb7\xaf"
"\xac\x8d\x87\x67\xca\x9e\x66\xd3\x65\xd7\x72\x2e");

        Obj[obj].loc = 0;
        TimePassed = 1;
      }

      return 1;
    }
  }

  if (container >= NUM_OBJECTS)
  {
    PrintCompLine("\x8b\xe7\x93\x70\xf7\xa3\x6e\x79\xa2\x84\xa7\xbd\x95\x61\x74\x21");
    return -1;
  }

  return 0; /* not intercepted*/
}


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
int GetPlayersVehicle(void)
{
  if (YouAreInBoat)
    return O_INFLATED_BOAT;
  else
    return 0;
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

int TakeOutOfRoutine(int *container)
{
  MatchCurWord("of");

  *container = GetAllObjFromInput(Obj[O_YOU].loc); if ((*container) <= 0) return 1;

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



/*returns 1 to signal to calling routine that command was processed*/
/*container may be 0 (none specified)*/

int VerifyTakeableObj(int obj, int container, int num_takes)
{
  if (obj == F_SCENERY_NOTVIS || obj == F_NOTVIS)
  {
    if (num_takes > 1)
      PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a"
"\x65\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21");
    else
      PrintCompLine("\x8b\x64\xca\x27\xa6\xd6\x9e\xa2\xaf\xc0\xac\x65\x21");

    return 1;
  }

  if (obj == F_AMB)
  {
    if (num_takes > 1)
      PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69"
"\x63\xa3\x62\xa5\xa6\xaf\xcb\xbf\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x2e");
    else
      PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63\x2e");

    return 1;
  }

  if (InterceptTakeFixedObj(obj)) return 1;

  if (obj == O_YOU || obj >= NUM_OBJECTS)
  {
    if (num_takes > 1)
      PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a"
"\x65\x63\x74\xa1\xe7\x93\xef\x9f\x61\x6b\x65\x6e\x21");
    else
      PrintCompLine("\x8b\xe7\x93\x74\x61\x6b\x9e\xa2\x61\x74\x21");

    return 1;
  }

  if (GetPlayersVehicle() == obj)
  {
    PrintObjectDesc(obj, 0);
    PrintCompLine("\x3a\x88\x27\xdf\xc0\x61\xd7\x89\x67\x65\xa6\xa5\xa6\xdd\xa8\xa6\x66\x69\x72\x73\x74\x21");
    return 1;
  }

  if (container != 0 &&
      Obj[obj].loc != INSIDE + container &&
      container != GetPlayersVehicle())
  {
    PrintObjectDesc(obj, 0);

    if (Obj[container].prop & PROP_SURFACE)
      PrintCompLine("\x3a\x98\xaf\x27\xa1\xe3\xa6\xca\xa8\x74\x2e");
    else
      PrintCompLine("\x3a\x98\xaf\x27\xa1\xe3\xa6\xa7\x73\x69\xe8\x8a\x69\x74\x2e");

    return 1;
  }

  return 0;
}
