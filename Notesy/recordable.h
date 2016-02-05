#pragma once
#include <ctime>
#include <iostream>
#include <fstream>

class Recordable
{
protected:
	time_t m_date_created;
	time_t m_date_modified;
public:
	Recordable();
	
	// accessors -----------------------------------
	time_t get_date_created() { return m_date_created; }
	void set_date_created(time_t date) { m_date_created = date; }
	time_t get_date_modified() { return m_date_modified; }
	void set_date_modified(time_t date) { m_date_modified = date; }

	// virtual methods
	virtual void pretty_print() const = 0;
	bool save(std::string path) const;
	bool save(std::ofstream &outf) const;
	virtual std::ostream& serialize(std::ostream &out) const = 0;
	virtual std::istream& deserialize(std::istream &in) = 0;
};

// stream op overloads for serialization

