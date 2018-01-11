#include "screen.h"
#include "interrupts\pic.h"
#include "interrupts\isr.h"
#include "interrupts\idt.h"
#include "devices\timer.h"
#include "devices\keyboard.h"
#include "devices\ata.h"

void EntryPoint(void)
{
	screen_init();

	PrintChar('\n');

	PrintString("Initializing interrupts...");
	init_idt();
	init_handlers();
	init_pics(0x20, 0x28);
	__sti();
	PrintString("done\n");

	PrintString("Initializing timer...");
	init_timer();
	waitSeconds(1);
	PrintString("done\n");

	PrintString("Initializing keyboard...");
	init_keyboard();
	waitSeconds(1);
	PrintString("done\n");

	PrintString("Initializing ide...");
	ata_init();
	waitSeconds(1);
	PrintString("done\n");

	Welcome();

	/*volatile int one = 1;
	volatile int zero = 0;
	volatile int rez = one / zero;*/

	while (1)
	{
	}
}
