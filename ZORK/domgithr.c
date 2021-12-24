#include <string.h>

/* defines, enums ------------------------------------------------------------- */
#define AllocBuf 0xBB00

#define OVR_OVERRIDE_OBJECT_DESC	38
#define OVR_OVERRIDE_ROOM_DESC 		39

#define NUM_OBJECTS  80
#define NUM_ROOMS  111 /* including null room 0 */

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

#define INSIDE  2048

#define O_YOU 1

/*room properties bit flags*/

#define R_DESCRIBED     1  /* set when room description already printed */
#define R_BODYOFWATER   2
#define R_LIT           4  /* set when there is natural light or a light fixture */
#define R_WATERHERE     8
#define R_SACRED       16  /* set when thief not allowed in room */
#define R_MAZE         32
#define R_ALWAYSDESC   64

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

#define O_LUNCH 17
#define O_WATER 14
#define O_BOTTLE 28
#define O_GARLIC 26
#define O_THIEF 5
#define O_STILETTO 60
#define O_AXE 21
#define O_TROLL 6
#define O_KNIFE 33
#define O_SWORD 61
#define O_LAMP 36
#define O_BROKEN_LAMP 22
#define O_EGG 72
#define O_BROKEN_EGG 73
#define O_CYCLOPS 2
#define O_LARGE_BAG 59
#define O_CHALICE 25

#define R_CYCLOPS_ROOM 42
#define R_TREASURE_ROOM 44

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

extern unsigned char VillainAttacking[];
extern int VillainStrength[];

extern unsigned char MirrorBroken;
extern unsigned char TimePassed;
extern int ThiefDescType;
extern int CyclopsState;
extern int TrollDescType;
extern unsigned char NotLucky;
extern unsigned char TrollAllowsPassage;
extern int CyclopsCounter;
extern int Verbosity;
extern unsigned char ThiefEngrossed;

/* ----------------------- extern functions -----------------*/
int xrnd(void);
int DecompressText(char *text_in, int size_in, char *text_out);
void PrintNewLine(void);
void PrintText(char *p);
void PrintLine(char *p);
void PrintCompText(char *comp_text);
void PrintCompLine(char *p);
int GetRandom(int Range);
int IsObjVisible(int obj);
int IsObjCarriedByActor(int obj);
int IsPlayerInDarkness(void);
void PrintObjectDesc(int obj, int desc_flag);
void PrintContents(int obj, char *heading, int print_empty);
void PrintPresentObjects(int location, char *heading, int list_flag);
void PrintRoomDesc(int room, int force_description);
void PrintPlayerRoomDesc(int force_description);
void VillainDead(int i);
int ThiefDepositBooty(int room);
void ThiefRecoverStiletto(void);

short CallOverlay0(void* Routine, char CalledRamBank);
short CallOverlay1(void* Routine, short Par1, char CalledRamBank);
void ReturnOverlay0(short);
void ReturnOverlay1(short);

/* ----------------------- variables ----------------------- */

/* ----------------------- code ----------------------- */

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

