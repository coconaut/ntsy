#pragma once

const int TIMESIZE = 26;
const int DATECOLSIZE = 20;
const int MAINCOLSIZE = 35;
const int ABBR_COLSIZE = 10;

void remove_char(std::string &str, char c);
void remove_whitespace(std::string &str);
void remove_newlines(std::string &str);
void trim_char(std::string &str);
void to_all_upper(std::string &str);
std::string format_time(const time_t *time);