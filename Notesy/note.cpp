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
		std::string sub_str;
		if (m_text.size() > MAINCOLSIZE)
			sub_str = m_text.substr(0, MAINCOLSIZE - 5) + "...";
		else
			sub_str = m_text;
		
		std::cout << std::left
			<< std::setw(5) << m_tmpId
			<< std::setw(MAINCOLSIZE) << sub_str
			<< std::setw(DATECOLSIZE) << "[" + dm + "]"
			<< std::setw(DATECOLSIZE) << "[" + dc + "]"
			<< std::endl
			<< std::endl;
	}

	// prints a header for collection / collection list
	void print_header()
	{
		// consider changiing (and resetting) console color? maybe wrap each part in function...
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		change_console_color(11, &csbi);
		std::cout << std::left
			<< std::setw(5) << "#"
			<< std::setw(MAINCOLSIZE) << "Note"
			<< std::setw(DATECOLSIZE) << "Last Modified"
			<< std::setw(DATECOLSIZE) << "Date Created"
			<< std::endl
			<< std::setw(5) << "---"
			<< std::setw(MAINCOLSIZE) << "---"
			<< std::setw(DATECOLSIZE) << "---"
			<< std::setw(DATECOLSIZE) << "---"
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
		// TODO: we are losing a character when we read it in!!!
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
			// get seems to work better than read for non-binary...
			in.get(&tmp_txt[0], len);
			n.m_text = std::string(&tmp_txt[0]);
		}
		return in;
	}

	/**
	* Got a little funky here. Wanted to make use of the overloaded i/o operators,
	* but also wanted to inherit base recordable's save methods.
	* So base uses serialize/deserialize member methods (pure virtuals), and
	* derived forms can call them and simply use the friend ops within.
	*/
	std::ostream& Note::serialize(std::ostream &out) const {
		out << *this;
		return out;
	}
	std::istream& Note::deserialize(std::istream &in) {
		in >> *this;
		return in;
	}

	/**
	 * Adds a note to a collection's note file.
	 * Returns false if unable to save.
	 * Command should handle validation of the collection, as
	 * well as any updates to date modified field for the collection.
	 */
	bool add_note(std::string text, std::string path) {
		bool inserted = false;
		Note n(text);
		if (n.save(path))
			inserted = true;
		return inserted;
	}

	/**
	 * Gets all notes in a collection.
	 */
	std::vector<Note> get_all_notes(std::string path) {
		std::vector<Note> notes;
		std::ifstream inf(path);
		if (!inf)
			std::cerr << "Oh no!!! Notesy can't find or open this collection!!! :(" << std::endl << std::endl;
		else {
			Note n;
			int ct = 0;
			while (inf >> n) {
				n.set_tmpId(++ct);
				notes.push_back(n);
				inf.ignore(1, '\n');
			}
			inf.close();
		}
		return notes;
	}


	/**
	* Lists all notes.
	*/
	void list_all_notes(std::vector<Note> &notes)
	{
		print_header();
		// use the const ref so we don't copy
		for (const auto &iter : notes) {
			iter.pretty_print();
		}
	}

	/**
	* Lists all notes.
	*/
	void list_all_notes(std::string path)
	{
		auto notes = get_all_notes(path);
		list_all_notes(notes);
	}
}