/*move order of "obj" to last in printing order*/
void MoveObjOrderToLast(int obj)
{
  int i, j;

  for (i=2; i<NUM_OBJECTS; i++)
    if (obj == Obj[i].order)
  {
    for (j=i; j<NUM_OBJECTS-1; j++)
      Obj[j].order = Obj[j+1].order;
    Obj[j].order = obj;
    break;
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
    decompressed_text = p = (char*) AllocBuf; /* malloc(GetDecompressTextSize(compressed_text, compressed_size)); */
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
    PrintCompLine("\x49\xa6\x9a\x70\xc7\xfa\xb0\xfd\x63\x6b\x8e\xc3\xbb\x9e\xf5\x6b\x65\xec\
\x89\xef\xfb\xaf\xd4\xb0\xc4\xd0\x67\x72\x75\x65\x2e");
  else
  {
    PrintRoomDesc(Obj[O_YOU].loc, force_description);
    if (force_description || Verbosity != V_SUPERBRIEF) 
	PrintPresentObjects(Obj[O_YOU].loc, 0, 0);
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

void GiveLunchToCyclops(void)
{
  TimePassed = 1;

  CyclopsCounter = 0;
  CyclopsState = 2; /* thirsty*/

  Obj[O_LUNCH].loc = 0;

  PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x73\x61\x79\xa1\x22\x4d\x6d\xf9"
"\x4d\x6d\x6d\xa4\x49\xcb\x6f\xd7\xc0\xff\xeb\x65\x70\xfc\x72\x73\x21\x20\x42"
"\xf7\xae\x68\xb5\x63\xa5\x6c\xab\x49\x20\xfe\x9e\xd0\x64\xf1\x6e\x6b\xa4\x50"
"\xac\xcd\x70\xa1\x49\xb3\xa5\x6c\xab\x64\xf1\x6e\x6b\x80\xb0\xd9\x6f\xab\xdd"
"\x95\xaf\x95\x97\x2e\x22\x20\x20\x46\xc2\xf9\x81\x67\xcf\x61\xf9\xa7\xc0\x9a"
"\x65\x79\x65\xb5\xc7\xb3\xa5\x6c\xab\xef\xaa\xd8\x6d\xb2\xd5\x95\xaf\x86\xa3"
"\xa9\x20\x22\xa2\xaf\x95\x97\x22\x2e");
}

void GiveBottleToCyclops(void)
{
  TimePassed = 1;

  if (Obj[O_WATER].loc != INSIDE + O_BOTTLE)
    PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\xa9\x66\xfe\xbe\x80\xfb\x6d\x70"
"\x74\xc4\x62\xff\x74\x6c\x65\x2e");
  else if (CyclopsState != 2) /* not thirsty*/
    PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x61\x70\x70\xbb\xd4\x74\xec\x87"
"\xe3\xa6\xa2\x69\x72\xc5\xc4\x8c\xa9\x66\xfe\xbe\x86\xb6\x67\xd4\xac\xa5\xa1"
"\xdd\x66\x65\x72\x2e");
  else
  {
    CyclopsState = 3; /* asleep*/

    Obj[O_WATER].loc = 0;
    Obj[O_BOTTLE].loc = R_CYCLOPS_ROOM;
    Obj[O_BOTTLE].prop |= PROP_OPEN;

    PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x74\x61\x6b\xbe\x80\xb0\xff\x74"
"\xcf\xb5\xfa\x65\x63\x6b\xa1\xa2\xaf\xa8\x74\x27\xa1\x6f\xfc\x6e\xb5\x8c\x64"
"\xf1\x6e\x6b\xa1\x81\x77\xaf\xac\xa4\x41\xee\xe1\xd4\xa6\xfd\xd1\x72\xb5\x94"
"\xcf\x74\xa1\xa5\xa6\xd0\x79\x61\x77\xb4\xa2\xaf\xe4\xbf\x72\xec\xb0\xd9\x77"
"\xa1\x8f\x6f\xd7\x72\xb5\x8c\x96\xb4\x66\xe2\x6c\xa1\x66\xe0\xa6\xe0\xcf\x65"
"\x70\x20\x28\x77\xcd\xa6\x64\x69\xab\x8f\x70\xf7\xa8\xb4\xa2\xaf\xcc\xf1\x6e"
"\x6b\xb5\xad\x79\x77\x61\x79\x3f\x29\x2e");
  }
}

void DoMiscGiveTo_cyclops(int obj)
{
  if (obj == O_WATER)
    obj = O_BOTTLE;

  if (Obj[obj].loc != INSIDE + O_YOU)
    PrintCompLine("\x8b\xbb\xd4\x27\xa6\x68\x6f\x6c\x64\x84\xa2\x61\x74\x21");
  else if (CyclopsState == 3)
    PrintCompLine("\x48\x65\x27\xa1\xe0\xcf\x65\x70\x2e");
  else
    switch (obj)
  {
    case O_LUNCH:  GiveLunchToCyclops();                                          break;
    case O_BOTTLE: GiveBottleToCyclops();                                         break;
    case O_GARLIC: PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x6d\x61\xc4\xef"
"\xc0\xf6\x67\x72\x79\xb5\x62\xf7\x80\xa9\x87\xd0\xf5\x6d\x69\x74\x2e"); break;
    default:         PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x9a\xe3\xa6"
"\x73\xba\xc5\x75\x70\x69\xab\xe0\x89\xbf\xa6\x54\x48\x41\x54\x21");     break;
  }

  ReturnOverlay1(0);
}

