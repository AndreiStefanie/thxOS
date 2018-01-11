#include "timer.h"
#include "..\interrupts\isr.h"
#include "..\boot.h"
#include "..\screen.h"

#define CHANNEL0_DATA 0x40
#define CHANNEL1_DATA 0x41
#define CHANNEL2_DATA 0x42
#define COMMAND_REG	  0x43

static volatile uint64 tick = 0;
static uint32 frequency = TIMER_FREQ;
static volatile uint64 sec = 0;

static void timer_callback(interrupt_context_t *context)
{
	UNUSED(context);
	tick++;

	if (tick % 1500 == 0)
	{
		sec++;
		PutInt((int)sec, MAX_COLUMNS - 3);
	}
}

void waitTicks(uint32 ticks)
{
	volatile uint64 final = tick + ticks;

	while (tick < final);
}

void waitSeconds(uint32 seconds)
{
	waitTicks(seconds * frequency * 15);
}

void waitMillis(uint32 millis)
{
	waitTicks(millis * frequency * 15 / 1000);
}

void init_timer()
{
	register_interrupt_handler(TIMER, &timer_callback);

	uint32 divisor = CLOCK_TICK_RATE / frequency;

	__outbyte(COMMAND_REG, 0x36);

	// Divisor has to be sent byte-wise
	uint8 l = (uint8)(divisor);
	uint8 h = (uint8)(divisor >> 8);

	__outbyte(CHANNEL0_DATA, l);
	__outbyte(CHANNEL0_DATA, h);

	unmask_irq(TIMER);
}