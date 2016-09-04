#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include <chrono>
#include <iomanip>

class Logger 
{
private:
	wstring GetCurrentDateTime();
	wstring GetCurrentDate();
	wstring GetLocalComputerName();
	wstring GetLogFileName();

public:
	Logger();
	~Logger();

	void Log(wstring record);
};