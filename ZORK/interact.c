#include <string.h>

/* defines, enums ------------------------------------------------------------- */
#define OVR_OVERRIDE_OBJECT_DESC	38
#define OVR_OVERRIDE_ROOM_DESC 		39

#define AllocBuf	((void*)0xBB00)

#define NUM_OBJECTS  80
#define NUM_ROOMS  111 /* including null room 0 */

#define A_GO 43 
#define A_NORTH 1
#define A_OUT 12 
#define A_RESTART 76 
#define A_RESTORE 77 
#define A_SAVE 79 
#define A_QUIT 72 
#define A_BRIEF 16 
#define A_VERBOSE 98 
#define A_VERSION 99 
#define A_SUPERBRIEF 88 
#define A_PRAY 67 
#define A_OPEN 64 
#define A_CLOSE 22 
#define A_EAT 33 
#define A_DRINK 31 
#define A_DEFLATE 26 
#define A_INFLATE 45 
#define A_TURN 95 
#define A_TIE 93 
#define A_UNTIE 97 
#define A_TOUCH 94 
#define A_SCORE 81 
#define A_DIAGNOSE 27 
#define A_WAIT 100 
#define A_ACTIVATE 13 
#define A_TAKE 90 
#define A_BREAK 15 
#define A_DROP 32 
#define A_INVENTORY 46 
#define A_LOOK 53 
#define A_ECHO 34 

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
  V_BRIEF,
  V_VERBOSE,
  V_SUPERBRIEF
};

#define  R_LOUD_ROOM 63

/*room properties bit flags*/
#define R_DESCRIBED     1  /* set when room description already printed */
#define R_BODYOFWATER   2
#define R_LIT           4  /* set when there is natural light or a light fixture */
#define R_WATERHERE     8
#define R_SACRED       16  /* set when thief not allowed in room */
#define R_MAZE         32
#define R_ALWAYSDESC   64

#define O_YOU 1
#define INSIDE  2048

/* extern data structures ---------------------------------------------------------- */

extern int Verbosity;
extern int NumStrWords;

extern unsigned char YouAreDead;
extern unsigned char GatesOpen;
extern unsigned char LowTide;
extern unsigned char GatesButton;
extern unsigned char LoudRoomQuiet;

extern char *ObjectDesc[];
extern char *RoomDesc[];
extern char *StrWord[]; 

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
struct OVERRIDEOBJECTDESC_STRUCT OverrideObjectDesc[];

struct OVERRIDEROOMDESC_STRUCT
{
   int room;
  void (*f)(void);
};
extern struct OVERRIDEROOMDESC_STRUCT OverrideRoomDesc[];

/* extern functions ---------------------------------------------------------------- */
void CallOverlay1(void* Routine, short Par1, char CalledRamBank);
void CallOverlay0(void* Routine, char CalledRamBank);
void ReturnOverlay1(short);

void PrintText(char *p);
void PrintCompText(char *comp_text);
void PrintCompLine(char *p);
int DecompressText(char *text_in, int size_in, char *text_out);
int IsObjVisible(int obj);

/* variables ----------------------------------------------------------------------- */

/* code ---------------------------------------------------------------------------- */

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
    decompressed_text = p = AllocBuf; /* malloc(GetDecompressTextSize(compressed_text, compressed_size)); */
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
  decompressed_text = p = AllocBuf; /* malloc(GetDecompressTextSize(compressed_text, compressed_size)); */
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

