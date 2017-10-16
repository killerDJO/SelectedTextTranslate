#include "View\Content\Translation\Content\Dto\TranslateResultContent.h"

TranslateResultContent::TranslateResultContent()
{
}

TranslateResultContent::TranslateResultContent(TranslateResult translateResult)
{
    this->translateResult = translateResult;

    for (size_t i = 0; i < translateResult.GetTranslateCategories().size(); ++i)
    {
        translateResultCategoryExpandedMap[i] = false;
    }
}

TranslateResult TranslateResultContent::GetTranslateResult() const
{
    return translateResult;
}

bool TranslateResultContent::IsExpanded(int categoryIndex)
{
    return translateResultCategoryExpandedMap[categoryIndex];
}

void TranslateResultContent::ToggleCategory(int categoryIndex)
{
    translateResultCategoryExpandedMap[categoryIndex] ^= true;
}