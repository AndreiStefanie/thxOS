#include "util.h"
#include "boot.h"

void* set_mem(void* dest, char val, uint32 len)
{
	char *temp = (char *)dest;

	for (; len != 0; len--)
	{
		*temp++ = val;
	}
		
	return dest;
}
