#pragma once
#include <ctime>
#include <ostream>
#include <cassert>

// for now, store a list somewhere
// worry about referencing topics later...

class Collection
{
	char *m_name;
	time_t m_date_created;
	time_t m_date_modified;

public:
	Collection(char *name) {
		assert(("Collection must have name!", name != "" && name != nullptr));
		// TODO: check for bad chars?
		m_name = name;
		time(&m_date_created);
		time(&m_date_modified);
	}
	// --- accessors ---
	char *get_name() { return m_name; }
	void set_name(char* new_name) { m_name = new_name; }
	time_t get_date_created() { return m_date_created; }
	void set_date_created(time_t date) { m_date_created = date; }
	time_t get_date_modified(time_t date) { return m_date_modified; }
	void set_date_modified(time_t date) { m_date_modified = date; }
	// -----------------

	// friend overload of << operator, returns ostream for chaining printing
	friend std::ostream& operator<< (std::ostream &out, const Collection &c);
	
	// TODO:
	// list topics
	// add topics
	// remove topics
	// delete self
	// rename
};
