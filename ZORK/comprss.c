/* ----------------------- compress.c ----------------------- */
/*fragments generated at https://www.dcode.fr/frequency-analysis*/

/*64 fragments of four letters each*/
extern char *FourFragments;

/*64 fragments of three letters each*/
extern char *ThreeFragments;

/*128 fragments of two letters each*/
extern char *TwoFragments;

/*must add up to 128*/
#define NUM_4_FRAGMENTS   20  /*max 64*/
#define NUM_3_FRAGMENTS   10  /*max 64*/
#define NUM_2_FRAGMENTS   98  /*max 128*/

/* ------------------------------------------------------------------
int GetDecompressTextSize(char *text_in, int size_in)
{
  char *p = text_in;
  int size_out = 0;

  while (p - text_in < size_in)
  {
     unsigned char ch = *p++;

    if (ch < 128) size_out++;
    else if (ch < 128 + NUM_4_FRAGMENTS) size_out += 4;
    else if (ch < 128 + NUM_4_FRAGMENTS+NUM_3_FRAGMENTS) size_out += 3;
    else size_out += 2;
  }
  return size_out;
}
--------------------------------------------------------------------*/

int DecompressText(char *text_in, int size_in, char *text_out)
{
  char *p = text_in, *t = text_out, *q;

  while (p - text_in < size_in)
  {
     unsigned char ch = *p++;

    if (ch < 128) *t++ = ch;
    else if (ch < 128 + NUM_4_FRAGMENTS)
    {
      q = FourFragments + 4*(ch - 128);
      *t++ = *q++; *t++ = *q++; *t++ = *q++; *t++ = *q++;
    }
    else if (ch < 128 + NUM_4_FRAGMENTS+NUM_3_FRAGMENTS)
    {
      q = ThreeFragments + 3*(ch - (128 + NUM_4_FRAGMENTS));
      *t++ = *q++; *t++ = *q++; *t++ = *q++;
    }
    else
    {
      q = TwoFragments + 2*(ch - (128 + NUM_4_FRAGMENTS+NUM_3_FRAGMENTS));
      *t++ = *q++; *t++ = *q++;
    }
  }
  return (t - text_out);
}
