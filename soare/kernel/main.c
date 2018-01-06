#include "screen.h"
#include "interrupts\pic.h"
#include "interrupts\isr.h"
#include "interrupts\idt.h"
#include "devices\timer.h"
#include "devices\keyboard.h"

void EntryPoint(void)
{
	ClearScreen();

	init_idt();
	init_handlers();
	init_pics(0x20, 0x28);

	mask_irq(ALL);

	unmask_irq(TIMER);
	unmask_irq(KEYBOARD);

	init_timer(50);
	init_keyboard();

	// cause DIV_BY_0
	/*volatile int test = 1;
	volatile int zero = 0;
	volatile int rez = test / zero;*/

	__sti();

	SetColor(VGA_LIGHT_RED);
	Welcome();

	//__magic();
	waitSeconds(30);
}
