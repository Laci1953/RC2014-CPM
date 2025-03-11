// Szilagyi Ladislau, March 2025

// >C -v -o hanoi.c biosr.as

#include <stdio.h>
#include <conio.h>
#include <string.h>

void CrtSetup(void);
char CrtIn(void);
char CrtSts(void);
void putstr(char*);

#define ESC 27

char ClearScreen[]={ESC,'[','2','J',0};
char CursorOff[]={ESC, '[','?','2','5','l'};
char CursorOn[]={ESC, '[','?','2','5','h'};
char EraseToEOL[]={ESC,'[','K'};

char SetYXPos[]={ESC,'[',' ',' ',';',' ',' ','H',0};
/* 2,3 must be filled with Y coordinates, in decimal */
/* 5,6 must be filled with X coordinates, in decimal */

void SetYX(char y0, char y1, char x0, char x1, char* buf)
{
  SetYXPos[2]=y0;
  SetYXPos[3]=y1;
  SetYXPos[5]=x0;
  SetYXPos[6]=x1;
  strcpy(buf,SetYXPos);
}

short size[3][9]; 	/* disk_size=1,2,...N_disks */
short count[3]; 	/* how many disks on the peg = 1,2,...N_disks */
short N_disks;
char bufHanoi[60];
int Counter = 0x2000;
char disks;

void SetDiskPos(short layer, short peg)
{
  short y0,y1,x0,x1;
  short y,x;

  y=20-layer;
  x=(15-N_disks)+layer+26*peg;

  y0=(short)('0'+(y/10));
  y1=(short)('0'+(y%10));
  x0=(short)('0'+(x/10));
  x1=(short)('0'+(x%10));

  SetYX(y0,y1,x0,x1,bufHanoi);
}

void DrawDisk(short len)
{
  char buf[30];
  short n;

  for (n=0; n<len; n++)
    buf[n]='*';

  buf[n]=0;
  strcat(bufHanoi,buf);
  putstr(bufHanoi);
}

void EraseDisk(short len)
{
  char buf[30];
  short n;

  for (n=0; n<len; n++)
    buf[n]=' ';

  buf[n]=0;
  strcat(bufHanoi,buf);
  putstr(bufHanoi);
}

/* Set cursor at the peg's top disk first char
   p = 0,1,2
*/
void GetTopPos(short p)
{
  short y0,y1,x0,x1;
  short y,x;
  short index;

  index=count[p]-1;

  y=20-index;
  x=15-size[p][index]+26*p;

  y0=(short)('0'+(y/10));
  y1=(short)('0'+(y%10));
  x0=(short)('0'+(x/10));
  x1=(short)('0'+(x%10));

  SetYX(y0,y1,x0,x1,bufHanoi);
}

void move(short frompeg, short topeg)
{
  short index;
  short sz;

  GetTopPos(frompeg);
  index=count[frompeg]-1;
  sz=size[frompeg][index];
  EraseDisk((2*sz)-1);
  count[frompeg]--;

  index=count[topeg];
  size[topeg][index]=sz;
  count[topeg]++;
  GetTopPos(topeg);
  DrawDisk((2*sz)-1);
}

void wait(void)
{
  int n, max=Counter;
  char ch;

  for (n=0;n<max;n++)
  {
    if (CrtSts())
    {
      ch = CrtIn();

      if (ch == 's')
      {
        if (Counter != 0x4000)
          Counter = Counter << 1;
      }
      else if (ch == 'f')
      {
        if (Counter != 0x200)
          Counter = Counter >> 1;
      }
      else if (ch == 3)
      {
        putstr(CursorOn);
        exit();
      }
    }
  }
}

void towers(short num, short frompeg, short topeg, short auxpeg)
{
  if (num == 1)
  {
    wait();
    move(frompeg, topeg);
    return;
  }

  towers(num - 1, frompeg, auxpeg, topeg);
  wait();
  move(frompeg, topeg);
  towers(num - 1, auxpeg, topeg, frompeg);
}

void InitDisks(void)
{
  short n;

  count[0]=N_disks;
  count[1]=0;
  count[2]=0;

  for (n=0; n<N_disks; n++)
    size[0][n]=N_disks-n;

  for (n=0; n<N_disks; n++)
  {
    SetDiskPos(n, 0);
    DrawDisk((2*N_disks)-1-(2*n));
  }

  SetYX('2','2','0','8',bufHanoi);
  strcat(bufHanoi,"Tower of Hanoi, ");
  putstr(bufHanoi);
  CrtOut(disks);
  putstr(" disks");
}

