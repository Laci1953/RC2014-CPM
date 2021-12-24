#include <string.h>

/* defines, enums ------------------------------------------------------------- */
#define AllocBuf 0xBB00

#define NUM_OBJECTS  80

#define CURE_WAIT  30
#define SCORE_MAX  350

#define STRENGTH_MIN  2
#define STRENGTH_MAX  7

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

#define F_SCENERY_NOTVIS 2049
#define F_NOTVIS 2050
#define F_AMB 2051

#define O_YOU 1
#define O_CYCLOPS 2
#define O_LAMP 36
#define O_BAR 47
#define O_INFLATED_BOAT 39
#define O_THIEF 5
#define O_TROLL 6
#define O_BAT 4
#define O_GHOSTS 3

#define R_CYCLOPS_ROOM 42
#define R_FOREST_1 6
#define R_TROLL_ROOM 18
#define R_LOUD_ROOM 63
#define R_RIVER_5 84
#define R_RIVER_3 80
#define R_RIVER_2 79
#define R_RIVER_1 78
#define R_RIVER_4 83
#define R_IN_STREAM 49
#define R_STREAM_VIEW 48
#define R_DAM_BASE 77
#define R_WHITE_CLIFFS_NORTH 81
#define R_WHITE_CLIFFS_SOUTH 82
#define R_SANDY_BEACH 86
#define R_RESERVOIR 46
#define R_RESERVOIR_SOUTH 45
#define R_RESERVOIR_NORTH 47
#define R_PATH 10
#define R_SHAFT_ROOM 97
#define R_ARAGAIN_FALLS 88
#define R_SOUTH_TEMPLE 73
#define R_DOME_ROOM 70
#define R_CHASM_ROOM 65
#define R_EAST_OF_CHASM 19
#define R_KITCHEN 14
#define R_SHORE 85
#define R_UP_A_TREE 11

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

extern int NumStrWords;
extern int CurWord;

extern unsigned char ExitFound;
extern unsigned char TimePassed;
extern unsigned char YouAreDead;
extern unsigned char YouAreInBoat;
extern unsigned char LoudRoomQuiet;
extern unsigned char GatesOpen;
extern unsigned char LowTide;
extern unsigned char TrollAllowsPassage;

extern int CyclopsState;
extern int NumDeaths;
extern int EnableCureRoutine;
extern int PlayerStrength;
extern int DownstreamCounter;
extern int TrollDescType;
extern int ThiefDescType; 
extern int Score;

/* extern functions ---------------------------------------------------------------- */
void PrintCompLine(char *p);
void PrintInteger(int num);
void PrintText(char *p);
void PrintLine(char *p);
void PrintCompText(char *comp_text);
void PrintPlayerRoomDesc(int force_description);
int IsPlayerInDarkness(void);
void YoureDead(void);
int MatchCurWord( char *match);
int GetAllObjFromInput(int room);
int GetRandom(int Range);

void ReturnOverlay0(short ret_val);
void ReturnOverlay0_(short ret_val);
void ReturnOverlay1(short ret_val);

/* extern variables ----------------------------------------------------------------------- */

/* variables ----------------------------------------------------------------------- */

int launch_from[8] = {R_DAM_BASE, R_WHITE_CLIFFS_NORTH, R_WHITE_CLIFFS_SOUTH, R_SHORE,
                        R_SANDY_BEACH, R_RESERVOIR_SOUTH, R_RESERVOIR_NORTH, R_STREAM_VIEW};
int   launch_to[8] = {R_RIVER_1, R_RIVER_3, R_RIVER_4, R_RIVER_5, R_RIVER_4, R_RESERVOIR,
                        R_RESERVOIR, R_IN_STREAM};

/* overlays ----------------------------------------------------------------------- */

/* code ---------------------------------------------------------------------------- */
void DoMisc_exit_inflated_boat(void)
{
  if (YouAreInBoat == 0)
    PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xa8\xb4\x69\x74\x21");
  else if (Room[Obj[O_YOU].loc].prop & R_BODYOFWATER)
    PrintCompLine("\x8b\x73\x68\xa5\x6c\xab\xfd\xb9\xb0\x65\x66\xd3\x9e\x64\xb2"
"\x65\x6d\x62\xbb\x6b\x97\x2e");
  else
  {
    YouAreInBoat = 0;
    Obj[O_INFLATED_BOAT].prop &= ~PROP_NOTTAKEABLE;
    PrintCompLine("\x4f\x6b\x61\x79\x2e");
    TimePassed = 1;
  }
}