int InterceptActionWhenDead(int action)
{
  if (YouAreDead == 0)
    return 0;

  if (action == A_GO || (action >= A_NORTH && action <= A_OUT))
    return 0;

  switch (action)
  {
    case A_QUIT: 
    case A_RESTART: 
    case A_RESTORE: 
    case A_SAVE:
    case A_BRIEF: 
    case A_VERBOSE: 
    case A_SUPERBRIEF: 
    case A_VERSION:
    case A_PRAY:
      return 0;

    /* A_BURN*/
    case A_OPEN: 
    case A_CLOSE: 
    case A_EAT: 
    case A_DRINK: 
    case A_INFLATE: 
    case A_DEFLATE:
    case A_TURN: 
    case A_TIE: 
    case A_UNTIE: 
    case A_TOUCH:
      PrintCompLine("\x45\xd7\xb4\x73\x75\xfa\xa3\xb4\x61\x63\xf0\xca\x87\xef\xc9\xb9\x86\xb6\xe7\x70\x61\x62\x69\xf5\xf0\x65\x73\x2e");
    break;

    case A_SCORE:
      PrintCompLine("\xdc\x75\x27\xa9\xcc\xbf\x64\x21\x20\x48\xf2\x91\x86\x95\xa7\x6b\x8a\x92\xaa\x63\xd3\x65\x3f");
    break;

    case A_DIAGNOSE:
      PrintCompLine("\x8b\xbb\x9e\xe8\x61\x64\x2e");
    break;

    case A_WAIT:
      PrintCompLine("\x4d\x69\x67\x68\xa6\xe0\xb7\x65\xdf\x8e\x75\x27\xd7\xe6\xff\xa3\xb4\x65\xd1\x72\x6e\xc7\x79\x2e");
    break;

    case A_ACTIVATE:
      PrintCompLine("\x8b\xed\xd5\xe4\xba\xf5\x67\x68\xa6\xbd\xe6\x75\x69\xe8\x86\x2e");
    break;

    case A_TAKE:
      PrintCompLine("\xdc\xd8\xc0\x8c\x70\xe0\xd6\xa1\xa2\xc2\x75\x67\xde\xc7\xa1\x6f\x62\x6a\x65\x63\x74\x2e");
    break;

    case A_BREAK:
      PrintCompLine("\x41\xdf\xaa\x75\xfa\xa3\x74\x74\x61\x63\x6b\xa1\xbb\x9e\x76\x61\xa7\xa8\xb4\x92\xb3\xca\x64\xc7\x69\x6f\x6e\x2e");
    break;

    /* A_THROW*/
    case A_DROP: 
    case A_INVENTORY:
      PrintCompLine("\x8b\xcd\xd7\xe4\xba\x70\x6f\x73\xd6\x73\x73\x69\xca\x73\x2e");
    break;

    case A_LOOK:
      PrintPlayerRoomDesc(1);
      PrintCompText("\x85\xc2\xe1\xcb\xe9\x6b\xa1\xc5\xf4\xb1\x9e\x8c\xf6\xbf\x72\xa2\x6c\x79");
      if (GetNumObjectsInLocation(Obj[O_YOU].loc) == 0)
        PrintCompLine("\x2e");
      else
        PrintCompLine("\x8d\xae\x62\x6a\x65\x63\x74\xa1\x61\x70\xfc\xbb\xa8\xb9\xb2\xf0\x6e\x63\x74\x2e");
      if ((Room[Obj[O_YOU].loc].prop & R_LIT) == 0)
        PrintCompLine("\x41\x6c\xa2\xa5\x67\xde\x96\xa9\x87\xe3\xcb\x69\x67\x68\x74\xb5\x81\xc2\xe1\xaa\xf3\x6d\xa1\x64\x69\x6d\xec\xa8\xdf\x75\x6d\xa7\xaf\x65\x64\x2e");
    break;

    default:
      PrintCompLine("\x8b\xe7\x93\x65\xd7\xb4\x64\xba\xa2\x61\x74\x2e");
    break;
  }

  return 1;
}

/* returns 0 if action not intercepted*/

int InterceptActionInLoudRoom(int action)
{
  if (Obj[O_YOU].loc != R_LOUD_ROOM)
    return 0;

  if (LoudRoomQuiet || (GatesOpen == 0 && LowTide))
    return 0; /* room not loud*/

  if ((action >= A_NORTH && action <= A_OUT) || action == A_GO ||
      action == A_SAVE || action == A_RESTORE || action == A_QUIT ||
      action == A_ECHO)
    return 0; /* let these commands through*/

  if (NumStrWords >= 1)
  {
    PrintText(StrWord[0]);
    PrintCompText("\x20");
    PrintText(StrWord[0]);
    PrintCompLine("\x2e\x2e\x2e");
  }
  else
    PrintCompLine("\x2e\x2e\xa4\x2e\x2e\x2e");

  return 1;
}

int InterceptAction(int action)
{
  if (InterceptActionWhenDead(action))   ReturnOverlay1(1); /* return 1; */
  if (InterceptActionInLoudRoom(action)) ReturnOverlay1(1); /* return 1; */

  ReturnOverlay1(0); 
  /* return 0; */
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

