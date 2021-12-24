#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* malloc free */
#include <ctype.h>

/* defines, enums ------------------------------------------------------------- */
#define MAX_PRINTED_LINES  23
#define LineWidth 79
#define BL0 (256-23)
#define NUM_OBJECTS  80
#define NUM_ROOMS  111 /* including null room 0 */
#define INSIDE  2048
#define O_YOU 1
#define R_LIT           4  /* set when there is natural light or a light fixture */

enum
{
  VILLAIN_TROLL,
  VILLAIN_THIEF,
  VILLAIN_CYCLOPS,

  NUM_VILLAINS
};

enum
{
  V_BRIEF,
  V_VERBOSE,
  V_SUPERBRIEF
};

#define  O_SWORD 61
#define  O_KNIFE 33
#define  O_CYCLOPS 2  
#define  O_THIEF 5
#define  O_TROLL 6  

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

#define StringsBuf	0x7D80
#define AllocBuf	0xBB00

/* extern data structures ---------------------------------------------------------- */

/* tables.c */
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

extern char *SceneryNouns;

struct VERBTOACTION_STRUCT
{
   char *phrase;
   unsigned short action;
};
extern struct VERBTOACTION_STRUCT VerbToAction[];

struct NOUNPHRASETOOBJ_STRUCT
{
   char *phrase;
   unsigned short obj;
};
extern struct NOUNPHRASETOOBJ_STRUCT NounPhraseToObj[];

struct NOUNPHRASETOFIXEDOBJ_STRUCT
{
   char *phrase;
   unsigned short room;
   unsigned short fobj;
};
extern struct NOUNPHRASETOFIXEDOBJ_STRUCT NounPhraseToFixedObj[];

/* data.c */
extern char *BlockMsg[];

extern char *StrAnd;   /*point to this string when comma is encountered in input*/
extern char *StrThen;  /*point to this string when period is encountered in input*/

/* ----------------------- extern functions -----------------*/
int xrnd(void);
int DecompressText(char *text_in, int size_in, char *text_out);

/* ----------------------- variables ----------------------- */

/* parser.c */
/* FILE *InputStream; */ /* input comes only from console! */

int NumPrintedLines;
int CursorColumn;
int NumStrWords;
int CurWord;

char *StrWord[80]; /*pointers to words in str; writing to data pointed-to here is prohibited*/

int Verbosity = V_BRIEF;

int ItObj, PrevItObj; /*the obj "it" refers to*/

unsigned char TimePassed; /*flag: time passed during action*/
unsigned char GameOver; /*flag, but with special value 2: restart was requested*/

/* game.c*/
/* 1-bit flags*/

/* save-restore area ----------------------------------------*/
unsigned char RugMoved;
unsigned char TrapOpen;
unsigned char ExitFound; /* set this when player finds an exit from dungeon other than the trapdoor*/
unsigned char KitchenWindowOpen;
unsigned char GratingRevealed;
unsigned char GratingUnlocked;
unsigned char GratingOpen;
unsigned char GatesOpen;
unsigned char LowTide;
unsigned char GatesButton;
unsigned char LoudRoomQuiet;
unsigned char RainbowSolid;
unsigned char WonGame;
unsigned char MirrorBroken; /* set NotLucky too*/
unsigned char RopeTiedToRail;
unsigned char SpiritsBanished;
unsigned char TrollAllowsPassage;
unsigned char YouAreSanta;
unsigned char YouAreInBoat;
unsigned char NotLucky;
unsigned char YouAreDead;
unsigned char SongbirdSang;
unsigned char ThiefHere;
unsigned char ThiefEngrossed;
unsigned char YouAreStaggered;
unsigned char BuoyFlag;

int NumMoves;
int LampTurnsLeft;
int MatchTurnsLeft;
int CandleTurnsLeft;
int MatchesLeft;
int ReservoirFillCountdown;
int ReservoirDrainCountdown;
int MaintenanceWaterLevel;
int DownstreamCounter;
int BellRungCountdown; /* these three are for ceremony*/
int CandlesLitCountdown;
int BellHotCountdown;
int CaveHoleDepth;
int Score;
int NumDeaths;
int CyclopsCounter;
int CyclopsState; /* 0: default  1: hungry  2: thirsty  3: asleep  4: fled*/
int LoadAllowed;
int PlayerStrength;
int TrollDescType;
int ThiefDescType; /* 0: default  1: unconscious*/
int EnableCureRoutine; /* countdown*/

unsigned char VillainAttacking[NUM_VILLAINS];
unsigned char VillainStaggered[NUM_VILLAINS];
int VillainWakingChance[NUM_VILLAINS];
int VillainStrength[NUM_VILLAINS];

