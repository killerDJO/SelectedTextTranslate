#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "DataAccess\RequestProvider.h"
#include "BusinessLogic\Translation\Dto\TranslateResult.h"
#include "BusinessLogic\Translation\TranslatePageParser.h"
#include "BusinessLogic\Dictionary\DictionaryService.h"

using namespace nlohmann;

class TranslationService
{
private:
    const long long DictionaryRefreshInterval = 60 * 60 * 24 * 365; // 1 year

    RequestProvider* requestProvider;
    TranslatePageParser* translatePageParser;
    Logger* logger;
    DictionaryService* dictionaryService;

    TranslateResult ParseJSONResponse(wstring jsonResponse, wstring input) const;
    TranslateResultSentence ParseTranslateResultSentence(json root, wstring input) const;
    vector<TranslateResultCategory> ParseTranslateCategories(json root) const;

    TranslateResult GetTranslatorResponse(wstring sentence, bool incrementTranslationsCount, bool forceTranslation) const;
    TranslateResult SendRequestAndParse(wstring sentence, bool forceTranslation, function<void(wstring, wstring, bool)> dictionaryAction) const;
    wstring SendTranslationRequest(wstring sentence, bool forceTranslation) const;

public:
    TranslationService(CompositionRoot* root);

    TranslateResult TranslateSentence(wstring sentence, bool incrementTranslationsCount, bool forceTranslation) const;
    wstring GetHash(wstring sentence) const;
};