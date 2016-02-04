#pragma once
#include <ctime>
#include <string>
#include "auditable.h"

namespace note {

	class Note : public Auditable 
	{
		std::string m_text;
	public:
		 // constructors -----------------------
		 Note() {}
		 Note(std::string text) : m_text(text) {}

		 // accessors -----------------------------
		 std::string get_text() { return m_text; }
		 void set_text(std::string text) { m_text = text; }

		 // methods --------------------
		 void pretty_print();

		 // friend overloads of i/o operators for serialization
		 friend std::ostream& operator<< (std::ostream &out, const Note &n);
		 friend std::istream& operator>> (std::istream &in, Note &n);
	};
}