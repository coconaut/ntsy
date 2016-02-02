#pragma once
#include <string>
#include <vector>

namespace cmd {


	// TODO: accept config object...
	typedef bool cmd_t(std::vector<std::string> args, std::string path);

	// class for command information and function calls, like shell built-in
	class NotesyCommand
	{
		// TODO: add usage
		// e.g. col [name] [abbr], or top [topic name] [col abbr] 
		std::string m_name;
		std::string m_help_info;
		cmd_t *m_command;
	public:
		NotesyCommand(std::string name, std::string help_info, cmd_t *cmd);

		std::string get_name() { return m_name; }
		std::string get_help() { return m_help_info; }
		bool run_command(std::vector<std::string> args, std::string path) { return m_command(args, path); }
		void pretty_print();

	};

	typedef std::map<std::string, NotesyCommand *> cmd_map_t;

	// declarations
	cmd_map_t init_commands();
	void clean_up_commands(cmd_map_t &cmds);
	bool cmd_list(std::vector<std::string> args, std::string path);
	bool cmd_col(std::vector<std::string> args, std::string path);
	void show_help(cmd_map_t &cmds);
	bool has_command(cmd_map_t &cmds, std::string cmd_name);
}