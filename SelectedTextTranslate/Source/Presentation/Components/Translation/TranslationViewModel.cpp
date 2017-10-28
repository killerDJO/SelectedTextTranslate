#include "Presentation\Components\Translation\TranslationViewModel.h"

TranslationViewModel::TranslationViewModel(TranslateResult translateResult)
{
    this->translateResult = translateResult;

    for (size_t i = 0; i < translateResult.GetTranslateCategories().size(); ++i)
    {
        translateResultCategoryExpandedMap[i] = false;
    }
}

TranslateResult TranslationViewModel::GetTranslateResult() const
{
    return translateResult;
}

bool TranslationViewModel::IsExpanded(int categoryIndex)
{
    return translateResultCategoryExpandedMap[categoryIndex];
}

void TranslationViewModel::ToggleCategory(int categoryIndex)
{
    translateResultCategoryExpandedMap[categoryIndex] ^= true;
}