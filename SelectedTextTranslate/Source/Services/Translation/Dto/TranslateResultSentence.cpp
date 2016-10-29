#include "Services\Translation\Dto\TranslateResultSentence.h"

TranslateResultSentence::TranslateResultSentence(wstring translation, wstring origin, wstring input, wstring suggestion)
{
    this->translation = translation;
    this->origin = origin;
    this->input = input;
    this->suggestion = suggestion;
}

TranslateResultSentence::TranslateResultSentence()
{
    this->translation = wstring();
    this->origin = wstring();
    this->input = wstring();
    this->suggestion = wstring();
}

wstring TranslateResultSentence::GetTranslation() const
{
    return translation;
}

wstring TranslateResultSentence::GetOrigin() const
{
    return origin;
}

wstring TranslateResultSentence::GetInput() const
{
    return input;
}

wstring TranslateResultSentence::GetSuggestion() const
{
    return suggestion;
}

TranslateResultSentence::~TranslateResultSentence()
{
}
