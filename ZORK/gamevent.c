#include <ovr46.h>

/* defines, enums ------------------------------------------------------------- */

#define OVR_VILLAINS			46

#define NUM_OBJECTS  80
#define NUM_ROOMS  111 /* including null room 0 */
#define NUM_TREASURESCORES  19
#define NUM_ROOMSCORES      5

#define SCORE_MAX  350

#define INSIDE  2048

#define R_TINY_CAVE 53 
#define R_RESERVOIR_SOUTH 45
#define R_RESERVOIR 46
#define R_RESERVOIR_NORTH 47
#define R_DEEP_CANYON 61 
#define R_LOUD_ROOM 63
#define R_DAMP_CAVE 62 
#define R_IN_STREAM 49
#define R_RIVER_1 78
#define R_RIVER_2 79 
#define R_RIVER_3 80 
#define R_RIVER_4 83
#define R_RIVER_5 84
#define R_MAINTENANCE_ROOM 76 
#define R_DAM_ROOM 74 
#define R_DAM_LOBBY 75 
#define R_BAT_ROOM 96 
#define R_MINE_1 106 
#define R_MINE_2 107 
#define R_MINE_3 108 
#define R_MINE_4 109 
#define R_LADDER_TOP 100 
#define R_LADDER_BOTTOM 101 
#define R_MINE_ENTRANCE 94 
#define R_SQUEEKY_ROOM 95 
#define R_GRATING_CLEARING 12 
#define R_GRATING_ROOM 36 
#define R_GAS_ROOM 99
#define R_ENTRANCE_TO_HADES 66
#define R_ON_RAINBOW 89
#define R_DOME_ROOM 70 
#define R_TORCH_ROOM 71 
#define R_UP_A_TREE 11 
#define R_PATH 10 
#define R_CYCLOPS_ROOM 42
#define R_WEST_OF_HOUSE 1 
#define R_ROUND_ROOM 60

#define O_YOU 1
#define O_SWORD  61
#define O_LAMP  36
#define O_CANDLES  42
#define O_INFLATED_BOAT  39
#define O_BUOY  49
#define O_THIEF  5	
#define O_SCEPTRE  23
#define O_KNIFE  33
#define O_RUSTY_KNIFE  51
#define O_AXE  21
#define O_STILETTO  60
#define O_PUNCTURED_BOAT  45
#define O_GARLIC  26
#define O_LEAVES  44
#define O_MATCH  40
#define O_TORCH  64
#define O_BELL  18
#define O_HOT_BELL  19
#define O_GUNK  43
#define O_NEST  71
#define O_EGG  72
#define O_BROKEN_EGG  73
#define O_WATER  14
#define O_CYCLOPS  2
#define O_TROPHY_CASE  9
#define O_MAP  62

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

struct R_PASSAGES_STRUCT
{
  char passage[10];
};

struct TREASURESCORE_STRUCT
{
  int obj;
  unsigned char take_value;
  unsigned char case_value;
  unsigned char flags; /* &1 taken  &2 put in case */
};
extern struct TREASURESCORE_STRUCT TreasureScore[];

struct ROOMSCORE_STRUCT
{
  int room;
  unsigned char value;
  unsigned char flag;
};
extern struct ROOMSCORE_STRUCT RoomScore[];

extern struct R_PASSAGES_STRUCT RoomPassages[];

extern unsigned char LowTide;
extern unsigned char YouAreInBoat;
extern unsigned char LoudRoomQuiet;
extern unsigned char BuoyFlag;
extern unsigned char GratingRevealed;
extern unsigned char ExitFound; /* set this when player finds an exit from dungeon other than the trapdoor*/
extern unsigned char YouAreDead;
extern unsigned char GatesOpen;
extern unsigned char WonGame;

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

/* extern functions ---------------------------------------------------------------- */
void PrintCompLine(char *);
void PrintPlayerRoomDesc(int);
void PrintNewLine(void);
int ValidateObject(int obj);
void PrintObjectDesc(int , int );
void PrintContents(int , char *, int );
void PrintScore(void);
int IsObjVisible(int );
int IsPlayerInDarkness(void);
void PrintPlayerRoomDesc(int );
void YoureDead(void);
int GetRandom(int Range);
int PercentChance(int x, int x_not_lucky);
int AreYouInForest(void);

short CallOverlay0(void* Routine, char CalledRamBank);
void ReturnOverlay0(short ret_val);
void ReturnOverlay0_(short ret_val);
/* variables ----------------------------------------------------------------------- */
int check_room[7] = {R_RESERVOIR, R_IN_STREAM, R_RIVER_1, R_RIVER_2, R_RIVER_3,
                       R_RIVER_4, R_RIVER_5};
int pointy_obj[6] = {O_SCEPTRE, O_KNIFE, O_SWORD, O_RUSTY_KNIFE, O_AXE, O_STILETTO};
int  float_from[5] = {R_RIVER_1, R_RIVER_2, R_RIVER_3, R_RIVER_4, R_RIVER_5};
int    float_to[5] = {R_RIVER_2, R_RIVER_3, R_RIVER_4, R_RIVER_5, 0};
int float_speed[5] = {4, 4, 3, 2, 1};
int random_room[3] = {R_DAMP_CAVE, R_ROUND_ROOM, R_DEEP_CANYON};
int random_room_bat[8] = {R_MINE_1, R_MINE_2, R_MINE_3, R_MINE_4, R_LADDER_TOP,
                          R_LADDER_BOTTOM, R_SQUEEKY_ROOM, R_MINE_ENTRANCE};
char *water_level_msg[9] =
    {
      "up to your ankles.",
      "up to your shin.",
      "up to your knees.",
      "up to your hips.",
      "up to your waist.",
      "up to your chest.",
      "up to your neck.",
      "over your head.",
      "high in your lungs."
    };

