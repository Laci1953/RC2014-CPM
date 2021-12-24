#include <ovr47.h>

/* defines, enums ------------------------------------------------------------- */
#define OVR_VILLAINS_BLOW		47

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


#define HERO  0  /* never set*/

#define NUM_ROOMS  111 /* including null room 0 */

#define R_TREASURE_ROOM 44

#define NUM_OBJECTS  80

#define CURE_WAIT  30

#define LOAD_MAX  100

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

#define  R_TROLL_ROOM	18

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
int GetRandom(int);

int PlayerFightStrength(int);
int VillainFightStrength(int , int );
int ThiefDepositBooty(int );

void ReturnOverlay0(short ret_val);
void ReturnOverlay0_(short ret_val);
short CallOverlay1(void* Routine, short Par1, char CalledRamBank);

/* variables ------------------------------------------------------------------------ */

char BlowForDefense1[13] =
{
  BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_STAGGER, BLOW_STAGGER,
  BLOW_UNCONSCIOUS, BLOW_UNCONSCIOUS, BLOW_KILLED, BLOW_KILLED, BLOW_KILLED,
  BLOW_KILLED, BLOW_KILLED
};

char BlowForDefense2[22] =
{
  BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_STAGGER,
  BLOW_STAGGER, BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED,
  BLOW_MISSED, BLOW_STAGGER, BLOW_STAGGER, BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND,
  BLOW_LIGHT_WOUND, BLOW_UNCONSCIOUS, BLOW_KILLED, BLOW_KILLED, BLOW_KILLED
};

char BlowForDefense3[31] =
{
  BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_MISSED, BLOW_STAGGER, BLOW_STAGGER,
  BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND, BLOW_SERIOUS_WOUND, BLOW_SERIOUS_WOUND, BLOW_MISSED,
  BLOW_MISSED, BLOW_MISSED, BLOW_STAGGER, BLOW_STAGGER, BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND,
  BLOW_LIGHT_WOUND, BLOW_SERIOUS_WOUND, BLOW_SERIOUS_WOUND, BLOW_SERIOUS_WOUND, BLOW_MISSED,
  BLOW_STAGGER, BLOW_STAGGER, BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND, BLOW_LIGHT_WOUND,
  BLOW_SERIOUS_WOUND, BLOW_SERIOUS_WOUND, BLOW_SERIOUS_WOUND
};

/* overlays ----------------------------------------------------------------------- */
/* ------
int VillainBusy(int i);
void CureRoutine(void);
int FindWeapon(int obj);

#define VillainBusy 0
#define CureRoutine 0
#define FindWeapon 0
--------- */
/* code ---------------------------------------------------------------------------- */

void PrintWeaponName(int weapon)
{
  switch (weapon)
  {
    case O_STILETTO:    PrintCompText("\xc5\x69\xcf\x74\x74\x6f");    break;
    case O_AXE:         PrintCompText("\x62\xd9\x6f\x64\xc4\x61\x78\x65");  break;
    case O_SWORD:       PrintCompText("\x73\x77\x6f\x72\x64");       break;
    case O_KNIFE:       PrintCompText("\x6e\xe0\x74\xc4\x6b\x6e\x69\x66\x65"); break;
    case O_RUSTY_KNIFE: PrintCompText("\x72\xfe\x74\xc4\x6b\x6e\x69\x66\x65"); break;
  }
}

