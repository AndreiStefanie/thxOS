#pragma once

#define PIC1	 0x20
#define PIC1DATA 0x21
#define PIC2	 0xA0
#define PIC2DATA 0xA1

#define ICW1 0x11
#define ICW4 0x01

void init_pics(int pic1, int pic2);