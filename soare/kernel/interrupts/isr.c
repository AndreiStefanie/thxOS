#include "isr.h"
#include "../screen.h"
#include "pic.h"

isr_t interrupt_handlers[256];

static void empty_handler(interrupt_context_t context)
{
	SetColor(VGA_WHITE);
	PutStringLine("Unhandled exception: ", 0);
	PutInt((int)context.int_no, 21);
}

void init_handlers()
{
	for (size_t i = 0; i < 256; i++)
	{
		interrupt_handlers[i] = (isr_t)&empty_handler;
	}
}

void register_interrupt_handler(uint8 interrupt, isr_t handler)
{
	interrupt_handlers[interrupt] = handler;
}

void isr_handler(interrupt_context_t context)
{
	__magic();
	volatile int temp = (int)context.int_no;
	isr_t handler = interrupt_handlers[temp];
	handler(context);
}

void __stdcall irq_handler(interrupt_context_t context)
{
	isr_handler(context);
	
	// Send an EOI (end of interrupt) signal to the PICs.
	if (0x28 <= context.int_no)
	{
		// Send reset signal to slave.
		__outbyte(PIC2, EOI);
	}

	// Send reset signal to master.
	__outbyte(PIC1, EOI);
}

void mask_irq(uint8 irq)
{
	uint8 value;
	uint16 port;
	
	if (ALL == irq)
	{
		port = PIC1DATA;
		value = 0xFF;
	}
	else
	{
		if (irq < 40)
		{
			port = PIC1DATA;
		}
		else
		{
			port = PIC2DATA;
			irq -= 8;
		}

		value = __inbyte(port) | (1 << irq);
	}

	__outbyte(port, value);
}

void unmask_irq(uint8 irq)
{
	uint8 value;
	uint16 port;
	
	if (ALL == irq)
	{
		port = PIC1DATA;
		value = 0x00;
	}
	else
	{
		if (irq < 40)
		{
			port = PIC1DATA;
		}
		else
		{
			port = PIC2DATA;
			irq -= 8;
		}

		value = __inbyte(port) & ~(1 << irq);
	}

	__outbyte(port, value);
}
