#include <ovr51.h>

/* defines, enums ------------------------------------------------------------- */
#define OVR_DO_MISC_WITH_TO_EXT		51

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
int IsPlayerInDarkness(void);
void YoureDead(void);
void PlayerBlow(int obj, int player_weapon);
void AttackVillain(int obj, int with_to);
void PrintUsingMsg(int obj);

void ReturnOverlay1(short);
short CallOverlay1(void* routine, short Par1, char bank);

/* variables ----------------------------------------------------------------------- */

/* overlays ----------------------------------------------------------------------- */
/*----
void DMWT_tie_rope_ext(int with_to);
void DMWT_untie_rope_ext(int with_to);
void DMWT_i_f_iated_boat_ext(int with_to);
void DMWT_i_f_p_boat_ext(int with_to);
void DMWT_d_ied_boat_ext(int with_to);
void DMWT_d_iable_boat_ext(int with_to);
void DMWT_d_pun_boat_ext(int with_to);
void DMWT_act_machine_ext(int with_to);
void DMWT_fill_bottle_ext(int with_to);
void DMWT_oil_bolt_ext(int with_to);
void DMWT_pour_putty_ext(int with_to);
void DMWT_turn_book_ext(int with_to);
void DMWT_dea_torch_ext(int with_to);
void DMWT_act_torch_ext(int with_to);
void DMWT_lock_grate_ext(int with_to);
void DMWT_unlock_grate_ext(int with_to);
void DMWT_tie_railing_ext(int with_to);
void DMWT_turn_bolt_ext(int with_to);
void DMWT_f_pun_boat_ext(int with_to);
void DMWT_i_f_ible_boat_ext(int with_to);
void DMWT_fix_leak_ext(int with_to);
void DMWT_act_match_ext(int with_to);
void DMWT_dea_match_ext(int with_to);
void DMWT_act_candles_ext(int with_to);
void DMWT_dea_candles_ext(int with_to);

#define DMWT_tie_rope_ext 0
#define DMWT_untie_rope_ext 0
#define DMWT_i_f_iated_boat_ext 0
#define DMWT_i_f_p_boat_ext 0
#define DMWT_d_ied_boat_ext 0
#define DMWT_d_iable_boat_ext 0
#define DMWT_d_pun_boat_ext 0
#define DMWT_act_machine_ext 0
#define DMWT_fill_bottle_ext 0
#define DMWT_oil_bolt_ext 0
#define DMWT_pour_putty_ext 0
#define DMWT_turn_book_ext 0
#define DMWT_dea_torch_ext 0
#define DMWT_act_torch_ext 0
#define DMWT_lock_grate_ext 0
#define DMWT_unlock_grate_ext 0
#define DMWT_tie_railing_ext 0
#define DMWT_turn_bolt_ext 0
#define DMWT_f_pun_boat_ext 0
#define DMWT_i_f_ible_boat_ext 0
#define DMWT_fix_leak_ext 0
#define DMWT_act_match_ext 0
#define DMWT_dea_match_ext 0
#define DMWT_act_candles_ext 0
#define DMWT_dea_candles_ext 0
----*/
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

