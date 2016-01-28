#include "stdafx.h"
#include "collection.h"
#include <ostream>
#include <ctime>
#include <string>

// --- constants ---
const int TIMESIZE = 26;


// remove new lines
std::string trim_date(char* buf) {
	std::string s(buf);
	return s.substr(0, TIMESIZE - 2);
}

/**
 * Prints a Collection object to an ostream.
 */
std::ostream& operator<< (std::ostream &out, const Collection &c) {
	// format time strings
	char buf_created[TIMESIZE], buf_modified[TIMESIZE];
	ctime_s(buf_created, TIMESIZE, &c.m_date_created);
	ctime_s(buf_modified, TIMESIZE, &c.m_date_modified);
	std::string dc = trim_date(buf_created);
	std::string dm = trim_date(buf_modified);


	// write to out stream
	out << c.m_name << "\t\t"
		<< "[" << dm << "]\t\t"
		<< "[" << dc << "]";
	
	return out;
}


