#pragma once
#include <windows.h>
#include "note.h"


bool launch_editor(std::wstring path, note::Note &note);
void create_child_process(PROCESS_INFORMATION &piProcInfo, std::wstring path);
void error_exit(PTSTR);
bool create_tmp_file(std::wstring path, note::Note n);