int dummy; /* from RugMoved, total size = 380H */
/* save-restore area ----------------------------------------*/

/* ----------------------- code ----------------------- */
void PrintNewLine(void)
{
  CursorColumn = 0;
  NumPrintedLines++;

  if (NumPrintedLines == MAX_PRINTED_LINES)
  {
    NumPrintedLines = 0;

    /* if (InputStream == stdin) */
      getchar();
    /* else putchar('\n'); */
  }
  else putchar('\n');
}

/*does word wrapping; recognizes newline char*/
/*print terminated by '^' or nullchar*/
void PrintText(char *p)
{
  int width;
  char *q;

  for (;;) /*outer loop*/
  {
    for (;;) /*inner loop*/
    {
      q = p;
      width = LineWidth - CursorColumn; /*try to print rest of current line*/
      while (width > 0 && *q != 0 && *q != '^' && *q != '\n') /*find actual width we can print*/
      {
        width--;
        q++;
      }
      if (width == 0) break; /*we can print all so break into outer loop*/

      while (*p != 0 && *p != '^' && *p != '\n') /*print up until end of string or newline*/
      {
        putchar(*p++);
        CursorColumn++;
      }
      if (*p == 0 || *p == '^') return; /*end of string; done*/
      PrintNewLine(); /*it was a newline*/
      p++; /*skip newline char and repeat inner loop*/
    }

    while (q > p && *q != ' ') q--; /*try to find a space char to break line*/
    width = q - p;
    if (width < 1) width = LineWidth - CursorColumn; /*didn't find a space so break it ourselves*/
    while (width > 0) /*print number of characters we calculated*/
    {
      width--;
      putchar(*p++);
    }
    PrintNewLine(); /*go to next line*/
    while (*p == ' ') p++; /*skip any spaces and repeat outer loop*/
  }
}

/*prints a newline automatically after text*/
void PrintLine(char *p)
{
  PrintText(p);
  PrintNewLine();
}

void PrintCompText(char *comp_text)
{
  char *uncomp_text;
  int comp_size;

  comp_size = strlen(comp_text) + 1;
  uncomp_text = (char*)AllocBuf; /* malloc(GetDecompressTextSize(comp_text, comp_size)); */
  DecompressText(comp_text, comp_size, uncomp_text);

  PrintText(uncomp_text);

/*  free(uncomp_text); */
}

/*prints a newline automatically after text*/
void PrintCompLine(char *p)
{
  PrintCompText(p);
  PrintNewLine();
}

void GetWords(char *prompt)
{
  char *p;
  char *str=(char*)StringsBuf; /* char str[80]; */

  NumStrWords = 0;
  CurWord = 0;

  memset(str, 80, 0); /* HiTech bug !!! */

  PrintText(prompt);

  CursorColumn = 0; /* this is due to fgets below*/
  NumPrintedLines = 0;

  /*get input line from stdin or file*/ /* input ONLY from console! */
  /* -----------------------------------------------------------------------
  fgets(str, 80, InputStream);
  if (InputStream != stdin && (feof(InputStream) || ferror(InputStream)))
  {
    fclose(InputStream);
    InputStream = stdin;
    fgets(str, 80, InputStream);
  }
  if (InputStream != stdin) PrintText(str);
  ------------------------------------------------------------------------- */
  gets(str);

  /*convert upper case chars to lower case, replace whitespace chars with null char, 
	replace ! and ? with .*/
  p = str;
  while (p < str+80)
  {
    if (isalpha(*p))
    {
      if (isupper(*p))
        *p = tolower(*p);
    }
    else if (/* isspace(*p) */ *p == ' ') *p = 0;
    else if (*p == '!' || *p == '?') *p = '.';
    p++;
  }

  /*fill array of pointers to words in str, or to string literals if replacing punctuation*/
  p = str;
  for (;;)
  {
    while (*p == 0 && p < str+80) p++;
    if (p == str+80) break;

    if (*p != ',' && *p != '.')
      StrWord[NumStrWords++] = p;

    while (*p != 0 && p < str+80)
    {
      if (*p == ',' || *p == '.')
      {
        StrWord[NumStrWords++] = (*p == ',') ? StrAnd : StrThen;
        *p++ = 0;
        break;
      }
      p++;
    }
    if (p == str+80) break;
  }
}

/*if "match" matches current word in input, move to next word and return 1*/
int MatchCurWord( char *match)
{
  if (NumStrWords >= CurWord+1 && strcmp(StrWord[CurWord], match) == 0)
  {
    CurWord++;
    return 1;
  }
  return 0;
}
