#pragma once
#include "Services\Translation\Dto\TranslateResult.h"

class TranslateResultViewModel
{
private:
    TranslateResult translateResult;
    map<int, bool> translateResultCategoryExpandedMap;

public:
    TranslateResultViewModel();
    TranslateResultViewModel(TranslateResult translateResult);

    TranslateResult GetTranslateResult() const;

    bool IsExpanded(int categoryIndex);
    void ToggleCategory(int categoryIndex);
};