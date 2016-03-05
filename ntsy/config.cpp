#include "stdafx.h"
#include "config.h"


NtsyConfig::NtsyConfig() {
	// TODO: read from root, load up
	// for now, set some default	
	// TODO: make a path combine func that deals with '/'
	m_ntsy_root = "./";
	m_editor = "vim";
	m_console_color = 10;
	m_heading_color = 11;
}

std::string NtsyConfig::get_index_path() {
	// TODO: make this safer? path wise, /s I mean
	return m_ntsy_root + "index.ntsy";
}


// TODO: (it'd be nice to save as YAML or something pleasant / editable)
std::ostream& operator<< (std::ostream &out, const NtsyConfig &c) {

	return out;
};
 
std::istream& operator>> (std::istream &in, NtsyConfig &c){
	return in;
};

