#include "Entities\TranslateResult.h"

TranslateResult::TranslateResult()
{
    Sentence.Origin = new wchar_t[0];
    Sentence.Translation = new wchar_t[0];
    Sentence.Translit = new wchar_t[0];
    Sentence.Input = new wchar_t[0];
    ErrorMessage = new wchar_t[0];
    isErrorResult = false;
}

bool TranslateResult::IsInputCorrected()
{
    int compareResult = wcscmp(Sentence.Origin, Sentence.Input);
    return !isErrorResult && compareResult != 0;
}

bool TranslateResult::IsErrorResult()
{
    return isErrorResult;
}

void TranslateResult::SetError(wstring errorMessage)
{
    ErrorMessage = StringUtilities::CopyWideChar(errorMessage);
}

void TranslateResult::Free()
{
    delete[] Sentence.Origin;
    delete[] Sentence.Translation;
    delete[] Sentence.Translit;
    delete[] Sentence.Input;
    delete[] ErrorMessage;

    for (size_t i = 0; i < TranslateCategories.size(); ++i)
    {
        auto category = TranslateCategories[i];
        delete[] category.BaseForm;
        delete[] category.PartOfSpeech;
        for (size_t j = 0; j < category.Entries.size(); ++j)
        {
            auto entry = category.Entries[j];
            delete[] entry.Word;
            for (size_t k = 0; k < entry.ReverseTranslation.size(); ++k)
            {
                delete[] entry.ReverseTranslation[k];
            }
        }
    }
}