#include "idt.h"
#include "..\util.h"
#include "..\boot.h"

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

static void idt_set_gate(uint8, uint64, uint16, uint8);

void init_idt()
{
	idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
	idt_ptr.base = (uint64)&idt_entries;

	set_mem(&idt_entries, 0, sizeof(idt_entry_t) * 256);

	idt_set_gate(0, (uint64)isr0, 0x08, 0x8E);
	idt_set_gate(1, (uint64)isr1, 0x08, 0x8E);
	idt_set_gate(2, (uint64)isr2, 0x08, 0x8E);
	idt_set_gate(3, (uint64)isr3, 0x08, 0x8E);
	idt_set_gate(4, (uint64)isr4, 0x08, 0x8E);
	idt_set_gate(5, (uint64)isr5, 0x08, 0x8E);
	idt_set_gate(6, (uint64)isr6, 0x08, 0x8E);
	idt_set_gate(7, (uint64)isr7, 0x08, 0x8E);
	idt_set_gate(8, (uint64)isr8, 0x08, 0x8E);
	idt_set_gate(9, (uint64)isr9, 0x08, 0x8E);
	idt_set_gate(10, (uint64)isr10, 0x08, 0x8E);
	idt_set_gate(11, (uint64)isr11, 0x08, 0x8E);
	idt_set_gate(12, (uint64)isr12, 0x08, 0x8E);
	idt_set_gate(13, (uint64)isr13, 0x08, 0x8E);
	idt_set_gate(14, (uint64)isr14, 0x08, 0x8E);
	idt_set_gate(15, (uint64)isr15, 0x08, 0x8E);
	idt_set_gate(16, (uint64)isr16, 0x08, 0x8E);
	idt_set_gate(17, (uint64)isr17, 0x08, 0x8E);
	idt_set_gate(18, (uint64)isr18, 0x08, 0x8E);
	idt_set_gate(19, (uint64)isr19, 0x08, 0x8E);
	idt_set_gate(20, (uint64)isr20, 0x08, 0x8E);
	idt_set_gate(21, (uint64)isr21, 0x08, 0x8E);
	idt_set_gate(22, (uint64)isr22, 0x08, 0x8E);
	idt_set_gate(23, (uint64)isr23, 0x08, 0x8E);
	idt_set_gate(24, (uint64)isr24, 0x08, 0x8E);
	idt_set_gate(25, (uint64)isr25, 0x08, 0x8E);
	idt_set_gate(26, (uint64)isr26, 0x08, 0x8E);
	idt_set_gate(27, (uint64)isr27, 0x08, 0x8E);
	idt_set_gate(28, (uint64)isr28, 0x08, 0x8E);
	idt_set_gate(29, (uint64)isr29, 0x08, 0x8E);
	idt_set_gate(30, (uint64)isr30, 0x08, 0x8E);
	idt_set_gate(31, (uint64)isr31, 0x08, 0x8E);

	idt_set_gate(TIMER, (uint64)irq0, 0x08, 0x8E);
	idt_set_gate(KEYBOARD, (uint64)irq1, 0x08, 0x8E);
	idt_set_gate(CASCADE, (uint64)irq2, 0x08, 0x8E);
	idt_set_gate(COM2_4, (uint64)irq3, 0x08, 0x8E);
	idt_set_gate(COM1_3, (uint64)irq4, 0x08, 0x8E);
	idt_set_gate(LPT, (uint64)irq5, 0x08, 0x8E);
	idt_set_gate(FLOPPY, (uint64)irq6, 0x08, 0x8E);
	idt_set_gate(FREE7, (uint64)irq7, 0x08, 0x8E);
	idt_set_gate(CLOCK, (uint64)irq8, 0x08, 0x8E);
	idt_set_gate(FREE9, (uint64)irq9, 0x08, 0x8E);
	idt_set_gate(FREE10, (uint64)irq10, 0x08, 0x8E);
	idt_set_gate(FREE11, (uint64)irq11, 0x08, 0x8E);
	idt_set_gate(PS2MOUSE, (uint64)irq12, 0x08, 0x8E);
	idt_set_gate(COPROC, (uint64)irq13, 0x08, 0x8E);
	idt_set_gate(IDE_1, (uint64)irq14, 0x08, 0x8E);
	idt_set_gate(IDE_2, (uint64)irq15, 0x08, 0x8E);

	__lidt((uint64)&idt_ptr);
}

static void idt_set_gate(uint8 num, uint64 base, uint16 sel, uint8 flags)
{
	idt_entries[num].base_low = base & 0xFFFF;
	idt_entries[num].base_mid = (base >> 16) & 0xFFFF;
	idt_entries[num].base_high = (base >> 32) & 0xFFFFFFFF;

	idt_entries[num].selector = sel;
	idt_entries[num].always0 = 0;
	idt_entries[num].reserved = 0;

	idt_entries[num].flags = flags /* | 0x60 */;
}
