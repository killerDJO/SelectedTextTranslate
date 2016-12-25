#pragma once

class TranslateResultSentence
{
private:
    wstring translation;
    wstring origin;
    wstring input;
    wstring suggestion;

public:
    TranslateResultSentence(wstring translation, wstring origin, wstring input, wstring suggestion);
    TranslateResultSentence();

    wstring GetTranslation() const;
    wstring GetOrigin() const;
    wstring GetInput() const;
    wstring GetSuggestion() const;
};