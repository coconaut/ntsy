#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "config.h"


const char *CONFIG_PATH = "./.ntsyconfig";

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
	std::ofstream outf(CONFIG_PATH);
	if (!outf) {
		std::cout << "Unable to open config!!" << std::endl;
		return false;
	}
	outf << *this;
	outf.close();
	return true;
}

bool NtsyConfig::load() {
	std::ifstream inf(CONFIG_PATH);
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
	std::string buf;
	in >> buf
		>> c.m_ntsy_root
		>> buf
		>> c.m_editor
		>> buf
		>> c.m_console_color
		>> buf
		>> c.m_heading_color;
	return in;
};

