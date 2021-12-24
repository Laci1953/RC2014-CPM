#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* defines, enums ------------------------------------------------------------- */

#define AllocBuf	((void*)0xBB00)

#define OVR_OVERRIDE_OBJECT_DESC	38
#define OVR_OVERRIDE_ROOM_DESC 		39

#define SAVE_FILENAME "ZORK.SAV"
#define SAVE_FILENAME_NUMCHAR_INDEX  6

#define SCORE_MAX  350
#define NUM_OBJECTS  80
#define NUM_ROOMS  111 /* including null room 0 */

#define NUM_TREASURESCORES  19
#define NUM_ROOMSCORES      5

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

/*fixed (unmoving) objects*/

enum
{
  F_SCENERY_VIS = 2048, /*some anonymous scenery object, visible*/
  F_SCENERY_NOTVIS,     /*                               not visible*/
  F_NOTVIS,             /*fixed object not visible*/
  F_AMB                /*amibiguous (ask for clarification)*/
};

enum
{
  V_BRIEF,
  V_VERBOSE,
  V_SUPERBRIEF
};

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

enum
{
  VILLAIN_TROLL,
  VILLAIN_THIEF,
  VILLAIN_CYCLOPS,

  NUM_VILLAINS
};

/* extern data structures ---------------------------------------------------------- */

extern int CursorColumn;
extern int NumPrintedLines;

extern int Score;
extern unsigned char GameOver;
extern int NumMoves;
extern int Verbosity;
extern int CurWord;
extern int NumStrWords;

extern char *ObjectDesc[];
extern char *RoomDesc[];
extern char *SceneryNouns;
extern int ItObj, PrevItObj; /*the obj "it" refers to*/
extern char *StrWord[]; 

/* save-restore area ----------------------------------------*/

extern unsigned char RugMoved;
extern unsigned char TrapOpen;
extern unsigned char ExitFound; /* set this when player finds an exit from dungeon other than the trapdoor*/
extern unsigned char KitchenWindowOpen;
extern unsigned char GratingRevealed;
extern unsigned char GratingUnlocked;
extern unsigned char GratingOpen;
extern unsigned char GatesOpen;
extern unsigned char LowTide;
extern unsigned char GatesButton;
extern unsigned char LoudRoomQuiet;
extern unsigned char RainbowSolid;
extern unsigned char WonGame;
extern unsigned char MirrorBroken; /* set NotLucky too*/
extern unsigned char RopeTiedToRail;
extern unsigned char SpiritsBanished;
extern unsigned char TrollAllowsPassage;
extern unsigned char YouAreSanta;
extern unsigned char YouAreInBoat;
extern unsigned char NotLucky;
extern unsigned char YouAreDead;
extern unsigned char SongbirdSang;
extern unsigned char ThiefHere;
extern unsigned char ThiefEngrossed;
extern unsigned char YouAreStaggered;
extern unsigned char BuoyFlag;

extern int NumMoves;
extern int LampTurnsLeft;
extern int MatchTurnsLeft;
extern int CandleTurnsLeft;
extern int MatchesLeft;
extern int ReservoirFillCountdown;
extern int ReservoirDrainCountdown;
extern int MaintenanceWaterLevel;
extern int DownstreamCounter;
extern int BellRungCountdown; /* these three are for ceremony*/
extern int CandlesLitCountdown;
extern int BellHotCountdown;
extern int CaveHoleDepth;
extern int Score;
extern int NumDeaths;
extern int CyclopsCounter;
extern int CyclopsState; /* 0: default  1: hungry  2: thirsty  3: asleep  4: fled*/
extern int LoadAllowed;
extern int PlayerStrength;
extern int TrollDescType;
extern int ThiefDescType; /* 0: default  1: unconscious*/
extern int EnableCureRoutine; /* countdown*/

extern unsigned char VillainAttacking[NUM_VILLAINS];
extern unsigned char VillainStaggered[NUM_VILLAINS];
extern int VillainWakingChance[NUM_VILLAINS];
extern int VillainStrength[NUM_VILLAINS];

extern int dummy; 

struct TREASURESCORE_STRUCT
{
  int obj;
  unsigned char take_value;
  unsigned char case_value;
  unsigned char flags; /* &1 taken  &2 put in case */
};

struct ROOMSCORE_STRUCT
{
  int room;
  unsigned char value;
  unsigned char flag;
};

extern struct TREASURESCORE_STRUCT TreasureScore[NUM_TREASURESCORES];
extern struct ROOMSCORE_STRUCT 	RoomScore[NUM_ROOMSCORES];

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

