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

		// load config

		// --- console color changing -----------------------
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		change_console_color(10, &csbi);
		std::cout << std::endl;

		//args
		// 0 is ntsy, 1 is command, rest are args...

		// get dir
		std::string current_path = get_current_directory();
		
		

		// collections / topics index (will pull from config)
		std::string path = current_path + "\\index.ntsy";

		// load commands
		cmd::cmd_map_t cmds = cmd::init_commands();

		// lookup and run command
		if (argc > 1 && cmd::has_command(cmds, argv[1])) {
			// TODO: pass config object as well...
			std::vector<std::string> sargs;
			for (int i = 1; i < argc; i++) {
				sargs.push_back(std::string(argv[i]));
			}
			if (!cmds[argv[1]]->run_command(sargs, path)) // todo show usage if failed??? here or inside?
				cmds[argv[1]]->pretty_print();
		}
		else
			cmd::show_help(cmds);

		

		// --- test a collection ----------------------------


		// serialize
		/*auto col1 = new col::Collection("Animals", "ANI");
		auto col2 = new col::Collection("Todo List", "TDO");
		std::map<std::string, col::Collection> cols;
		cols[col1->get_name()] = *col1;
		cols[col2->get_name()] = *col2;
		col::save_all_collections(path, cols);
		delete col1;
		delete col2;*/


		// clean up commands
		cmd::clean_up_commands(cmds);

		// reset old color
		reset_console_color(csbi);

	}
	_CrtDumpMemoryLeaks();
	// ---------------------
	return 0;
}


/**
 * Just a test.
 */
void test(std::string args[])
{
	std::cout << "TEST" << std::endl;
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
