#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <strsafe.h>
#include <tchar.h>
#include <windows.h>
#include "editor.h"
#include "note.h"
#include "config.h"
#include "text.h"

/**
 * Launch an editor.
 * Use config to determine editor.
 * Create tmp file.
 * Try to launch it in editor.
 * On success, read the file back in to the note and save.
 */
bool launch_editor(note::Note &note, NtsyConfig *config) {
	// TODO: we should really pass in config and create
	// this is the ntsy dir, not the current dir, especially
	// for write permissions...

	std::wstring root = txt::convert_to_wide(config->get_ntsy_root());
	std::wstring tmp_path = root + L"__tmp.ntsy";

	// --- security attributes ---
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// --- create tmp file with note's text ---
	if (!write_tmp_file(tmp_path, note))
	{
		std::cout << "Unable to create tmp file" << std::endl;
		return false;
	}

	// --- create the child process ---
	PROCESS_INFORMATION piProcInfo;
	create_child_process(piProcInfo, tmp_path, config);

	// --- loop until process finishes ---
	DWORD exit_code;
	do {
		GetExitCodeProcess(piProcInfo.hProcess, &exit_code);
		Sleep(200);
	} while (exit_code == STILL_ACTIVE);
		
	// then we can close handles and resume parent execution
	CloseHandle(piProcInfo.hProcess);
	CloseHandle(piProcInfo.hThread);

	// re-read into note
	bool success = read_tmp_file(tmp_path, note);

	// update modified
	if (success)
	{
		time_t t;
		time(&t);
		note.set_date_modified(t);
	}

	// delete tmp
	_wremove(&tmp_path[0]);

	return success;
}

void create_child_process(PROCESS_INFORMATION &piProcInfo, std::wstring path, NtsyConfig *config)
{
	// prep process args
	std::wstring editor = txt::convert_to_wide(config->get_editor());
	path = editor + L" " + path;

	// zero up the process info block
	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
	
	// Set up members of the STARTUPINFO structure (not passing on stdin or stdout)
	STARTUPINFO siStartInfo;
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	
	// Create the child process. 
	bool bSuccess = false;
	bSuccess = CreateProcess(NULL, // application name!!! (use path), must include ext.
		&path[0],     // command line (need if no application name)
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		FALSE,         // handles are inherited
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	// check result
	if (!bSuccess)
		error_exit(TEXT("CreateProcess"));
}

bool write_tmp_file(std::wstring path, note::Note &note)
{
	// create / overwrite a tmp file
	std::ofstream outf(path);
	if (!outf)
		return false;
	outf << note.get_text();
	outf.close();
	return true;
}

bool read_tmp_file(std::wstring path, note::Note &note)
{
	std::ifstream inf(path);
	if (!inf)
		return false;
	std::string buffer;
	std::string tmp;
	while (std::getline(inf, tmp))
	{
		buffer += tmp + "\n";
	}

	note.set_text(buffer);
	return true;
}


void error_exit(PTSTR lpszFunction)

// Format a readable error message, print, and exit.
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40)*sizeof(TCHAR));

	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);

	std::wcout << (LPCTSTR)lpDisplayBuf << std::endl;
	
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(1);
}