void DMWT_dig_sand(int with_to)
{
  int need = O_SHOVEL;

  if (with_to == 0 && Obj[need].loc == INSIDE + O_YOU) {with_to = need; PrintUsingMsg(with_to);}
  if (with_to == 0) {PrintCompLine("\x44\x69\x67\x67\x84\xf8\xa2\x86\xb6\xcd\xb9\xa1\x9a\x73\x69\xdf\x79\x2e"); ReturnOverlay1(0); /* return; */}
  if (with_to != need) {PrintFutileMsg(with_to); ReturnOverlay1(0); /* return; */}
  if (Obj[with_to].loc != INSIDE + O_YOU) {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x69\x74\x2e"); ReturnOverlay1(0); /* return; */}

  TimePassed = 1;
  CaveHoleDepth++;
  switch (CaveHoleDepth)
  {
    case 1: PrintCompLine("\x8b\xd6\x65\xf9\xbd\xb0\x9e\x64\x69\x67\x67\x84"
"\xd0\x68\x6f\xcf\xc0\xac\x65\x2e");                break;
    case 2: PrintCompLine("\x85\x68\x6f\xcf\x87\x67\x65\x74\xf0\x9c\xe8\x65"
"\xfc\x72\xb5\x62\xf7\x95\xaf\x27\xa1\x61\x62\xa5\xa6\x69\x74\x2e");   break;
    case 3: PrintCompLine("\x8b\xbb\x9e\x73\xd8\xc2\xf6\xe8\xab\x62\xc4\xd0"
"\x77\xe2\xea\xdd\xaa\x8c\xca\xa3\xdf\xaa\x69\xe8\x73\x2e"); break;

    case 4:
      if (Obj[O_SCARAB].prop & PROP_NODESC)
      {
        Obj[O_SCARAB].prop &= ~PROP_NOTTAKEABLE;
        Obj[O_SCARAB].prop &= ~PROP_NODESC;

        PrintCompLine("\x8b\xe7\xb4\xd6\x9e\xd0\x73\xe7\xf4\x62\xc0\xac\x9e\xa7\x80\xaa\xad\x64\x2e");
        ItObj = O_SCARAB;
      }
      else
        PrintCompLine("\x8b\x66\xa7\xab\xe3\xa2\x84\x65\x6c\x73\x65\x2e");
    break;

    default:
      CaveHoleDepth = 0;
      if (Obj[O_SCARAB].loc == R_SANDY_CAVE)
      {
        Obj[O_SCARAB].prop |= PROP_NOTTAKEABLE;
        Obj[O_SCARAB].prop |= PROP_NODESC;
      }
      PrintCompLine("\x85\x68\x6f\xcf\xb3\x6f\xdf\x61\x70\xd6\x73\xb5\x73\x6d\xff\xa0\xf1\x9c\xc9\x75\x2e");
      YoureDead(); /* ##### RIP #####*/
    break;
  }
  ReturnOverlay1(0);
}

void ActivateObj(int obj)
{
  int prev_darkness;

  if (Obj[obj].prop & PROP_LIT)
  {
    PrintCompLine("\x49\x74\x27\xa1\xe2\xa9\x61\x64\xc4\x6f\x6e\x21");
    return;
  }

  TimePassed = 1;
  PrintCompLine("\x49\x74\x27\xa1\x6f\x6e\x2e");

  prev_darkness = IsPlayerInDarkness();
  Obj[obj].prop |= PROP_LIT;
  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}

void DeactivateObj(int obj)
{
  int prev_darkness;

  if ((Obj[obj].prop & PROP_LIT) == 0)
  {
    PrintCompLine("\x49\x74\x27\xa1\xe2\xa9\x61\x64\xc4\xdd\x66\x21");
    return;
  }

  TimePassed = 1;
  PrintCompLine("\x49\x74\x27\xa1\xdd\x66\x2e");

  prev_darkness = IsPlayerInDarkness();
  Obj[obj].prop &= ~PROP_LIT;
  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}

void DMWT_act_lamp(int with_to)
{
  if (with_to != 0) PrintCompLine("\x8b\xe7\x93\xfe\x9e\xa2\x61\x74\x2e");
  else if (LampTurnsLeft == 0) PrintCompLine("\x41\xb0\xd8\xed\x64\x2d\xa5\xa6\xfd\x6d\x70\xb7\xca\x27\xa6\xf5\x67\x68\x74\x2e");
  else ActivateObj(O_LAMP);
  ReturnOverlay1(0);
}

void DMWT_dea_lamp(int with_to)
{
  if (with_to != 0) PrintCompLine("\x8b\xe7\x93\xfe\x9e\xa2\x61\x74\x2e");
  else if (LampTurnsLeft == 0) PrintCompLine("\x85\xfd\x6d\x70\xc0\xe0\xa3\x6c\xa9\x61\x64\xc4\x62\xd8\xed\xab\xa5\x74\x2e");
  else DeactivateObj(O_LAMP);
  ReturnOverlay1(0);
}

void DMWT_attack_bat     (int with_to) {AttackVillain(O_BAT    , with_to);  ReturnOverlay1(0);
}
void DMWT_att_ghosts  (int with_to) {AttackVillain(O_GHOSTS , with_to);  ReturnOverlay1(0);
}
void DMWT_att_cyclops (int with_to) {AttackVillain(O_CYCLOPS, with_to);  ReturnOverlay1(0);
}
void DMWT_attack_thief   (int with_to) {AttackVillain(O_THIEF  , with_to);  ReturnOverlay1(0);
}
void DMWT_attack_troll   (int with_to) {AttackVillain(O_TROLL  , with_to);  ReturnOverlay1(0);
}
void DMWT_attack_you(int with_to) {AttackVillain(O_YOU    , with_to);  ReturnOverlay1(0);
}

int CheckFlameSource(int obj, char *msg)
{
  if (Obj[obj].loc == INSIDE + O_YOU &&
      (Obj[obj].prop & PROP_LIT))
  {
    PrintLine(msg);
    return obj;
  }
  return 0;
}

void BurnObj(int obj, int with)
{
  if (with == 0) with = CheckFlameSource(O_MATCH  , "(with the match)");
  if (with == 0) with = CheckFlameSource(O_CANDLES, "(with the candles)");
  if (with == 0) with = CheckFlameSource(O_TORCH  , "(with the torch)");

  if (with == 0)
    {PrintCompLine("\x8b\x73\x68\xa5\x6c\xab\x73\x61\xc4\x77\xcd\xa6\xbd\xcb\x69\x67\x68\xa6\xc7\xb7\xc7\x68\x2e");  return; }

  if (Obj[with].loc != INSIDE + O_YOU)
  {
    switch (with)
    {
      case O_MATCH:   PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x81\x6d\xaf\x63\x68\x2e");   break;
      case O_CANDLES: PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x81\xe7\xb9\xcf\x73\x2e"); break;
      case O_TORCH:   PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x81\xbd\x72\x63\x68\x2e");   break;
      default:          PrintCompLine("\x8b\xe7\x93\xf5\x67\x68\xa6\xc7\xb7\xc7\xde\xa2\x61\x74\x21");   break;
    }
     return; 
  }

  if ((Obj[with].prop & PROP_LIT) == 0)
    {PrintCompLine("\x8b\xcd\xd7\x89\xf5\x67\x68\xa6\xc7\xb7\xc7\xde\x73\xe1\x65"
"\xa2\x84\xa2\xaf\x27\xa1\x62\xd8\x6e\x97\xb5\x8f\x6b\xe3\x77\x2e"); return; }

  if (obj == F_WHITE_HOUSE)
    {PrintCompLine("\x8b\x6d\xfe\xa6\xef\x20\x6a\x6f\x6b\x97\x2e"); return;}
  else if (obj == F_FRONT_DOOR)
    {PrintCompLine("\x8b\xe7\x6e\xe3\xa6\x62\xd8\xb4\xa2\x9a\x64\xe9\x72\x2e"); return;}
  else if (obj >= NUM_OBJECTS)
    {PrintCompLine("\x8b\xe7\x93\x62\xd8\xb4\xa2\x61\x74\x21"); return;}

  TimePassed = 1;

  if (obj == O_INFLATED_BOAT && YouAreInBoat)
  {
    PrintCompLine("\x49\xa6\xe7\x74\xfa\xbe\xc6\x69\xa9\xa4\x55\x6e\x66\xd3\x74"
"\xf6\xaf\x65\xec\xb5\x8f\x77\xac\x9e\xa7\xa8\xa6\xaf\x80\x9f\x69\x6d\x65\x2e");
    YouAreInBoat = 0;
    Obj[obj].loc = 0;
    YoureDead(); /* ##### RIP #####*/
    return;
  }

  if (Obj[obj].loc == INSIDE + O_YOU)
  {
    if (obj == O_LEAVES)
      PrintCompLine("\x85\xcf\x61\xd7\xa1\x62\xd8\x6e\xb5\x8c\x73\xba\x64\xba\xc9\x75\x2e");
    else
      PrintCompLine("\x49\xa6\xe7\x74\xfa\xbe\xc6\x69\xa9\xa4\x55\x6e\x66\xd3"
"\x74\xf6\xaf\x65\xec\xb5\x8f\x77\xac\x9e\x68\x6f\x6c\x64\x84\xc7\xa3\xa6\x81\xf0\x6d\x65\x2e");
    Obj[obj].loc = 0;
    YoureDead(); /* ##### RIP #####*/
    return;
  }

  Obj[obj].loc = 0;

  if (obj == O_LEAVES)
  {
    PrintCompLine("\x85\xcf\x61\xd7\xa1\x62\xd8\x6e\x2e");
    if (GratingRevealed == 0)
    {
      GratingRevealed = 1;
      PrintCompLine("\x49\xb4\x64\xb2\x74\xd8\x62\x84\x81\x70\x69\xcf\x8a\xcf"
"\x61\xd7\x73\xb5\xd0\x67\xf4\xf0\x9c\x9a\xa9\xd7\xe2\x65\x64\x2e");
    }
  }
  else if (obj == O_BOOK)
  {
    PrintCompLine("\x41\xb0\xe9\x6d\x84\x76\x6f\x69\x63\x9e\x73\x61\x79\xa1\x22"
"\x57\xc2\xb1\xb5\x63\xa9\xf0\x6e\x21\x22\x8d\x86\xe4\xff\x69\x63\x9e\xa2\xaf"
"\x86\xc0\x61\xd7\x9f\xd8\xed\xab\xa7\xbd\xa3\xeb\x69\xcf\x8a\x64\xfe\x74\xa4"
"\x48\xf2\xb5\x49\x91\x27\xa6\x69\x6d\x61\x67\xa7\x65\x2e");
    YoureDead(); /* ##### RIP #####*/
  }
  else
    PrintCompLine("\x49\xa6\xe7\x74\xfa\xbe\xc6\x69\xa9\x8d\x87\x63\xca\x73\x75\x6d\x65\x64\x2e");
}

void DMWT_act_leaves         (int with_to) {BurnObj(O_LEAVES         , with_to);  ReturnOverlay1(0);
}
void DMWT_activate_book           (int with_to) {BurnObj(O_BOOK           , with_to);  ReturnOverlay1(0);
}
void DMWT_act_s_bag   (int with_to) {BurnObj(O_SANDWICH_BAG   , with_to);  ReturnOverlay1(0);
}
void DMWT_act_adv  (int with_to) {BurnObj(O_ADVERTISEMENT  , with_to);  ReturnOverlay1(0);
}
void DMWT_act_ifed_boat  (int with_to) {BurnObj(O_INFLATED_BOAT  , with_to);  ReturnOverlay1(0);
}
void DMWT_act_paint       (int with_to) {BurnObj(O_PAINTING       , with_to);  ReturnOverlay1(0);
}
void DMWT_act_pun_boat (int with_to) {BurnObj(O_PUNCTURED_BOAT , with_to);  ReturnOverlay1(0);
}
void DMWT_act_ifble_boat(int with_to) {BurnObj(O_INFLATABLE_BOAT, with_to);  ReturnOverlay1(0);
}
void DMWT_activate_coal           (int with_to) {BurnObj(O_COAL           , with_to);  ReturnOverlay1(0);
}
void DMWT_act_b_label     (int with_to) {BurnObj(O_BOAT_LABEL     , with_to);  ReturnOverlay1(0);
}
void DMWT_activate_guide          (int with_to) {BurnObj(O_GUIDE          , with_to);  ReturnOverlay1(0);
}
void DMWT_activate_nest           (int with_to) {BurnObj(O_NEST           , with_to);  ReturnOverlay1(0);
}
void DMWT_act_white_h    (int with_to) {BurnObj(F_WHITE_HOUSE   , with_to);  ReturnOverlay1(0);
}
void DMWT_act_front_d     (int with_to) {BurnObj(F_FRONT_DOOR    , with_to);  ReturnOverlay1(0);
}

void DMWT_pour_water(int with_to)
{
  if (Obj[O_BOTTLE].loc != INSIDE + O_YOU ||
      Obj[O_WATER].loc != INSIDE + O_BOTTLE)
    PrintCompLine("\x8b\x64\xca\x27\xa6\xcd\xd7\xa3\x6e\xc4\x77\xaf\x65\x72\x2e");
  else if ((Obj[O_BOTTLE].prop & PROP_OPEN) == 0)
    PrintCompLine("\xdc\x75\x27\xdf\xc0\x61\xd7\x89\x6f\xfc\xb4\x81\x62\xff\x74\xcf\xc6\x69\x72\x73\x74\x2e");
  else if (with_to == 0)
    PrintCompLine("\x8b\xed\xd5\x89\x70\x90\xc7\xae\xb4\x73\xe1\x65\xa2\x97\x2e");
  else
  {
    TimePassed = 1;
    Obj[O_WATER].loc = 0;      

    switch (with_to)
    {
      case O_HOT_BELL:
        PrintCompLine("\x85\x77\xaf\xac\xb3\xe9\x6c\xa1\x81\xef\xdf\x8d\x87\x65\x76\x61\x70\xd3\xaf\x65\x64\x2e");
        BellHotCountdown = 0;
        Obj[O_BELL].loc = R_ENTRANCE_TO_HADES;
        Obj[O_HOT_BELL].loc = 0;
      break;

      case O_TORCH:
        PrintCompLine("\x85\x77\xaf\xac\xfb\x76\x61\x70\xd3\xaf\xbe\xb0\x65\x66"
"\xd3\x9e\xc7\xe6\x65\x74\xa1\x63\xd9\x73\x65\x2e");
      break;

      case O_MATCH:
      case O_CANDLES:
        if (Obj[with_to].prop & PROP_LIT)
        {
          int prev_darkness = IsPlayerInDarkness();

          PrintCompLine("\x49\xa6\x9a\x65\x78\xf0\xb1\x75\xb2\xa0\x64\x2e");
          Obj[with_to].prop &= ~PROP_LIT;
          if (with_to == O_MATCH) MatchTurnsLeft = 0;

          if (IsPlayerInDarkness() != prev_darkness)
          {
            PrintNewLine();
            PrintPlayerRoomDesc(1);
          }
        }
        else
          PrintCompLine("\x85\x77\xaf\xac\xaa\x70\x69\xdf\xa1\x6f\xd7\xb6\xc7\xb5"
"\xbd\x80\xc6\xd9\xd3\xb5\x8c\x65\x76\x61\x70\xd3\xaf\x65\x73\x2e");
      break;

      default: /* note that this includes with_to >= NUM_OBJECTS*/
        PrintCompLine("\x85\x77\xaf\xac\xaa\x70\x69\xdf\xa1\x6f\xd7\xb6\xc7\xb5"
"\xbd\x80\xc6\xd9\xd3\xb5\x8c\x65\x76\x61\x70\xd3\xaf\x65\x73\x2e");
      break;
    }
  }
  ReturnOverlay1(0);
}

void DMWT_brush_teeth(int with_to)
{
  if (with_to == 0)
    PrintCompLine("\x44\xd4\x74\xe2\xc0\x79\x67\x69\xd4\x9e\x9a\xce\x67\x68\xec"
"\xda\x65\x63\xe1\x6d\xd4\xe8\x64\xb5\x62\xf7\x20\x49\x27\xf9\xe3\xa6\x73\xd8"
"\x9e\x77\xcd\xa6\x8f\x77\xad\xa6\xbd\xb0\x72\xfe\xde\x96\xf9\xf8\x74\x68\x2e");
  else if (with_to != O_PUTTY)
    PrintCompLine("\x41\xe4\x69\x63\x9e\x69\xe8\x61\xb5\x62\xf7\xb7\xc7\xde\xa2\x61\x74\x3f");
  else if (Obj[with_to].loc != INSIDE + O_YOU)
    PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x69\x74\x2e");
  else
  {
    TimePassed = 1;
    PrintCompLine("\x57\x65\xdf\xb5\x8f\xd6\x65\xf9\xbd\xc0\x61\xd7\xb0\xf3\xb4"
"\x62\x72\xfe\xce\x9c\x92\x9f\xf3\xa2\xb7\xc7\xde\x73\xe1\x9e\x73\xd3\xa6\xdd"
"\xe6\x6c\x75\x65\xa4\x41\xa1\xd0\xa9\x73\x75\x6c\x74\xb5\x92\xee\xa5\xa2\xe6"
"\x65\x74\xa1\x67\x6c\x75\xd5\x9f\x6f\x67\x65\x96\xb6\x28\xf8\xa2\x86\xb6\xe3"
"\xd6\x29\x8d\x86\xcc\x69\x9e\xdd\xda\xbe\x70\x69\xf4\xbd\x72\xc4\x66\x61\x69\x6c\xd8\x65\x2e");
    YoureDead(); /* ##### RIP #####*/
  }
  ReturnOverlay1(0);
}

void TieUpRoutine(int i, int with_to)
{
  if (with_to == 0 && Obj[O_ROPE].loc == INSIDE + O_YOU)
  {
    with_to = O_ROPE;
    PrintUsingMsg(with_to);
  }
  if (with_to == 0) {PrintCompLine("\x50\xcf\xe0\x9e\x73\xfc\x63\x69\x66\xc4\x77"
"\xcd\xa6\xbd\x9f\x69\x9e\xce\xf9\xf8\x74\x68\x2e"); return;}
  if (with_to != O_ROPE) {PrintCompLine("\x8b\xe7\x93\xf0\x9e\xce\xf9\xf8\xa2\x95\x61\x74\x21"); return;}


  if (i == VILLAIN_CYCLOPS)
    PrintCompLine("\x8b\xe7\x6e\xe3\xa6\xf0\x9e\x81\x63\x79\x63\xd9\x70\x73\xb5"
"\xa2\xa5\x67\xde\x94\x9a\x66\xc7\x89\xef\x9f\x69\x65\x64\x2e");
  else
  {
    char *name;

    if (i == VILLAIN_THIEF) name = "thief"; else name = "troll";

    if (VillainStrength[i] < 0)
    {
      PrintCompText("\xdc\xd8\xa3\x74\xd1\x6d\x70\xa6\xbd\x9f\x69\x9e\x75\x70\x80\x20"); PrintText(name); PrintCompLine("\xa3\x77\x61\x6b\xd4\xa1\xce\x6d\x2e");
      VillainStrength[i] = -VillainStrength[i];
      VillainConscious(i);
    }
    else
      {PrintCompText("\x85"); PrintText(name); PrintCompLine("\xaa\x74\x72\x75\x67\x67\xcf\xa1\x8c\x8f\xe7\x6e\xe3\xa6\xf0\x9e\xce\xf9\x75\x70\x2e");}
  }
}

void DMWT_tie_cyclops(int with_to) {TieUpRoutine(VILLAIN_CYCLOPS, with_to);  ReturnOverlay1(0);
}
void DMWT_tie_thief  (int with_to) {TieUpRoutine(VILLAIN_THIEF  , with_to);  ReturnOverlay1(0);
}
void DMWT_tie_troll  (int with_to) {TieUpRoutine(VILLAIN_TROLL  , with_to);  ReturnOverlay1(0);
}

/* extension ---------------------------------------------------------------------------------- */

void DMWT_tie_rope(int with_to)
{
  CallOverlay1((void*)DMWT_tie_rope_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_untie_rope(int with_to)
{
  CallOverlay1((void*)DMWT_untie_rope_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_i_f_iated_boat(int with_to)
{
  CallOverlay1((void*)DMWT_i_f_iated_boat_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_i_f_p_boat(int with_to)
{
  CallOverlay1((void*)DMWT_i_f_p_boat_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_d_ied_boat(int with_to)
{
  CallOverlay1((void*)DMWT_d_ied_boat_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_d_iable_boat(int with_to)
{
  CallOverlay1((void*)DMWT_d_iable_boat_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_d_pun_boat(int with_to)
{
  CallOverlay1((void*)DMWT_d_pun_boat_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_act_machine(int with_to)
{
  CallOverlay1((void*)DMWT_act_machine_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_fill_bottle(int with_to)
{
  CallOverlay1((void*)DMWT_fill_bottle_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_oil_bolt(int with_to)
{
  CallOverlay1((void*)DMWT_oil_bolt_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_pour_putty(int with_to)
{
  CallOverlay1((void*)DMWT_pour_putty_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_turn_book(int with_to)
{
  CallOverlay1((void*)DMWT_turn_book_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_dea_torch(int with_to)
{
  CallOverlay1((void*)DMWT_dea_torch_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_act_torch(int with_to)
{
  CallOverlay1((void*)DMWT_act_torch_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_lock_grate(int with_to)
{
  CallOverlay1((void*)DMWT_lock_grate_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_unlock_grate(int with_to)
{
  CallOverlay1((void*)DMWT_unlock_grate_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_tie_railing(int with_to)
{
  CallOverlay1((void*)DMWT_tie_railing_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_turn_bolt(int with_to)
{
  CallOverlay1((void*)DMWT_turn_bolt_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_f_pun_boat(int with_to)
{
  CallOverlay1((void*)DMWT_f_pun_boat_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_i_f_ible_boat(int with_to)
{
  CallOverlay1((void*)DMWT_i_f_ible_boat_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_fix_leak(int with_to)
{
  CallOverlay1((void*)DMWT_fix_leak_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_act_match(int with_to)
{
  CallOverlay1((void*)DMWT_act_match_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_dea_match(int with_to)
{
  CallOverlay1((void*)DMWT_dea_match_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_act_candles(int with_to)
{
  CallOverlay1((void*)DMWT_act_candles_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

void DMWT_dea_candles(int with_to)
{
  CallOverlay1((void*)DMWT_dea_candles_ext, with_to, OVR_DO_MISC_WITH_TO_EXT);
  ReturnOverlay1(0);
}

