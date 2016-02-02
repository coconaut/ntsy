#include "stdafx.h"
#include <cassert>
#include <string>
#include "note.h"

namespace note {

	/**
	 * Note constructor.
	 */
	Note::Note(std::string text, int Id) {
		assert(("Id must be greater than 0", Id > 0));
		assert(("Note text cannot be empty", !text.empty() && text.compare("") != 0));
		m_Id = Id;
		m_text = text;
	}



}
