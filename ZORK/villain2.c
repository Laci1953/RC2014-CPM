#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* malloc free */
#include <ctype.h>

/* defines, enums ------------------------------------------------------------- */
#define HERO  0  /* never set*/

#define NUM_ROOMS  111 /* including null room 0 */

#define R_TREASURE_ROOM 44

#define NUM_OBJECTS  80

enum
{
  VILLAIN_TROLL,
  VILLAIN_THIEF,
  VILLAIN_CYCLOPS,

  NUM_VILLAINS
};

#define STRENGTH_MIN  2
#define STRENGTH_MAX  7

#define SCORE_MAX  350

#define INSIDE  2048

#define  O_YOU 1
#define  O_CYCLOPS 2
#define  O_THIEF 5
#define  O_TROLL 6
#define  O_AXE 21
#define  O_CHALICE 25
#define  O_KNIFE 33
#define  O_ROPE 50
#define  O_RUSTY_KNIFE 51
#define  O_LARGE_BAG 59          
#define  O_STILETTO 60      
#define  O_SWORD 61 
#define  O_EGG 72 

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

extern int VillainObj[];
extern char *VillainName[];
extern int VillainBestWeaponAgainst[];
extern int VillainBestWeaponAgainstAdvantage[];

/* extern data structures ---------------------------------------------------------- */
/* parser */
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

extern  char RopeTiedToRail;
extern  char TrollAllowsPassage;
extern  char YouAreDead;
extern  char ThiefHere;
extern  char ThiefEngrossed;
extern  char YouAreStaggered;

extern int Score;
extern int LoadAllowed;
extern int PlayerStrength;
extern int TrollDescType;
extern int ThiefDescType;
extern int EnableCureRoutine;

extern  char VillainAttacking[NUM_VILLAINS];
extern  char VillainStaggered[NUM_VILLAINS];
extern int VillainWakingChance[NUM_VILLAINS];
extern int VillainStrength[NUM_VILLAINS];

extern int VillainObj[];
extern char *VillainName[];
extern int VillainBestWeaponAgainst[];
extern int VillainBestWeaponAgainstAdvantage[];

/* extern functions ---------------------------------------------------------------- */
/*game.c*/
int PercentChance(int , int );

/*parser.c*/
void PrintCompText(char *);
void PrintCompLine(char *);
int IsPlayerInDarkness(void);

/* code ---------------------------------------------------------------------------- */

void ThiefRecoverStiletto(void)
{
  if (Obj[O_STILETTO].loc == Obj[O_THIEF].loc)
  {
    Obj[O_STILETTO].loc = INSIDE + O_THIEF;
    Obj[O_STILETTO].prop |= PROP_NODESC;
    Obj[O_STILETTO].prop |= PROP_NOTTAKEABLE;
  }
}

int ThiefRob(int loc, int prob)
{
  int flag = 0, obj;

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == loc &&
        (Obj[obj].prop & PROP_NODESC) == 0 &&
        (Obj[obj].prop & PROP_SACRED) == 0 &&
        Obj[obj].thiefvalue > 0 &&
        (prob < 0 || PercentChance(prob, -1)))
  {
    flag = 1;
    Obj[obj].loc = INSIDE + O_THIEF;
    Obj[obj].prop |= PROP_MOVEDDESC;
    Obj[obj].prop |= PROP_NODESC;
    Obj[obj].prop |= PROP_NOTTAKEABLE;
  }

  return flag;
}

int ThiefDepositBooty(int room)
{
  int flag = 0, obj;

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == INSIDE + O_THIEF &&
        Obj[obj].thiefvalue > 0 &&
        obj != O_STILETTO &&
        obj != O_LARGE_BAG)
  {
    flag = 1;
    Obj[obj].loc = room;
    if (obj == O_EGG)
      Obj[O_EGG].prop |= PROP_OPEN;
  }

  return flag;
}

int ThiefDropJunk(int room)
{
  int flag = 0, obj;

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == INSIDE + O_THIEF &&
        Obj[obj].thiefvalue == 0 &&
        PercentChance(30, -1) &&
        obj != O_STILETTO &&
        obj != O_LARGE_BAG)
  {
    if (flag == 0 && room == Obj[O_YOU].loc)
    {
      flag = 1;
      PrintCompLine("\x85\xc2\x62\xef\x72\xb5\x72\x75\x6d\x6d\x61\x67\x84\xa2\xc2\x75\x67\xde"
"\xce\xa1\x62\x61\x67\xb5\x64\xc2\x70\xfc\xab\xd0\x66\x65\x77\xa8\xd1\x6d\xa1\x94\x66\xa5\xb9"
"\x20\x76\xe2\x75\x65\xcf\x73\x73\x2e");
    }
    Obj[obj].loc = room;
    Obj[obj].prop &= ~PROP_NODESC;
    Obj[obj].prop &= ~PROP_NOTTAKEABLE;
  }

  return flag;
}

