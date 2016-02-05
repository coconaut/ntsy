#pragma once
#include <ctime>
#include <string>
#include "recordable.h"

namespace note {

	class Note : public Recordable 
	{
		std::string m_text;
	public:
		 // constructors -----------------------
		 Note() {}
		 Note(std::string text) : m_text(text) {}

		 // accessors -----------------------------
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
	bool add_note(std::string text, std::string path);
	
}