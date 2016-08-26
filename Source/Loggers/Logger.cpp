#include "PrecompiledHeaders\stdafx.h"
#include "Loggers\Logger.h"
#include <chrono>
#include <iomanip>

void Logger::Log(string record)
{
	HANDLE hFile = CreateFile(GetLogFileName().c_str(), FILE_APPEND_DATA, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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

wstring Logger::GetLogFileName()
{
	string currentDate = GetCurrentDate();
	wstring computerName = GetLocalComputerName();

	wstring fullFileName = L"logs\\" + wstring(currentDate.begin(), currentDate.end()) + L"_" + computerName + L"_trace.txt";

	return fullFileName;
}

wstring Logger::GetLocalComputerName()
{
	wchar_t buffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD bufCharCount = MAX_COMPUTERNAME_LENGTH + 1;

	GetComputerName(buffer, &bufCharCount);

	return wstring(buffer);
}

tm GetTimeInfo() 
{
	auto now = chrono::system_clock::now();
	time_t time = chrono::system_clock::to_time_t(now);

	tm timeinfo;
	localtime_s(&timeinfo, &time);

	return timeinfo;
}

string Logger::GetCurrentDate()
{
	tm timeinfo = GetTimeInfo();

	char buffer[30];
	strftime(buffer, 30, "%d-%m-%Y", &timeinfo);

	string date = string(buffer);

	return date;
}

string Logger::GetCurrentDateTime()
{
	tm timeinfo = GetTimeInfo();

	char buffer[30];
	strftime(buffer, 30, "%d-%m-%Y %I:%M:%S", &timeinfo);

	auto now = chrono::system_clock::now();
	auto ms =
		chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) -
		chrono::duration_cast<chrono::seconds>(now.time_since_epoch());

	stringstream ss;
	ss << setw(3) << setfill('0') << ms.count();

	string dateTime = string(buffer) + "." + ss.str();

	return dateTime;
}