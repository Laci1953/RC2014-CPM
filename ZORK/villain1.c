#include <ovr47.h>

/* defines, enums ------------------------------------------------------------- */
#define OVR_VILLAINS_BLOW	47

enum
{
W_O_AXE,
W_O_KNIFE,
W_O_RUSTY_KNIFE,
W_O_STILETTO,      
W_O_SWORD 
};

#define  O_AXE 21
#define  O_KNIFE 33
#define  O_RUSTY_KNIFE 51
#define  O_STILETTO 60      
#define  O_SWORD 61 

/* extern functions ---------------------------------------------------------------- */
int GetRandom(int);

short CallOverlay1(void* Routine, short Par1, char CalledRamBank);

/* variables-------------------------------------------------------------------------*/

/* overlays ----------------------------------------------------------------------- */
/* ------
void BlowRemark(short msg);

#define BlowRemark 0
--------- */
/* code ---------------------------------------------------------------------------- */


short BlowMsgOffset[10 * 4] =
{
  0, 6, 11, 14, 18, 22, 27, 29, 30, 31,
  0, 4,  5,  8, 12, 15, 19, 22, 24, 25,
  0, 4,  6,  9, 13, 17, 20, 23, 26, 28,
  0, 2,  3,  4,  6,  8, 10, 12, 13, 14
};

/* i:       0 - NUM_VILLAINS-1*/
/* blow:    1 - 9*/
/* weapon:  O_**/

void PrintBlowRemark(int player_flag, int i, int blow, int weapon)
{
  int j, index, num, msg, nweapon, arg;

  j = player_flag ? 0 : 1+i;
  index = 10*j + (blow-1);
  num = BlowMsgOffset[index+1] - BlowMsgOffset[index];
  msg = 100*j + BlowMsgOffset[index] + GetRandom(num);

  switch (weapon)
  {
    case O_STILETTO:    nweapon = W_O_STILETTO;		break;
    case O_AXE:         nweapon = W_O_AXE;  		break;
    case O_SWORD:       nweapon = W_O_SWORD;       	break;
    case O_KNIFE:       nweapon = W_O_KNIFE; 		break;
    case O_RUSTY_KNIFE: nweapon = W_O_RUSTY_KNIFE; 	break;
  }

  arg = (msg << 5) + (nweapon << 2) + i;

  CallOverlay1((void*)BlowRemark, arg, OVR_VILLAINS_BLOW);
}

