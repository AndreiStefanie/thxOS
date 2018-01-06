#include "keyboard.h"
#include "..\interrupts\isr.h"
#include "..\boot.h"

static void keyboard_callback(const interrupt_context_t context)
{
	(void)context;
	
	__magic();
}

void init_keyboard()
{
	register_interrupt_handler(KEYBOARD, (isr_t)&keyboard_callback);
}