/* save-restore area ----------------------------------------*/

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
void ReturnOverlay0(short);

void PrintText(char *p);
void PrintCompText(char *comp_text);
int MatchCurWord( char *match);
int IsObjVisible(int obj);

void cleanfcb(void);
void setdisk(void);
void setdma(char*);
void setname(char*, short);
void setext(char*, short);
char openfiler(void);
void openfilew(void);
char readfile(void);
void writefile(void);
void closefile(void);

char* loadbyte(char* dest, char* source);
char* loadword(short* dest, char* source);
char* savebyte(char* dest, char* source);
char* saveword(char* dest, short* source);

/* variables ----------------------------------------------------------------------- */

/* code ---------------------------------------------------------------------------- */

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

char* p;
short i;

void SaveVars(void)
{
  p = AllocBuf;

  p = savebyte(p, (char*)&RugMoved          );
  p = savebyte(p, (char*)&TrapOpen          );
  p = savebyte(p, (char*)&ExitFound         );
  p = savebyte(p, (char*)&KitchenWindowOpen );
  p = savebyte(p, (char*)&GratingRevealed   );
  p = savebyte(p, (char*)&GratingUnlocked   );
  p = savebyte(p, (char*)&GratingOpen       );
  p = savebyte(p, (char*)&GatesOpen         );
  p = savebyte(p, (char*)&LowTide           );
  p = savebyte(p, (char*)&GatesButton       );
  p = savebyte(p, (char*)&LoudRoomQuiet     );
  p = savebyte(p, (char*)&RainbowSolid      );
  p = savebyte(p, (char*)&WonGame           );
  p = savebyte(p, (char*)&MirrorBroken      );
  p = savebyte(p, (char*)&RopeTiedToRail    );
  p = savebyte(p, (char*)&SpiritsBanished   );
  p = savebyte(p, (char*)&TrollAllowsPassage);
  p = savebyte(p, (char*)&YouAreSanta       );
  p = savebyte(p, (char*)&YouAreInBoat      );
  p = savebyte(p, (char*)&NotLucky          );
  p = savebyte(p, (char*)&YouAreDead        );
  p = savebyte(p, (char*)&SongbirdSang      );
  p = savebyte(p, (char*)&ThiefHere         );
  p = savebyte(p, (char*)&ThiefEngrossed    );
  p = savebyte(p, (char*)&YouAreStaggered   );
  p = savebyte(p, (char*)&BuoyFlag          );
  p = saveword(p, (short*)&NumMoves               );     
  p = saveword(p, (short*)&LampTurnsLeft          );     
  p = saveword(p, (short*)&MatchTurnsLeft         );     
  p = saveword(p, (short*)&CandleTurnsLeft        );     
  p = saveword(p, (short*)&MatchesLeft            );     
  p = saveword(p, (short*)&ReservoirFillCountdown );     
  p = saveword(p, (short*)&ReservoirDrainCountdown);     
  p = saveword(p, (short*)&MaintenanceWaterLevel  );     
  p = saveword(p, (short*)&DownstreamCounter      );     
  p = saveword(p, (short*)&BellRungCountdown      );     
  p = saveword(p, (short*)&CandlesLitCountdown    );     
  p = saveword(p, (short*)&BellHotCountdown       );     
  p = saveword(p, (short*)&CaveHoleDepth          );     
  p = saveword(p, (short*)&Score                  );     
  p = saveword(p, (short*)&NumDeaths              );     
  p = saveword(p, (short*)&CyclopsCounter         );     
  p = saveword(p, (short*)&CyclopsState           );     
  p = saveword(p, (short*)&LoadAllowed            );     
  p = saveword(p, (short*)&PlayerStrength         );     
  p = saveword(p, (short*)&TrollDescType          );     
  p = saveword(p, (short*)&ThiefDescType          );     
  p = saveword(p, (short*)&EnableCureRoutine      ); 
  p = saveword(p, (short*)&dummy		    );    
  for (i=0; i<NUM_VILLAINS; i++) 
  {                         
    p = savebyte(p, (char*)&VillainAttacking[i]);
    p = savebyte(p, (char*)&VillainStaggered[i]);
    p = saveword(p, (short*)&VillainWakingChance[i]);    
    p = saveword(p, (short*)&VillainStrength[i]); 
  }  
  for (i=0; i<NUM_TREASURESCORES; i++)                     
    {p = savebyte(p, (char*)&TreasureScore[i].flags);}
  for (i=0; i<NUM_ROOMSCORES; i++)                         
    {p = savebyte(p, (char*)&RoomScore[i].flag);}   
  for (i=0; i<NUM_ROOMS; i++)                              
    {p = saveword(p, (short*)&Room[i].prop);}   
  for (i=0; i<NUM_OBJECTS; i++) 
  {                          
    p = saveword(p, (short*)&Obj[i].loc       );    
    p = saveword(p, (short*)&Obj[i].order     );    
    p = saveword(p, (short*)&Obj[i].prop      );    
    p = savebyte(p, (char*)&Obj[i].thiefvalue);
  }
}

