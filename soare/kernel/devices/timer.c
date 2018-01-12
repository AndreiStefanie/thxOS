#include "timer.h"
#include "isr.h"
#include "boot.h"
#include "screen.h"

#define CHANNEL0_DATA 0x40
#define CHANNEL1_DATA 0x41
#define CHANNEL2_DATA 0x42
#define COMMAND_REG	  0x43

static volatile uint64_t tick = 0;
static uint32_t frequency = TIMER_FREQ;
static volatile uint64_t sec = 0;

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

void waitTicks(uint32_t ticks)
{
	volatile uint64_t final = tick + ticks;

	while (tick < final);
}

void waitSeconds(uint32_t seconds)
{
	waitTicks(seconds * frequency * 15);
}

void waitMillis(uint32_t millis)
{
	waitTicks(millis * frequency * 15 / 1000);
}

void init_timer()
{
	register_interrupt_handler(TIMER, &timer_callback);

	uint32_t divisor = CLOCK_TICK_RATE / frequency;

	__outbyte(COMMAND_REG, 0x36);

	// Divisor has to be sent byte-wise
	uint8_t l = (uint8_t)(divisor);
	uint8_t h = (uint8_t)(divisor >> 8);

	__outbyte(CHANNEL0_DATA, l);
	__outbyte(CHANNEL0_DATA, h);

	unmask_irq(TIMER);
}