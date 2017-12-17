#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "boot.h"
#include <stdlib.h>

#define MAX_LINES       25
#define MAX_COLUMNS     80
#define MAX_OFFSET      2000 //25 lines * 80 chars

#pragma pack(push)
#pragma pack(1)
typedef struct _SCREEN
{
    char c;
    BYTE color;
}SCREEN, *PSCREEN;
#pragma pack(pop)

void SetColor(BYTE Color);
void SetScreenColor(BYTE Color);
void ClearScreen(void);
void PutChar(char C, int Pos);
void PutCharAt(char C, int X, int Y);
void PutInt(int I, int Pos);
void PrintChar(char C, BYTE Color);
void PutString(char* String, int Pos);
void PutStringLine(char* String, int Line);
void Welcome(void);

#endif // _SCREEN_H_