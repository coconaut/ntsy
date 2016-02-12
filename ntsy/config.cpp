#include "stdafx.h"
#include "config.h"


NtsyConfig::NtsyConfig() {
	// TODO: read from root, load up
	// for now, set some defaults
	m_console_color = 11;
}



// TODO: (it'd be nice to save as YAML or something pleasant / editable)
std::ostream& operator<< (std::ostream &out, const NtsyConfig &c) {

	return out;
};
 
std::istream& operator>> (std::istream &in, NtsyConfig &c){
	return in;
};

