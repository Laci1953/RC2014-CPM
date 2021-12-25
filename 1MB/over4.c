#include <stdio.h>
#include <string.h>
#include <dyn512.h>
#include <overlays.h>

#define proc5	0x4000

void proc(char* p, char bank)
{
  int ret;

  setbank(bank);
  strcpy(p,"\r\nStored by proc_4");
  ret=CallOverlay0((void*)proc5, 5);
  printf("\r\nOverlay5 returned %d",ret);
  ReturnOverlay2(1);
}