void ThiefHackTreasures(void)
{
  int obj;

  ThiefRecoverStiletto();

  Obj[O_THIEF].prop |= PROP_NODESC;

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == R_TREASURE_ROOM &&
        obj != O_CHALICE &&
        obj != O_THIEF)
  {
    Obj[obj].prop &= ~PROP_NODESC;
    Obj[obj].prop &= ~PROP_NOTTAKEABLE;
  }
}

void ThiefRobMaze(int room)
{
  int obj;

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == room &&
        (Obj[obj].prop & PROP_NODESC) == 0 &&
        (Obj[obj].prop & PROP_NOTTAKEABLE) == 0 &&
        PercentChance(40, -1))
  {
    PrintCompLine("\x8b\xa0\xbb\xb5\xdd\xd2\xa7\x80\xcc\xb2\x74\xad\x63\x65\xb5\x73\xe1"
"\x65\xca\x9e\x73\x61\x79\x84\x22\x4d\x79\xb5\x49\xb7\xca\xe8\xb6\x77\xcd\xa6\xa2\x9a"
"\x66\xa7\x9e\xc7\x65\xf9\x9a\x64\x6f\x84\xa0\xa9\x2e\x22");
    if (PercentChance(60, 80))
    {
      Obj[obj].loc = INSIDE + O_THIEF;
      Obj[obj].prop |= PROP_MOVEDDESC;
      Obj[obj].prop |= PROP_NODESC;
      Obj[obj].prop |= PROP_NOTTAKEABLE;
    }
    break;
  }
}

void ThiefStealJunk(int room)
{
  int obj;

  for (obj=2; obj<NUM_OBJECTS; obj++)
    if (Obj[obj].loc == room &&
        Obj[obj].thiefvalue == 0 &&
        (Obj[obj].prop & PROP_NODESC) == 0 &&
        (Obj[obj].prop & PROP_NOTTAKEABLE) == 0 &&
        (Obj[obj].prop & PROP_SACRED) == 0 &&
        (obj == O_STILETTO || PercentChance(10, -1)))
  {
    Obj[obj].loc = INSIDE + O_THIEF;
    Obj[obj].prop |= PROP_MOVEDDESC;
    Obj[obj].prop |= PROP_NODESC;
    Obj[obj].prop |= PROP_NOTTAKEABLE;

    if (obj == O_ROPE) /* will never happen because it's sacred*/
      RopeTiedToRail = 0;

    if (room == Obj[O_YOU].loc)
      PrintCompLine("\x8b\x73\x75\x64\xe8\x6e\xec\xe4\xff\x69\x63\x9e\xa2\xaf\xaa"
"\xe1\x65\xa2\x84\x76\xad\xb2\xa0\x64\x2e");

    break;
  }
}

int PlayerFightStrength(int adjust)
{
  int s =  STRENGTH_MIN + (STRENGTH_MAX - STRENGTH_MIN) * Score / SCORE_MAX;

  if (adjust)
    s += PlayerStrength;

  return s;
}

int VillainFightStrength(int i, int player_weapon)
{
  int obj, strength;

  obj = VillainObj[i];
  strength = VillainStrength[i];

  if (strength >= 0)
  {
    if (obj == O_THIEF && ThiefEngrossed)
    {
      ThiefEngrossed = 0;
      if (strength > 2) strength = 2;
    }

    if (player_weapon &&
        (Obj[player_weapon].prop & PROP_WEAPON) &&
        player_weapon == VillainBestWeaponAgainst[i])
    {
      strength -= VillainBestWeaponAgainstAdvantage[i];
      if (strength < 1) strength = 1;
    }
  }

  return strength;
}

int ThiefWinning(void)
{
  int vs = VillainStrength[VILLAIN_THIEF];
  int ps = vs - PlayerFightStrength(1);

       if (ps >  3) return PercentChance(90, -1);
  else if (ps >  0) return PercentChance(75, -1);
  else if (ps == 0) return PercentChance(50, -1);
  else if (vs >  1) return PercentChance(25, -1);
  else              return PercentChance(10, -1);
}

