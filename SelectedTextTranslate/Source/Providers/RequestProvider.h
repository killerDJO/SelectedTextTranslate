#pragma once
#include "Logging\Logger.h"

class RequestProvider
{
private:
    Logger* logger;

public:
    RequestProvider(Logger* logger);
    ~RequestProvider();

    wstring GetStringResponse(wstring url);
    vector<unsigned char> GetResponse(wstring url);
    wstring EscapeText(wstring text) const;
};