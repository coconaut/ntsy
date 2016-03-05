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
#include "editor.h"
#include "note.h"
#include "ntsy.h"
#include "text.h"



// --- for debugging mem leaks ---
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


/**
 * Entry point: gets command and launches
 */
int main(int argc, char *argv[]) {
	{
		// for debugging
		setCrtFlags();

		// load config
		NtsyConfig config;

		// --- console color changing -----------------------
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		change_console_color(config.get_console_color(), &csbi);
		std::cout << std::endl;

		try
		{			
			// load commands
			cmd::cmd_map_t cmds = cmd::init_commands(&config);

			// lookup and run command
			if (argc > 1 && cmd::has_command(cmds, argv[1])) {
				std::vector<std::string> sargs;
				bool isHelp = false;
				for (int i = 1; i < argc; i++) {
					auto s = std::string(argv[i]);
					if (s == "-h" || s == "-help") {
						isHelp = true;
						break;
					}
					sargs.push_back(s);
				}
				if (isHelp || !cmds[argv[1]]->run_command(sargs)) {
					cmds[argv[1]]->pretty_print_usage();
				}
			}
			else {
				cmd::show_descriptions(cmds);
			}

		
		// clean up commands
		cmd::clean_up_commands(cmds);
		}
		catch (...)
		{
			// reset old color
			reset_console_color(csbi);
			throw;
		}

		// reset old color
		reset_console_color(csbi);

	}
	_CrtDumpMemoryLeaks();
	// ---------------------
	return 0;
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
	cout << "Welcome to ntsy!" << endl;
	cout << "Please specify a directory for your notes: " << endl;
	std::cin.getline(dir_buf, sizeof(dir_buf));
	// TODO: trim the directory buffer?

	cout << "Checking directory: " << dir_buf << endl;
	// TODO: check dir
	cout << "Initializing notesy directory in: " << dir_buf << endl;

	// TODO: create notesy dir, collections index file
	cout << "ntsy directory created!" << endl;

	// color here or only on config? save for config, don't want to do too much here...


	// TODO: create and save config file
	// must point to notesy dir to read later
	// any other info? ultimately a username, password lock sort of deal?

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
