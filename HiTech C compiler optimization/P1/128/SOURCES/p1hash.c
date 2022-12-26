#include "p1.h"

char*	Get_hashtab(int index);
char*	Get_SymList(void);

bool IsHeader(char* addr);

void InitHash(void)
{
	int n;
	char* h;

	for (n = 0; n < HASHTABSIZE; n++)
	{
		h = Get_hashtab(n);
		PutWord(h+OFF_first, (short)h);
		PutWord(h+OFF_last, (short)h); 
	}

	h = Get_SymList();
	PutWord(h+OFF_first, (short)h);
	PutWord(h+OFF_last, (short)h); 
}

void InitHashEntry(int index, char* sym)
{
	register char* h = Get_hashtab(index);

	PutWord(h+OFF_first, (short)sym);
	PutWord(h+OFF_last, (short)sym); 
	PutWord(sym+OFF_next, (short)h);
	PutWord(sym+OFF_prev, (short)h);
}

void AddToList(register char* hdr, char* new)
{
	char* first = (char*)GetWord(hdr+OFF_first);

	if (first == hdr)
	{
		PutWord(hdr+OFF_first, (short)new);
		PutWord(hdr+OFF_last, (short)new);
		PutWord(new+OFF_next, (short)hdr);
		PutWord(new+OFF_prev, (short)hdr);
	}
	else
	{
		PutWord(first+OFF_prev, (short)new);
		PutWord(new+OFF_next, (short)first);
		PutWord(new+OFF_prev, (short)hdr);
		PutWord(hdr+OFF_first, (short)new);
	}
}

void RemoveFromList(register char* sym)
{
	char* prev;
	char* next;

	prev = (char*)GetWord(sym+OFF_prev);
	next = (char*)GetWord(sym+OFF_next);

	if (IsHeader(prev))
		PutWord(prev+OFF_first, (short)next);
	else
		PutWord(prev+OFF_next, (short)next);

	if (IsHeader(next))
		PutWord(next+OFF_last, (short)prev);
	else
		PutWord(next+OFF_prev, (short)prev);
}

char* GetNextSym(char* hdr, register char* crt)
{
	char* next = (char*)GetWord(crt+OFF_next);

	if (hdr == next)
		return 0;
	else
		return next;
}

char* GetFirstSym(register char* hdr)
{
	char* first = (char*)GetWord(hdr+OFF_first);

	if (first == hdr)
		return 0;
	else
		return first;
}

