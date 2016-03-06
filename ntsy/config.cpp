#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "windows.h"
#include "config.h"
#include "text.h"


/**
* Gets the directory of the runnig exe.
* Appends config file name.
* This way ntsy can be added to path and run from
* anywhere while still finding it's config.
*/
std::string GetConfigFile() {
	const char *CONFIG_FILE_NAME = "\\config.toml";
	WCHAR buf[MAX_PATH];
	GetModuleFileName(NULL, buf, sizeof(buf));
	std::string full_exe_path = txt::convert_to_narrow(buf);
	std::string::size_type pos = full_exe_path.find_last_of("\\/");
	std::string dir = full_exe_path.substr(0, pos);
	std::string name = dir + CONFIG_FILE_NAME;
	return name;
}

NtsyConfig::NtsyConfig() {
	m_is_loaded = this->load();
}

std::string NtsyConfig::get_index_path() {
	// TODO: make a path combine func that deals with '/'
	return m_ntsy_root + "index.ntsy";
}

bool NtsyConfig::save() {
	// config must always be with the exe, or somewhere certain.
	// we can't load a path to read since we wouldn't know where to load from!
	std::ofstream outf(GetConfigFile());
	if (!outf) {
		std::cout << "Unable to open file!!" << std::endl;
		return false;
	}
	outf << *this;
	outf.close();
	return true;
}

bool NtsyConfig::load() {
	std::ifstream inf(GetConfigFile());
	if (!inf) {
		std::cout << "Unable to open config!!! (^.^)" 
			<< std::endl 
			<< "Have you run 'ntsy init' yet?" 
			<< std::endl;
		return false;
	}
	inf >> *this;
	inf.close();
	return true;
}

bool NtsyConfig::change_setting(std::string key, char *val) {
	auto mapper = create_config_keys_mapper(*this);
	std::string map_key = "[" + key + "]";
	if (has_key(map_key, mapper)) {
		mapper[map_key](val, this);
		return this->save();
	}
	else {
		std::cout << "Unable to find config setting: " << key << std::endl;
		return false;
	}
}

std::ostream& operator<< (std::ostream &out, const NtsyConfig &c) {
	using std::endl;
	out << "[root]" << endl
		<< c.m_ntsy_root << endl << endl
		<< "[editor]" << endl
		<< c.m_editor << endl << endl
		<< "[console_color]" << endl
		<< c.m_console_color << endl << endl
		<< "[heading_color]" << endl
		<< c.m_heading_color;
	return out;
};


std::istream& operator>> (std::istream &in, NtsyConfig &c) {
	// note: it'd be nice to make this real TOML, but for now,
	// since we control all serialization, can go by prop order
	auto mapper = create_config_keys_mapper(c);
	std::string key;
	std::string val;
	while (in >> key)
	{
		if (has_key(key, mapper)) {
			in >> val;
			mapper[key](&val[0], &c);
		}
	}

	return in;
};

std::map<std::string, config_setter*> create_config_keys_mapper(NtsyConfig &c) {
	std::map<std::string, config_setter*> m;
	m["[root]"] = set_root_path_c;
	m["[editor]"] = set_editor_c;
	m["[console_color]"] = set_console_color_c;
	m["[heading_color]"] = set_heading_color_c;
	return m;
}

bool has_key(std::string &key, std::map<std::string, config_setter*> &mapper) {
	return (mapper.find(key) != mapper.end());
}

// wrappers for easier mapping from void* funcs, all using char* as arg
void set_heading_color_c(char* color, NtsyConfig *c) { 
	c->set_heading_color(atoi(color)); 
}
void set_console_color_c(char* color, NtsyConfig *c) { 
	c->set_console_color(atoi(color)); 
}
void set_editor_c(char* editor, NtsyConfig *c) { 
	c->set_editor(std::string(editor)); 
}
void set_root_path_c(char *rp, NtsyConfig *c) { 
	c->set_root_path(std::string(rp)); 
}



