/* defines, enums ------------------------------------------------------------- */
#define LOAD_MAX  100
#define NUM_ROOMS  111 /* including null room 0 */
#define NUM_OBJECTS  80

#define INSIDE  2048

#define F_RAILING 2089
#define F_WHITE_HOUSE 2055
#define F_FRONT_DOOR 2081
#define F_LEAK 2085

#define O_YOU 1
#define O_LAMP  36
#define O_COFFIN 29
#define O_SWORD  61
#define O_CYCLOPS 2
#define O_BAT 4
#define O_THIEF 5
#define O_TROLL 6
#define O_TROPHY_CASE 9
#define O_MACHINE 10
#define O_MAILBOX 11
#define O_KITCHEN_TABLE 12
#define O_ATTIC_TABLE 13
#define O_WATER 14
#define O_HOT_BELL 19
#define O_BOTTLE 28
#define O_TOOL_CHEST 53
#define O_ROPE 50
#define O_RUSTY_KNIFE 51
#define O_CHALICE 25
#define O_LARGE_BAG 59
#define O_STILETTO 60
#define O_AXE 21
#define O_SWORD  61
#define O_KNIFE 33
#define O_EGG 72
#define O_GHOSTS 3
#define O_WRENCH 70
#define O_PUTTY 68
#define O_PUMP 30
#define O_INFLATABLE_BOAT 46
#define O_INFLATED_BOAT 39
#define O_BOAT_LABEL 63
#define O_PUNCTURED_BOAT 45
#define O_MATCH 40
#define O_CANDLES 42
#define O_TORCH 64
#define O_SCREWDRIVER 54
#define O_COAL 57
#define O_DIAMOND 31
#define O_GUNK 43
#define O_SHOVEL 56
#define O_LEAVES 44
#define O_BOOK 20
#define O_SANDWICH_BAG 24
#define O_ADVERTISEMENT 38
#define O_PAINTING 41
#define O_BOAT_LABEL 63
#define O_GUIDE 65
#define O_NEST 71
#define O_BELL 18
#define O_TUBE 67
#define O_SCARAB 58
#define O_KEYS 55 

#define R_FOREST_1 6
#define R_FOREST_2 7
#define R_FOREST_3 9
#define R_PATH 10 
#define R_UP_A_TREE 11
#define R_ENTRANCE_TO_HADES 66
#define R_SOUTH_TEMPLE 73
#define R_WEST_OF_HOUSE 1
#define R_NORTH_OF_HOUSE 3
#define R_SOUTH_OF_HOUSE 4
#define R_EAST_OF_HOUSE 5
#define R_PATH 10
#define R_UP_A_TREE 11
#define R_GRATING_CLEARING 12
#define R_CLEARING 13
#define R_CANYON_VIEW 93
#define R_EGYPT_ROOM 69
#define R_LIVING_ROOM 16
#define R_TREASURE_ROOM 44
#define R_DOME_ROOM 70
#define R_RESERVOIR_SOUTH 45
#define R_LOUD_ROOM 63
#define R_GRATING_ROOM 36
#define R_LOWER_SHAFT 104
#define R_TIMBER_ROOM 103
#define R_SANDY_CAVE 87

enum
{
  BLOW_NULL,
  BLOW_MISSED,         /* attacker misses*/
  BLOW_UNCONSCIOUS,    /* defender unconscious*/
  BLOW_KILLED,         /* defender dead*/
  BLOW_LIGHT_WOUND,    /* defender lightly wounded*/
  BLOW_SERIOUS_WOUND,  /* defender seriously wounded*/
  BLOW_STAGGER,        /* defender staggered (miss turn)*/
  BLOW_LOSE_WEAPON,    /* defender loses weapon*/
  BLOW_HESITATE,       /* hesitates (miss on free swing)*/
  BLOW_SITTING_DUCK    /* sitting duck (crunch!)*/
};

enum
{
  VILLAIN_TROLL,
  VILLAIN_THIEF,
  VILLAIN_CYCLOPS,

  NUM_VILLAINS
};

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

/*room properties bit flags*/
#define R_DESCRIBED     1  /* set when room description already printed */
#define R_BODYOFWATER   2
#define R_LIT           4  /* set when there is natural light or a light fixture */
#define R_WATERHERE     8
#define R_SACRED       16  /* set when thief not allowed in room */
#define R_MAZE         32
#define R_ALWAYSDESC   64

#define MAX_INVENTORY_ITEMS  7
#define INV_LIMIT_CHANCE     8

#define BL0 233 /* (256-23) */

#define STRENGTH_MIN  2
#define STRENGTH_MAX  7

#define SCORE_MAX  350

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

extern int ItObj, PrevItObj; /*the obj "it" refers to*/
extern unsigned char RopeTiedToRail;
extern unsigned char LowTide;
extern unsigned char YouAreInBoat;
extern unsigned char LoudRoomQuiet;
extern unsigned char BuoyFlag;
extern unsigned char GratingRevealed;
extern unsigned char ExitFound; /* set this when player finds an exit from dungeon other than the trapdoor*/
extern unsigned char YouAreDead;
extern unsigned char GatesOpen;
extern unsigned char WonGame;
extern unsigned char NotLucky;
extern unsigned char GatesButton;
extern unsigned char GameOver; 
extern unsigned char TrollAllowsPassage;
extern unsigned char TimePassed; /*flag: time passed during action*/
extern unsigned char GratingUnlocked;
extern unsigned char GratingOpen;
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
extern unsigned char ThiefEngrossed;
extern unsigned char YouAreStaggered;

