#include "boot.h"
#include "screen.h"

void EntryPoint(void)
{
	ClearScreen();
	
	SetColor(10);
	PutStringLine("Boot OK!", 4);
	PutChar('a', 400);

	welcome();
}