void DoMiscGiveTo_thief(int obj)
{
  if (Obj[obj].loc != INSIDE + O_YOU)
    {PrintCompLine("\x8b\xbb\xd4\x27\xa6\x68\x6f\x6c\x64\x84\xa2\x61\x74\x21"); ReturnOverlay1(0);}

  if (Obj[O_THIEF].prop & PROP_NODESC)
    {PrintCompLine("\x8b\xe7\x93\xd6\x9e\xce\x6d\xb5\x62\xf7\xc0\x9e\x63\xa5\x6c"
"\xab\xef\xe4\xbf\x72\x62\x79\x2e"); ReturnOverlay1(0);}

  TimePassed = 1;

  if (VillainStrength[VILLAIN_THIEF] < 0)
  {
    VillainStrength[VILLAIN_THIEF] = -VillainStrength[VILLAIN_THIEF];
    VillainAttacking[VILLAIN_THIEF] = 1;
    ThiefRecoverStiletto();
    ThiefDescType = 0; /* default*/
    PrintCompLine("\xdc\xd8\xeb\xc2\x70\x6f\xd6\xab\x76\x69\x63\xf0\xf9\x73\x75"
"\x64\xe8\x6e\xec\xda\x65\x63\x6f\xd7\x72\xa1\x63\xca\x73\x63\x69\xa5\x73\xed\x73\x73\x2e");
  }

  Obj[obj].loc = INSIDE + O_THIEF;
  Obj[obj].prop |= PROP_NODESC;
  Obj[obj].prop |= PROP_NOTTAKEABLE;

  if (obj == O_STILETTO)
    PrintCompLine("\x85\xa2\x69\x65\xd2\x74\x61\x6b\xbe\xc0\x9a\xc5\x69\xcf\x74"
"\xbd\x8d\xaa\xe2\xf7\xbe\x86\xb7\xc7\xde\xd0\x73\x6d\xe2\xea\xe3\xab\xdd\xc0\x9a\xa0\x61\x64\x2e");
  else if (Obj[obj].thiefvalue > 0)
  {
    ThiefEngrossed = 1;
    PrintCompLine("\x85\xa2\x69\x65\xd2\x9a\x74\x61\x6b\xd4\xa3\x62\x61\x63\x6b"
"\xb0\xc4\x92\x20\xf6\x65\x78\xfc\x63\xd1\xab\x67\xd4\xac\x6f\x73\xc7\x79\xb5"
"\x62\xf7\xa3\x63\x63\x65\x70\x74\xa1\xc7\x8d\xaa\xbd\x70\xa1\xbd\xa3\x64\x6d"
"\x69\xa9\xa8\x74\xa1\xef\x61\xf7\x79\x2e");
  }
  else
    PrintCompLine("\x85\xa2\x69\x65\xd2\x70\xfd\x63\xbe\xa8\xa6\xa7\xc0\x9a\x62"
"\x61\xc1\x8c\xa2\xad\x6b\xa1\x8f\x70\x6f\xf5\xd1\x6c\x79\x2e");

  ReturnOverlay1(0);
}

