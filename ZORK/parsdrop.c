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

#define R_GRATING_CLEARING 12
#define R_GRATING_ROOM 36
#define R_CELLAR 17
#define R_SLIDE_ROOM 110

#define O_YOU 1
#define O_LARGE_BAG 59
#define O_LOWERED_BASKET 7
#define O_CHALICE 25
#define O_WATER 14
#define O_INFLATED_BOAT 39

#define F_SLIDE 2052
#define F_RIVER 2067
#define F_SCENERY_VIS 2048
#define F_SCENERY_NOTVIS 2049
#define F_NOTVIS 2050 
#define F_AMB 2051 
#define F_GRATE 2062

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

extern char *StrWord[];

extern int ItObj, PrevItObj;

extern unsigned char TimePassed;
extern unsigned char RopeTiedToRail;
extern unsigned char YouAreInBoat;
extern int DownstreamCounter;
extern int ThiefDescType;
extern int LoadAllowed;

/* extern functions ---------------------------------------------------------------- */
void PrintNewLine(void);
void PrintText(char *p);
void PrintLine(char *p);
void PrintCompText(char *comp_text);
void PrintCompLine(char *p);
int MatchCurWord( char *match);

int IsObjVisible(int obj);
int IsPlayerInDarkness(void);
void PrintObjectDesc(int obj, int desc_flag);
void PrintPlayerRoomDesc(int force_description);
int GetAllObjFromInput(int room);
int GetTotalSizeInLocation(int loc);
void MoveObjOrderToLast(int obj);
int MatchCurWord( char *match);
int InterceptDropPutObj(int obj, int container, int test, int multi);
int InterceptTakeObj(int obj);
int GetPlayersVehicle(void);
void PrintCantAction(int action);
int DropPutVerifyContainer(int container);
void DropPutAllRoutine(int put_flag);

void ReturnOverlay1(short);

/* variables ----------------------------------------------------------------------- */

/* overlays ------------------------------------------------------------------------ */
/* code ---------------------------------------------------------------------------- */
/* if player is inside vehicle, return vehicle obj; otherwise return 0*/
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

int GetPlayersVehicle(void)
{
  if (YouAreInBoat)
    return O_INFLATED_BOAT;
  else
    return 0;
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

void ParseActionDropPut_ext(int put_flag)
{
  int obj, i, num_drops, container, into_flag, outside_flag, prev_darkness;
  unsigned short* drop = (unsigned short*)AllocBuf; /* unsigned short drop[80]; */ /*stores objects to be dropped*/

  if (MatchCurWord("all") || MatchCurWord("everything"))
    {DropPutAllRoutine(put_flag); ReturnOverlay1(0);}

  container = 0;
  into_flag = 0;
  outside_flag = 0;
  num_drops = 0;

  for (;;)
  {
    obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) ReturnOverlay1(0);
    drop[num_drops++] = obj;
    if (num_drops == 80 || CurWord == NumStrWords) break;

    if (MatchCurWord("in") || MatchCurWord("into") || MatchCurWord("inside") 
 	|| MatchCurWord("through") || MatchCurWord("on") || MatchCurWord("onto"))
    {
      put_flag = 1; /* change "drop" to "put" (if not already)*/
      into_flag = 1;

      container = GetAllObjFromInput(Obj[O_YOU].loc); if (container <= 0) ReturnOverlay1(0);
      if (DropPutVerifyContainer(container)) ReturnOverlay1(0);

      break;
    }

    if (MatchCurWord("outside"))
    {
      put_flag = 1; /* change "drop" to "put" (if not already)*/
      outside_flag = 1;

      container = GetAllObjFromInput(Obj[O_YOU].loc); if (container <= 0) ReturnOverlay1(0);
      if (DropPutVerifyContainer(container)) ReturnOverlay1(0);

      break;
    }

    if (MatchCurWord("then")) 
    {CurWord--; break;} /* end of this turn's command; back up so "then" can be matched later*/
    if (MatchCurWord("and") == 0)
      {PrintCompLine("\x50\xcf\xe0\x9e\xfe\x9e\xd0\x63\xe1\x6d\xd0\xd3\x80\xb7"
"\xd3\xab\x22\xad\x64\x22\xb0\x65\x74\x77\xf3\xb4\xe3\xf6\x73\x2e"); ReturnOverlay1(0);}
    for (;;) if (MatchCurWord("and") == 0) 
               break; /* handle repeated "and"s like "take one, two, and three"*/
  }

  if (put_flag)
  {
    if (into_flag == 0 && outside_flag == 0)
      {PrintCompLine("\x8b\xed\xd5\x89\x73\xfc\x63\x69\x66\xc4\xd0\x63\xca\x74\x61\xa7\x65\x72\x2e"); ReturnOverlay1(0);}

    if (outside_flag)
    {
      if (container != GetPlayersVehicle())
        {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xa8\x6e\x73\x69\xe8\x95\x61\x74\x2e"); ReturnOverlay1(0);}

      put_flag = 0;
      container = 0;
    }
  }

  if (outside_flag == 0 && container == 0 && GetPlayersVehicle() != 0)
  {
    put_flag = 1;
    container = GetPlayersVehicle();
  }

  for (i=0; i<num_drops; i++)
  {
    obj = drop[i];

    if (obj == O_YOU || obj >= NUM_OBJECTS)
    {
      if (num_drops > 1)
        PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\xaf\xcb\xbf"
"\xc5\xae\xed\x8a\xa2\x6f\xd6\xae\x62\x6a\x65\x63\x74\x73\x21");
      else
        PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\xa2\x61\x74\x21");
      ReturnOverlay1(0);
    }

    if (InterceptDropPutObj(obj, container, 1, (num_drops > 1)) == -1) /* first 1: test only*/
      ReturnOverlay1(0);
  }

  prev_darkness = IsPlayerInDarkness();

  for (i=0; i<num_drops; i++)
  {
    obj = drop[i];

    if (Obj[obj].loc != INSIDE + O_YOU)
    {
      if (num_drops > 1)
        {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
      PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\xa2\x61\x74\x21");
    }
    else if (obj == container)
    {
      if (num_drops > 1)
        {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
      PrintCompLine("\x42\xf7\xa8\xa6\x77\xa5\x6c\xab\x64\xb2\x61\x70\xfc\xbb\xc6\xc2\xf9\xa9\xe2\xc7\x79\x21");
    }
    else if (InterceptDropPutObj(obj, container, 0, (num_drops > 1)) == 0) /* first 0: not a test*/
    {
      if (num_drops > 1)
        {PrintObjectDesc(obj, 0); PrintCompText("\x3a\x20");}
      if (put_flag && GetTotalSizeInLocation(INSIDE + container) + Obj[obj].size >
Obj[container].capacity)
        PrintCompLine("\x49\xa6\x77\xca\x27\xa6\x68\x6f\x6c\xab\xad\xc4\x6d\xd3\x65\x21");
      else
      {
        Obj[obj].loc = put_flag ? INSIDE + container : Obj[O_YOU].loc;
        MoveObjOrderToLast(obj);
        PrintCompLine("\x4f\x6b\x61\x79\x2e");
        TimePassed = 1;
      }
    }
  }

  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }

  ReturnOverlay1(0);
}