extern int VillainStrength[];

/* extern functions ---------------------------------------------------------------- */
void PrintText(char *p);
void PrintLine(char *p);
void PrintNewLine(void);
void PrintCompText(char *comp_text);
void PrintCompLine(char *);
void PrintObjectDesc(int obj, int desc_flag);
void PrintPlayerRoomDesc(int force_description);
int IsPlayerInDarkness(void);
void PrintUsingMsg(int obj);

void ReturnOverlay1(short);
short CallOverlay1(void* routine, short Par1, char bank);

/* variables ----------------------------------------------------------------------- */

/* overlays ----------------------------------------------------------------------- */
/* code ---------------------------------------------------------------------------- */
void PrintFutileMsg(int obj)
{
  PrintCompText("\x55\x73\x84");

  if (obj > 0 && obj < NUM_OBJECTS)
    PrintObjectDesc(obj, 0);
  else
    PrintCompText("\xa2\x61\x74");

  PrintCompText("\xb7\xa5\x6c\xab\xef\xc6\xf7\x69\xcf\x2e\x0a");
}

void TieRopeToRailingRoutine(void)
{
  if (RopeTiedToRail) {PrintCompLine("\x85\xc2\xfc\x87\xe2\xa9\x61\x64\xc4\xf0\xd5\x89\x69\x74\x2e"); return;}

  RopeTiedToRail = 1;
  Obj[O_ROPE].loc = R_DOME_ROOM;
  Obj[O_ROPE].prop |= PROP_NODESC;
  Obj[O_ROPE].prop |= PROP_NOTTAKEABLE;

  PrintCompLine("\x85\xc2\xfc\xcc\xc2\x70\xa1\x6f\xd7\xb6\x81\x73\x69\xe8\x8d"
"\xb3\xe1\xbe\xb7\xc7\xce\xb4\xd1\xb4\x66\xf3\xa6\xdd\x80\xc6\xd9\x6f\x72\x2e");

  TimePassed = 1;
}

void DMWT_tie_rope_ext(int with_to)
{
  if (with_to == 0 && Obj[O_YOU].loc == R_DOME_ROOM) {with_to = F_RAILING; PrintCompLine("\x28\xbd\xda\x61\x69\xf5\x6e\x67\x29");}
  if (with_to == 0) {PrintCompLine("\x50\xcf\xe0\x9e\x73\xfc\x63\x69\x66\xc4\x77\xcd\xa6\xbd\x9f\x69\x9e\xc7\x9f\x6f\x2e"); 
ReturnOverlay1(0); /* return; */}
  if (with_to != F_RAILING) {PrintCompLine("\x8b\xe7\x93\xf0\x9e\x81\xc2\xfc\x89\xa2\x61\x74\x2e"); 
ReturnOverlay1(0); /* return; */}

  TieRopeToRailingRoutine();
  ReturnOverlay1(0);
}

void DMWT_untie_rope_ext(int with_to)
{
  if (with_to == 0 && Obj[O_YOU].loc == R_DOME_ROOM && RopeTiedToRail) PrintCompLine("\x28\x66\xc2\xf9\xf4\x69\xf5\x6e\x67\x29");
  if (with_to != 0 && with_to != F_RAILING) {PrintCompLine("\x85\xc2\xfc\xa8\x73\x93\xf0\xd5\x89\xa2\x61\x74\x2e"); ReturnOverlay1(0); /* return; */}

  if (RopeTiedToRail == 0) {PrintCompLine("\x49\xa6\x9a\xe3\xa6\xf0\xd5\x89\xad\x79\xa2\x97\x2e"); ReturnOverlay1(0); /* return; */}

  RopeTiedToRail = 0;
  Obj[O_ROPE].prop &= ~PROP_NODESC;
  Obj[O_ROPE].prop &= ~PROP_NOTTAKEABLE;

  PrintCompLine("\x85\xc2\xfc\x87\xe3\x77\x20\xf6\xf0\x65\x64\x2e");

  TimePassed = 1;
  ReturnOverlay1(0);
}

void DMWT_i_f_iated_boat_ext(int with_to)
{
  PrintCompLine("\x49\x6e\x66\xfd\xf0\x9c\xc7\xc6\xd8\x96\xb6\x77\xa5\x6c\xab\x70\xc2\x62\x61\x62\xec\xb0\xd8\xc5\xa8\x74\x2e");
  ReturnOverlay1(0);
}

void DMWT_i_f_p_boat_ext(int with_to)
{
  PrintCompLine("\x4e\xba\xfa\xad\x63\x65\xa4\x53\xe1\x9e\x6d\xd3\xca\xeb\xf6\x63\x74\xd8\xd5\xa8\x74\x2e");
  ReturnOverlay1(0);
}

void DMWT_d_ied_boat_ext(int with_to)
{
  if (YouAreInBoat) {PrintCompLine("\x8b\xe7\x93\xe8\x66\xfd\xd1\x80\xb0\x6f\xaf\xb7\xce\xcf\x86\x27\xa9\xa8\xb4\x69\x74\x2e"); ReturnOverlay1(0); /* return; */}

  if (Obj[O_INFLATED_BOAT].loc != Obj[O_YOU].loc) {PrintCompLine("\x85\x62\x6f\xaf\xee\xfe\xa6\xef\xae\xb4\x81\x67\xc2\xf6\xab\xbd\xb0\x9e\xe8\x66\xfd\xd1\x64\x2e"); ReturnOverlay1(0); /* return; */}

  TimePassed = 1;

  PrintCompLine("\x85\x62\x6f\xaf\xcc\x65\x66\xfd\xd1\x73\x2e");
  ItObj = O_INFLATABLE_BOAT;

  Obj[O_INFLATABLE_BOAT].loc = Obj[O_INFLATED_BOAT].loc;
  Obj[O_INFLATED_BOAT].loc = 0;
  ReturnOverlay1(0);
}

