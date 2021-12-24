/* defines, enums ------------------------------------------------------------- */
#define INSIDE  2048

#define CURE_WAIT  30

#define LOAD_MAX  100

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

extern int TrollDescType;
extern unsigned char NotLucky;
extern int EnableCureRoutine; /* countdown*/
extern int PlayerStrength;
extern int LoadAllowed;

/* extern functions ---------------------------------------------------------------- */
void PrintCompLine(char *);
void ReturnOverlay1(short);
void ReturnOverlay0(short);
void ReturnOverlay0_(short);
short xrnd(void);

/* variables ----------------------------------------------------------------------- */
int weapon[5] = {O_STILETTO, O_AXE, O_SWORD, O_KNIFE, O_RUSTY_KNIFE};

/* code ---------------------------------------------------------------------------- */

int GetRandom(int Range)
{
  long tmp;

  tmp = xrnd() * Range;

  return ((int)(tmp >> 16));
}

/* returns 1 if event of x% chance occurred*/
/* second parameter is used instead if it is >=0 and you're not lucky*/
int PercentChance(int x, int x_not_lucky)
{
  if (NotLucky && x_not_lucky >= 0) x = x_not_lucky;

  if (GetRandom(100) < x) return 1;
  else return 0;
}

int VillainBusy(int i)
{
  if (i == VILLAIN_TROLL)
  {
    if (Obj[O_AXE].loc == INSIDE + O_TROLL)
    {
    }
    else if (Obj[O_AXE].loc == Obj[O_YOU].loc && PercentChance(75, 90))
    {
      Obj[O_AXE].loc = INSIDE + O_TROLL;
      Obj[O_AXE].prop |= PROP_NODESC;
      Obj[O_AXE].prop |= PROP_NOTTAKEABLE;
      Obj[O_AXE].prop &= ~PROP_WEAPON;

      TrollDescType = 0; /* default*/

      if (Obj[O_TROLL].loc == Obj[O_YOU].loc)
        PrintCompLine("\x85\x74\xc2\xdf\xb5\xad\x67\xac\xd5\x8d\xc0\x75\x6d\x69\xf5"
"\xaf\xd5\xb5\xa9\x63\x6f\xd7\x72\xa1\xce\xa1\x77\xbf\x70\xca\xa4\x48\x9e\x61\x70"
"\xfc\xbb\xa1\xbd\xc0\x61\xd7\xa3\xb4\x61\x78\x9e\xbd\xe6\xf1\xb9\xb7\xc7\xde\xc9\x75\x2e");
      /* return 1 */ ReturnOverlay1(1);
    }
    else if (Obj[O_TROLL].loc == Obj[O_YOU].loc)
    {
      TrollDescType = 2; /* unarmed*/
      PrintCompLine("\x85\x74\xc2\xdf\xb5\x64\xb2\xbb\x6d\xd5\xb5\x63\xf2\xac\xa1"
"\xa7\x9f\xac\xc2\x72\xb5\x70\xcf\x61\x64\x84\x66\xd3\xc0\x9a\xf5\x66\x9e\xa7\x80"
"\xe6\xf7\x74\xd8\xe2\x9f\xca\x67\x75\x9e\xdd\x80\x9f\xc2\xdf\x73\x2e");
      /* return 1 */ ReturnOverlay1(1);
    }
  }
  else if (i == VILLAIN_THIEF)
  {
    if (Obj[O_STILETTO].loc == INSIDE + O_THIEF)
    {
    }
    else if (Obj[O_STILETTO].loc == Obj[O_THIEF].loc)
    {
      Obj[O_STILETTO].loc = INSIDE + O_THIEF;
      Obj[O_STILETTO].prop |= PROP_NODESC;
      Obj[O_STILETTO].prop |= PROP_NOTTAKEABLE;
      if (Obj[O_THIEF].loc == Obj[O_YOU].loc)
        PrintCompLine("\x85\xc2\x62\xef\x72\xb5\x73\xe1\x65\x77\xcd\xa6\x73\xd8\x70"
"\xf1\xd6\xab\xaf\x95\x9a\x74\xd8\xb4\xdd\xfb\xd7\xe5\x73\xb5\x6e\x69\x6d\x62\xec"
"\xda\x65\x74\xf1\x65\xd7\xa1\xce\xa1\xc5\x69\xcf\x74\x74\x6f\x2e");
      /* return 1 */ ReturnOverlay1(1);
    }
  }

 /* return 0 */ ReturnOverlay1(0);
}

void CureRoutine(void)
{
  if (EnableCureRoutine == 0) ReturnOverlay0_(0); /* return; */
  EnableCureRoutine--;
  if (EnableCureRoutine != 0) ReturnOverlay0_(0); /* return; */

       if (PlayerStrength > 0) PlayerStrength = 0;
  else if (PlayerStrength < 0) PlayerStrength++;

  if (PlayerStrength < 0)
  {
    if (LoadAllowed < LOAD_MAX)
      LoadAllowed += 10;
    EnableCureRoutine = CURE_WAIT;
  }
  else
  {
    LoadAllowed = LOAD_MAX;
    EnableCureRoutine = 0;
  }

  ReturnOverlay0_(0);
}

/* obj is player or villain obj*/
int FindWeapon(int obj)
{
  int i;

  for (i=0; i<5; i++)
    if (Obj[weapon[i]].loc == INSIDE + obj) ReturnOverlay1(weapon[i]); /* return weapon[i]; */

  /* return 0; */
  ReturnOverlay1(0);
}

