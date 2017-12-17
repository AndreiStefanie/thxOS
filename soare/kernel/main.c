#include "screen.h"
#include "interrupts/timer.h"
#include "interrupts/pic.h"

void EntryPoint(void)
{
	ClearScreen();
	SetColor(VGA_LIGHT_RED);
	Welcome();

	init_pics(0x20, 0x28);

	__sti();

	init_timer(100);

	__magic();
	waitSeconds(1);
}
