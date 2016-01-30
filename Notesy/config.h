#pragma once

class NotesyConfig
{
	char *m_root_path;
	int m_console_color;

public:
	// TODO: constructor w/ filepath
	// assert path valid
	char *get_root_path() { return m_root_path; }
	void set_root_path(char *rp) { m_root_path = rp; }
	int get_console_color() { return m_console_color; }
	void set_console_color(int color);

	// TODO: save / serialize
};
