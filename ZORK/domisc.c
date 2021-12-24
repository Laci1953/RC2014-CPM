#include <stdio.h>

#include <ovr48.h>
#include <ovr49.h>

/* defines, enums ------------------------------------------------------------- */

#define OVR_DO_MISC_EXT			48
#define OVR_DO_MISC_TAKE_TO		49


#define NUM_OBJECTS  80

#define INSIDE  2048

#define O_YOU 1
#define O_SWORD  61
#define O_WATER 14
#define O_HOT_BELL 19
#define O_BOTTLE 28
#define O_RUSTY_KNIFE 51
#define O_LOWERED_BASKET 7
#define O_RAISED_BASKET 8
#define O_INFLATED_BOAT 39
#define O_SCEPTRE 23
#define O_KNIFE 33
#define O_AXE 21
#define O_STILETTO 60
#define O_PUNCTURED_BOAT 45
#define O_INFLATED_BOAT 39
#define O_LEAVES 44
#define O_BELL 18
#define O_CANDLES 42
#define O_BAUBLE 74
#define O_POT_OF_GOLD 48
#define O_BOOK 20
#define O_GHOSTS 3
#define O_ADVERTISEMENT 38
#define O_MATCH 40
#define O_MAP 62
#define O_BOAT_LABEL 63
#define O_TUBE 67
#define O_OWNERS_MANUAL 69
#define O_EGG 72
#define O_BROKEN_EGG 73
#define O_LUNCH 17
#define O_GARLIC 26
#define O_TUBE 67
#define O_PUTTY 68
#define O_SANDWICH_BAG 24
#define O_LAMP 36
#define O_GUIDE 65

#define R_FOREST_1 6
#define R_FOREST_2 7
#define R_FOREST_3 9
#define R_PATH 10 
#define R_UP_A_TREE 11
#define R_ENTRANCE_TO_HADES 66
#define R_WEST_OF_HOUSE 1
#define R_NORTH_OF_HOUSE 3
#define R_SOUTH_OF_HOUSE 4
#define R_EAST_OF_HOUSE 5
#define R_GRATING_CLEARING 12
#define R_CLEARING 13
#define R_LIVING_ROOM 16
#define R_TREASURE_ROOM 44
#define R_SHAFT_ROOM 97
#define R_LOWER_SHAFT 104
#define R_MAINTENANCE_ROOM 76
#define R_DAM_ROOM 74
#define R_GRATING_ROOM 36
#define R_ARAGAIN_FALLS 88
#define R_ON_RAINBOW 89
#define R_END_OF_RAINBOW 90
#define R_MIRROR_R_1 50
#define R_MIRROR_R_2 51
#define R_KITCHEN 14
#define R_CELLAR 17
#define R_MINE_4 109
#define R_SLIDE_ROOM 110
#define R_ATTIC 15
#define R_RESERVOIR_NORTH 47
#define R_NORTH_TEMPLE 72

#define BL0 233 /* (256-23) */
#define BL9 242
#define BLA 243

/*object bit flags*/

#define PROP_OPENABLE        1
#define PROP_OPEN            2
#define PROP_LIT             4
#define PROP_NODESC          8
#define PROP_NOTTAKEABLE    16
#define PROP_MOVEDDESC      32  /* set when object is first taken */
#define PROP_WEAPON        512
#define PROP_TOOL         2048

/*room properties bit flags*/
#define R_DESCRIBED     1  /* set when room description already printed */
#define R_BODYOFWATER   2
#define R_LIT           4  /* set when there is natural light or a light fixture */
#define R_WATERHERE     8

/*fixed (unmoving) objects*/
enum
{
  F_SCENERY_VIS = 2048, /*some anonymous scenery object, visible*/
  F_SCENERY_NOTVIS,     /*                               not visible*/
  F_NOTVIS,             /*fixed object not visible*/
  F_AMB                /*amibiguous (ask for clarification)*/
};

#define F_TRAP_DOOR 2069
#define F_KITCHEN_WINDOW 2058 

#define MAX_INVENTORY_ITEMS  7
#define INV_LIMIT_CHANCE     8

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

extern FILE *InputStream;
extern int ItObj, PrevItObj; /*the obj "it" refers to*/
extern char TimePassed; /*flag: time passed during action*/

extern unsigned char RugMoved;
extern unsigned char TrapOpen;
extern unsigned char ExitFound; /* set this when player finds an exit from dungeon other than the trapdoor*/
extern unsigned char KitchenWindowOpen;
extern unsigned char GratingRevealed;
extern unsigned char GratingUnlocked;
extern unsigned char GratingOpen;
extern unsigned char GatesButton;
extern unsigned char RainbowSolid;
extern unsigned char MirrorBroken; /* set NotLucky too*/
extern unsigned char RopeTiedToRail;
extern unsigned char SpiritsBanished;
extern unsigned char YouAreInBoat;
extern unsigned char NotLucky;
extern unsigned char SongbirdSang;

extern int LampTurnsLeft;
extern int MatchesLeft;
extern int MaintenanceWaterLevel;
extern int BellRungCountdown; /* these three are for ceremony*/
extern int CandlesLitCountdown;
extern int BellHotCountdown;
extern int CyclopsState; /* 0: default  1: hungry  2: thirsty  3: asleep  4: fled*/
extern int ThiefDescType; /* 0: default  1: unconscious*/