int ThiefVsAdventurer(int here)
{
  int prev_darkness;
  int robbed = 0; /* 1: player  2: room*/

  if (YouAreDead == 0 && Obj[O_YOU].loc == R_TREASURE_ROOM)
  {
  }
  else if (ThiefHere == 0)
  {
    if (YouAreDead == 0 && here == 0 && PercentChance(30, -1))
    {
      if (Obj[O_STILETTO].loc == INSIDE + O_THIEF)
      {
        Obj[O_THIEF].prop &= ~PROP_NODESC;
        PrintCompLine("\x53\xe1\x65\xca\x9e\xe7\x72\x72\x79\x84\xd0\xfd\x72\x67\x9e\x62"
"\x61\xc1\x9a\xe7\x73\x75\xe2\xec\xcb\xbf\x6e\x84\x61\x67\x61\xa7\xc5\xae\xed\x8a\x81"
"\x77\xe2\x6c\xa1\xa0\xa9\xa4\x48\x9e\x64\x6f\xbe\xe4\xff\xaa\xfc\x61\x6b\xb5\x62\xf7"
"\xa8\xa6\x9a\x63\xcf\xbb\xc6\xc2\xf9\xce\xa1\xe0\xfc\x63\xa6\xa2\xaf\x80\xb0\x61\xc1"
"\xf8\xdf\xb0\x9e\x74\x61\x6b\xd4\xae\x6e\xec\xae\xd7\xb6\xce\xa1\xe8\x61\xab\x62\x6f\x64\x79\x2e");
        ThiefHere = 1;
        return 1;
      }
      else
      {
        Obj[O_STILETTO].loc = INSIDE + O_THIEF;
        Obj[O_STILETTO].prop |= PROP_NODESC;
        Obj[O_STILETTO].prop |= PROP_NOTTAKEABLE;
        Obj[O_THIEF].prop &= ~PROP_NODESC;
        PrintCompLine("\x8b\x66\xf3\xea\xd0\xf5\x67\x68\xa6\x66\x97\xac\x2d\xbd\x75\xfa"
"\xb5\x8c\x74\xd8\x6e\x97\xb5\xe3\xf0\x63\x9e\xd0\x67\xf1\x6e\x6e\x84\x66\x69\x67\xd8"
"\x9e\x68\x6f\x6c\x64\x84\xd0\xfd\x72\x67\x9e\x62\x61\xc1\xa7\xae\xed\xc0\x8c\x8c\xd0"
"\xc5\x69\xcf\x74\xbd\xa8\xb4\x81\xff\xa0\x72\x2e");
        ThiefHere = 1;
        return 1;
      }
    }
    else if (here && VillainAttacking[VILLAIN_THIEF] && ThiefWinning() == 0)
    {
      PrintCompLine("\xdc\xd8\xae\x70\x70\xca\xd4\x74\xb5\xe8\xd1\x72\x6d\xa7\x84\x64\xb2"
"\x63\xa9\xf0\xca\x89\xef\x80\xb0\x65\x74\xd1\xb6\x70\xbb\xa6\xdd\x20\x76\xe2\xd3\xb5\xe8"
"\x63\x69\xe8\xa1\xbd\x9f\xac\x6d\xa7\xaf\x9e\xa2\x9a\xf5\x74\x74\xcf\xb3\xca\x74\xa9\xd1"
"\x6d\x70\x73\xa4\x57\xc7\xde\xd0\x72\x75\x65\x66\x75\xea\xe3\xab\xdd\xc0\x9a\xa0\x61\x64"
"\xb5\x94\xc5\x65\x70\xa1\x62\x61\x63\x6b\x77\xbb\xab\xa7\xbd\x80\xe6\xd9\xe1\x8d\xcc\xb2\x61\x70\xfc\xbb\x73\x2e");
      Obj[O_THIEF].prop |= PROP_NODESC;
      VillainAttacking[VILLAIN_THIEF] = 0;
      ThiefRecoverStiletto();
      return 1;
    }
    else if (here && VillainAttacking[VILLAIN_THIEF] && PercentChance(90, -1))
      return 0;
    else if (here && PercentChance(30, -1))
    {
      PrintCompLine("\x85\x68\x6f\x6c\xe8\xb6\xdd\x80\xcb\xbb\x67\x9e\x62\x61\xc1\x6a\xfe"
"\xa6\xcf\x66\x74\xb5\xd9\x6f\x6b\x84\x64\xb2\x67\xfe\xd1\x64\xa4\x46\xd3\x74\xf6\xaf\x65"
"\xec\xb5\x94\xbd\x6f\x6b\xe4\xff\xce\x6e\x67\x2e");
      Obj[O_THIEF].prop |= PROP_NODESC;
      ThiefRecoverStiletto();
      return 1;
    }
    else if (PercentChance(70, -1))
      return 0;
    else if (YouAreDead == 0)
    {
      prev_darkness = IsPlayerInDarkness();

           if (ThiefRob(Obj[O_YOU].loc, 100))  robbed = 2; /* room*/
      else if (ThiefRob(INSIDE + O_YOU,  -1))  robbed = 1; /* player*/

      ThiefHere = 1;

      if (robbed && here == 0)
      {
        PrintCompText("\x41\xaa\xf3\x64\x79\x2d\xd9\x6f\x6b\x84\xa7\x64\x69\x76\x69\x64\x75\xe2"
"\xb7\xc7\xde\xd0\xfd\x72\x67\x9e\x62\x61\xc1\x6a\xfe\xa6\x77\xad\xe8\xa9\xab\xa2\xc2\x75\x67"
"\xde\x81\xc2\xe1\xa4\x4f\xb4\x81\x77\x61\xc4\xa2\xc2\x75\x67\x68\xb5\x94\x71\x75\x69\x65\x74"
"\xec\xa3\x62\xc5\xf4\x63\xd1\xab\x73\xe1\x9e\x76\xe2\x75\x61\x62\xcf\xa1\x66\xc2\x6d\x20");

        if (robbed == 2)
          PrintCompText("\x81\xc2\x6f\x6d");
        else
          PrintCompText("\x92\xeb\x6f\x73\xd6\x73\x73\x69\x6f\x6e");

        PrintCompLine("\xb5\x6d\x75\x6d\x62\xf5\x9c\x73\xe1\x65\xa2\x84\x61\x62\xa5\xa6\x22"
"\x44\x6f\x84\xf6\xbd\xae\x96\x72\xa1\xef\x66\xd3\x65\x2e\x2e\x2e\x22");

        if (IsPlayerInDarkness() != prev_darkness)
          PrintCompLine("\x85\xa2\x69\x65\xd2\xd6\x65\x6d\xa1\xbd\xc0\x61\xd7\xcb\x65\x66"
"\xa6\x8f\xa7\x80\xcc\xbb\x6b\x2e");
      }
      else if (here)
      {
        ThiefRecoverStiletto();

        if (robbed)
        {
          PrintCompText("\x85\xa2\x69\x65\xd2\x6a\xfe\xa6\xcf\x66\x74\xb5\xc5\x69\xdf\xb3\xbb"
"\x72\x79\x84\xce\xa1\xfd\x72\x67\x9e\x62\x61\x67\x8e\xc3\x6d\x61\xc4\xe3\xa6\xcd\xd7\xe4\xff"
"\x69\x63\xd5\x95\xaf\xc0\x65\x20");

          if (robbed == 2)
            PrintCompLine("\x61\x70\x70\xc2\x70\xf1\xaf\xd5\x80\x20\x76\xe2\x75\x61\x62\xcf\xa1"
"\xa7\x80\xda\xe9\x6d\x2e");
          else
            PrintCompLine("\xc2\x62\xef\xab\x8f\x62\xf5\xb9\xc6\x69\x72\x73\x74\x2e");

          if (IsPlayerInDarkness() != prev_darkness)
            PrintCompLine("\x85\xa2\x69\x65\xd2\xd6\x65\x6d\xa1\xbd\xc0\x61\xd7\xcb\x65\x66\xa6"
"\x8f\xa7\x80\xcc\xbb\x6b\x2e");
        }
        else
          PrintCompLine("\x85\xa2\x69\x65\x66\xb5\x66\xa7\x64\x84\xe3\xa2\x84\xdd\x20\x76\xe2\x75"
"\x65\xb5\xcf\x66\xa6\x64\xb2\x67\xfe\xd1\x64\x2e");

        Obj[O_THIEF].prop |= PROP_NODESC;
        here = 0;
        return 1;
      }
      else
      {
        PrintCompLine("\x41\x20\x22\xcf\xad\x8d\xc0\xf6\x67\x72\x79\x22\xe6\xd4\x74\xcf\x6d\xad"
"\x20\x6a\xfe\xa6\x77\xad\xe8\xa9\xab\xa2\xc2\x75\x67\x68\xb5\xe7\x72\x72\x79\x84\xd0\xfd\x72"
"\x67\x9e\x62\x61\x67\xa4\x46\xa7\x64\x84\xe3\xa2\x84\xdd\x20\x76\xe2\x75\x65\xb5\x94\xcf\x66"
"\xa6\x64\xb2\x67\x72\xf6\x74\xcf\x64\x2e");
        return 1;
      }
    }
  }
  else
  {
    if (here)
    {
      if (PercentChance(30, -1))
      {
        prev_darkness = IsPlayerInDarkness();

             if (ThiefRob(Obj[O_YOU].loc, 100))  robbed = 2; /* room*/
        else if (ThiefRob(INSIDE + O_YOU,  -1))  robbed = 1; /* player*/

        if (robbed)
        {
          PrintCompText("\x85\xa2\x69\x65\xd2\x6a\xfe\xa6\xcf\x66\x74\xb5\xc5\x69\xdf\xb3\xbb"
"\x72\x79\x84\xce\xa1\xfd\x72\x67\x9e\x62\x61\x67\x8e\xc3\x6d\x61\xc4\xe3\xa6\xcd\xd7\xe4\xff"
"\x69\x63\xd5\x95\xaf\xc0\x65\x20");

          if (robbed == 2)
            PrintCompLine("\x61\x70\x70\xc2\x70\xf1\xaf\xd5\x80\x20\x76\xe2\x75\x61\x62\xcf\xa1"
"\xa7\x80\xda\xe9\x6d\x2e");
          else
            PrintCompLine("\xc2\x62\xef\xab\x8f\x62\xf5\xb9\xc6\x69\x72\x73\x74\x2e");

          if (IsPlayerInDarkness() != prev_darkness)
            PrintCompLine("\x85\xa2\x69\x65\xd2\xd6\x65\x6d\xa1\xbd\xc0\x61\xd7\xcb\x65\x66"
"\xa6\x8f\xa7\x80\xcc\xbb\x6b\x2e");
        }
        else
          PrintCompLine("\x85\xa2\x69\x65\x66\xb5\x66\xa7\x64\x84\xe3\xa2\x84\xdd\x20\x76\xe2"
"\x75\x65\xb5\xcf\x66\xa6\x64\xb2\x67\xfe\xd1\x64\x2e");

        Obj[O_THIEF].prop |= PROP_NODESC;
        here = 0;
        ThiefRecoverStiletto();
      }
    }
  }

  return 0;
}