void LoadVars(void)
{
  p = AllocBuf;

  p = loadbyte((char*)&RugMoved          , p);
  p = loadbyte((char*)&TrapOpen          , p);
  p = loadbyte((char*)&ExitFound         , p);
  p = loadbyte((char*)&KitchenWindowOpen , p);
  p = loadbyte((char*)&GratingRevealed   , p);
  p = loadbyte((char*)&GratingUnlocked   , p);
  p = loadbyte((char*)&GratingOpen       , p);
  p = loadbyte((char*)&GatesOpen         , p);
  p = loadbyte((char*)&LowTide           , p);
  p = loadbyte((char*)&GatesButton       , p);
  p = loadbyte((char*)&LoudRoomQuiet     , p);
  p = loadbyte((char*)&RainbowSolid      , p);
  p = loadbyte((char*)&WonGame           , p);
  p = loadbyte((char*)&MirrorBroken      , p);
  p = loadbyte((char*)&RopeTiedToRail    , p);
  p = loadbyte((char*)&SpiritsBanished   , p);
  p = loadbyte((char*)&TrollAllowsPassage, p);
  p = loadbyte((char*)&YouAreSanta       , p);
  p = loadbyte((char*)&YouAreInBoat      , p);
  p = loadbyte((char*)&NotLucky          , p);
  p = loadbyte((char*)&YouAreDead        , p);
  p = loadbyte((char*)&SongbirdSang      , p);
  p = loadbyte((char*)&ThiefHere         , p);
  p = loadbyte((char*)&ThiefEngrossed    , p);
  p = loadbyte((char*)&YouAreStaggered   , p);
  p = loadbyte((char*)&BuoyFlag          , p);
  p = loadword((short*)&NumMoves               , p);     
  p = loadword((short*)&LampTurnsLeft          , p);     
  p = loadword((short*)&MatchTurnsLeft         , p);     
  p = loadword((short*)&CandleTurnsLeft        , p);     
  p = loadword((short*)&MatchesLeft            , p);     
  p = loadword((short*)&ReservoirFillCountdown , p);     
  p = loadword((short*)&ReservoirDrainCountdown, p);     
  p = loadword((short*)&MaintenanceWaterLevel  , p);     
  p = loadword((short*)&DownstreamCounter      , p);     
  p = loadword((short*)&BellRungCountdown      , p);     
  p = loadword((short*)&CandlesLitCountdown    , p);     
  p = loadword((short*)&BellHotCountdown       , p);     
  p = loadword((short*)&CaveHoleDepth          , p);     
  p = loadword((short*)&Score                  , p);     
  p = loadword((short*)&NumDeaths              , p);     
  p = loadword((short*)&CyclopsCounter         , p);     
  p = loadword((short*)&CyclopsState           , p);     
  p = loadword((short*)&LoadAllowed            , p);     
  p = loadword((short*)&PlayerStrength         , p);     
  p = loadword((short*)&TrollDescType          , p);     
  p = loadword((short*)&ThiefDescType          , p);     
  p = loadword((short*)&EnableCureRoutine      , p); 
  p = loadword((short*)&dummy		    , p);    
  for (i=0; i<NUM_VILLAINS; i++) 
  {                         
    p = loadbyte((char*)&VillainAttacking[i], p);
    p = loadbyte((char*)&VillainStaggered[i], p);
    p = loadword((short*)&VillainWakingChance[i], p);    
    p = loadword((short*)&VillainStrength[i], p); 
  }  
  for (i=0; i<NUM_TREASURESCORES; i++)                     
    {p = loadbyte((char*)&TreasureScore[i].flags, p);}
  for (i=0; i<NUM_ROOMSCORES; i++)                         
    {p = loadbyte((char*)&RoomScore[i].flag, p);}   
  for (i=0; i<NUM_ROOMS; i++)                              
    {p = loadword((short*)&Room[i].prop, p);}   
  for (i=0; i<NUM_OBJECTS; i++) 
  {                          
    p = loadword((short*)&Obj[i].loc       , p);    
    p = loadword((short*)&Obj[i].order     , p);    
    p = loadword((short*)&Obj[i].prop      , p);    
    p = loadbyte((char*)&Obj[i].thiefvalue, p);
  }
}

