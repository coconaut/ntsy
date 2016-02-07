#include "stdafx.h"
#include <iomanip>
#include <map>
#include <string>
#include "command.h"
#include "collection.h"
#include "note.h"
#include "text.h"

namespace cmd {

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
			cmds["list"] = new NtsyCommand("list", "Lists collections. If collection is specified, lists notes.", "[<abbr.>]", cmd_list);
			cmds["col"] = new NtsyCommand("col", "Adds a new collection.","<name> <abbr.>", cmd_col);
			cmds["rm"] = new NtsyCommand("rm", "Removes a collection.", "<abbr.>", cmd_rm);
			cmds["jot"] = new NtsyCommand("jot", "Adds a note to a collection", "<abbr.> <text>", cmd_jot);
			cmds["read"] = new NtsyCommand("read", "Opens a collection for reading.", "<abbr.>", cmd_read);
		}

		return cmds;

		// init (welcome message, brief help info, set up dir)
		// config (change dir, maybe colors..., show dates, etc.)

		// edit <abbr.> -> opens a collection in edit mode
		// erase (removes a note from a collection)
		
		// launch <num>
		// edit? (save for last...)
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
			// list notes
			std::string note_path = args[1] + ".ntsy";
			auto notes = note::get_all_notes(note_path);
			// if no notes, make sure that was a valid collection, otherwise display
			if (notes.size() == 0) {
				std::cout << "No notes found for " + args[1] + "."
					<< std::endl
					<< "If this is a valid collection, add some notes!!! (^.^)"
					<< std::endl;
			}
			else 
				note::list_all_notes(note_path);
		}
		else {
			// otherwise, list all the collections
			col::list_all_collections(path);
		}
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
		if (!col::has_collection(cols, args[1])) {
			std::cout << "Oh no!!! Unable to find collection with abbreviation: " + args[1] << "!!! :[" << std::endl;
		}
		else {
			// add n to file (using rel for now...)
			std::string note_file_path = args[1] + ".ntsy";
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
			else {
				std::cout << "Oh noo!!! Unable to add note to collection. Save failed!!! :(((" << std::endl;
			}
		}
		return true;
	}


	/**
	 * Opens a collection for reading mode. 
	 * Prompts user to choose a collection.
	 * Displays full note text.
	 */
	bool cmd_read(std::vector<std::string> args, std::string path)
	{
		// TODO:
		return true;
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
}