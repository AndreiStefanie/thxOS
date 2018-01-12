#include "screen.h"
#include "pic.h"
#include "isr.h"
#include "idt.h"
#include "timer.h"
#include "keyboard.h"
#include "ata.h"
#include "page.h"

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
	//waitSeconds(1);
	PrintString("done\n");

	PrintString("Initializing keyboard...");
	init_keyboard();
	//waitSeconds(1);
	PrintString("done\n");

	PrintString("Initializing ide...");
	ata_init();
	//waitSeconds(1);
	PrintString("done\n");

	Welcome();

	uint8_t disk_data[SECTOR_SIZE];
	ata_read(ATA_PRIMARY_BUS, 0x28, disk_data);

	/*volatile int one = 1;
	volatile int zero = 0;
	volatile int rez = one / zero;
	UNUSED(rez);*/

	mem_init();
	page_t *page_1 = mem_alloc_page();
	page_t *page_2 = mem_alloc_page();

	PrintString("Page 1 space start address: ");
	PrintInt(mem_get_start_address(page_1));
	PrintString("\nPage 2 space start address: ");
	PrintInt(mem_get_start_address(page_2));
	PrintChar('\n');

	while (1)
	{
	}
}
