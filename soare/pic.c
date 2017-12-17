#define PIC1 0x20
#define PIC2 0xA0

#define ICW1 0x11
#define ICW4 0x01

void init_pics(int pic1, int pic2)
{
	/* send ICW1 */
	__outbyte(PIC1, ICW1);
	__outbyte(PIC2, ICW1);

	/* send ICW2 */
	__outbyte(PIC1 + 1, pic1);
	__outbyte(PIC2 + 1, pic2);

	__outbyte(PIC1 + 1, 4);	/* IRQ2 -> connection to slave */
	__outbyte(PIC2 + 1, 2);

	/* send ICW4 */
	__outbyte(PIC1 + 1, ICW4);
	__outbyte(PIC2 + 1, ICW4);

	/* disable all IRQs */
	__outbyte(PIC1 + 1, 0xFF);
}