#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Helpers\Utilities.h"

class RequestHelper
{
public:
	static string GetResponse(string url);
	static string EscapeText(string text);
private:
	static int Writer(void *data, size_t size, size_t nmemb, string *buffer_in);
};