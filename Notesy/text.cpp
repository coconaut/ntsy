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

void remove_newlines(std::string &str)
{
	remove_char(str, '\n');
}

void trim_char(std::string &str) {
	// TODO:
}

void to_all_upper(std::string &str) {
	std::transform(str.begin(), str.end(), str.begin(), toupper);
}

std::string trim_date(char* buf) {
	std::string s(buf);
	return s.substr(0, TIMESIZE - 2);
}

std::string format_time(const time_t *time) {
	char buf[TIMESIZE];
	ctime_s(buf, TIMESIZE, time);
	return trim_date(buf);
}