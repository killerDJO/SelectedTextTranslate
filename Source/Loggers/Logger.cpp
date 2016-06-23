#include "PrecompiledHeaders\stdafx.h"
#include "Loggers\Logger.h"
#include <chrono>
#include <iomanip>

const wchar_t* Logger::logFileName = L"logs\\trace_log.txt";

void Logger::Log(string record)
{
	HANDLE hFile = CreateFile(logFileName, FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD error = GetLastError();
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	record = GetCurrentDateTime() + " " + record + "\r\n";

	DWORD nWritten;
	const char* buffer = record.c_str();
	WriteFile(hFile, buffer, strlen(buffer), &nWritten, NULL);

	CloseHandle(hFile);
}

string Logger::GetCurrentDateTime()
{
	char buffer[30];

	auto now = chrono::system_clock::now();
	time_t time = chrono::system_clock::to_time_t(now);
	auto ms =
		chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) -
		chrono::duration_cast<chrono::seconds>(now.time_since_epoch());

	strftime(buffer, 30, "%d-%m-%Y %I:%M:%S", localtime(&time));

	stringstream ss;
	ss << setw(3) << setfill('0') << ms.count();

	string dateTime = string(buffer) + "." + ss.str();

	return dateTime;
}