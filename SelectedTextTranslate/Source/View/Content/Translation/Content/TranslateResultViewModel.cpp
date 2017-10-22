#include "View\Content\Translation\Content\TranslateResultViewModel.h"

TranslateResultViewModel::TranslateResultViewModel()
{
}

TranslateResultViewModel::TranslateResultViewModel(TranslateResult translateResult)
{
    this->translateResult = translateResult;

    for (size_t i = 0; i < translateResult.GetTranslateCategories().size(); ++i)
    {
        translateResultCategoryExpandedMap[i] = false;
    }
}

TranslateResult TranslateResultViewModel::GetTranslateResult() const
{
    return translateResult;
}

bool TranslateResultViewModel::IsExpanded(int categoryIndex)
{
    return translateResultCategoryExpandedMap[categoryIndex];
}

void TranslateResultViewModel::ToggleCategory(int categoryIndex)
{
    translateResultCategoryExpandedMap[categoryIndex] ^= true;
}