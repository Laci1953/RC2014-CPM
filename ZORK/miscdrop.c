#include <string.h>

/* defines, enums ------------------------------------------------------------- */
#define OVR_OVERRIDE_OBJECT_DESC	38
#define OVR_OVERRIDE_ROOM_DESC 		39

#define AllocBuf 0xBB00

#define NUM_ROOMS  111 /* including null room 0 */

#define NUM_OBJECTS  80

#define INSIDE  2048

#define BL0 (256-23)

enum
{
  V_BRIEF,
  V_VERBOSE,
  V_SUPERBRIEF
};

/*fixed (unmoving) objects*/
enum
{
  F_SCENERY_VIS = 2048, /*some anonymous scenery object, visible*/
  F_SCENERY_NOTVIS,     /*                               not visible*/
  F_NOTVIS,             /*fixed object not visible*/
  F_AMB                /*amibiguous (ask for clarification)*/
};

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

#define O_YOU 1
#define O_LAMP 36
#define O_COFFIN 29
#define O_SWORD 61

#define R_LIVING_ROOM 16
#define R_EGYPT_ROOM 69
#define R_WEST_OF_HOUSE 1
#define R_NORTH_OF_HOUSE 3
#define R_EAST_OF_HOUSE 5
#define R_SOUTH_OF_HOUSE 4
#define R_FOREST_1 6
#define R_FOREST_2 7
#define R_FOREST_3 9
#define R_PATH 10
#define R_CLEARING 13
#define R_GRATING_CLEARING 12
#define R_CANYON_VIEW 93
#define R_SOUTH_TEMPLE 73
#define R_ENTRANCE_TO_HADES 66


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

extern char *SceneryNouns;
extern int ItObj, PrevItObj; /*the obj "it" refers to*/
extern char *StrWord[]; 
extern char *BlockMsg[];
extern char *ObjectDesc[];

extern int NumStrWords;
extern int CurWord;

extern unsigned char GameOver;
extern int NumDeaths;

extern unsigned char YouAreInBoat;
extern unsigned char ExitFound;
extern unsigned char YouAreDead;
extern unsigned char TrollAllowsPassage;
extern unsigned char NotLucky;

extern int Verbosity;

/* ----------------------- extern functions -----------------*/
void PrintNewLine(void);
void PrintText(char *p);
void PrintLine(char *p);
void PrintCompText(char *comp_text);
void PrintCompLine(char *p);
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
int GetAllObjFromInput(int room);
int GetTotalSizeInLocation(int loc);
int DecompressText(char *text_in, int size_in, char *text_out);
int MatchCurWord( char *match);
int IsWordInCommaString( char *p,  char *word);
int GetFixedObjFromInput(int room);
void MoveObjOrderToLast(int obj);
int GetObjFromInput(void);

short CallOverlay0(void* Routine, char CalledRamBank);
short CallOverlay1(void* Routine, short Par1, char CalledRamBank);

/* ---------------variables ----------------------------*/

/* ----------------------- code ----------------------- */
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
