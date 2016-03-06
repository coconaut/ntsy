#pragma once
#include <iostream>

class NtsyConfig
{
	bool m_is_loaded;
	std::string m_ntsy_root;
	std::string m_editor;
	int m_console_color;
	int m_heading_color;

public:
	// --- constructor ---
	NtsyConfig();

	// --- accessors ---
	std::string get_ntsy_root() { return m_ntsy_root; }
	void set_root_path(std::string rp) { m_ntsy_root = rp; }
	std::string get_editor() { return m_editor; }
	void set_editor(std::string editor) { m_editor = editor; }
	int get_console_color() { return m_console_color; }
	void set_console_color(int color) { m_console_color = color; }
	int get_heading_color() { return m_heading_color; }
	void set_heading_color(int color) { m_heading_color = color; }
	bool is_loaded() { return m_is_loaded; }

	// --- methods ---
	bool save();
	bool load();
	std::string get_index_path();
	
	
	// --- i/o serialization overloads ---
	friend std::ostream& operator<< (std::ostream &out, const NtsyConfig &c);
	friend std::istream& operator>> (std::istream &in, NtsyConfig &c);
};
