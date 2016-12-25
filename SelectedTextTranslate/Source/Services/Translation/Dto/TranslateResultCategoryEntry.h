#pragma once

class TranslateResultCategoryEntry
{
private:
    wstring word;
    vector<wstring> reverseTranslations;
    double score;

public:
    TranslateResultCategoryEntry(wstring word, vector<wstring> reverseTranslations, double score);
    TranslateResultCategoryEntry();

    wstring GetWord() const;
    vector<wstring> GetReverseTranslations() const;
    double GetScore() const;
};
