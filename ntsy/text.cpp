#include "stdafx.h"
#include <ctime>
#include <string>
#include "text.h"

namespace txt {

	const char *NTSY_EXT = ".ntsy";

	void remove_whitespace(std::string &str)
	{
		remove_char(str, '\n');
		remove_char(str, '\t');
		remove_char(str, ' ');
	}

	void remove_newlines(std::string &str)
	{
		remove_char(str, '\n');
	}

	std::string format_time(const time_t *time) {
		char buf[TIMESIZE];
		struct tm *timeinfo = new tm();
		localtime_s(timeinfo, time);
		strftime(buf, TIMESIZE, "%D %I:%M", timeinfo);
		//ctime_s(buf, TIMESIZE, time);
		//return trim_date(buf);
		delete timeinfo;
		return std::string(buf);
	}

	/**
	* Forms a ntsy file name from abbr.
	*/
	std::string get_ntsy_file_name(std::string &name)
	{
		return name + NTSY_EXT;
	}	


}