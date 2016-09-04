#pragma once
#include "Helpers\StringUtilities.h"
#include "Loggers\Logger.h"

class RequestProvider
{
private:
    Logger* logger;

    void LogRequestException(wstring url, exception exception);
    void LogRequestError(wstring url, wstring message);

public:
    RequestProvider(Logger* logger);
    ~RequestProvider();

    wstring GetStringResponse(wstring url);
    vector<unsigned char> GetResponse(wstring url);
    wstring EscapeText(wstring text);
};