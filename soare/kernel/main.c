#include "screen.h"
#include "interrupts\pic.h"
#include "interrupts\isr.h"
#include "interrupts\idt.h"
#include "devices\timer.h"
#include "devices\keyboard.h"
#include "devices\ata.h"
#include "memory\page.h"

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

    
    ata_read(ATA_PRIMARY_IO, 0x28, );

	/*volatile int one = 1;
	volatile int zero = 0;
	volatile int rez = one / zero;*/

    mem_init();
    page_t *page_1 = mem_alloc_page();
    page_t *page_2 = mem_alloc_page();

    PrintString("Page 1 start address: ");
    PrintInt((int)mem_get_start_address(page_1));
    PrintString("\nPage 2 start address: ");
    PrintInt((int)mem_get_start_address(page_2));
    PrintChar('\n');

	while (1)
	{
	}
}
