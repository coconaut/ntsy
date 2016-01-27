
#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "Notesy.h"

// declarations (to be moved)
// really, should separate out collections, topics, and notes
void printer(char *text);
void list_collections();
void test(char *args[]);
NotesyCommand **init_commands();


// constants
const int CONSOLE_COLOR = 10; // 10 for green or 13 for pink on Windows
const char *CONFIG_DIR = "";
const char *NOTESY_DIR = "D:\notesy_root";


/**
 * Entry point: gets command and launches
 */
int main() {
	printer("Hello");
	// init commands (return pointer)
	// get config...
	// listen to command
	// try to look up and execute...
	// if not valid, display help
	// eventually allow help w/ args to specify which command...
	// clean up
	NotesyCommand **ncmds = init_commands();
	NotesyCommand *c = ncmds[0];
	printer(c->get_name());
	printer(c->get_help());
	delete c; // note: really we'll have to loop nc here and delete nc[i], etc.

	return 0;
}


/**
 * intializes list of notesy commands
 */
NotesyCommand **init_commands()
{
	// keep these static, so we don't try to access local mem out of scope
	static NotesyCommand *nc = new NotesyCommand("Test", "This is a test command", test);
	static NotesyCommand *cmds[] = { 
		nc
	};
	return cmds;
}


// just a test
void test(char *args[])
{
	printer("TEST");
}


/**
 * Load up config;
 */
void init_config() {
	// TODO: use the config dir, load up some object
	// really should return a config obj, not void, but todo
}


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

