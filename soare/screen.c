#include "screen.h"

volatile BYTE color = 10;

void HelloBoot()
{
	int i, len;
	char boot[] = "Boot OK!\0";
	color = 10;

	len = 0;
	while (boot[len] != 0)
	{
		len++;
	}

	PutChar('a', 100);

	for (i = 0; (i < len) && (i < MAX_OFFSET); i++)
	{
		PutChar('a', i + 100);
	}
}

void SetColor(BYTE Color)
{
	color = Color;
}

void SetScreenColor(BYTE Color)
{
	int i = 0;
	PSCREEN vidMem = (PSCREEN)(0x000B8000);

	for (i = 0; i < MAX_OFFSET; i++)
	{
		vidMem[i].color = Color;
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

void PutChar(char C, int Pos)
{
	PSCREEN vidMem = (PSCREEN)(0x000B8000);
	vidMem[Pos].color = color;
	vidMem[Pos].c = C;
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

void welcome()
{
	PutStringLine("   _____                         ____   _____ ", 5);
	PutStringLine("  / ____|                       / __ \\ / ____|", 6);
	PutStringLine(" | (___   ___   __ _ _ __ ___  | |  | | (___  ", 7);
	PutStringLine("  \\___ \\ / _ \\ / _` | '__/ _ \\ | |  | |\\___ \\ ", 8);
	PutStringLine("  ____) | (_) | (_| | | |  __/ | |__| |____) |", 9);
	PutStringLine(" |_____/ \\___/ \\__,_|_|  \\___|  \\____/|_____/ ", 10);
}
