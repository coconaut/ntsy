#include "stdafx.h"
#include <ctime>
#include <iostream>
#include <windows.h>

#include <ostream>

namespace prt
{
	// --- constants ---
	const int CONSOLE_COLOR = 10; // 10 for green or 13 for pink on Windows -> SHOULD PULL FROM CONFIG...


	/**
	* A helper console printing function. Expand if needed.
	*/
	void printer(char *text) {
		HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);

		// remember old color
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsoleOut, &csbi);

		// write line	
		SetConsoleTextAttribute(hConsoleOut, CONSOLE_COLOR);
		std::cout << text << std::endl;

		// reset old color
		SetConsoleTextAttribute(hConsoleOut, csbi.wAttributes);
	}
}