#pragma once
#include <windows.h>
#include "note.h"
#include "config.h"

bool launch_editor(note::Note &note, NtsyConfig *config);
void create_child_process(PROCESS_INFORMATION &piProcInfo, std::wstring path, NtsyConfig *config);
void error_exit(PTSTR);
bool write_tmp_file(std::wstring path, note::Note &note);
bool read_tmp_file(std::wstring path, note::Note &note);