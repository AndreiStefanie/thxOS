#ifndef TIMER_H
#define TIMER_H

#include "../defs.h"

void init_timer(uint32);
void waitTicks(uint32);
volatile void waitSeconds(uint32);

#endif