void DMWT_d_iable_boat_ext(int with_to)
{
  PrintCompLine("\x49\x74\x27\xa1\xe2\xa9\x61\x64\xc4\xe8\x66\xfd\xd1\x64\x2e");
  ReturnOverlay1(0);
}

void DMWT_d_pun_boat_ext(int with_to)
{
  PrintCompLine("\x49\x74\x27\xa1\xe2\xa9\x61\x64\xc4\xe8\x66\xfd\xd1\x64\x2e");
  ReturnOverlay1(0);
}

void DMWT_act_machine_ext(int with_to)
{
  int found, obj;

  if (with_to == 0)
    {PrintCompLine("\x49\x74\x27\xa1\xe3\xa6\x63\xcf\xbb\xc0\xf2\x89\x74\xd8"
"\xb4\xc7\xae\xb4\xf8\xa2\x86\xb6\x62\xbb\x9e\xcd\xb9\x73\x2e"); ReturnOverlay1(0); /* return; */}

  if (with_to != O_SCREWDRIVER)
    {PrintCompLine("\x49\xa6\xd6\x65\x6d\xa1\xa2\xaf\xb7\xca\x27\xa6\x64\x6f\x2e"); ReturnOverlay1(0); /* return; */}

  if (Obj[O_SCREWDRIVER].loc != INSIDE + O_YOU)
    {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x81\x73\x63\xa9\x77\x64\xf1\xd7\x72\x2e"); ReturnOverlay1(0); /* return; */}

  if (Obj[O_MACHINE].prop & PROP_OPEN)
    {PrintCompLine("\x85\x6d\x61\xfa\xa7\x9e\x64\x6f\xbe\x93\xd6\x65\xf9\xbd"
"\xb7\xad\xa6\xbd\xcc\xba\xad\x79\xa2\x97\x2e"); ReturnOverlay1(0); /* return; */}

  TimePassed = 1;
  PrintCompLine("\x85\x6d\x61\xfa\xa7\x9e\x63\xe1\xbe\x89\xf5\x66\x9e\x28\x66"
"\x69\x67\xd8\xaf\x69\xd7\xec\x29\xb7\xc7\xde\xd0\x64\x61\x7a\x7a\xf5\x9c\x64"
"\xb2\x70\xfd\xc4\xdd\xb3\x6f\xd9\xa9\xab\xf5\x67\x68\x74\xa1\x8c\x62\x69\x7a"
"\xbb\xa9\xe4\x6f\xb2\xbe\xa4\x41\x66\xd1\xb6\xd0\x66\x65\x77\xee\xe1\xd4\x74"
"\x73\xb5\x81\x65\x78\x63\xc7\x65\x6d\xd4\xa6\x61\x62\xaf\x65\x73\x2e");

  found = 0;
  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == INSIDE + O_MACHINE)
  {
    if (found == 0) found = 1;
    if (obj == O_COAL) found = 2;
    Obj[obj].loc = 0;
  }

  if (found == 2)
    Obj[O_DIAMOND].loc = INSIDE + O_MACHINE;
  else if (found == 1)
    Obj[O_GUNK].loc = INSIDE + O_MACHINE;
  ReturnOverlay1(0);
}

void DMWT_fill_bottle_ext(int with_to)
{
  if (with_to == 0 && (Room[Obj[O_YOU].loc].prop & R_WATERHERE))
    {with_to = O_WATER; PrintCompLine("\x28\xf8\xa2\xb7\xaf\x65\x72\x29");}

  if (with_to == 0) {PrintCompLine("\x46\x69\xdf\xa8\xa6\xf8\xa2\xb7\xcd\x74\x3f"); ReturnOverlay1(0); /* return; */}
  if (with_to != O_WATER) {PrintCompLine("\x8b\xe7\x93\x66\x69\xdf\xa8\xa6\xf8\xa2\x95\x61\x74\x21"); ReturnOverlay1(0); /* return; */}
  if ((Room[Obj[O_YOU].loc].prop & R_WATERHERE) == 0) {PrintCompLine("\x99\xa9\x27\xa1\xe3\xb7\xaf\xac\xc0\xac\x65\x21"); ReturnOverlay1(0); /* return; */}
  if ((Obj[O_BOTTLE].prop & PROP_OPEN) == 0)
  {
    PrintCompLine("\x85\x62\xff\x74\xcf\x87\x63\xd9\xd6\x64\x2e");
    ItObj = O_BOTTLE;
    ReturnOverlay1(0); /* return; */
  }
  if (Obj[O_WATER].loc == INSIDE + O_BOTTLE) {PrintCompLine("\x85\x62\xff\x74\xcf\x87\xe2\xa9\x61\x64\xc4\x66\x75\xdf\x8a\x77\xaf\x65\x72\x2e"); ReturnOverlay1(0); /* return; */}

  TimePassed = 1;
  Obj[O_WATER].loc = INSIDE + O_BOTTLE;
  PrintCompLine("\x85\x62\xff\x74\xcf\x87\xe3\x77\xc6\x75\xdf\x8a\x77\xaf\x65\x72\x2e");
  ReturnOverlay1(0);
}