/* overlays ----------------------------------------------------------------------- */
/* ------
void VillainsRoutine(void);

#define VillainsRoutine 0
-------- */
/* code ---------------------------------------------------------------------------- */

int IsActorInRoom(int room)
{
  int obj;

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == room &&
        (Obj[obj].prop & PROP_ACTOR) &&
        (Obj[obj].prop & PROP_NODESC) == 0)
    return 1;

  return 0;
}

/* thiefvalue for sword indicates glow level*/

void SwordRoutine(void)
{
  int glow, new_glow, i, room;

  if (Obj[O_SWORD].loc != INSIDE + O_YOU) return;

  glow = Obj[O_SWORD].thiefvalue;
  new_glow = 0;

  if (IsActorInRoom(Obj[O_YOU].loc))
    new_glow = 2;
  else
    for (i=0; i<10; i++)
  {
    room = RoomPassages[Obj[O_YOU].loc].passage[i];
    if (room > 0 && room < NUM_ROOMS && IsActorInRoom(room))
    {
      new_glow = 1;
      break;
    }
  }

  if (new_glow != glow)
  {
         if (new_glow == 0) PrintCompLine("\xdc\xd8\xaa\x77\xd3\xab\x9a\xe3\xcb\xca\x67\xac\xe6\xd9\xf8\x6e\x67\x2e");
    else if (new_glow == 1) PrintCompLine("\xdc\xd8\xaa\x77\xd3\xab\x9a\x67\xd9\xf8\x9c\xf8\xa2\xa3\xc6\x61\xa7\xa6\x62\x6c\x75\x9e\x67\xd9\x77\x2e");
    else                    PrintCompLine("\xdc\xd8\xaa\x77\xd3\xab\xcd\xa1\xef\x67\xf6\x89\x67\xd9\x77\x20\xd7\x72\xc4\x62\xf1\x67\x68\x74\x6c\x79\x2e");
    Obj[O_SWORD].thiefvalue = new_glow;
  }
}

void LampDrainRoutine(void)
{
  if (Obj[O_LAMP].loc == 0) return; /* destroyed by machine or lost*/

  if ((Obj[O_LAMP].prop & PROP_LIT) == 0) return;

  if (LampTurnsLeft > 0) LampTurnsLeft--;

  if (IsObjVisible(O_LAMP) && (Obj[O_LAMP].prop & PROP_NODESC) == 0)
    switch (LampTurnsLeft)
  {
    case 100: PrintCompLine("\x85\xfd\x6d\x70\xa3\x70\xfc\xbb\xa1\xd0\x62\xc7\xcc\x69\x6d\x6d\x65\x72\x2e");     break;
    case  70: PrintCompLine("\x85\xfd\x6d\x70\x87\xe8\x66\xa7\xc7\x65\xec\xcc\x69\x6d\x6d\xac\xe4\x6f\x77\x2e"); break;
    case  15: PrintCompLine("\x85\xfd\x6d\x70\x87\xed\xbb\xec\xae\x75\x74\x2e");            break;
  }

  if (LampTurnsLeft == 0)
  {
    int prev_darkness;

    prev_darkness = IsPlayerInDarkness();
    Obj[O_LAMP].prop &= ~PROP_LIT;
    if (IsPlayerInDarkness() != prev_darkness)
    {
      PrintNewLine();
      PrintPlayerRoomDesc(1);
    }
  }
}

/* also handles candles put out by dropping or draft*/

void CandlesShrinkRoutine(void)
{
  int prev_darkness;

  if (Obj[O_CANDLES].loc == 0) return; /* destroyed by machine or lost*/

  if ((Obj[O_CANDLES].prop & PROP_MOVEDDESC) == 0) return; /* still sitting on altar*/

  if ((Obj[O_CANDLES].prop & PROP_LIT) == 0) return; /* not lit*/

  if (CandleTurnsLeft > 0) CandleTurnsLeft--;

  if (IsObjVisible(O_CANDLES))
    switch (CandleTurnsLeft)
  {
    case 20: PrintCompLine("\x85\xe7\xb9\xcf\xa1\x67\xc2\x77\xaa\x68\xd3\xd1\x72\x2e");                           break;
    case 10: PrintCompLine("\x85\xe7\xb9\xcf\xa1\xbb\x9e\xef\x63\xe1\x84\x71\x75\xc7\x9e\x73\x68\xd3\x74\x2e");               break;
    case  5: PrintCompLine("\x85\xe7\xb9\xcf\xa1\x77\xca\x27\xa6\xfd\xc5\xcb\xca\xc1\xe3\x77\x2e");                    break;
    case  0: PrintCompLine("\xdc\x75\x27\xab\xef\x74\xd1\xb6\xcd\xd7\xee\xd3\x9e\xf5\x67\x68\xa6\xa2\xad\xc6\xc2\xf9\x81\xe7\xb9\xcf\x73\x2e"); break;
  }

  prev_darkness = IsPlayerInDarkness();

  if (CandleTurnsLeft == 0)
    Obj[O_CANDLES].prop &= ~PROP_LIT;
  else
  {
    if (Obj[O_CANDLES].loc != INSIDE + O_YOU)
    {
      Obj[O_CANDLES].prop &= ~PROP_LIT;
      if (IsObjVisible(O_CANDLES)) PrintCompLine("\x85\xe7\xb9\xcf\xa1\x67\xba\xa5\x74\x2e");
    }
    else if (Obj[O_YOU].loc == R_TINY_CAVE && PercentChance(50, 80))
    {
      Obj[O_CANDLES].prop &= ~PROP_LIT;
      if (IsObjVisible(O_CANDLES)) PrintCompLine("\x41\xe6\xfe\xa6\xdd\xb7\xa7\xab\x62\xd9\x77\xa1\xa5\xa6\x92\x91\x64\xcf\x73\x21");
    }
  }

  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}

