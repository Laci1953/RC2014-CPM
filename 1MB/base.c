#include <stdio.h>
#include <string.h>
#include <dyn512.h>
#include <overlays.h>

char *pBuf;
short ret;
char bank;

#define proc_4		0x4000

void main(void)
{
  int ret;

  InitOverlays();
  Init512Banks();
  pBuf=alloc512(40, &bank);
  ret=CallOverlay2((void*)proc_4, pBuf, (short)bank, 4);
  setbank(bank);
  printf(pBuf);
  printf("\r\nOverlay4 returned %d",ret);
  free512(pBuf,bank);
}
