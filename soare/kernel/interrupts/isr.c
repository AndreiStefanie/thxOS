#include "isr.h"
#include "screen.h"
#include "pic.h"

isr_t interrupt_handlers[256];

static void empty_handler(interrupt_context_t *context)
{
	const char *exception_desc[] =
	{
		"#DE: Divide by zero",
		"#DB: Debug",
		"Non-maskable interrupt",
		"#BP: Breakpoint",
		"#OF: Overflow",
		"#BR: BOUND Range exceeded",
		"#UD: Invalid opcode",
		"#NM: Device not available",
		"#DF: Double fault",
		"Coprocessor segment overrun",
		"#TS: Invalid TSS",
		"#NP: Segment not present",
		"#SS: Stack fault",
		"#GP: General protection",
		"#PF: Page fault",
		"Unknown",
		"#MF: x87 FPU floating-point error",
		"#AC: Alignment check",
		"#MC: Machine-check",
		"#XM: SIMD floating-point",
		"#VE: Virtualization",
	};

	ClearScreen();
	SetColor(VGA_WHITE);
	PrintString("Unhandled exception: ");
	PrintString(exception_desc[context->int_no]);
	PrintChar('\n');
	panic(context);
}

void init_handlers()
{
	for (size_t i = 0; i < 256; i++)
	{
		interrupt_handlers[i] = (isr_t)&empty_handler;
	}
}

void register_interrupt_handler(uint8_t interrupt, isr_t handler)
{
	interrupt_handlers[interrupt] = handler;
}

void isr_handler(interrupt_context_t *context)
{
	isr_t handler = interrupt_handlers[(int)context->int_no];
	handler(context);
}

void irq_handler(interrupt_context_t *context)
{
	// Send an EOI (end of interrupt) signal to the PICs.
	if (0x28 <= context->int_no)
	{
		// Send to slave.
		__outbyte(PIC2, EOI);
	}

	// Send to master.
	__outbyte(PIC1, EOI);

	isr_handler(context);
}

void mask_irq(uint8_t irq)
{
	uint8_t value;
	uint16_t port;

	if (ALL == irq)
	{
		port = PIC1DATA;
		value = 0xFF;
	}
	else
	{
		if (irq < 0x28)
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

void unmask_irq(uint8_t irq)
{
	uint8_t value;
	uint16_t port;

	if (ALL == irq)
	{
		port = PIC1DATA;
		value = 0x00;
	}
	else
	{
		if (irq < 0x28)
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

void panic(interrupt_context_t *context)
{
	const char *int_frame_desc[] =
	{
		"rax", "rbx", "rcx", "rdx",
		"rsi", "rdi", "rbp",
		"r8", "r9", "r10", "r11",
		"r12", "r13", "r14", "r15",
		"int_no", "retaddr",
		"cs", "rflags",
		"rsp", "ss"
	};

	uint64_t *addr = &context->regs.rax;
	for (size_t i = 0; i < 21; i++)
	{
		PrintString(int_frame_desc[i]);
		PrintString(": ");
		PrintInt(*addr);
		PrintChar('\n');

		++addr;
	}

	PrintString("CPU halted.");

	__cli();
	__halt();
}