void DoMiscGiveTo_troll(int obj)
{
  if (Obj[obj].loc != INSIDE + O_YOU)
    {PrintCompLine("\x8b\xbb\xd4\x27\xa6\x68\x6f\x6c\x64\x84\xa2\x61\x74\x21"); ReturnOverlay1(0);}

  if (TrollDescType == 1) /* unconscious*/
    {PrintCompLine("\x85\xf6\x63\xca\x73\x63\x69\xa5\xa1\x74\xc2\xdf\xa8\x67\xe3"
"\xa9\xa1\x92\xe6\x69\x66\x74\x2e"); ReturnOverlay1(0);}

  TimePassed = 1;

  if (obj == O_AXE)
  {
    PrintCompLine("\x85\x74\xc2\xdf\xaa\x63\xf4\x74\xfa\xbe\xc0\x9a\xa0\x61\xab"
"\xa7\xb3\xca\x66\xfe\x69\xca\xb5\x96\xb4\x74\x61\x6b\xbe\x80\xa3\x78\x65\x2e");

    Obj[O_AXE].loc = INSIDE + O_TROLL;
    Obj[O_AXE].prop |= PROP_NODESC;
    Obj[O_AXE].prop |= PROP_NOTTAKEABLE;
    Obj[O_AXE].prop &= ~PROP_WEAPON;

    VillainAttacking[VILLAIN_TROLL] = 1;
  }
  else
  {
    PrintCompText("\x85\x74\xc2\xdf\xb5\x77\x68\xba\x9a\xe3\xa6\x6f\xd7\x72\xec"
"\xeb\xc2\x75\x64\xb5\x67\xf4\x63\x69\xa5\x73\xec\xa3\x63\x63\x65\x70\x74\xa1\x81\x67\x69\x66\x74");
    if (obj == O_KNIFE || obj == O_SWORD)
    {
      if (PercentChance(20, -1))
      {
        PrintCompLine("\x8d\xfb\xaf\xa1\xc7\xc0\xf6\x67\xf1\xec\xa4\x50\xe9\xb6"
"\x74\xc2\xdf\xb5\x94\x64\x69\xbe\xc6\xc2\xf9\xad\xa8\xe5\xac\x6e\xe2\xc0\x65\x6d"
"\xd3\x72\xcd\x67\x9e\x8c\xce\xa1\xe7\x72\xe7\x73\xa1\x64\xb2\x61\x70\xfc\xbb\xa1"
"\xa7\xa3\xaa\xa7\xb2\xd1\xb6\x62\xfd\x63\x6b\xc6\x6f\x67\x2e");
        Obj[obj].loc = 0;
        Obj[O_TROLL].loc = 0;
        VillainDead(VILLAIN_TROLL);
      }
      else
      {
        PrintCompLine("\x8d\xb5\xef\x84\x66\xd3\x80\xee\xe1\xd4\xa6\x73\xaf\xd5"
"\xb5\xa2\xc2\x77\xa1\xc7\xb0\x61\x63\x6b\xa4\x46\xd3\x74\xf6\xaf\x65\xec\xb5\x81"
"\x74\xc2\xdf\xc0\xe0\xeb\xe9\xb6\x63\xca\x74\xc2\x6c\xb5\x8c\xc7\xc6\xe2\x6c\xa1"
"\xbd\x80\xc6\xd9\xd3\xa4\x48\x9e\x64\x6f\xbe\xe4\xff\xcb\xe9\x6b\xeb\xcf\xe0\x65\x64\x2e");
        Obj[obj].loc = Obj[O_YOU].loc;
        MoveObjOrderToLast(obj);
        VillainAttacking[VILLAIN_TROLL] = 1;
      }
    }
    else
    {
      int prev_darkness;

      PrintCompLine("\x8d\xe4\xff\xc0\x61\x76\x84\x81\x6d\x6f\xc5\xcc\xb2\x63\xf1"
"\x6d\xa7\xaf\x84\x74\xe0\xd1\x73\xb5\x67\xcf\x65\x66\x75\xdf\xc4\xbf\x74\xa1\x69\x74\x2e");

      prev_darkness = IsPlayerInDarkness();
      Obj[obj].loc = 0;
      if (IsPlayerInDarkness() != prev_darkness)
      {
        PrintNewLine();
        PrintPlayerRoomDesc(1);
      }
    }
  }

  ReturnOverlay1(0);
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
      if (to == 0) PrintCompLine("\x49\xa6\x74\x75\x6d\x62\xcf\xa1\xbd\x80\xe6"
"\xc2\xf6\x64\x2e");
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

void DoMiscThrowTo_chasm(int obj)
{
  int prev_darkness = IsPlayerInDarkness();

  PrintCompLine("\x49\xa6\x64\xc2\x70\xa1\xa5\xa6\xdd\xaa\x69\x67\x68\xa6\xa7"
"\xbd\x80\xb3\xcd\x73\x6d\x2e");
  TimePassed = 1;
  Obj[obj].loc = 0;
  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }

  ReturnOverlay1(0);
}

void DoMiscThrowTo_river(int obj)
{
  int prev_darkness = IsPlayerInDarkness();

  PrintCompLine("\x49\xa6\x74\x75\x6d\x62\xcf\xa1\xa7\xbd\x80\xda\x69\xd7\xb6"
"\x8c\x9a\xd6\xd4\xe4\xba\x6d\xd3\x65\x2e");
  TimePassed = 1;
  Obj[obj].loc = 0;
  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }

  ReturnOverlay1(0);
}

void DoMiscThrowTo_mirror(int obj)
{
  if (MirrorBroken)
    PrintCompLine("\x48\x61\xd7\x93\x8f\x64\xca\x9e\xd4\xa5\x67\xde\x64\x61\x6d"
"\x61\x67\x9e\xe2\xa9\x61\x64\x79\x3f");
  else
  {
    PrintCompLine("\x8b\xcd\xd7\xb0\xc2\x6b\xd4\x80\xee\x69\x72\xc2\x72\xa4\x49"
"\xc0\x6f\xfc\x86\xc0\x61\xd7\xa3\xaa\x65\xd7\xb4\x79\xbf\x72\x73\x27\xaa\x75\x70"
"\x70\xec\x8a\x67\xe9\xab\x6c\x75\x63\x6b\xc0\xad\x64\x79\x2e");
    TimePassed = 1;

    MirrorBroken = 1;
    NotLucky = 1;

    ThrowObjRoutine(obj, 0);
  }

  ReturnOverlay1(0);
}