void ReservoirFillRoutine(void)
{
  if (ReservoirFillCountdown == 0) return;
      ReservoirFillCountdown--;
  if (ReservoirFillCountdown > 0) return;

  Room[R_RESERVOIR  ].prop |= R_BODYOFWATER;
  Room[R_DEEP_CANYON].prop &= ~R_DESCRIBED;
  Room[R_LOUD_ROOM  ].prop &= ~R_DESCRIBED;

  LowTide = 0;

  switch (Obj[O_YOU].loc)
  {
    case R_RESERVOIR:
      if (YouAreInBoat)
        PrintCompLine("\x85\x62\x6f\xaf\xcb\x69\x66\x74\xa1\x67\xd4\x74\xec\xae\xf7\x8a\x81\x6d\x75\xab"
"\x8c\x9a\xe3\x77\xc6\xd9\xaf\x84\xca\x80\xda\xbe\xac\x76\x6f\x69\x72\x2e");
      else
      {
        PrintCompLine("\x8b\xbb\x9e\xf5\x66\xd1\xab\x75\x70\xb0\xc4\x81\xf1\x73\x84\xf1\xd7\x72\x21\x88\x9f\x72\xc4\xbd\xaa\xf8\x6d"
"\xb5\x62\xf7\x80\xb3\xd8\xa9\xe5\xa1\xbb\x9e\xbd\xba\xc5\xc2\xb1\x8e\xc3\x63\xe1\x9e\x63\xd9\xd6\x72\xb5\x63"
"\xd9\xd6\xb6\xbd\x80\xa3\x77\xbe\xe1\x9e\xc5\x72\x75\x63\x74\xd8\x9e\xdd\x20\x46\xd9\x6f\xab\x43\xca\x74\xc2"
"\xea\x44\x61\xf9\x23\x33\x83\x9e\x64\x61\xf9\xef\x63\x6b\xca\xa1\xbd\x86\x83\x9e\xc2\xbb\x8a\x81\x77\xaf\xac"
"\xe4\xbf\x72\xec\xcc\xbf\x66\xd4\xa1\xc9\x75\xb5\x62\xf7\x86\xda\x65\x6d\x61\xa7\xb3\xca\x73\x63\x69\xa5\xa1"
"\xe0\x86\x9f\x75\x6d\x62\xcf\xae\xd7\xb6\x81\x64\x61\xf9\xbd\x77\xbb\xab\x92\xb3\xac\x74\x61\xa7\xcc\xe9\xf9"
"\x61\x6d\xca\xc1\x81\xc2\x63\x6b\xa1\xaf\xa8\x74\xa1\x62\xe0\x65\x2e");
        YoureDead(); /* ##### RIP #####*/
      }
    break;

    case R_DEEP_CANYON:
      PrintCompLine("\x41\xaa\xa5\xb9\xb5\xf5\x6b\x9e\xa2\xaf\x8a\x66\xd9\xf8\x9c\x77\xaf\xac\xb5\xc5\xbb\x74"
"\xa1\xbd\xb3\xe1\x9e\x66\xc2\xf9\xef\xd9\x77\x2e");
    break;

    case R_LOUD_ROOM:
      if (LoudRoomQuiet == 0)
      {
        PrintCompLine("\x41\xdf\x8a\xd0\x73\x75\x64\xe8\x6e\xb5\xad\xa3\xfd\x72\x6d\x97\xec\xcb\xa5\xab\xc2\xbb"
"\x84\x73\xa5\xb9\xc6\x69\xdf\xa1\x81\xc2\xe1\xa4\x46\x69\xdf\xd5\xb7\xc7\xde\x66\xbf\x72\xb5\x8f\x73\x63\xf4"
"\x6d\x62\xcf\xa3\x77\x61\x79\x2e\x0a");
        YouAreInBoat = 0; /* in case you're in it*/
        GoToRoutine(random_room[GetRandom(3)]);
      }
    break;

    case R_RESERVOIR_NORTH:
    case R_RESERVOIR_SOUTH:
      PrintCompLine("\x8b\xe3\xf0\x63\x9e\xa2\xaf\x80\xb7\xaf\xac\xcb\x65\xd7\xea\xcd\xa1\xf1\xd6\xb4\xbd"
"\x80\xeb\x6f\xa7\xa6\xa2\xaf\xa8\xa6\x9a\x69\x6d\x70\x6f\x73\x73\x69\x62\xcf\x89\x63\xc2\x73\x73\x2e");
    break;
  }
}

void ReservoirDrainRoutine(void)
{
  if (ReservoirDrainCountdown == 0) return;
      ReservoirDrainCountdown--;
  if (ReservoirDrainCountdown > 0) return;

  Room[R_RESERVOIR  ].prop &= ~R_BODYOFWATER;
  Room[R_DEEP_CANYON].prop &= ~R_DESCRIBED;
  Room[R_LOUD_ROOM  ].prop &= ~R_DESCRIBED;

  LowTide = 1;

  switch (Obj[O_YOU].loc)
  {
    case R_RESERVOIR:
      if (YouAreInBoat)
        PrintCompLine("\x85\x77\xaf\xac\xcb\x65\xd7\xea\xcd\xa1\x64\xc2\x70\xfc\xab\xbd\x80\xeb\x6f"
"\xa7\xa6\xaf\xb7\xce\xfa\x80\xb0\x6f\xaf\x91\xe4\xba\xd9\xb1\xac\xaa\x74\x61\xc4\x61\x66\xd9\xaf"
"\xa4\x49\xa6\x73\xa7\x6b\xa1\xa7\xbd\x80\xee\x75\x64\x2e");
    break;

    case R_DEEP_CANYON:
      PrintCompLine("\x85\xc2\xbb\x8a\x72\xfe\xce\x9c\x77\xaf\xac\x87\x71\x75\x69\x65\xd1\xb6\xe3\x77\x2e");
    break;

    case R_RESERVOIR_NORTH:
    case R_RESERVOIR_SOUTH:
      PrintCompLine("\x85\x77\xaf\xac\xcb\x65\xd7\xea\x9a\xe3\x77\x20\x71\x75\xc7\x9e\xd9\x77\xc0"
"\xac\x9e\x8c\x8f\x63\xa5\x6c\xab\xbf\x73\x69\xec\xb3\xc2\x73\xa1\x6f\xd7\xb6\xbd\x80\xae\x96\xb6\x73\x69\x64\x65\x2e");
    break;
  }
}

