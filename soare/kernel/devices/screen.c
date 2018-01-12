#include "screen.h"
#include "util.h"
#include "boot.h"

void incIndex(void);
void incIndexY(void);

char *vidMem = NULL;
BYTE color = 0;
uint8_t indexX = 0;
uint8_t indexY = 0;

void PutChar(char C, int Pos)
{
	if ('\n' == C)
	{
		incIndexY();
		indexX = 0;
	}
	else
	{
		vidMem[Pos * 2] = C;
		vidMem[Pos * 2 + 1] = color;

		incIndex();
	}
}

void PutCharAt(char C, int X, int Y)
{
	PutChar(C, Y * MAX_COLUMNS + X);
}

void incIndex()
{

	/*if (indexX == MAX_COLUMNS - 1)
	{
		__magic();
		indexX = 0;
		incIndexY();
	}
	else
	{*/
	indexX++;
	//}
}

void incIndexY()
{
	if (indexY == MAX_LINES - 1)
	{
		ClearScreen();
		indexY = 0;
	}
	else
	{
		indexY++;
	}
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

	indexX = 0;
	indexY = 0;

	color = color_temp;
}

void PutString(const char *String, int Pos)
{
	for (int i = 0; (String[i] != 0) && (i < MAX_OFFSET); i++)
	{
		PutChar(String[i], Pos + i);
	}
}

void PutStringLine(const char *String, int Line)
{
	PutString(String, Line * MAX_COLUMNS);
}

void PrintChar_C(char C, BYTE Color)
{
	BYTE color_temp = color;
	SetColor(Color);
	PutCharAt(C, indexX, indexY);
	SetColor(color_temp);
}

void PrintChar(char C)
{
	PrintChar_C(C, color);
}

void PrintString_C(const char *String, BYTE Color)
{
	for (size_t i = 0; String[i] != 0; i++)
	{
		PrintChar_C(String[i], Color);
	}
}

void PrintString(const char *String)
{
	PrintString_C(String, color);
}

void PrintInt_C(int64_t I, BYTE Color)
{
	char buf[10];

	itoa_cust(I, buf);
	PrintString_C(buf, Color);
}

void PrintInt(int64_t I)
{
	PrintInt_C(I, color);
}

void screen_init()
{
	vidMem = (char *)0xb8000;
	ClearScreen();
	SetColor(VGA_WHITE);
}

void Welcome()
{
	incIndexY();

	PrintString("   _____                         ____   _____ \n");
	PrintString("  / ____|                       / __ \\ / ____|\n");
	PrintString(" | (___   ___   __ _ _ __ ___  | |  | | (___  \n");
	PrintString("  \\___ \\ / _ \\ / _` | '__/ _ \\ | |  | |\\___ \\ \n");
	PrintString("  ____) | (_) | (_| | | |  __/ | |__| |____) |\n");
	PrintString(" |_____/ \\___/ \\__,_|_|  \\___|  \\____/|_____/ \n");
}
