#pragma once

// alias for a basic command function
typedef void cmd_t(char *arg[]);

/**
 * NotesyCommandDesc: a class that contains name,
 * info, and a pointer to a notesy command function.
 * Somewhat analogous to a shell built-in, which is basically
 * what we're going for here.
*/
class NotesyCommand
{
	char *name;
	char *help_info;
	cmd_t *command;
public:
	char *get_name() { return name; }
	char *get_help() { return help_info; }
	void run_command(char *args[]) { command(args); }
	NotesyCommand(char *n, char *h_info, cmd_t *cmd) {
		name = n;
		help_info = h_info;
		command = cmd;
	}
};

