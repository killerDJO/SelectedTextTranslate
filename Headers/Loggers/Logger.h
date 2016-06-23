#pragma once
#include "PrecompiledHeaders\stdafx.h"

class Logger 
{
public:
	static void Log(string record);

private:
	static const wchar_t* logFileName;
	static string GetCurrentDateTime();
};