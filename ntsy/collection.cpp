#include "stdafx.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "config.h"
#include "collection.h"
#include "console.h"
#include "text.h"


namespace col {

	Collection::Collection() {}

	Collection::Collection(std::string name, std::string abbr)
	{
		// set defaults
		m_name = name;
		m_abbr = abbr;
	}

	// prints a header for collection / collection list
	void print_header(NtsyConfig *config)
	{
		// consider changiing (and resetting) console color? maybe wrap each part in function...
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		change_console_color(config->get_heading_color(), &csbi);
		std::cout << std::left
			<< std::setw(txt::ABBR_COLSIZE) << "Abbr."
			<< std::setw(txt::MAINCOLSIZE) << "Collection"
			<< std::setw(txt::DATECOLSIZE) << "Last Modified"
			<< std::setw(txt::DATECOLSIZE) << "Date Created"
			<< std::endl
			<< std::setw(txt::ABBR_COLSIZE) << "---"
			<< std::setw(txt::MAINCOLSIZE) << "---"
			<< std::setw(txt::DATECOLSIZE) << "---"
			<< std::setw(txt::DATECOLSIZE) << "---"
			<< std::endl;
		reset_console_color(csbi);
	}


	/**
	 * Prints a Collection object to an ostream.
	 */
	void Collection::pretty_print() const
	{
		// format time strings
		std::string dc = txt::format_time(&m_date_created);
		std::string dm = txt::format_time(&m_date_modified);

		// write to out stream
		std::cout << std::left 
			<< std::setw(txt::ABBR_COLSIZE) << m_abbr
			<< std::setw(txt::MAINCOLSIZE) << m_name
			<< std::setw(txt::DATECOLSIZE) << "[" + dm + "]"
			<< std::setw(txt::DATECOLSIZE) << "[" + dc + "]"
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
		char buf[txt::MAINCOLSIZE];
		in >> c.m_date_created;
		in >> delim;
		in >> c.m_date_modified;
		in >> delim;
		in >> c.m_abbr;
		in.get();
		in.getline(buf, txt::MAINCOLSIZE);
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
	 * Overwrites index file with all collections in memory.
	 */
	bool save_all_collections(std::string path, col_map_t &cols)
	{
		std::ofstream outf(path);
		if (!outf) {
			std::cerr << "Oh nooo!!! Could not open ntsy index!!! :(" << std::endl;
			return false;
		}
		bool res = true;
		for (const auto &iter : cols) {
			if (!iter.second.save(outf)) {
				res = false;
				break;
			}
		}
		outf.close();
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
			std::cerr << "Oh no!!! Notesy can't find its index file!!! :(" << std::endl;
		else
		{
			// order here is important for annoying EOF stuff in while condition...
			Collection col;
			while (inf >> col) {
				cols[col.get_abbr()] = col;
			}
			inf.close();
		}
		return cols;
	}


	/**
	 * Lists all collections.
	 */
	void list_all_collections(col_map_t &cols, NtsyConfig *config)
	{
		print_header(config);
		// use the const ref so we don't copy
		for (const auto &iter : cols) {
			iter.second.pretty_print();
		}
	}

	/**
	 * Lists all collections.
	 */
	void list_all_collections(std::string path, NtsyConfig *config)
	{
		col_map_t cols = get_all_collections(path);
		list_all_collections(cols, config);
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
		bool success = (cols.erase(abbr) > 0) && save_all_collections(path, cols);
		if (success) {
			std::string note_path = txt::get_ntsy_file_name(abbr);
			remove(&note_path[0]);
		}
		return success;
	}


	/**
	 * Checks collection map
	 */
	bool has_collection(col_map_t &cols, std::string abbr)
	{
		return (cols.find(abbr) != cols.end());
	}
}