/*returns 1 if successful*/
int DoRestore(void)
{
  int slot;
  char ret;
  char* string = AllocBuf; 
  char* p = AllocBuf;

  GetWords("Restore from which slot (0-9; default=0; q to cancel)>");

  if (NumStrWords == 0) slot = 0;
  else
  {
    for (slot=0; slot<10; slot++)
    {
      string[0] = '0' + slot;
      string[1] = 0;
      if (MatchCurWord(string)) break;
    }
    if (slot == 10) 
    {
	PrintCompLine("\x2a\x2a\x2a\x20\x52\xbe\xbd\xa9\x91\x63\x65\xdf\xd5\xa4\x2a\x2a\x2a"); 
	ReturnOverlay0(0); /* return 0; */
    }
  }

  strcpy(string, SAVE_FILENAME);
  string[SAVE_FILENAME_NUMCHAR_INDEX] = '0' + slot;

  /* size = 0x37E; */ /* GetSaveStateSize(); */

  cleanfcb();
  setdisk();
  setname(string, 4);
  setext(string+5, 3);

  ret = openfiler();

  if (ret == (char)0xFF)  
  {
    PrintCompLine("\x46\x69\xcf\xe4\xff\xc6\xa5\x6e\x64\x2e");
    ReturnOverlay0(0); /* return 0; */
  }

  ret = (char)0;

  setdma(p);
  ret += readfile();
  setdma(p+0x80);
  ret += readfile();
  setdma(p+0x100);
  ret += readfile();
  setdma(p+0x180);
  ret += readfile();
  setdma(p+0x200);
  ret += readfile();
  setdma(p+0x280);
  ret += readfile();
  setdma(p+0x300);
  ret += readfile();

  closefile();

  if (ret != (char)0)
  {
    PrintCompLine("\x2a\x2a\x2a\x20\x52\xbe\xbd\xa9\xc6\x61\x69\xcf\x64\xa4\x2a\x2a\x2a");
    ReturnOverlay0(0); /* return 0; */
  }
  else
  {
    LoadVars();
   
    NumStrWords = 0;
    ItObj = 0;
    GameOver = 0;

    PrintCompLine("\x2a\x2a\x2a\x20\x52\xbe\xbd\xa9\xaa\x75\x63\x63\xbe\x73\x66\x75\x6c\xa4\x2a\x2a\x2a\x0a");
    PrintPlayerRoomDesc(1); /*force description*/

    ReturnOverlay0(1); /* return 1; */
  }
}

/*returns 1 if successful*/
int DoSave(void)
{
  int slot;
  char* string = AllocBuf; 
  char* p = AllocBuf;

  GetWords("Save to which slot (0-9; default=0; q to cancel)>");

  if (NumStrWords == 0) slot = 0;
  else
  {
    for (slot=0; slot<10; slot++)
    {
      string[0] = '0' + slot;
      string[1] = 0;
      if (MatchCurWord(string)) break;
    }
    if (slot == 10)
    {
	PrintCompLine("\x2a\x2a\x2a\x20\x53\x61\xd7\x91\x63\x65\xdf\xd5\xa4\x2a\x2a\x2a");
    	ReturnOverlay0(0); /* return 0; */
    }
  }

  strcpy(string, SAVE_FILENAME);
  string[SAVE_FILENAME_NUMCHAR_INDEX] = '0' + slot;

  /* size = 0x37E; */ /* GetSaveStateSize(); */

  cleanfcb();
  setdisk();
  setname(string, 4);
  setext(string+5, 3);

  openfilew();

  SaveVars();

  setdma(p);
  writefile();
  setdma(p+0x80);
  writefile();
  setdma(p+0x100);
  writefile();
  setdma(p+0x180);
  writefile();
  setdma(p+0x200);
  writefile();
  setdma(p+0x280);
  writefile();
  setdma(p+0x300);
  writefile();

  closefile();

  PrintCompLine("\x2a\x2a\x2a\x20\x53\x61\xd7\xaa\x75\x63\x63\xbe\x73\x66\x75\x6c\xa4\x2a\x2a\x2a");
  ReturnOverlay0(1); /* return 1; */
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

