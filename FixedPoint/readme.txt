Fixed point arithmetic library

Can be used in assembler or C (HiTech)

// add with +
// sub with -
int neg(int fp); //negate
int positive(int fp); // abs
int mul(int fp1, int fp2);
int div(int pf1, int fp2);
int fpsqrt(int fp);
int sin(int fp);
int cos(int fp);
int arctan(int fp);
int xdivytofp(int x, int y); // transform X/Y (X < Y) into fixed point
void fixedtoa(int fp, char* buf);
int atofixed(char* p); // accepts iii[.ddd] (max 127.999)