void SinkingObjectsRoutine(void)
{
  int obj, i;

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (obj != O_INFLATED_BOAT && obj != O_BUOY && obj != O_THIEF)
      for (i=0; i<7; i++)
        if (Obj[obj].loc == check_room[i])
  {
    if ((Room[check_room[i]].prop & R_BODYOFWATER) && (Obj[obj].prop & PROP_NODESC) == 0)
    {
      /* if room is filled with water and object hasn't sunk, sink object*/
      Obj[obj].prop |= PROP_NODESC;
      Obj[obj].prop |= PROP_NOTTAKEABLE;
    }
    else if ((Room[check_room[i]].prop & R_BODYOFWATER) == 0 && (Obj[obj].prop & PROP_NODESC))
    {
      /* if room is not filled with water and object has sunk, unsink object*/
      Obj[obj].prop &= ~PROP_NODESC;
      Obj[obj].prop &= ~PROP_NOTTAKEABLE;
    }
  }
}

void LoudRoomRoutine(void)
{
  if (Obj[O_YOU].loc == R_LOUD_ROOM && LoudRoomQuiet == 0 && GatesOpen && LowTide == 0)
  {
    PrintCompLine("\x49\xa6\x9a\xf6\xef\xbb\x61\x62\xec\xcb\xa5\xab\xa0\xa9\xb5\xf8\xa2\xa3\xb4"
"\xbf\x72\x2d\x73\x70\xf5\x74\xf0\x9c\xc2\xbb\xaa\xf3\x6d\x84\xbd\xb3\xe1\x9e\x66\xc2\xf9\xe2"
"\xea\xbb\xa5\xb9\x86\x83\xac\x9e\x9a\xd0\x70\xa5\xb9\x84\xa7\x86\xb6\xa0\x61\xab\x77\xce\xfa"
"\xb7\xca\x27\xa6\xc5\x6f\x70\xa4\x57\xc7\xde\xd0\x74\xa9\x6d\xd4\x64\xa5\xa1\x65\x66\x66\xd3"
"\x74\xb5\x8f\x73\x63\xf4\x6d\x62\xcf\xae\xf7\x8a\x81\xc2\xe1\x2e\x0a");

    YouAreInBoat = 0; /* in case you're in it*/

    GoToRoutine(random_room[GetRandom(3)]);
  }
}

void MaintenanceLeakRoutine(void)
{
  if (MaintenanceWaterLevel <= 0 || MaintenanceWaterLevel > 16) return;

  if (Obj[O_YOU].loc == R_MAINTENANCE_ROOM)
  {
    PrintCompText("\x85\x77\xaf\xac\xcb\x65\xd7\xea\xa0\xa9\x87\xe3\x77\x20");

    PrintLine(water_level_msg[MaintenanceWaterLevel / 2]);
  }

  MaintenanceWaterLevel++;
  if (MaintenanceWaterLevel > 16 && Obj[O_YOU].loc == R_MAINTENANCE_ROOM)
  {
    PrintCompLine("\x49\x27\xf9\x61\x66\xf4\x69\xab\x8f\xcd\xd7\xcc\xca\x9e\x64\xc2\x77"
"\xed\xab\x92\xd6\x6c\x66\x2e");
    if (YouAreInBoat)
      switch (Obj[O_YOU].loc)
    {
      case R_MAINTENANCE_ROOM:
      case R_DAM_ROOM:
      case R_DAM_LOBBY:
        PrintCompLine("\x85\xf1\x73\x84\x77\xaf\xac\xb3\xbb\xf1\xbe\x80\xb0\x6f\xaf\xae"
"\xd7\xb6\x81\x64\x61\x6d\xb5\x64\xf2\xb4\x81\xf1\xd7\x72\xb5\x8c\x6f\xd7\xb6\x81\x66\xe2"
"\x6c\x73\x9d\x73\x6b\xb5\x74\x73\x6b\x2e");
      break;
    }
    YoureDead(); /* ##### RIP #####*/
  }
}

