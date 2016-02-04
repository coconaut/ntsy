#pragma once
#include <ctime>
#include <string>
#include "base.h"

namespace note {

	class Note : public Base 
	{
		std::string m_text;
	public:
		 // constructors -----------------------
		 Note() {}
		 Note(std::string text) : m_text(text) {}

		 // accessors -----------------------------
		 std::string get_text() { return m_text; }
		 void set_text(std::string text) { m_text = text; }
			
		 // methods ------------------------------
		 virtual void pretty_print() const;
		 virtual bool save(std::string path) const;
		 virtual bool save(std::ofstream &outf) const;

		 // friend overloads of i/o operators for serialization
		 friend std::ostream& operator<< (std::ostream &out, const Note &n);
		 friend std::istream& operator>> (std::istream &in, Note &n);
	};

	void print_header();
}