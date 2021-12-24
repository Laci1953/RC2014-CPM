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
      obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) return;
      if (VerifyTakeAllExceptObj(obj)) return;
      exception[num_exceptions++] = obj; 
      if (num_exceptions == 80 || CurWord == NumStrWords) 
        break;
  
      if (MatchCurWord("from") || MatchCurWord("out") || MatchCurWord("off"))
      {
        if (TakeOutOfRoutine(&container)) return;
        break;
      }
  
      if (MatchCurWord("then")) 
      {CurWord--; break;}/*end of this turn's command; back up so "then" can be matched later*/
      if (MatchCurWord("and") == 0)
        {PrintCompLine("\x50\xcf\xe0\x9e\xfe\x9e\xd0\x63\xe1\x6d\xd0\xd3\x80\xb7"
"\xd3\xab\x22\xad\x64\x22\xb0\x65\x74\x77\xf3\xb4\xe3\xf6\x73\x2e"); return;}
      for (;;) if (MatchCurWord("and") == 0)
		 break; /* handle repeated "and"s like "take one, two, and three"*/
    }
  }
  else if (MatchCurWord("from") || MatchCurWord("out") || MatchCurWord("off")) 
  { /* take all from * except * (and ...)*/
    if (TakeOutOfRoutine(&container)) return;

    if (MatchCurWord("but") || MatchCurWord("except"))
    {
      MatchCurWord("for"); /* skip "for" if it exists*/
      for (;;)
      {
        obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) return;
        if (VerifyTakeAllExceptObj(obj)) return;
        exception[num_exceptions++] = obj; if (num_exceptions == 80 || CurWord == NumStrWords)
						 break;
    
        if (MatchCurWord("then")) 
        {CurWord--; break;}/*end of this turn's command;back up so "then" can be matched later*/
        if (MatchCurWord("and") == 0)
          {PrintCompLine("\x50\xcf\xe0\x9e\xfe\x9e\xd0\x63\xe1\x6d\xd0\xd3\x80"
"\xb7\xd3\xab\x22\xad\x64\x22\xb0\x65\x74\x77\xf3\xb4\xe3\xf6\x73\x2e"); return;}
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
}

int DropPutVerifyContainer(int container)
{
  if (container == F_SCENERY_NOTVIS || container == F_NOTVIS)
  {
    PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65"
"\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21");
    return 1;
  }

  if (container == O_YOU)
  {
    PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21");
    return 1;
  }

  if (container < NUM_OBJECTS)
  {
    if (IsObjVisible(container) == 0)
    {
      PrintObjectDesc(container, 0);
      PrintCompLine("\x3a\x88\x91\x27\xa6\xd6\x9e\xa2\xaf\xc0\xac\x65\x2e");
      return 1;
    }

    if ((Obj[container].prop & PROP_OPEN) == 0 ||
        (Obj[container].prop & PROP_ACTOR))
    {
      PrintObjectDesc(container, 0);
      PrintCompText("\x3a\x20");

      if (Obj[container].prop & PROP_OPENABLE)
      {
        PrintCompLine("\x8b\xed\xd5\x89\x6f\xfc\xb4\xc7\xc6\x69\x72\x73\x74\x2e");
        ItObj = container;
      }
      else
        PrintCompLine("\x8b\xe7\x93\x70\xf7\xa3\x6e\x79\xa2\x84\xa7\xbd\x95\x61\x74\x21");

      return 1;
    }
  }

  return 0;
}

