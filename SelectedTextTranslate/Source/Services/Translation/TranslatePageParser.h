#pragma once
#include "Providers\RequestProvider.h"
#include "Infrastructure\Logging\Logger.h"

class TranslatePageParser
{
private:
    RequestProvider* requestProvider;
    Logger* logger;

    long long tkk1;
    long long tkk2;
    time_t lastTkkRequestTime;

    void UpateTkkIfNeccessary();
    vector<string> Split(const string &s, char delim) const;
    string SearchScriptTag(GumboNode* node) const;

public:
    TranslatePageParser(Logger* logger, RequestProvider* requestProvider);
    ~TranslatePageParser();

    long long GetTkk1();
    long long GetTkk2();
};