void BoatPuncturedRoutine(void)
{
  int i, flag;

  flag = 0;
  for (i=0; i<6; i++)
    if (Obj[pointy_obj[i]].loc == INSIDE + O_INFLATED_BOAT)
  {
    flag = 1;
    Obj[pointy_obj[i]].loc = Obj[O_INFLATED_BOAT].loc;
  }
  if (flag == 0) return;

  PrintCompLine("\x49\xa6\xd6\x65\x6d\xa1\xa2\xaf\xaa\xe1\x65\xa2\x84\x70\x6f\xa7\x74\xc4"
"\x64\x69\x64\x93\x61\x67\xa9\x9e\xf8\xa2\x80\xb0\x6f\xaf\xb5\xe0\xfb\x76\x69\xe8\x6e\x63"
"\xd5\xb0\xc4\x81\xd9\x75\xab\xce\x73\x73\x84\xe3\xb2\x9e\xb2\x73\x75\x84\x96\xa9\x66\xc2"
"\x6d\xa4\x57\xc7\xde\xd0\x70\xaf\xa0\xf0\x63\xaa\x70\xf7\xd1\x72\xb5\x81\x62\x6f\xaf\xcc"
"\x65\x66\xfd\xd1\x73\xb5\xcf\x61\x76\x84\x8f\xf8\xa2\xa5\x74\x2e");

  Obj[O_PUNCTURED_BOAT].loc = Obj[O_INFLATED_BOAT].loc;
  Obj[O_INFLATED_BOAT].loc = 0;

  if (YouAreInBoat) YouAreInBoat = 0;

  if (Room[Obj[O_YOU].loc].prop & R_BODYOFWATER)
  {
    if (Obj[O_YOU].loc == R_RESERVOIR || Obj[O_YOU].loc == R_IN_STREAM)
      PrintCompLine("\x41\xe3\x96\xb6\x70\xaf\xa0\xf0\x63\xaa\x70\xf7\xd1\x72\xb5\xa2\x9a"
"\xf0\x6d\x9e\x66\xc2\xf9\xc9\x75\xb5\xa0\xf4\x6c\x64\xa1\x92\xcc\xc2\x77\x6e\x97\x2e");
    else
      PrintCompLine("\x49\xb4\xff\xa0\xb6\x77\xd3\x64\x73\xb5\x66\x69\x67\x68\xf0\x9c\x81"
"\x66\x69\xac\x63\x9e\x63\xd8\xa9\xe5\xa1\xdd\x80\x20\x46\xf1\x67\x69\xab\x52\x69\xd7\x72"
"\x8e\xc3\x6d\xad\x61\x67\x9e\xbd\xc0\x6f\x6c\xab\x92\xae\x77\xb4\x66\xd3\xa3\xb0\xc7\xb5"
"\x62\xf7\x80\xb4\x8f\xbb\x9e\xe7\x72\xf1\xd5\xae\xd7\xb6\xd0\x77\xaf\xac\x66\xe2\xea\x8c"
"\xa7\xbd\xaa\xe1\x9e\x6e\xe0\x74\xc4\xc2\x63\x6b\x73\xa4\x4f\x75\x63\x68\x21");
    YoureDead(); /* ##### RIP #####*/
  }
}

void BuoyRoutine(void)
{
  if (BuoyFlag == 0 && Obj[O_BUOY].loc == INSIDE + O_YOU)
  {
    BuoyFlag = 1;
    PrintCompLine("\x8b\xe3\xf0\x63\x9e\x73\xe1\x65\xa2\x84\x66\xf6\x6e\xc4\x61\x62\xa5\xa6"
"\x81\x66\xf3\xea\xdd\x80\xb0\x75\x6f\x79\x2e");
  }
}

void DownstreamRoutine(void)
{
  int i;

  for (i=0; i<5; i++)
    if (Obj[O_YOU].loc == float_from[i]) break;
  if (i == 5) return;

  DownstreamCounter++;
  if (DownstreamCounter < float_speed[i]) return;

  if (float_to[i] == 0)
  {
    PrintCompLine("\x55\x6e\x66\xd3\x74\xf6\xaf\x65\xec\xb5\x81\x6d\x61\x67\x69\x63\xb0\x6f"
"\xaf\xcc\x6f\xbe\x93\x70\xc2\x76\x69\xe8\xeb\xc2\xd1\x63\xf0\xca\xc6\xc2\xf9\x81\xc2\x63"
"\x6b\xa1\x8c\x62\xa5\x6c\xe8\x72\xa1\xca\x9e\x6d\xf3\x74\xa1\xaf\x80\xb0\xff\xbd\xf9\xdd"
"\xb7\xaf\xac\x66\xe2\x6c\x73\xa4\x49\x6e\x63\x6c\x75\x64\x84\xa2\x9a\xca\x65\x2e");
    YoureDead(); /* ##### RIP #####*/
    return;
  }

  PrintCompLine("\x85\x66\xd9\x77\x8a\x81\xf1\xd7\xb6\xe7\x72\xf1\xbe\x86\xcc\xf2\x6e\xc5\xa9\x61\x6d\x2e\x0a");
  DownstreamCounter = 0;
  BoatGoToRoutine(float_to[i]);
}

void BatRoomRoutine(void)
{
  if (Obj[O_YOU].loc == R_BAT_ROOM && IsObjVisible(O_GARLIC) == 0)
  {
    PrintCompLine("\x20\x20\x20\x20\x46\x77\xf3\x70\x21\x0a\x20\x20\x20\x20\x46\x77\xf3\x70"
"\x21\x0a\x20\x20\x20\x20\x46\x77\xf3\x70\x21\x0a\x85\x62\xaf\xe6\xf4\x62\xa1\x8f\x62\xc4"
"\x81\x73\x63\x72\x75\x66\xd2\xdd\x86\xb6\xed\x63\x6b\x8d\xcb\x69\x66\x74\xa1\x8f\x61\x77"
"\x61\x79\x2e\x2e\x2e\x2e\x0a");
    GoToRoutine(random_room_bat[GetRandom(8)]);
  }
}

void LeavesTakenRoutine(void)
{
  if (Obj[O_YOU].loc == R_GRATING_CLEARING &&
      Obj[O_LEAVES].loc != R_GRATING_CLEARING &&
      GratingRevealed == 0)
  {
    GratingRevealed = 1;
    PrintCompLine("\x57\xc7\xde\x81\xcf\x61\xd7\xa1\x6d\x6f\xd7\x64\xb5\xd0\x67\xf4\xf0"
"\x9c\x9a\xa9\xd7\xe2\x65\x64\x2e");
  }

  /* also reveal grating just by being in grating room*/
  if (Obj[O_YOU].loc == R_GRATING_ROOM)
    GratingRevealed = 1;
}