int GetBlow(int attack, int defense)
{
  int blow = 0;

  if (defense == 1)
  {
    int j = attack - 1, offset[3]; /* = {0, 2, 4}; */
    offset[0]=0; offset[1]=2; offset[2]=4;

    if (j < 0) j = 0; else if (j > 2) j = 2;
    blow = BlowForDefense1[offset[j] + GetRandom(9)];
  }
  else if (defense == 2)
  {
    int j = attack - 1, offset[4]; /* = {0, 9, 11, 13}; */
    offset[0]=0;offset[1]=9;offset[2]=11;offset[3]=13;

    if (j < 0) j = 0; else if (j > 3) j = 3;
    blow = BlowForDefense2[offset[j] + GetRandom(9)];
  }
  else if (defense > 2)
  {
    int j = attack - defense + 2, offset[5]; /* = {0, 2, 11, 13, 22}; */
    offset[0]=0;offset[1]=2;offset[2]=11;offset[3]=13;offset[4]=22;

    if (j < 0) j = 0; else if (j > 4) j = 4;
    blow = BlowForDefense3[offset[j] + GetRandom(9)];
  }

  return blow;
}

int PlayerResult(int defense, int blow, int original_defense)
{
  PlayerStrength = (defense == 0) ? -10000 : (defense - original_defense);

  if (defense - original_defense < 0)
    EnableCureRoutine = CURE_WAIT;

  if (PlayerFightStrength(1) <= 0)
  {
    PlayerStrength = 1 - PlayerFightStrength(0);
    PrintCompLine("\x49\xa6\x61\x70\xfc\xbb\xa1\xa2\xaf\x95\xaf\xcb\xe0\xa6\x62\xd9\x77"
"\xb7\xe0\x9f\xe9\xee\x75\xfa\xc6\xd3\x86\xa4\x49\x27\xf9\x61\x66\xf4\x69\xab\x8f\xbb"
"\x9e\xe8\x61\x64\x2e");
    YoureDead(); /* ##### RIP #####*/
    return 0;
  }
  else
    return blow;
}

int VillainBlow(int i, int youre_out)
{
  int attack, defense, original_defense, blow, defense_weapon, next_weapon;

  YouAreStaggered = 0;

  if (VillainStaggered[i])
  {
    VillainStaggered[i] = 0;
    PrintCompText("\x85");
    PrintText(VillainName[i]);
    PrintCompLine("\xaa\xd9\x77\xec\xda\x65\x67\x61\xa7\xa1\xce\xa1\x66\xf3\x74\x2e");
    return 1;
  }

  attack = VillainFightStrength(i, 0); /* don't specify player weapon here*/

  defense = PlayerFightStrength(1);
  if (defense <= 0) return 1;

  original_defense = PlayerFightStrength(0);

  defense_weapon = CallOverlay1((void*)FindWeapon, O_YOU, OVR_VILLAINS_BLOW);      /* FindWeapon(O_YOU); */

  blow = GetBlow(attack, defense);

  if (youre_out)
  {
    if (blow == BLOW_STAGGER)
      blow = BLOW_HESITATE;
    else
      blow = BLOW_SITTING_DUCK;
  }

  if (blow == BLOW_STAGGER && defense_weapon && PercentChance(25, HERO ? 10 : 50))
    blow = BLOW_LOSE_WEAPON;

  PrintBlowRemark(0, i, blow, defense_weapon); /* 0: villain blow*/

  if (blow == BLOW_MISSED || blow == BLOW_HESITATE)
    {}
  else if (blow == BLOW_UNCONSCIOUS)
    {}
  else if (blow == BLOW_KILLED || blow == BLOW_SITTING_DUCK)
    defense = 0;
  else if (blow == BLOW_LIGHT_WOUND)
  {
    defense--; if (defense < 0) defense = 0;
    if (LoadAllowed > 50) LoadAllowed -= 10;
  }
  else if (blow == BLOW_SERIOUS_WOUND)
  {
    defense -= 2; if (defense < 0) defense = 0;
    if (LoadAllowed > 50) LoadAllowed -= 20;
  }
  else if (blow == BLOW_STAGGER)
    YouAreStaggered = 1;
  else
  {
    Obj[defense_weapon].loc = Obj[O_YOU].loc;

    next_weapon = CallOverlay1((void*)FindWeapon, O_YOU, OVR_VILLAINS_BLOW);      /* FindWeapon(O_YOU); */
    if (next_weapon)
    {
      PrintCompText("\x46\xd3\x74\xf6\xaf\x65\xec\xb5\x8f\xc5\x69\xdf\xc0\x61\xd7\x20\x61\x20");
      PrintWeaponName(next_weapon);
      PrintCompLine("\x2e");
    }
  }

  return PlayerResult(defense, blow, original_defense);
}

