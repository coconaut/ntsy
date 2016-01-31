#include "stdafx.h"
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "collection.h"
#include "console.h"


namespace col {

	// --- constants ---
	const int TIMESIZE = 26;
	const int COLSIZE = 30;
	const int ABBR_COLSIZE = 10;
	const int MAXLENGTH = COLSIZE;

	void remove_char(std::string &str, char c)
	{
		str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	}

	void remove_whitespace(std::string &str)
	{
		remove_char(str, '\n');
		remove_char(str, '\t');
		remove_char(str, ' ');
	}

	void remove_newlines(std::string &str)
	{
		remove_char(str, '\n');
	}

	void trim_char(std::string &str) {
		// TODO:
	}



	Collection::Collection() {}

	Collection::Collection(std::string name, std::string abbr)
	{
		assert(("Collection must have name!", !name.empty() && name.compare("") != 0));
		assert(("Collection must have abbreviation!", !abbr.empty() && abbr.compare("") != 0));
		// TODO: check for other bad chars?
		// TODO: trim whitespace at ends...
		remove_newlines(name);
		remove_whitespace(abbr);
		// TODO: pull the max length from MAXLENGTH
		assert(("Name can't be more than 30 characters!", name.length() <= MAXLENGTH));
		assert(("Abbreviation must be exactly 3 characters!", abbr.length() == 3));
		
		// set defaults
		m_name = name;
		m_abbr = abbr;
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
			<< std::setw(ABBR_COLSIZE) << "Abbr."
			<< std::setw(COLSIZE) << "Collection"
			<< std::setw(COLSIZE) << "Last Modified"
			<< std::setw(COLSIZE) << "Date Created"
			<< std::endl
			<< std::setw(ABBR_COLSIZE) << "---"
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
	void Collection::pretty_print() const
	{
		// format time strings
		char buf_created[TIMESIZE], buf_modified[TIMESIZE];
		ctime_s(buf_created, TIMESIZE, &m_date_created);
		ctime_s(buf_modified, TIMESIZE, &m_date_modified);
		std::string dc = trim_date(buf_created);
		std::string dm = trim_date(buf_modified);

		// write to out stream
		std::cout << std::left 
			<< std::setw(ABBR_COLSIZE) << m_abbr
			<< std::setw(COLSIZE) << m_name
			<< std::setw(COLSIZE) << "[" + dm + "]"
			<< std::setw(COLSIZE) << "[" + dc + "]"
			<< std::endl;
	}


	/**
	 * Serialize the Collection.
	 */
	std::ostream& operator<< (std::ostream &out, const Collection &c) {
		out << c.m_date_modified
			<< ','
			<< c.m_date_created
			<< ','
			<< c.m_abbr
			<< ' '
			<< c.m_name
			<< std::endl;
		return out;
	}


	/**
	 * Deserialize the Collection.
	 */
	std::istream& operator>> (std::istream &in, Collection &c) {
		char delim;
		char buf[MAXLENGTH];
		in >> c.m_date_created;
		in >> delim;
		in >> c.m_date_modified;
		in >> delim;
		in >> c.m_abbr;
		in.get();
		in.getline(buf, MAXLENGTH);
		c.m_name = std::string(buf);
		return in;
	}

	/**
	 * Appends a single collection to index file.
	 */
	bool Collection::save(std::string path) const
	{
		std::ofstream outf(path, std::ios::app);
		if (!outf) {
			std::cerr << "Could not open notesy index!!!" << std::endl;
			return false;
		}
		outf << *this;
		return true;
	}

	/**
	 * Appends a single collection to index file.
	 * This overload expects an already open filestream.
	 * Intended for looping.
	 */
	bool Collection::save(std::ofstream &outf) const
	{
		if (!outf) {
			std::cerr << "Could not write to notesy index!!!" << std::endl;
			return false;
		}
		outf << *this;
		return true;
	}

	/**
	 * Overwrites index file file all collections in memory.
	 */
	bool save_all_collections(std::string path, std::map<std::string, Collection> &cols)
	{
		std::ofstream outf(path);
		if (!outf) {
			std::cerr << "Could not open notesy index!!!" << std::endl;
			return false;
		}
		bool res = true;
		for (const auto &iter : cols) {
			if (!iter.second.save(outf)) {
				res = false;
				break;
			}
		}
		return res;
	}


	/**
	 * Loads up and returns all collections.
	 */
	std::map<std::string, Collection> get_all_collections(std::string path)
	{
		std::map<std::string, Collection> cols;
		std::ifstream inf(path);
		if (!inf)
			std::cerr << "Oh no!!! Notesy can't find its index file!!!" << std::endl;
		else
		{
			// order here is important for annoying EOF stuff in while condition...
			Collection col;
			while (inf >> col)
				cols[col.get_name()] = col;
		}
		return cols;
	}


	/**
	* Lists all collections.
	*/
	void list_all_collections(std::string path)
	{
		std::map<std::string, Collection> cols = get_all_collections(path);
		print_header();
		// use the const ref so we don't copy
		for (const auto &iter : cols) {
			iter.second.pretty_print();
		}
	}

	/**
	 * Lists all collections.
	 * A convenience overload for consistency with cmd_t pointers.
	 * args[0] expects path to index file.
	 */
	void list_all_collections(std::string args[])
	{
		list_all_collections(args[0]);
	}

	// TODO:
	// adding
	// removing
	// renaming
	// abbreviation + serialize + edit
	// help

	
	// version the serialization format -> first 4 chars V___
	// each obj: should serialize self (all props), call serialize method of any child objects...
	// if child is a vector, first write the size
	// let's try it! first, without the topic child... then add the map / vector

	//$(ProjectName)
}