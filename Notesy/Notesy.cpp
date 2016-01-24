// Notesy.cpp : Defines the entry point for the console application.
// Ultimately, we should worry about syncing, search? passwords, config file...
// for now, just get the basics with hard coded functions

// TODO: command table w/ pointers, think of like shell app


#include "stdafx.h"
#include <iostream>
#include <windows.h>

// declarations (to be moved)
void printer(char *text);
void list_collections();
void list_topics(char *collection_name);
int init();


// constants
const int CONSOLE_COLOR = 10; // 10 for green or 13 for pink on Windows
const char *NOTESY_DIR = "C:\\";

/**
 * Entry point: gets command and launches
 */
int main() {
	printer("Hello");
	return 0;
}

int init() {

}

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