/* extern functions ---------------------------------------------------------------- */
void PrintText(char *p);
void PrintLine(char *p);
void PrintNewLine(void);
void PrintCompText(char *comp_text);
void PrintCompLine(char *);
void PrintInteger(int num);
int GetRandom(int Range);
int IsPlayerInDarkness(void);
void PrintPlayerRoomDesc(int force_description);
void YoureDead(void);
int GetWith(void);
void PrintBlockMsg(int newroom);
void PrintContents(int obj, char *heading, int print_empty);
void PrintDesc_Troll(int desc_flag);
void ReturnOverlay0(short);
void ReturnOverlay0_(short);
short CallOverlay0(void* Routine, char CalledRamBank);

/* variables ----------------------------------------------------------------------- */

char *no_effect[3] =
  {
    "doesn't seem to work.",
    "isn't notably helpful.",
    "has no effect."
  };

/* overlays ------------------------------------------------------------------------ */
/* _EXT */
/* --- 
void DM_read_prayer_ext(void);
void DM_read_wooden_door_ext(void);
void DM_read_engravings_ext(void);
void DM_whereis_forest_ext(void);
void DM_read_granite_wall_ext(void);
void DM_examine_zorkmid_ext(void);
void DM_examine_grue_ext(void);
void DM_whereis_zorkmid_ext(void);
void DM_whereis_grue_ext(void);
void DM_listento_troll_ext(void);
void DM_listento_thief_ext(void);
void DM_listento_cyclops_ext(void);
void DM_listento_forest_ext(void);
void DM_listento_songbird_ext(void);
void DM_cross_lake_ext(void);
void DM_cross_stream_ext(void);
void DM_cross_chasm_ext(void);
void DM_exorcise_ghosts_ext(void);
void DM_raise_rug_ext(void);
void DM_smell_gas_ext(void);
void DM_examine_l_b_ext(void);
void DM_lookin_large_bag_ext(void);
void DM_lookthrough_grate_ext(void);
void DM_lookin_water_ext(void);
void DM_open_kit_w_ext(void);
void DM_close_kit_w_ext(void);
void DM_move_push_rug_ext(void);
void DM_open_trap_door_ext(void);
void DM_close_trap_door_ext(void);
void DM_smell_sandwich_bag_ext(void);
void DM_raise_trap_door_ext(void);
void DM_push_blue_button_ext(void);
void DM_wind_broken_canary_ext(void);
void DM_climb_m_r_ext(void);
void DM_climbup_white_cliff_ext(void);
void DM_examine_match_ext(void);
void DM_examine_candles_ext(void);
void DM_examine_torch_ext(void);
void DM_examine_thief_ext(void);
void DM_examine_tool_chest_ext(void);
void DM_examine_board_ext(void);
void DM_examine_chain_ext(void);
void DM_open_tool_chest_ext(void);
void DM_open_book_ext(void);
void DM_close_book_ext(void);
void DM_open_boarded_window_ext(void);
void DM_break_boarded_window_ext(void);
void DM_open_close_dam_ext(void);
void DM_ring_hot_bell_ext(void);
void DM_read_button_ext(void);
void DM_raise_l_g_wall_ext(void);
void DM_raise_lower_chain_ext(void);
void DM_move_chain_ext(void);
void DM_count_candles_ext(void);
void DM_count_leaves_ext(void);
void DM_examine_lamp_ext(void);
void DM_examine_cyclops_ext(void);
void DM_examine_white_house_ext(void);
void DM_open_close_b_d_ext(void);
void DM_open_close_s_d_ext(void);
void DM_open_close_b_of_c_ext(void);
void DM_open_close_trunk_ext(void);
void DM_open_close_large_bag_ext(void);
void DM_open_front_door_ext(void);
void DM_drink_water_ext(void);
void DM_break_mirror_ext(void);
void DM_lookin_mirror_ext(void);
void DM_lookthrough_k_w_ext(void);
void DM_lookunder_rug_ext(void);
void DM_lookunder_leaves_ext(void);
void DM_lookunder_rainbow_ext(void);
void DM_lookin_chimney_ext(void);
void DM_examine_k_w_ext(void);
void DM_lookin_bag_of_coins_ext(void);
void DM_lookin_trunk_ext(void);
void DM_squeeze_tube_ext(void);
void DM_whereis_songbird_ext(void);
void DM_examine_sword_ext(void);
void DM_push_brown_button_ext(void);
void DM_push_yellow_button_ext(void);
void DM_enter_inf_b_ext(void);
void DM_exit_inflated_boat_ext(void);
void DM_move_leaves_ext(void);
void DM_ring_bell_ext(void);
void DM_climbthrough_crack_ext(void);
void DM_whereis_granite_wall_ext(void);
void DM_whereis_white_house_ext(void);
void DM_eat_lunch_ext(void);
void DM_eat_garlic_ext(void);
void DM_wind_canary_ext(void);

#define DM_read_prayer_ext 0
#define DM_read_wooden_door_ext 0
#define DM_read_engravings_ext 0
#define DM_whereis_forest_ext 0
#define DM_read_granite_wall_ext 0
#define DM_examine_zorkmid_ext 0
#define DM_examine_grue_ext 0
#define DM_whereis_zorkmid_ext 0
#define DM_whereis_grue_ext 0
#define DM_listento_troll_ext 0
#define DM_listento_thief_ext 0
#define DM_listento_cyclops_ext 0
#define DM_listento_forest_ext 0
#define DM_listento_songbird_ext 0
#define DM_cross_lake_ext 0
#define DM_cross_stream_ext 0
#define DM_cross_chasm_ext 0
#define DM_exorcise_ghosts_ext 0
#define DM_raise_rug_ext 0
#define DM_smell_gas_ext 0
#define DM_examine_l_b_ext 0
#define DM_lookin_large_bag_ext 0
#define DM_lookthrough_grate_ext 0
#define DM_lookin_water_ext 0
#define DM_open_kit_w_ext 0
#define DM_close_kit_w_ext 0
#define DM_move_push_rug_ext 0
#define DM_open_trap_door_ext 0
#define DM_close_trap_door_ext 0
#define DM_smell_sandwich_bag_ext 0
#define DM_raise_trap_door_ext 0
#define DM_push_blue_button_ext 0
#define DM_wind_broken_canary_ext 0
#define DM_climb_m_r_ext 0
#define DM_climbup_white_cliff_ext 0
#define DM_examine_match_ext 0
#define DM_examine_candles_ext 0
#define DM_examine_torch_ext 0
#define DM_examine_thief_ext 0
#define DM_examine_tool_chest_ext 0
#define DM_examine_board_ext 0
#define DM_examine_chain_ext 0
#define DM_open_tool_chest_ext 0
#define DM_open_book_ext 0
#define DM_close_book_ext 0
#define DM_open_boarded_window_ext 0
#define DM_break_boarded_window_ext 0
#define DM_open_close_dam_ext 0
#define DM_ring_hot_bell_ext 0
#define DM_read_button_ext 0
#define DM_raise_l_g_wall_ext 0
#define DM_raise_lower_chain_ext 0
#define DM_move_chain_ext 0
#define DM_count_candles_ext 0
#define DM_count_leaves_ext 0
#define DM_examine_lamp_ext 0
#define DM_examine_cyclops_ext 0
#define DM_examine_white_house_ext 0
#define DM_open_close_b_d_ext 0
#define DM_open_close_s_d_ext 0
#define DM_open_close_b_of_c_ext 0
#define DM_open_close_trunk_ext 0
#define DM_open_close_large_bag_ext 0
#define DM_open_front_door_ext 0
#define DM_drink_water_ext 0
#define DM_break_mirror_ext 0
#define DM_lookin_mirror_ext 0
#define DM_lookthrough_k_w_ext 0
#define DM_lookunder_rug_ext 0
#define DM_lookunder_leaves_ext 0
#define DM_lookunder_rainbow_ext 0
#define DM_lookin_chimney_ext 0
#define DM_examine_k_w_ext 0
#define DM_lookin_bag_of_coins_ext 0
#define DM_lookin_trunk_ext 0
#define DM_squeeze_tube_ext 0
#define DM_whereis_songbird_ext 0
#define DM_examine_sword_ext 0
#define DM_push_brown_button_ext 0
#define DM_push_yellow_button_ext 0
#define DM_enter_inf_b_ext 0
#define DM_exit_inflated_boat_ext 0
#define DM_move_leaves_ext 0
#define DM_ring_bell_ext 0
#define DM_climbthrough_crack_ext 0
#define DM_whereis_granite_wall_ext 0
#define DM_whereis_white_house_ext 0
#define DM_eat_lunch_ext 0
#define DM_eat_garlic_ext 0
#define DM_wind_canary_ext 0
----- */
/* TAKE_TO */
/* ---
void DM_read_book_tk(void);
void DM_read_advertisement_tk(void);
void DM_read_match_tk(void);
void DM_read_map_tk(void);
void DM_read_boat_label_tk(void);
void DM_read_tube_tk(void);
void DM_read_owners_manual_tk(void);
void DM_read_guide_tk(void);

#define DM_read_book_tk 0
#define DM_read_advertisement_tk 0
#define DM_read_match_tk 0
#define DM_read_map_tk 0
#define DM_read_boat_label_tk 0
#define DM_read_tube_tk 0
#define DM_read_owners_manual_tk 0
#define DM_read_guide_tk 0
---*/
/* code ---------------------------------------------------------------------------- */

