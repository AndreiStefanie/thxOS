#include "isr.h"
#include "../screen.h"

#define EOI 0x20

irq_t interrupt_handlers[256];

void register_interrupt_handler(uint8 n, irq_t handler)
{
	interrupt_handlers[n] = handler;
}

void isr_handler(registers_t regs)
{
	if (0 != interrupt_handlers[regs.int_no])
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
	if (40 <= regs.int_no)
	{
		// Send reset signal to slave.
		__outbyte(0xA0, EOI);
	}

	// Send reset signal to master. (As well as slave, if necessary).
	__outbyte(0x20, EOI);

	if (0 != interrupt_handlers[regs.int_no])
	{
		irq_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}