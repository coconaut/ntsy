#include "stdafx.h"
#include <windows.h>
#include "console.h"


void change_console_color(int color, CONSOLE_SCREEN_BUFFER_INFO *csbi) {
	HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, csbi);
	SetConsoleTextAttribute(hConsoleOut, color);
}

void reset_console_color(CONSOLE_SCREEN_BUFFER_INFO csbi)
{
	HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleOut, csbi.wAttributes);
}
