#include "screen.h"

void incIndex(void);
int itoa_cust(int i, char *buf);

BYTE color = 0;
uint16 indexX = 0;
uint16 indexY = 0;

void PutChar(char C, int Pos)
{
	char *vidMem = (char *) 0xb8000;

	vidMem[Pos * 2] = C;
	vidMem[Pos * 2 + 1] = color;
}

void PutCharAt(char C, int X, int Y)
{
	PutChar(C, Y * MAX_COLUMNS + X);
}

void PrintChar(char C, BYTE Color)
{
	BYTE color_temp = color;
	SetColor(Color);
	PutCharAt(C, indexX, indexY);
	incIndex();
	SetColor(color_temp);
}

void incIndex()
{
	if (indexX == MAX_COLUMNS - 1)
	{
		indexX = 0;
		if (indexY == MAX_LINES - 1)
		{
			ClearScreen();
			indexY = 0;
		}
		else
			indexY++;
	}
	else
		indexX++;
}

void PutInt(int I, int Pos)
{
	char buf[10];
	int len = itoa_cust(I, buf);
	for (int i = 0; i < len; i++)
	{
		PutChar(buf[i], Pos + i);
	}
}

void SetColor(BYTE Color)
{
	color = Color;
}

void SetScreenColor(BYTE Color)
{
	int i = 0;
	char *vidMem = (char *)0xb8000;

	for (i = 0; i < MAX_OFFSET; i++)
	{
		vidMem[i * 2 + 1] = Color;
	}
}

void ClearScreen()
{
	int i = 0;
	BYTE color_temp = color;
	color = 0;

	for (i = 0; i < MAX_OFFSET; i++)
	{
		PutChar(0, i);
	}

	color = color_temp;
}

void PutString(char *String, int Pos)
{
	for (int i = 0; (String[i] != 0) && (i < MAX_OFFSET); i++)
	{
		PutChar(String[i], Pos + i);
	}
}

void PutStringLine(char *String, int Line)
{
	PutString(String, Line * MAX_COLUMNS);
}

void Welcome()
{
	int line = 1;
	
	PutStringLine("   _____                         ____   _____ ", line);
	PutStringLine("  / ____|                       / __ \\ / ____|", line + 1);
	PutStringLine(" | (___   ___   __ _ _ __ ___  | |  | | (___  ", line + 2);
	PutStringLine("  \\___ \\ / _ \\ / _` | '__/ _ \\ | |  | |\\___ \\ ", line + 3);
	PutStringLine("  ____) | (_) | (_| | | |  __/ | |__| |____) |", line + 4);
	PutStringLine(" |_____/ \\___/ \\__,_|_|  \\___|  \\____/|_____/ ", line + 5);
}

int itoa_cust(int i, char *buf) 
{
	char const digit[] = "0123456789";
	int shifter = 0;
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
