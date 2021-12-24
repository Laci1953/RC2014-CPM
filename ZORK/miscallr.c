#include <string.h>

/* defines, enums ------------------------------------------------------------- */

#define AllocBuf 0xBB00

#define OVR_OVERRIDE_OBJECT_DESC	38

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

struct OVERRIDEOBJECTDESC_STRUCT
{
  int obj;
  void (*f)(int);
};
struct OVERRIDEOBJECTDESC_STRUCT OverrideObjectDesc[];

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

extern char *ObjectDesc[];

extern char *SceneryNouns;

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
extern unsigned char NotLucky;

/* ----------------------- extern functions -----------------*/
int IsObjVisible(int obj);
int IsObjCarriedByActor(int obj);
int IsPlayerInDarkness(void);
void PrintObjectDesc(int obj, int desc_flag);
void PrintContents(int obj, char *heading, int print_empty);
void PrintPresentObjects(int location, char *heading, int list_flag);
void PrintRoomDesc(int room, int force_description);
void PrintPlayerRoomDesc(int force_description);

int GetObjFromInput(void);
int GetAllObjFromInput(int room);
int GetNumObjectsInLocation(int loc);
int GetTotalSizeInLocation(int loc);
void MoveObjOrderToLast(int obj);
int MatchCurWord( char *match);
int PercentChance(int x, int x_not_lucky);
short xrnd(void);
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

/*string contains words separated by commas with no whitespace*/
int IsWordInCommaString( char *p,  char *word)
{
  char *q, *buffer = (char*)AllocBuf;
  int len;

  if (p != 0)
    for (;;)
  {
    q = strchr(p, ',');
/* ------------------------
    if (q == 0) q = strchr(p, 0);
    len = q-p;
----------------------------------- */
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
/* --------------------------
      if (q == 0) q = strchr(p, 0);
      len = q-p;
----------------------------- */
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
      PrintCompLine("\x49\x27\xf9\xe3\xa6\x73\xd8\x9e\x77\xcd\xa6\xc9\x75\x27"
"\xa9\xda\x65\x66\xac\xf1\x9c\xbd\xb7\xc7\xde\xca\x9e\xd3\xee\xd3\x9e\xdd\x95\x6f\xd6\xe4\xa5\x6e\x73\x2e");
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
    PrintCompLine("\x49\xcc\x69\x64\x93\xa9\x63\x6f\x67\x6e\x69\x7a\x9e\xca\x9e"
"\xd3\xee\xd3\x9e\xdd\x95\x6f\xd6\xe4\xa5\x6e\x73\xb5\xd3\x86\xcc\x69\x64\x93\x73\xfc\x63\x69\x66\xc4\xca\
\x65\x2e");
  }
  else if (obj == -1)
  {
    ItObj = -1;
    PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63"
"\xb7\xc7\xde\xca\x9e\xd3\xee\xd3\x9e\xdd\x95\x6f\xd6\xe4\xa5\x6e\x73\x2e");
  }
  else
  {
    if (ItObj == 0) ItObj = obj; /*first object encountered this sentence; set itobj*/
    else ItObj = -1; /*another obj encountered; invalidate itobj*/
  }

  return obj;
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

/*returns total size of objects in location*/
/*location can be a room, player's inventory, or inside object*/
int GetTotalSizeInLocation(int loc)
{
  int size, i, obj;

  size = 0;
  for (i=2; i<NUM_OBJECTS; i++)
  {
    obj = Obj[i].order;
    if (Obj[obj].loc == loc) size += Obj[obj].size;
  }
  return size;
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
    int obj, size;
     char *p;
      int len;
       char *q;

  num_matches = 0;

  i = 0;
  for (;;) /*look through noun phrase table*/
  {
    obj = NounPhraseToObj[i].obj;
    if (obj == 0) break;

    size = 0;
    p = NounPhraseToObj[i].phrase;
    for (j=0; j<3; j++)
    {
      q = strchr(p, ' ');
/* ------------------------------------- modify this
      if (q == 0) q = strchr(p, 0);
      len = q-p;
---------------------------------------- to */
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
