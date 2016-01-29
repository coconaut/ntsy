#include "stdafx.h"
#include <iostream>
#include "notesy.h"
#include "collection.h"
#include "printing.h"

// --- for debugging mem leaks ---
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// -------------------------------


/* TODO:
 - think about serialization format (go for speed, efficiency, simplicity)
	- for now, text
 - think about interactive typing of note (vs. one shot)
	- you know, type until Ctrl + D or something
 - think about loading / unloading of objs, I guess this goes with serialization though...
 */


// --- declarations (to be moved) ---
NotesyCommand **init_commands();
void list_collections();
void test(char *args[]);


// --- for debugging ---
void setCrtFlags();
// ---------------------


// --- constants ---
//const int CONSOLE_COLOR = 10; // 10 for green or 13 for pink on Windows
const char *CONFIG_DIR = "";
const char *NOTESY_DIR = "D:\notesy_root";


/**
 * Entry point: gets command and launches
 */
int main() {
	// --- for debugging ---
	setCrtFlags();
	// ---------------------

	// TODO: do console logic here, rather than repeat each print, and reset before exit!!!
	// this will allow us to eliminate printing function and stick with ostream

	auto col = new Collection("Vikas Collection");
	std::cout << "Test Collection I/O operator" << std::endl << *col << std::endl;
	delete col;

	NotesyCommand **ncmds = init_commands();
	NotesyCommand *c = ncmds[0];
	prt::printer(c->get_name());
	prt::printer(c->get_help());
	c->run_command(NULL);
	delete c; // note: really we'll have to loop nc here and delete nc[i], etc.

	// --- for debugging ---
	_CrtDumpMemoryLeaks();
	// ---------------------
	return 0;
}

/**
 * Intializes list of notesy commands.
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
	std::cout << "TEST" << std::endl;
}


/**
 * Creates config file for the first time.
 */
void init_config(char *notesy_dir) {
	// create the config file
	// save in user home
	// must point to notesy dir to read later
	// any other info? ultimately a username, password lock sort of deal?

}


/**
 * Load up config.
 */
void load_config() {
	// TODO: use the config dir, load up some object
	// if not present, error, and tell the user to launch init config "notesy dir" to create one
}


/**
* Sets necessary flags for debugging memory leaks.
*/
void setCrtFlags()
{
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
}