void VillainDead(int i)
{
  if (i == VILLAIN_TROLL)
  {
    if (Obj[O_AXE].loc == INSIDE + O_TROLL)
    {
      Obj[O_AXE].loc = Obj[O_YOU].loc;
      Obj[O_AXE].prop &= ~PROP_NODESC;
      Obj[O_AXE].prop &= ~PROP_NOTTAKEABLE;
      Obj[O_AXE].prop |= PROP_WEAPON;
    }
    TrollAllowsPassage = 1;
  }
  else if (i == VILLAIN_THIEF)
  {
    int flag;

    Obj[O_STILETTO].loc = Obj[O_YOU].loc;
    Obj[O_STILETTO].prop &= ~PROP_NODESC;
    Obj[O_STILETTO].prop &= ~PROP_NOTTAKEABLE;

    flag = ThiefDepositBooty(Obj[O_YOU].loc);

    if (Obj[O_YOU].loc == R_TREASURE_ROOM)
    {
      int obj;

      for (obj=2; obj<NUM_OBJECTS; obj++)
        if (Obj[obj].loc == R_TREASURE_ROOM &&
            obj != O_CHALICE &&
            obj != O_THIEF)
      {
        Obj[obj].prop &= ~PROP_NODESC;
        Obj[obj].prop &= ~PROP_NOTTAKEABLE;
      }

      Obj[O_CHALICE].prop |= PROP_NODESC;
      PrintPresentObjects(R_TREASURE_ROOM, "As the thief dies, the power of his magic decreases, and his treasures reappear:", 1); /* 1: list, no desc*/
      Obj[O_CHALICE].prop &= ~PROP_NODESC;

      PrintCompLine("\x85\xfa\xe2\x69\x63\x9e\x9a\xe3\x77\xaa\x61\x66\x9e\xbd\x9f\x61\x6b\x65\x2e");
    }
    else if (flag)
      PrintCompLine("\x48\x9a\x62\xe9\x74\xc4\xa9\x6d\x61\xa7\x73\x2e");
  }
}

int VillainStrikeFirst(int i)
{
  if (i == VILLAIN_TROLL)
  {
    if (PercentChance(33, -1))
    {
      VillainAttacking[i] = 1;
      return 1;
    }
  }
  else if (i == VILLAIN_THIEF)
  {
    if (ThiefHere && (Obj[O_THIEF].prop & PROP_NODESC) == 0 && PercentChance(20, -1))
    {
      VillainAttacking[i] = 1;
      return 1;
    }
  }

  return 0;
}

void VillainUnconcious(int i)
{
  if (i == VILLAIN_TROLL)
  {
    VillainAttacking[i] = 0;

    if (Obj[O_AXE].loc == INSIDE + O_TROLL)
    {
      Obj[O_AXE].loc = Obj[O_YOU].loc;
      Obj[O_AXE].prop &= ~PROP_NODESC;
      Obj[O_AXE].prop &= ~PROP_NOTTAKEABLE;
      Obj[O_AXE].prop |= PROP_WEAPON;
    }

    TrollDescType = 1; /* unconcious*/
    TrollAllowsPassage = 1;
  }
  else if (i == VILLAIN_THIEF)
  {
    VillainAttacking[i] = 0;

    Obj[O_STILETTO].loc = Obj[O_YOU].loc;
    Obj[O_STILETTO].prop &= ~PROP_NODESC;
    Obj[O_STILETTO].prop &= ~PROP_NOTTAKEABLE;

    ThiefDescType = 1; /* unconcious*/
  }
}

