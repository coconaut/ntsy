#pragma once
#include <windows.h>

void change_console_color(int color, CONSOLE_SCREEN_BUFFER_INFO *csbi);
void reset_console_color(CONSOLE_SCREEN_BUFFER_INFO csbi);

