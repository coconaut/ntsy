#include "stdafx.h"
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "collection.h"
#include "console.h"

// --- constants ---
const int TIMESIZE = 26;
const int COLSIZE = 30;
const int MAXLENGTH = 200;

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
	// truncate max length
	remove_char(name, '\n');
	remove_char(name, '\t');
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
	char comma = ',';
	out << c.m_date_modified
		<< comma
		<< c.m_date_created
		<< comma
		<< c.m_name
		<< std::endl;
	return out;
}


/**
 * Deserialize the Collection. 
 */
std::istream& operator>> (std::istream &in, Collection &c) {
	char comma;
	char buf[MAXLENGTH];
	in >> c.m_date_created;
	in >> comma;
	in >> c.m_date_modified;
	in >> comma;
	in.getline(buf, MAXLENGTH);
	c.m_name = std::string(buf);
	return in;
}

/**
 * Saves a single collection to the list.
 */
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


/**
 * Loads up and returns all collections.
 * TODO: this should be changed to load a map / hash table instead...
 */
std::vector<Collection> get_all_collections(std::string path)
{
	std::vector<Collection> cols;
	std::ifstream inf(path);
	if (!inf)
		std::cerr << "Oh no!!! Notesy can't find it's index file!!!" << std::endl;
	else
	{
		// order here is important for annoying EOF stuff...
		Collection col;
		while (inf >> col)
			cols.push_back(col);
	}
	return cols;
}





/**
 * Lists all collections. Note: args[0] expects path to index file.
 */
void list_all_collections(std::string args[])
{
	list_all_collections(args[0]);
}

void list_all_collections(std::string path)
{
	std::vector<Collection> cols = get_all_collections(path);
	print_header();
	for (size_t i = 0; i < cols.size(); i++) {
		cols[i].pretty_print();
	}
}


// do we want to overload the filestream operator??? or otherwise roll our own serialization?
// could be a good way to learn
// version the serialization format -> first 4 chars V___
// each obj: should serialize self (all props), call serialize method of any child objects...
// if child is a vector, first write the size
// let's try it! first, without the topic child... then add the vector!!!



