#pragma once
#include "Services\Translation\Dto\TranslateResult.h"

class TranslateResultContent
{
private:
    TranslateResult translateResult;
    map<int, bool> translateResultCategoryExpandedMap;

public:
    TranslateResultContent();
    TranslateResultContent(TranslateResult translateResult);

    TranslateResult GetTranslateResult() const;

    bool IsExpanded(int categoryIndex);
    void ToggleCategory(int categoryIndex);
};