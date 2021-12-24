#include <stdio.h>
#include <ovr40.h>
#include <ovr42.h>
#include <ovr44.h>
#include <ovr45.h>
#include <ovr52.h>

/* defines, enums ------------------------------------------------------------- */

#define OVR_SAVE_RESTORE		40
#define OVR_DO_MISC			41
#define OVR_INTERCEPT			42
#define OVR_GO_FROM			43
#define OVR_DO_MISC_WITH_TO		44
#define OVR_DO_PARSE_ACTION		45
#define OVR_DO_MISC_DO			52

enum
{
  V_BRIEF,
  V_VERBOSE,
  V_SUPERBRIEF
};

#define SAVE_FILENAME "zork.sav"
#define SAVE_FILENAME_NUMCHAR_INDEX  6

#define BL0 (256-23)
#define O_YOU 1
#define NUM_ROOMS  111 /* including null room 0 */
#define NUM_OBJECTS  80
#define LOAD_MAX  100
#define MAX_INVENTORY_ITEMS  7
#define INV_LIMIT_CHANCE     8
#define INSIDE  2048

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

#define NUM_TREASURESCORES  19
#define NUM_ROOMSCORES      5

/*actions*/
enum
{
  A_NOTHING,
  A_NORTH, /*direction actions must be grouped together in this order*/
  A_SOUTH,
  A_EAST,
  A_WEST,
  A_NORTHEAST,
  A_NORTHWEST,
  A_SOUTHEAST,
  A_SOUTHWEST,
  A_UP,
  A_DOWN,
  A_IN,
  A_OUT,
  A_ACTIVATE,
  A_ATTACK,
  A_BREAK,
  A_BRIEF,
  A_BRUSH,
  A_CLIMB,
  A_CLIMBDOWN,
  A_CLIMBTHROUGH,
  A_CLIMBUP,
  A_CLOSE,
  A_COUNT,
  A_CROSS,
  A_DEACTIVATE,
  A_DEFLATE,
  A_DIAGNOSE,
  A_DIG,
  A_DISEMBARK,
  A_DISMOUNT,
  A_DRINK,
  A_DROP,
  A_EAT,
  A_ECHO,
  A_EMPTY,
  A_ENTER,
  A_EXAMINE,
  A_EXIT,
  A_EXORCISE,
  A_FILL,
  A_FIX,
  A_GIVE,
  A_GO,
  A_GREET,
  A_INFLATE,
  A_INVENTORY,
  A_JUMP,
  A_KNOCK,
  A_LAND,
  A_LAUNCH,
  A_LISTENTO,
  A_LOCK,
  A_LOOK,
  A_LOOKBEHIND,
  A_LOOKIN,
  A_LOOKON,
  A_LOOKTHROUGH,
  A_LOOKUNDER,
  A_LOWER,
  A_MOUNT,
  A_MOVE,
  A_ODYSSEUS,
  A_OIL,
  A_OPEN,
  A_PLAY,
  A_POUR,
  A_PRAY,
  A_PRY,
  A_PULL,
  A_PUSH,
  A_PUT,
  A_QUIT,
  A_RAISE,
  A_READ,
  A_REMOVE,
  A_RESTART,
  A_RESTORE,
  A_RING,
  A_SAVE,
  A_SAY,
  A_SCORE,
  A_SLEEP,
  A_SLEEPON,
  A_SLIDEDOWN,
  A_SLIDEUP,
  A_SMELL,
  A_SQUEEZE,
  A_SUPERBRIEF,
  A_SWIM,
  A_TAKE,
  A_TALKTO,
  A_THROW,
  A_TIE,
  A_TOUCH,
  A_TURN,
  A_UNLOCK,
  A_UNTIE,
  A_VERBOSE,
  A_VERSION,
  A_WAIT,
  A_WAVE,
  A_WEAR,
  A_WHEREIS,
  A_WIND
};

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

struct VERBTOACTION_STRUCT
{
  char *phrase;
  unsigned short action;
};
extern struct VERBTOACTION_STRUCT VerbToAction[];

struct GOFROM_STRUCT
{
  int room;
  int action; /*go-direction action (but technically could be any action)*/
  int (*f)(void); /*function returns 1 if action completed; otherwise fall through*/
};
extern struct GOFROM_STRUCT GoFrom[];

struct DOMISCWITH_STRUCT
{
  int action;
  int obj; /*can be obj or fobj*/
  void (*f)(int);
};
extern struct DOMISCWITH_STRUCT DoMiscWithTo[];