void VillainConscious(int i)
{
  if (i == VILLAIN_TROLL)
  {
    if (Obj[O_TROLL].loc == Obj[O_YOU].loc)
    {
      VillainAttacking[i] = 1;
      PrintCompLine("\x85\x74\xc2\xdf\xaa\xf0\x72\x73\xb5\x71\x75\x69\x63\x6b\xec"
"\xda\xbe\x75\x6d\x84\xd0\x66\x69\x67\x68\xf0\x9c\xc5\xad\x63\x65\x2e");
    }

    if (Obj[O_AXE].loc == INSIDE + O_TROLL)
      TrollDescType = 0; /* default*/
    else if (Obj[O_AXE].loc == R_TROLL_ROOM)
    {
      Obj[O_AXE].loc = INSIDE + O_TROLL;
      Obj[O_AXE].prop |= PROP_NODESC;
      Obj[O_AXE].prop |= PROP_NOTTAKEABLE;
      Obj[O_AXE].prop &= ~PROP_WEAPON;
      TrollDescType = 0; /* default*/
    }
    else
      TrollDescType = 3; /* simple description*/

    TrollAllowsPassage = 0;
  }
  else if (i == VILLAIN_THIEF)
  {
    if (Obj[O_THIEF].loc == Obj[O_YOU].loc)
    {
      VillainAttacking[i] = 1;
      PrintCompLine("\x85\xc2\x62\xef\xb6\xa9\x76\x69\xd7\x73\xb5\x62\xf1\x65\x66\xec"
"\xc6\x65\x69\x67\x6e\x84\x63\xca\xf0\x6e\x75\xd5\x20\xf6\x63\xca\x73\x63\x69\xa5\x73"
"\xed\x73\x73\xb5\xad\x64\xb5\x77\xa0\xb4\x94\xd6\xbe\xc0\x9a\x6d\xe1\xd4\x74\xb5\x73"
"\x63\xf4\x6d\x62\xcf\xa1\x61\x77\x61\xc4\x66\xc2\xf9\xc9\x75\x2e");
    }

    ThiefDescType = 0; /* default*/
    ThiefRecoverStiletto();
  }
}

void FightRoutine(void)
{
  int i, obj, youre_attacked = 0, youre_out = 0;

  if (YouAreDead)
    return;

  for (i=0; i<NUM_VILLAINS; i++)
  {
    obj = VillainObj[i];

    if (Obj[obj].loc == Obj[O_YOU].loc &&
        (Obj[obj].prop & PROP_NODESC) == 0)
    {
      if (obj == O_THIEF && ThiefEngrossed)
        ThiefEngrossed = 0;
      else if (VillainStrength[i] < 0)
      {
        if (VillainWakingChance[i] != 0 &&
            PercentChance(VillainWakingChance[i], -1))
        {
          VillainWakingChance[i] = 0;
          if (VillainStrength[i] < 0)
          {
            VillainStrength[i] = -VillainStrength[i];
            VillainConscious(i);
          }
        }
        else
          VillainWakingChance[i] += 25;
      }
      else if (VillainAttacking[i] || VillainStrikeFirst(i))
        youre_attacked = 1;
    }
    else
    {
      if (VillainAttacking[i])
	{
        /* VillainBusy(i); */
	CallOverlay1((void*)VillainBusy, i, OVR_VILLAINS_BLOW);
	}
      if (obj == O_THIEF)
        ThiefEngrossed = 0;
      YouAreStaggered = 0;
      VillainStaggered[i] = 0;
      VillainAttacking[i] = 0;
      if (VillainStrength[i] < 0)
      {
        VillainStrength[i] = -VillainStrength[i];
        VillainConscious(i);
      }
    }
  }

  if (youre_attacked)
    for (;;)
  {
    for (i=0; i<NUM_VILLAINS; i++)
    {
      if (VillainAttacking[i] == 0) {}
      else if (CallOverlay1((void*)VillainBusy, i, OVR_VILLAINS_BLOW) /* VillainBusy(i) */) {}
      else
      {
        int blow = VillainBlow(i, youre_out);

        if (blow == 0) return;
        else if (blow == BLOW_UNCONSCIOUS)
          youre_out = 1 + 1 + GetRandom(3);
      }
    }

    if (youre_out) youre_out--;
    if (youre_out == 0) break;
  }
}