int PlayerFightStrength(int adjust)
{
  int s =  STRENGTH_MIN + (STRENGTH_MAX - STRENGTH_MIN) * Score / SCORE_MAX;

  if (adjust)
    s += PlayerStrength;

  return s;
}

void DoOdysseus(void)
{
  if (Obj[O_YOU].loc != R_CYCLOPS_ROOM || Obj[O_CYCLOPS].loc == 0)
    PrintCompLine("\x57\xe0\x93\x94\xd0\x73\x61\x69\xd9\x72\x3f");
  else if (CyclopsState == 3)
    PrintCompLine("\x4e\xba\xfe\x9e\x74\xe2\x6b\x84\xbd\xc0\x69\x6d\xa4\x48\x65"
"\x27\xa1\x66\xe0\xa6\xe0\xcf\x65\x70\x2e");
  else
  {
    CyclopsState = 4;
    Obj[O_CYCLOPS].loc = 0;
    PrintCompLine("\x85\x63\x79\x63\xd9\x70\x73\xb5\xa0\xbb\x84\x81\x6e\x61\x6d"
"\x9e\xdd\xc0\x9a\x66\xaf\xa0\x72\x27\xa1\xe8\x61\x64\xec\xe4\x65\x6d\xbe\xb2"
"\xb5\x66\xcf\xbe\x80\xda\xe9\xf9\x62\xc4\x6b\xe3\x63\x6b\x84\x64\xf2\xb4\x81"
"\x77\xe2\xea\xca\x80\xfb\xe0\xa6\xdd\x80\xda\xe9\x6d\x2e");
    TimePassed = 1;
    ExitFound = 1;
  }

  ReturnOverlay0_(0);
}

void ActorResponse(int obj, int odysseus)
{
  switch (obj)
  {
    case O_CYCLOPS:
      if (odysseus)
        DoOdysseus();
      else
        PrintCompLine("\x85\x63\x79\x63\xd9\x70\xa1\x70\xa9\x66\xac\xa1\xbf\xf0"
"\x9c\xbd\xee\x61\x6b\x84\x63\xca\xd7\x72\x73\xaf\x69\x6f\x6e\x2e");
    break;

    case O_GHOSTS:
      PrintCompLine("\x85\x73\x70\x69\xf1\x74\xa1\x6a\xf3\xb6\xd9\x75\x64\xec\x8d"
"\xa8\x67\xe3\xa9\x86\x2e");
    break;

    case O_BAT:
      PrintCompLine("\x20\x20\x20\x20\x46\x77\xf3\x70\x21\x0a\x20\x20\x20\x20\x46"
"\x77\xf3\x70\x21\x0a\x20\x20\x20\x20\x46\x77\xf3\x70\x21\x0a\x20\x20\x20\x20\x46"
"\x77\xf3\x70\x21\x0a\x20\x20\x20\x20\x46\x77\xf3\x70\x21\x0a\x20\x20\x20\x20\x46"
"\x77\xf3\x70\x21");
    break;

    case O_THIEF:
      PrintCompLine("\x85\xa2\x69\x65\xd2\x9a\xd0\xc5\xc2\xb1\xb5\x73\x69\xcf\xe5"
"\x9f\x79\x70\x65\x2e");
    break;

    case O_TROLL:
      PrintCompLine("\x85\x74\xc2\xdf\xa8\x73\x93\x6d\x75\xfa\x8a\xd0\x63\xca\xd7"
"\x72\x73\xaf\x69\xca\xe2\xb2\x74\x2e");
    break;
  }
}

