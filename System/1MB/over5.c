#include <stdio.h>
#include <string.h>
#include <overlays.h>

void proc(void)
{
  int ret;

  ret=2;
  printf("\r\nPrinted by proc_5");
  ReturnOverlay0(ret);
}
