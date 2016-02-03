#include "stdafx.h"
#include <iomanip>
#include <map>
#include "command.h"
#include "collection.h"

namespace cmd {

	/**
	 * NotesyCommand constructor.
	 */
	NotesyCommand::NotesyCommand(std::string name, std::string desc, std::string usage, cmd_t *cmd)
	{
		m_name = name;
		m_desc = desc;
		m_usage = usage;
		m_command = cmd;
	}


	/**
	 * Prints command help / basic info.
	 */
	void NotesyCommand::pretty_print_desc() {
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
	void NotesyCommand::pretty_print_usage() {
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
			cmds["list"] = new NotesyCommand("list", "Lists collections, or if collection is specified, list notes.",
				"[-col <abbr.>]", cmd_list);
			cmds["col"] = new NotesyCommand("col", "Adds a new collection.",
				"<name> <abbr.>", cmd_col);

			// do help, then work on col functionality
			
		}

		return cmds;

		// col
		// jot
		// read
		// init
		// config
		// rm -col -note <Id>
		// edit? (save for last)...
		// help
		// destroy (all notes, notesy dir, config, etc.)
		// if none, display help...
		/*std::vector<NotesyCommand *> cmds = { nc, nc2 };
		return cmds;*/
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
	*/
	bool cmd_col(std::vector<std::string> args, std::string path)
	{
		// TODO: check if dir exists, if not error (tell to call init/ config)
		// TODO: check if index file exists, if not, error (tell to call init / config)

		if (args.size() < 3)
		{
			//TODO: possibly display usage here...
			return false;
		}

		// retrieve collections, add, and show
		auto cols = col::get_all_collections(path);
		if (col::add_collection(cols, path, args[1], args[2]))
			col::list_all_collections(cols);
		else
			std::cout << "There is already a collection with this abbreviation." << std::endl;

		return true;
	}


	bool cmd_list(std::vector<std::string> args, std::string path)
	{
		// TODO: check for -col switch (list notes vs list cols)
		col::list_all_collections(path);
		return true;
	}

	void show_descriptions(cmd_map_t &cmds) {
		for (const auto &iter : cmds) {
			iter.second->pretty_print_desc();
		}
	}

	bool has_command(cmd_map_t &cmds, std::string cmd_name) {
		return (cmds.find(cmd_name) != cmds.end());
	}
}