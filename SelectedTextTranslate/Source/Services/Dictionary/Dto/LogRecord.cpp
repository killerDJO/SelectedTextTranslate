#include "Services\Dictionary\Dto\LogRecord.h"

LogRecord::LogRecord(wstring word, bool isForcedTranslation, int count, wstring json, time_t createdDate, time_t updatedDate, bool isActive)
{
    this->Word = word;
    this->Count = count;
    this->Json = json;
    this->IsForcedTranslation = isForcedTranslation;
    this->CreatedDate = createdDate;
    this->UpdatedDate = updatedDate;
    this->IsActive = isActive;
}

LogRecord::LogRecord()
{
    this->Word = wstring();
    this->Count = 0;
    this->Json = wstring();
    this->IsForcedTranslation = 0;
    this->CreatedDate = 0;
    this->UpdatedDate = 0;
    this->IsActive = 0;
}

LogRecord::~LogRecord()
{
}