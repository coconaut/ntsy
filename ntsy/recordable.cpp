#include "stdafx.h"
#include "recordable.h"
#include <ctime>


/**
 * Auditable constructor: just initializes time fields.
 */
Recordable::Recordable() {
	time(&m_date_created);
	time(&m_date_modified);
}


/**
* Appends a single recordable to notesy file.
*/
bool Recordable::save(std::string path) const
{
	std::ofstream outf(path, std::ios::app);
	if (!outf) {
		std::cerr << "Could not open notesy file!!!" << std::endl;
		return false;
	}
	serialize(outf);
	return true;
}


/**
* Appends a single recordable to a file.
* This overload expects an already open filestream.
* Intended for looping.
*/
bool Recordable::save(std::ofstream &outf) const
{
	if (!outf) {
		std::cerr << "Could not write to notesy file!!!" << std::endl;
		return false;
	}
	serialize(outf);
	return true;
}
