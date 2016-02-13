#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <strsafe.h>
#include <tchar.h>
#include <windows.h>
#include "editor.h"
#include "note.h"

/**
 * Launch an editor.
 * Use config to determine editor.
 * Create tmp file.
 * Try to launch it in editor.
 * On success, read the file back in to the note and save.
 */
bool launch_editor(std::wstring path, note::Note &note) {
	
	// --- security attributes ---
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// TODO: create tmp file (path) with note's text
	if (!create_tmp_file(path, note))
	{
		std::cout << "Unable to create tmp file" << std::endl;
		return false;
	}

	// --- create the child process ---
	PROCESS_INFORMATION piProcInfo;
	create_child_process(piProcInfo, path);

	// --- loop until process finishes ---
	DWORD exit_code;
	do {
		GetExitCodeProcess(piProcInfo.hProcess, &exit_code);
		Sleep(200);
	} while (exit_code == STILL_ACTIVE);
		
	// then we can close handles and resume parent execution
	CloseHandle(piProcInfo.hProcess);
	CloseHandle(piProcInfo.hThread);
	
	// TODO:
	// The remaining open handles are cleaned up when this process terminates. 
	// To avoid resource leaks in a larger application, close handles explicitly. 

	// TODO: re-read into note

	return true;
}

void create_child_process(PROCESS_INFORMATION &piProcInfo, std::wstring path)
{
	// prep process args
	path = L"vim " + path; //TEXT("C:\\Users\\Alexander\\AppData\\Local\\atom\\app-1.5.0\\atom.exe");

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

bool create_tmp_file(std::wstring path, note::Note n)
{
	// create / overwrite a tmp file
	std::ofstream outf(path);
	if (!outf)
		return false;
	outf << n.get_text();
	outf.close();
	return true;
}


void error_exit(PTSTR lpszFunction)

// Format a readable error message, print, and exit.
{
	LPVOID lpMsgBuf;
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

	std::cout << lpMsgBuf << std::endl;
	
	LocalFree(lpMsgBuf);
	ExitProcess(1);
}