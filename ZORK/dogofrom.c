#include <ovr43.h>

/* defines, enums ------------------------------------------------------------- */

#define OVR_GO_FROM			43

#define O_YOU 1
#define O_GUIDE 65

#define INSIDE  2048

#define R_LIVING_ROOM 16
#define R_GRATING_CLEARING 12
#define R_KITCHEN 14
#define R_STUDIO 21
#define R_CELLAR 17
#define R_CYCLOPS_ROOM 42
#define R_RESERVOIR_NORTH 47
#define R_ATLANTIS_ROOM 58
#define R_LOUD_ROOM 63
#define R_CHASM_ROOM 65
#define R_EGYPT_ROOM 69
#define R_GAS_ROOM 99
#define R_LADDER_TOP 100
#define R_ATTIC 15
#define R_SMALL_CAVE 52
#define R_EW_PASSAGE 59
#define R_DEEP_CANYON 61
#define R_NORTH_TEMPLE 72
#define R_SMELLY_ROOM 98
#define R_MINE_4 109
#define R_TREASURE_ROOM 44
#define R_TINY_CAVE 53
#define R_ENTRANCE_TO_HADES 66
#define R_EAST_OF_HOUSE 5
#define R_LAND_OF_LIVING_DEAD 67
#define R_PATH 10 
#define R_UP_A_TREE 11
#define R_LADDER_BOTTOM 101
#define R_SLIDE_ROOM 110
#define R_CANYON_BOTTOM 91
#define R_CLIFF_MIDDLE 92
#define R_CANYON_VIEW 93
#define R_DOME_ROOM 70
#define R_TORCH_ROOM 71
#define R_ARAGAIN_FALLS 88
#define R_END_OF_RAINBOW 90

#define F_KITCHEN_WINDOW 2058

#define BL0 233 /* (256-23) */
#define BL9 242
#define BLA 243

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

extern unsigned char KitchenWindowOpen;
extern unsigned char YouAreInBoat;
extern unsigned char RainbowSolid;
extern unsigned char SpiritsBanished;
extern char TimePassed; /*flag: time passed during action*/
extern unsigned char RopeTiedToRail;

extern int ItObj, PrevItObj; /*the obj "it" refers to*/
/* extern functions ---------------------------------------------------------------- */

int GoToRoutine(int newroom);
void PrintBlockMsg(int newroom);
int GoToRoutine(int newroom);

short CallOverlay0(void* Routine, char CalledRamBank);
void ReturnOverlay0(short);
void ReturnOverlay0_(short);
/* variables ----------------------------------------------------------------------- */

/* overlays ------------------------------------------------------------------------ */
/* ----
void GoFrom_LivingRoom_Down(void);
void GoFrom_Cellar_Up(void);
void GoFrom_GratingClearing_Down(void);
void GoFrom_GratingRoom_Up(void);
void GoFrom_Kitchen_Down(void);
void GoFrom_Studio_Up(void);
void GoFrom_StoneBarrow_West(void);
void GoFrom_CyclopsRoom_Up(void);

#define GoFrom_LivingRoom_Down 0
#define GoFrom_Cellar_Up 0
#define GoFrom_GratingClearing_Down 0
#define GoFrom_GratingRoom_Up 0
#define GoFrom_Kitchen_Down 0
#define GoFrom_Studio_Up 0
#define GoFrom_StoneBarrow_West 0
#define GoFrom_CyclopsRoom_Up 0
---- */
/* code ---------------------------------------------------------------------------- */

void DM_climb_t_d(void)
{
  if (Obj[O_YOU].loc == R_LIVING_ROOM)
    CallOverlay0((void*)GoFrom_LivingRoom_Down, OVR_GO_FROM); /* GoFrom_LivingRoom_Down(); */
  else
    CallOverlay0((void*)GoFrom_Cellar_Up, OVR_GO_FROM); /* GoFrom_Cellar_Up(); */
  ReturnOverlay0_(0);
}

void DM_climbthrough_grate(void)
{
  if (Obj[O_YOU].loc == R_GRATING_CLEARING)
    CallOverlay0((void*)GoFrom_GratingClearing_Down, OVR_GO_FROM); /* GoFrom_GratingClearing_Down(); */
  else
    CallOverlay0((void*)GoFrom_GratingRoom_Up, OVR_GO_FROM); /* GoFrom_GratingRoom_Up(); */
  ReturnOverlay0_(0);
}

void DM_climb_ch(void)
{
  if (Obj[O_YOU].loc == R_KITCHEN)
    CallOverlay0((void*)GoFrom_Kitchen_Down, OVR_GO_FROM); /* GoFrom_Kitchen_Down(); */
  else
    CallOverlay0((void*)GoFrom_Studio_Up, OVR_GO_FROM); /* GoFrom_Studio_Up(); */
  ReturnOverlay0_(0);
}

