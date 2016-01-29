#include "stdafx.h"
#include "collection.h"
#include "notesy.h"
#include <iostream>
#include <vector>
#include <windows.h>

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
std::vector<NotesyCommand *> init_commands();
void list_collections();
void test(char *args[]);


// --- for debugging ---
void setCrtFlags();


// --- constants ---
const int CONSOLE_COLOR = 10;
const char *CONFIG_DIR = "";
const char *NOTESY_DIR = "D:\notesy_root";


/**
 * Entry point: gets command and launches
 */
int main() {
	// for debugging
	setCrtFlags();

	// --- console color changing -----------------------
	HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsoleOut, &csbi);
	SetConsoleTextAttribute(hConsoleOut, CONSOLE_COLOR);

	// --- test a collection ----------------------------------------------------
	auto col = new Collection("Test Collection");
	std::cout << "Test Collection I/O operator" << std::endl << *col << std::endl;
	delete col;


	// --- test the commands ---------------------
	{
		auto ncmds = init_commands();
		NotesyCommand *c = ncmds[0];
		std::cout << c->get_name() << std::endl;
		std::cout << c->get_help() << std::endl;
		c->run_command(NULL);
		delete c; // note: really we'll have to loop ncmds here and delete nc[i], etc.
	}

	// reset old color
	SetConsoleTextAttribute(hConsoleOut, csbi.wAttributes);

	// --- for debugging ---
	// NOTE: this must be called after everything has gone out of scope
	// Vectors in main are tricky, as they'll be in scope until end of prg.
	// We can use blocks to test though :)
	_CrtDumpMemoryLeaks();
	// ---------------------
	return 0;
}


/**
 * Intializes vector of notesy commands.
 */
std::vector<NotesyCommand *> init_commands()
{
	// keep these static, so we don't try to access local mem out of scope
	static NotesyCommand *nc = new NotesyCommand("Test", "This is a test command", test);
	std::vector<NotesyCommand *> cmds = {nc};
	return cmds;
}


// just a test
void test(char *args[])
{
	std::cout << "TEST" << std::endl;
}


/**
 * Adds a new collection to our set.
 */
void add_collection(char *name)
{
	Collection collection(name);
	// TODO: save / serialize



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
