Fixed point arithmetic library

It uses 8.8 ( 16 bit ) fixed point numbers, containing the signed integer part in the bits 15 - 8, and the fractional part in the bits 7 - 0.

The fractional part value is the sum of 1/2^N parts (if bit 7 = 1, add 1/2, if bit 6 = 1, add 1/4, ...)

For example, in C, the fixed point value 0x1C0 will be interpreted as 1.75 ( 1 + 1/2 + 1/4 )

The functions provided can be used in assembler or C (HiTech)

Functions:

// add with + (add as integers)
// sub with - (sub as integers)
int neg(int fp); //negate
int positive(int fp); // absolute value of
int mul(int fp1, int fp2);
int div(int pf1, int fp2);
int fpsqrt(int fp);
int sin(int fp);
int cos(int fp);
int arctan(int fp);
int xdivytofp(int x, int y); // transform X/Y (X < Y) into fixed point
void fixedtoa(int fp, char* buf); // stores in buf an ascii representation of the fixed point value
int atofixed(char* p); // converts the ascii string to fixed point : accepts iii[.ddd] (max 127.999)

This library is used in the following games: arrow, bowling, curling, marksman, pool, treasure

If you want to use, on the screen, objects placed in a (-127 ... 127) x (-127 ... 127) system of coordinates, this library works ok.
It is faster, compared to floating point, and offers good enough accuracy.
However, some basic rules must be observed, e.g. if you want to compute S = A * B / C, use S = A * (B / C), to avoid overflow.