void PrintNoEffect(char *prefix)
{

  PrintText(prefix);
  PrintLine(no_effect[GetRandom(3)]);
}

void RaiseLowerBasketRoutine(int raise)
{
  int prev_darkness = IsPlayerInDarkness();

  Obj[O_RAISED_BASKET ].loc = raise ? R_SHAFT_ROOM  : R_LOWER_SHAFT;
  Obj[O_LOWERED_BASKET].loc = raise ? R_LOWER_SHAFT : R_SHAFT_ROOM ;

  TimePassed = 1;

  if (raise) 
    PrintCompLine("\x85\x62\xe0\x6b\x65\xa6\x9a\xf4\xb2\xd5\x89\x81\xbd"
"\x70\x8a\x81\x73\xcd\x66\x74\x2e");
  else       
    PrintCompLine("\x85\x62\xe0\x6b\x65\xa6\x9a\xd9\x77\xac\xd5\x89\x81"
"\x62\xff\xbd\xf9\xdd\x80\xaa\xcd\x66\x74\x2e");

  if (Obj[O_RAISED_BASKET].loc == Obj[O_YOU].loc) 
    ItObj = O_RAISED_BASKET;
  else
    ItObj = O_LOWERED_BASKET;

  /*did room become darkened when basket moved*/
  if (IsPlayerInDarkness() != prev_darkness && prev_darkness == 0)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
}

