#include "stdafx.h"
#include <algorithm>
#include <ctime>
#include <string>
#include "text.h"


void remove_char(std::string &str, char c)
{
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
}

void remove_whitespace(std::string &str)
{
	remove_char(str, '\n');
	remove_char(str, '\t');
	remove_char(str, ' ');
}

void replace_char(std::string &str, char old, char replacer)
{
	std::replace(str.begin(), str.end(), old, replacer);
}

void remove_newlines(std::string &str)
{
	remove_char(str, '\n');
}

void trim_char(std::string &str) {
	// TODO:
}

void trim_newlines(std::string &str) {
	// TODO:
}

void to_all_upper(std::string &str) {
	std::transform(str.begin(), str.end(), str.begin(), toupper);
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