void DoMiscThrowTo_troll(int obj)
{
  if (TrollDescType == 1) /* unconscious*/
    ThrowObjRoutine(obj, O_TROLL);
  else
  {
    PrintCompLine("\x85\x74\xc2\xdf\xb5\x77\x68\xba\x9a\xa9\x6d\xbb\x6b\x61\x62"
"\xec\xb3\xe9\x72\x64\xa7\xaf\xd5\xb5\xe7\x74\xfa\xbe\xa8\x74\x2e");
    DoMiscGiveTo_troll(obj);
  }

  ReturnOverlay1(0);
}

void DoMiscThrowTo_cyclops(int obj)
{
  if (CyclopsState == 3) /* sleeping*/
    ThrowObjRoutine(obj, O_CYCLOPS);
  else
  {
    PrintCompLine("\x22\x44\xba\x8f\xa2\xa7\x6b\x20\x49\x27\xf9\xe0\xaa\x74\x75"
"\x70\x69\xab\xe0\xee\xc4\x66\xaf\xa0\xb6\x77\xe0\x3f\x22\xb5\x94\x73\x61\x79"
"\x73\xb5\x64\x6f\x64\x67\x97\x2e");
    ThrowObjRoutine(obj, 0);
  }

  ReturnOverlay1(0);
}

void ThiefLoseBagContents(void)
{
  int flag = 0, obj;

  PrintCompText("\x8b\x65\x76\x69\xe8\xe5\xec\xc6\xf1\x67\x68\xd1\xed\xab\x81"
"\xc2\x62\xef\x72\xb5\xa2\xa5\x67\xde\x8f\x64\x69\x64\x93\xce\xa6\xce\x6d\xa4"
"\x48\x9e\x66\xcf\x65\x73");

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == INSIDE + O_THIEF &&
        obj != O_LARGE_BAG &&
        obj != O_STILETTO)
  {
    flag = 1;
    Obj[obj].loc = Obj[O_YOU].loc;
    Obj[obj].prop &= ~PROP_NODESC;
    Obj[obj].prop &= ~PROP_NOTTAKEABLE;
  }

  if (flag)
    PrintCompLine("\xb5\x62\xf7\x80\xb3\xca\xd1\xe5\xa1\xdd\xc0\x9a\x62\x61\xc1"
"\x66\xe2\xea\xca\x80\xc6\xd9\x6f\x72\x2e");
  else
    PrintCompLine("\x2e");
}

void DoMiscThrowTo_thief(int obj)
{
  if (Obj[O_THIEF].prop & PROP_NODESC)
    {PrintCompLine("\x8b\xe7\x93\xd6\x9e\xce\x6d\xb5\x62\xf7\xc0\x9e\x63\xa5"
"\x6c\xab\xef\xe4\xbf\x72\x62\x79\x2e"); return;}

  if (ThiefDescType == 1) /* unconscious*/
    ThrowObjRoutine(obj, O_THIEF);
  else
  {
    TimePassed = 1;

    if (obj == O_KNIFE &&
        VillainAttacking[VILLAIN_THIEF] == 0)
    {
      Obj[O_KNIFE].loc = Obj[O_YOU].loc;

      if (PercentChance(10, 0))
      {
        ThiefLoseBagContents();
        Obj[O_THIEF].prop |= PROP_NODESC;
      }
      else
      {
        PrintCompLine("\x8b\x6d\xb2\xd6\x64\x83\x9e\xa2\x69\x65\xd2\x6d\x61\x6b"
"\xbe\xe4\xba\xaf\xd1\x6d\x70\xa6\xbd\x9f\x61\x6b\x9e\x81\x6b\x6e\x69\x66\x65\xb5"
"\xa2\xa5\x67\xde\xc7\xb7\xa5\x6c\xab\xef\xa3\xc6\xa7\x9e\x61\x64\x64\xc7\x69\xca"
"\x89\x81\x63\x6f\xdf\x65\x63\xf0\xca\xa8\xb4\xce\xa1\x62\x61\x67\xa4\x48\x9e\x64"
"\x6f\xbe\xaa\xf3\xf9\xad\x67\xac\xd5\xb0\xc4\x92\xa3\x74\xd1\x6d\x70\x74\x2e");
        VillainAttacking[VILLAIN_THIEF] = 1;
      }
    }
    else
      ThrowObjRoutine(obj, O_THIEF);
  }

  ReturnOverlay1(0);
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

