#include "timer.h"
#include "isr.h"
#include "boot.h"

uint32 tick = 0;
uint32 freq = 0;

static void timer_callback()
{
	tick++;
}

void waitTicks(uint32 dist) 
{
	uint32 final = tick + dist;

	while (tick < final);
}

void waitSeconds(uint32 dist) 
{
	uint32 final = tick + dist * freq;
	
	while (tick < final);
}

void init_timer(uint32 frequency)
{
	freq = frequency;

	// Firstly, register the timer callback.
	register_interrupt_handler(IRQ0, (irq_t)&timer_callback);

	// The value sent to the PIT is the value to divide it's input clock
	// (1193180 Hz) by, to get the required frequency.
	// The divisor must fit into 16-bits.
	uint32 divisor = 1193180 / frequency;

	// Send the command byte.
	__outb(0x43, 0x36);

	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	uint8 l = (uint8)(divisor & 0xFF);
	uint8 h = (uint8)((divisor >> 8) & 0xFF);

	// Send the frequency divisor.
	__outb(0x40, l);
	__outb(0x40, h);
}