void DM_climb_b_d(void)
{
  CallOverlay0((void*)GoFrom_StoneBarrow_West, OVR_GO_FROM); /* GoFrom_StoneBarrow_West(); */
  ReturnOverlay0_(0);
}

void DM_climbup_chimney(void)
{
  if (Obj[O_YOU].loc == R_STUDIO)
    CallOverlay0((void*)GoFrom_Studio_Up, OVR_GO_FROM); /* GoFrom_Studio_Up(); */
  else
    PrintBlockMsg(BL0);
  ReturnOverlay0_(0);
}

void DM_climbdown_chimney(void)
{
  if (Obj[O_YOU].loc == R_KITCHEN)
    CallOverlay0((void*)GoFrom_Kitchen_Down, OVR_GO_FROM); /* GoFrom_Kitchen_Down(); */
  else
    PrintBlockMsg(BL0);
  ReturnOverlay0_(0);
}

void DM_climbup_stairs(void)
{
  switch (Obj[O_YOU].loc)
  {
    case R_CELLAR:          CallOverlay0((void*)GoFrom_Cellar_Up, OVR_GO_FROM); /* GoFrom_Cellar_Up(); */             break;
    case R_CYCLOPS_ROOM:    CallOverlay0((void*)GoFrom_CyclopsRoom_Up, OVR_GO_FROM); /* GoFrom_CyclopsRoom_Up(); */        break;
    case R_KITCHEN:         GoToRoutine(R_ATTIC);         break;
    case R_RESERVOIR_NORTH: GoToRoutine(R_ATLANTIS_ROOM); break;
    case R_ATLANTIS_ROOM:   GoToRoutine(R_SMALL_CAVE);    break;
    case R_LOUD_ROOM:       GoToRoutine(R_DEEP_CANYON);   break;
    case R_CHASM_ROOM:      GoToRoutine(R_EW_PASSAGE);    break;
    case R_EGYPT_ROOM:      GoToRoutine(R_NORTH_TEMPLE);  break;
    case R_GAS_ROOM:        GoToRoutine(R_SMELLY_ROOM);   break;
    case R_LADDER_TOP:      GoToRoutine(R_MINE_4);        break;
    default:                   PrintBlockMsg(BL0);              break;
  }
  ReturnOverlay0_(0);
}

void DM_climbdown_stairs(void)
{
  switch (Obj[O_YOU].loc)
  {
    case R_LIVING_ROOM:   CallOverlay0((void*)GoFrom_LivingRoom_Down, OVR_GO_FROM); /* GoFrom_LivingRoom_Down(); */           break;
    case R_ATTIC:         GoToRoutine(R_KITCHEN);           break;
    case R_TREASURE_ROOM: GoToRoutine(R_CYCLOPS_ROOM);      break;
    case R_SMALL_CAVE:    GoToRoutine(R_ATLANTIS_ROOM);     break;
    case R_TINY_CAVE:     GoToRoutine(R_ENTRANCE_TO_HADES); break;
    case R_EW_PASSAGE:    GoToRoutine(R_CHASM_ROOM);        break;
    case R_DEEP_CANYON:   GoToRoutine(R_LOUD_ROOM);         break;
    case R_TORCH_ROOM:    GoToRoutine(R_NORTH_TEMPLE);      break;
    case R_NORTH_TEMPLE:  GoToRoutine(R_EGYPT_ROOM);        break;
    case R_SMELLY_ROOM:   GoToRoutine(R_GAS_ROOM);          break;
    default:                 PrintBlockMsg(BL0);                  break;
  }
  ReturnOverlay0_(0);
}

void DM_climb_k_w(void)
{
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
  ReturnOverlay0_(0);
}

void DM_climbthrough_slide(void)
{
  if (Obj[O_YOU].loc == R_CELLAR)
    PrintBlockMsg(BLA);
  else
  {
    if (YouAreInBoat == 0) 
      PrintCompLine("\x8b\x74\x75\x6d\x62\xcf\xcc\xf2\xb4\x81\x73\xf5\xe8\x2e\x2e\x2e\x2e\x0a");
    GoToRoutine(R_CELLAR);
  }
  ReturnOverlay0_(0);
}

void DM_climbthrough_gate(void)
{
  if (SpiritsBanished == 0)
    PrintCompLine("\x85\x67\xaf\x9e\x9a\x70\xc2\xd1\x63\xd1\xab\x62\xc4\xad\xa8\x6e"
"\x76\xb2\x69\x62\xcf\xc6\xd3\x63\x65\xa4\x49\xa6\x6d\x61\x6b\xbe\x86\xb6\xd1\x65"
"\xa2\xa3\xfa\x9e\xbd\x9f\xa5\xfa\xa8\x74\x2e");
  else
    GoToRoutine(R_LAND_OF_LIVING_DEAD);
  ReturnOverlay0_(0);
}