void DMWT_oil_bolt_ext(int with_to)
{
  if (with_to == 0)
    PrintCompLine("\x4f\x69\xea\xc7\xb7\xc7\xde\x77\xcd\x74\x3f");
  else if (with_to != O_PUTTY)
    PrintCompLine("\x8b\xe7\x93\x6f\x69\xea\xc7\xb7\xc7\xde\xa2\x61\x74\x21");
  else if (Obj[with_to].loc != INSIDE + O_YOU)
    PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x69\x74\x2e");
  else
  {
    TimePassed = 1;
    PrintCompLine("\x48\x6d\x6d\xa4\x49\xa6\x61\x70\xfc\xbb\xa1\x81\x74\x75\xef"
"\xb3\xca\x74\x61\xa7\xd5\xe6\x6c\x75\x65\xb5\xe3\xa6\x6f\x69\x6c\x9d\xd8\x6e"
"\x84\x81\x62\x6f\x6c\xa6\x77\xca\x27\xa6\x67\x65\xa6\xad\xc4\xbf\x73\x69\xac\x2e\x2e\x2e\x2e");
  }
  ReturnOverlay1(0);
}

void DMWT_pour_putty_ext(int with_to)
{
  if (Obj[O_PUTTY].loc != INSIDE + O_YOU &&
      ( Obj[O_TUBE].loc != INSIDE + O_YOU ||
        Obj[O_PUTTY].loc != INSIDE + O_TUBE))
    PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x69\x74\x2e");
  else if ((Obj[O_TUBE].prop & PROP_OPEN) == 0)
    PrintCompLine("\x85\x74\x75\xef\x87\x63\xd9\xd6\x64\x2e");
  else if (with_to == 0)
    PrintCompLine("\x8b\xed\xd5\x89\x70\x90\xc7\xae\xb4\x73\xe1\x65\xa2\x97\x2e");
  else
    switch (with_to)
  {
    case F_LEAK:
      if (MaintenanceWaterLevel <= 0)
        PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a"
"\x65\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21");
      else
      {
        TimePassed = 1;
        MaintenanceWaterLevel = -1;
        PrintCompLine("\x42\xc4\x73\xe1\x9e\x6d\x69\xf4\x63\xcf\x8a\x5a\xd3\x6b"
"\x69\xad\x9f\x65\xfa\xe3\xd9\x67\x79\xb5\x8f\xcd\xd7\xee\xad\x61\x67\xd5\x89"
"\xc5\x6f\x70\x80\xcb\xbf\x6b\xa8\xb4\x81\x64\x61\x6d\x2e");
      }
    break;

    case O_PUNCTURED_BOAT:
      TimePassed = 1;
      Obj[O_INFLATABLE_BOAT].loc = Obj[O_PUNCTURED_BOAT].loc;
      Obj[O_PUNCTURED_BOAT].loc = 0;
      PrintCompLine("\x57\x65\xdf\xcc\xca\x65\x83\x9e\x62\x6f\xaf\x87\xa9\x70\x61\x69\xa9\x64\x2e");
    break;

    default: /* note that this includes with_to >= NUM_OBJECTS*/
      PrintCompLine("\xbc\xaf\xb7\xa5\x6c\xab\xef\xc6\xf7\x69\x6c\x65\x2e");
    break;
  }
  ReturnOverlay1(0);
}

void DMWT_turn_book_ext(int with_to)
{
  PrintCompLine("\x42\xbe\x69\xe8\xeb\x61\x67\x9e\x35\x36\x39\xb5\x96\xa9\x87"
"\xca\xec\xae\xed\xae\x96\xb6\x70\x61\x67\x9e\xf8\xa2\xa3\x6e\xc4\xcf\x67\x69"
"\x62\xcf\xeb\xf1\xe5\x84\xca\xa8\x74\xa4\x4d\x6f\xc5\x8a\xc7\x87\xf6\xa9\x61"
"\x64\x61\x62\xcf\xb5\x62\xf7\x80\xaa\x75\x62\x6a\x65\x63\xa6\xd6\x65\x6d\xa1"
"\xbd\xb0\x9e\x81\x62\xad\xb2\x68\x6d\xd4\xa6\xdd\xfb\x76\x69\x6c\xa4\x41\x70"
"\x70\xbb\xd4\x74\xec\xb5\x63\xac\x74\x61\xa7\xe4\x6f\xb2\xbe\xb5\xf5\x67\x68"
"\x74\x73\xb5\x8c\x70\xf4\x79\xac\xa1\xbb\x9e\x65\x66\x66\x69\xe7\x63\x69\xa5"
"\xa1\xa7\x95\x9a\xa9\x67\xbb\x64\x2e");
  ReturnOverlay1(0);
}

void DMWT_dea_torch_ext(int with_to)
{
  PrintCompLine("\x8b\xed\xbb\xec\xb0\xd8\xb4\x92\xc0\x8c\x74\x72\x79\x84\xbd\xfb\x78\xf0\xb1\x75\xb2\xde\x81\x66\xfd\x6d\x65\x2e");
  ReturnOverlay1(0);
}

void DMWT_act_torch_ext(int with_to)
{
  PrintCompLine("\x49\x74\x27\xa1\xe2\xa9\x61\x64\xc4\x62\xd8\x6e\x97\x2e");
  ReturnOverlay1(0);
}

