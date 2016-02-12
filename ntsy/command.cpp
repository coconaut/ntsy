#include "stdafx.h"
#include <conio.h>
#include <iomanip>
#include <map>
#include <string>
#include "command.h"
#include "collection.h"
#include "note.h"
#include "text.h"
#include "console.h"


namespace cmd {

	// --- constants ----------------
	const char *NTSY_EXT = ".ntsy";

	/**
	 * NtsyCommand constructor.
	 */
	NtsyCommand::NtsyCommand(std::string name, std::string desc, std::string usage, cmd_t *cmd)
	{
		m_name = name;
		m_desc = desc;
		m_usage = usage;
		m_command = cmd;
	}


	/**
	 * Prints command help / basic info.
	 */
	void NtsyCommand::pretty_print_desc() {
		std::cout
			<< std::left
			<< "   "
			<< std::setw(11)
			<< m_name
			<< m_desc
			<< std::endl;
	}


	/**
	* Prints more detailed command usage.
	*/
	void NtsyCommand::pretty_print_usage() {
		std::cout
			<< std::left
			<< "usage: ntsy "
			<< m_name
			<< " "
			<< m_usage
			<< std::endl;
	}

	/**
	 * Intializes map of notesy commands.
	 */
	cmd_map_t init_commands()
	{
		cmd_map_t cmds;
		if (cmds.empty())
		{
			cmds["ls"] = new NtsyCommand("ls", "Lists collections. If collection is specified, lists notes.", "[<abbr.>]", cmd_list);
			cmds["col"] = new NtsyCommand("col", "Adds a new collection.","<name> <abbr.>", cmd_col);
			cmds["rm"] = new NtsyCommand("rm", "Removes a collection.", "<abbr.>", cmd_rm);
			cmds["jot"] = new NtsyCommand("jot", "Adds a note to a collection", "<abbr.> <text>", cmd_jot);
			cmds["open"] = new NtsyCommand("open", "Opens a collection in interactive mode.", "<abbr.>", cmd_open);
		}

		return cmds;

		// init (welcome message, brief help info, set up dir)
		// config (change dir, maybe colors..., show dates, etc.)
		// rename <abbr.> <new name>
		// reabbr <abbr.> <new abbr.>
		
		// launch <num> (sub command...)
		// destroy (all notes, notesy dir, config, etc.)
	}


	/**
	* Deallocates the commands.
	*/
	void clean_up_commands(cmd_map_t &cmds)
	{
		for (const auto &iter : cmds) {
			delete iter.second;
		}
	}


	/**
	* Adds a new collection to our set.
	* Args should be path, name, abbr.
	* Returns bool of whether this command was executed,
	* regardless of success.
	*/
	bool cmd_col(std::vector<std::string> args, std::string path)
	{
		// TODO: check if dir exists, if not error (tell to call init/ config)
		// TODO: check if index file exists, if not, error (tell to call init / config)
		// TODO: clean chars

		if (args.size() < 3)
			return false;

		remove_newlines(args[1]);
		remove_whitespace(args[2]);

		if (args[1].empty() || args[1].compare("") == 0) {
			std::cout << "Oh no!!! Name cannot be empty :[" << std::endl;
			return false;
		}

		if (args[1].length() > col::MAXLENGTH) {
			std::cout << "Oh no!!! Name must be less than " << std::to_string(col::MAXLENGTH) << " characters :[" << std::endl;
			return false;
		}

		if (args[2].empty() || args[2].length() != 3) {
			std::cout << "Oh no!!! Abbreviation must be exactly 3 characters. :[" << std::endl;
			return false;
		}

		// retrieve collections, add, and show
		auto cols = col::get_all_collections(path);
		if (col::add_collection(cols, path, args[1], args[2]))
			col::list_all_collections(cols);
		else
			std::cout << "Oh noooo!!! Unable to add collection. Please make sure abbreviation is unique!!! :[" << std::endl;

		return true;
	}


	/**
	 * Removes a collection by abbreviation.
	 * Args[1] is expected to be the abbr.
	 * Returns bool of whether this command was executed,
	 * regardless of success.
	 */
	bool cmd_rm(std::vector<std::string> args, std::string path) {
		if (args.size() < 2) {
			return false;
		}
		
		auto cols = col::get_all_collections(path);
		if (col::remove_collection(cols, path, args[1]))
			col::list_all_collections(cols);
		else
			std::cout << "Oh no noooo!!! Unable to erase collection with the abbreviation: " << args[1] << "!!! :[" << std::endl;

		return true;
	}


	/**
	 * Lists all collections.
	 * If abbr. is passed, lists notes under that collection instead.
	 */
	bool cmd_list(std::vector<std::string> args, std::string path)
	{
		// check for collection abbr.
		if (args.size() > 1) {
			
			// get notes
			std::string note_path = get_note_file_name(args[1]);
			auto notes = note::get_all_notes(note_path);

			// display if any notes, error msg otherwise
			if (!handle_no_notes(notes, args[1])) 
				note::list_all_notes(note_path);
		}
		else 
			col::list_all_collections(path);
		
		return true;
	}


	/**
	 * Adds a note to a collection.
	 * Expects args[1] to be the abbreviation/
	 * Expects args[2] to be the text of the note.
	 * of the collection to add to.
	 * Verify the collection.
	 * Get file path (for now, just abbr (MUST CLEAN), but later
	 * may serialize a guid with the collection obj.
	 */
	bool cmd_jot(std::vector<std::string> args, std::string path)
	{
		if (args.size() < 3)
			return false;

		auto cols = col::get_all_collections(path);
		if (!handle_bad_collection(cols, args[1])) {
			// add n to file (using rel for now...)
			std::string note_file_path = get_note_file_name(args[1]);
			if (note::add_note(args[2], note_file_path))
			{
				// update date modified
				time_t t;
				time(&t);
				cols[args[1]].set_date_modified(t);

				// save collections
				col::save_all_collections(path, cols);
				std::cout << "Note added!!!" << std::endl;
			}
			else
				std::cout << "Oh noo!!! Unable to add note to collection. Save failed!!! :(((" << std::endl;
		}
		return true;
	}


