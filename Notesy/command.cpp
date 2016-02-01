#include "stdafx.h"
#include <iomanip>
#include <map>
#include "command.h"
#include "collection.h"

namespace cmd {

	/**
	 * Prints command info.
	 */
	void NotesyCommand::pretty_print() {
		// TODO!!!
		std::cout
			<< std::left
			<< std::setw(10)
			<< m_name
			<< m_help_info
			<< std::endl;
	}

	/**
	 * Intializes vector of notesy commands.
	 */
	cmd_map_t init_commands()
	{
		cmd_map_t cmds;
		cmds["list"] = new NotesyCommand(
			"list",
			"Lists collections.",
			list_all_collections
			);
		cmds["col"] = new NotesyCommand(
			"col",
			"Adds a new collection.",
			add_collection);

		return cmds;

		// add
		// init
		// config
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
	bool add_collection(std::vector<std::string> args, std::string path)
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

	
	bool list_all_collections(std::vector<std::string> args, std::string path)
	{
		col::list_all_collections(path);
		return true;
	}

	void show_help(cmd_map_t &cmds) {
		for (const auto &iter : cmds) {
			iter.second->pretty_print();
		}
	}

	bool has_command(cmd_map_t &cmds, std::string cmd_name) {
		return (cmds.find(cmd_name) != cmds.end());
	}
}