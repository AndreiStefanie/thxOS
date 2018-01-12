#include "keyboard.h"
#include "isr.h"
#include "screen.h"

#define KB_PORT_DATA 0x60

// US English PS/2 keyboard scan map
static const keylayout_t ps2_layout =
{
	.shifted =
	{
		INV, ESC, '!', '@', '#', '$', '%', '^',
		'&', '*', '(', ')', '_', '+', BSP, TAB, // 0
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
		'O', 'P', '{', '}', ENT, CTL, 'A', 'S', // 1
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
		'"', '~', SHF, '|', 'Z', 'X', 'C', 'V', // 2
		'B', 'N', 'M', '<', '>', '?', SHF, PSC,
		ALT, ' ', CLK, F_1, F_2, F_3, F_4, F_5, // 3
		F_6, F_7, F_8, F_9, F10, NLK, SLK, KHM,
		KUP, KPU, KMI, KLT, KCT, KRT, KPL, KEN, // 4
		KDN, KPD, KIN, KDL, INV, INV, INV, F11,
		F12, INV, INV, INV, INV, INV, INV, INV, // 5
		SES, SES, INV, INV, INV, INV, INV, INV,
		INV, INV, INV, INV, INV, INV, INV, INV, // 6
		INV, INV, INV, INV, INV, INV, INV, INV,
		INV, INV, INV, INV, INV, INV, INV, INV, // 7
	},
	.unshifted =
	{
		INV, ESC, '1', '2', '3', '4', '5', '6',
		'7', '8', '9', '0', '-', '=', BSP, TAB, // 0
		'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
		'o', 'p', '[', ']', ENT, CTL, 'a', 's', // 1
		'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
		APO, '`', SHF, BSL, 'z', 'x', 'c', 'v', // 2
		'b', 'n', 'm', ',', '.', '/', SHF, PSC,
		ALT, ' ', CLK, F_1, F_2, F_3, F_4, F_5, // 3
		F_6, F_7, F_8, F_9, F10, NLK, SLK, KHM,
		KUP, KPU, KMI, KLT, KCT, KRT, KPL, KEN, // 4
		KDN, KPD, KIN, KDL, INV, INV, INV, F11,
		F12, INV, INV, INV, INV, INV, INV, INV, // 5
		SES, SES, INV, INV, INV, INV, INV, INV,
		INV, INV, INV, INV, INV, INV, INV, INV, // 6
		INV, INV, INV, INV, INV, INV, INV, INV,
		INV, INV, INV, INV, INV, INV, INV, INV, // 7
	}
};

static bool shiftDown = false;
static bool capsLock = false;

static void keyboard_callback(interrupt_context_t *context)
{
	UNUSED(context);

	if (__inbyte(0x64) & 0x1)
	{
		uint8_t scancode = __inbyte(KB_PORT_DATA);
		bool keyup = !!(scancode & 0x80);

		scancode &= ~0x80;

		uint8_t keycode = shiftDown != capsLock ?
			ps2_layout.shifted[scancode] : ps2_layout.unshifted[scancode];

		if (keyup)
		{
			switch (keycode)
			{
			case KEY_SHIFT:
				shiftDown = false;
				break;
			case KEY_CAPSLOCK:
				capsLock = !capsLock;
				break;
			default:
				break;
			}
		}
		else
		{
			switch (keycode)
			{
			case KEY_SHIFT:
				shiftDown = true;
				break;
			default:
				if (keycode > KEY_SPACE && keycode < KEY_CTRL)
				{
					PrintChar((char)keycode);
				}
				break;
			}
		}
	}
}

void init_keyboard()
{
	register_interrupt_handler(KEYBOARD, (isr_t)&keyboard_callback);

	unmask_irq(KEYBOARD);
}