/* returns 1 if move is legal, 0 else */
char TryMove(short frompeg, short topeg)
{
  short indexto, indexfrom;
  short sz;

  indexto=count[topeg];
  indexfrom=count[frompeg];

  if (indexfrom == 0)
    return 0;

  indexfrom--;

  if (indexto == 0 || size[frompeg][indexfrom] < size[topeg][indexto-1])
  {
    GetTopPos(frompeg);
    sz=size[frompeg][indexfrom];
    EraseDisk((2*sz)-1);
    count[frompeg]--;

    size[topeg][indexto]=sz;
    count[topeg]++;
    GetTopPos(topeg);
    DrawDisk((2*sz)-1);

    return 1;
  }
  else
    return 0;
}

/* returns 0 : abandon, 1 : done */
char Play(void)
{
  char ch, valid;
  short from, to;

  do
  {
    SetYX('0','1','0','1',bufHanoi);
    strcat(bufHanoi,EraseToEOL);
    strcat(bufHanoi,"Move from (^C = quit) : ");
    putstr(bufHanoi);

    do
      ch=CrtIn();
    while (ch < '0' && ch > '3');

    if (ch == 3)
      return 0;

    CrtOut(ch);

    from=(short)(ch-'0');

    putstr(" to (^C = quit) : ");

    do
      ch=CrtIn();
    while (ch < '0' && ch > '3');

    if (ch == 3)
      return 0;

    CrtOut(ch);

    to=(short)(ch-'0');

    valid=TryMove(from, to);

    if (!valid)
    {
      SetYX('0','2','0','1',bufHanoi);
      strcat(bufHanoi,"Illegal move! Hit any key!");
      putstr(bufHanoi);
      ch=CrtIn();
      SetYX('0','2','0','1',bufHanoi);
      strcat(bufHanoi,EraseToEOL);
      putstr(bufHanoi);
    }
  }
  while (count[1] != N_disks);

  return 1;
}

void main(void)
{
  char status;

  CrtSetup();
  putstr("Tower of Hanoi\r\n\r\nYou have three rods and some disks of different diameters,\r\n");
  putstr(" which can slide onto any rod.\r\n");
  putstr("The game starts with the disks stacked on the leftmost rod in order of\r\n");
  putstr(" decreasing size, the smallest at the top.\r\n");
  putstr("The objective of the puzzle is to move the entire stack to the middle rod,\r\n");
  putstr(" obeying the following 3 simple rules:\r\n");
  putstr(" - Only one disk may be moved at a time\r\n");
  putstr(" - Each move takes the upper disk from one of the stacks, placing it\r\n");
  putstr("     on top of another stack or an empty rod\r\n");
  putstr(" - No disk may be placed on top of a disk that is smaller than it\r\n\r\n");
  putstr("Refer the rods using the digits 0, 1 and 2 (left ro right)\r\n\r\n");
  putstr("How many disks you prefer to use (2 to 9) : ");

  do
    disks = CrtIn();
  while (disks < '2' || disks > '9');

  N_disks = (short)(disks - '0');

  putstr(CursorOff);
  putstr(ClearScreen);
  InitDisks();

  status=Play();

  if (status == 0)
  {
    SetYX('0','1','0','1',bufHanoi);
    strcat(bufHanoi,EraseToEOL);
    strcat(bufHanoi,"Want to see the solution? (ENTER=yes) :");
    putstr(bufHanoi);

    if (CrtIn() != 0xD)
    {
      putstr(CursorOn);
      exit();
    }

    putstr(ClearScreen);
    SetYX('0','1','0','1',bufHanoi);
    strcat(bufHanoi,EraseToEOL);
    strcat(bufHanoi,"Use s=slower, f=faster or ^C=quit");
    putstr(bufHanoi);

    InitDisks();
    towers(N_disks, 0, 1, 2);
  }

  SetYX('2','3','1','2',bufHanoi);
  strcat(bufHanoi,"DONE!\n");
  putstr(bufHanoi);
  putstr(CursorOn);
}

