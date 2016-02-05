#include "stdafx.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "collection.h"
#include "console.h"
#include "text.h"


namespace col {


	Collection::Collection() {}

	Collection::Collection(std::string name, std::string abbr)
	{
		assert(("Collection must have name!", !name.empty() && name.compare("") != 0));
		assert(("Collection must have abbreviation!", !abbr.empty() && abbr.compare("") != 0));
		assert(("Name can't be more than " + std::to_string(MAXLENGTH) + " characters!", name.length() <= MAXLENGTH));
		assert(("Abbreviation must be exactly 3 characters!", abbr.length() == 3));

		// TODO: check for other bad chars?

		// make abbr all uppercase
		to_all_upper(abbr);
		
		// set defaults
		m_name = name;
		m_abbr = abbr;
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


	/**
	 * Prints a Collection object to an ostream.
	 */
	void Collection::pretty_print() const
	{
		// format time strings
		std::string dc = format_time(&m_date_created);
		std::string dm = format_time(&m_date_modified);

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
		out << c.m_date_created
			<< ','
			<< c.m_date_modified
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
	* Got a little funky here. Wanted to make use of the overloaded i/o operators,
	* but also wanted to inherit base recordable's save methods.
	* So base uses serialize/deserialize member methods (pure virtuals), and
	* derived forms can call them and simply use the friend ops within.
	*/
	std::ostream& Collection::serialize(std::ostream &out) const {
		out << *this;
		return out;
	}

	std::istream& Collection::deserialize(std::istream &in) {
		in >> *this;
		return in;
	}

	
	/**
	 * Overwrites index file file all collections in memory.
	 */
	bool save_all_collections(std::string path, col_map_t &cols)
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
	col_map_t get_all_collections(std::string path)
	{
		col_map_t cols;
		std::ifstream inf(path);
		if (!inf)
			std::cerr << "Oh no!!! Notesy can't find its index file!!!" << std::endl;
		else
		{
			// order here is important for annoying EOF stuff in while condition...
			Collection col;
			while (inf >> col)
				cols[col.get_abbr()] = col;
		}
		return cols;
	}


	/**
	 * Lists all collections.
	 */
	void list_all_collections(col_map_t &cols)
	{
		print_header();
		// use the const ref so we don't copy
		for (const auto &iter : cols) {
			iter.second.pretty_print();
		}
	}

	/**
	 * Lists all collections.
	 */
	void list_all_collections(std::string path)
	{
		col_map_t cols = get_all_collections(path);
		list_all_collections(cols);
	}

	

	/**
	 * Adds a collection.
	 * Makes sure that abbreviation is unique.
	 */
	bool add_collection(col_map_t &cols, std::string path, std::string name, std::string abbr)
	{
		bool inserted = false;
		Collection col(name, abbr);
		if (cols.find(col.get_abbr()) == cols.end()) {
			cols[col.get_abbr()] = col;
			col.save(path);
			inserted = true;
		}
		return inserted;
	}


	/**
	 * Removes a collection, if it is found.
	 */
	bool remove_collection(col_map_t &cols, std::string path, std::string abbr)
	{
		size_t erased = cols.erase(abbr);
		if (erased > 0)
			erased = save_all_collections(path, cols);
		
		return erased > 0;
	}
}