extern int NumMoves;
extern FILE *InputStream;
extern int ItObj, PrevItObj; /*the obj "it" refers to*/
extern char GameOver; /*flag, but with special value 2: restart was requested*/

/* extern functions ---------------------------------------------------------------- */

/* common */
short CallOverlay0(void* Routine, char CalledRamBank);
short CallOverlay1(void* Routine, short Par1, char CalledRamBank);
void ReturnOverlay0(short ret_val);

void PrintCompLine(char *);
void PrintBlockMsg(int);
int MatchCurWord( char *);

int ValidateObject(int obj);
int GetActionFromInput(void);
void SkipObjFromInput(int );
void PrintObjectDesc(int , int );
void PrintContents(int , char *, int );
void PrintScore(void);
int IsObjVisible(int );
int IsPlayerInDarkness(void);
void PrintPlayerRoomDesc(int );
int GetAllObjFromInput(int );
int GetObjFromInput(void);

/* extern variables ----------------------------------------------------------------------- */

extern int Verbosity;
extern int NumStrWords;
extern int CurWord;
extern char TimePassed; /*flag: time passed during action*/

extern unsigned char RugMoved;

/* variables ----------------------------------------------------------------------- */

/* overlays ----------------------------------------------------------------------- */

/* -------------
int InterceptAction(int );

void DoCommandActor(int obj);
void DoVersion(void);
void DoDiagnose(void);
void DoEcho(void);
void DoPray(void);
void DoOdysseus(void);
void DoJump(void);
void DoSleep(void);
void DoDisembark(void);
void DoLaunch(void);
void DoLand(void);
void DoSwim(void);
void DoTalkTo(void);
void DoGreet(void);
void DoSay(void);

void DoActionOnObject(int );
void ParseActionDirection(int);
void ParseActionRestartOrQuit(int);
void ParseActionTake(void);
void ParseActionDropPut(int );
void ParseActionExamine(void);
void ParseActionGive(void);
void ParseActionWhereIs(void);
void ParseActionThrow(void);

int DoSave(void);
int DoRestore(void);

#define InterceptAction 0

#define DoCommandActor	0
#define DoVersion	0
#define DoDiagnose	0
#define DoEcho		0
#define DoPray		0
#define DoOdysseus	0
#define DoJump		0
#define DoSleep		0
#define DoDisembark	0
#define DoLaunch	0
#define DoLand		0
#define DoSwim		0
#define DoTalkTo	0
#define DoGreet		0
#define DoSay		0

#define DoActionOnObject	0
#define ParseActionDirection	0
#define ParseActionRestartOrQuit 0
#define ParseActionTake		0
#define ParseActionDropPut	0
#define ParseActionExamine	0
#define ParseActionGive		0
#define ParseActionWhereIs	0
#define ParseActionThrow	0

#define DoSave 0
#define DoRestore 0
------------ */
/* code ---------------------------------------------------------------------------- */
/* CAUTION: called function will have to check if with_to >= NUM_OBJECTS*/
void ParseActionWithTo(int action, char *match_hack, char *cant)
{
  int obj, with_to, i;

  obj = GetAllObjFromInput(Obj[O_YOU].loc); if (obj <= 0) return;
  if (ValidateObject(obj)) return;

  if (match_hack != 0) MatchCurWord(match_hack); /*skip specified word*/

  with_to = 0;
  if (MatchCurWord("with") || MatchCurWord("using") || MatchCurWord("to") || 
MatchCurWord("from") || MatchCurWord("on") || MatchCurWord("onto"))
  {
    with_to = GetAllObjFromInput(Obj[O_YOU].loc); if (with_to <= 0) return;
  }
  if (ValidateObject(with_to)) return;

  for (i=0; DoMiscWithTo[i].f != 0; i++)
    if (DoMiscWithTo[i].action == action && DoMiscWithTo[i].obj == obj)
  {
    CallOverlay1((void*)DoMiscWithTo[i].f, with_to, OVR_DO_MISC_WITH_TO);
    /* DoMiscWithTo[i].f(with_to); */
    return;
  }

  if (obj == O_YOU) {PrintCompLine("\x53\xac\x69\xa5\x73\xec\x3f\x21"); return;}

  PrintCompText("\x8b\xe7\x93");
  PrintLine(cant);
}

extern char* StrWord[];

