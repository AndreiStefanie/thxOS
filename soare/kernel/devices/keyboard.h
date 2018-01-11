#pragma once
#include "../defs.h"

enum keycode
{
	KEY_BACKSPACE = 0x08,
	KEY_TAB = 0x09,
	KEY_ENTER = 0x0d,
	KEY_ESCAPE = 0x1b,
	KEY_SPACE = 0x20,
	KEY_CTRL = 0x81,
	KEY_SHIFT = 0x82,
	KEY_ALT = 0x83,
	KEY_PRTSCR = 0x90,
	KEY_CAPSLOCK = 0x91,
	KEY_NUMLOCK = 0x92,
	KEY_SCRLOCK = 0x93,
	KEY_INSERT = 0xa0,
	KEY_END = 0xa1,
	KEY_DOWN = 0xa2,
	KEY_PGDN = 0xa3,
	KEY_LEFT = 0xa4,
	KEY_CENTER = 0xa5,
	KEY_RIGHT = 0xa6,
	KEY_HOME = 0xa7,
	KEY_UP = 0xa8,
	KEY_PGUP = 0xa9,
	KEY_DEL = 0xaa,
	KEY_MINUS = 0xab,
	KEY_PLUS = 0xac,
	KEY_F1 = 0xb0,
	KEY_F2 = 0xb1,
	KEY_F3 = 0xb2,
	KEY_F4 = 0xb3,
	KEY_F5 = 0xb4,
	KEY_F6 = 0xb5,
	KEY_F7 = 0xb6,
	KEY_F8 = 0xb7,
	KEY_F9 = 0xb8,
	KEY_F10 = 0xb9,
	KEY_F11 = 0xba,
	KEY_F12 = 0xbb,
	KEY_SCANESC = 0xfe,
	KEY_INVALID = 0xff,
};

typedef enum keycode keycode_t;

#define BSP KEY_BACKSPACE
#define TAB KEY_TAB
#define ENT KEY_ENTER
#define ESC KEY_ESCAPE
#define CTL KEY_CTRL
#define SHF KEY_SHIFT
#define ALT KEY_ALT
#define PSC KEY_PRTSCR
#define CLK KEY_CAPSLOCK
#define NLK KEY_NUMLOCK
#define SLK KEY_SCRLOCK
#define KIN KEY_INSERT
#define KEN KEY_END
#define KDN KEY_DOWN
#define KPD KEY_PGDN
#define KLT KEY_LEFT
#define KCT KEY_CENTER
#define KRT KEY_RIGHT
#define KHM KEY_HOME
#define KUP KEY_UP
#define KPU KEY_PGUP
#define KDL KEY_DEL
#define KMI KEY_MINUS
#define KPL KEY_PLUS
#define F_1 KEY_F1
#define F_2 KEY_F2
#define F_3 KEY_F3
#define F_4 KEY_F4
#define F_5 KEY_F5
#define F_6 KEY_F6
#define F_7 KEY_F7
#define F_8 KEY_F8
#define F_9 KEY_F9
#define F10 KEY_F10
#define F11 KEY_F11
#define F12 KEY_F12
#define SES KEY_SCANESC
#define INV KEY_INVALID
#define APO '\''
#define BSL '\\'

struct keylayout
{
	uint8 shifted[128];
	uint8 unshifted[128];
};

typedef struct keylayout keylayout_t;

void init_keyboard(void);
