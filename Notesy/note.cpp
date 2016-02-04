#include "stdafx.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "console.h"
#include "note.h"
#include "text.h"

namespace note {

	void Note::pretty_print() const
	{
		std::string dc = format_time(&m_date_created);
		std::string dm = format_time(&m_date_modified);
		std::cout << std::left
			<< std::setw(COLSIZE) << "[" + dm + "]"
			<< std::setw(COLSIZE) << "[" + dc + "]"
			<< std::endl
			<< std::endl
			<< m_text
			<< std::endl;
	}

	// prints a header for collection / collection list
	void print_header()
	{
		// consider changiing (and resetting) console color? maybe wrap each part in function...
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		change_console_color(11, &csbi);
		std::cout << std::left
			<< std::setw(COLSIZE) << "Last Modified"
			<< std::setw(COLSIZE) << "Date Created"
			<< std::endl
			<< std::setw(COLSIZE) << "---"
			<< std::setw(COLSIZE) << "---"
			<< std::endl;
		reset_console_color(csbi);
	}

	/**
	 * Serialize the note.
	 */
	std::ostream& operator<< (std::ostream &out, const Note &n) {
		out << n.m_date_created
			<< ','
			<< n.m_date_modified
			<< ','
			<< n.m_text.size()
			<< ','
			<< n.m_text
			<< std::endl;
		return out;
	}

	/**
	 * Deserialize the note.
	 */
	std::istream& operator>> (std::istream &in, Note &n) {
		int len = 0;
		char comma;
		in >> n.m_date_created
			>> comma
			>> n.m_date_modified
			>> comma
			>> len
			>> comma;
		if (in && len) {
			std::vector<char> tmp_txt(len);
			in.read(&tmp_txt[0], len);
			n.m_text = std::string(&tmp_txt[0]);
		}
		return in;
	}

	bool Note::save(std::ofstream &outf) const {
		if (!outf) {
			std::cerr << "Could not write to notesy index!!!" << std::endl;
			return false;
		}
	}

	bool Note::save(std::string path) const {
		// TODO:
	}
}
