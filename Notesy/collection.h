#pragma once
#include <ctime>
#include <iostream>
#include <cassert>
#include <string>

// declarations
void print_header();


/**
 * Main class for Collection objects. Should ultimately
 * contain printing and serialization / deserialization methods?
 */
class Collection
{
	std::string m_name;
	time_t m_date_created;
	time_t m_date_modified;

public:
	// --- constructors ---
	Collection();
	Collection(std::string name);

	// --- accessors ------------------
	std::string get_name() { return m_name; }
	void set_name(std::string new_name) { m_name = new_name; }
	time_t get_date_created() { return m_date_created; }
	void set_date_created(time_t date) { m_date_created = date; }
	time_t get_date_modified(time_t date) { return m_date_modified; }
	void set_date_modified(time_t date) { m_date_modified = date; }
	
	// --- methods ---
	void pretty_print();
	bool save(std::string path);

	// friend overloads of i/o operators for serialization
	friend std::ostream& operator<< (std::ostream &out, const Collection &c);
	friend std::istream& operator>> (std::istream &in, Collection &c);

	// destructor
	~Collection();
	
	// TODO:
	// list topics
	// add topics
	// remove topics
	// delete self
	// rename
};