void VillainsRoutine(void)
{
  ThiefRoutine();
  FightRoutine();

  CallOverlay0((void*)CureRoutine, OVR_VILLAINS_BLOW);
  /* CureRoutine(); */

  ReturnOverlay0_(0);
}

void VillainResult(int i, int defense, int blow)
{
  VillainStrength[i] = defense;

  if (defense == 0)
  {
    PrintCompText("\x41\x6c\x6d\x6f\xc5\xa3\xa1\x73\xe9\xb4\xe0\x80\x20");
    PrintText(VillainName[i]);
    PrintCompLine("\xb0\xa9\xaf\xa0\xa1\xce\xa1\xfd\xc5\xb0\xa9\xaf\x68\xb5\xd0\x63"
"\xd9\x75\xab\xdd\xaa\xa7\xb2\xd1\xb6\x62\xfd\x63\x6b\xc6\x6f\xc1\xd4\xd7\xd9\x70"
"\xa1\xce\x6d\xb5\x8c\x77\xa0\xb4\x81\x66\x6f\xc1\xf5\x66\x74\x73\xb5\x81\xe7\x72"
"\xe7\x73\xa1\xcd\xa1\x64\xb2\x61\x70\xfc\xbb\x65\x64\x2e");

    VillainAttacking[i] = 0;
    Obj[VillainObj[i]].loc = 0;

    VillainDead(i);
  }
  else if (blow == BLOW_UNCONSCIOUS)
    VillainUnconcious(i);
}

/* obj is thing being attacked by player*/

void PlayerBlow(int obj, int player_weapon)
{
  int i, attack, defense, defense_weapon, blow;

  for (i=0; i<NUM_VILLAINS; i++)
    if (VillainObj[i] == obj) break;

  if (i < NUM_VILLAINS)
    VillainAttacking[i] = 1;

  if (YouAreStaggered)
  {
    YouAreStaggered = 0;
    PrintCompLine("\x8b\xbb\x9e\xc5\x69\xdf\xda\x65\x63\x6f\xd7\xf1\x9c\x66\xc2\xf9"
"\xa2\xaf\xcb\xe0\xa6\x62\xd9\x77\xb5\x73\xba\x92\xa3\x74\x74\x61\x63\x6b\x87\xa7"
"\x65\x66\x66\x65\x63\xf0\x76\x65\x2e");
    return;
  }

  if (obj == O_YOU)
  {
    PrintCompLine("\x57\x65\xdf\xb5\x8f\xa9\xe2\xec\xcc\x69\xab\xc7\x95\xaf\x9f\x69"
"\x6d\x65\xa4\x49\xa1\x73\x75\x69\x63\x69\xe8\xeb\x61\xa7\xcf\x73\x73\x3f");
    YoureDead(); /* ##### RIP #####*/
    return;
  }

  attack = PlayerFightStrength(1);
  if (attack < 1) attack = 1;

  if (i < NUM_VILLAINS)
    defense = VillainFightStrength(i, player_weapon);
  else
    defense = 0;

  if (defense == 0) /* catches case of i == NUM_VILLAINS*/
  {
    PrintCompLine("\x41\x74\x74\x61\x63\x6b\x84\xa2\xaf\x87\x70\x6f\xa7\x74\xcf\x73\x73\x2e");
    return;
  }

  defense_weapon = CallOverlay1((void*)FindWeapon, obj, OVR_VILLAINS_BLOW);  /* FindWeapon(obj); */

  if ((defense_weapon == 0 && obj != O_CYCLOPS) || defense < 0)
  {
    PrintCompText("\x85");
    if (defense < 0) PrintCompText("\xf6\x63\xca\x73\x63\x69\xa5\x73\x20");
    else             PrintCompText("\xf6\xbb\x6d\x65\x64\x20");
    PrintText(VillainName[i]);
    PrintCompLine("\x91\xe3\xa6\xe8\x66\xd4\xab\xce\x6d\xd6\x6c\x66\x3a\x20\x48\x9e\x64\x69\x65\x73\x2e");
    blow = BLOW_KILLED;
  }
  else
  {
    blow = GetBlow(attack, defense);

    if (blow == BLOW_STAGGER && defense_weapon && PercentChance(25, -1))
      blow = BLOW_LOSE_WEAPON;

    PrintBlowRemark(1, i, blow, player_weapon); /* 1: player blow*/
  }

  if (blow == BLOW_MISSED || blow == BLOW_HESITATE)
  {
  }
  else if (blow == BLOW_UNCONSCIOUS)
    defense = -defense;
  else if (blow == BLOW_KILLED || blow == BLOW_SITTING_DUCK)
    defense = 0;
  else if (blow == BLOW_LIGHT_WOUND)
  {
    defense--;
    if (defense < 0) defense = 0;
  }
  else if (blow == BLOW_SERIOUS_WOUND)
  {
    defense -= 2;
    if (defense < 0) defense = 0;
  }
  else if (blow == BLOW_STAGGER)
    VillainStaggered[i] = 1;
  else
  {
    Obj[defense_weapon].loc = Obj[O_YOU].loc;
    Obj[defense_weapon].prop &= ~PROP_NODESC;
    Obj[defense_weapon].prop &= ~PROP_NOTTAKEABLE;
    Obj[defense_weapon].prop |= PROP_WEAPON;
  }

  VillainResult(i, defense, blow);
}

