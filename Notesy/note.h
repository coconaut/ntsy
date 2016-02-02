#pragma once
#include <string>

namespace note {

	class Note {
		int m_Id;
		std::string m_text;
	public:;
		   Note(std::string text, int Id);
		   int get_Id() { return m_Id; }
		   void set_Id(int Id) { m_Id = Id; }
		   std::string get_text() { return m_text; }
		   void set_text(std::string text) { m_text = text; }
	};
}