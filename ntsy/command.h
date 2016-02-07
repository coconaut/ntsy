#pragma once
#include <string>
#include <vector>

namespace cmd {


	// TODO: accept config object...
	typedef bool cmd_t(std::vector<std::string> args, std::string path);

	// class for command information and function calls, like shell built-in
	class NtsyCommand
	{
		// TODO: add usage
		// e.g. col [name] [abbr], or top [topic name] [col abbr] 
		std::string m_name;
		std::string m_desc;
		std::string m_usage;
		cmd_t *m_command;
	public:
		NtsyCommand(std::string name, std::string desc, std::string usage, cmd_t *cmd);

		std::string get_name() { return m_name; }
		std::string get_desc() { return m_desc; }
		std::string get_usage() { return m_usage; }
		bool run_command(std::vector<std::string> args, std::string path) { return m_command(args, path); }
		void pretty_print_desc();
		void pretty_print_usage();

	};

	typedef std::map<std::string, NtsyCommand *> cmd_map_t;

	// --- declarations ---
	cmd_map_t init_commands();
	void clean_up_commands(cmd_map_t &cmds);
	void show_descriptions(cmd_map_t &cmds);
	bool has_command(cmd_map_t &cmds, std::string cmd_name);

	// --- commands ---
	bool cmd_list(std::vector<std::string> args, std::string path);
	bool cmd_col(std::vector<std::string> args, std::string path);
	bool cmd_rm(std::vector<std::string> args, std::string path);
	bool cmd_jot(std::vector<std::string> args, std::string path);
	bool cmd_read(std::vector<std::string> args, std::string path);
}