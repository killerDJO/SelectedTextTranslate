#pragma once
#include "PrecompiledHeaders\stdafx.h"

class Logger 
{
public:
	static void Log(string record);

private:
	static string GetCurrentDateTime();
	static string GetCurrentDate();
	static wstring GetLocalComputerName();
	static wstring GetLogFileName();
};