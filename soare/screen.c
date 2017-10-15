#include "screen.h"

static PSCREEN gVideo = (PSCREEN)(0x000B8000);
int index = 0;
BYTE color = 10;

void HelloBoot()
{
	int i, len;
	char boot[] = "Boot OK!";

	len = 0;
	while (boot[len] != 0)
	{
		len++;
	}

	for (i = 0; (i < len) && (i < MAX_OFFSET); i++)
	{
		gVideo[i].color = 10;
		gVideo[i].c = boot[i];
	}
}

void SetColor(BYTE Color)
{
	color = Color;
	int i = 0;

	for (i = 0; i < MAX_OFFSET; i++)
	{
		gVideo[i].color = Color;
	}
}

void ClearScreen()
{
	int i = 0;

	for (i = 0; i < MAX_OFFSET; i++)
	{
		gVideo[i].color = 0;
		gVideo[i].c = 0;
	}
}

void PutChar(char C, int Pos)
{
	gVideo[Pos].color = color;
	gVideo[Pos].c = C;
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