void DropPutAllRoutine(int put_flag)
{
  int i, j, obj, num_exceptions, nothing_dropped, into_flag, outside_flag;
  int container, prev_darkness;
  unsigned short* exception = (unsigned short*)AllocBuf; /* unsigned short exception[80]; */ /*stores objects not to be dropped/put by "drop/put all"*/

  num_exceptions = 0;
  container = 0;
  into_flag = 0;
  outside_flag = 0;

  if (MatchCurWord("but") || MatchCurWord("except"))
  {
    MatchCurWord("for"); /* skip "for" if it exists*/
    for (;;)
    {
      obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) return;
      if (obj == O_YOU || obj >= NUM_OBJECTS)
        {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\xaf\xcb\xbf"
"\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x21"); return;}
      if (Obj[obj].loc != INSIDE + O_YOU)
        {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\xaf\xcb\xbf"
"\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x21"); return;}
      exception[num_exceptions++] = obj; if (num_exceptions == 80 || CurWord == NumStrWords) break;

      if (MatchCurWord("in") || MatchCurWord("into") || MatchCurWord("inside") || 
		MatchCurWord("through") || MatchCurWord("on") || MatchCurWord("onto"))
        into_flag = 1;
      else if (MatchCurWord("outside"))
        outside_flag = 1;
      if (into_flag || outside_flag)
      {
        put_flag = 1; /* change "drop" to "put" (if not already)*/
        container = GetAllObjFromInput(Obj[O_YOU].loc); if (container <= 0) return;
        if (DropPutVerifyContainer(container)) return;
        break;
      }

      if (MatchCurWord("then")) 
      {CurWord--; break;} /*end of this turn's command; back up so "then" can be matched later*/
      if (MatchCurWord("and") == 0)
        {PrintCompLine("\x50\xcf\xe0\x9e\xfe\x9e\xd0\x63\xe1\x6d\xd0\xd3\x80"
"\xb7\xd3\xab\x22\xad\x64\x22\xb0\x65\x74\x77\xf3\xb4\xe3\xf6\x73\x2e"); return;}
      for (;;) if (MatchCurWord("and") == 0) 
                 break; /* handle repeated "and"s like "take one, two, and three"*/
    }
  }
  else
  {
    if (MatchCurWord("in") || MatchCurWord("into") || MatchCurWord("inside")
 	|| MatchCurWord("through") || MatchCurWord("on") || MatchCurWord("onto"))
      into_flag = 1;
    else if (MatchCurWord("outside"))
      outside_flag = 1;
    if (into_flag || outside_flag)
    {
      put_flag = 1; /* change "drop" to "put" (if not already)*/
      container = GetAllObjFromInput(Obj[O_YOU].loc); if (container <= 0) return;
      if (DropPutVerifyContainer(container)) return;

      if (MatchCurWord("but") || MatchCurWord("except"))
      {
        MatchCurWord("for"); /* skip "for" if it exists*/
        for (;;)
        {
          obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) return;
          if (obj == O_YOU || obj >= NUM_OBJECTS)
            {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\xaf"
"\xcb\xbf\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x21"); return;}
          if (Obj[obj].loc != INSIDE + O_YOU)
            {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\xaf"
"\xcb\xbf\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x21"); return;}
          exception[num_exceptions++] = obj; if (num_exceptions == 80 || CurWord == NumStrWords)
						 break;
    
          if (MatchCurWord("then")) {CurWord--; break;} /*end of this turn's command; back up so "then" can be matched later*/
          if (MatchCurWord("and") == 0)
            {PrintCompLine("\x50\xcf\xe0\x9e\xfe\x9e\xd0\x63\xe1\x6d\xd0\xd3"
"\x80\xb7\xd3\xab\x22\xad\x64\x22\xb0\x65\x74\x77\xf3\xb4\xe3\xf6\x73\x2e"); return;}
          for (;;) if (MatchCurWord("and") == 0) 
			break; /* handle repeated "and"s like "take one, two, and three"*/
        }
      }
    }
  }

  if (put_flag)
  {
    if (into_flag == 0 && outside_flag == 0)
      {PrintCompLine("\x8b\xed\xd5\x89\x73\xfc\x63\x69\x66\xc4\xd0\x63\xca\x74\x61\xa7\x65\x72\x2e"); return;}

    if (outside_flag)
    {
      if (container != GetPlayersVehicle())
        {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xa8\x6e\x73\x69\xe8\x95\x61\x74\x2e"); 
         return;}

      put_flag = 0;
      container = 0;
    }
  }

  if (outside_flag == 0 && container == 0 && GetPlayersVehicle() != 0)
  {
    put_flag = 1;
    container = GetPlayersVehicle();
  }

  for (i=2; i<NUM_OBJECTS; i++)
  {
    obj = Obj[i].order;

    if (Obj[obj].loc == INSIDE + O_YOU && obj != container)
    {
      for (j=0; j<num_exceptions; j++)  /* look for obj in exception list*/
        if (obj == exception[j]) break; /* if obj is in exception list, break early*/
      if (j == num_exceptions)          /* if obj is not in exception list*/
      {
        if (InterceptDropPutObj(obj, container, 1, 1) == -1) /* first 1: test only*/
          return;
      }
    }
  }

  prev_darkness = IsPlayerInDarkness();

  nothing_dropped = 1;

  for (;;)
  {
    for (i=2; i<NUM_OBJECTS; i++)
    {
      obj = Obj[i].order;

      if (Obj[obj].loc == INSIDE + O_YOU && obj != container)
      {
        for (j=0; j<num_exceptions; j++)  /* look for obj in exception list*/
          if (obj == exception[j]) break; /* if obj is in exception list, break early*/
        if (j == num_exceptions)          /* if obj is not in exception list*/
        {
          /* caution: if obj doesn't leave inventory here, 
		or function return, outer loop will never end*/

          nothing_dropped = 0;

          if (InterceptDropPutObj(obj, container, 0, 1) == 0) /* first 0: not a test*/
          {
            PrintObjectDesc(obj, 0);
            PrintCompText("\x3a\x20");
			  
            if (put_flag &&
                GetTotalSizeInLocation(INSIDE + container) + Obj[obj].size > 
					Obj[container].capacity)
              {PrintCompLine("\x49\xa6\x77\xca\x27\xa6\x68\x6f\x6c\xab\xad\xc4\x6d\xd3\x65\x21"); return;}
			  
            Obj[obj].loc = put_flag ? INSIDE + container : Obj[O_YOU].loc;
            MoveObjOrderToLast(obj);
            PrintCompLine("\x4f\x6b\x61\x79\x2e");
            TimePassed = 1;
          }

          break; /* break inner loop early to cause outer loop 
		to repeat for any remaining objects*/
        }
      }
    }
    if (i == NUM_OBJECTS) break; /* break outer loop if inner loop was not broken above*/
  }

  if (nothing_dropped)
  {
    if (put_flag)
    {
      if (Obj[container].prop & PROP_SURFACE)
        PrintCompLine("\x99\xa9\xb7\xe0\xe4\xff\xce\x9c\xbd\xeb\xf7\xae\xb4\x69\x74\x2e");
      else
        PrintCompLine("\x99\xa9\xb7\xe0\xe4\xff\xce\x9c\xbd\xeb\xf7\xa8\xe5\xba\x69\x74\x2e");
    }
    else
      PrintCompLine("\x99\xa9\xb7\xe0\xe4\xff\xce\x9c\xbd\xcc\xc2\x70\x2e");
  }

  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}

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
