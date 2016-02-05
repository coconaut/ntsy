#pragma once
#include <ctime>
#include <string>
#include "recordable.h"

namespace note {

	class Note : public Recordable 
	{
		int m_tmpId;
		std::string m_text;
	public:
		 // constructors -----------------------
		 Note() { m_tmpId = 0; }
		 Note(std::string text) : m_text(text) { m_tmpId = 0; }

		 // accessors -----------------------------
		 int get_tmpId() { return m_tmpId; }
		 void set_tmpId(int Id) { m_tmpId = Id; }
		 std::string get_text() { return m_text; }
		 void set_text(std::string text) { m_text = text; }
			
		 // methods / overrides ------------------------------
		 virtual void pretty_print() const override;
		 virtual std::ostream& serialize(std::ostream &out) const override;
		 virtual std::istream& deserialize(std::istream &in) override;

		 // friend overloads of i/o operators for serialization
		 friend std::ostream& operator<< (std::ostream &out, const Note &n);
		 friend std::istream& operator>> (std::istream &in, Note &n);
	};

	// declarations --------------
	void print_header();
	std::vector<Note> get_all_notes(std::string path);
	bool add_note(std::string text, std::string path);
	void list_all_notes(std::vector<Note> &notes);
	void list_all_notes(std::string path);
	
}