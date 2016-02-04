#pragma once
#include <ctime>
#include <iostream>
#include <cassert>
#include <map>
#include <string>
#include <vector>
#include "base.h"
#include "text.h"

namespace col {

	// --- constants ---
	const int MAXLENGTH = COLSIZE;


	/**
	 * Main class for Collection objects. Should ultimately
	 * contain printing and serialization / deserialization methods?
	 */
	class Collection : public Base
	{
		std::string m_name;
		std::string m_abbr;

	public:
		// --- constructors ---
		Collection();
		Collection(std::string name, std::string abbr);

		// --- accessors ------------------
		std::string get_name() { return m_name; }
		void set_name(std::string name) { m_name = name; }
		std::string get_abbr() { return m_abbr; }
		void set_abbr(std::string abbr) { m_abbr = abbr; }

		// --- methods ---
		virtual void pretty_print() const;
		virtual bool save(std::string path) const;
		virtual bool save(std::ofstream &outf) const;

		// friend overloads of i/o operators for serialization
		friend std::ostream& operator<< (std::ostream &out, const Collection &c);
		friend std::istream& operator>> (std::istream &in, Collection &c);

	};


	typedef std::map<std::string, Collection> col_map_t;

	// declarations
	void print_header();
	col_map_t get_all_collections(std::string path);
	void list_all_collections(col_map_t &cols);
	void list_all_collections(std::string path);
	bool save_all_collections(std::string path, col_map_t &cols);
	bool add_collection(col_map_t &cols, std::string path, std::string name, std::string abbr);
	bool remove_collection(col_map_t &cols, std::string path, std::string abbr);

}