/* must call before match routine*/

void GasRoomRoutine(void)
{
  if (Obj[O_YOU].loc == R_GAS_ROOM)
  {
    int match   = (Obj[O_MATCH  ].loc == INSIDE + O_YOU && (Obj[O_MATCH  ].prop & PROP_LIT));
    int candles = (Obj[O_CANDLES].loc == INSIDE + O_YOU && (Obj[O_CANDLES].prop & PROP_LIT));
    int torch   = (Obj[O_TORCH  ].loc == INSIDE + O_YOU && (Obj[O_TORCH  ].prop & PROP_LIT));
    int type = 0; /* 1: lighted  2: carried*/

    if (match && MatchTurnsLeft == 2)
      type = 1;
    else if (match || candles || torch)
      type = 2;

    if (type)
    {
      if (type == 1)
        PrintCompLine("\x48\xf2\xaa\x61\xab\x66\xd3\xa3\xb4\xe0\x70\x69\xf1\x9c\x61\x64"
"\xd7\xe5\xd8\xac\x89\xf5\x67\x68\xa6\xd0\x6d\xaf\xfa\xa8\xb4\xd0\xc2\xe1\xb7\xce\xfa"
"\xda\xf3\x6b\xa1\xdd\xe6\xe0\xa4\x46\xd3\x74\xf6\xaf\x65\xec\xb5\x96\xa9\x87\x6a\xfe"
"\xf0\x63\x9e\xa7\x80\xb7\xd3\x6c\x64\x2e");
      else
        PrintCompLine("\x4f\xde\xe8\xbb\xa4\x49\xa6\x61\x70\xfc\xbb\xa1\xa2\xaf\x80"
"\xaa\x6d\x65\xdf\xb3\xe1\x84\x66\xc2\xf9\xa2\x9a\xc2\xe1\xb7\xe0\xb3\x6f\xe2\xe6"
"\xe0\xa4\x49\xb7\xa5\x6c\xab\xcd\xd7\x95\xa5\x67\x68\xa6\x74\xf8\x63\x9e\x61\x62"
"\xa5\xa6\xe7\x72\x72\x79\x84\x66\xfd\x6d\x84\x6f\x62\x6a\x65\x63\x74\xa1\xa7\xc0\xac\x65\x2e");
      PrintCompLine("\x0a\x20\x20\x20\x20\x20\x20\x2a\x2a\x20\x42\x4f\x4f\x4f\x4f"
"\x4f\x4f\x4f\x4f\x4f\x4f\x4f\x4d\x20\x2a\x2a");
      YoureDead(); /* ##### RIP #####*/
    }
  }
}

void MatchRoutine(void)
{
  if (MatchTurnsLeft == 0) return;

  MatchTurnsLeft--;
  if (MatchTurnsLeft == 0)
  {
    int prev_darkness;

    if (IsObjVisible(O_MATCH))
      PrintCompLine("\x85\x6d\xaf\xfa\xc0\xe0\xe6\xca\x9e\xa5\x74\x2e");

    prev_darkness = IsPlayerInDarkness();
    Obj[O_MATCH].prop &= ~PROP_LIT;
    if (IsPlayerInDarkness() != prev_darkness)
    {
      PrintNewLine();
      PrintPlayerRoomDesc(1);
    }
  }
}

void CeremonyBroken(void)
{
  if (Obj[O_YOU].loc == R_ENTRANCE_TO_HADES)
    PrintCompLine("\x85\xd1\x6e\x73\x69\xca\x8a\xa2\x9a\x63\xac\x65\x6d\xca\xc4\x9a\x62"
"\xc2\x6b\xd4\xb5\x8c\x81\x77\xf4\xc7\x68\x73\xb5\x61\x6d\xfe\xd5\xb0\xf7\xaa\xcd\x6b"
"\xd4\xa3\xa6\x92\xb3\x6c\x75\x6d\x73\xc4\xaf\xd1\x6d\x70\x74\xb5\xa9\x73\x75\x6d\x9e"
"\x96\x69\xb6\xce\xe8\xa5\xa1\x6a\xf3\xf1\x6e\x67\x2e");
}

void BellRungRoutine(void)
{
  if (BellRungCountdown == 0) return;
      BellRungCountdown--;
  if (BellRungCountdown == 0)
    CeremonyBroken();
}

void CandlesLitRoutine(void)
{
  if (CandlesLitCountdown == 0) return;
      CandlesLitCountdown--;
  if (CandlesLitCountdown == 0)
    CeremonyBroken();
}

void BellHotRoutine(void)
{
  if (BellHotCountdown == 0) return;
      BellHotCountdown--;
  if (BellHotCountdown == 0)
  {
    if (Obj[O_YOU].loc == R_ENTRANCE_TO_HADES)
      PrintCompLine("\x85\xef\xdf\xa3\x70\xfc\xbb\xa1\xbd\xc0\x61\xd7\xb3\xe9\xcf\xab\x64\xf2\x6e\x2e");

    Obj[O_BELL].loc = R_ENTRANCE_TO_HADES;
    Obj[O_HOT_BELL].loc = 0;
  }
}

void HoldingGunkRoutine(void)
{
  if (Obj[O_GUNK].loc == INSIDE + O_YOU)
  {
    Obj[O_GUNK].loc = 0;
    PrintCompLine("\x85\x73\xfd\xc1\x77\xe0\xda\xaf\xa0\xb6\xa7\x73\x75\x62\xc5\xad\xf0"
"\xe2\xb5\x8c\x63\x72\x75\x6d\x62\xcf\xa1\xa7\xbd\xcc\xfe\xa6\xaf\x86\xb6\xbd\x75\x63\x68\x2e");
  }
}