void ThiefRoutine(void)
{
  int room, here, once = 0;


  /* if thief is dead or unconcious*/
  if (Obj[O_THIEF].loc == 0 ||
      ThiefDescType == 1) /* unconcious*/
    return;

  for (;;) /* used only to allow use of break instead of goto*/
  {
    room = Obj[O_THIEF].loc;
    here = ((Obj[O_THIEF].prop & PROP_NODESC) == 0);

    if (here)
      room = Obj[O_THIEF].loc;

    if (room == R_TREASURE_ROOM &&
        room != Obj[O_YOU].loc)
    {
      if (here)
      {
        here = 0;
        ThiefHackTreasures();
      }
      ThiefDepositBooty(R_TREASURE_ROOM);
    }
    else if (Obj[O_YOU].loc == room &&
             (Room[room].prop & R_LIT) == 0 &&
             Obj[O_TROLL].loc != Obj[O_YOU].loc)
    {
      if (ThiefVsAdventurer(here))
        break; /* break out of for(;;)*/

      if (Obj[O_THIEF].prop & PROP_NODESC)
        here = 0;
    }
    else
    {
      if (Obj[O_THIEF].loc == room &&
          (Obj[O_THIEF].prop & PROP_NODESC) == 0)
      {
        Obj[O_THIEF].prop |= PROP_NODESC;
        here = 0;
      }

      if (Room[room].prop & R_DESCRIBED)
      {
        ThiefRob(room, 75);

        if ((Room[room].prop & R_MAZE) &&
            (Room[Obj[O_YOU].loc].prop & R_MAZE))
          ThiefRobMaze(room);
        else
          ThiefStealJunk(room);
      }
    }

    once = 1-once;
    if (once && here == 0)
    {
      ThiefRecoverStiletto();

      for (;;)
      {
        room++;
        if (room == NUM_ROOMS) room = 1;

        if ((Room[room].prop & R_SACRED) == 0 &&
            (Room[room].prop & R_BODYOFWATER) == 0)
        {
          Obj[O_THIEF].loc = room;
          Obj[O_THIEF].prop |= PROP_NODESC;
          VillainAttacking[VILLAIN_THIEF] = 0;
          ThiefHere = 0;
          break;
        }
      }
    }

    break; /* break out of for(;;)*/
  }

  if (room != R_TREASURE_ROOM)
    ThiefDropJunk(room);
}
