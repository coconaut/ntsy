#pragma once
#include <iostream>

class NtsyConfig
{
	char *m_ntsy_root;
	char *m_editor;
	int m_console_color;
	int m_heading_color;


public:
	// --- constructor ---
	NtsyConfig();

	// --- accessors ---
	char *get_ntsy_root() { return m_ntsy_root; }
	void set_root_path(char *rp) { m_ntsy_root = rp; }
	char *get_editor() { return m_editor; }
	void set_editor(char * editor) { m_editor = editor; }
	int get_console_color() { return m_console_color; }
	void set_console_color(int color) { m_console_color = color; }
	int get_heading_color() { return m_heading_color; }
	void set_heading_color(int color) { m_heading_color = color; }
	
	// --- i/o serialization overloads ---
	friend std::ostream& operator<< (std::ostream &out, const NtsyConfig &c);
	friend std::istream& operator>> (std::istream &in, NtsyConfig &c);
};