void InRoomOnRainbowRoutine(void)
{
  if (Obj[O_YOU].loc == R_ON_RAINBOW)
    ExitFound = 1;
}

void DomeRoomRoutine(void)
{
  if (Obj[O_YOU].loc == R_DOME_ROOM && YouAreDead)
  {
    PrintCompLine("\x41\xa1\x8f\xd4\xd1\xb6\x81\x64\xe1\x9e\x8f\x66\xf3\xea\xd0\xc5\xc2"
"\x9c\x70\x75\xdf\xa3\xa1\x69\xd2\x66\xc2\xf9\xd0\xf8\xb9\xcc\xf4\xf8\x9c\x8f\x6f\xd7"
"\xb6\x81\xf4\x69\xf5\x9c\x8c\x64\xf2\x6e\x2e\x0a");
    GoToRoutine(R_TORCH_ROOM);
  }
}

void UpATreeRoutine(void)
{
  int obj, other_fell = 0, count = 0;

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == R_UP_A_TREE)
  {
    if (obj == O_NEST)
    {
      if (Obj[obj].prop & PROP_MOVEDDESC)
      {
        count++;
        Obj[obj].loc = R_PATH;
        if (Obj[O_EGG].loc == INSIDE + O_NEST)
        {
          other_fell = 1;
          Obj[O_EGG].loc = 0;
          Obj[O_BROKEN_EGG].loc = R_PATH;
        }
      }
    }
    else if (obj == O_EGG)
    {
      other_fell = 2;
      count++;
      Obj[O_EGG].loc = 0;
      Obj[O_BROKEN_EGG].loc = R_PATH;
      Obj[O_BROKEN_EGG].prop |= PROP_OPENABLE;
      Obj[O_BROKEN_EGG].prop |= PROP_OPEN;
    }
    else
    {
      count++;
      Obj[obj].loc = R_PATH;
    }
  }

  if (count == 1 && other_fell == 0)
    PrintCompLine("\x49\xa6\x66\xe2\x6c\xa1\xbd\x80\xe6\xc2\xf6\x64\x2e");
  else if (count > 1)
    PrintCompLine("\x99\xc4\x66\xe2\xea\xbd\x80\xe6\xc2\xf6\x64\x2e");

  if (other_fell == 1)
    PrintCompLine("\x85\xed\xc5\xc6\xe2\x6c\xa1\xbd\x80\xe6\xc2\xf6\x64\xb5\x8c\x81\x65"
"\x67\xc1\x73\x70\x69\xdf\xa1\xa5\xa6\xdd\xa8\x74\xb5\xd6\xf1\xa5\x73\xec\xcc\x61\x6d\x61\x67\x65\x64\x2e");
  else if (other_fell == 2)
    PrintCompLine("\x85\x65\x67\xc1\x66\xe2\x6c\xa1\xbd\x80\xe6\xc2\xf6\xab\x8c\x73\x70"
"\xf1\xb1\xa1\x6f\xfc\x6e\xb5\xd6\xf1\xa5\x73\xec\xcc\x61\x6d\x61\x67\x65\x64\x2e");
}

void SongbirdRoutine(void)
{
  if (AreYouInForest() && PercentChance(15, -1))
    PrintCompLine("\x8b\xa0\xbb\xa8\xb4\x81\x64\xb2\x74\xad\x63\x9e\x81\xfa\x69\x72\x70"
"\x84\xdd\xa3\xaa\xca\xc1\x62\x69\x72\x64\x2e");
}

void WaterSpilledRoutine(void)
{
  if (Obj[O_WATER].loc == Obj[O_YOU].loc)
  {
    Obj[O_WATER].loc = 0;
    PrintCompLine("\x85\x77\xaf\xac\xaa\x70\x69\xdf\xa1\xbd\x80\xc6\xd9\xd3\x8d\xfb\x76"
"\x61\x70\xd3\xaf\x65\x73\x2e");
  }
}

void CyclopsRoomRoutine(void)
{
  if (Obj[O_YOU].loc != R_CYCLOPS_ROOM)
    {CyclopsCounter = 0; return;}

  if (CyclopsState >= 3 ||                  /* asleep or fled*/
      VillainAttacking[VILLAIN_CYCLOPS] ||  /* attacking*/
      Obj[O_CYCLOPS].loc == 0)            /* dead*/
    return;

  CyclopsCounter++;

  if (CyclopsState >= 1) /* hungry or thirsty*/
  {
    switch (CyclopsCounter - 1)
    {
      case 0: PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\xd6\x65\x6d\xa1\x73\xe1\x65"
"\x77\xcd\xa6\x61\x67\xc7\xaf\x65\x64\x2e"); break;
      case 1: PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x61\x70\xfc\xbb\xa1\xbd\xb0"
"\x9e\x67\x65\x74\xf0\x9c\x6d\xd3\x9e\x61\x67\xc7\xaf\x65\x64\x2e"); break;
      case 2: PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x9a\x6d\x6f\x76\x84\x61\x62"
"\xa5\xa6\x81\xc2\xe1\xb5\xd9\x6f\x6b\x84\x66\xd3\xaa\xe1\x65\xa2\x97\x2e"); break;
      case 3: PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x77\xe0\xcb\xe9\x6b\x84\x66"