void DoMisc_open_grate(void)
{
  int leaves_fall = 0, prev_darkness;

  if (GratingRevealed == 0) 
  {PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95"
"\x6f\xd6\xae\x62\x6a\x65\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); 
    }
  if (GratingOpen) 
  {PrintCompLine("\x85\x67\xf4\xf0\x9c\x9a\xe2\xa9\x61\x64\xc4\x6f\xfc\x6e\x2e"); 
  }
  if (GratingUnlocked == 0) 
  {PrintCompLine("\x85\x67\xf4\xf0\x9c\x9a\xd9\x63\x6b\x65\x64\x2e"); 
  }

  TimePassed = 1;
  GratingOpen = 1;

  if ((Obj[O_LEAVES].prop & PROP_MOVEDDESC) == 0)
  {
    leaves_fall = 1;
    Obj[O_LEAVES].prop |= PROP_MOVEDDESC;
    Obj[O_LEAVES].loc = R_GRATING_ROOM;
  }

  if (Obj[O_YOU].loc == R_GRATING_CLEARING)
    PrintCompLine("\x85\x67\xf4\xf0\x9c\x6f\xfc\x6e\x73\x2e");
  else
  {
    PrintCompLine("\x85\x67\xf4\xf0\x9c\x6f\xfc\x6e\xa1\xbd\xda\x65\xd7\xe2\x9f"
"\xa9\xbe\xa3\x62\x6f\xd7\x86\x2e");
    if (leaves_fall)
      PrintCompLine("\x41\xeb\x69\xcf\x8a\xcf\x61\xd7\xa1\x66\xe2\x6c\xa1\xca"
"\xbd\x86\xb6\xa0\x61\xab\x8c\xbd\x80\xe6\xc2\xf6\x64\x2e");
  }

  prev_darkness = IsPlayerInDarkness();
  Room[R_GRATING_ROOM].prop |= R_LIT; /* light spilling from grate opening*/
  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(0);
  }
  ReturnOverlay1(0);
}

