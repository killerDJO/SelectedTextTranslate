#pragma once
#include "BusinessLogic\Translation\Dto\TranslateResult.h"

class TranslationViewModel
{
private:
    TranslateResult translateResult;
    map<int, bool> translateResultCategoryExpandedMap;

public:
    TranslationViewModel(TranslateResult translateResult);

    TranslateResult GetTranslateResult() const;

    bool IsExpanded(int categoryIndex);
    void ToggleCategory(int categoryIndex);
};