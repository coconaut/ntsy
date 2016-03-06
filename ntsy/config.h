#pragma once
#include <iostream>
#include <map>

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
	bool change_setting(std::string key, char *val);
	std::string get_index_path();
	
	
	// --- i/o serialization overloads ---
	friend std::ostream& operator<< (std::ostream &out, const NtsyConfig &c);
	friend std::istream& operator>> (std::istream &in, NtsyConfig &c);
};


typedef void config_setter(char *val, NtsyConfig *c);

// --- declarations --
std::map<std::string, config_setter*> create_config_keys_mapper(NtsyConfig &c);
bool has_key(std::string &key, std::map<std::string, config_setter*> &mapper);
void set_heading_color_c(char* color, NtsyConfig *c);
void set_console_color_c(char* color, NtsyConfig *c);
void set_editor_c(char* editor, NtsyConfig *c);
void set_root_path_c(char *rp, NtsyConfig *c);
