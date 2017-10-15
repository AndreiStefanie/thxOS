#include "boot.h"
#include "screen.h"

void EntryPoint(void)
{
    //__magic();                  // break into BOCHS
	PutStringLine("Boot OK!", 4);

	PutStringLine("   _____                         ____   _____ ", 5);
	PutStringLine("  / ____|                       / __ \\ / ____|", 6);
	PutStringLine(" | (___   ___   __ _ _ __ ___  | |  | | (___  ", 7);
	PutStringLine("  \\___ \\ / _ \\ / _` | '__/ _ \\ | |  | |\\___ \\ ", 8);
	PutStringLine("  ____) | (_) | (_| | | |  __/ | |__| |____) |", 9);
	PutStringLine(" |_____/ \\___/ \\__,_|_|  \\___|  \\____/|_____/ ", 10);
	
	SetColor(10);
}
