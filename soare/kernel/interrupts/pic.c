#include "pic.h"
#include "isr.h"

void init_pics(int pic1, int pic2)
{
	unsigned char temp1, temp2;
	temp1 = __inbyte(PIC1DATA);
	temp2 = __inbyte(PIC2DATA);
	
	// send ICW1
	__outbyte(PIC1, ICW1);
	__outbyte(PIC2, ICW1);

	// send ICW2
	__outbyte(PIC1DATA, pic1);
	__outbyte(PIC2DATA, pic2);

	__outbyte(PIC1DATA, 0x04);	// inform master about slave at 0100b
	__outbyte(PIC2DATA, 0x02);

	// send ICW4
	__outbyte(PIC1DATA, ICW4);
	__outbyte(PIC2DATA, ICW4);

	// restore masks
	__outbyte(PIC1DATA, temp1);
	__outbyte(PIC2DATA, temp2);

	mask_irq(ALL);
}
