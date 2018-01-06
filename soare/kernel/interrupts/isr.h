#pragma once

#include "..\defs.h"
#include "..\cpu.h"

#define ALL         0xFF

#define TIMER		0x20
#define KEYBOARD	0x21
#define CASCADE		0x22
#define COM2_4		0x23
#define COM1_3		0x24
#define LPT			0x25
#define FLOPPY		0x26
#define FREE7		0x27

#define CLOCK		0x28
#define FREE9		0x29
#define FREE10		0x2A
#define FREE11		0x2B
#define PS2MOUSE	0x2C
#define COPROC		0x2D
#define IDE_1		0x2E
#define IDE_2		0x2F

#define EOI 0x20

//#pragma pack(push, 1)
struct interrupt_context
{
	registers_t regs;       // all general-purpose registers.
	uint64      error;      // exception error identifier.
	uint64      int_no;		// interrupt vector number.
	uint64      retaddr;    // interrupt return address.
	uint64      cs;         // code segment.
	uint64      rflags;     // flags register.
	uint64      rsp;        // stack pointer.
	uint64      ss;         // stack segment.
};
//#pragma pack(pop)

typedef struct interrupt_context interrupt_context_t;

typedef void(*isr_t)(interrupt_context_t context);
void register_interrupt_handler(uint8 interrupt, isr_t handler);

void mask_irq(uint8);
void unmask_irq(uint8);
void init_handlers(void);
