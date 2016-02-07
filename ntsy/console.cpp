#include "stdafx.h"
#include <windows.h>
#include "console.h"
#include <iostream>

void change_console_color(int color, CONSOLE_SCREEN_BUFFER_INFO *csbi) {
	HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, csbi);
	SetConsoleTextAttribute(hConsoleOut, color);
}

void reset_console_color(CONSOLE_SCREEN_BUFFER_INFO &csbi)
{
	HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsoleOut, csbi.wAttributes);
}

void scroll_to_top(CONSOLE_SCREEN_BUFFER_INFO &csbi)
{
	COORD topLeft = { 0, 0 };
	HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written;
	GetConsoleScreenBufferInfo(hConsoleOut, &csbi);

	FillConsoleOutputCharacterA(hConsoleOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, topLeft, &written);
	FillConsoleOutputAttribute(hConsoleOut, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		csbi.dwSize.X * csbi.dwSize.Y, topLeft, &written);

	SetConsoleCursorPosition(hConsoleOut, topLeft);

}
