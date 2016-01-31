#include "stdafx.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <limits>
#include <string>
#include <vector>
#define NOMINMAX // shut up windows.h min and max macros
#include <windows.h>
#include "collection.h"
#include "config.h"
#include "console.h"
#include "notesy.h"

// --- for debugging mem leaks ---
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


std::string get_current_directory()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring ws(buffer);
	std::string fullpath(ws.begin(), ws.end());
	std::string::size_type pos = fullpath.find_last_of("\\/");
	return fullpath.substr(0, pos);
}


/**
 * Entry point: gets command and launches
 */
int main(int argc, char *argv[]) {
	{
		// for debugging
		setCrtFlags();

		// get dir
		std::string current_path = get_current_directory();
		
		// collections / topics index
		std::string path = current_path + "\\index.ntsy";

		// --- console color changing -----------------------
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		change_console_color(10, &csbi);
		std::cout << std::endl;

		// --- test a collection ----------------------------


		// serialize
		auto col1 = new col::Collection("Animals", "ANI");
		auto col2 = new col::Collection("Todo List", "TDO");
		std::map<std::string, col::Collection> cols;
		cols[col1->get_name()] = *col1;
		cols[col2->get_name()] = *col2;
		col::save_all_collections(path, cols);
		delete col1;
		delete col2;

		// deserialize -> NOTE: in reality, we should load a map / hash table for better lookups...
		// guarantee uniqueness before serializing anyway...
	    
		col::list_all_collections(path);

		

		
		// --- test the commands ---------------------
		/*{
			auto ncmds = init_commands();
			NotesyCommand *c = ncmds[0];
			std::cout << c->get_name() << std::endl;
			std::cout << c->get_help() << std::endl;
			c->run_command(NULL);
			ncmds[1]->run_command(NULL);
			clean_up_commands(ncmds);
		}*/


		// --- MAIN PROCESS FLOW ---
		// look for config
		// if yes:
			// load config
			// lookup command 
			// if found: run it
			// if not: help display of commands
		// if no:
			// ask if they are new / want to init
			// if yes: init
			// if no: ask to make new config
				// if yes: 

		// reset old color
		reset_console_color(csbi);

		// --- for debugging ---
		// NOTE: this must be called after everything has gone out of scope
		// Vectors in main are tricky, as they'll be in scope until end of prg.
		// We can use blocks to test though :)
	}
	_CrtDumpMemoryLeaks();
	// ---------------------
	return 0;
}


/**
 * Intializes vector of notesy commands.
 * TODO: make this a map...
 */
std::vector<NotesyCommand *> init_commands()
{
	// keep these static, so we don't try to access local mem out of scope
	static NotesyCommand *nc = new NotesyCommand("test", "This is a test command", test);
	static NotesyCommand *nc2 = new NotesyCommand("second_test", "This is another test command", test);
	static NotesyCommand *list = 
		new NotesyCommand(
			"list",
			"Lists collections.\nIf collection is specified, lists topics.\nIf collection and topic is specific, lists notes.\n", 
			col::list_all_collections
		);

	// add
	// init
	// config
	// destroy (all notes, notesy dir, config, etc.)
	// if none, display help...
	std::vector<NotesyCommand *> cmds = { nc, nc2 };
	return cmds;
}


/**
 * Deallocates the commands.
 */
void clean_up_commands(std::vector<NotesyCommand *> cmds)
{
	for (size_t i = 0; i < cmds.size(); i++) {
		delete cmds[i];
	}
}


/**
 * Just a test.
 */
void test(std::string args[])
{
	std::cout << "TEST" << std::endl;
}


/**
 * Adds a new collection to our set.
 */
void add_collection(std::string name)
{
	// TODO: check if dir exists, if not create it (outside function)
	// TODO: check if index file exists, if not, create it (outside function)
	// TODO: save / serialize (by appending to file, shouldn't need to pull
}


/**
 * Creates config file for the first time.
 * Call from init command (if they specify)
 */
void init_config() {
	using std::cout;
	using std::endl;
	char dir_buf[200];

	// TODO: ridiculous ascii art for notesy!!!
	cout << "Welcome to notesy!" << endl;
	cout << "Please specify a directory for your notes: " << endl;
	std::cin.getline(dir_buf, sizeof(dir_buf));
	// TODO: trim the directory buffer?

	cout << "Checking directory: " << dir_buf << endl;
	// TODO: check dir
	cout << "Initializing notesy directory in: " << dir_buf << endl;

	// TODO: create notesy dir, collections index file
	cout << "Notesy directory created!" << endl;

	// color here or only on config? save for config, don't want to do too much here...


	// TODO: create and save config file
	// must point to notesy dir to read later
	// any other info? ultimately a username, password lock sort of deal?

}

// helper to create a new directory
bool create_directory(std::string path) {
	return true;
}

// create file, and possibly
bool create_file(std::string path) {
	return true;
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
