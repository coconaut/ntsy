#include "stdafx.h"
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include "collection.h"
#include "console.h"

// --- constants ---
const int TIMESIZE = 26;
const int COLSIZE = 30;


void remove_char(std::string &str, char c)
{
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
}


void trim_char(std::string &str) {
	// TODO:
}



Collection::Collection() {}

Collection::Collection(std::string name)
{
	assert(("Collection must have name!", !name.empty() && name.compare("")));
	// TODO: check for bad chars?
	// trim spaces
	// strip newlines especially!!! (or use getline to prevent...)
	remove_char(name, '\n');
	// truncate length?
	m_name = name;
	time(&m_date_created);
	time(&m_date_modified);
}

Collection::~Collection()
{

}

// prints a header for collection / collection list
void print_header()
{
	// consider changiing (and resetting) console color? maybe wrap each part in function...
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	change_console_color(11, &csbi);
	std::cout << std::left
		<< std::setw(COLSIZE) << "Collection"
		<< std::setw(COLSIZE) << "Date Modified"
		<< std::setw(COLSIZE) << "Date Created"
		<< std::endl
		<< std::setw(COLSIZE) << "---"
		<< std::setw(COLSIZE) << "---"
		<< std::setw(COLSIZE) << "---"
		<< std::endl;
	reset_console_color(csbi);
}


// remove new lines
std::string trim_date(char* buf) {
	std::string s(buf);
	return s.substr(0, TIMESIZE - 2);
}


/**
 * Prints a Collection object to an ostream.
 */
void Collection::pretty_print()
{
	// format time strings
	char buf_created[TIMESIZE], buf_modified[TIMESIZE];
	ctime_s(buf_created, TIMESIZE, &m_date_created);
	ctime_s(buf_modified, TIMESIZE, &m_date_modified);
	std::string dc = trim_date(buf_created);
	std::string dm = trim_date(buf_modified);


	// write to out stream
	std::cout << std::left << std::setw(COLSIZE) << m_name
		<< std::setw(COLSIZE) << "[" + dm + "]"
		<< std::setw(COLSIZE) << "[" + dc + "]"
		<< std::endl;
}


/**
 * Serialize the Collection.
 */
std::ostream& operator<< (std::ostream &out, const Collection &c) {
	out << '"' << c.m_name << '"' << std::endl
		<< " " << c.m_date_created << " "
		<< c.m_date_modified 
		<< std::endl;
	return out;
}


/**
 * Deserialize the Collection. 
 */
std::istream& operator>> (std::istream &in, Collection &c) {
	// get the name: may include spaces...
	std::getline(in, c.m_name);
	in >> c.m_date_created;
	in >> c.m_date_modified;
	return in;
}

bool Collection::save (std::string path)
{
	std::ofstream outf(path, std::ios::app);
	if (!outf) {
		std::cerr << "Could not open notesy root directory!!!" << std::endl;
		return false;
	}

	outf << *this;
	return true;
}

// do we want to overload the filestream operator??? or otherwise roll our own serialization?
// could be a good way to learn
// version the serialization format -> first 4 chars V___
// each obj: should serialize self (all props), call serialize method of any child objects...
// if child is a vector, first write the size
// let's try it! first, without the topic child... then add the vector!!!



