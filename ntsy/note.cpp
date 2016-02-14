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
#include "editor.h"

namespace note {

	void Note::pretty_print() const
	{
		std::string dc = format_time(&m_date_created);
		std::string dm = format_time(&m_date_modified);
		std::string sub_str = m_text;
		
		// for printing purposes, replace newlines with spaces...
		replace_char(sub_str, '\n', ' ');

		if (sub_str.size() > MAINCOLSIZE)
			sub_str = sub_str.substr(0, MAINCOLSIZE - 5) + "...";
		
		
		std::cout << std::left
			<< std::setw(5) << m_tmpId
			<< std::setw(MAINCOLSIZE) << sub_str
			<< std::setw(DATECOLSIZE) << "[" + dm + "]"
			<< std::setw(DATECOLSIZE) << "[" + dc + "]"
			<< std::endl
			<< std::endl;
	}


	/**
	 * Prints a note's full text.
	 */
	void Note::print_full_note()
	{
		std::cout << m_text << std::endl;
	}


	/**
	 * Prints a header for note list.
	 */ 
	void print_header()
	{
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
		// TODO:!!! this loses new lines!!! need to check count, get
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
			// leave room for the null terminator, or buffer will be too small
			char *buf = new char[len + 1];
			// switching to read and manually adding null terminator
			// ifstream::get was losing newlines
			in.read(buf, len);
			buf[len] = '\0'; 
			//in.get(buf, len + 1);
			n.m_text = std::string(buf);
			delete buf;
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
		/*if (!inf)
			std::cerr << "Oh no!!! ntsy can't find or open this collection!!! :(" << std::endl << std::endl;*/
		if (inf) {
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

	/**
	 * Display's that note's full text.
	 */
	void read_note(std::vector<Note> &notes, int noteId)
	{
		
		if (check_note_is_there(notes, noteId))
			notes[noteId - 1].print_full_note();
		
	}

	/**
	 * Just a helper to make sure we're in the vector bounds
	 * and display an error message if not.
	 */
	bool check_note_is_there(std::vector<Note> &notes, int noteId)
	{
		// remember, a noteId is not 0 indexed...
		if (noteId <= 0 || noteId > notes.size())
		{
			std::cout << "That # isn't here!!! (^_&)" << std::endl;
			return false;
		}
		return true;
	}


	/**
	 * Checks if string is a valid int.
	 * If it is, returns the int.
	 * Otherwise returns 0.
	 * This doesn't check null / empty strings.
	 */
	int parseNoteId(std::string id)
	{
		// TODO: this errors on non-numbers...
		size_t nonIntChar = 0;
		int noteId = 0;
		noteId = std::stoi(id, &nonIntChar);
		return noteId;
	}

	/**
	 * Loops note vector and overwrites collection with all notes in memory.
	 */
	bool save_all_notes(std::string path, std::vector<Note> &notes)
	{
		std::ofstream outf(path);
		if (!outf) {
			std::cerr << "Oh nooo!!! Could not open collection note file!!! :(" << std::endl;
			return false;
		}
		bool res = true;
		for (const auto &iter : notes) {
			if (!iter.save(outf)) {
				res = false;
				break;
			}
		}
		outf.close();
		return res;
	}


	/**
	 * Removes a note from a collection.
	 */
	bool remove_note(std::string path, std::vector<Note> &notes, size_t index)
	{
		if (index > 0 && index <= notes.size())
		{
			notes.erase(notes.begin() + index);
			return save_all_notes(path, notes);
		}
		else
		{
			std::cout << "That # isn't here!!! (^_&)" << std::endl;
			return false;
		}
	}
}
