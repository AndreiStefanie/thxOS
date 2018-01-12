#pragma once
#include "defs.h"

#define TIMER_FREQ		100 /* in HZ */
#define CLOCK_TICK_RATE 1193180 /* 8254 chip's internal oscillator frequency */

void init_timer();
void waitTicks(uint32_t);
void waitSeconds(uint32_t);
void waitMillis(uint32_t millis);