int VerifyActor(int obj)
{
  if (obj == F_SCENERY_NOTVIS || obj == F_NOTVIS)
    {PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21"); return 1;}
  else if (obj == F_AMB)
    {PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63"
"\xa3\x62\xa5\xa6\x77\x68\xba\x8f\x77\xad\xa6\xbd\x9f\xe2\x6b\x9f\x6f\x2e"); return 1;}
  else if (obj == O_YOU || obj >= NUM_OBJECTS)
    {PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21"); return 1;}
  else if ((Obj[obj].prop & PROP_ACTOR) == 0)
    {PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21"); return 1;}
  else if (Obj[obj].loc != Obj[O_YOU].loc)
    {PrintCompLine("\xbc\xaf\xeb\xac\x73\xca\xa8\x73\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); return 1;}

  return 0;
}

/* actor, *** until end of input*/
void DoCommandActor(int obj)
{
  int odysseus = 0;

  while (CurWord < NumStrWords)
  {
    if (MatchCurWord("odysseus") || MatchCurWord("ulysses"))
      odysseus = 1;
    else
      CurWord++;
  }

  if (VerifyActor(obj) == 0)
    ActorResponse(obj, odysseus);

  ReturnOverlay1(0);
}

/* talkto/ask/tell actor (about) (***)*/
void DoTalkTo(void)
{
  int obj, odysseus = 0;

  obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) return;
  if (VerifyActor(obj)) return;

  while (CurWord < NumStrWords)
  {
    if (MatchCurWord("then"))
      {CurWord--; break;} /* end of this turn's command; back up so "then" can be matched later*/
    else if (MatchCurWord("odysseus") || MatchCurWord("ulysses"))
      odysseus = 1;
    else
      CurWord++;
  }

  ActorResponse(obj, odysseus);

  ReturnOverlay0(0);
}

/* greet/hello (,) actor*/
void DoGreet(void)
{
  int obj, odysseus = 0;

  MatchCurWord("and");
  obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) return;
  if (VerifyActor(obj)) return;

  if (obj == O_THIEF && ThiefDescType == 1) /* unconscious*/
    PrintCompLine("\x85\xa2\x69\x65\x66\xb5\xef\x84\xd1\x6d\x70\xd3\xbb\x69\xec"
"\xa8\x6e\xe7\x70\x61\x63\xc7\xaf\xd5\xb5\x9a\xf6\x61\x62\xcf\x89\x61\x63\x6b"
"\xe3\x77\xcf\x64\x67\x9e\x92\xe6\xa9\x65\xf0\x9c\xf8\xa2\xc0\x9a\xfe\x75\xe2"
"\xe6\xf4\x63\x69\xa5\x73\xed\x73\x73\x2e");
  else if (obj == O_TROLL && TrollDescType == 1) /* unconscious*/
    PrintCompLine("\x55\x6e\x66\xd3\x74\xf6\xaf\x65\xec\xb5\x81\x74\xc2\xdf"
"\x91\x27\xa6\xa0\xbb\x86\x2e");
  else
    ActorResponse(obj, odysseus);

  ReturnOverlay0(0);
}

/* say *** (to actor)*/
void DoSay(void)
{
  int obj = 0, odysseus = 0, temp;

  while (CurWord < NumStrWords)
  {
    if (MatchCurWord("to"))
      {CurWord--; break;} /* back up so "to" can be matched below*/
    else if (MatchCurWord("odysseus") || MatchCurWord("ulysses"))
      odysseus = 1;
    else
      CurWord++;
  }

  if (MatchCurWord("to"))
  {
    obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) return;
  }

  if (obj == 0)
  {
    /* look for exactly one actor in player's room who is described (thief can be invisible)*/
    for (temp=2; temp<NUM_OBJECTS; temp++)
      if ((Obj[temp].prop & PROP_ACTOR) &&
          (Obj[temp].prop & PROP_NODESC) == 0 &&
          Obj[temp].loc == Obj[O_YOU].loc)
        {if (obj == 0) obj = temp; else {obj = 0; break;}}
    if (obj == 0) /* more than one or no actors*/
      {PrintCompLine("\x8b\xed\xd5\x89\x73\xfc\x63\x69\x66\xc4\x77\x68\xba\xbd\x9f"
"\xe2\x6b\x9f\x6f\x2e"); return;}
  }

  if (VerifyActor(obj) == 0)
    ActorResponse(obj, odysseus);

  ReturnOverlay0(0);
}

void PrintRandomFun(void)
{
  switch (GetRandom(4))
  {
    case 0: PrintCompLine("\x56\xac\xc4\x67\xe9\x64\xa4\x4e\xf2\x86\x91\xe6\xba"
"\xbd\x80\xaa\x65\x63\xca\xab\x67\xf4\x64\x65\x2e"); break;
    case 1: PrintCompLine("\x41\xa9\x86\xfb\x6e\x6a\x6f\x79\x84\x92\xd6\x6c\x66\x3f");                     break;
    case 2: PrintCompLine("\x57\xa0\xf3\xf3\xf3\xf3\x65\x21\x21\x21\x21\x21");                              break;
    case 3: PrintCompLine("\x44\xba\x8f\x65\x78\xfc\x63\xa6\x6d\x9e\xbd\xa3\x70"
"\x70\xfd\x75\x64\x3f");                   break;
  }
}

void PrintRandomJumpDeath(void)
{
  switch (GetRandom(3))
  {
    case 0: PrintCompLine("\x8b\x73\x68\xa5\x6c\xab\xcd\xd7\xcb\xe9\x6b\xd5\xb0"
"\x65\x66\xd3\x9e\x8f\xcf\x61\xfc\x64\x2e");                   break;
    case 1: PrintCompLine("\x49\xb4\x81\x6d\x6f\x76\x69\xbe\xb5\x92\xcb\x69\x66"
"\x9e\x77\xa5\x6c\xab\xef\xeb\xe0\x73\x84\xef\x66\xd3\x9e\x92\xfb\x79\x65\x73\x2e"); break;
    case 2: PrintCompLine("\x47\xac\xca\x69\x6d\x6f\x2e\x2e\x2e");                                                 break;
  }
}

void DoJump(void)
{
  int obj = 0;

  if (MatchCurWord("across") || MatchCurWord("from") || MatchCurWord("in") ||
      MatchCurWord("into") || MatchCurWord("off") || MatchCurWord("over"))
  {
    obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) return;

    if (obj == F_SCENERY_NOTVIS || obj == F_NOTVIS)
      {PrintCompLine("\xbc\xaf\xa8\x73\x93\x76\xb2\x69\x62\xcf\xc0\xac\x65\x21"); return;}
    else if (obj == F_AMB)
      {PrintCompLine("\x8b\xed\xd5\x89\xef\xee\xd3\x9e\x73\xfc\x63\x69\x66\x69\x63\x2e"); return;}
    else if (obj == O_YOU)
      {PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21"); return;}
  }

  if (obj == 0 || obj >= NUM_OBJECTS)
  {
    switch (Obj[O_YOU].loc)
    {
      case R_KITCHEN:
      case R_EAST_OF_CHASM:
      case R_RESERVOIR:
      case R_CHASM_ROOM:
      case R_DOME_ROOM:
      case R_SOUTH_TEMPLE:
      case R_ARAGAIN_FALLS:
      case R_SHAFT_ROOM:
        PrintCompLine("\xbc\x9a\x77\xe0\xe4\xff\xa3\x20\xd7\x72\xc4\x73\x61\x66"
"\x9e\x70\xfd\x63\x9e\xbd\x9f\x72\xc4\x6a\x75\x6d\x70\x97\x2e");
        PrintRandomJumpDeath();
        YoureDead(); /* ##### RIP #####*/
      break;

      case R_UP_A_TREE:
        PrintCompLine("\x49\xb4\xd0\x66\xbf\xa6\xdd\x20\xf6\x61\x63\x63\xfe\xbd"
"\x6d\xd5\xcc\xbb\x97\xb5\x8f\x6d\xad\x61\x67\x9e\xbd\xcb\x8c\xca\x86\xb6\x66"
"\xf3\xa6\xf8\xa2\xa5\xa6\x6b\x69\xdf\x84\x92\xd6\x6c\x66\x2e\x0a");
        GoToRoutine(R_PATH);
      break;

      default:
        PrintRandomFun();
      break;
    }
  }
  else if (Obj[obj].loc == Obj[O_YOU].loc)
  {
    if (Obj[obj].prop & PROP_ACTOR)
      PrintCompLine("\x49\xa6\x9a\xbd\xba\x62\x69\xc1\xbd\x20\x6a\x75\x6d\x70"
"\xae\xd7\x72\x2e");
    else
      PrintRandomFun();
  }
  else
    PrintCompLine("\xbc\xaf\xb7\xa5\x6c\xab\xef\xa3\xe6\xe9\xab\x74\xf1\x63\x6b\x2e");

  ReturnOverlay0(0);
}

void DoSleep(void)
{
  PrintCompLine("\x99\xa9\x27\xa1\xe3\xa2\x84\xbd\xaa\xcf\x65\x70\xae\x6e\x2e");
  ReturnOverlay0_(0);
}

void DoDisembark(void)
{
  if (YouAreInBoat == 0)
    PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xa3\x62\x6f\xbb\xab\xad\x79\xa2\x97\x21");
  else
    DoMisc_exit_inflated_boat();
  ReturnOverlay0_(0);
}

void BoatGoToRoutine(int newroom)
{
  int prev_darkness;

  if ((Room[newroom].prop & R_BODYOFWATER) == 0)
    PrintCompLine("\x85\x6d\x61\x67\x69\x63\xb0\x6f\xaf\xb3\xe1\xbe\x89\xd0\xa9"
"\xc5\xae\xb4\x81\x73\x68\xd3\x65\x2e\x0a");

  Obj[O_INFLATED_BOAT].loc = newroom;

  prev_darkness = IsPlayerInDarkness();

  Obj[O_YOU].loc = newroom;
  TimePassed = 1;

  if (IsPlayerInDarkness())
  {
    if (prev_darkness)
    {
      /*kill player that tried to go from dark to dark*/
      PrintCompLine("\x0a\x0a\x0a\x0a\x0a\x4f\x68\xb5\xe3\x21\x88\xc0\x61\xd7"
"\xb7\xe2\x6b\xd5\xa8\xe5\xba\x81\x73\xfd\xd7\xf1\x9c\x66\xad\x67\xa1\xdd\xa3"
"\xcb\xd8\x6b\x84\x67\x72\x75\x65\x21");
      YoureDead(); /* ##### RIP #####*/
      return;
    }
    else PrintCompLine("\x8b\xcd\xd7\xee\x6f\xd7\xab\xa7\xbd\xa3\xcc\xbb\x6b"
"\xeb\xfd\x63\x65\x2e");
  }

  PrintPlayerRoomDesc(0);
}

void DoLaunch(void)
{
  int i;

  if (Room[Obj[O_YOU].loc].prop & R_BODYOFWATER)
  {
    PrintCompText("\x8b\xbb\x9e\xca\x80\x20");
    if (Obj[O_YOU].loc == R_RESERVOIR)
      PrintCompText("\xa9\xd6\x72\x76\x6f\x69\x72");
    else if (Obj[O_YOU].loc == R_IN_STREAM)
      PrintCompText("\xc5\xa9\x61\x6d");
    else
      PrintCompText("\xf1\x76\x65\x72");
    PrintCompLine("\xb5\xd3\xc0\x61\xd7\x86\xc6\xd3\x67\xff\xd1\x6e\x3f");
    return;
  }

  if (YouAreInBoat == 0) {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xa8\xb4\x81"
"\x62\x6f\x61\x74\x21"); return;}

  for (i=0; i<8; i++)
    if (Obj[O_YOU].loc == launch_from[i]) break;
  if (i == 8) {PrintCompLine("\x8b\xe7\x93\xfd\xf6\xfa\xa8\xa6\xa0\x72\x65\x2e"); return;}

  DownstreamCounter = -1; /* start at -1 to account for this turn*/
  BoatGoToRoutine(launch_to[i]);

  ReturnOverlay0(0);
}

