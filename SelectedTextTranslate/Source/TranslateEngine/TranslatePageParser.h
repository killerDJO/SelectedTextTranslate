#pragma once
#include "Helpers\RequestProvider.h"
#include "Helpers\StringUtilities.h"
#include "Loggers\Logger.h"

class TranslatePageParser
{
private:
    RequestProvider* requestProvider;
    Logger* logger;

    long long tkk1;
    long long tkk2;
    time_t lastTkkRequestTime;

    void UpateTkkIfNeccessary();	
    vector<string> Split(const string &s, char delim);
    string SearchScriptTag(GumboNode* node);

public:
    TranslatePageParser(Logger* logger, RequestProvider* requestProvider);
    ~TranslatePageParser();

    long long GetTkk1();
    long long GetTkk2();
};