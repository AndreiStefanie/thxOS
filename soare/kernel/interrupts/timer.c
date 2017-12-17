#include "timer.h"
#include "isr.h"
#include "../boot.h"

#define CHANNEL0_DATA 0x40
#define CHANNEL1_DATA 0x41
#define CHANNEL2_DATA 0x42
#define COMMAND_REG 0x43

#define CLOCK 1193180

volatile static uint32 tick = 0;
volatile static uint32 freq = 0;

static void timer_callback()
{
	tick++;
}

void waitTicks(uint32 ticks) 
{
	uint32 final = tick + ticks;

	while (tick < final);
}

volatile void waitSeconds(uint32 seconds) 
{
	uint32 final = tick + seconds * freq;
	
	while (tick < final);
}

void init_timer(uint32 frequency)
{
	freq = frequency;

	register_interrupt_handler(IRQ0, (irq_t)&timer_callback);

	uint32 divisor = CLOCK / frequency;

	__outbyte(COMMAND_REG, 0x36);

	// Divisor has to be sent byte-wise
	uint8 l = (uint8)(divisor & 0xFF);
	uint8 h = (uint8)((divisor >> 8) & 0xFF);

	__outbyte(CHANNEL0_DATA, l);
	__outbyte(CHANNEL0_DATA, h);
}