void Parse(void)
{
  int obj, action, i, temp, temp2;

  TimePassed = 0;

  if (CurWord == NumStrWords || MatchCurWord("then")) ReturnOverlay0(0); /* return; */

  if (CurWord > 0 && Verbosity != V_SUPERBRIEF) 
    PrintNewLine(); /*print blank line between turns*/

  /* "actor, command1 (then command2 ...)"*/
  /* match noun phrase instead of verb phrase here to see if player is talking to actor*/
  /* NOTES:*/
  /*   comma is represented by "and"*/
  /*   all following actions in input will be directed at actor*/

  temp = CurWord;
  obj = GetObjFromInput(); /* note that this can return -1 (noun phrase not specific enough)*/

  if (MatchCurWord("and") && obj > 1 && (Obj[obj].prop & PROP_ACTOR))
    {
      CallOverlay1((void*)DoCommandActor, (short)obj, OVR_DO_MISC_DO);
      /* DoCommandActor(obj); */
      ReturnOverlay0(0);
      /*return;*/
    }

  CurWord = temp;

  action = GetActionFromInput();

  if (CallOverlay1((void*)InterceptAction, (short)action, OVR_INTERCEPT) /* InterceptAction(action) */) 
    ReturnOverlay0(0); /* return; */

  if (action == 0)
  {
    PrintCompLine("\x49\xcc\x69\x64\x93\xa9\x63\x6f\x67\x6e\x69\x7a\x9e\xd0\xd7"
"\x72\x62\xa8\xb4\xa2\xaf\xaa\xd4\xd1\x6e\x63\x65\x2e");
    /* return; */
    ReturnOverlay0(0);
  }

  /*hacks to allow "turn obj on/off" "take obj off" "put obj on" to be*/
  /*caught by "activate/deactivate" "remove" and "wear"*/
  /*NOTE: this will allow strange commands 
   like "spin obj on" "remove obj off" "wear obj on"*/

  temp = CurWord; SkipObjFromInput(Obj[O_YOU].loc);

  if (action == A_TURN && MatchCurWord("on"))
    action = A_ACTIVATE; /* "turn obj on"*/

  if (action == A_TURN && MatchCurWord("off"))
    action = A_DEACTIVATE; /* "turn obj off""*/

  if (action == A_TAKE && MatchCurWord("off"))
  {
    MatchCurWord("of");
    /* if no obj is after "off (of)", change action to remove*/
    temp2 = CurWord; SkipObjFromInput(Obj[O_YOU].loc);
    if (temp2 == CurWord) action = A_REMOVE; /* "take obj off"*/
  }

  if (action == A_PUT && MatchCurWord("on"))
  {
    /* if no obj is after "on", change action to wear*/
    temp2 = CurWord; SkipObjFromInput(Obj[O_YOU].loc);
    if (temp2 == CurWord) action = A_WEAR; /* "put obj on"*/
  }

  CurWord = temp;

  /*replace "go dir" with "dir"*/
  if (action == A_GO)
  {
    for (i=0; ; i++)
    {
      if (VerbToAction[i].phrase == 0)
      {
        PrintCompLine("\x49\xb3\xa5\x6c\x64\x93\x66\xa7\xab\xd0\x64\x69\xa9"
"\x63\xf0\xca\x89\x67\xba\xa7\x95\xaf\xaa\xd4\xd1\x6e\x63\x65\x2e");
        /* return; */
        ReturnOverlay0(0);
      }

      action = VerbToAction[i].action;
      if (action >= A_NORTH && action <= A_OUT && MatchCurWord(VerbToAction[i].phrase)) break;
    }
  }

  /*special movements; executed function can fall through if it returns 0*/
  /*A_IN and A_OUT must be handled here*/
  for (i=0; ; i++)
  {
    if (GoFrom[i].f == 0) break;
    if (GoFrom[i].room == Obj[O_YOU].loc && GoFrom[i].action == action)
    {
      SkipObjFromInput(Obj[O_YOU].loc); /* hack; in case player types 
						something like "go in house"*/
      if (CallOverlay0((void*)GoFrom[i].f, OVR_GO_FROM) /* (*(GoFrom[i].f))() */) 
	ReturnOverlay0(0);/*return;*/
    }
  }

  if (action == A_IN || action == A_OUT) /*if these actions are not 
						handled above, print blocked message*/
  {
    PrintBlockMsg(BL0);
    ReturnOverlay0(0);
    /* return; */
  }

  if (action >= A_NORTH && action <= A_DOWN)
  {
    SkipObjFromInput(Obj[O_YOU].loc); /* hack; in case player 
						types something like "go down chute"*/
    CallOverlay1((void*)ParseActionDirection, action, OVR_DO_PARSE_ACTION);
    /* ParseActionDirection(action); */
    ReturnOverlay0(0);
    /* return; */
  }

  switch (action)
  {
    case A_SAVE:     
			CallOverlay0((void*)DoSave, OVR_SAVE_RESTORE);
			/* DoSave(); */
			ReturnOverlay0(0);
			/* return; */
    case A_RESTORE:
			CallOverlay0((void*)DoRestore, OVR_SAVE_RESTORE);
			/* DoRestore(); */  
			ReturnOverlay0(0);
			/* return; */
    case A_SCORE:    	PrintScore(); 
			ReturnOverlay0(0);
			/* return; */
    case A_VERSION:  

		       CallOverlay0((void*)DoVersion, OVR_DO_MISC_DO);
		       /* DoVersion(); */
			ReturnOverlay0(0);
			/* return; */
    case A_DIAGNOSE: 
		       CallOverlay0((void*)DoDiagnose, OVR_DO_MISC_DO);
		       /* DoDiagnose(); */
			ReturnOverlay0(0);
			/* return; */
    case A_RESTART:
    case A_QUIT:
    			CallOverlay1((void*)ParseActionRestartOrQuit, action, OVR_DO_PARSE_ACTION);
      			/* ParseActionRestartOrQuit(action); */
			ReturnOverlay0(0);
			/* return; */
    case A_BRIEF:      Verbosity = V_BRIEF;      
			PrintCompLine("\x4e\xf2\x20\xfe\x84\x62\xf1\x65\xd2\xe8\x73\x63\xf1\x70\xf0\xca\x73\x2e"); 
			ReturnOverlay0(0);
			/* return; */
    case A_VERBOSE:    Verbosity = V_VERBOSE;
    PrintCompLine("\x4e\xf2\x20\xfe\x84\xd7\x72\x62\x6f\xd6\xcc\xbe\x63\xf1\x70\xf0\xca\x73\x2e\x0a"); 
			PrintPlayerRoomDesc(1); 
			ReturnOverlay0(0);
			/* return; */
    case A_SUPERBRIEF: Verbosity = V_SUPERBRIEF; 
			PrintCompLine("\x4e\xf2\x20\xfe\x84\x73\x75\xfc\x72\x62\xf1\x65\xd2\xe8\x73"
"\x63\xf1\x70\xf0\xca\x73\x2e"); 
			ReturnOverlay0(0);
			/* return; */

    case A_ECHO:
		       CallOverlay0((void*)DoEcho, OVR_DO_MISC_DO);
		       /* DoEcho(); */
			ReturnOverlay0(0);
			/* return; */
    case A_PRAY:     
		       CallOverlay0((void*)DoPray, OVR_DO_MISC_DO);
		       /* DoPray(); */
			ReturnOverlay0(0);
			/* return; */
    case A_ODYSSEUS: 
		       CallOverlay0((void*)DoOdysseus, OVR_DO_MISC_DO);
		       /* DoOdysseus(); */
			ReturnOverlay0(0);
			/* return; */
    case A_LOOK: 	PrintPlayerRoomDesc(1);            
			ReturnOverlay0(0);
			/* return; */
    case A_WAIT: 	PrintCompLine("\x5a\x7a\x7a\x2e"); 
			TimePassed = 1; 
			ReturnOverlay0(0);
			/* return; */

    case A_ACTIVATE:   ParseActionWithTo(action, "on", "light that!");   
			ReturnOverlay0(0);
			/* return; */
    case A_DEACTIVATE: ParseActionWithTo(action, "off", "extinguish that!"); 
			ReturnOverlay0(0);
			/* return; */

    /* NOTE: when in darkness, player is still allowed to open/close 
				because light might be inside container*/
    case A_OPEN:
    case A_CLOSE:
		  CallOverlay1((void*)DoActionOnObject, action, OVR_DO_PARSE_ACTION);
      		  /* DoActionOnObject(action); */
		  ReturnOverlay0(0);
		  /* return; */
  }

  /* actions above this line work when in darkness*/
  if (IsPlayerInDarkness()) 
	{
	  PrintCompLine("\x49\xa6\x9a\xbd\xba\x64\xbb\x6b\x89\xd6\x9e\xad\x79\xa2\x97\x2e"); 
	  ReturnOverlay0(0);
	 /* return; */
	}
  /* actions below this line are not possible when in darkness*/

  switch (action)
  {
    case A_INVENTORY: PrintPresentObjects(INSIDE + O_YOU, 0, 0); 
		goto ret; /* return; */

    case A_TAKE:   
		CallOverlay0((void*)ParseActionTake, OVR_DO_PARSE_ACTION);
		/* ParseActionTake(); */  
		goto ret; /* return; */
    case A_DROP:
		CallOverlay1((void*)ParseActionDropPut, 0, OVR_DO_PARSE_ACTION);
		/* ParseActionDropPut(0); */
		goto ret; /* return; */
    case A_PUT:
		CallOverlay1((void*)ParseActionDropPut, 1, OVR_DO_PARSE_ACTION);
		/* ParseActionDropPut(1); */
		goto ret; /* return; */
    case A_EXAMINE: 
		CallOverlay0((void*)ParseActionExamine, OVR_DO_PARSE_ACTION);
		/* ParseActionExamine(); */
		goto ret; /* return; */
    case A_GIVE:    
		CallOverlay0((void*)ParseActionGive, OVR_DO_PARSE_ACTION);
		/* ParseActionGive(); */   
		goto ret; /* return; */
    case A_WHEREIS: 
		CallOverlay0((void*)ParseActionWhereIs, OVR_DO_PARSE_ACTION);
		/* ParseActionWhereIs(); */
		goto ret; /* return; */
    case A_THROW:   
		CallOverlay0((void*)ParseActionThrow, OVR_DO_PARSE_ACTION);
		/* ParseActionThrow(); */    
		goto ret; /* return; */

    case A_DIG:     ParseActionWithTo(action, 0, "dig that!");     
		goto ret; /* return; */
    case A_LOCK:    ParseActionWithTo(action, 0, "lock that!");    
		goto ret; /* return; */
    case A_UNLOCK:  ParseActionWithTo(action, 0, "unlock that!");  
		goto ret; /* return; */
    case A_TURN:    ParseActionWithTo(action, 0, "turn that!");    
		goto ret; /* return; */
    case A_OIL:     ParseActionWithTo(action, 0, "oil that!");     
		goto ret; /* return; */
    case A_TIE:     ParseActionWithTo(action, 0, "tie that!");     
		goto ret; /* return; */
    case A_UNTIE:   ParseActionWithTo(action, 0, "untie that!");   
		goto ret; /* return; */
    case A_FIX:     ParseActionWithTo(action, 0, "fix that!");     
		goto ret; /* return; */
    case A_INFLATE: ParseActionWithTo(action, 0, "inflate that!"); 
		goto ret; /* return; */
    case A_DEFLATE: ParseActionWithTo(action, 0, "deflate that!"); 
		goto ret; /* return; */
    case A_FILL:    ParseActionWithTo(action, 0, "fill that!");    
		goto ret; /* return; */
    case A_ATTACK:  ParseActionWithTo(action, 0, "attack that!");  
		goto ret; /* return; */
    case A_POUR:    ParseActionWithTo(action, 0, "pour that!");    
		goto ret; /* return; */
    case A_BRUSH:   ParseActionWithTo(action, 0, "brush that!");   
		goto ret; /* return; */

    case A_JUMP:    
	       CallOverlay0((void*)DoJump, OVR_DO_MISC_DO);
	       /* DoJump(); */
	       goto ret; /* return; */
    case A_SLEEP:    
	       CallOverlay0((void*)DoSleep, OVR_DO_MISC_DO);
	       /* DoSleep(); */
	       goto ret; /* return; */
    case A_DISEMBARK: 
	       CallOverlay0((void*)DoDisembark, OVR_DO_MISC_DO);
	       /* DoDisembark(); */
	       goto ret; /* return; */
    case A_LAUNCH:    
	       CallOverlay0((void*)DoLaunch, OVR_DO_MISC_DO);
	       /* DoLaunch(); */
	       goto ret; /* return; */
    case A_LAND:      
	       CallOverlay0((void*)DoLand, OVR_DO_MISC_DO);
	       /* DoLand(); */
	       goto ret; /* return; */
    case A_SWIM:      
	       CallOverlay0((void*)DoSwim, OVR_DO_MISC_DO);
	       /* DoSwim(); */
	       goto ret; /* return; */
    case A_TALKTO:    
	       CallOverlay0((void*)DoTalkTo, OVR_DO_MISC_DO);
	       /* DoTalkTo(); */
	       goto ret; /* return; */
    case A_GREET:     
	       CallOverlay0((void*)DoGreet, OVR_DO_MISC_DO);
	       /* DoGreet(); */
	       goto ret; /* return; */
    case A_SAY:       
	       CallOverlay0((void*)DoSay, OVR_DO_MISC_DO);
	       /* DoSay(); */
	       goto ret; /* return; */
    default:
	  	CallOverlay1((void*)DoActionOnObject, action, OVR_DO_PARSE_ACTION);
      	  	/* DoActionOnObject(action); */
      	  	goto ret; /* return; */
  }
ret:
  ReturnOverlay0(0);
}
