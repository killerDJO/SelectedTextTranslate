#pragma once
#include "Infrastructure\Logging\Logger.h"

class RequestProvider
{
private:
    Logger* logger;

public:
    RequestProvider(Logger* logger);

    wstring GetStringResponse(wstring url);
    vector<unsigned char> GetResponse(wstring url);
    wstring EscapeText(wstring text) const;
};