void DM_raise_basket(void)
{
  if (Obj[O_RAISED_BASKET].loc == Obj[O_YOU].loc)
  {
    if (Obj[O_YOU].loc == R_LOWER_SHAFT)
      RaiseLowerBasketRoutine(1);
    else PrintCompLine("\x50\xfd\x79\x84\xa7\x95\x9a\x77\x61\xc4\xf8\xa2\x80\xb0"
"\xe0\x6b\x65\xa6\xcd\xa1\xe3\xfb\x66\x66\x65\x63\x74\x2e");
  }
  else
  {
    if (Obj[O_YOU].loc == R_SHAFT_ROOM)
      RaiseLowerBasketRoutine(1);
    else PrintCompLine("\x85\x62\xe0\x6b\x65\xa6\x9a\xaf\x80\xae\x96\xb6\xd4\xab"
"\xdd\x80\xb3\xcd\x69\x6e\x2e");
  }
  ReturnOverlay0_(0);
}

void DM_lower_basket(void)
{
  if (Obj[O_RAISED_BASKET].loc == Obj[O_YOU].loc)
  {
    if (Obj[O_YOU].loc == R_SHAFT_ROOM)
      RaiseLowerBasketRoutine(0);
    else PrintCompLine("\x50\xfd\x79\x84\xa7\x95\x9a\x77\x61\xc4\xf8\xa2\x80\xb0"
"\xe0\x6b\x65\xa6\xcd\xa1\xe3\xfb\x66\x66\x65\x63\x74\x2e");
  }
  else
  {
    if (Obj[O_YOU].loc == R_LOWER_SHAFT)
      RaiseLowerBasketRoutine(0);
    else PrintCompLine("\x85\x62\xe0\x6b\x65\xa6\x9a\xaf\x80\xae\x96\xb6\xd4\xab"
"\xdd\x80\xb3\xcd\x69\x6e\x2e");
  }
  ReturnOverlay0_(0);
}

void DM_push_red_button(void)
{
  int prev_darkness = IsPlayerInDarkness();

  TimePassed = 1;

  PrintCompText("\x85\xf5\x67\x68\x74\xa1\xf8\xa2\xa7\x80\xda\xe9\x6d\x20");

  if (Room[R_MAINTENANCE_ROOM].prop & R_LIT)
  {
    Room[R_MAINTENANCE_ROOM].prop &= ~R_LIT;
    PrintCompLine("\x73\x68\xf7\xae\x66\x66\x2e");
  }
  else
  {
    Room[R_MAINTENANCE_ROOM].prop |= R_LIT;
    PrintCompLine("\x63\xe1\x9e\x6f\x6e\x2e");
  }

  /*did room become darkened*/
  if (IsPlayerInDarkness() != prev_darkness && prev_darkness == 0)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(1);
  }
  ReturnOverlay0(0);
}

