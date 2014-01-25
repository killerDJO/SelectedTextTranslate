#pragma once
#include "stdafx.h"

class RequestHelper
{
public:
	static string GetResponse(string url);
	static string EscapeText(string text);
private:
	static int Writer(void *data, size_t size, size_t nmemb, string *buffer_in);
};