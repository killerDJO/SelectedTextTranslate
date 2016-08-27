#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Helpers\Utilities.h"
#include "Loggers\Logger.h"

class RequestHelper
{
public:
	static wstring GetStringResponse(wstring url);
	static vector<unsigned char> GetResponse(wstring url);
	static wstring EscapeText(wstring text);
private:
	static void LogRequestException(wstring url, exception exception);
	static void LogRequestError(wstring url, wstring message);
};