void DM_slidedown_slide(void)
{
  if (Obj[O_YOU].loc == R_CELLAR)
    PrintCompLine("\xdc\x75\x27\xa9\xa3\x6c\xa9\x61\x64\xc4\xaf\x80\xb0\xff\xbd\x6d\x2e");
  else
  {
    if (YouAreInBoat == 0) PrintCompLine("\x8b\x74\x75\x6d\x62\xcf\xcc\xf2\xb4"
"\x81\x73\xf5\xe8\x2e\x2e\x2e\x2e\x0a");
    GoToRoutine(R_CELLAR);
  }
  ReturnOverlay0_(0);
}

void DM_climbup_tree(void)
{
  if (Obj[O_YOU].loc == R_PATH)
    GoToRoutine(R_UP_A_TREE);
  else
    PrintBlockMsg(BL9);
  ReturnOverlay0_(0);
}

void DM_climbdown_tree(void)
{
  if (Obj[O_YOU].loc == R_PATH)
    PrintBlockMsg(BL0);
  else
    GoToRoutine(R_PATH);
  ReturnOverlay0_(0);
}

void DM_climbup_ladder(void)
{
  if (Obj[O_YOU].loc == R_LADDER_BOTTOM)
    GoToRoutine(R_LADDER_TOP);
  else
    PrintBlockMsg(BL0);
  ReturnOverlay0_(0);
}

void DM_climbdown_ladder(void)
{
  if (Obj[O_YOU].loc == R_LADDER_TOP)
    GoToRoutine(R_LADDER_BOTTOM);
  else
    PrintBlockMsg(BL0);
  ReturnOverlay0_(0);
}

void DM_climbdown_slide(void)
{
  if (Obj[O_YOU].loc == R_SLIDE_ROOM)
  {
    if (YouAreInBoat == 0) PrintCompLine("\x8b\x74\x75\x6d\x62\xcf\xcc\xf2\xb4\x81\x73\xf5\xe8\x2e\x2e\x2e\x2e\x0a");
    GoToRoutine(R_CELLAR);
  }
  else
    PrintBlockMsg(BL0);
  ReturnOverlay0_(0);
}

void DM_climb_c_c(void)
{
  switch (Obj[O_YOU].loc)
  {
    default:                 PrintBlockMsg(BL0);             break;
    case R_CLIFF_MIDDLE:  GoToRoutine(R_CANYON_VIEW);  break;
    case R_CANYON_BOTTOM: GoToRoutine(R_CLIFF_MIDDLE); break;
  }
  ReturnOverlay0_(0);
}

void DM_climbd_c_c(void)
{
  switch (Obj[O_YOU].loc)
  {
    case R_CANYON_VIEW:   GoToRoutine(R_CLIFF_MIDDLE);  break;
    case R_CLIFF_MIDDLE:  GoToRoutine(R_CANYON_BOTTOM); break;
    default:                 PrintBlockMsg(BL0);              break;
  }
  ReturnOverlay0_(0);
}

void DM_climbdown_rope(void)
{
  if (RopeTiedToRail && Obj[O_YOU].loc == R_DOME_ROOM)
    GoToRoutine(R_TORCH_ROOM);
  else
    PrintCompLine("\x85\xc2\xfc\xa8\x73\x93\xf0\xd5\x89\xad\x79\xa2\x97\x2e");
  ReturnOverlay0_(0);
}

void DM_cross_rainbow(void)
{
  if (Obj[O_YOU].loc == R_CANYON_VIEW)
    PrintCompLine("\x46\xc2\xf9\xa0\xa9\x3f\x21\x3f");
  else if (RainbowSolid == 0)
    PrintCompLine("\x43\xad\x86\xb7\xe2\x6b\xae\xb4\x77\xaf\xac\x20\x76\x61\x70\x6f\x72\x3f");
  else
  {
    if (Obj[O_YOU].loc == R_ARAGAIN_FALLS)
      GoToRoutine(R_END_OF_RAINBOW);
    else if (Obj[O_YOU].loc == R_END_OF_RAINBOW)
      GoToRoutine(R_ARAGAIN_FALLS);
    else
      PrintCompLine("\xdc\x75\x27\xdf\xc0\x61\xd7\x89\x73\x61\xc4\x77\xce\xfa\xb7\x61\x79\x2e\x2e\x2e");
  }    
  ReturnOverlay0_(0);
}

