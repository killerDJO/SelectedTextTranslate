#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Infrastructure\CompositionRoot.h"

class RequestProvider
{
private:
    Logger* logger;

    const int BufferSize = 100000;
    unsigned char* buffer;

    void AssertStatusCode(HINTERNET request) const;
    vector<unsigned char> GetData(HINTERNET request) const;

public:
    RequestProvider(CompositionRoot* root);

    wstring GetStringResponse(const wstring host, const wstring url) const;
    vector<unsigned char> GetResponse(const wstring host, const wstring url) const;
    wstring EscapeText(wstring text) const;
};
