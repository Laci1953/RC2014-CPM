#include <stdio.h>
#include <string.h>
#include <ctype.h>

char HexFileName[14]; 
char buf[78];

short GetNibble(char* p)
{
  if (isalpha(*p))
    return (short)((short)*p - (short)'A' + 10);
  else
    return (short)((short)*p - (short)'0');  
}

short GetByte(char* p)
{
  short high,low;

  high = GetNibble(p);
  p++;
  low = GetNibble(p);
  return (16*high + low);
}

void HexLoad(void)
{
  FILE* f;
  char* pbuf, *pcode;
  short count, highaddr, lowaddr, byte;
  short endmark;

  strcpy(HexFileName, "zorkcomm.hex");
  f = fopen(HexFileName, "r");
  
  do
  {
    fgets(buf, 78, f);
    pbuf = buf+1;
    count = GetByte(pbuf);
    pbuf += 2;
    highaddr = GetByte(pbuf);
    pbuf += 2;
    lowaddr = GetByte(pbuf);
    pbuf += 2;
    pcode = (char*)((highaddr << 8) + lowaddr);
    endmark = GetByte(pbuf);
    pbuf += 2;
    while (count)
    {
      byte = GetByte(pbuf);
      pbuf += 2;
      *pcode++ = (char)byte;
      count--;
    }
  }
  while (!endmark);
  
  fclose(f);
}