	/**
	 * Opens a collection for interactive mode. 
	 * Expects args[1] to be collection abbreviation.
	 * Starts interactive note loop.
	 */
	bool cmd_open(std::vector<std::string> args, std::string path)
	{
		// check args
		if (args.size() < 2)
			return false;

		// get collection, if valid
		auto cols = col::get_all_collections(path);
		if (!handle_bad_collection(cols, args[1]))
		{
			std::string note_path = get_note_file_name(args[1]);
			auto notes = note::get_all_notes(note_path);
			loop_interactive(notes, cols[args[1]].get_name(), note_path);
		}
			
		return true;
	}


	void loop_interactive(std::vector<note::Note> &notes, std::string col_name, std::string note_path)
	{
		// TODO: massive refactor for sub commands...
		std::string sub_cmd = "";
		
		while (true) {
			std::cout << col_name << std::endl << "---" << std::endl << std::endl;
			note::list_all_notes(notes);
			skip_some_lines(5);
			print_instructions("Enter a sub-command and note #. Enter 'help' for help. Enter 'exit' to quit.");
			std::cin >> sub_cmd;

			// TODO: convert to smarted map w/function pointers, like cmd table?
			if (sub_cmd == "read")
				sub_cmd_read(notes);
			else if (sub_cmd == "help")
				sub_cmd_help();
			else if (sub_cmd == "edit")
				sub_cmd_edit();
			else if (sub_cmd == "rm")
				sub_cmd_rm(note_path, notes);
			else if (sub_cmd == "add")
				sub_cmd_add();
			else if (sub_cmd == "exit")
				break;
			else {
				print_instructions("Sub-command not found!!! :[[[");
				wait_for_continue(5);
			}
		}
	}


	void sub_cmd_read(std::vector<note::Note> &notes)
	{
		std::string id;
		std::cin >> id;
		int noteId = note::parseNoteId(id);
		skip_some_lines(5);
		note::read_note(notes, noteId);
		std::cout << std::endl;
		wait_for_continue(5);
	}

	void sub_cmd_help()
	{
		skip_some_lines(5);
		print_instructions("Available commands:\n\n"
			"add     - usage: add         - Adds a new note.\n"
			"read    - usage: read <#>    - Reads full note text.\n"
			"edit    - usage: edit <#>    - Opens note for editing.\n"
			"rm      - usage: rm <#>      - Deletes a note");
		wait_for_continue(5);
	}

	void sub_cmd_edit()
	{
		// TODO: need to check for and launch external editor
		// TODO: save, save collection modified date, redisplay
		print_instructions("todo!!!");
		wait_for_continue(5);
	}

	void sub_cmd_rm(std::string path, std::vector<note::Note> &notes)
	{
		std::string id;
		std::cin >> id;
		int noteId = note::parseNoteId(id);
		if (note::remove_note(path, notes, noteId - 1))
		{
			// note: don't update collection -> really uneccesarry overhead just for a date
			// that date will only refer to editing the collection meta info itself...
			std::cout << "Note removed!!!" << std::endl;
		}
		wait_for_continue(5);
	}

	void sub_cmd_add()
	{
		// TODO: need to launch external editor
		// TODO: add a note, save, save collection modified date, redisplay
		print_instructions("todo!!!");
		wait_for_continue(5);
	}

	void wait_for_continue(size_t num_lines)
	{
		print_instructions("Press any key to continue...");
		while (!_kbhit()) {}
		skip_some_lines(num_lines);
	}

	void print_instructions(char *instructions)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		change_console_color(11, &csbi);
		std::cout << instructions << std::endl << std::endl;
		reset_console_color(csbi);
	}

	void skip_some_lines(size_t num_lines)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		change_console_color(8, &csbi);
		std::string lines = "";
		for (size_t i = 0; i < num_lines; i++)
		{
			if (i % 2 == 0)
				lines += ".";
			lines += "\n";
		}
		std::cout << lines << std::endl;
		reset_console_color(csbi);
	}


	/**
	 * Loops command map and pretty prints.
	 */
	void show_descriptions(cmd_map_t &cmds) {
		for (const auto &iter : cmds) {
			iter.second->pretty_print_desc();
		}
	}


	/**
	 * Checks if command is a key of the command map.
	 */
	bool has_command(cmd_map_t &cmds, std::string cmd_name) {
		return (cmds.find(cmd_name) != cmds.end());
	}

	/**
	 * Forms a ntsy file name from abbr.
	 */
	std::string get_note_file_name(std::string &abbr)
	{
		return abbr + NTSY_EXT;
	}


	/**
	 * Deals with error message when we find no notes.
	 */
	bool handle_no_notes(std::vector<note::Note> &notes, std::string &abbr)
	{
		// if no notes, make sure that was a valid collection, otherwise display
		if (notes.size() == 0) {
			std::cout << "No notes found for " + abbr + "."
				<< std::endl
				<< "If this is a valid collection, add some notes!!! (^.^)"
				<< std::endl;
			return true;
		}
		return false;
	}

	bool handle_bad_collection(col::col_map_t cols, std::string abbr)
	{
		bool has_col = false;
		if (!col::has_collection(cols, abbr)) {
			has_col = true;
			std::cout << "Oh no!!! Unable to find collection with abbreviation: " + abbr << "!!! :[" << std::endl;
		}
		return has_col;
	}
}