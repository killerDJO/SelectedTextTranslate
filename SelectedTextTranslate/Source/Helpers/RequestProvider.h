#pragma once
#include "Loggers\Logger.h"

class RequestProvider
{
private:
    Logger* logger;

    void LogRequestException(wstring url, exception exception) const;
    void LogRequestError(wstring url, wstring message) const;

public:
    RequestProvider(Logger* logger);
    ~RequestProvider();

    wstring GetStringResponse(wstring url);
    vector<unsigned char> GetResponse(wstring url);
    wstring EscapeText(wstring text) const;
};