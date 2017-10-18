#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Providers\RequestProvider.h"

class TranslatePageParser
{
private:
    RequestProvider* requestProvider;
    Logger* logger;

    long long tkk1;
    long long tkk2;
    time_t lastTkkRequestTime;

    void UpateTkkIfNeccessary();
    string SearchScriptTag(GumboNode* node) const;

public:
    TranslatePageParser(CompositionRoot* root);

    long long GetTkk1();
    long long GetTkk2();
};