void DoLand(void)
{
  if ((Room[Obj[O_YOU].loc].prop & R_BODYOFWATER) == 0)
    {PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xae\xb4\x81\x77\xaf\x65\x72\x21"); return;}

  switch (Obj[O_YOU].loc)
  {
    case R_RESERVOIR : PrintCompLine("\x8b\xe7\xb4\xfd\xb9\xfb\xc7\xa0\xb6\xbd"
"\x80\xe4\xd3\xa2\xae\xb6\x81\x73\xa5\x74\x68\x2e"); break;
    case R_RIVER_2   : PrintCompLine("\x99\xa9\x87\xe3\xaa\x61\x66\x9e\xfd\xb9"
"\x84\x73\x70\xff\xc0\xac\x65\x2e");            break;
    case R_RIVER_4   : PrintCompLine("\x8b\xe7\xb4\xfd\xb9\xfb\xc7\xa0\xb6\xbd"
"\x80\xfb\xe0\xa6\xd3\x80\xb7\xbe\x74\x2e");   break;

    case R_IN_STREAM : BoatGoToRoutine(R_STREAM_VIEW       ); break;
    case R_RIVER_1   : BoatGoToRoutine(R_DAM_BASE          ); break;
    case R_RIVER_3   : BoatGoToRoutine(R_WHITE_CLIFFS_NORTH); break;
    case R_RIVER_5   : BoatGoToRoutine(R_SHORE             ); break;

    default: PrintCompLine("\xdc\x75\x27\xa9\xe4\xff\xae\xb4\x81\x77\xaf\x65\x72\x21"); break;
  }

  ReturnOverlay0_(0);
}

void DoEcho(void)
{
  if (Obj[O_YOU].loc == R_LOUD_ROOM &&
      LoudRoomQuiet == 0 &&
      (GatesOpen || LowTide == 0))
  {
    LoudRoomQuiet = 1;
    Obj[O_BAR].prop &= ~PROP_SACRED;
    PrintCompLine("\x85\x61\x63\xa5\xc5\x69\x63\xa1\xdd\x80\xda\xe9\xf9\xfa\xad"
"\x67\x9e\x73\x75\x62\x74\x6c\x79\x2e");
    TimePassed = 1;
  }
  else
    PrintCompLine("\x45\xfa\xba\x65\xfa\x6f\x2e\x2e\x2e");

  ReturnOverlay0_(0);
}

void DoPray(void)
{
  TimePassed = 1;

  if (Obj[O_YOU].loc == R_SOUTH_TEMPLE)
  {
    if (YouAreDead)
    {
      PrintCompLine("\x46\xc2\xf9\x81\x64\xb2\x74\xad\x63\x9e\x81\x73\xa5\xb9"
"\x8a\xd0\xd9\xed\x9f\x72\x75\x6d\xfc\xa6\x9a\xa0\xbb\x64\x83\x9e\xc2\xe1\xb0"
"\x65\x63\xe1\xbe\x20\xd7\x72\xc4\x62\xf1\x67\x68\xa6\x8c\x8f\x66\xf3\xea\x64"
"\xb2\x65\x6d\x62\x6f\x64\x69\xd5\xa4\x49\xb4\xd0\x6d\xe1\xd4\x74\xb5\x81\x62"
"\xf1\x67\x68\x74\xed\x73\xa1\x66\x61\xe8\xa1\x8c\x8f\x66\xa7\xab\x92\xd6\x6c"
"\xd2\xf1\x73\x84\xe0\xa8\xd2\x66\xc2\xf9\xd0\xd9\x9c\x73\xcf\x65\x70\xb5\xe8"
"\x65\x70\xa8\xb4\x81\x77\xe9\x64\x73\xa4\x49\xb4\x81\x64\xb2\x74\xad\x63\x9e"
"\x8f\xe7\xb4\x66\x61\xa7\x74\xec\xc0\xbf\xb6\xd0\x73\xca\x67\x62\x69\x72\xab"
"\x8c\x81\x73\xa5\xb9\xa1\xdd\x80\xc6\xd3\xbe\x74\x2e\x0a");
      YouAreDead = 0;
      if (Obj[O_TROLL].loc == R_TROLL_ROOM)
        TrollAllowsPassage = 0;
      Obj[O_LAMP].prop &= ~PROP_NODESC;
      Obj[O_LAMP].prop &= ~PROP_NOTTAKEABLE;
      Obj[O_YOU].prop &= ~PROP_LIT;
    }
    else
      YouAreInBoat = 0; /* in case you're in it*/

    ExitFound = 1;
    GoToRoutine(R_FOREST_1);
  }
  else
  {
    if (YouAreDead)
      PrintCompLine("\xdc\xd8\xeb\xf4\x79\xac\xa1\xbb\x9e\xe3\xa6\xa0\xbb\x64\x2e");
    else
      PrintCompLine("\x49\xd2\x8f\x70\xf4\xc4\xd4\xa5\x67\x68\xb5\x92\xeb\xf4"
"\x79\xac\xa1\x6d\x61\xc4\xef\xa3\x6e\x73\x77\xac\x65\x64\x2e");
  }

  ReturnOverlay0_(0);
}

void DoVersion(void)
{
  char* buffer = (char*)AllocBuf;

  strcpy(buffer, "Compiled on ");
  strcat(buffer, "December 2021");

  PrintLine(buffer);

  ReturnOverlay0(0);
}

void DoDiagnose(void)
{
  int wounds, count, death_dist = PlayerFightStrength(0) + PlayerStrength;

  if (EnableCureRoutine == 0) wounds = 0;
  else                        wounds = -PlayerStrength;

  if (wounds == 0)
    PrintCompLine("\x8b\xbb\x9e\xa7\xeb\xac\x66\x65\x63\xa6\xa0\xe2\x74\x68\x2e");
  else
  {
    PrintCompText("\x8b\xcd\x76\x65\x20");
         if (wounds == 1) PrintCompText("\xd0\xf5\x67\x68\xa6\x77\xa5\x6e\x64");
    else if (wounds == 2) PrintCompText("\xd0\xd6\xf1\xa5\xa1\x77\xa5\x6e\x64");
    else if (wounds == 3) PrintCompText("\xd6\xd7\xf4\xea\x77\xa5\x6e\x64\x73");
    else                  PrintCompText("\xd6\xf1\xa5\xa1\x77\xa5\x6e\x64\x73");
    PrintCompText("\xb5\x77\xce\xfa\xb7\x69\xdf\xb0\x9e\x63\xd8\xd5\xa3\x66\xd1\x72\x20");
    count = CURE_WAIT * (wounds - 1) + EnableCureRoutine;
    PrintInteger(count);
    if (count == 1) PrintCompLine("\xee\x6f\x76\x65\x2e");
    else            PrintCompLine("\xee\x6f\xd7\x73\x2e");
  }

  PrintCompText("\x8b\xe7\x6e\x20");
       if (death_dist == 0) PrintCompLine("\x65\x78\xfc\x63\xa6\xe8\xaf\xde\x73"
"\xe9\x6e\x2e");
  else if (death_dist == 1) PrintCompLine("\xef\x20\x6b\x69\xdf\xd5\xb0\xc4\xca"
"\x9e\x6d\xd3\x9e\xf5\x67\x68\xa6\x77\xa5\x6e\x64\x2e");
  else if (death_dist == 2) PrintCompLine("\xef\x20\x6b\x69\xdf\xd5\xb0\xc4\xd0"
"\xd6\xf1\xa5\xa1\x77\xa5\x6e\x64\x2e");
  else if (death_dist == 3) PrintCompLine("\x73\xd8\x76\x69\xd7\xae\xed\xaa\xac"
"\x69\xa5\xa1\x77\xa5\x6e\x64\x2e");
  else                      PrintCompLine("\x73\xd8\x76\x69\xd7\xaa\x65\xd7\xf4"
"\xea\x77\xa5\xb9\x73\x2e");

  if (NumDeaths != 0)
  {
    PrintCompText("\x8b\xcd\xd7\xb0\xf3\xb4\x6b\x69\xdf\x65\x64\x20");
    if (NumDeaths == 1) PrintCompLine("\xca\x63\x65\x2e");
    else                PrintCompLine("\x74\xf8\x63\x65\x2e");
  }

  ReturnOverlay0(0);
}

void DoSwim(void)
{
  if (Room[Obj[O_YOU].loc].prop & R_WATERHERE)
    PrintCompLine("\x53\xf8\x6d\x6d\x84\xb2\x93\xfe\x75\xe2\xec\xa3\xdf\xf2"
"\xd5\xa8\xb4\x81\x64\xf6\x67\x65\x6f\x6e\x2e");
  else
    PrintCompLine("\x47\xba\x6a\x75\x6d\x70\xa8\xb4\xd0\xfd\x6b\x65\x21");

  ReturnOverlay0_(0);
}




