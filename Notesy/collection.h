//#pragma once
//#include <ctime>
//#include <fstream>
//#include <objbase.h>
//
//// TODO: add times... 
//// std::time_t m_date_created;
//// std::time_t m_last_modified;
//// probably store as header and pointers to topics
//// this might be way overboard... could just use dirs, topics as files...
//// might be fun though...
//
//class Collection
//{
//	char *m_name;
//	GUID m_id;
//	std::fstream *file;
//
//public:
//	Collection(char *name)
//	{
//		// TODO: assert not null or empty name
//		m_name = name;
//		CoCreateGuid(&m_id);
//	}
//	char *get_name() { return m_name; }
//	char *set_name(char* new_name) { m_name = new_name; }
//	GUID get_id() { return m_id; }
//	//void save();
//	// list topics
//	// add topics
//	// remove topics
//	// delete self
//	// rename
//	~Collection() 
//	{
//		// TODO: destroy the fstream...
//		// remember, if collections are directories, there won't be a file stream, save that for next level (topics)
//	}
//};
