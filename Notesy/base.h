#pragma once
#include <ctime>

class Base
{
protected:
	time_t m_date_created;
	time_t m_date_modified;
public:
	Base();
	
	// accessors -----------------------------------
	time_t get_date_created() { return m_date_created; }
	void set_date_created(time_t date) { m_date_created = date; }
	time_t get_date_modified() { return m_date_modified; }
	void set_date_modified(time_t date) { m_date_modified = date; }

	// virtual methods
	virtual void pretty_print() const = 0;
	virtual bool save(std::string path) const = 0;
	virtual bool save(std::ofstream &outf) const = 0;
};
