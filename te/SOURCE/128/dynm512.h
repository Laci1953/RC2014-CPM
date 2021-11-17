void Init512Banks(void);
char*	alloc512(short Size, char* BankPointer);
void	free512(char* Buf, char Bank);
void	setbank(char b);
int	GetTotalFree();
