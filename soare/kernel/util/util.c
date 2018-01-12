#include "util.h"
#include "boot.h"

void* set_mem(void* dest, char val, uint32_t len)
{
	char *temp = (char *)dest;

	for (; len != 0; len--)
	{
		*temp++ = val;
	}

	return dest;
}

int itoa_cust(int64_t i, char *buf)
{
	char const digit[] = "0123456789";
	int64_t shifter = 0;
	int digits = 0;

	if (i < 0)
	{
		*buf++ = '-';
		i *= -1;
	}

	shifter = i;
	do {
		buf++;
		shifter = shifter / 10;
		digits++;
	} while (shifter);

	*buf = '\0';

	do {
		*--buf = digit[i % 10];
		i = i / 10;
	} while (i);

	return digits;
}
