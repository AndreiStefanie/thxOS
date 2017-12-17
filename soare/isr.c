#include "isr.h"
#include "screen.h"

irq_t interrupt_handlers[256];

void register_interrupt_handler(uint8 n, irq_t handler)
{
	interrupt_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
	if (interrupt_handlers[regs.int_no] != 0)
	{
		irq_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
	else
	{
		SetColor(VGA_WHITE);
		PutStringLine("unhandled interrupt: ", 0);
		PutInt(regs.int_no, MAX_COLUMNS);
	}
}

void irq_handler(registers_t regs)
{
	// Send an EOI (end of interrupt) signal to the PICs.
	// If this interrupt involved the slave.
	if (regs.int_no >= 40)
	{
		// Send reset signal to slave.
		__outbyte(0xA0, 0x20);
	}
	// Send reset signal to master. (As well as slave, if necessary).
	__outbyte(0x20, 0x20);

	if (interrupt_handlers[regs.int_no] != 0)
	{
		irq_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}