#pragma once
#include <string>
#include <vector>
#include "collection.h"
#include "note.h"

namespace cmd {


	// TODO: accept config object, not just path...
	typedef bool cmd_t(std::vector<std::string> args, std::string path);

	// class for command information and function calls, like shell built-in
	class NtsyCommand
	{
		std::string m_name;
		std::string m_desc;
		std::string m_usage;
		cmd_t *m_command;
	public:
		// --- constructor ---
		NtsyCommand(std::string name, std::string desc, std::string usage, cmd_t *cmd);

		// --- accessors ---
		std::string get_name() { return m_name; }
		std::string get_desc() { return m_desc; }
		std::string get_usage() { return m_usage; }

		// --- methods ---
		bool run_command(std::vector<std::string> args, std::string path) { return m_command(args, path); }
		void pretty_print_desc();
		void pretty_print_usage();

	};

	typedef std::map<std::string, NtsyCommand *> cmd_map_t;


	// --- commands ---
	bool cmd_list(std::vector<std::string> args, std::string path);
	bool cmd_col(std::vector<std::string> args, std::string path);
	bool cmd_rm(std::vector<std::string> args, std::string path);
	bool cmd_jot(std::vector<std::string> args, std::string path);
	bool cmd_open(std::vector<std::string> args, std::string path);

	// --- sub-commands ---
	void sub_cmd_help();
	void sub_cmd_read(std::vector<note::Note> &notes);
	void sub_cmd_edit();
	void sub_cmd_rm(std::string path, std::vector<note::Note> &notes);
	void sub_cmd_add();
	void sub_cmd_edit();

	// --- declarations ---
	cmd_map_t init_commands();
	void clean_up_commands(cmd_map_t &cmds);
	void show_descriptions(cmd_map_t &cmds);
	bool has_command(cmd_map_t &cmds, std::string cmd_name);
	void print_instructions(char *instructions);
	std::string get_note_file_name(std::string &abbr);
	bool handle_no_notes(std::vector<note::Note> &notes, std::string &abbr);
	void loop_interactive(std::vector<note::Note> &notes, std::string col_name, std::string note_path);
	void skip_some_lines(size_t num_lines);
	bool handle_bad_collection(col::col_map_t cols, std::string abbr);
	void wait_for_continue(size_t num_lines);
	
}