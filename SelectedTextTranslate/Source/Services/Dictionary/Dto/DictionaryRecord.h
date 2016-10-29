#pragma once

class DictionaryRecord
{
private:
    wstring word;
    bool isForcedTranslation;
    wstring json;
    int count;
    time_t createdDate;
    time_t updatedDate;
    bool isActive;

public:
    DictionaryRecord(wstring word, bool isForcedTranslation, int count, wstring json, time_t createdDate, time_t updatedDate, bool isActive);
    DictionaryRecord();
    ~DictionaryRecord();

    wstring GetWord() const;
    bool IsForcedTranslation() const;
    wstring GetJson() const;
    int GetCount() const;
    time_t GetCreatedDate() const;
    time_t GetUpdatedDate() const;
    bool IsActive() const;
};