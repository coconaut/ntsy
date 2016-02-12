#include "stdafx.h"
#include <iostream>
#include <strsafe.h>
#include <tchar.h>
#include <windows.h>
#include "editor.h"

#define BUFSIZE 4096 

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

HANDLE g_hInputFile = NULL;

void CreateChildProcess(PROCESS_INFORMATION &piProcInfo);
void WriteToPipe(std::string *text);
void ReadFromPipe();
void ErrorExit(PTSTR);


int launch_editor() {

	// overcomplicating this!!! just need to write note to temp file, then open it in
	// vim/editor and wait for quit
	// this is what git does!!! 
	// even if you pipe into vim, can't pipe out!
	// unless you make your own editor...
	// then can reset tmp message / note


	SECURITY_ATTRIBUTES saAttr;

	printf("\n->Start of parent execution.\n");

	// Set the bInheritHandle flag so pipe handles are inherited. 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT. 
	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
		ErrorExit(TEXT("StdoutRd CreatePipe"));

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
		ErrorExit(TEXT("Stdout SetHandleInformation"));

	// Create a pipe for the child process's STDIN. 
	if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
		ErrorExit(TEXT("Stdin CreatePipe"));

	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
		ErrorExit(TEXT("Stdin SetHandleInformation"));

	// Create the child process. 
	PROCESS_INFORMATION piProcInfo;
	CreateChildProcess(piProcInfo);

	
	// Write to the pipe that is the standard input for a child process. 
	// Data is written to the pipe's buffers, so it is not necessary to wait
	// until the child process is running before writing data.
	std::string text = "Hello, my name is Bingo.";
	WriteToPipe(&text);
	std::cout << L"\n->Contents of text written to child STDIN pipe.\n";

	// TODO: read...

	// Close handles to the child process and its primary thread.
	// Some applications might keep these handles to monitor the status
	// of the child process, for example. 

	// may want to read process info here, wait for it to finish!!!
	DWORD exit_code;
	do {
		GetExitCodeProcess(piProcInfo.hProcess, &exit_code);
	} while (exit_code == STILL_ACTIVE);
		
	// then we can close handles and resume parent execution
	CloseHandle(piProcInfo.hProcess);
	CloseHandle(piProcInfo.hThread);
	
	// Read from pipe that is the standard output for child process. 
	//printf("\n->Contents of child process STDOUT:\n\n");
	//ReadFromPipe();

	printf("\n->End of parent execution.\n");

	// The remaining open handles are cleaned up when this process terminates. 
	// To avoid resource leaks in a larger application, close handles explicitly. 

	return 0;
}

void CreateChildProcess(PROCESS_INFORMATION &piProcInfo)
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{

	// TODO: deal with signals...
	
	//TCHAR szCmdline[] = TEXT("C:\\Users\\Alexander\\AppData\\Local\\atom\\app-1.5.0\\atom.exe");
	TCHAR szCmdline[] = TEXT("vim - -e");
	//PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;


	// Set up members of the PROCESS_INFORMATION structure. 
	// ZeroMemory fills a block of mem with 0s...
	// it will get info after creating process
	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));


	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO); // size of the struct in bytes
	siStartInfo.hStdError = g_hChildStd_OUT_Wr; 
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 

	bSuccess = CreateProcess(NULL, // application name!!! (use path), must include ext.
		szCmdline,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

					   // If an error occurs, exit the application. 
	if (!bSuccess)
		ErrorExit(TEXT("CreateProcess"));
	/*else
	{
		
	}*/
}

void WriteToPipe(std::string *text)
{
	DWORD num_bytes_to_write, num_bytes_written;
	num_bytes_to_write = text->size();
	//CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;

	// write
	bSuccess = WriteFile(g_hChildStd_IN_Wr, text, num_bytes_to_write, &num_bytes_written, NULL);
	
	// Close the pipe handle so the child process stops reading. 
	if (!CloseHandle(g_hChildStd_IN_Wr))
		ErrorExit(TEXT("StdInWr CloseHandle"));
}

void ReadFromPipe(void)

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{
	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	for (;;)
	{
		bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0) break;

		bSuccess = WriteFile(hParentStdOut, chBuf,
			dwRead, &dwWritten, NULL);
		if (!bSuccess) break;
	}
}

void ErrorExit(PTSTR lpszFunction)

// Format a readable error message, display a message box, 
// and exit from the application.
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

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40)*sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(1);
}