void LockUnlockGrating(int with_to, int lock_flag)
{
  int need = O_KEYS;

  if (GratingRevealed == 0) {PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95\x6f\xd6\xae\x62\x6a\x65\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21");  return; }

  if (with_to == 0 && Obj[need].loc == INSIDE + O_YOU) {with_to = need; PrintUsingMsg(with_to);}
  if (with_to == 0) {PrintCompLine("\xdc\x75\x27\xdf\xe4\xf3\xab\xbd\x20\xfe\x9e\x73\xe1\x65\xa2\x97\x2e"); return;}
  if (with_to != need) {PrintFutileMsg(with_to); return;}
  if (Obj[with_to].loc != INSIDE + O_YOU) {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x69\x74\x2e"); return;}

  if (lock_flag)
  {
    if (GratingUnlocked == 0)
      PrintCompLine("\x85\x67\xf4\xf0\x9c\x9a\xe2\xa9\x61\x64\xc4\xd9\x63\x6b\x65\x64\x2e");
    else if (Obj[O_YOU].loc == R_GRATING_CLEARING)
      PrintCompLine("\x8b\xe7\x93\xd9\x63\x6b\xa8\xa6\x66\xc2\xf9\xa2\x9a\x73\x69\x64\x65\x2e");
    else
    {
      int prev_darkness;

      PrintCompLine("\x85\x67\xf4\xf0\x9c\x9a\xd9\x63\x6b\x65\x64\x2e");

      TimePassed = 1;
      GratingUnlocked = 0;
      GratingOpen = 0; /* grating may already be closed here*/

      prev_darkness = IsPlayerInDarkness();
      Room[R_GRATING_ROOM].prop &= ~R_LIT; /* no light spilling from grate opening*/
      if (IsPlayerInDarkness() != prev_darkness)
      {
        PrintNewLine();
        PrintPlayerRoomDesc(0);
      }
    }
  }
  else /*unlock*/
  {
    if (GratingUnlocked)
      PrintCompLine("\x85\x67\xf4\xf0\x9c\x9a\xe2\xa9\x61\x64\xc4\xf6\xd9\x63\x6b\x65\x64\x2e");
    else if (Obj[O_YOU].loc == R_GRATING_CLEARING)
      PrintCompLine("\x8b\xe7\x93\xa9\x61\xfa\x80\xcb\x6f\x63\x6b\xc6\xc2\xf9\xa0\x72\x65\x2e");
    else
    {
      TimePassed = 1;
      GratingUnlocked = 1;
      /* grating is closed here*/

      DoMisc_open_grate();
    }
  }
}

void DMWT_lock_grate_ext(int with_to)
{
  LockUnlockGrating(with_to, 1); /*1: lock*/
  ReturnOverlay1(0);
}

void DMWT_unlock_grate_ext(int with_to)
{
  LockUnlockGrating(with_to, 0); /*0: unlock*/
  ReturnOverlay1(0);
}

void DMWT_tie_railing_ext(int with_to)
{
  if (with_to == 0 && (Obj[O_ROPE].loc == INSIDE + O_YOU || Obj[O_ROPE].loc == R_DOME_ROOM))
  {
    with_to = O_ROPE;
    PrintUsingMsg(with_to);
  }
  if (with_to == 0) {PrintCompLine("\x50\xcf\xe0\x9e\x73\xfc\x63\x69\x66\xc4\x77\xcd\xa6\xbd\x9f\x69\x9e\xc7\xb7\xc7\x68\x2e"); ReturnOverlay1(0); /* return; */}
  if (with_to != O_ROPE) {PrintCompLine("\x8b\xe7\x93\xf0\x9e\x81\xf4\x69\xf5\x9c\xf8\xa2\x95\x61\x74\x2e"); ReturnOverlay1(0); /* return; */}

  TieRopeToRailingRoutine();
  ReturnOverlay1(0);
}

void DMWT_turn_bolt_ext(int with_to)
{
  int need = O_WRENCH;

  if (with_to == 0 && Obj[need].loc == INSIDE + O_YOU) {with_to = need; PrintUsingMsg(with_to);}
  if (with_to == 0) {PrintCompLine("\x85\x62\x6f\x6c\xa6\x77\xca\x27\xa6\x74\xd8\xb4\xf8\xa2\x86\xb6\xef\xc5\xfb\x66\x66\xd3\x74\x2e"); ReturnOverlay1(0); /* return; */}
  if (with_to != need) {PrintFutileMsg(with_to); ReturnOverlay1(0); /* return; */}
  if (Obj[with_to].loc != INSIDE + O_YOU) {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x69\x74\x2e"); ReturnOverlay1(0); /* return; */}

  if (GatesButton)
  {
    TimePassed = 1;
    Room[R_RESERVOIR_SOUTH].prop &= ~R_DESCRIBED;

    if (GatesOpen)
    {
      GatesOpen = 0;
      Room[R_LOUD_ROOM].prop &= ~R_DESCRIBED;
      ReservoirFillCountdown = 8;
      ReservoirDrainCountdown = 0;
      PrintCompLine("\x85\x73\x6c\x75\x69\x63\x9e\x67\xaf\xbe\xb3\xd9\xd6\x8d"
"\xb7\xaf\xac\xaa\x74\xbb\x74\xa1\xbd\xb3\x6f\xdf\x65\x63\xa6\xef\xce\xb9\x80\xcc\x61\x6d\x2e");
    }
    else
    {
      GatesOpen = 1;
      ReservoirFillCountdown = 0;
      ReservoirDrainCountdown = 8;
      PrintCompLine("\x85\x73\x6c\x75\x69\x63\x9e\x67\xaf\xbe\xae\xfc\xb4\x8c"
"\x77\xaf\xac\xeb\xa5\x72\xa1\xa2\xc2\x75\x67\xde\x81\x64\x61\x6d\x2e");
    }
  }
  else
    PrintCompLine("\x85\x62\x6f\x6c\xa6\x77\xca\x27\xa6\x74\xd8\xb4\xf8\xa2\x86\xb6\xef\xc5\xfb\x66\x66\xd3\x74\x2e");
  ReturnOverlay1(0);
}

void DMWT_f_pun_boat_ext(int with_to)
{
  int need = O_PUTTY;

  if (with_to == 0 && Obj[need].loc == INSIDE + O_YOU) {with_to = need; PrintUsingMsg(with_to);}
  if (with_to == 0) {PrintCompLine("\x46\x69\x78\xa8\xa6\xf8\xa2\xb7\xcd\x74\x3f"); ReturnOverlay1(0); /* return; */}
  if (with_to != need) {PrintFutileMsg(with_to); ReturnOverlay1(0); /* return; */}
  if (Obj[with_to].loc != INSIDE + O_YOU) {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x69\x74\x2e"); ReturnOverlay1(0); /* return; */}

  TimePassed = 1;

  Obj[O_INFLATABLE_BOAT].loc = Obj[O_PUNCTURED_BOAT].loc;
  Obj[O_PUNCTURED_BOAT].loc = 0;

  PrintCompLine("\x57\x65\xdf\xcc\xca\x65\x83\x9e\x62\x6f\xaf\x87\xa9\x70\x61\x69\xa9\x64\x2e");
  ReturnOverlay1(0);
}

void DMWT_i_f_ible_boat_ext(int with_to)
{
  int need = O_PUMP;

  if (with_to == 0 && Obj[need].loc == INSIDE + O_YOU) {with_to = need; PrintUsingMsg(with_to);}
  if (with_to == 0) {PrintCompLine("\x8b\x64\xca\x27\xa6\xcd\xd7\xfb\xe3\x75\x67\xde\x6c\xf6\xc1\x70\xf2\xac\x89\xa7\x66\xfd\xd1\xa8\x74\x2e"); ReturnOverlay1(0); /* return; */}
  if (with_to != need) {PrintFutileMsg(with_to); ReturnOverlay1(0); /* return; */}
  if (Obj[with_to].loc != INSIDE + O_YOU) {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x81\x70\x75\x6d\x70\x2e"); ReturnOverlay1(0); /* return; */}

  if (Obj[O_INFLATABLE_BOAT].loc != Obj[O_YOU].loc) {PrintCompLine("\x85\x62\x6f\xaf\xee\xfe\xa6\xef\xae\xb4\x81\x67\xc2\xf6\xab\xbd\xb0\x9e\xa7\x66\xfd\xd1\x64\x2e"); ReturnOverlay1(0); /* return; */}

  TimePassed = 1;

  PrintCompLine("\x85\x62\x6f\xaf\xa8\x6e\x66\xfd\xd1\xa1\x8c\x61\x70\xfc\xbb\xa1\xd6\x61\x77\xd3\xa2\x79\x2e");
  ItObj = O_INFLATED_BOAT;

  if ((Obj[O_BOAT_LABEL].prop & PROP_MOVEDDESC) == 0)
    PrintCompLine("\x41\x9f\xad\xcb\x61\xef\xea\x9a\xec\x84\xa7\x73\x69\xe8\x80\xb0\x6f\x61\x74\x2e");

  Obj[O_INFLATED_BOAT].loc = Obj[O_INFLATABLE_BOAT].loc;
  Obj[O_INFLATABLE_BOAT].loc = 0;
  ReturnOverlay1(0);
}

void DMWT_fix_leak_ext(int with_to)
{
  int need = O_PUTTY;

  if (MaintenanceWaterLevel <= 0) {PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e"
"\xdd\x95\x6f\xd6\xae\x62\x6a\x65\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); ReturnOverlay1(0); /* return; */}

  if (with_to == 0 && Obj[need].loc == INSIDE + O_YOU) {with_to = need; PrintUsingMsg(with_to);}
  if (with_to == 0) {PrintCompLine("\x46\x69\x78\xa8\xa6\xf8\xa2\xb7\xcd\x74\x3f"); ReturnOverlay1(0); /* return; */}
  if (with_to != need) {PrintFutileMsg(with_to); ReturnOverlay1(0); /* return; */}
  if (Obj[with_to].loc != INSIDE + O_YOU) {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x69\x74\x2e"); ReturnOverlay1(0); /* return; */}

  TimePassed = 1;
  MaintenanceWaterLevel = -1;
  PrintCompLine("\x42\xc4\x73\xe1\x9e\x6d\x69\xf4\x63\xcf\x8a\x5a\xd3\x6b\x69"
"\xad\x9f\x65\xfa\xe3\xd9\x67\x79\xb5\x8f\xcd\xd7\xee\xad\x61\x67\xd5\x89\xc5"
"\x6f\x70\x80\xcb\xbf\x6b\xa8\xb4\x81\x64\x61\x6d\x2e");
  ReturnOverlay1(0);
}

void DMWT_act_match_ext(int with_to)
{
  int prev_darkness;

  if (with_to != 0)
  {
    PrintCompLine("\x8b\xe7\x93\xfe\x9e\xa2\x61\x74\x2e");
    ReturnOverlay1(0); /* return; */
  }

  if (Obj[O_MATCH].loc != INSIDE + O_YOU)
  {
    PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x69\x74\x2e");
    ReturnOverlay1(0); /* return; */
  }

  if (Obj[O_MATCH].prop & PROP_LIT)
  {
    PrintCompLine("\x41\xee\xaf\xfa\x87\xe2\xa9\x61\x64\xc4\xf5\x74\x2e");
    ReturnOverlay1(0); /* return; */
  }

  if (MatchesLeft <= 1)
  {
    PrintCompLine("\x49\x27\xf9\x61\x66\xf4\x69\xab\xa2\xaf\x86\xc0\x61\xd7\xda\xf6\xae\xf7\x8a\x6d\xaf\xfa\x65\x73\x2e");
    if (MatchesLeft == 0) ReturnOverlay1(0); /* return; */
  }
  MatchesLeft--;

  TimePassed = 1;

  if (Obj[O_YOU].loc == R_LOWER_SHAFT ||
      Obj[O_YOU].loc == R_TIMBER_ROOM)
  {
    PrintCompLine("\xbc\x9a\xc2\xe1\x87\x64\xf4\x66\x74\x79\xb5\x8c\x81\x6d\xaf\xfa\xe6\x6f\xbe\xae\xf7\xa8\x6e\xc5\xad\x74\x6c\x79\x2e");
    ReturnOverlay1(0); /* return; */
  }

  MatchTurnsLeft = 2;
  PrintCompLine("\x4f\xed\x8a\x81\x6d\xaf\xfa\xbe\xaa\x74\xbb\x74\xa1\xbd\xb0\xd8\x6e\x2e");

  prev_darkness = IsPlayerInDarkness();
  Obj[O_MATCH].prop |= PROP_LIT;
  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
  ReturnOverlay1(0);
}

void DMWT_dea_match_ext(int with_to)
{
  int prev_darkness;

  if (with_to != 0)
  {
    PrintCompLine("\x8b\xe7\x93\xfe\x9e\xa2\x61\x74\x2e");
    ReturnOverlay1(0); /* return; */
  }

  if ((Obj[O_MATCH].prop & PROP_LIT) == 0)
  {
    PrintCompLine("\x4e\xba\x6d\xaf\xfa\x87\xf5\x74\x2e");
    ReturnOverlay1(0); /* return; */
  }

  TimePassed = 1;
  MatchTurnsLeft = 0;
  PrintCompLine("\x85\x6d\xaf\xfa\x87\xa5\x74\x2e");

  prev_darkness = IsPlayerInDarkness();
  Obj[O_MATCH].prop &= ~PROP_LIT;
  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
  ReturnOverlay1(0);
}

void DMWT_act_candles_ext(int with_to)
{
  if (CandleTurnsLeft == 0)
  {
    PrintCompLine("\x41\xfd\x73\xb5\x96\xa9\x27\xa1\xe3\xa6\x6d\x75\xfa\xcb\x65"
"\x66\xa6\xdd\x80\x91\x64\xcf\x73\xa4\x43\xac\x74\x61\xa7\xec\xe4\xff\xfb\xe3\x75\x67\xde\xbd\xb0\xd8\x6e\x2e");
    ReturnOverlay1(0); /* return; */
  }

  if (Obj[O_CANDLES].loc != INSIDE + O_YOU)
  {
    PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x81\xe7\xb9\xcf\x73\x2e");
    ReturnOverlay1(0); /* return; */
  }

  if (with_to == 0 &&
      Obj[O_MATCH].loc == INSIDE + O_YOU &&
      (Obj[O_MATCH].prop & PROP_LIT))
  {
    with_to = O_MATCH;
    PrintCompLine("\x28\xf8\xa2\x80\xee\xaf\x63\x68\x29");
  }

  if (with_to == 0)
  {
    PrintCompLine("\x8b\x73\x68\xa5\x6c\xab\x73\x61\xc4\x77\xcd\xa6\xbd\xcb\x69\x67\x68\xa6\x96\xf9\xf8\x74\x68\x2e");
    ReturnOverlay1(0); /* return; */
  }

  if ((with_to == O_MATCH) && (Obj[O_MATCH].prop & PROP_LIT))
  {
    if (Obj[O_MATCH].loc != INSIDE + O_YOU)
      PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x81\x6d\xaf\x63\x68\x2e");
    else if (Obj[O_CANDLES].prop & PROP_LIT)
      PrintCompLine("\x85\xe7\xb9\xcf\xa1\xbb\x9e\xe2\xa9\x61\x64\xc4\xf5\x74\x2e");
    else
    {
      int prev_darkness;

      TimePassed = 1;
      PrintCompLine("\x85\xe7\xb9\xcf\xa1\xbb\x9e\xf5\x74\x2e");

      if (Obj[O_YOU].loc == R_ENTRANCE_TO_HADES &&
          BellRungCountdown > 0 &&
          CandlesLitCountdown == 0)
      {
        PrintCompLine("\x85\x66\xfd\x6d\xbe\xc6\xf5\x63\x6b\xac\xb7\x69\x6c\x64\xec"
"\x8d\xa3\x70\xfc\xbb\x89\x64\xad\x63\x65\x83\x9e\xbf\x72\xa2\xb0\xd4\xbf\xa2"
"\x86\xb6\x66\xf3\xa6\x74\xa9\x6d\x62\xcf\x73\xb5\x8c\x92\xcb\x65\x67\xa1\xed"
"\xbb\xec\xb0\x75\x63\x6b\xcf\xb0\xd4\xbf\xa2\x86\x83\x9e\x73\x70\x69\xf1\x74"
"\xa1\x63\xf2\xac\xa3\xa6\x92\x20\xf6\xbf\x72\xa2\xec\xeb\xf2\x65\x72\x2e");

        BellRungCountdown = 0;
        CandlesLitCountdown = 3;
      }

      prev_darkness = IsPlayerInDarkness();
      Obj[O_CANDLES].prop |= PROP_LIT;
      if (IsPlayerInDarkness() != prev_darkness)
      {
        PrintNewLine();
        PrintPlayerRoomDesc(1);
      }
    }
  }
  else if ((with_to == O_TORCH) && (Obj[O_TORCH].prop & PROP_LIT))
  {
    if (Obj[O_TORCH].loc != INSIDE + O_YOU)
      PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x81\xbd\x72\x63\x68\x2e");
    else if (Obj[O_CANDLES].prop & PROP_LIT)
      PrintCompLine("\x8b\xa9\xe2\x69\x7a\x65\xb5\x6a\xfe\xa6\xa7\x9f\x69\x6d"
"\x65\xb5\xa2\xaf\x80\x91\x64\xcf\xa1\xbb\x9e\xe2\xa9\x61\x64\xc4\xf5\x67\x68\xd1\x64\x2e");
    else
    {
      TimePassed = 1;
      Obj[O_CANDLES].loc = 0;

      PrintCompLine("\x85\xa0\xaf\xc6\xc2\xf9\x81\xbd\x72\xfa\x87\x73\xba\xa7"
"\xd1\x6e\xd6\x95\xaf\x80\x91\x64\xcf\xa1\xbb\x9e\x76\x61\x70\xd3\x69\x7a\x65\x64\x2e");
    }
  }
  else
    PrintCompLine("\x8b\xcd\xd7\x89\xf5\x67\x68\xa6\x96\xf9\xf8\xa2\xaa\xe1"
"\x65\xa2\x84\xa2\xaf\x27\xa1\x62\xd8\x6e\x97\xb5\x8f\x6b\xe3\x77\x2e");
  ReturnOverlay1(0);
}

void DMWT_dea_candles_ext(int with_to)
{
  int prev_darkness;

  if (with_to != 0)
  {
    PrintCompLine("\x8b\xe7\x93\xfe\x9e\xa2\x61\x74\x2e");
    ReturnOverlay1(0); /* return; */
  }

  if ((Obj[O_CANDLES].prop & PROP_LIT) == 0)
  {
    PrintCompLine("\x85\xe7\xb9\xcf\xa1\xbb\x9e\xe3\xa6\xf5\x67\x68\xd1\x64\x2e");
    ReturnOverlay1(0); /* return; */
  }

  TimePassed = 1;
  PrintCompLine("\x85\x66\xfd\x6d\x9e\x9a\x65\x78\xf0\xb1\x75\xb2\xa0\x64\x2e");
  Obj[O_CANDLES].prop |= PROP_MOVEDDESC; /* needed since unmoved description of candles says they are burning*/

  prev_darkness = IsPlayerInDarkness();
  Obj[O_CANDLES].prop &= ~PROP_LIT;
  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
  ReturnOverlay1(0);
}