/* call just before player enters treasure room*/

void ThiefProtectsTreasure(void)
{
  int obj, flag = 0;

  /* if thief is dead or unconcious*/
  if (Obj[O_THIEF].loc == 0 ||
      ThiefDescType == 1) /* unconcious*/
    return;

  if (Obj[O_THIEF].loc != R_TREASURE_ROOM)
  {
    PrintCompLine("\x8b\xa0\xbb\xa3\xaa\x63\xa9\x61\xf9\xdd\xa3\xb1\x75\xb2\xde\xe0"
"\x86\x20\x76\x69\x6f\xfd\xd1\x80\xda\x6f\x62\xef\x72\x27\xa1\xce\xe8\x61\x77\x61"
"\x79\xa4\x55\x73\x84\x70\xe0\x73\x61\x67\xbe\x20\xf6\x6b\xe3\x77\xb4\xbd\x86\xb5"
"\x94\x72\xfe\xa0\xa1\xbd\xa8\x74\xa1\xe8\x66\xd4\x73\x65\x2e");
  
    Obj[O_THIEF].loc = R_TREASURE_ROOM;
    Obj[O_THIEF].prop &= ~PROP_NODESC;
  
    VillainAttacking[VILLAIN_THIEF] = 1;
  
    for (obj=2; obj<NUM_OBJECTS; obj++)
      if (Obj[obj].loc == R_TREASURE_ROOM &&
          obj != O_CHALICE &&
          obj != O_THIEF)
    {
      if (flag == 0)
      {
        flag = 1;
        PrintCompLine("\x85\xa2\x69\x65\xd2\x67\xbe\x74\xd8\xbe\xee\x79\xc5\xac\x69"
"\xa5\x73\xec\xb5\x8c\x81\x74\xa9\xe0\xd8\xbe\xa8\xb4\x81\xc2\xe1\xaa\x75\x64\xe8"
"\x6e\xec\x20\x76\xad\xb2\x68\x2e");
      }
  
      Obj[obj].prop |= PROP_NODESC;
      Obj[obj].prop |= PROP_NOTTAKEABLE;
    }

    PrintCompText("\x0a");
  }
}

