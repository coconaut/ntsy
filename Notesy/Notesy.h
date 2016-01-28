#pragma once

// alias for a basic command function
typedef void cmd_t(char *arg[]);


// class for command information and function calls, like shell built-in
class NotesyCommand
{
	char *name;
	char *help_info;
	cmd_t *command;
public:
	NotesyCommand(char *n, char *h_info, cmd_t *cmd) 
	{
		name = n;
		help_info = h_info;
		command = cmd;
	}

	char *get_name() { return name; }
	char *get_help() { return help_info; }
	void run_command(char *args[]) { command(args); }
	
};

