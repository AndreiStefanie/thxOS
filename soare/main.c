#include "screen.h"
#include "timer.h"

void EntryPoint(void)
{
	ClearScreen();
	SetColor(VGA_LIGHT_RED);
	Welcome();

	//init_timer(100);
}
