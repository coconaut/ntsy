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
#include "editor.h"

namespace cmd {


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
	cmd_map_t init_commands(NtsyConfig *config)
	{
		cmd_map_t cmds;
		if (cmds.empty())
		{
			cmds["ls"] = new NtsyCommand("ls", "Lists collections. If collection is specified, lists notes.", "[<abbr.>]", cmd_list, config);
			cmds["col"] = new NtsyCommand("col", "Adds a new collection.","<name> <abbr.>", cmd_col, config);
			cmds["rm"] = new NtsyCommand("rm", "Removes a collection.", "<abbr.>", cmd_rm, config);
			cmds["jot"] = new NtsyCommand("jot", "Adds a note to a collection", "<abbr.> <text>", cmd_jot, config);
			cmds["open"] = new NtsyCommand("open", "Opens a collection in interactive mode.", "<abbr.>", cmd_open, config);
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
	bool cmd_col(std::vector<std::string> args, NtsyConfig *config)
	{
		// TODO: check if dir exists, if not error (tell to call init/ config)
		// TODO: check if index file exists, if not, error (tell to call init / config)
		// TODO: clean chars

		if (args.size() < 3)
			return false;

		txt::remove_newlines(args[1]);
		txt::trim(args[1]);

		txt::remove_whitespace(args[2]);
		txt::trim(args[2]);

		if (args[1].empty() || args[1].compare("") == 0) {
			std::cout << "Oh no!!! Name cannot be empty :[" << std::endl;
			return false;
		}

		if (args[1].length() > txt::MAINCOLSIZE) {
			std::cout << "Oh no!!! Name must be less than " << std::to_string(txt::MAINCOLSIZE) << " characters :[" << std::endl;
			return false;
		}

		if (!txt::is_alphanumeric(args[2])) {
			std::cout << "Oh no!!! Abbreviation must be alphanumeric!!!" << std::endl;
			return false;
		}

		if (args[2].empty() || args[2].length() > 3) {
			std::cout << "Oh no!!! Abbreviation must be 3 characters. :[" << std::endl;
			return false;
		}

		// retrieve collections, add, and show
		auto path = config->get_index_path();
		auto cols = col::get_all_collections(path);
		if (col::add_collection(cols, path, args[1], args[2]))
			col::list_all_collections(cols, config);
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
	bool cmd_rm(std::vector<std::string> args, NtsyConfig *config) {
		if (args.size() < 2) {
			return false;
		}
		
		std::string path = config->get_index_path();
		auto cols = col::get_all_collections(path);
		if (col::remove_collection(cols, path, args[1]))
			col::list_all_collections(cols, config);
		else
			std::cout << "Oh no noooo!!! Unable to erase collection with the abbreviation: " << args[1] << "!!! :[" << std::endl;

		return true;
	}


	/**
	 * Lists all collections.
	 * If abbr. is passed, lists notes under that collection instead.
	 */
	bool cmd_list(std::vector<std::string> args, NtsyConfig *config)
	{
		// check for collection abbr.
		if (args.size() > 1) {
			
			// get notes
			std::string note_path = get_note_path(args[1], config);
			auto notes = note::get_all_notes(note_path);

			// display if any notes, error msg otherwise
			if (!handle_no_notes(notes, args[1])) 
				note::list_all_notes(note_path, config);
		}
		else 
			col::list_all_collections(config->get_index_path(), config);
		
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
	bool cmd_jot(std::vector<std::string> args, NtsyConfig *config)
	{
		if (args.size() < 3)
			return false;

		auto cols = col::get_all_collections(config->get_index_path());
		if (!handle_bad_collection(cols, args[1])) {
			// add n to file (using rel for now...)
			std::string note_file_path = get_note_path(args[1], config);
			if (note::add_note(args[2], note_file_path))
			{
				// update date modified
				time_t t;
				time(&t);
				cols[args[1]].set_date_modified(t);

				// save collections
				col::save_all_collections(config->get_index_path(), cols);
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
	bool cmd_open(std::vector<std::string> args, NtsyConfig *config)
	{
		// check args
		if (args.size() < 2)
			return false;

		// get collection, if valid
		auto cols = col::get_all_collections(config->get_index_path());
		if (!handle_bad_collection(cols, args[1]))
		{
			std::string note_path = get_note_path(args[1], config);
			auto notes = note::get_all_notes(note_path);
			loop_interactive(notes, cols[args[1]].get_name(), note_path, config);
		}
			
		return true;
	}


	void loop_interactive(std::vector<note::Note> &notes, std::string col_name, std::string note_path, NtsyConfig *config)
	{
		std::string sub_cmd = "";
		
		while (true) {
			std::cout << col_name << std::endl << "---" << std::endl << std::endl;
			note::list_all_notes(notes, config);
			skip_some_lines(5);
			print_instructions("Enter a sub-command and note #. Enter 'help' for help. Enter 'exit' to quit.", config);
			std::cin >> sub_cmd;

			// --- sub commands ---
			if (sub_cmd == "read")
				sub_cmd_read(notes, config);
			else if (sub_cmd == "help")
				sub_cmd_help(config);
			else if (sub_cmd == "edit")
				sub_cmd_edit(note_path, notes, config);
			else if (sub_cmd == "rm")
				sub_cmd_rm(note_path, notes, config);
			else if (sub_cmd == "add")
				sub_cmd_add(note_path, notes, config);
			else if (sub_cmd == "exit")
				break;
			else {
				print_instructions("Sub-command not found!!! :[[[", config);
				wait_for_continue(5, config);
			}
		}
	}


	void sub_cmd_read(std::vector<note::Note> &notes, NtsyConfig *config)
	{
		std::string id;
		std::cin >> id;
		int noteId = note::parseNoteId(id);
		skip_some_lines(5);
		note::read_note(notes, noteId);
		std::cout << std::endl;
		wait_for_continue(5, config);
	}

	void sub_cmd_help(NtsyConfig *config)
	{
		skip_some_lines(5);
		print_instructions("Available commands:\n\n"
			"add     - usage: add         - Adds a new note.\n"
			"read    - usage: read <#>    - Reads full note text.\n"
			"edit    - usage: edit <#>    - Opens note for editing.\n"
			"rm      - usage: rm <#>      - Deletes a note", config);
		wait_for_continue(5, config);
	}

	void sub_cmd_edit(std::string note_path, std::vector<note::Note> &notes, NtsyConfig *config)
	{
		bool success = false;
		
		// get id
		std::string id;
		std::cin >> id;
		int noteId = note::parseNoteId(id);
		
		// launch the editor
		if (note::check_note_is_there(notes, noteId)){
			success = launch_editor(notes[noteId - 1], config);
		}

		// save
		if (success) {
			note::save_all_notes(note_path, notes);
			std::cout << "Note saved!!! (^^)" << std::endl;
		}

		wait_for_continue(5, config);
	}

	void recount_notes(std::vector<note::Note> &notes)
	{
		for (size_t i = 0; i < notes.size(); i++) {
			notes[i].set_tmpId(i + 1);
		}
	}

	void sub_cmd_rm(std::string note_path, std::vector<note::Note> &notes, NtsyConfig *config)
	{
		std::string id;
		std::cin >> id;
		int noteId = note::parseNoteId(id);
		if (note::remove_note(note_path, notes, noteId))
		{
			// note: don't update collection -> really uneccesarry overhead just for a date
			// that date will only refer to editing the collection meta info itself...
			std::cout << "Note removed!!!" << std::endl;
			recount_notes(notes);
		}
		wait_for_continue(5, config);
	}

	void sub_cmd_add(std::string note_path, std::vector<note::Note> &notes, NtsyConfig *config)
	{
		// create new note
		note::Note n;

		// launch editor - make sure we actually added a note before saving!!!
		if (launch_editor(n, config) && !n.get_text().empty()) {
			// save 
			n.set_tmpId(notes.size() + 1);
			notes.push_back(n);
			note::save_all_notes(note_path, notes);
			std::cout << "Note added!!! (^w^)" << std::endl;
		}
		else
			std::cout << "No note added!!! (^.^)" << std::endl;
		
		wait_for_continue(5, config);
	}

	void wait_for_continue(size_t num_lines, NtsyConfig *config)
	{
		print_instructions("Press any key to continue...", config);
		while (!_kbhit()) {}
		skip_some_lines(num_lines);
	}

	void print_instructions(char *instructions, NtsyConfig *config)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		change_console_color(config->get_heading_color(), &csbi);
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

	std::string get_note_path(std::string &name, NtsyConfig *config){
		return config->get_ntsy_root() + txt::get_ntsy_file_name(name);
	}
}