void InitOverlays(void);
short CallOverlay0(void* Routine, char CalledRomBank);
short CallOverlay1(void* Routine, short Par1, char CalledRomBank);
short CallOverlay2(void* Routine, short Par1, short Par2, char CalledRomBank);
void ReturnOverlay0(short ret_val);
void ReturnOverlay0_(short ret_val);
void ReturnOverlay1(short ret_val);
void ReturnOverlay2(short ret_val);