"\xd3\xaa\xe2\xa6\x8c\xfc\x70\xfc\x72\xa4\x4e\xba\x64\xa5\x62\xa6\x96\xc4\xbb\x9e\x63"
"\xca\x64\x69\x6d\xd4\x74\xa1\x66\xd3\xc0\x9a\x75\x70\x63\xe1\x84\x73\x6e\x61\x63\x6b\x2e"); break;
      case 4: PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x9a\x6d\x6f\x76\x84\xbd\x77\xbb"
"\xab\x8f\xa7\xa3\xb4\xf6\x66\xf1\xd4\x64\xec\xee\xad\xed\x72\x2e"); break;
      case 5: PrintCompLine("\x8b\xcd\xd7\x9f\x77\xba\xfa\x6f\x69\x63\xbe\x3a\x20\x31\xa4"
"\x4c\xbf\xd7\x20\x20\x32\xa4\x42\x65\x63\xe1\x9e\x64\xa7\xed\x72\x2e"); break;
      case 6: PrintCompLine("\x85\x63\x79\x63\xd9\x70\x73\xb5\xf0\xa9\xab\xdd\xa3\xdf\x8a"
"\x92\xe6\x61\x6d\xbe\x8d\x9f\xf1\x63\x6b\xac\x79\xb5\x67\xf4\x62\xa1\x8f\x66\x69\x72\x6d"
"\xec\xa4\x41\xa1\x94\xf5\x63\x6b\xa1\xce\xa1\xfa\x6f\x70\x73\xb5\x94\x73\x61\x79\xa1\x22"
"\x4d\x6d\x6d\xa4\x4a\xfe\xa6\xf5\x6b\x9e\x4d\xe1\x20\xfe\xd5\x89\x6d\x61\x6b\x9e\x27\x65"
"\x6d\x2e\x22\x20\x49\x74\x27\xa1\x6e\x69\x63\x9e\xbd\xb0\x9e\x61\x70\x70\xa9\x63\x69\xaf\x65\x64\x2e"); break;
    }

    if (CyclopsCounter == 7)
      YoureDead(); /* ##### RIP #####*/
  }
  else if (CyclopsCounter == 5)
  {
    CyclopsCounter = 0;
    CyclopsState = 1; /* hungry*/
  }
}

/*-----------------------------------------------------------------------------*/

void ScoreUpdateRoutine(void)
{
  int i, old_score;

  old_score = Score;

  for (i=0; i<NUM_TREASURESCORES; i++)
  {
    int loc = Obj[TreasureScore[i].obj].loc;

    if (loc == INSIDE + O_YOU && (TreasureScore[i].flags & 1) == 0)
    {
      TreasureScore[i].flags |= 1;
      Score += TreasureScore[i].take_value;
    }
    else if (loc == INSIDE + O_TROPHY_CASE && (TreasureScore[i].flags & 2) == 0)
    {
      TreasureScore[i].flags |= 2;
      Score += TreasureScore[i].case_value;
    }
  }

  for (i=0; i<NUM_ROOMSCORES; i++)
    if (Obj[O_YOU].loc == RoomScore[i].room && RoomScore[i].flag == 0)
  {
    RoomScore[i].flag = 1;
    Score += RoomScore[i].value;
  }

  if (Score - old_score > 0)
  {
    /* PrintCompText("\x5b\xdc\xd8\xaa\x63\xd3\x9e\x6a\xfe\xa6\x77\xd4\xa6\x75\x70\xb0\x79\x20");*/
    /* PrintInteger(Score - old_score);*/
    /* if (Score - old_score == 1) PrintCompLine("\xeb\x6f\xa7\x74\x2e\x5d");*/
    /* else                        PrintCompLine("\xeb\x6f\xa7\x74\x73\x2e\x5d");*/
  }

  if (Score == SCORE_MAX && WonGame == 0)
  {
    WonGame = 1;
    Obj[O_MAP].prop &= ~PROP_NODESC;
    Obj[O_MAP].prop &= ~PROP_NOTTAKEABLE;
    Room[R_WEST_OF_HOUSE].prop &= ~R_DESCRIBED;
    PrintCompLine("\x41\xb4\xe2\x6d\x6f\xc5\xa8\x6e\x61\x75\x64\x69\x62\xcf\x20\x76\x6f\x69"
"\x63\x9e\x77\xce\x73\xfc\x72\xa1\xa7\x86\xb6\xbf\x72\xb5\x22\x4c\xe9\x6b\x89\x92\x9f\xa9"
"\xe0\xd8\xbe\xc6\xd3\x80\xc6\xa7\xe2\xaa\x65\x63\xa9\x74\x2e\x22");
  }
}

/*run event routines after each action that set time-passed flag*/
void RunEventRoutines(void)
{
  SwordRoutine();
  LampDrainRoutine();
  CandlesShrinkRoutine();
  ReservoirFillRoutine();
  ReservoirDrainRoutine();
  SinkingObjectsRoutine(); /* must be called after reservoir fill/drain routines*/
  LoudRoomRoutine();
  MaintenanceLeakRoutine();
  BoatPuncturedRoutine();
  BuoyRoutine(); /* should be called before downstream routine because of message order*/
  DownstreamRoutine();
  BatRoomRoutine();
  LeavesTakenRoutine();
  GasRoomRoutine(); /* must be called before match routine*/
  MatchRoutine();
  BellRungRoutine();
  CandlesLitRoutine();
  BellHotRoutine();
  HoldingGunkRoutine();
  InRoomOnRainbowRoutine();
  DomeRoomRoutine();
  UpATreeRoutine();
  SongbirdRoutine();
  WaterSpilledRoutine();
  CyclopsRoomRoutine();
  ScoreUpdateRoutine();

  CallOverlay0((void*)VillainsRoutine, OVR_VILLAINS);
/*  VillainsRoutine(); */

  ReturnOverlay0_(0);
}
