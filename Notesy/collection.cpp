#include "stdafx.h"
#include "collection.h"
#include <iomanip>
#include <iostream>
#include <ctime>
#include <string>

// --- constants ---
const int TIMESIZE = 26;
const int COLSIZE = 30;


// prints a header for collection / collection list
void print_header()
{
	std::cout << std::left
		<< std::setw(COLSIZE) << "Collection"
		<< std::setw(COLSIZE) << "Date Modified"
		<< std::setw(COLSIZE) << "Date Created"
		<< std::endl
		<< std::setw(COLSIZE) << "---"
		<< std::setw(COLSIZE) << "---"
		<< std::setw(COLSIZE) << "---"
		<< std::endl;
}


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
	out << std::setw(COLSIZE) << c.m_name 
		<< std::setw(COLSIZE) << "[" + dm + "]"
		<< std::setw(COLSIZE) << "[" + dc + "]";
	
	return out;
}


