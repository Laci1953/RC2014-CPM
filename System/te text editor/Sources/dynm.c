#include <stdlib.h>
#include <string.h>

void InitDynM(void)
{
}

void* Alloc(short size, char* bank)
{
return malloc(size);
}

void Free(void* buf, char flag)
{
free(buf);
}

int GetTotalFree(void)
{
return 0;
}

/*	destination is in Lower RAM */
void	GetString(char* dest, char* source, char source_flag)
{
strcpy(dest, source);
}

/*	source is in Lower RAM */
void	PutString(char* source, char* dest, char dest_flag)
{
strcpy(dest, source);
}

short	StringLen(char* s, char s_flag)
{
return strlen(s);
}

/*	get/put vector[index] */
char	GetByte(char* vector, short index, char flag)
{
return vector[index];
}

void	PutByte(char* vector, short index, char byte, char flag)
{
vector[index]=byte;
}

char*	GetWord(char* vector, short index, char flag)
{
return (char*)((int*)vector)[index];
}

void	PutWord(char* vector, short index, short word, char flag)
{
((int*)vector)[index] = word;
}

