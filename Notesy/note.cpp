#include "stdafx.h"
#include <ctime>
#include <string>
#include <vector>
#include "note.h"

namespace note {

	void pretty_print() {
		
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
}
