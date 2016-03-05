#pragma once
#include <string>
#include <vector>
#include "collection.h"
#include "note.h"
#include "config.h"

namespace cmd {

	typedef bool cmd_t(std::vector<std::string> args, NtsyConfig *config);

	// class for command information and function calls, like shell built-in
	class NtsyCommand
	{
		std::string m_name;
		std::string m_desc;
		std::string m_usage;
		cmd_t *m_command;
		NtsyConfig *m_config;
	public:
		// --- constructor / member init ---
		NtsyCommand(std::string name, std::string desc, std::string usage, cmd_t *cmd, NtsyConfig *config) : m_name{ name }, 
			m_desc{ desc }, m_usage{ usage }, m_command{ cmd }, m_config{ config } {}

		// --- accessors ---
		std::string get_name() { return m_name; }
		std::string get_desc() { return m_desc; }
		std::string get_usage() { return m_usage; }
		NtsyConfig *get_config() { return m_config; }

		// --- methods ---
		bool run_command(std::vector<std::string> args) { return m_command(args, m_config); }
		void pretty_print_desc();
		void pretty_print_usage();

	};


	typedef std::map<std::string, NtsyCommand *> cmd_map_t;

	// --- commands ---
	bool cmd_col(std::vector<std::string> args, NtsyConfig *config);
	bool cmd_jot(std::vector<std::string> args, NtsyConfig *config);
	bool cmd_list(std::vector<std::string> args, NtsyConfig *config);
	bool cmd_open(std::vector<std::string> args, NtsyConfig *config);
	bool cmd_rm(std::vector<std::string> args, NtsyConfig *config);

	// --- sub-commands ---
	void sub_cmd_add(std::string note_path, std::vector<note::Note> &notes, NtsyConfig *config);
	void sub_cmd_edit(std::string note_path, std::vector<note::Note> &notes, NtsyConfig *config);
	void sub_cmd_help(NtsyConfig *config);
	void sub_cmd_read(std::vector<note::Note> &notes, NtsyConfig *config);
	void sub_cmd_rm(std::string path, std::vector<note::Note> &notes, NtsyConfig *config);
	

	// --- declarations ---
	bool has_command(cmd_map_t &cmds, std::string cmd_name);
	bool handle_no_notes(std::vector<note::Note> &notes, std::string &abbr);
	bool handle_bad_collection(col::col_map_t cols, std::string abbr);
	cmd_map_t init_commands(NtsyConfig *config);
	void clean_up_commands(cmd_map_t &cmds);
	void loop_interactive(std::vector<note::Note> &notes, std::string col_name, std::string note_path, NtsyConfig *config);
	void print_instructions(char *instructions, NtsyConfig *config);
	void show_descriptions(cmd_map_t &cmds);
	void skip_some_lines(size_t num_lines);
	void wait_for_continue(size_t num_lines, NtsyConfig *config);
}