void DM_open_grate(void)
{
  int leaves_fall = 0, prev_darkness;

  if (GratingRevealed == 0) 
  {PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd\x95"
"\x6f\xd6\xae\x62\x6a\x65\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); 
     ReturnOverlay0(0);
    }
  if (GratingOpen) 
  {PrintCompLine("\x85\x67\xf4\xf0\x9c\x9a\xe2\xa9\x61\x64\xc4\x6f\xfc\x6e\x2e"); 
   ReturnOverlay0(0);
  }
  if (GratingUnlocked == 0) 
  {PrintCompLine("\x85\x67\xf4\xf0\x9c\x9a\xd9\x63\x6b\x65\x64\x2e"); 
   ReturnOverlay0(0);
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
  ReturnOverlay0(0);
}

void DM_close_grate(void)
{
  int prev_darkness;

  if (GratingRevealed == 0) {PrintCompLine("\x41\xa6\xcf\xe0\xa6\xca\x9e\xdd"
"\x95\x6f\xd6\xae\x62\x6a\x65\x63\x74\xa1\xb2\x93\x76\xb2\x69\x62\xcf\xc0\xac"
"\x65\x21"); 
  ReturnOverlay0(0);
  }
  if (GratingOpen == 0) {PrintCompLine("\x85\x67\xf4\xf0\x9c\x9a\xe2\xa9\x61"
"\x64\xc4\x63\xd9\xd6\x64\x2e"); 
  ReturnOverlay0(0);
  }

  TimePassed = 1;
  GratingOpen = 0;

  PrintCompLine("\x85\x67\xf4\xf0\x9c\x9a\x63\xd9\xd6\x64\x2e");

  prev_darkness = IsPlayerInDarkness();
  Room[R_GRATING_ROOM].prop &= ~R_LIT; /* no light spilling from grate opening*/
  if (IsPlayerInDarkness() != prev_darkness)
  {
    PrintNewLine();
    PrintPlayerRoomDesc(0);
  }
  ReturnOverlay0(0);
}

void DM_wave_sceptre(void)
{
  TimePassed = 1;

  if (Obj[O_YOU].loc == R_ARAGAIN_FALLS ||
      Obj[O_YOU].loc == R_END_OF_RAINBOW)
  {
    if (RainbowSolid == 0)
    {
      RainbowSolid = 1;
      PrintCompLine("\x53\x75\x64\xe8\x6e\xec\xb5\x81\xf4\xa7\x62\xf2\xa3\x70"
"\xfc\xbb\xa1\xbd\xb0\x65\x63\xe1\x9e\x73\x6f\xf5\xab\xad\x64\xb5\x49\x20\xd7"
"\xe5\xd8\x65\xb5\x77\xe2\x6b\x61\x62\xcf\x20\x28\x49\x95\xa7\x6b\x80\xe6\x69"
"\xd7\x61\x77\x61\xc4\x77\xe0\x80\xaa\x74\x61\x69\x72\xa1\x8c\x62\xad\x6e\xb2"
"\xd1\x72\x29\x2e");

      if (Obj[O_YOU].loc == R_END_OF_RAINBOW &&
          (Obj[O_POT_OF_GOLD].prop & PROP_NODESC))
        PrintCompLine("\x41\xaa\xce\x6d\x6d\xac\x84\x70\xff\x8a\x67\x6f\x6c\xab"
"\x61\x70\xfc\xbb\xa1\xaf\x80\xfb\xb9\x8a\x81\xf4\xa7\x62\x6f\x77\x2e");

      Obj[O_POT_OF_GOLD].prop &= ~PROP_NOTTAKEABLE;
      Obj[O_POT_OF_GOLD].prop &= ~PROP_NODESC;
    }
    else
    {
      RainbowSolid = 0;
      PrintCompLine("\x85\xf4\xa7\x62\xf2\xaa\xf3\x6d\xa1\xbd\xc0\x61\xd7\xb0"
"\x65\x63\xe1\x9e\x73\xe1\x65\x77\xcd\xa6\x72\xf6\x2d\xdd\x2d\x96\x2d\x6d\x69\x6c\x6c\x2e");
    }
  }
  else if (Obj[O_YOU].loc == R_ON_RAINBOW)
  {
    RainbowSolid = 0;
    PrintCompLine("\x85\xc5\x72\x75\x63\x74\xd8\xe2\xa8\xe5\x65\x67\xf1\x74"
"\xc4\xdd\x80\xda\x61\xa7\x62\xf2\x87\xd6\xd7\xa9\xec\xb3\xe1\x70\xc2\x6d\xb2"
"\xd5\xb5\xcf\x61\x76\x84\x8f\xcd\xb1\x84\xa7\xee\x69\x64\x61\x69\x72\xb5\x73"
"\x75\x70\x70\xd3\xd1\xab\xca\xec\xb0\xc4\x77\xaf\xac\x20\x76\x61\x70\xd3\xa4\x42\x79\x65\x2e");
    YoureDead(); /* ##### RIP #####*/
  }
  else
    PrintCompLine("\x41\xcc\x61\x7a\x7a\xf5\x9c\x64\xb2\x70\xfd\xc4\xdd\xb3"
"\x6f\xd9\xb6\x62\xf1\x65\x66\xec\xfb\x6d\xad\xaf\xbe\xc6\xc2\xf9\x81\x73\x63"
"\x65\x70\x74\x72\x65\x2e");
  ReturnOverlay0(0);
}

void DM_raise_sceptre(void)
{
  if (Obj[O_SCEPTRE].loc != INSIDE + O_YOU)
    PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xc0\x6f\x6c\x64\x84\x69\x74\x2e");
  else
    /* DM_wave_sceptre(); */
  {
  TimePassed = 1;

  if (Obj[O_YOU].loc == R_ARAGAIN_FALLS ||
      Obj[O_YOU].loc == R_END_OF_RAINBOW)
  {
    if (RainbowSolid == 0)
    {
      RainbowSolid = 1;
      PrintCompLine("\x53\x75\x64\xe8\x6e\xec\xb5\x81\xf4\xa7\x62\xf2\xa3\x70"
"\xfc\xbb\xa1\xbd\xb0\x65\x63\xe1\x9e\x73\x6f\xf5\xab\xad\x64\xb5\x49\x20\xd7"
"\xe5\xd8\x65\xb5\x77\xe2\x6b\x61\x62\xcf\x20\x28\x49\x95\xa7\x6b\x80\xe6\x69"
"\xd7\x61\x77\x61\xc4\x77\xe0\x80\xaa\x74\x61\x69\x72\xa1\x8c\x62\xad\x6e\xb2"
"\xd1\x72\x29\x2e");

      if (Obj[O_YOU].loc == R_END_OF_RAINBOW &&
          (Obj[O_POT_OF_GOLD].prop & PROP_NODESC))
        PrintCompLine("\x41\xaa\xce\x6d\x6d\xac\x84\x70\xff\x8a\x67\x6f\x6c\xab"
"\x61\x70\xfc\xbb\xa1\xaf\x80\xfb\xb9\x8a\x81\xf4\xa7\x62\x6f\x77\x2e");

      Obj[O_POT_OF_GOLD].prop &= ~PROP_NOTTAKEABLE;
      Obj[O_POT_OF_GOLD].prop &= ~PROP_NODESC;
    }
    else
    {
      RainbowSolid = 0;
      PrintCompLine("\x85\xf4\xa7\x62\xf2\xaa\xf3\x6d\xa1\xbd\xc0\x61\xd7\xb0"
"\x65\x63\xe1\x9e\x73\xe1\x65\x77\xcd\xa6\x72\xf6\x2d\xdd\x2d\x96\x2d\x6d\x69\x6c\x6c\x2e");
    }
  }
  else if (Obj[O_YOU].loc == R_ON_RAINBOW)
  {
    RainbowSolid = 0;
    PrintCompLine("\x85\xc5\x72\x75\x63\x74\xd8\xe2\xa8\xe5\x65\x67\xf1\x74"
"\xc4\xdd\x80\xda\x61\xa7\x62\xf2\x87\xd6\xd7\xa9\xec\xb3\xe1\x70\xc2\x6d\xb2"
"\xd5\xb5\xcf\x61\x76\x84\x8f\xcd\xb1\x84\xa7\xee\x69\x64\x61\x69\x72\xb5\x73"
"\x75\x70\x70\xd3\xd1\xab\xca\xec\xb0\xc4\x77\xaf\xac\x20\x76\x61\x70\xd3\xa4\x42\x79\x65\x2e");
    YoureDead(); /* ##### RIP #####*/
  }
  else
    PrintCompLine("\x41\xcc\x61\x7a\x7a\xf5\x9c\x64\xb2\x70\xfd\xc4\xdd\xb3"
"\x6f\xd9\xb6\x62\xf1\x65\x66\xec\xfb\x6d\xad\xaf\xbe\xc6\xc2\xf9\x81\x73\x63"
"\x65\x70\x74\x72\x65\x2e");

  }
  ReturnOverlay0_(0);
}

void DM_touch_mirror(void)
{
  int obj;

  if (MirrorBroken)
  {
    PrintNoEffect("Fiddling with that ");
    ReturnOverlay0(0);
  }

  TimePassed = 1;
  PrintCompLine("\x99\xa9\x87\xd0\x72\x75\x6d\x62\xcf\xc6\xc2\xf9\xe8\x65\x70"
"\xb7\xc7\xce\xb4\x81\xbf\x72\xa2\x8d\x80\xda\xe9\xf9\x73\xcd\x6b\x65\x73\x2e");

  /* note that this includes object 1: O_YOU*/
  for (obj=1; obj<NUM_OBJECTS; obj++)
  {
         if (Obj[obj].loc == R_MIRROR_R_1) Obj[obj].loc = R_MIRROR_R_2;
    else if (Obj[obj].loc == R_MIRROR_R_2) Obj[obj].loc = R_MIRROR_R_1;
  }
  ReturnOverlay0(0);
}

void DM_open_egg(void)
{
  int with;

  with = GetWith(); 
  if (with < 0) 
     ReturnOverlay0(0);

  if (Obj[O_EGG].loc != INSIDE + O_YOU)
    {PrintCompLine("\x8b\xbb\xd4\x27\xa6\x68\x6f\x6c\x64\x84\x81\x65\x67\x67\x2e"); 
     ReturnOverlay0(0);
    }

  if (Obj[O_EGG].prop & PROP_OPEN)
    {PrintCompLine("\x85\x65\x67\xc1\x9a\xe2\xa9\x61\x64\xc4\x6f\xfc\x6e\x2e"); 
     ReturnOverlay0(0);
    }

  if (with >= NUM_OBJECTS)
    {PrintCompLine("\x8b\xbb\xd4\x27\xa6\x68\x6f\x6c\x64\x84\xa2\x61\x74\x21"); 
     ReturnOverlay0(0);
     }

  if (with == 0)
    {PrintCompLine("\x8b\xcd\xd7\xe4\x65\xc7\xa0\xb6\x81\xbd\x6f\x6c\xa1\xe3\xb6"
"\x81\x65\x78\xfc\x72\xf0\x73\x65\x2e"); 
     ReturnOverlay0(0);
    }

  if (with == O_YOU)
    {PrintCompLine("\x49\xcc\xa5\x62\xa6\x8f\x63\xa5\x6c\xab\x64\xba\xa2\xaf\xb7"
"\xc7\x68\xa5\xa6\x64\x61\x6d\x61\x67\x84\x69\x74\x2e"); 
     ReturnOverlay0(0);
    }

  if ((Obj[with].prop & PROP_WEAPON) ||
      (Obj[with].prop & PROP_TOOL))
  {
    PrintCompLine("\x85\x65\x67\xc1\x9a\xe3\x77\xae\xfc\x6e\xb5\x62\xf7\x80\xb3"
"\x6c\x75\x6d\x73\xa7\xbe\xa1\xdd\x86\xb6\xaf\xd1\x6d\x70\xa6\xcd\xa1\xd6\xf1"
"\xa5\x73\xec\xb3\xe1\x70\xc2\x6d\xb2\xd5\xa8\x74\xa1\xbe\x96\xf0\x63\xa3\x70\xfc\x61\x6c\x2e");
    TimePassed = 1;

    Obj[O_EGG].loc = 0;
    Obj[O_BROKEN_EGG].loc = INSIDE + O_YOU;
    Obj[O_BROKEN_EGG].prop |= PROP_OPENABLE;
    Obj[O_BROKEN_EGG].prop |= PROP_OPEN;
    ReturnOverlay0(0);
  }

  PrintCompLine("\x8b\xe7\x93\x6f\xfc\xb4\xc7\xb7\xc7\xde\xa2\x61\x74\x21");
  ReturnOverlay0(0);
}

void DM_enter_white_house(void)
{
  if (Obj[O_YOU].loc != R_EAST_OF_HOUSE)
    PrintCompLine("\x49\x91\x27\xa6\xd6\x9e\x68\xf2\x89\x67\x65\xa6\xa7\xc6\xc2"
"\xf9\xa0\x72\x65\x2e");
  else
  {
  /* DM_climb_k_w(); */

  if (KitchenWindowOpen == 0)
  {
    PrintCompLine("\x85\xf8\xb9\xf2\x87\x63\xd9\xd6\x64\x2e");
    ItObj = F_KITCHEN_WINDOW;
  }
  else
  {
    if (Obj[O_YOU].loc == R_EAST_OF_HOUSE)
      GoToRoutine(R_KITCHEN);
    else
      GoToRoutine(R_EAST_OF_HOUSE);
  }
  }
  ReturnOverlay0_(0);
}

void DM_climbup_slide(void)
{
  if (Obj[O_YOU].loc == R_CELLAR)
    PrintBlockMsg(BLA);
  else
    PrintBlockMsg(BL0);
  ReturnOverlay0_(0);
}

void DM_examine_troll(void)
{
  PrintDesc_Troll(1);
  PrintCompText("\x0a"); /* above omits end newline*/
  ReturnOverlay0_(0);
}

void DM_count_matches(void)
{
  PrintCompText("\x8b\xcd\x76\x65\x20");

  if (MatchesLeft == 0) PrintCompText("\x6e\x6f");
  else PrintInteger(MatchesLeft);

  if (MatchesLeft == 1) PrintCompLine("\xee\xaf\x63\x68\x2e");
  else                  PrintCompLine("\xee\xaf\xfa\x65\x73\x2e");
  ReturnOverlay0_(0);
}

void DM_examine_r_b(void)
{
  PrintContents(O_RAISED_BASKET, "It contains:", 1);
  ReturnOverlay0_(0);
}


/* Extension *****************************************************************************************/

void DM_read_prayer(void)
{
  CallOverlay0((void*)DM_read_prayer_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_read_wooden_door(void)
{
  CallOverlay0((void*)DM_read_wooden_door_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_read_engravings(void)
{
  CallOverlay0((void*)DM_read_engravings_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_whereis_forest(void)
{
  CallOverlay0((void*)DM_whereis_forest_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_read_granite_wall(void)
{
  CallOverlay0((void*)DM_read_granite_wall_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_zorkmid(void)
{
  CallOverlay0((void*)DM_examine_zorkmid_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_grue(void)
{
  CallOverlay0((void*)DM_examine_grue_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_whereis_zorkmid(void)
{
  CallOverlay0((void*)DM_whereis_zorkmid_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_whereis_grue(void)
{
  CallOverlay0((void*)DM_whereis_grue_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_listento_troll(void)
{
  CallOverlay0((void*)DM_listento_troll_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_listento_thief(void)
{
  CallOverlay0((void*)DM_listento_thief_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_listento_cyclops(void)
{
  CallOverlay0((void*)DM_listento_cyclops_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_listento_forest(void)
{
  CallOverlay0((void*)DM_listento_forest_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_listento_songbird(void)
{
  CallOverlay0((void*)DM_listento_songbird_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_cross_lake(void)
{
  CallOverlay0((void*)DM_cross_lake_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_cross_stream(void)
{
  CallOverlay0((void*)DM_cross_stream_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_cross_chasm(void)
{
  CallOverlay0((void*)DM_cross_chasm_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_exorcise_ghosts(void)
{
  CallOverlay0((void*)DM_exorcise_ghosts_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_raise_rug(void)
{
  CallOverlay0((void*)DM_raise_rug_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_smell_gas(void)
{
  CallOverlay0((void*)DM_smell_gas_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_l_b(void)
{
  CallOverlay0((void*)DM_examine_l_b_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_lookin_large_bag(void)
{
  CallOverlay0((void*)DM_lookin_large_bag_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_lookthrough_grate(void)
{
  CallOverlay0((void*)DM_lookthrough_grate_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_lookin_water(void)
{
  CallOverlay0((void*)DM_lookin_water_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_kit_w(void)
{
  CallOverlay0((void*)DM_open_kit_w_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_close_kit_w(void)
{
  CallOverlay0((void*)DM_close_kit_w_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_move_push_rug(void)
{
  CallOverlay0((void*)DM_move_push_rug_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_trap_door(void)
{
  CallOverlay0((void*)DM_open_trap_door_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_close_trap_door(void)
{
  CallOverlay0((void*)DM_close_trap_door_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_smell_sandwich_bag(void)
{
  CallOverlay0((void*)DM_smell_sandwich_bag_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_raise_trap_door(void)
{
  CallOverlay0((void*)DM_raise_trap_door_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_push_blue_button(void)
{
  CallOverlay0((void*)DM_push_blue_button_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_wind_broken_canary(void)
{
  CallOverlay0((void*)DM_wind_broken_canary_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_climb_m_r(void)
{
  CallOverlay0((void*)DM_climb_m_r_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_climbup_white_cliff(void)
{
  CallOverlay0((void*)DM_climbup_white_cliff_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_match(void)
{
  CallOverlay0((void*)DM_examine_match_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_candles(void)
{
  CallOverlay0((void*)DM_examine_candles_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_torch(void)
{
  CallOverlay0((void*)DM_examine_torch_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_thief(void)
{
  CallOverlay0((void*)DM_examine_thief_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_tool_chest(void)
{
  CallOverlay0((void*)DM_examine_tool_chest_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_board(void)
{
  CallOverlay0((void*)DM_examine_board_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_chain(void)
{
  CallOverlay0((void*)DM_examine_chain_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_tool_chest(void)
{
  CallOverlay0((void*)DM_open_tool_chest_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_book(void)
{
  CallOverlay0((void*)DM_open_book_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_close_book(void)
{
  CallOverlay0((void*)DM_close_book_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_boarded_window(void)
{
  CallOverlay0((void*)DM_open_boarded_window_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_break_boarded_window(void)
{
  CallOverlay0((void*)DM_break_boarded_window_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_close_dam(void)
{
  CallOverlay0((void*)DM_open_close_dam_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_ring_hot_bell(void)
{
  CallOverlay0((void*)DM_ring_hot_bell_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_read_button(void)
{
  CallOverlay0((void*)DM_read_button_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_raise_l_g_wall(void)
{
  CallOverlay0((void*)DM_raise_l_g_wall_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_raise_lower_chain(void)
{
  CallOverlay0((void*)DM_raise_lower_chain_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_move_chain(void)
{
  CallOverlay0((void*)DM_move_chain_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_count_candles(void)
{
  CallOverlay0((void*)DM_count_candles_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_count_leaves(void)
{
  CallOverlay0((void*)DM_count_leaves_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_lamp(void)
{
  CallOverlay0((void*)DM_examine_lamp_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_cyclops(void)
{
  CallOverlay0((void*)DM_examine_cyclops_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_white_house(void)
{
  CallOverlay0((void*)DM_examine_white_house_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_close_b_d(void)
{
  CallOverlay0((void*)DM_open_close_b_d_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_close_s_d(void)
{
  CallOverlay0((void*)DM_open_close_s_d_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_close_b_of_c(void)
{
  CallOverlay0((void*)DM_open_close_b_of_c_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_close_trunk(void)
{
  CallOverlay0((void*)DM_open_close_trunk_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_close_large_bag(void)
{
  CallOverlay0((void*)DM_open_close_large_bag_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_open_front_door(void)
{
  CallOverlay0((void*)DM_open_front_door_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}
 
void DM_drink_water(void)
{
  CallOverlay0((void*)DM_drink_water_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_break_mirror(void)
{
  CallOverlay0((void*)DM_break_mirror_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_lookin_mirror(void)
{
  CallOverlay0((void*)DM_lookin_mirror_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_lookthrough_k_w(void)
{
  CallOverlay0((void*)DM_lookthrough_k_w_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_lookunder_rug(void)
{
  CallOverlay0((void*)DM_lookunder_rug_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_lookunder_leaves(void)
{
  CallOverlay0((void*)DM_lookunder_leaves_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_lookunder_rainbow(void)
{
  CallOverlay0((void*)DM_lookunder_rainbow_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_lookin_chimney(void)
{
  CallOverlay0((void*)DM_lookin_chimney_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_k_w(void)
{
  CallOverlay0((void*)DM_examine_k_w_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_lookin_bag_of_coins(void)
{
  CallOverlay0((void*)DM_lookin_bag_of_coins_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_lookin_trunk(void)
{
  CallOverlay0((void*)DM_lookin_trunk_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_squeeze_tube(void)
{
  CallOverlay0((void*)DM_squeeze_tube_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_whereis_songbird(void)
{
  CallOverlay0((void*)DM_whereis_songbird_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_examine_sword(void)
{
  CallOverlay0((void*)DM_examine_sword_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_push_brown_button(void)
{
  CallOverlay0((void*)DM_push_brown_button_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_push_yellow_button(void)
{
  CallOverlay0((void*)DM_push_yellow_button_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_enter_inf_b(void)
{
  CallOverlay0((void*)DM_enter_inf_b_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_exit_inflated_boat(void)
{
  CallOverlay0((void*)DM_exit_inflated_boat_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_move_leaves(void)
{
  CallOverlay0((void*)DM_move_leaves_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_ring_bell(void)
{
  CallOverlay0((void*)DM_ring_bell_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_climbthrough_crack(void)
{
  CallOverlay0((void*)DM_climbthrough_crack_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_whereis_granite_wall(void)
{
  CallOverlay0((void*)DM_whereis_granite_wall_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_whereis_white_house(void)
{
  CallOverlay0((void*)DM_whereis_white_house_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_eat_lunch(void)
{
  CallOverlay0((void*)DM_eat_lunch_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_eat_garlic(void)
{
  CallOverlay0((void*)DM_eat_garlic_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_wind_canary(void)
{
  CallOverlay0((void*)DM_wind_canary_ext, OVR_DO_MISC_EXT);
  ReturnOverlay0_(0);
}

void DM_read_book(void)
{
  CallOverlay0((void*)DM_read_book_tk, OVR_DO_MISC_TAKE_TO);
  ReturnOverlay0_(0);
}

void DM_read_advertisement(void)
{
  CallOverlay0((void*)DM_read_advertisement_tk, OVR_DO_MISC_TAKE_TO);
  ReturnOverlay0_(0);
}

void DM_read_match(void)
{
  CallOverlay0((void*)DM_read_match_tk, OVR_DO_MISC_TAKE_TO);
  ReturnOverlay0_(0);
}

void DM_read_map(void)
{
  CallOverlay0((void*)DM_read_map_tk, OVR_DO_MISC_TAKE_TO);
  ReturnOverlay0_(0);
}

void DM_read_boat_label(void)
{
  CallOverlay0((void*)DM_read_boat_label_tk, OVR_DO_MISC_TAKE_TO);
  ReturnOverlay0_(0);
}

void DM_read_tube(void)
{
  CallOverlay0((void*)DM_read_tube_tk, OVR_DO_MISC_TAKE_TO);
  ReturnOverlay0_(0);
}

void DM_read_owners_manual(void)
{
  CallOverlay0((void*)DM_read_owners_manual_tk, OVR_DO_MISC_TAKE_TO);
  ReturnOverlay0_(0);
}

void DM_read_guide(void)
{
  CallOverlay0((void*)DM_read_guide_tk, OVR_DO_MISC_TAKE_TO);
  ReturnOverlay0_(0);
}
