#include "BusinessLogic\Dictionary\Dto\DictionaryRecord.h"

DictionaryRecord::DictionaryRecord(wstring word, bool isForcedTranslation, int count, wstring json, time_t createdDate, time_t updatedDate, bool isActive)
{
    this->word = word;
    this->count = count;
    this->json = json;
    this->isForcedTranslation = isForcedTranslation;
    this->createdDate = createdDate;
    this->updatedDate = updatedDate;
    this->isActive = isActive;
}

DictionaryRecord::DictionaryRecord()
{
    this->word = wstring();
    this->count = 0;
    this->json = wstring();
    this->isForcedTranslation = false;
    this->createdDate = 0;
    this->updatedDate = 0;
    this->isActive = false;
}

wstring DictionaryRecord::GetWord() const
{
    return word;
}

bool DictionaryRecord::IsForcedTranslation() const
{
    return isForcedTranslation;
}

wstring DictionaryRecord::GetJson() const
{
    return json;
}

int DictionaryRecord::GetCount() const
{
    return count;
}

time_t DictionaryRecord::GetCreatedDate() const
{
    return createdDate;
}

time_t DictionaryRecord::GetUpdatedDate() const
{
    return updatedDate;
}

bool DictionaryRecord::IsActive() const
{
    return isActive;
}