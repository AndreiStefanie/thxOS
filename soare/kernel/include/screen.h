#pragma once
#include "boot.h"

#define MAX_LINES       25
#define MAX_COLUMNS     80
#define MAX_OFFSET      2000 //25 lines * 80 chars

#pragma pack(push, 1)
typedef struct _SCREEN
{
    char c;
    BYTE color;
}SCREEN, *PSCREEN;
#pragma pack(pop)

void screen_init(void);

void SetColor(BYTE Color);
void SetScreenColor(BYTE Color);
void ClearScreen(void);

void PutChar(char C, int Pos);
void PutCharAt(char C, int X, int Y);
void PutInt(int I, int Pos);
void PutString(const char* String, int Pos);
void PutStringLine(const char* String, int Line);

void PrintChar(char C);
void PrintChar_C(char C, BYTE Color);
void PrintString(const char *String);
void PrintString_C(const char *String, BYTE Color);
void PrintInt(int64_t I);
void PrintInt_C(int64_t I, BYTE Color);

void Welcome(void);
