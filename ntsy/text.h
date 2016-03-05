#pragma once
#include <algorithm>
#include <locale>
#include <codecvt>

namespace txt {

	// --- constants ----------------
	const int TIMESIZE = 26;
	const int DATECOLSIZE = 20;
	const int MAINCOLSIZE = 35;
	const int ABBR_COLSIZE = 10;
	

	void remove_whitespace(std::string &str);
	void remove_newlines(std::string &str);
	std::string format_time(const time_t *time);
	std::string get_ntsy_file_name(std::string &name);

	// --- base string formatters ---
	inline void to_all_upper(std::string &str) {
		std::transform(str.begin(), str.end(), str.begin(), toupper);
	}

	inline void remove_char(std::string &str, char c)
	{
		str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	}

	inline void replace_char(std::string &str, char old, char replacer)
	{
		std::replace(str.begin(), str.end(), old, replacer);
	}

	inline void trim_left(std::string &str) {
		// creates iterator up until the first non-space character, and passes to erase
		str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int c) {return !isspace(c);}));
	}

	inline void trim_right(std::string &str) {
		str.erase(std::find_if(str.rbegin(), str.rend(), [](int c) {return !isspace(c);}).base(), str.end());
	}

	inline void trim(std::string &str) {
		trim_right(str);
		trim_left(str);
	}

	inline bool is_alphanumeric(std::string &str) {
		return std::find_if(str.begin(), str.end(), [](int c) {return !isalnum(c);}) == str.end();
	}

	inline std::wstring convert_to_wide(std::string str) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}

	inline std::string convert_to_narrow(std::wstring wstr)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(wstr);
	}

}